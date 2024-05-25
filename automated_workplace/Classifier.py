from PySide2 import QtCore, QtGui, QtWidgets
import os
import numpy as np

import boot
from yolo_segment_with_nanobind import yolo_segment_ext as yolo_inference

from Profiler import Profiler, profile #debug

class MaskType:
	BRAIN = 1
	ISCHEMIA = 2
	MSC = 4

def setGlobalThreadPoolSize(num_threads):
	return yolo_inference.setGlobalThreadPoolSize(num_threads)

class YoloModel:
	def __init__(self, name, weights_file, input_width, input_height, useGPU, useFP16, cuda_id):
		self.name = name
		self.weights_file = weights_file
		self.input_width = input_width
		self.input_height = input_height
		self.useGPU = useGPU
		self.useFP16 = useFP16
		self.cuda_id = cuda_id

		self.__model = yolo_inference.YOLO(name=self.name, net_path=self.weights_file, use_gpu=self.useGPU, use_fp16=self.useFP16, net_width=self.input_width, net_height=self.input_height, cudaID=self.cuda_id)

	def warmup(self):
		print(f"Warming up model {self.name} (GPU={self.useGPU})")
		self.__model.warmup()

	def predict(self, input_image, **kwargs):
		return self.__model.process(input=input_image, **kwargs)


class ModelDescription:
	def __init__(self, short_model_desc, models_config):
		self.name = None
		self.path = None
		self.format = None
		self.input_width = None
		self.input_height = None
		self.inference_device = None
		self.inference_cudaid = None #GPU mode only

		if len(short_model_desc) == 2:
			self.name = short_model_desc[0]
			self.loadFromConfig(self.name, models_config)
			
			if short_model_desc[1] == "CPU":
				self.inference_device = "CPU"
				self.inference_cudaid = 0 #default value (not used)
			elif short_model_desc[1] == "GPU":
				self.inference_device = "GPU"
				self.inference_cudaid = 0 #default value
			else:
				raise Exception(f"Unknown inference device {short_model_desc[1]}")
				
		elif len(short_model_desc) == 3:
			self.name = short_model_desc[0]
			self.loadFromConfig(self.name, models_config)

			if short_model_desc[1] == "CPU":
				raise Exception("")
			if short_model_desc[1] == "GPU":
				self.inference_device = "GPU"
				try:
					self.inference_cudaid = int(short_model_desc[2])
					if self.inference_cudaid < 0:
						raise Exception("Wrong CUDA device id")
				except:
					raise Exception("Wrong CUDA device id")
			else:
				raise Exception(f"Unknown inference device {short_model_desc[1]}")
		else:
			raise Exception("")

	def loadFromConfig(self, name, models_config):
		groups = models_config.childGroups()
		if name == None or not (name in groups):
			raise Exception(f"Referenced \'{name}\' not exists")

		models_config.beginGroup(name)

		self.path = models_config.value("path", None)
		if self.path == None:
			raise Exception(f"Model path not set")
		if not os.path.exists(self.path):
			raise Exception(f"File \'{self.path}\' not exists")

		self.format = models_config.value("format", None)
		if self.format == None:
			raise Exception(f"Model format not set")
		if not (self.format in ["FP16", "FP32"]):
			raise Exception(f"Unsupported model format")

		self.input_width = models_config.value("input_width", None)
		if self.input_width == None:
			raise Exception(f"Model input_width not set")
		else:
			try:
				self.input_width = int(self.input_width)
				if self.input_width < 0:
					raise Exception("Incorrect input_width value")
			except:
				raise Exception("Incorrect input_width value")


		self.input_height = models_config.value("input_height", None)
		if self.input_height == None:
			raise Exception(f"Model input_height not set")
		else:
			try:
				self.input_height = int(self.input_height)
				if self.input_height < 0:
					raise Exception("Incorrect input_height value")
			except:
				raise Exception("Incorrect input_height value")

		models_config.endGroup()

	def createModel(self):
		return YoloModel(self.name, self.path, self.input_width, self.input_height, self.inference_device == "GPU", self.format == "FP16", self.inference_cudaid)
			

class Classifier:
	def __init__(self):
		self.models_config_fname = None
		self.models_config = None

		self.inference_threads_count = None

		self.models = { 'adc_brain_model' : [None, None], \
                                'adc_ischemia_model' : [None, None], \
                                'swi_brain_model' : [None, None], \
                                'swi_msc_model' : [None, None], \
                                't2_brain_model' : [None, None], \
                                't2_ischemia_model' : [None, None] }

	def loadSettings(self, settings):
		settings.beginGroup("Classifier")

		#global thread pool
		self.inference_threads_count = settings.value("inference_threads_count", None)
		if self.inference_threads_count != None:
			try:
				self.inference_threads_count = int(self.inference_threads_count)
			except:
				raise Exception(f"Error: inference_threads_count incorrect value")
		else:
			raise Exception(f"Warning: inference_threads_count not set, use default")

		if (not setGlobalThreadPoolSize(self.inference_threads_count)):
			raise Exception(f"Error: Can't change thread pool size")

		#load models.ini
		self.models_config_fname = settings.value("models_config", None)
		if self.models_config_fname == None:
			raise Exception(f"Error: Models description file not set")
		if not os.path.exists(self.models_config_fname):
			raise Exception(f"Error: Models description file \'{self.models_config_fname}\' not found")
		self.models_config = QtCore.QSettings(self.models_config_fname, QtCore.QSettings.IniFormat)

		#load models info
		for model in self.models.keys():
			short_model_desc = settings.value(model, None)
			if short_model_desc == None:
				raise Exception(f"Error: {model} missing")
			try:				
				self.models[model][0] = ModelDescription(short_model_desc, self.models_config)
			except Exception as ex:     
				raise Exception(f"Error: {model} incorrect description. " + str(ex))

		settings.endGroup()

		for model in self.models.keys():
			self.models[model][1] = self.models[model][0].createModel()
			self.models[model][1].warmup()

	def saveSettings(self, settings):
		settings.beginGroup("Classifier")

		if self.models_config_fname != None:
			settings.setValue('models_config', self.models_config_fname)

		if self.inference_threads_count != None:
			settings.setValue('inference_threads_count', self.inference_threads_count)

		settings.endGroup()

	def preprocess(self, cached_data):
		def preprocess(slice_data, low_value, high_value):
			low = (slice_data < low_value)
			high = (slice_data > high_value)
			medium = np.logical_not(np.logical_or(low, high))
			delta = (high_value - low_value) / 255.0
			return np.piecewise(slice_data, [low, medium, high], [0, lambda x: np.floor((x - low_value) / delta + 0.5), 255]).astype(np.uint8)

		cached_data.preprocessed = None
		if cached_data.protocol == "ep2d_diff_tra_14b": # ADC
			cached_data.preprocessed = preprocess(cached_data.ds.pixel_array, 0, 855)
		elif cached_data.protocol == "swi_tra":         # SWI
			cached_data.preprocessed = preprocess(cached_data.ds.pixel_array, 25, 383)
		elif cached_data.protocol == "t2_tse_tra_fs":   # T2
			cached_data.preprocessed = preprocess(cached_data.ds.pixel_array, 25, 855)

	@profile
	def getMask(self, mask_type, cached_data):
		yolo_input = cached_data.preprocessed
		protocol = cached_data.protocol

		if mask_type == MaskType.BRAIN:
			if protocol == "ep2d_diff_tra_14b":
				cached_data.brain_mask = self.models["adc_brain_model"][1].predict(yolo_input, rec_treshold=0.15, max_results=1, get_brain=True, erode_level=0)
			elif protocol == "swi_tra":
				cached_data.brain_mask = self.models["swi_brain_model"][1].predict(yolo_input, rec_treshold=0.15, max_results=1, get_brain=True, erode_level=0)
			elif protocol == "t2_tse_tra_fs":
				cached_data.brain_mask = self.models["t2_brain_model"][1].predict(yolo_input, rec_treshold=0.15, max_results=1, get_brain=True, erode_level=0)
		elif mask_type == MaskType.ISCHEMIA:
			if protocol == "ep2d_diff_tra_14b":
				cached_data.ischemia_mask = self.models["adc_ischemia_model"][1].predict(yolo_input, rec_treshold=0.05, max_results=255, get_brain=False, erode_level=2)
			elif protocol == "t2_tse_tra_fs":
				cached_data.ischemia_mask = self.models["t2_ischemia_model"][1].predict(yolo_input, rec_treshold=0.05, max_results=255, get_brain=False, erode_level=2)
		elif mask_type == MaskType.MSC:
			if protocol == "swi_tra":
				cached_data.msc_mask = self.models["swi_msc_model"][1].predict(yolo_input, rec_treshold=0.05, max_results=1024, get_brain=False, erode_level=2)
