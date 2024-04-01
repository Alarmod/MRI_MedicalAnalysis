import cv2, os, sys, shutil, math, contextlib
os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"

import numpy as np
from pathlib import Path

import torch

from ultralytics import YOLO
from ultralytics.models.yolo import segment
from ultralytics.data import build_dataloader
from ultralytics.data.utils import check_det_dataset
from ultralytics.data.dataset import YOLODataset
from ultralytics.utils import ops, TQDM, callbacks, colorstr
from ultralytics.utils.torch_utils import de_parallel, smart_inference_mode
from ultralytics.utils.plotting import Annotator, colors, output_to_target
from ultralytics.nn.autobackend import AutoBackend

def chunks(xs, n):
    n = max(1, n)
    return (xs[i:i+n] for i in range(0, len(xs), n))

# Конфигурация
find_contours_scale=2        # целое число, больше 2 не имеет особого смысла ввиду особенностей реализации кода
global_iou=0.50
global_overlap_mask=False    # полезная функция, позволяет (со значением False) описать контур каждой находимой клетки отдельно, без неё большие расхождения по цифрам
global_single_cls=True
global_save_json=False
global_mask_ratio=1
global_retina_masks=False
global_half=True
global_workers=0             # ставить больше лишь при высоком объеме графической памяти

# использовать только rect=True при обучении, в том числе из-за особенностей текущей версии Yolo!!!
# другие режимы не поддерживаются в настоящий момент

global_visualize_conf_thres=0.01
global_hq_threshold=0.05

brain_and_ischemia_imgsz=512  # без ошибки на крайних версиях Yolo
#brain_and_ischemia_imgsz=320 # с ошибкой на ultralytics от 8.0.206, на 8.1.6 уже исправлено

msk_imgsz=1280

# Based on original Yolo codes
def plot_images_and_export_data(
    images,
    batch_idx,
    cls,
    bboxes=np.zeros(0, dtype=np.float32),
    confs=None,
    masks=np.zeros(0, dtype=np.uint8),
    kpts=np.zeros((0, 51), dtype=np.float32),
    paths=None,
    fname="images.jpg",
    names=None,
    on_plot=None,
    max_subplots=16,
    save=True,
    conf_thres=0.25, 
    show_labels=True
):
    """Plot image grid with labels."""
    if isinstance(images, torch.Tensor):
        images = images.cpu().float().numpy()
    if isinstance(cls, torch.Tensor):
        cls = cls.cpu().numpy()
    if isinstance(bboxes, torch.Tensor):
        bboxes = bboxes.cpu().numpy()
    if isinstance(masks, torch.Tensor):
        masks = masks.cpu().numpy().astype(int)
    if isinstance(kpts, torch.Tensor):
        kpts = kpts.cpu().numpy()
    if isinstance(batch_idx, torch.Tensor):
        batch_idx = batch_idx.cpu().numpy()

    max_size = 1920  # max image size
    bs, _, h, w = images.shape  # batch size, _, height, width
    bs = min(bs, max_subplots)  # limit plot images
    ns = np.ceil(bs**0.5)  # number of subplots (square)
    if np.max(images[0]) <= 1:
        images *= 255  # de-normalise (optional)

    # Build Image
    mosaic = np.full((int(ns * h), int(ns * w), 3), 255, dtype=np.uint8)  # init
    for i in range(bs):
        x, y = int(w * (i // ns)), int(h * (i % ns))  # block origin
        mosaic[y : y + h, x : x + w, :] = images[i].transpose(1, 2, 0)

    # Resize (optional)
    scale = max_size / ns / max(h, w)
    if scale < 1:
        h = math.ceil(scale * h)
        w = math.ceil(scale * w)
        mosaic = cv2.resize(mosaic, tuple(int(x * ns) for x in (w, h)))

    # Annotate
    fs = int((h + w) * ns * 0.01)  # font size
    annotator = Annotator(mosaic, line_width=round(fs / 10), font_size=fs, pil=True, example=names)
    for i in range(bs):
        x, y = int(w * (i // ns)), int(h * (i % ns))  # block origin
        annotator.rectangle([x, y, x + w, y + h], None, (255, 255, 255), width=2)  # borders
        if paths:
            annotator.text((x + 5, y + 5), text=Path(paths[i]).name[:40], txt_color=(220, 220, 220))  # filenames
        if len(cls) > 0:
            idx = batch_idx == i
            classes = cls[idx].astype("int")
            labels = confs is None

            if len(bboxes):
                boxes = bboxes[idx]
                conf = confs[idx] if confs is not None else None  # check for confidence presence (label vs pred)
                is_obb = boxes.shape[-1] == 5  # xywhr
                boxes = ops.xywhr2xyxyxyxy(boxes) if is_obb else ops.xywh2xyxy(boxes)
                if len(boxes):
                    if boxes[:, :4].max() <= 1.1:  # if normalized with tolerance 0.1
                        boxes[..., 0::2] *= w  # scale to pixels
                        boxes[..., 1::2] *= h
                    elif scale < 1:  # absolute coords need scale if image scales
                        boxes[..., :4] *= scale
                boxes[..., 0::2] += x
                boxes[..., 1::2] += y
                for j, box in enumerate(boxes.astype(np.int64).tolist()):
                    c = classes[j]
                    color = colors(c)
                    c = names.get(c, c) if names else c
                    if labels or conf[j] > conf_thres:
                       if show_labels: 
                          label = f"{c}" if labels else f"{c} {conf[j]:.1f}"
                          annotator.box_label(box, label, color=color, rotated=is_obb)
                       else: 
                          annotator.box_label(box, '', color=color)

            elif len(classes):
                for c in classes:
                    color = colors(c)
                    c = names.get(c, c) if names else c

                    if labels or conf[j] > conf_thres: 
                       if show_labels: 
                          annotator.text((x, y), f"{c}", txt_color=color, box_style=True)
                       else: 
                          annotator.text((x, y), '', txt_color=color, box_style=True)

            # Plot keypoints
            if len(kpts):
                kpts_ = kpts[idx].copy()
                if len(kpts_):
                    if kpts_[..., 0].max() <= 1.01 or kpts_[..., 1].max() <= 1.01:  # if normalized with tolerance .01
                        kpts_[..., 0] *= w  # scale to pixels
                        kpts_[..., 1] *= h
                    elif scale < 1:  # absolute coords need scale if image scales
                        kpts_ *= scale
                kpts_[..., 0] += x
                kpts_[..., 1] += y

                for j in range(len(kpts_)):
                    if labels or conf[j] > conf_thres:
                        annotator.kpts(kpts_[j])

            # Plot masks
            if len(masks):
                if idx.shape[0] == masks.shape[0]:  # overlap_masks=False
                    image_masks = masks[idx]
                else:  # overlap_masks=True
                    image_masks = masks[[i]]  # (1, 640, 640)
                    nl = idx.sum()
                    index = np.arange(nl).reshape((nl, 1, 1)) + 1
                    image_masks = np.repeat(image_masks, nl, axis=0)
                    image_masks = np.where(image_masks == index, 1.0, 0.0)

                im = np.asarray(annotator.im).copy()
                for j in range(len(image_masks)):
                    if labels or conf[j] > conf_thres:
                        color = colors(classes[j])
                        mh, mw = image_masks[j].shape
                        if mh != h or mw != w:
                            mask = image_masks[j].astype(np.uint8)
                            mask = cv2.resize(mask, (w, h))
                            mask = mask.astype(bool)
                        else:
                            mask = image_masks[j].astype(bool)
                        with contextlib.suppress(Exception):
                            im[y : y + h, x : x + w, :][mask] = (
                                im[y : y + h, x : x + w, :][mask] * 0.4 + np.array(color) * 0.6
                            )
                annotator.fromarray(im)

    annotator.im.save(fname)  # save
    if on_plot:
        on_plot(fname)

def drawpoly(img, pts, color, thickness=2, gap=10):
    for p in pts[::gap]:
        cv2.circle(img, p[0], thickness, color, -1)

def sort_coordinates(list_of_xy_coords):
    cx, cy = list_of_xy_coords.mean(0)
    x, y = list_of_xy_coords.T
    angles = np.arctan2(x-cx, y-cy)
    indices = np.argsort(angles)
    return list_of_xy_coords[indices]

def order_points_new(pts):
    # sort the points based on their x-coordinates
    xSorted = pts[np.argsort(pts[:, 0]), :]

    # grab the left-most and right-most points from the sorted
    # x-roodinate points
    leftMost = xSorted[:2, :]
    rightMost = xSorted[2:, :]

    # now, sort the left-most coordinates according to their
    # y-coordinates so we can grab the top-left and bottom-left
    # points, respectively
    leftMost = leftMost[np.argsort(leftMost[:, 1]), :]
    (tl, bl) = leftMost

    # if use Euclidean distance, it will run in error when the object
    # is trapezoid. So we should use the same simple y-coordinates order method.

    # now, sort the right-most coordinates according to their
    # y-coordinates so we can grab the top-right and bottom-right
    # points, respectively
    rightMost = rightMost[np.argsort(rightMost[:, 1]), :]
    (tr, br) = rightMost

    # return the coordinates in top-left, top-right,
    # bottom-right, and bottom-left order
    return np.array([tl, tr, br, bl], dtype="float32")

def save_results(results, mask, imgsz_val, brain=None, brain_imgsz_val=None, too_many_fragments_warning=300, brain_color=(255, 127, 255), target_color=(127, 255, 255), thickness_mod=1, use_computed_brain_mask_for_prediction_zone_correction=True, erode_level=0, erode_mask_size=5): 
    brain_data = []
    brain_data_ok = []

    if brain != None: 
       if len(results) > 0: 
          brains_path = os.path.dirname(results[0].path)
          #print(f"brains_path: {brains_path}")
          results_brain = brain.predict(workers=global_workers, verbose=False, name="temp", batch=4, source=brains_path, imgsz=brain_imgsz_val, save=False, conf=0.25, show_labels=False, show_conf=False, show_boxes=False, max_det=1)

          for i in range(len(results_brain)):
              r = results_brain[i]
              r_orig_img_shape = r.orig_img.shape[:2]

              img = None
              img_ready = False
              h0, w0 = r_orig_img_shape[0], r_orig_img_shape[1]

              h, w = (h0, w0)
              ratio = brain_imgsz_val / max(h0, w0)  # ratio
              if r != 1: 
                 h, w = (min(math.ceil(h0 * ratio), brain_imgsz_val), min(math.ceil(w0 * ratio), brain_imgsz_val))

              en_for_r = enumerate(r)
              for ci,c in en_for_r: 
                  b_mask2 = (c.masks.data.cpu().numpy().astype(np.uint8) * 255)
                  b_mask2 = b_mask2.reshape(b_mask2.shape[1], b_mask2.shape[2])
                  b_mask2_start_shape = b_mask2.shape

                  h_pad, w_pad = ((int(b_mask2_start_shape[0] - h) // 2), (int(b_mask2_start_shape[1] - w) // 2))
                  if (b_mask2_start_shape[0] - h - h_pad * 2) > 0: 
                     h_pad = h_pad + 1
                  if (b_mask2_start_shape[1] - w - w_pad * 2) > 0: 
                     w_pad = w_pad + 1

                  b_mask2 = b_mask2[h_pad:h_pad+h, w_pad:w_pad+w]
                  b_mask = ((cv2.resize(b_mask2, (w * find_contours_scale, h * find_contours_scale), interpolation=cv2.INTER_LINEAR) > 127) * 255).astype(np.uint8) # wokerd

                  if img_ready: 
                     img = cv2.bitwise_or(b_mask, img, mask=None)
                  else: 
                     img = b_mask.copy()
                     img_ready = True

              brain_contours, _ = cv2.findContours(img, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
              sorted_contours = sorted(brain_contours, key=cv2.contourArea, reverse=True)
              if len(sorted_contours) > 0: 
                 draw_result = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)
                 img = cv2.drawContours(draw_result, sorted_contours, 0, (255), cv2.FILLED)

              if not img_ready: 
                 img = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)
                 img.fill(255)

              brain_data.append(img)
              brain_data_ok.append(img_ready)

    if ((brain != None and len(brain_data) == len(results)) or brain == None): 
       # new validation code
       total_background_pixels = 0
       total_target_pixels = 0
       total_predicted_as_background_pixels = 0
       total_predicted_as_target_pixels = 0
       total_true_predicted = 0
       total_background_pixels_predicted_as_target = 0

       # new validation code
       if len(results) > 0: 
          labels_dir_for_test_images_targets = os.path.abspath(os.path.join(os.path.join(os.path.dirname(results[0].path), '..'), 'labels'))

       for i in range(len(results)): 
           r = results[i]
           r_orig_img_shape = r.orig_img.shape[:2]
           h0, w0 = r_orig_img_shape[0], r_orig_img_shape[1]

           h, w = (h0, w0)
           ratio = imgsz_val / max(h0, w0)  # ratio
           if r != 1:  # if sizes are not equal
              h, w = (min(math.ceil(h0 * ratio), imgsz_val), min(math.ceil(w0 * ratio), imgsz_val))

           # Save input image with big resolution
           img_orig_big = cv2.resize(cv2.imread(Path(r.path), cv2.IMREAD_GRAYSCALE), (w * find_contours_scale, h * find_contours_scale), interpolation=cv2.INTER_LINEAR)
           #cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_input_big.png", img_orig_big, [cv2.IMWRITE_PNG_COMPRESSION, 3])

           img2 = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)

           if brain != None: 
              brain_obj = (cv2.resize(brain_data[i], (w * find_contours_scale, h * find_contours_scale), interpolation=cv2.INTER_LINEAR) > 127).astype(np.uint8) * 255

           pth = Path(r.save_dir + "\\masks").resolve()
           pth.mkdir(parents=True, exist_ok=True)

           contours = []
           en_for_r = enumerate(r)
           for ci,c in en_for_r: 
               b_mask2 = (c.masks.data.cpu().numpy().astype(np.uint8) * 255)
               b_mask2 = b_mask2.reshape(b_mask2.shape[1], b_mask2.shape[2])
               b_mask2_start_shape = b_mask2.shape

               h_pad, w_pad = ((int(b_mask2_start_shape[0] - h) // 2), (int(b_mask2_start_shape[1] - w) // 2))
               if (b_mask2_start_shape[0] - h - h_pad * 2) > 0: 
                  h_pad = h_pad + 1
               if (b_mask2_start_shape[1] - w - w_pad * 2) > 0: 
                  w_pad = w_pad + 1

               b_mask2 = b_mask2[h_pad:h_pad+h, w_pad:w_pad+w]
               #print(f"b_mask2.shape: {b_mask2.shape}")
               res = cv2.resize(b_mask2, (find_contours_scale * b_mask2.shape[1], find_contours_scale * b_mask2.shape[0]), interpolation=cv2.INTER_NEAREST)
               #print(f"res.shape: {res.shape}")

               #cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + "_debug2.png", res)

               ct, _ = cv2.findContours(res, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
               ct = sorted(ct, key=cv2.contourArea, reverse=True)

               if len(ct) > 0: 
                  appended_contours = 0
                  for obj_index in range(len(ct)):
                      obj = ct[obj_index].astype(np.int32)

                      if brain != None: 
                         draw_result = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)
                         #print(f"draw_result.shape: {draw_result.shape}")
                         draw_result = cv2.drawContours(draw_result, [obj], 0, (255), cv2.FILLED)

                         #cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + "_draw_result_" + str(ci) + "_" + str(obj_index) + ".png", draw_result)

                         b_sum_1 = (draw_result > 127).sum()

                         if b_sum_1 >= (find_contours_scale * find_contours_scale): 
                            intersection = cv2.bitwise_and(brain_obj, draw_result, mask=None)
                            b_sum_0 = (intersection > 127).sum()
                            if b_sum_0 / b_sum_1 > 0.1: 
                               contours.append(obj)
                               appended_contours = appended_contours + 1
                      else: 
                         contours.append(obj)
                         appended_contours = appended_contours + 1

                  if appended_contours >= too_many_fragments_warning: 
                     print(f"Too many fragments in '{Path(r.path).stem}' ({len(ct)})")

                  if brain == None: # only detect brain
                     img2 = cv2.drawContours(np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8), ct, 0, (255), cv2.FILLED)
                     break
                  else: 
                     img2 = cv2.bitwise_or(res, img2, mask=None)

           #cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + "_debug4.png", img2)

           #if brain != None: 
           #   cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + "_debug5.png", brain_obj)

           # erode prediction result
           if brain != None and erode_level > 0: 
              img2 = cv2.erode(src=img2, kernel=np.ones((5, 5)), iterations=erode_level)

           if use_computed_brain_mask_for_prediction_zone_correction == False: 
              img2_cpy = img2.copy()

           if brain != None: 
              img2 = cv2.bitwise_and(brain_obj, img2, mask=None)

           # new validation code
           if use_computed_brain_mask_for_prediction_zone_correction: 
              total_predicted_as_background_pixels = total_predicted_as_background_pixels + np.sum(img2 < 128)
              total_predicted_as_target_pixels = total_predicted_as_target_pixels + np.sum(img2 > 127)
           else: 
              total_predicted_as_background_pixels = total_predicted_as_background_pixels + np.sum(img2_cpy < 128)
              total_predicted_as_target_pixels = total_predicted_as_target_pixels + np.sum(img2_cpy > 127)
           txt_dataset_path = os.path.join(labels_dir_for_test_images_targets, Path(r.path).stem + ".txt")
           if os.path.exists(txt_dataset_path): 
              #print(f"Parsing {txt_dataset_path}\r\n")
              cv_dataset_contours = []
              with open(txt_dataset_path) as f:
                   lines = f.readlines()
                   for line in lines: 
                       line = list(chunks([np.float32(x) for x in line[2:].split()], 2))
                       #print(f"line: {line}")
                       line = np.array([[np.int32(math.floor(x * w * find_contours_scale + 0.5)), np.int32(math.floor(y * h * find_contours_scale + 0.5))] for x,y in line])
                       cv_dataset_contours.append(line)
              dataset_image = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)

              for c_id in range(len(cv_dataset_contours)): 
                 dataset_image = cv2.drawContours(dataset_image, cv_dataset_contours, c_id, (255), cv2.FILLED)

              if brain != None and erode_level > 0: 
                 dataset_image = cv2.erode(src=dataset_image, kernel=np.ones((5, 5)), iterations=erode_level)

              cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + "_dataset.png", dataset_image, [cv2.IMWRITE_PNG_COMPRESSION, 3])

              # use computed brain mask for target zone dataset correction (tested for adc ischemia dataset)
              #if brain != None: 
              #   dataset_image = cv2.bitwise_and(dataset_image, dataset_image, mask=brain_obj)
              #   cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + "_dataset_corrected.png", dataset_image, [cv2.IMWRITE_PNG_COMPRESSION, 3])

              total_background_pixels = total_background_pixels + np.sum(dataset_image < 128)
              total_target_pixels = total_target_pixels + np.sum(dataset_image > 127)

              if use_computed_brain_mask_for_prediction_zone_correction: 
                 total_true_predicted = total_true_predicted + np.sum(np.logical_and(dataset_image > 127, img2 > 127))
                 total_background_pixels_predicted_as_target = total_background_pixels_predicted_as_target + np.sum(np.logical_and(dataset_image < 128, img2 > 127))
              else: 
                 total_true_predicted = total_true_predicted + np.sum(np.logical_and(dataset_image > 127, img2_cpy > 127))
                 total_background_pixels_predicted_as_target = total_background_pixels_predicted_as_target + np.sum(np.logical_and(dataset_image < 128, img2_cpy > 127))
           else: 
              print(f"Can't find {txt_dataset_path}")

           # Save mask with big resolution
           cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + "_big.png", img2, [cv2.IMWRITE_PNG_COMPRESSION, 3])

           # save visual
           if brain == None: 
              new_image_ct = cv2.cvtColor(img_orig_big, cv2.COLOR_GRAY2RGB)

              # filled images
              new_image = cv2.bitwise_and(img_orig_big, img_orig_big, mask=img2)
              new_image = cv2.cvtColor(new_image, cv2.COLOR_GRAY2RGB)
              indices_background = np.where(img2 == 0)
              new_image[indices_background[0], indices_background[1], :] = [brain_color[0], brain_color[1], brain_color[2]]

              cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual.png", new_image, [cv2.IMWRITE_PNG_COMPRESSION, 3])

              have_brain = np.sum(img2 == 255) > 0
              # contours
              if have_brain: 
                 ct_brain, _ = cv2.findContours(img2, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                 img_contours = cv2.drawContours(new_image_ct, ct_brain, -1, (brain_color[0], brain_color[1], brain_color[2]), 2 * thickness_mod)
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual_ct.png", img_contours, [cv2.IMWRITE_PNG_COMPRESSION, 3])
              else:
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual_ct.png", new_image_ct, [cv2.IMWRITE_PNG_COMPRESSION, 3])
           else: 
              new_image_ct = cv2.cvtColor(img_orig_big, cv2.COLOR_GRAY2RGB)

              # filled images
              new_image = cv2.bitwise_and(img_orig_big, img_orig_big, mask=brain_obj)
              new_image = cv2.cvtColor(new_image, cv2.COLOR_GRAY2RGB)
              indices_background = np.where(brain_obj == 0)
              new_image[indices_background[0], indices_background[1], :] = [brain_color[0], brain_color[1], brain_color[2]]
              indices_target = np.where(img2 == 255)
              new_image[indices_target[0], indices_target[1], :] = [target_color[0], target_color[1], target_color[2]]
              cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual.png", new_image, [cv2.IMWRITE_PNG_COMPRESSION, 3])

              have_brain = brain_data_ok[i] and np.sum(brain_obj == 255) > 0
              have_target = np.sum(img2 == 255) > 0

              if have_brain: 
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_brain_big.png", brain_obj, [cv2.IMWRITE_PNG_COMPRESSION, 3])
                 brain_small = ((cv2.resize(brain_obj, (w0, h0), interpolation=cv2.INTER_AREA) > 127) * 255).astype(np.uint8)
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_brain.png", brain_small, [cv2.IMWRITE_PNG_COMPRESSION, 3])
              else: 
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_brain_big.png", np.zeros((h * find_contours_scale, w * find_contours_scale)), [cv2.IMWRITE_PNG_COMPRESSION, 3])
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_brain.png", np.zeros((w0, h0)), [cv2.IMWRITE_PNG_COMPRESSION, 3])
              
              # contours
              if have_brain and have_target: 
                 ct_brain, _ = cv2.findContours(brain_obj, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                 _, thresh = cv2.threshold(img2, 127, 255, 0)
                 ct_target, hierarchy_target = cv2.findContours(thresh, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)

                 img_contours = cv2.drawContours(new_image_ct, ct_brain, -1, (brain_color[0], brain_color[1], brain_color[2]), 2 * thickness_mod)

                 for idx in range(len(hierarchy_target[0])): 
                     if hierarchy_target[0][idx][3] == -1: 
                        img_contours = cv2.drawContours(img_contours, ct_target, idx, (target_color[0], target_color[1], target_color[2]), 2 * thickness_mod)
                     else: 
                        drawpoly(img_contours, ct_target[idx], (target_color[0], target_color[1], target_color[2]), thickness=2 * thickness_mod, gap=10)
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual_ct.png", img_contours, [cv2.IMWRITE_PNG_COMPRESSION, 3])
              elif have_brain: 
                 ct_brain, _ = cv2.findContours(brain_obj, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                 img_contours = cv2.drawContours(new_image_ct, ct_brain, -1, (brain_color[0], brain_color[1], brain_color[2]), 2 * thickness_mod)
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual_ct.png", img_contours, [cv2.IMWRITE_PNG_COMPRESSION, 3])
              elif have_target: 
                 _, thresh = cv2.threshold(img2, 127, 255, 0)
                 ct_target, hierarchy_target = cv2.findContours(thresh, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)

                 for idx in range(len(hierarchy_target[0])): 
                     if hierarchy_target[0][idx][3] == -1: 
                        img_contours = cv2.drawContours(img_contours, ct_target, idx, (target_color[0], target_color[1], target_color[2]), 2 * thickness_mod)
                     else: 
                        drawpoly(img_contours, ct_target[idx], (target_color[0], target_color[1], target_color[2]), thickness=2 * thickness_mod, gap=10)

                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual_ct.png", img_contours, [cv2.IMWRITE_PNG_COMPRESSION, 3])
              else:
                 cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + "_visual_ct.png", new_image_ct, [cv2.IMWRITE_PNG_COMPRESSION, 3])

           # Save mask with original resolution
           img2 = ((cv2.resize(img2, (w0, h0), interpolation=cv2.INTER_AREA) > 127) * 255).astype(np.uint8)
           cv2.imwrite(r.save_dir + "\\masks\\" + Path(r.path).stem + mask + ".png", img2, [cv2.IMWRITE_PNG_COMPRESSION, 3])

           # Save contours data
           txt_data = ""
           first_countour_found = False
           for contour in contours: 
               contour_for_txt = (contour.astype(np.float32) / (r_orig_img_shape[1] * ratio * find_contours_scale, r_orig_img_shape[0] * ratio * find_contours_scale)).reshape(-1)

               txt_data = txt_data + ("" if first_countour_found == False else "\n") + "0 " + ' '.join(f'{x:.7f}' for x in contour_for_txt)
               first_countour_found = True

           text_file = open(r.save_dir + "\\masks\\" + Path(r.path).stem + ".txt", "w")
           text_file.write(txt_data)
           text_file.close()

       # new validation code
       if len(results) > 0: 
          # count in kilopixels
          total_background_pixels = total_background_pixels / 1000
          total_target_pixels = total_target_pixels / 1000
          total_predicted_as_background_pixels = total_predicted_as_background_pixels / 1000
          total_predicted_as_target_pixels = total_predicted_as_target_pixels / 1000
          total_true_predicted = total_true_predicted / 1000
          total_background_pixels_predicted_as_target = total_background_pixels_predicted_as_target / 1000

          #precision_div = total_background_pixels_predicted_as_target * total_target_pixels + (total_predicted_as_target_pixels - total_background_pixels_predicted_as_target) * total_background_pixels
          #if precision_div > 0: 
          #   precision = (total_true_predicted * total_background_pixels) / precision_div
          #   print(f"Normalized precision --- {precision:.6f}")

          if total_predicted_as_target_pixels > 0: 
             precision = (total_true_predicted) / total_predicted_as_target_pixels
             print(f"Precision --- {precision:.6f}")

          if total_target_pixels > 0: 
             recall = total_true_predicted / total_target_pixels
             print(f"Recall    --- {recall:.6f}")

          print("\n")
    else: 
       print("Invalid input data")

# Based on original Yolo codes
def internal_validate(exp_name, big_data, imgsz_val, model_path, dataset_info, batch_size=1, visualize_conf_thres=global_visualize_conf_thres, visualize_show_labels = False, iou_val=0.6, conf_val=0.01, max_det_val=300): 
    dir_path = "./runs/segment/" + exp_name + ("_big" if big_data else "")
    os.makedirs(dir_path, exist_ok = True)
    shutil.rmtree(dir_path)

    # rect=True не трогать

    with torch.no_grad(): 
      args = dict(workers=global_workers, plots=True, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, show_labels=True, show_conf=True, task="segment", name=exp_name + ("_big" if big_data else ""), model=model_path, imgsz=imgsz_val, iou=iou_val, conf=conf_val, max_det=max_det_val)
      validator = segment.SegmentationValidator(args=args)
      validator.device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
      validator.seen = 0
      validator.run_callbacks('on_val_start')
      validator.jdict = []

      modelxxx = AutoBackend(validator.args.model, 
                             device=validator.device, 
                             dnn=validator.args.dnn, 
                             data=validator.args.data, 
                             fp16=validator.args.half, 
                             verbose=validator.args.verbose, 
                             fuse=True)

      validator.data = check_det_dataset(dataset_info)

      yolo_data = YOLODataset(img_path=validator.data.get('test'),
                              imgsz=validator.args.imgsz,
                              augment=False,
                              hyp=validator.args,
                              rect=validator.args.rect, 
                              cache=validator.args.cache or None,
                              single_cls=validator.args.single_cls,
                              stride=max(int(modelxxx.stride), 32),
                              pad=0.0, #pad=0.0 if mode == 'train' else 0.5,
                              prefix=colorstr(f'val: '),
                              task="segment",
                              classes=validator.args.classes,
                              data=validator.data,
                              fraction=1.0)
      validator.dataloader = build_dataloader(yolo_data, batch=batch_size, workers=validator.args.workers, shuffle=False, rank=-1)

      modelxxx.model.eval()
      modelxxx = de_parallel(modelxxx)
      validator.init_metrics(modelxxx)

      bar = TQDM(validator.dataloader, desc=validator.get_desc(), total=len(validator.dataloader))
      dt = ops.Profile(), ops.Profile(), ops.Profile()
      for batch_i, batch_with_data in enumerate(bar): 
          validator.plot_masks = []

          validator.run_callbacks('on_val_batch_start')
          validator.batch_i = batch_i

          # Preprocess
          with dt[0]:
               batch_with_data = validator.preprocess(batch_with_data)

          # Inference
          with dt[1]: 
               preds = modelxxx(batch_with_data['img'])

          # Postprocess
          with dt[2]:
               preds = validator.postprocess(preds)

          #validator.update_metrics(preds, batch_with_data)
          #print(f"\r\n")

          for si, (pred, proto) in enumerate(zip(preds[0], preds[1])):
               validator.seen += 1
               npr = len(pred)
               stat = dict(
                   conf=torch.zeros(0, device=validator.device),
                   pred_cls=torch.zeros(0, device=validator.device),
                   tp=torch.zeros(npr, validator.niou, dtype=torch.bool, device=validator.device),
                   tp_m=torch.zeros(npr, validator.niou, dtype=torch.bool, device=validator.device),
               )
               pbatch = validator._prepare_batch(si, batch_with_data)
               cls, bbox = pbatch.pop("cls"), pbatch.pop("bbox")
               nl = len(cls)
               stat["target_cls"] = cls
               if npr == 0:
                   if nl:
                       for k in validator.stats.keys():
                           validator.stats[k].append(stat[k])
                       if validator.args.plots:
                           validator.confusion_matrix.process_batch(detections=None, gt_bboxes=bbox, gt_cls=cls)
                   continue

               # Masks
               gt_masks = pbatch.pop("masks")
               # Predictions
               if validator.args.single_cls:
                   pred[:, 5] = 0
               predn, pred_masks = validator._prepare_pred(pred, pbatch, proto)
               stat["conf"] = predn[:, 4]
               stat["pred_cls"] = predn[:, 5]

               # Evaluate
               if nl:
                   stat["tp"] = validator._process_batch(predn, bbox, cls)
                   stat["tp_m"] = validator._process_batch(
                       predn, bbox, cls, pred_masks, gt_masks, validator.args.overlap_mask, masks=True
                   )
                   if validator.args.plots:
                       validator.confusion_matrix.process_batch(predn, bbox, cls)

               for k in validator.stats.keys():
                   validator.stats[k].append(stat[k])

               # Save data to plotting
               pred_masks = torch.as_tensor(pred_masks, dtype=torch.uint8)
               validator.plot_masks.append(pred_masks[:validator.args.max_det])

          # plot_val_samples
          plot_images_and_export_data(batch_with_data['img'], batch_with_data['batch_idx'], batch_with_data['cls'].squeeze(-1), batch_with_data['bboxes'], None, batch_with_data['masks'], paths=batch_with_data['im_file'], fname=validator.save_dir / f'val_batch{batch_i}_labels.jpg', names=validator.names, on_plot=validator.on_plot, conf_thres=visualize_conf_thres, show_labels=visualize_show_labels)

          # plot_predictions
          plot_images_and_export_data(batch_with_data['img'], *output_to_target(preds[0], max_det=validator.args.max_det), torch.cat(validator.plot_masks, dim=0) if len(validator.plot_masks) else validator.plot_masks, paths=batch_with_data['im_file'], fname=validator.save_dir / f'val_batch{batch_i}_pred.jpg', names=validator.names, on_plot=validator.on_plot, conf_thres=visualize_conf_thres, show_labels=visualize_show_labels)
          validator.plot_masks.clear()

          validator.run_callbacks('on_val_batch_end')
      stats = validator.get_stats()
      validator.check_stats(stats)
      validator.finalize_metrics()
      validator.print_results()
      validator.run_callbacks('on_val_end')

def runtime_func(big_data, default_validate, generate_data, generate_data_hq):
    if default_validate: 
       # t2_brain
       model_brain = YOLO("./runs/segment/t2_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt")
       model_brain.val(workers=global_workers, task="segment", overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, name="val_t2_brain_" + str(brain_and_ischemia_imgsz) + "_default", batch=4, imgsz=brain_and_ischemia_imgsz, iou=global_iou, conf=0.01, max_det=1)

       # t2_ischemia
       model = YOLO("./runs/segment/t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented/weights/best.pt")
       model.val(workers=global_workers, task="segment", overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, name="val_t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_default", batch=4, imgsz=brain_and_ischemia_imgsz, iou=global_iou, conf=0.01)

       # adc_brain
       model_brain = YOLO("./runs/segment/adc_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt")
       model_brain.val(workers=global_workers, task="segment", overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, name="val_adc_brain_" + str(brain_and_ischemia_imgsz) + "_default", batch=4, imgsz=brain_and_ischemia_imgsz, iou=global_iou, conf=0.01, max_det=1)

       # adc_ischemia
       model = YOLO("./runs/segment/adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented/weights/best.pt")
       model.val(workers=global_workers, task="segment", overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, name="val_adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_default", batch=4, imgsz=brain_and_ischemia_imgsz, iou=global_iou, conf=0.01)

       # swi_brain
       model_brain = YOLO("./runs/segment/swi_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt")
       model_brain.val(workers=global_workers, task="segment", overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, name="val_swi_brain_" + str(brain_and_ischemia_imgsz) + "_default", batch=4, imgsz=brain_and_ischemia_imgsz, iou=global_iou, conf=0.01, max_det=1)

       # swi_msc
       model = YOLO("./runs/segment/swi_msc_mod_" + str(msk_imgsz) + "_augmented/weights/best.pt")
       model.val(workers=global_workers, task="segment", overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, name="val_swi_msc_" + str(msk_imgsz) + "_default", batch=4, imgsz=msk_imgsz, iou=global_iou, conf=0.01)
    elif generate_data: 
       def_tresh = 0.01

       # t2_brain
       model_brain = YOLO("./runs/segment/t2_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt")
       source_val = "./datasets/t2_data/brain/test/images" + ("_big" if big_data else "")
       if os.path.exists(source_val) and os.listdir(source_val): 
          results = model_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_brain_" + str(brain_and_ischemia_imgsz) + "_default" + ("_big" if big_data else ""), batch=44, source=source_val, imgsz=brain_and_ischemia_imgsz, save=True, conf=(global_hq_threshold if generate_data_hq else def_tresh), iou=global_iou, show_labels=big_data, show_boxes=big_data, show_conf=big_data, max_det=1)
          save_results(results, "_brain", imgsz_val=brain_and_ischemia_imgsz, thickness_mod=1)

       # t2_ischemia
       model = YOLO("./runs/segment/t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented/weights/best.pt")
       source_val = "./datasets/t2_data/ischemia/test/images" + ("_big" if big_data else "")
       if os.path.exists(source_val) and os.listdir(source_val): 
          results = model.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_default" + ("_big" if big_data else ""), batch=4, source=source_val, imgsz=brain_and_ischemia_imgsz, save=True, conf=(global_hq_threshold if generate_data_hq else def_tresh), iou=global_iou, show_labels=big_data, show_boxes=big_data, show_conf=big_data)
          save_results(results, "_ischemia", brain=model_brain, brain_imgsz_val=brain_and_ischemia_imgsz, imgsz_val=brain_and_ischemia_imgsz, thickness_mod=1)

       # adc_brain
       model_brain = YOLO("./runs/segment/adc_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt")
       source_val = "./datasets/adc_data/brain/test/images" + ("_big" if big_data else "")
       if os.path.exists(source_val) and os.listdir(source_val): 
          results = model_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_adc_brain_" + str(brain_and_ischemia_imgsz) + "_default" + ("_big" if big_data else ""), batch=44, source=source_val, imgsz=brain_and_ischemia_imgsz, save=True, conf=(global_hq_threshold if generate_data_hq else def_tresh), iou=global_iou, show_labels=big_data, show_boxes=big_data, show_conf=big_data, max_det=1)
          save_results(results, "_brain", imgsz_val=brain_and_ischemia_imgsz, brain_color=(255, 0, 255), target_color=(255, 0, 0), thickness_mod=1)

       # adc_ischemia with erode dataset info and results
       model = YOLO("./runs/segment/adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented/weights/best.pt")
       source_val = "./datasets/adc_data/ischemia/test/images" + ("_big" if big_data else "")
       if os.path.exists(source_val) and os.listdir(source_val): 
          results = model.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_default" + ("_big" if big_data else ""), batch=4, source=source_val, imgsz=brain_and_ischemia_imgsz, save=True, conf=(global_hq_threshold if generate_data_hq else def_tresh), iou=global_iou, show_labels=big_data, show_boxes=big_data, show_conf=big_data)
          save_results(results, "_ischemia", brain=model_brain, brain_imgsz_val=brain_and_ischemia_imgsz, imgsz_val=brain_and_ischemia_imgsz, brain_color=(255, 0, 255), target_color=(255, 0, 0), thickness_mod=1, erode_level=2, erode_mask_size=5)

       # swi_brain
       model_brain = YOLO("./runs/segment/swi_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt")
       source_val = "./datasets/swi_data/brain/test/images" + ("_big" if big_data else "")
       if os.path.exists(source_val) and os.listdir(source_val): 
          results = model_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_swi_brain_" + str(brain_and_ischemia_imgsz) + "_default" + ("_big" if big_data else ""), batch=44, source=source_val, imgsz=brain_and_ischemia_imgsz, save=True, conf=(global_hq_threshold if generate_data_hq else def_tresh), iou=global_iou, show_labels=big_data, show_boxes=big_data, show_conf=big_data, max_det=1)
          save_results(results, "_brain", imgsz_val=brain_and_ischemia_imgsz, thickness_mod=2)

       # swi_msc
       model = YOLO("./runs/segment/swi_msc_mod_" + str(msk_imgsz) + "_augmented/weights/best.pt")
       source_val = "./datasets/swi_data/msc/test/images" + ("_big" if big_data else "")
       if os.path.exists(source_val) and os.listdir(source_val): 
          results = model.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_swi_msc_" + str(msk_imgsz) + "_default" + ("_big" if big_data else ""), batch=6, source=source_val, imgsz=msk_imgsz, save=True, conf=(global_hq_threshold if generate_data_hq else def_tresh), iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)
          save_results(results, "_msk", brain=model_brain, brain_imgsz_val=brain_and_ischemia_imgsz, imgsz_val=msk_imgsz, thickness_mod=2)
    else: 
       internal_validate(exp_name="val_t2_brain_" + str(brain_and_ischemia_imgsz) + "_new", big_data=big_data, imgsz_val=brain_and_ischemia_imgsz, model_path="./runs/segment/t2_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt", dataset_info="data_t2_brain.yaml", batch_size=4, visualize_conf_thres=global_visualize_conf_thres, visualize_show_labels=True, iou_val=global_iou, conf_val=0.01, max_det_val=1)
       internal_validate(exp_name="val_t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_new", big_data=big_data, imgsz_val=brain_and_ischemia_imgsz, model_path="./runs/segment/t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented/weights/best.pt", dataset_info="data_t2_ischemia.yaml", batch_size=4, visualize_conf_thres=global_visualize_conf_thres, visualize_show_labels=True, iou_val=global_iou, conf_val=0.01)

       internal_validate(exp_name="val_adc_brain_" + str(brain_and_ischemia_imgsz) + "_new", big_data=big_data, imgsz_val=brain_and_ischemia_imgsz, model_path="./runs/segment/adc_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt", dataset_info="data_adc_brain.yaml", batch_size=4, visualize_conf_thres=global_visualize_conf_thres, visualize_show_labels=True, iou_val=global_iou, conf_val=0.01, max_det_val=1)
       internal_validate(exp_name="val_adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_new", big_data=big_data, imgsz_val=brain_and_ischemia_imgsz, model_path="./runs/segment/adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented/weights/best.pt", dataset_info="data_adc_ischemia.yaml", batch_size=4, visualize_conf_thres=global_visualize_conf_thres, visualize_show_labels=True, iou_val=global_iou, conf_val=0.01)

       internal_validate(exp_name="val_swi_brain_" + str(brain_and_ischemia_imgsz) + "_new", big_data=big_data, imgsz_val=brain_and_ischemia_imgsz, model_path="./runs/segment/swi_brain_" + str(brain_and_ischemia_imgsz) + "/weights/best.pt", dataset_info="data_swi_brain.yaml", batch_size=4, visualize_conf_thres=global_visualize_conf_thres, visualize_show_labels=True, iou_val=global_iou, conf_val=0.01, max_det_val=1)
       internal_validate(exp_name="val_swi_msc_" + str(msk_imgsz) + "_new", big_data=big_data, imgsz_val=msk_imgsz, model_path="./runs/segment/swi_msc_mod_" + str(msk_imgsz) + "_augmented/weights/best.pt", dataset_info="data_swi_msc.yaml", batch_size=4, visualize_conf_thres=global_visualize_conf_thres, visualize_show_labels=False, iou_val=global_iou, conf_val=0.01)

@smart_inference_mode()
def cmd():
    big_data = False
    generate_data_hq = False

    args = sys.argv[1:]
    if len(args) > 0: 
       if args[0] == 'default_validate': 
          default_validate = True
          generate_data = False
       elif args[0] == 'generate_data': 
          default_validate = False
          generate_data = True
       elif args[0] == 'internal_validate': 
          default_validate = False
          generate_data = False
       else: 
          print(f"Set mode: default_validate, generate_data or internal_validate")
          return
    else: 
      print(f"Set mode: default_validate, generate_data or internal_validate")
      return

    runtime_func(big_data, default_validate, generate_data, generate_data_hq)

if __name__ == '__main__':
   torch.multiprocessing.freeze_support()
   cmd()