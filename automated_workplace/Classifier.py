from PySide2 import QtCore, QtGui, QtWidgets
import numpy as np
import cv2
import math
import threading

from Profiler import Profiler, profile #debug

class MaskType:
	BRAIN = 1
	ISCHEMIA = 2
	MSC = 4

from ultralytics import YOLO

find_contours_scale=2
global_iou=0.50
global_retina_masks=False
global_half=True
global_workers=0
brain_and_ischemia_imgsz=512
msk_imgsz=1280

@profile
def get_zone_mask(result, imgsz_val, get_brain=True, erode_level=0, erode_mask_size=5): 
	r_orig_img_shape = result.orig_img.shape[:2]
	h0, w0 = r_orig_img_shape[0], r_orig_img_shape[1]
	ratio = imgsz_val / max(h0, w0)
	h, w = min(math.ceil(h0 * ratio), imgsz_val), min(math.ceil(w0 * ratio), imgsz_val)

	img = np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8)

	en_for_r = enumerate(result)
	for ci, c in en_for_r:
		b_mask = c.masks.data.detach().cpu().numpy().astype(np.uint8) * 255
		b_mask = b_mask.reshape(b_mask.shape[1], b_mask.shape[2])
		b_mask_start_shape = b_mask.shape

		h_pad, w_pad = int(b_mask_start_shape[0] - h) // 2, int(b_mask_start_shape[1] - w) // 2
		if (b_mask_start_shape[0] - h - h_pad * 2) > 0:
			h_pad = h_pad + 1
		if (b_mask_start_shape[1] - w - w_pad * 2) > 0:
			w_pad = w_pad + 1

		b_mask = b_mask[h_pad:h_pad+h, w_pad:w_pad+w]
		res = cv2.resize(b_mask, (find_contours_scale * b_mask.shape[1], find_contours_scale * b_mask.shape[0]), interpolation=cv2.INTER_NEAREST)

		if get_brain:
			ct, _ = cv2.findContours(res, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
			ct = sorted(ct, key=cv2.contourArea, reverse=True)
			img = cv2.drawContours(np.zeros((h * find_contours_scale, w * find_contours_scale), np.uint8), ct, 0, (255), cv2.FILLED)
			break
		else:
			img = cv2.bitwise_or(res, img, mask=None)

	if erode_level > 0:
		img = cv2.erode(src=img, kernel=np.ones((erode_mask_size, erode_mask_size)), iterations=erode_level)

	img = ((cv2.resize(img, (w0, h0), interpolation=cv2.INTER_AREA) > 127) * 255).astype(np.uint8)

	return img

class YoloModel:
	def __init__(self, weights_file):
		self.weights_file = weights_file
		self.model = YOLO(self.weights_file) if QtCore.QFileInfo.exists(self.weights_file) else None
		if self.model == None:
			raise Exception("Incorrect settings, file \'{}\' not found".format(self.weights_file))
		self.lock = threading.Lock()
		#self.warmup()

	'''
	def warmup(self):
		with self.lock:
			self.model.predict(...) #perform model warmup here!
	'''

	@profile
	def predict(self, *args, **kwargs):
		with self.lock:
			return self.model.predict(*args, **kwargs, workers=global_workers, retina_masks=global_retina_masks, half=global_half, verbose=False, save=False, iou=global_iou, show_labels=False, show_boxes=False, show_conf=False)

def batched(it, batch_size):
	batch=[None]*batch_size
	count = 0
	for value in it:
		batch[count] = value
		count = count + 1
		if count == batch_size:
			yield batch
			batch = [None]*batch_size #comment this line in single thread mode
			count = 0
	if count != 0:
		yield batch[0:count]

'''
#this one a little faster, but need to unpack generator
def batched(it, batch_size):
	iterable = [*it]
	for i in range(0, len(iterable), batch_size):
		yield iterable[i:i + batch_size]
'''

class Classifier:
	def __init__(self):
		self.path_adc_brain = './resources/runs/segment/adc_brain_' + str(brain_and_ischemia_imgsz) + '/weights/best.pt'
		self.path_adc_ischemia = './resources/runs/segment/adc_ischemia_' + str(brain_and_ischemia_imgsz) + '_augmented/weights/best.pt'
		self.path_swi_brain = './resources/runs/segment/swi_brain_' + str(brain_and_ischemia_imgsz) + '/weights/best.pt'
		self.path_swi_msc = './resources/runs/segment/swi_msc_mod_' + str(msk_imgsz) + '_augmented/weights/best.pt'
		self.path_t2_brain = './resources/runs/segment/t2_brain_' + str(brain_and_ischemia_imgsz) + '/weights/best.pt'
		self.path_t2_ischemia = './resources/runs/segment/t2_ischemia_' + str(brain_and_ischemia_imgsz) + '_augmented/weights/best.pt'

	def loadSettings(self, settings):
		settings.beginGroup("Classifier")
		self.path_adc_brain = settings.value('path_adc_brain', self.path_adc_brain)
		self.path_adc_ischemia = settings.value('path_adc_ischemia', self.path_adc_ischemia)
		self.path_swi_brain = settings.value('path_swi_brain', self.path_swi_brain)
		self.path_swi_msc = settings.value('path_swi_msc', self.path_swi_msc)
		self.path_t2_brain = settings.value('path_t2_brain', self.path_t2_brain)
		self.path_t2_ischemia = settings.value('path_t2_ischemia', self.path_t2_ischemia)
		settings.endGroup()

		self.adc_brain = YoloModel(self.path_adc_brain)
		self.adc_ischemia = YoloModel(self.path_adc_ischemia)
		self.swi_brain = YoloModel(self.path_swi_brain)
		self.swi_msc = YoloModel(self.path_swi_msc)
		self.t2_brain = YoloModel(self.path_t2_brain)
		self.t2_ischemia = YoloModel(self.path_t2_ischemia)

		#torch.use_deterministic_algorithms(True)

	def saveSettings(self, settings):
		settings.beginGroup("Classifier")
		settings.setValue('path_adc_brain', self.path_adc_brain)
		settings.setValue('path_adc_ischemia', self.path_adc_ischemia)
		settings.setValue('path_swi_brain', self.path_swi_brain)
		settings.setValue('path_swi_msc', self.path_swi_msc)
		settings.setValue('path_t2_brain', self.path_t2_brain)
		settings.setValue('path_t2_ischemia', self.path_t2_ischemia)
		settings.endGroup()

	def preprocess(self, cached_data):
		def preprocess(slice_data, low_value, high_value):
			low = (slice_data < low_value)
			high = (slice_data > high_value)
			medium = np.logical_not(np.logical_or(low, high))
			delta = (high_value - low_value) / 255.0
			return np.piecewise(slice_data, [low, medium, high], [0, lambda x: np.floor((x - low_value) / delta + 0.5), 255]).astype(np.uint8)

		preprocessed_slice = None
		if cached_data.protocol == "ep2d_diff_tra_14b": # ADC
			preprocessed_slice = preprocess(cached_data.ds.pixel_array, 0, 855)
		elif cached_data.protocol == "swi_tra":         # SWI
			preprocessed_slice = preprocess(cached_data.ds.pixel_array, 25, 383)
		elif cached_data.protocol == "t2_tse_tra_fs":   # T2
			preprocessed_slice = preprocess(cached_data.ds.pixel_array, 25, 855)

		cached_data.preprocessed = cv2.cvtColor(preprocessed_slice, cv2.COLOR_GRAY2RGB)

	@profile
	def getMask(self, mask_type, cached_data_gen):
		batch_size = 4
		for batch in batched(cached_data_gen, batch_size):
			yolo_input = [cached_data.preprocessed for cached_data in batch]
			protocol = batch[0].protocol

			if mask_type == MaskType.BRAIN:
				if protocol == "ep2d_diff_tra_14b": # ADC
					yolo_output = self.adc_brain.predict(name="predict_adc_brain", batch=batch_size, source=yolo_input, imgsz=brain_and_ischemia_imgsz, conf=0.15, max_det=1)
					for batch_index in range(len(batch)):
						batch[batch_index].brain_mask = get_zone_mask(yolo_output[batch_index], imgsz_val=brain_and_ischemia_imgsz, get_brain=True, erode_level=0)
				elif protocol == "swi_tra":         # SWI
					yolo_output = self.swi_brain.predict(name="predict_swi_brain", batch=batch_size, source=yolo_input, imgsz=brain_and_ischemia_imgsz, conf=0.15, max_det=1)
					for batch_index in range(len(batch)):
						batch[batch_index].brain_mask = get_zone_mask(yolo_output[batch_index], imgsz_val=brain_and_ischemia_imgsz, get_brain=True, erode_level=0)
				elif protocol == "t2_tse_tra_fs":   # T2
					yolo_output = self.t2_brain.predict(name="predict_t2_brain", batch=batch_size, source=yolo_input, imgsz=brain_and_ischemia_imgsz, conf=0.15, max_det=1)
					for batch_index in range(len(batch)):
						batch[batch_index].brain_mask = get_zone_mask(yolo_output[batch_index], imgsz_val=brain_and_ischemia_imgsz, get_brain=True, erode_level=0)
			elif mask_type == MaskType.ISCHEMIA:
				if protocol == "ep2d_diff_tra_14b": # ADC
					yolo_output = self.adc_ischemia.predict(name="predict_adc_isc", batch=batch_size, source=yolo_input, imgsz=brain_and_ischemia_imgsz, conf=0.05)
					for batch_index in range(len(batch)):
						batch[batch_index].ischemia_mask = get_zone_mask(yolo_output[batch_index], imgsz_val=brain_and_ischemia_imgsz, get_brain=False, erode_level=2)
				elif protocol == "t2_tse_tra_fs":   # T2
					yolo_output = self.t2_ischemia.predict(name="predict_t2_isc", batch=batch_size, source=yolo_input, imgsz=brain_and_ischemia_imgsz, conf=0.05)
					for batch_index in range(len(batch)):
						batch[batch_index].ischemia_mask = get_zone_mask(yolo_output[batch_index], imgsz_val=brain_and_ischemia_imgsz, get_brain=False, erode_level=0)
			elif mask_type == MaskType.MSC:
				if protocol == "swi_tra": # SWI
					yolo_output = self.swi_msc.predict(name="predict_swi_msc", batch=batch_size, source=yolo_input, imgsz=msk_imgsz, conf=0.05)
					for batch_index in range(len(batch)):
						batch[batch_index].msc_mask = get_zone_mask(yolo_output[batch_index], imgsz_val=msk_imgsz, get_brain=False, erode_level=2)
