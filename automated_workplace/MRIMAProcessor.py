from PySide2 import QtCore, QtGui, QtWidgets
import vtk
import numpy as np
import cv2
import functools
import concurrent.futures
from datetime import datetime

from Cache import get_cached_data
from Dataset import Dataset
from Entity import *
from ViewMode import ViewMode

from Classifier import Classifier, MaskType
from VolumeCombiner import VolumeCombiner
from MSCTracker import MSCTracker, distance

from Profiler import profile #debug

class Marker:
	BRAIN = 29000
	ISCHEMIA = 30000
	MSC = 31000
	REFERENCED_MSC = 32000

class Mesh(vtk.vtkActor):
	def __init__(self):
		self.mbds = vtk.vtkMultiBlockDataSet()
		self.mapper = vtk.vtkCompositePolyDataMapper()
		self.mapper.SetInputDataObject(self.mbds)
		cdda = vtk.vtkCompositeDataDisplayAttributes()
		self.mapper.SetCompositeDataDisplayAttributes(cdda)
		self.SetMapper(self.mapper)

	def addLine(self, x1, y1, z1, x2, y2, z2, color, opacity):
		line = vtk.vtkLineSource()
		line.SetPoint1(x1, y1, z1)
		line.SetPoint2(x2, y2, z2)
		line.Update()

		currentBlock = self.mbds.GetNumberOfBlocks()
		self.mbds.SetBlock(currentBlock, line.GetOutput())
		self.mapper.SetBlockColor(currentBlock+1, color)
		self.mapper.SetBlockOpacity(currentBlock+1, opacity)

def _percents_str(total, count):
	if total == 0 or count == 0:
		return ""
	else:
		return " / {:.2f}%".format(count * 100 / total)

def run_in_threadpool(thread_pool, func, *args, **kwargs):
	return thread_pool.submit(functools.partial(func, *args, **kwargs))

class MRIMAProcessor(QtCore.QObject):
	def __init__(self):
		super().__init__()

		self.gradientMinValue = 100
		self.gradientMaxValue = 255

		self.volumeSpacingADC = [0.40000000596046, 0.40000000596046, 1.05] #row col slice
		self.volumeSpacingSWI = [0.1171875, 0.1171875, 0.5]
		self.volumeSpacingT2 = [0.1171875, 0.1171875, 0.805]

		#3d view
		self.brainColorRGBA = [1.0, 1.0, 0.0, 0.05]
		self.ischemiaColorRGBA = [1.0, 0.0, 0.0, 0.30]
		self.MSCColorRGBA = [0.0, 1.0, 0.0, 0.95]
		self.RefMSCColorRGBA = [0.0, 1.0, 0.0, 0.80]
		self.trackColorRGBA = [0.0, 0.5, 0.0, 0.70]

		#2d view
		self.brainColorRGB = np.array(self.brainColorRGBA[0:3])*255
		self.ischemiaColorRGB = np.array(self.ischemiaColorRGBA[0:3])*255
		self.MSCColorRGB = np.array(self.MSCColorRGBA[0:3])*255
		self.use_2D_contours = 1

		self.classifier = Classifier()
		self.mscTracker = MSCTracker()

		self.volume_property = None

	def loadSettings(self, settings):
		settings.beginGroup("Processor")

		self.gradientMinValue = int(settings.value('GradientMinValue', self.gradientMinValue))
		self.gradientMaxValue = int(settings.value('GradientMaxValue', self.gradientMaxValue))

		self.volumeSpacingADC = list(map(float, settings.value('VolumeSpacingADC', self.volumeSpacingADC)))
		self.volumeSpacingSWI = list(map(float, settings.value('VolumeSpacingSWI', self.volumeSpacingSWI)))
		self.volumeSpacingT2 = list(map(float, settings.value('VolumeSpacingT2', self.volumeSpacingT2)))

		self.brainColorRGBA = list(map(float, settings.value('BrainColorRGBA', self.brainColorRGBA)))
		self.ischemiaColorRGBA = list(map(float, settings.value('IschemiaColorRGBA', self.ischemiaColorRGBA)))
		self.MSCColorRGBA = list(map(float, settings.value('MSCColorRGBA', self.MSCColorRGBA)))
		self.RefMSCColorRGBA = list(map(float, settings.value('RefMSCColorRGBA', self.RefMSCColorRGBA)))
		self.trackColorRGBA = list(map(float, settings.value('TrackColorRGBA', self.trackColorRGBA)))

		self.use_2D_contours = int(settings.value('use_2D_contours', self.use_2D_contours))

		settings.endGroup()

		#2d view
		self.brainColorRGB = np.array(self.brainColorRGBA[0:3])*255
		self.ischemiaColorRGB = np.array(self.ischemiaColorRGBA[0:3])*255
		self.MSCColorRGB = np.array(self.MSCColorRGBA[0:3])*255

		self.classifier.loadSettings(settings)

		self.volume_property = self.__createVolumeProperty()

	def saveSettings(self, settings):
		settings.beginGroup("Processor")

		settings.setValue('GradientMinValue', self.gradientMinValue)
		settings.setValue('GradientMaxValue', self.gradientMaxValue)

		settings.setValue('VolumeSpacingADC', self.volumeSpacingADC)
		settings.setValue('VolumeSpacingSWI', self.volumeSpacingSWI)
		settings.setValue('VolumeSpacingT2', self.volumeSpacingT2)

		settings.setValue('BrainColorRGBA', self.brainColorRGBA)
		settings.setValue('IschemiaColorRGBA', self.ischemiaColorRGBA)
		settings.setValue('MSCColorRGBA', self.MSCColorRGBA)
		settings.setValue('RefMSCColorRGBA', self.RefMSCColorRGBA)
		settings.setValue('TrackColorRGBA', self.trackColorRGBA)

		settings.setValue('use_2D_contours', self.use_2D_contours)

		settings.endGroup()

		self.classifier.saveSettings(settings)

	#voxels - [slices, rows, cols]
	#spacing - [x, y, z]
	@staticmethod
	def numpy2vtk(voxels, spacing):
		data = voxels.tobytes()

		importer = vtk.vtkImageImport()
		importer.SetDataScalarTypeToUnsignedShort()
		importer.SetNumberOfScalarComponents(1)
		importer.CopyImportVoidPointer(data, len(data))
		importer.SetDataExtent(0, voxels.shape[2]-1, 0, voxels.shape[1]-1, 0, voxels.shape[0]-1)
		importer.SetWholeExtent(0, voxels.shape[2]-1, 0, voxels.shape[1]-1, 0, voxels.shape[0]-1)
		importer.SetDataSpacing(spacing[0], spacing[1], spacing[2])
	                                 
		#warning
		#https://discourse.vtk.org/t/render-single-slice-of-a-volume-with-vtkvolume-shows-no-result/12069
		volumeMapper = vtk.vtkGPUVolumeRayCastMapper() 
		volumeMapper.AutoAdjustSampleDistancesOff()
		volumeMapper.LockSampleDistanceToInputSpacingOn()
		volumeMapper.SetInputConnection(importer.GetOutputPort())

		vtk_volume = vtk.vtkVolume()
		vtk_volume.SetMapper(volumeMapper)

		return vtk_volume

	def __createVolumeProperty(self):
		colorFunc = vtk.vtkColorTransferFunction()
		colorFunc.AddRGBPoint(0, 0.0, 0.0, 0.0)
		colorFunc.AddRGBPoint(255, 1.0, 1.0, 1.0)
		colorFunc.AddRGBPoint(Marker.BRAIN, self.brainColorRGBA[0], self.brainColorRGBA[1], self.brainColorRGBA[2])
		colorFunc.AddRGBPoint(Marker.ISCHEMIA, self.ischemiaColorRGBA[0], self.ischemiaColorRGBA[1], self.ischemiaColorRGBA[2])
		colorFunc.AddRGBPoint(Marker.MSC, self.MSCColorRGBA[0], self.MSCColorRGBA[1], self.MSCColorRGBA[2])
		colorFunc.AddRGBPoint(Marker.REFERENCED_MSC, self.RefMSCColorRGBA[0], self.RefMSCColorRGBA[1], self.RefMSCColorRGBA[2])

		alphaFunc = vtk.vtkPiecewiseFunction()
		alphaFunc.AddPoint(0, 0.00)
		alphaFunc.AddPoint(16, 0.05)
		alphaFunc.AddPoint(128, 0.3)
		alphaFunc.AddPoint(255, 0.6)
		alphaFunc.AddPoint(Marker.BRAIN, self.brainColorRGBA[3])
		alphaFunc.AddPoint(Marker.ISCHEMIA, self.ischemiaColorRGBA[3])
		alphaFunc.AddPoint(Marker.MSC, self.MSCColorRGBA[3])
		alphaFunc.AddPoint(Marker.REFERENCED_MSC, self.RefMSCColorRGBA[3])

		volumeProperty = vtk.vtkVolumeProperty()
		volumeProperty.SetColor(colorFunc)
		volumeProperty.SetScalarOpacity(alphaFunc)
		volumeProperty.SetInterpolationTypeToNearest()
		volumeProperty.ShadeOff()
		volumeProperty.SetAmbient(1.0)
		volumeProperty.SetDiffuse(0.0)
		volumeProperty.SetSpecular(0.0)

		return volumeProperty

	'''-----------------------------------------'''
	@profile
	def scanFolder(self, path):
		dataset = Dataset()
		dataset.scanFolder(path)
		return dataset

	'''-----------------------------------------'''
	def getSpacing(self, entity):
		if entity.protocol == "ep2d_diff_tra_14b": 
			return self.volumeSpacingADC
		if entity.protocol == "swi_tra":
			return self.volumeSpacingSWI
		if entity.protocol == "t2_tse_tra_fs":
			return self.volumeSpacingT2
		return [1,1,1]

	@staticmethod
	def compute_gradient(min_value, max_value, values_count):
		step = (max_value - min_value) / (values_count - 1)
		grad = np.arange(0, values_count) * step + min_value
		return grad

	@staticmethod
	def apply_gradient_correction(a, b):
		result = np.zeros(a.shape, dtype=np.float64)
		a = a.astype(np.float64)
		b = b / 127.5

		low = (a * b[:, np.newaxis])
		high = (255.0 - a) * b[:, np.newaxis] + 2 * a - 255

		low_mask = (a < 127.5)
		high_mask = ~low_mask
		result[low_mask] = low[low_mask]
		result[high_mask] = high[high_mask]
		return result

	@profile
	def __preprocessSliceData(self, source, view_mode):
		if view_mode.flags & (ViewMode.MARK_BRAIN_AREA | ViewMode.MARK_ISCHEMIA_AREA | ViewMode.MARK_MSC_AREA) and type(source.preprocessed) == type(None):
			self.classifier.preprocess(source)

		if view_mode.flags & (ViewMode.MARK_BRAIN_AREA | ViewMode.MARK_ISCHEMIA_AREA | ViewMode.MARK_MSC_AREA) and type(source.brain_mask) == type(None):
			self.classifier.getMask(MaskType.BRAIN, source)
		if view_mode.flags & ViewMode.MARK_ISCHEMIA_AREA and type(source.ischemia_mask) == type(None):
			self.classifier.getMask(MaskType.ISCHEMIA, source)
			source.ischemia_mask = cv2.bitwise_and(source.brain_mask, source.ischemia_mask)
		if view_mode.flags & ViewMode.MARK_MSC_AREA and type(source.msc_mask) == type(None):
			self.classifier.getMask(MaskType.MSC, source)
			source.msc_mask = cv2.bitwise_and(source.brain_mask, source.msc_mask)


	@profile
	def processSlice(self, slice, view_mode):
		source = get_cached_data(slice.filename)
		info = dict()

		self.__preprocessSliceData(source, view_mode)

		spacing = self.getSpacing(slice)
		pixel_area = spacing[0]*spacing[1]

		pixels = cv2.normalize(source.ds.pixel_array, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_8U)
		if view_mode.flags & ViewMode.USE_PREPROCESSED_SOURCE:
			grad = MRIMAProcessor.compute_gradient(self.gradientMinValue, self.gradientMaxValue, pixels.shape[0])
			pixels = MRIMAProcessor.apply_gradient_correction(pixels, grad).astype(np.ubyte)

		rgb_image = cv2.cvtColor(pixels, cv2.COLOR_GRAY2RGB)
		scale_factor = 512.0 / np.float64(pixels.shape[1])
		if self.use_2D_contours == 1:
			rgb_image = cv2.resize(rgb_image, (0, 0), fx=scale_factor, fy=scale_factor, interpolation=cv2.INTER_NEAREST)

		def draw_contours(rgb_image, data, color, thickness_value, scale_factor_val):
			data = ((cv2.resize(data, (0, 0), fx=scale_factor_val, fy=scale_factor_val, interpolation=cv2.INTER_LINEAR) > 127) * 255).astype(np.uint8)
			ct_target, hierarchy_target = cv2.findContours(data, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
			if type(hierarchy_target) != type(None):
				for idx in range(len(hierarchy_target[0])): 
					if hierarchy_target[0][idx][3] == -1: 
						img_contours = cv2.drawContours(np.zeros(data.shape, np.uint8), ct_target, idx, 255, thickness_value)
						rgb_image[(img_contours == 255)] = color
					else: 
						for p in ct_target[idx][::5]: # set gap value
							cv2.circle(rgb_image, p[0], thickness_value, color, -1)

		brain_pixels_count = 0
		if view_mode.flags & ViewMode.MARK_BRAIN_AREA:
			marked = (source.brain_mask == 255)
			brain_pixels_count = np.sum(marked)
			if brain_pixels_count > 0:
				if self.use_2D_contours == 1:
					draw_contours(rgb_image, source.brain_mask, self.brainColorRGB, 2, scale_factor)
				else:
					rgb_image[marked] = self.brainColorRGB
			info["Brain pixels:"] = str(brain_pixels_count) + " ({:.2f} mm\u00b2)".format(brain_pixels_count*pixel_area)

		if view_mode.flags & ViewMode.MARK_ISCHEMIA_AREA:
			marked = (source.ischemia_mask == 255)
			count = np.sum(marked)
			if count > 0:
				if self.use_2D_contours == 1:
					draw_contours(rgb_image, source.ischemia_mask, self.ischemiaColorRGB, 1, scale_factor)
				else:
					rgb_image[marked] = self.ischemiaColorRGB
			info["Ischemia pixels:"] = str(count) + " ({:.2f} mm\u00b2)".format(count*pixel_area) + _percents_str(brain_pixels_count, count)

		if view_mode.flags & ViewMode.MARK_MSC_AREA:
			marked = (source.msc_mask == 255)
			count = np.sum(marked)
			if count > 0:
				if self.use_2D_contours == 1:
					draw_contours(rgb_image, source.msc_mask, self.MSCColorRGB, 1, scale_factor)
				else:
					rgb_image[marked] = self.MSCColorRGB
			info["MSC pixels:"] = str(count) + " ({:.2f} mm\u00b2)".format(count*pixel_area) + _percents_str(brain_pixels_count, count)

		return Image(rgb_image, info)
		
	'''-----------------------------------------'''
	def daysBetween(self, study, ref_study):
		a = datetime.strptime(study.date, '%Y%m%d')
		b = datetime.strptime(ref_study.date, '%Y%m%d')
		delta = a - b
		return abs(delta.days)


	@profile
	def __preprocessStudyData(self, source, referenced, view_mode):
		def filter(it, func):
			for value in it:
				if func(value): yield value

		if view_mode.flags & (ViewMode.MARK_BRAIN_AREA | ViewMode.MARK_ISCHEMIA_AREA | ViewMode.MARK_MSC_AREA | ViewMode.MARK_MSC_FROM | ViewMode.TRACK_MSC_FROM):
			for cached_slice in filter(source, lambda c: type(c.preprocessed) == type(None)):
				self.classifier.preprocess(cached_slice) 
		if view_mode.flags & (ViewMode.MARK_MSC_FROM | ViewMode.TRACK_MSC_FROM):
			for cached_slice in filter(referenced, lambda c: type(c.preprocessed) == type(None)):
				self.classifier.preprocess(cached_slice)

		if view_mode.flags & (ViewMode.MARK_BRAIN_AREA | ViewMode.MARK_ISCHEMIA_AREA | ViewMode.MARK_MSC_AREA | ViewMode.MARK_MSC_FROM | ViewMode.TRACK_MSC_FROM):
			for cached_slice in filter(source, lambda c: type(c.brain_mask) == type(None)):
				self.classifier.getMask(MaskType.BRAIN, cached_slice)
		if view_mode.flags & ViewMode.MARK_ISCHEMIA_AREA:
			for cached_slice in filter(source, lambda c: type(c.ischemia_mask) == type(None)):
				self.classifier.getMask(MaskType.ISCHEMIA, cached_slice)
				cached_slice.ischemia_mask = cv2.bitwise_and(cached_slice.brain_mask, cached_slice.ischemia_mask)
		if view_mode.flags & (ViewMode.MARK_MSC_AREA | ViewMode.TRACK_MSC_FROM):
			for cached_slice in filter(source, lambda c: type(c.msc_mask) == type(None)):
				self.classifier.getMask(MaskType.MSC, cached_slice)
				cached_slice.msc_mask = cv2.bitwise_and(cached_slice.brain_mask, cached_slice.msc_mask)

		if view_mode.flags & (ViewMode.MARK_MSC_FROM | ViewMode.TRACK_MSC_FROM):
			for cached_slice in filter(referenced, lambda c: type(c.brain_mask) == type(None)):
				self.classifier.getMask(MaskType.BRAIN, cached_slice)
			for cached_slice in filter(referenced, lambda c: type(c.msc_mask) == type(None)):
				self.classifier.getMask(MaskType.MSC, cached_slice)
				cached_slice.msc_mask = cv2.bitwise_and(cached_slice.brain_mask, cached_slice.msc_mask)

	@profile
	def processStudy(self, study, view_mode):
		source = [get_cached_data(filename) for filename in study.filename_list]
		referenced = []
		if view_mode.flags & ViewMode.MARK_MSC_FROM:
			referenced = [get_cached_data(filename) for filename in view_mode.ref_msc_study.filename_list]
		if view_mode.flags & ViewMode.TRACK_MSC_FROM:
			referenced = [get_cached_data(filename) for filename in view_mode.ref_tracking_study.filename_list]
		info = dict()

		self.__preprocessStudyData(source, referenced, view_mode)

		study_spacing = self.getSpacing(study)
		voxel_vol = study_spacing[0]*study_spacing[1]*study_spacing[2]

		voxels = np.stack([s.ds.pixel_array for s in source])
		voxels = cv2.normalize(voxels, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_16U)
		if view_mode.flags & ViewMode.USE_PREPROCESSED_SOURCE:
			grad = MRIMAProcessor.compute_gradient(self.gradientMinValue, self.gradientMaxValue, voxels.shape[1])
			for i in range(voxels.shape[0]):
				voxels[i] = MRIMAProcessor.apply_gradient_correction(voxels[i], grad).astype(np.uint16)

		brain_mask = None
		brain_voxels_count = 0
		if view_mode.flags & ViewMode.MARK_BRAIN_AREA:
			brain_mask = np.stack([cached.brain_mask for cached in source])
			marked = (brain_mask == 255)
			voxels[marked] = Marker.BRAIN
			brain_voxels_count = np.sum(marked)
			info["Brain voxels:"] = str(brain_voxels_count) + " ({:.2f} mm\u00b3)".format(brain_voxels_count*voxel_vol)

		if view_mode.flags & ViewMode.MARK_ISCHEMIA_AREA:
			mask = np.stack([cached.ischemia_mask for cached in source])
			marked = (mask == 255)
			voxels[marked] = Marker.ISCHEMIA
			count = np.sum(marked)
			info["Ischemia voxels:"] = str(count) + " ({:.2f} mm\u00b3)".format(count*voxel_vol) + _percents_str(brain_voxels_count, count)

		msc_mask = None
		if view_mode.flags & ViewMode.MARK_MSC_AREA:
			msc_mask = np.stack([cached.msc_mask for cached in source])
			marked = (msc_mask == 255)
			voxels[marked] = Marker.MSC
			count = np.sum(marked)
			info["MSC voxels:"] = str(count) + " ({:.2f} mm\u00b3)".format(count*voxel_vol) + _percents_str(brain_voxels_count, count)

		if view_mode.flags & ViewMode.MARK_MSC_FROM:
			if type(brain_mask) == type(None):
				brain_mask = np.stack([cached.brain_mask for cached in source])
			referenced_brain_mask = np.stack([cached.brain_mask for cached in referenced])
			referenced_msc_mask = np.stack([cached.msc_mask for cached in referenced])
			combiner = VolumeCombiner(brain_mask, study_spacing, referenced_brain_mask, self.getSpacing(view_mode.ref_msc_study))
			transformed_msc_mask = combiner.transform(referenced_msc_mask)
			voxels[transformed_msc_mask == 255] = Marker.REFERENCED_MSC

		transformed_msc_mask = None
		if view_mode.flags & ViewMode.TRACK_MSC_FROM:
			if type(brain_mask) == type(None):
				brain_mask = np.stack([cached.brain_mask for cached in source])
			if type(msc_mask) == type(None):
				msc_mask = np.stack([cached.msc_mask for cached in source])
			referenced_brain_mask = np.stack([cached.brain_mask for cached in referenced])
			referenced_msc_mask = np.stack([cached.msc_mask for cached in referenced])
			combiner = VolumeCombiner(brain_mask, study_spacing, referenced_brain_mask, self.getSpacing(view_mode.ref_tracking_study))
			transformed_msc_mask = combiner.transform(referenced_msc_mask)
			voxels[transformed_msc_mask == 255] = Marker.REFERENCED_MSC

		vtk_volume = MRIMAProcessor.numpy2vtk(voxels, study_spacing)
		vtk_volume.SetProperty(self.volume_property)

		vtk_mesh = Mesh()
		bounds = vtk_volume.GetBounds()
		vtk_mesh.addLine(bounds[0], 0, 0,   bounds[1], 0, 0,   (1,1,1), 1.0) #x -> cols
		vtk_mesh.addLine(0, bounds[2], 0,   0, bounds[3], 0,   (1,1,1), 1.0) #y -> rows
		vtk_mesh.addLine(0, 0, bounds[4],   0, 0, bounds[5],   (1,1,1), 1.0) #z -> slices

		if view_mode.flags & ViewMode.TRACK_MSC_FROM:
			tracks = self.mscTracker.track(transformed_msc_mask, msc_mask, study_spacing)
			tracks_count = len(tracks)
			if tracks_count != 0:
				avg_track_length = 0
				for track in tracks:
					vtk_mesh.addLine(track.start[0], track.start[1], track.start[2],
							 track.end[0], track.end[1], track.end[2],
							 self.trackColorRGBA[0:3], 
							 self.trackColorRGBA[3])
					avg_track_length = avg_track_length + distance(track.start, track.end)
				avg_track_length = avg_track_length / len(tracks)
				days_between = self.daysBetween(study, view_mode.ref_tracking_study)
				info["Avg track length:"] = "{:.2f} mm".format(avg_track_length)
				info["Days between:"] = "{}".format(days_between)
				if days_between != 0:
					info["Avg MSC speed:"] = "{:.2f}".format(avg_track_length / days_between)
				
		return Volume(vtk_volume, vtk_mesh, info)
	

class AsyncMRIMAProcessor(MRIMAProcessor):
	emitContentSignal = QtCore.Signal(object)

	def __init__(self):
		super().__init__()
		self.background_thread = concurrent.futures.ThreadPoolExecutor(max_workers=1)

	'''-----------------------------------------'''
	@QtCore.Slot(str)
	def scanFolder(self, path):
		run_in_threadpool(self.background_thread, self.__emitContentSignal, functools.partial(super().scanFolder, path))

	@QtCore.Slot(object, object)
	def processSlice(self, slice, vm):
		run_in_threadpool(self.background_thread, self.__emitContentSignal, functools.partial(super().processSlice, slice, vm))

	@QtCore.Slot(object, object)
	def processStudy(self, study, vm):
		run_in_threadpool(self.background_thread, self.__emitContentSignal, functools.partial(super().processStudy, study, vm))

	def __emitContentSignal(self, func):
		try:
			self.emitContentSignal.emit(func())
		except Exception as ex:
			import traceback
			print(traceback.format_exc())
			#print("Exception:", ex)
			self.emitContentSignal.emit(ex) #emit exception as result (close app in MRIMAGUI.py)

