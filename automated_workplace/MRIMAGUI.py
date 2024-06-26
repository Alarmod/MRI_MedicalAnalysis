from PySide2 import QtCore, QtGui, QtWidgets

from MainWindow import MainWindow
from DatasetViewSubWindow import DatasetViewSubWindow
from ViewerSubWindow import ViewerSubWindow
from InfoSubWindow import InfoSubWindow

from Protocol import Protocol
from Dataset import Dataset
from Entity import *
from ViewMode import ViewMode

class MRIMAGUI(QtCore.QObject):
	scanFolderSignal = QtCore.Signal(str)
	processSliceSignal = QtCore.Signal(object, object)
	processStudySignal = QtCore.Signal(object, object)
	beforeExitSignal = QtCore.Signal()

	def __init__(self, *args, **kwargs):
		super().__init__()
		self.initUI()

		self.selected_input = None
		self.selected_view_mode  = ViewMode()
		self.reset_camera = False
        
	def initUI(self):
		self.mainWindow = MainWindow("MRIMA App") 
		self.mainWindow.scanFolderSignal.connect(self.onScanFolder)
		self.mainWindow.beforeExitSignal.connect(lambda: self.beforeExitSignal.emit())
		self.mainWindow.setStatusBarText("Ready")

		self.infoSubWindow = InfoSubWindow("Info")
		self.mainWindow.addSubwindow(self.infoSubWindow)

		self.viewerSubWindow = ViewerSubWindow("View")
		self.viewerSubWindow.contextMenuActionTriggeredSignal.connect(self.onContextMenuActionTriggered)
		self.mainWindow.addSubwindow(self.viewerSubWindow)

		self.datasetSubWindow = DatasetViewSubWindow("Studies")
		self.datasetSubWindow.inputSelectedSignal.connect(self.onInputSelected)
		self.mainWindow.addSubwindow(self.datasetSubWindow)

	def __updateViewMode(self, state, obj):
		flag, ref_study = obj
		if state == False:
			self.selected_view_mode.dropFlag(flag)
		else:
			if flag == ViewMode.MARK_MSC_FROM or flag == ViewMode.TRACK_MSC_FROM:
				self.selected_view_mode.setFlag(flag, ref_study)
			else:
				self.selected_view_mode.setFlag(flag)

	def __getViewModeForInput(self, view_mode, input_data):
		vm = ViewMode()

		if input_data == None:
			return vm

		if view_mode.flags & ViewMode.USE_PREPROCESSED_SOURCE:
			vm.setFlag(ViewMode.USE_PREPROCESSED_SOURCE)
		if view_mode.flags & ViewMode.MARK_BRAIN_AREA:
			vm.setFlag(ViewMode.MARK_BRAIN_AREA)

		isSWI = (input_data.protocol == Protocol.SWI)
		if view_mode.flags & ViewMode.MARK_ISCHEMIA_AREA and not isSWI:
			vm.setFlag(ViewMode.MARK_ISCHEMIA_AREA)
		if view_mode.flags & ViewMode.MARK_MSC_AREA and isSWI:
			vm.setFlag(ViewMode.MARK_MSC_AREA)

		if isinstance(input_data, Study):
			if view_mode.flags & ViewMode.MARK_MSC_FROM and not isSWI and view_mode.ref_msc_study.index in input_data.ref_studies_list:
				vm.setFlag(ViewMode.MARK_MSC_FROM, view_mode.ref_msc_study)
			if view_mode.flags & ViewMode.TRACK_MSC_FROM and isSWI and view_mode.ref_tracking_study.index in input_data.ref_studies_list:
				vm.setFlag(ViewMode.TRACK_MSC_FROM, view_mode.ref_tracking_study)

		return vm


	@QtCore.Slot(str)
	def onScanFolder(self, path):
		self.__setDisabled(True)
		self.__clearContent()
		self.mainWindow.setStatusBarText("Scanning folder...")

		self.viewerSubWindow.switchTo2DView()
		self.scanFolderSignal.emit(path)

	def __processSelectedInput(self):
		self.__setDisabled(True)
		self.infoSubWindow.clearContent()

		if self.selected_input == None:
			self.viewerSubWindow.clearContent(contextMenu=False)
			self.viewerSubWindow.switchTo2DView()
			self.__setDisabled(False)
			return

		wm = self.__getViewModeForInput(self.selected_view_mode, self.selected_input)
		if isinstance(self.selected_input, Slice):
			self.mainWindow.setStatusBarText("Processing slice...")
			self.processSliceSignal.emit(self.selected_input, wm)
		elif isinstance(self.selected_input, Study):
			self.mainWindow.setStatusBarText("Processing study...")
			self.processStudySignal.emit(self.selected_input, wm)

	@QtCore.Slot(object)
	def onInputSelected(self, obj):
		self.selected_input = obj
		self.viewerSubWindow.updateContextMenu(self.selected_input)
		self.reset_camera = True
		self.__processSelectedInput()

	@QtCore.Slot(bool, object)
	def onContextMenuActionTriggered(self, state, obj):
		self.__updateViewMode(state, obj)
		self.reset_camera = False
		self.__processSelectedInput()

	def loadSettings(self, settings):
		self.mainWindow.loadSettings(settings)
		self.datasetSubWindow.loadSettings(settings)
		self.infoSubWindow.loadSettings(settings)
		self.viewerSubWindow.loadSettings(settings)

	def saveSettings(self, settings):
		self.mainWindow.saveSettings(settings)
		self.datasetSubWindow.saveSettings(settings)
		self.infoSubWindow.saveSettings(settings)
		self.viewerSubWindow.saveSettings(settings)

	def __setDisabled(self, state):
		self.mainWindow.setDisabled(state)
		self.datasetSubWindow.setDisabled(state)
		self.infoSubWindow.setDisabled(state)
		self.viewerSubWindow.setDisabled(state)

	def __clearContent(self, tree=True, info=True, view2d=True, view3d=True, contextMenu=True):
		if tree: self.datasetSubWindow.clearContent()
		if info: self.infoSubWindow.clearContent()
		self.viewerSubWindow.clearContent(view2d, view3d, contextMenu)

	@QtCore.Slot(object)
	def setContent(self, content):
		if isinstance(content, Dataset):
			self.datasetSubWindow.setContent(content)
			self.viewerSubWindow.setContent(content)
			self.selected_input = None
			self.selected_view_mode.dropFlag(ViewMode.MARK_MSC_FROM)
			self.selected_view_mode.dropFlag(ViewMode.TRACK_MSC_FROM)
		if isinstance(content, Image):
			self.viewerSubWindow.setContent(content)
			self.infoSubWindow.setContent(content)
		elif isinstance(content, Volume):
			self.viewerSubWindow.setContent(content, self.reset_camera)
			self.infoSubWindow.setContent(content)
		if isinstance(content, Exception):
			QtWidgets.QApplication.instance().quit() #exit app without confirmation

		self.mainWindow.setStatusBarText("Ready")
		self.__setDisabled(False)

	def show(self):
		self.mainWindow.show()
