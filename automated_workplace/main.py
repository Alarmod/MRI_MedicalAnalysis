from PySide2 import QtCore, QtGui, QtWidgets
import asyncio
import qasync
import sys

from MRIMAProcessor import MRIMAProcessor
from MRIMAGUI import MRIMAGUI

class MRIMAApp:
	def __init__(self):
		#load settings
		self.settings = QtCore.QSettings("settings.ini", QtCore.QSettings.IniFormat)

		#create data processor
		self.processor = MRIMAProcessor()
		self.processor.loadSettings(self.settings)

		#create GUI
		self.gui = MRIMAGUI()
		self.gui.loadSettings(self.settings)

		#connect GUI siugnals
		self.gui.scanFolderSignal.connect(self.onScanFolder)
		self.gui.processSliceSignal.connect(self.onProcessSlice)
		self.gui.processStudySignal.connect(self.onProcessStudy)
		self.gui.beforeExitSignal.connect(self.onBeforeExit)

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
		self.processor.shutdownThreadPool()

	def showGUI(self):
		self.gui.show()
		
if __name__ == '__main__':
	#create Qt app
	qtApp = QtWidgets.QApplication(sys.argv)

	app = MRIMAApp()
	app.showGUI()

	#prepare & start event loop
	event_loop = qasync.QEventLoop(qtApp)
	asyncio.set_event_loop(event_loop)

	app_close_event = asyncio.Event()
	qtApp.aboutToQuit.connect(app_close_event.set)

	event_loop.run_until_complete(app_close_event.wait())
	event_loop.close()

	#debug
	from Profiler import Profiler
	Profiler.getInstance().print()

