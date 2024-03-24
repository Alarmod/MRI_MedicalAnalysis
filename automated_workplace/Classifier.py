from PySide2 import QtCore, QtGui, QtWidgets
import numpy as np
import cv2, math, os.path
from memoization import cached

from Dataset import loadDICOMFile

class MaskType:
  BRAIN = 1
  ISCHEMIA = 2
  MSC = 4

from ultralytics import YOLO
from ultralytics.utils.torch_utils import smart_inference_mode

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

def get_zones_data(results, imgsz_val, get_brain=True, erode_level=0, erode_mask_size=5): 
    for i in range(len(results)): 
        r = results[i]
        r_orig_img_shape = r.orig_img.shape[:2]
        h0, w0 = r_orig_img_shape[0], r_orig_img_shape[1]

        h, w = (h0, w0)
        ratio = imgsz_val / max(h0, w0)
        if r != 1: 
           h, w = (min(math.ceil(h0 * ratio), imgsz_val), min(math.ceil(w0 * ratio), imgsz_val))

        img2 = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)

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

            if get_brain: 
               ct, _ = cv2.findContours(res, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
               ct = sorted(ct, key=cv2.contourArea, reverse=True)
               img2 = cv2.drawContours(np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8), ct, 0, (255), cv2.FILLED)
               break
            else:
               img2 = cv2.bitwise_or(res, img2, mask=None)

        if get_brain == False:
           if erode_level > 0: 
              img2 = cv2.erode(src=img2, kernel=np.ones((erode_mask_size, erode_mask_size)), iterations=erode_level)

        img2 = ((cv2.resize(img2, (w0, h0), interpolation=cv2.INTER_AREA) > 127) * 255).astype(np.uint8)

        return img2

class Classifier:
  def __init__(self):
    self.__hash = hash(str(self))
    self.path_adc_brain = './resources/runs/segment/adc_brain_' + str(brain_and_ischemia_imgsz) + '/weights/best.pt'
    self.path_adc_ischemia = './resources/runs/segment/adc_ischemia_' + str(brain_and_ischemia_imgsz) + '_augmented/weights/best.pt'
    self.path_swi_brain = './resources/runs/segment/swi_brain_' + str(brain_and_ischemia_imgsz) + '/weights/best.pt'
    self.path_swi_msc = './resources/runs/segment/swi_msc_mod_' + str(msk_imgsz) + '_augmented/weights/best.pt'
    self.path_t2_brain = './resources/runs/segment/t2_brain_' + str(brain_and_ischemia_imgsz) + '/weights/best.pt'
    self.path_t2_ischemia = './resources/runs/segment/t2_ischemia_' + str(brain_and_ischemia_imgsz) + '_augmented/weights/best.pt'

  def loadSettings(self, settings):
    settings.beginGroup("Classifier")
    self.path = settings.value('path_adc_brain', self.path_adc_brain)
    self.path = settings.value('path_adc_ischemia', self.path_adc_ischemia)
    self.path = settings.value('path_swi_brain', self.path_swi_brain)
    self.path = settings.value('path_swi_msc', self.path_swi_msc)
    self.path = settings.value('path_t2_brain', self.path_t2_brain)
    self.path = settings.value('path_t2_ischemia', self.path_t2_ischemia)
    settings.endGroup()

    self.adc_brain = YOLO(self.path_adc_brain) if os.path.exists(self.path_adc_brain) else None
    self.adc_ischemia = YOLO(self.path_adc_ischemia) if os.path.exists(self.path_adc_ischemia) else None
    self.swi_brain = YOLO(self.path_swi_brain) if os.path.exists(self.path_swi_brain) else None
    self.swi_msc = YOLO(self.path_swi_msc) if os.path.exists(self.path_swi_msc) else None
    self.t2_brain = YOLO(self.path_t2_brain) if os.path.exists(self.path_t2_brain) else None
    self.t2_ischemia = YOLO(self.path_t2_ischemia) if os.path.exists(self.path_t2_ischemia) else None

    if self.adc_brain == None or self.adc_ischemia == None or self.swi_brain == None or self.swi_msc == None or self.t2_brain == None or self.t2_ischemia == None: 
       print("Not all artificial neural networks are successfully loaded (problem with settings)")

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

  @cached(max_size=64)
  def __preprocessSlice(self, filename):
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

    preprocessed_slice = None
    if ds.ProtocolName == "ep2d_diff_tra_14b": #ADC
        preprocessed_slice = preprocess(ds.pixel_array, 0, 855)
    elif ds.ProtocolName == "swi_tra":         #SWI
        preprocessed_slice = preprocess(ds.pixel_array, 25, 383)
    elif ds.ProtocolName == "t2_tse_tra_fs":   #T2
        preprocessed_slice = preprocess(ds.pixel_array, 25, 855)

    img = cv2.cvtColor(preprocessed_slice, cv2.COLOR_GRAY2RGB)

    return ds.ProtocolName, img

  @cached(max_size=512)
  @smart_inference_mode()
  def getMask(self, mask_type, filename):
    protocolName, yolo_input = self.__preprocessSlice(filename)

    mask = None
    if mask_type == MaskType.BRAIN: 
       if protocolName == "ep2d_diff_tra_14b": # ADC
          if self.adc_brain != None: 
             yolo_output = self.adc_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_adc_brain", batch=0, source=yolo_input, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.15, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False, max_det=1)
             mask = get_zones_data(yolo_output, imgsz_val=brain_and_ischemia_imgsz, get_brain=True, erode_level=0)
       elif protocolName == "swi_tra":         # SWI
          if self.swi_brain != None: 
             yolo_output = self.swi_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_swi_brain", batch=0, source=yolo_input, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.15, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False, max_det=1)
             mask = get_zones_data(yolo_output, imgsz_val=brain_and_ischemia_imgsz, get_brain=True, erode_level=0)
       elif protocolName == "t2_tse_tra_fs":   # T2
          if self.t2_brain != None: 
             yolo_output = self.t2_brain.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_brain", batch=0, source=yolo_input, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.15, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False, max_det=1)
             mask = get_zones_data(yolo_output, imgsz_val=brain_and_ischemia_imgsz, get_brain=True, erode_level=0)
    elif mask_type == MaskType.ISCHEMIA:
       brain_mask = self.getMask(MaskType.BRAIN, filename)
       if protocolName == "ep2d_diff_tra_14b": # ADC
          if self.adc_ischemia != None: 
             yolo_output = self.adc_ischemia.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_adc_isc", batch=0, source=yolo_input, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.05, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)
             mask = get_zones_data(yolo_output, imgsz_val=brain_and_ischemia_imgsz, get_brain=False, erode_level=2)
       elif protocolName == "t2_tse_tra_fs":   # T2
          if self.t2_ischemia != None: 
             yolo_output = self.t2_ischemia.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_t2_isc", batch=0, source=yolo_input, imgsz=brain_and_ischemia_imgsz, save=False, conf=0.05, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)
             mask = get_zones_data(yolo_output, imgsz_val=brain_and_ischemia_imgsz, get_brain=False, erode_level=0)
       if type(brain_mask) != type(None) and type(mask) != type(None): 
          mask = cv2.bitwise_and(brain_mask, mask)
    elif mask_type == MaskType.MSC:
       brain_mask = self.getMask(MaskType.BRAIN, filename)
       if protocolName == "swi_tra": # SWI
          if self.swi_msc != None: 
             yolo_output = self.swi_msc.predict(workers=global_workers, overlap_mask=global_overlap_mask, single_cls=global_single_cls, save_json=global_save_json, mask_ratio=global_mask_ratio, retina_masks=global_retina_masks, half=global_half, rect=True, verbose=False, name="predict_swi_msc", batch=0, source=yolo_input, imgsz=msk_imgsz, save=False, conf=0.05, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)
             mask = get_zones_data(yolo_output, imgsz_val=msk_imgsz, get_brain=False, erode_level=2)
       if type(brain_mask) != type(None) and type(mask) != type(None): 
          mask = cv2.bitwise_and(brain_mask, mask)

    if type(mask) == type(None):
       return np.zeros((yolo_input.shape[0], yolo_input.shape[1]), dtype=np.uint8)

    return mask