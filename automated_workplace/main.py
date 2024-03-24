from PySide2 import QtCore, QtGui, QtWidgets
import asyncio
import qasync
import sys

from MRIMAProcessor import MRIMAProcessor
from MRIMAGUI import MRIMAGUI

def run_in_threadpool(executor, func, *args, **kw_args):
	return asyncio.get_running_loop().run_in_executor(executor, func, *args, **kw_args)

class MRIMAApp:
	def __init__(self):
		self.settings = QtCore.QSettings("settings.ini", QtCore.QSettings.IniFormat)

		self.thread_pool = qasync.QThreadExecutor(1)

		self.processor = MRIMAProcessor()
		self.processor.loadSettings(self.settings)

		self.gui = MRIMAGUI()
		self.gui.loadSettings(self.settings)

		self.gui.scanFolderSignal.connect(self.onScanFolder)
		self.gui.processSliceSignal.connect(self.onProcessSlice)
		self.gui.processStudySignal.connect(self.onProcessStudy)
		self.gui.beforeExitSignal.connect(self.onBeforeExit)

	@qasync.asyncSlot(str)
	async def onScanFolder(self, path):
		content = await run_in_threadpool(self.thread_pool, self.processor.scanFolder, path)
		self.gui.setContent(content)

	@qasync.asyncSlot(object, object)
	async def onProcessSlice(self, slice, view_mode):
		content = await run_in_threadpool(self.thread_pool, self.processor.processSlice, slice, view_mode)
		self.gui.setContent(content)

	@qasync.asyncSlot(object, object, bool)
	async def onProcessStudy(self, study, view_mode, reset_camera):
		content = await run_in_threadpool(self.thread_pool, self.processor.processStudy, study, view_mode)
		self.gui.setContent(content, reset_camera)

	@QtCore.Slot()
	def onBeforeExit(self):
		self.gui.saveSettings(self.settings)
		self.processor.saveSettings(self.settings)
		self.thread_pool.shutdown(wait=True)

	def showGUI(self):
		self.gui.show()

if __name__ == '__main__':
	qtApp = QtWidgets.QApplication(sys.argv)

	app = MRIMAApp()
	app.showGUI()

	event_loop = qasync.QEventLoop(qtApp)
	asyncio.set_event_loop(event_loop)

	app_close_event = asyncio.Event()
	qtApp.aboutToQuit.connect(app_close_event.set)

	event_loop.run_until_complete(app_close_event.wait())
	event_loop.close()

	#debug
	from Profiler import Profiler
	Profiler.getInstance().print()

