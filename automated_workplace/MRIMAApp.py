from PySide2 import QtCore, QtGui, QtWidgets
import asyncio
import qasync

from Entity import *
from MRIMAGUI import MRIMAGUI
from ViewMode import ViewMode
from MRIMAProcessor import MRIMAProcessor

class MRIMAApp(QtWidgets.QApplication):
	def __init__(self, args):
		super(MRIMAApp, self).__init__(args)

		self.settings = QtCore.QSettings("settings.ini", QtCore.QSettings.IniFormat)

		self.processor = MRIMAProcessor()
		self.processor.loadSettings(self.settings)

		self.gui = MRIMAGUI()
		self.gui.loadSettings(self.settings)

		self.gui.scanFolderSignal.connect(self.onScanFolder)
		self.gui.processSliceSignal.connect(self.onProcessSlice)
		self.gui.processStudySignal.connect(self.onProcessStudy)
		self.gui.beforeExitSignal.connect(self.onBeforeExit)

		self.gui.show()

	@qasync.asyncSlot(str)
	async def onScanFolder(self, path):
		self.gui.setContent(await self.processor.scanFolder(path))

	@qasync.asyncSlot(object, object)
	async def onProcessSlice(self, slice, view_mode):
		self.gui.setContent(await self.processor.processSlice(slice, view_mode))

	@qasync.asyncSlot(object, object, bool)
	async def onProcessStudy(self, study, view_mode, reset_camera):
		self.gui.setContent(await self.processor.processStudy(study, view_mode), reset_camera)

	@QtCore.Slot()
	def onBeforeExit(self):
		self.gui.saveSettings(self.settings)
		self.processor.saveSettings(self.settings)
