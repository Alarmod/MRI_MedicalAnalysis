import vtk
import numpy as np
import cv2
import asyncio
import qasync
from datetime import datetime

from Dataset import Dataset, loadDICOMFile
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

def run_in_threadpool(pool, func, *args, **kw_args):
	return asyncio.get_running_loop().run_in_executor(pool, func, *args, **kw_args)

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

class MRIMAProcessor:
	def __init__(self):
		self.pool = qasync.QThreadExecutor(1)

		self.gradientMinValue = 100
		self.gradientMaxValue = 255

		self.volumeSpacingADC = [0.40000000596046, 0.40000000596046, 1.05] #row col slice
		self.volumeSpacingSWI = [0.1171875, 0.1171875, 0.5]
		self.volumeSpacingT2 = [0.1171875, 0.1171875, 0.805]

		#3d view
		self.brainColorRGBA = [1.0, 1.0, 0.0, 0.05]
		self.ischemiaColorRGBA = [1.0, 0.0, 0.0, 0.3]
		self.MSCColorRGBA = [0.0, 1.0, 0.0, 1.0]
		self.RefMSCColorRGBA = [0.0, 1.0, 0.0, 0.8]
		self.trackColorRGBA = [0.0, 0.5, 0.0, 0.7]

		#2d view
		self.brainColorRGB = np.array(self.brainColorRGBA[0:3])*255
		self.ischemiaColorRGB = np.array(self.ischemiaColorRGBA[0:3])*255
		self.MSCColorRGB = np.array(self.MSCColorRGBA[0:3])*255

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
	def __scanFolderImpl(self, path):
		dataset = Dataset()
		dataset.scanFolder(path)
		return dataset

	async def scanFolder(self, path):
		return await run_in_threadpool(self.pool, self.__scanFolderImpl, path)

	'''-----------------------------------------'''
	@staticmethod
	def percents_str(total, count):
		if total == 0 or count == 0:
			return ""
		else:
			return " / {:.2f}%".format(count * 100 / total)

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
		rows, cols = a.shape
		result = np.zeros((rows, cols), dtype=np.float64)
		for row in range(rows):
			for col in range(cols):
				if a[row, col] < 127.5:
					result[row, col] = (b[row] * a[row, col])/127.5
				else:
					result[row, col] = b[row] * (255 - a[row, col]) / 127.5 + 2 * a[row, col] - 255
		return result

	@profile
	def __processSliceImpl(self, slice, view_mode):
		source = loadDICOMFile(slice.filename).pixel_array
		info = dict()

		pixels = cv2.normalize(source, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_8U)
		if view_mode.flags & ViewMode.USE_PREPROCESSED_SOURCE:
			grad = MRIMAProcessor.compute_gradient(self.gradientMinValue, self.gradientMaxValue, pixels.shape[0])
			pixels = MRIMAProcessor.apply_gradient_correction(pixels, grad).astype(np.ubyte)

		spacing = self.getSpacing(slice)
		pixel_area = spacing[0]*spacing[1]

		rgb_image = cv2.cvtColor(pixels, cv2.COLOR_GRAY2RGB)

		brain_pixels_count = 0
		if view_mode.flags & ViewMode.MARK_BRAIN_AREA:
			mask = self.classifier.getMask(MaskType.BRAIN, slice.filename)
			filter = (mask == 255)
			rgb_image[filter] = self.brainColorRGB
			brain_pixels_count = np.sum(filter)
			info["Brain pixels:"] = str(brain_pixels_count) + " ({:.2f} mm\u00b2)".format(brain_pixels_count*pixel_area)


		if view_mode.flags & ViewMode.MARK_ISCHEMIA_AREA:
			mask = self.classifier.getMask(MaskType.ISCHEMIA, slice.filename)
			filter = (mask == 255)
			rgb_image[filter] = self.ischemiaColorRGB
			count = np.sum(filter)
			info["Ischemia pixels:"] = str(count) + " ({:.2f} mm\u00b2)".format(count*pixel_area) + MRIMAProcessor.percents_str(brain_pixels_count, count)

		if view_mode.flags & ViewMode.MARK_MSC_AREA:
			mask = self.classifier.getMask(MaskType.MSC, slice.filename)
			filter = (mask == 255)
			rgb_image[filter] = self.MSCColorRGB
			count = np.sum(filter)
			info["MSC pixels:"] = str(count) + " ({:.2f} mm\u00b2)".format(count*pixel_area) + MRIMAProcessor.percents_str(brain_pixels_count, count)

		return Image(rgb_image, info)
		
	async def processSlice(self, slice, view_mode):
		return await run_in_threadpool(self.pool, self.__processSliceImpl, slice, view_mode)

	'''-----------------------------------------'''
	def daysBetween(self, study, ref_study):
		a = datetime.strptime(study.date, '%Y%m%d')
		b = datetime.strptime(ref_study.date, '%Y%m%d')
		delta = a - b
		return abs(delta.days)

	@profile
	def __processStudyImpl(self, study, view_mode):
		source = np.stack([loadDICOMFile(filename).pixel_array for filename in study.filename_list])
		info = dict()

		study_spacing = self.getSpacing(study)
		voxel_vol = study_spacing[0]*study_spacing[1]*study_spacing[2]
		
		voxels = cv2.normalize(source, None, 0, 255, cv2.NORM_MINMAX, dtype=cv2.CV_16U)
		if view_mode.flags & ViewMode.USE_PREPROCESSED_SOURCE:
			grad = MRIMAProcessor.compute_gradient(self.gradientMinValue, self.gradientMaxValue, voxels.shape[1])
			for i in range(voxels.shape[0]):
				voxels[i] = MRIMAProcessor.apply_gradient_correction(voxels[i], grad).astype(np.uint16)

		brain_mask = None
		brain_voxels_count = 0
		if view_mode.flags & ViewMode.MARK_BRAIN_AREA:
			brain_mask = np.stack([self.classifier.getMask(MaskType.BRAIN, filename) for filename in study.filename_list])
			filter = (brain_mask == 255)
			voxels[filter] = Marker.BRAIN
			brain_voxels_count = np.sum(filter)
			info["Brain voxels:"] = str(brain_voxels_count) + " ({:.2f} mm\u00b3)".format(brain_voxels_count*voxel_vol)

		if view_mode.flags & ViewMode.MARK_ISCHEMIA_AREA:
			mask = np.stack([self.classifier.getMask(MaskType.ISCHEMIA, filename) for filename in study.filename_list])
			filter = (mask == 255)
			voxels[filter] = Marker.ISCHEMIA
			count = np.sum(filter)
			info["Ischemia voxels:"] = str(count) + " ({:.2f} mm\u00b3)".format(count*voxel_vol) + MRIMAProcessor.percents_str(brain_voxels_count, count)

		msc_mask = None
		if view_mode.flags & ViewMode.MARK_MSC_AREA:
			msc_mask = np.stack([self.classifier.getMask(MaskType.MSC, filename) for filename in study.filename_list])
			filter = (msc_mask == 255)
			voxels[filter] = Marker.MSC
			count = np.sum(filter)
			info["MSC voxels:"] = str(count) + " ({:.2f} mm\u00b3)".format(count*voxel_vol) + MRIMAProcessor.percents_str(brain_voxels_count, count)

		if view_mode.flags & ViewMode.MARK_MSC_FROM:
			if type(brain_mask) == type(None):
				brain_mask = np.stack([self.classifier.getMask(MaskType.BRAIN, filename) for filename in study.filename_list])
			referenced_brain_mask = np.stack([self.classifier.getMask(MaskType.BRAIN, filename) for filename in view_mode.ref_msc_study.filename_list])
			combiner = VolumeCombiner(brain_mask, study_spacing, referenced_brain_mask, self.getSpacing(view_mode.ref_msc_study))
			referenced_msc_mask = np.stack([self.classifier.getMask(MaskType.MSC, filename) for filename in view_mode.ref_msc_study.filename_list])
			transformed_msc_mask = combiner.transform(referenced_msc_mask)
			voxels[transformed_msc_mask == 255] = Marker.REFERENCED_MSC

		transformed_msc_mask = None
		if view_mode.flags & ViewMode.TRACK_MSC_FROM:
			if type(brain_mask) == type(None):
				brain_mask = np.stack([self.classifier.getMask(MaskType.BRAIN, filename) for filename in study.filename_list])
			referenced_brain_mask = np.stack([self.classifier.getMask(MaskType.BRAIN, filename) for filename in view_mode.ref_tracking_study.filename_list])
			combiner = VolumeCombiner(brain_mask, study_spacing, referenced_brain_mask, self.getSpacing(view_mode.ref_tracking_study))
			if type(msc_mask) == type(None):
				msc_mask = np.stack([self.classifier.getMask(MaskType.MSC, filename) for filename in study.filename_list])
			referenced_msc_mask = np.stack([self.classifier.getMask(MaskType.MSC, filename) for filename in view_mode.ref_tracking_study.filename_list])
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
		
	async def processStudy(self, study, view_mode):
		return await run_in_threadpool(self.pool, self.__processStudyImpl, study, view_mode)