from PySide2 import QtCore, QtGui, QtWidgets
import numpy as np
import cv2
from memoization import cached

from Dataset import loadDICOMFile

class MaskType:
  BRAIN = 1
  ISCHEMIA = 2
  MSC = 4

from ultralytics import YOLO
import math

find_contours_scale=2
global_iou=0.50
global_overlap_mask=False
global_single_cls=True
global_save_json=False
global_mask_ratio=1
global_retina_masks=False
global_half=False
global_workers=0

brain_and_ischemia_imgsz=512
msk_imgsz=1280

def get_results(results, mask, imgsz_val, brain=[], brain_imgsz_val=None, erode_level=0, erode_mask_size=5): 
    brain_data = []

    if len(brain) > 0: 
       if len(results) > 0: 
          for i in range(len(brain)):
              r = brain[i]
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

    if ((len(brain) > 0 and len(brain_data) == len(results)) or len(brain) == 0): 
       for i in range(len(results)): 
           r = results[i]
           r_orig_img_shape = r.orig_img.shape[:2]
           h0, w0 = r_orig_img_shape[0], r_orig_img_shape[1]

           h, w = (h0, w0)
           ratio = imgsz_val / max(h0, w0) # ratio
           if r != 1:  # if sizes are not equal
              h, w = (min(math.ceil(h0 * ratio), imgsz_val), min(math.ceil(w0 * ratio), imgsz_val))

           img2 = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)

           if len(brain) > 0: 
              brain_obj = (cv2.resize(brain_data[i], (w * find_contours_scale, h * find_contours_scale), interpolation=cv2.INTER_LINEAR) > 127).astype(np.uint8) * 255

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
               res = cv2.resize(b_mask2, (find_contours_scale * b_mask2.shape[1], find_contours_scale * b_mask2.shape[0]), interpolation=cv2.INTER_NEAREST)

               if len(brain) == 0: # Only detect brain
                  ct, _ = cv2.findContours(res, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
                  ct = sorted(ct, key=cv2.contourArea, reverse=True)

                  img2 = cv2.drawContours(np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8), ct, 0, (255), cv2.FILLED)
                  break
               else: 
                  img2 = cv2.bitwise_or(res, img2, mask=None)

           # Erode prediction result
           if len(brain) > 0 and erode_level > 0: 
              img2 = cv2.erode(src=img2, kernel=np.ones((5, 5)), iterations=erode_level)

           # Result
           if len(brain) > 0: 
              img2 = cv2.bitwise_and(brain_obj, img2, mask=None)

           # Save mask with original resolution
           img2 = ((cv2.resize(img2, (w0, h0), interpolation=cv2.INTER_AREA) > 127) * 255).astype(np.uint8)

           return img2
    else: 
       return None

class Classifier:
  def __init__(self):
    self.__hash = hash(str(self))
    self.path_adc_brain = './resources/runs/segment/adc_brain_512/weights/best.pt'
    self.path_adc_ischemia = './resources/runs/segment/adc_ischemia_512_augmented/weights/best.pt'
    self.path_swi_brain = './resources/runs/segment/swi_brain_512/weights/best.pt'
    self.path_swi_msc = './resources/runs/segment/swi_msc_mod_1280_augmented/weights/best.pt'
    self.path_t2_brain = './resources/runs/segment/t2_brain_512/weights/best.pt'
    self.path_t2_ischemia = './resources/runs/segment/t2_ischemia_512_augmented/weights/best.pt'

  def loadSettings(self, settings):
    settings.beginGroup("Classifier")
    self.path = settings.value('path_adc_brain', self.path_adc_brain)
    self.path = settings.value('path_adc_ischemia', self.path_adc_ischemia)
    self.path = settings.value('path_swi_brain', self.path_swi_brain)
    self.path = settings.value('path_swi_msc', self.path_swi_msc)
    self.path = settings.value('path_t2_brain', self.path_t2_brain)
    self.path = settings.value('path_t2_ischemia', self.path_t2_ischemia)
    settings.endGroup()

    self.adc_brain = YOLO(self.path_adc_brain)
    self.adc_ischemia = YOLO(self.path_adc_ischemia)

    self.swi_brain = YOLO(self.path_swi_brain)
    self.swi_msc = YOLO(self.path_swi_msc)

    self.t2_brain = YOLO(self.path_t2_brain)
    self.t2_ischemia = YOLO(self.path_t2_ischemia)

  def saveSettings(self, settings):
    settings.beginGroup("Classifier")
    settings.setValue('path_adc_brain', self.path_adc_brain)
    settings.setValue('path_adc_ischemia', self.path_adc_ischemia)
    settings.setValue('path_swi_brain', self.path_swi_brain)
    settings.setValue('path_swi_msc', self.path_swi_msc)
    settings.setValue('path_t2_brain', self.path_t2_brain)
    settings.setValue('path_t2_ischemia', self.path_t2_ischemia)
    settings.endGroup()

  def __hash__(self):
    return self.__hash

  @cached(max_size=512)
  def getMask(self, mask_type, filename):
    ds = loadDICOMFile(filename)

    '''
    arr = ds.pixel_array

    ds_width = ds.Columns
    ds_height = ds.Rows

    img = np.empty((ds_height, ds_width, 3), dtype=np.uint8)
    for y in range(ds_height): 
        for x in range(ds_width): 
            val_uint16 = arr[y, x]

            if ds.ProtocolName == "ep2d_diff_tra_14b": #ADC
               if val_uint16 > 855: 
                 img[y, x, 0] = 255
               else: 
                 img[y, x, 0] = np.floor(val_uint16 / 855.0 * 255.0 + 0.5)
            elif ds.ProtocolName == "swi_tra": #SWI
               if val_uint16 > 383:
                 img[y, x, 0] = 255
               elif val_uint16 < 25: 
                 img[y, x, 0] = 0
               else: 
                 img[y, x, 0] = np.floor((val_uint16 - 25) / 358.0 * 255.0 + 0.5)
            elif ds.ProtocolName == "t2_tse_tra_fs": #T2
               if val_uint16 > 855: 
                 img[y, x, 0] = 255
               elif val_uint16 < 25: 
                 img[y, x, 0] = 0
               else: 
                 img[y, x, 0] = np.floor((val_uint16 - 25) / 830.0 * 255.0 + 0.5)

            img[y, x, 1] = img[y, x, 0]
            img[y, x, 2] = img[y, x, 0]
    '''

    def preprocess(slice_data, low_value, high_value):
        low = (slice_data < low_value)
        high = (slice_data > high_value)
        medium = np.logical_not(np.logical_or(low, high))
        delta = (high_value - low_value) / 255.0
        return np.piecewise(slice_data, [low, medium, high], [0, lambda x: np.floor((x - low_value) / delta + 0.5), 255]).astype(np.uint8)

    preprocesed_slice = None
    if ds.ProtocolName == "ep2d_diff_tra_14b": #ADC
        preprocesed_slice = preprocess(ds.pixel_array, 0, 855)
    elif ds.ProtocolName == "swi_tra": #SWI
        preprocesed_slice = preprocess(ds.pixel_array, 25, 383)
    elif ds.ProtocolName == "t2_tse_tra_fs": #T2
        preprocesed_slice = preprocess(ds.pixel_array, 25, 855)

    img = cv2.cvtColor(preprocesed_slice, cv2.COLOR_GRAY2RGB)


    if ds.ProtocolName == "ep2d_diff_tra_14b": # ADC
       adc_brain_data = self.adc_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_brain", batch=0, source=img, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.15, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False, max_det=1)
    elif ds.ProtocolName == "swi_tra":         # SWI
       swi_brain_data = self.swi_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_brain", batch=0, source=img, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.15, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False, max_det=1)
    elif ds.ProtocolName == "t2_tse_tra_fs":   # T2
       t2_brain_data = self.t2_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_brain", batch=0, source=img, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.15, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False, max_det=1)

    mask = None
    if mask_type == MaskType.BRAIN: 
       if ds.ProtocolName == "ep2d_diff_tra_14b": # ADC
          mask = get_results(adc_brain_data, "_brain", imgsz_val=brain_and_ischemia_imgsz)
       elif ds.ProtocolName == "swi_tra":         # SWI
          mask = get_results(swi_brain_data, "_brain", imgsz_val=brain_and_ischemia_imgsz)
       elif ds.ProtocolName == "t2_tse_tra_fs":   # T2
          mask = get_results(t2_brain_data, "_brain", imgsz_val=brain_and_ischemia_imgsz)
    elif mask_type == MaskType.ISCHEMIA: # ADC and #T2
       if ds.ProtocolName == "ep2d_diff_tra_14b": # ADC
          adc_ISC_data = self.adc_ischemia.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_adc_isc", batch=0, source=img, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.05, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)
          mask = get_results(adc_ISC_data, "_adc_ischemia", brain=adc_brain_data, brain_imgsz_val=brain_and_ischemia_imgsz, imgsz_val=brain_and_ischemia_imgsz, erode_level=2)
       elif ds.ProtocolName == "t2_tse_tra_fs":   # T2
          t2_ISC_data = self.t2_ischemia.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_isc", batch=0, source=img, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.05, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)
          mask = get_results(t2_ISC_data, "_t2_ischemia", brain=t2_brain_data, brain_imgsz_val=brain_and_ischemia_imgsz, imgsz_val=brain_and_ischemia_imgsz)
    elif mask_type == MaskType.MSC:      # SWI
       if ds.ProtocolName == "swi_tra": 
          swi_MSC_data = self.swi_msc.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_swi_msc", batch=0, source=img, imgsz=msk_imgsz, save=False, conf=0.05, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)
          mask = get_results(swi_MSC_data, "_swi_msc", brain=swi_brain_data, brain_imgsz_val=brain_and_ischemia_imgsz, imgsz_val=msk_imgsz)

    # на случай, если результаты получить не удалось, следует вернуть пустую маску
    if type(mask) == type(None):
       return np.zeros(ds.pixel_array.shape, dtype=np.uint8)

    return mask