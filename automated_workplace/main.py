from PySide2 import QtCore, QtGui, QtWidgets
import sys

from MRIMAProcessor import AsyncMRIMAProcessor
from MRIMAGUI import MRIMAGUI

class MRIMAApp:
	def __init__(self):
		self.settings = QtCore.QSettings("settings.ini", QtCore.QSettings.IniFormat)

		self.processor = AsyncMRIMAProcessor()
		self.processor.loadSettings(self.settings)

		self.gui = MRIMAGUI()
		self.gui.loadSettings(self.settings)

		self.processor.emitContentSignal.connect(self.gui.setContent)

		self.gui.scanFolderSignal.connect(self.processor.scanFolder)
		self.gui.processSliceSignal.connect(self.processor.processSlice)
		self.gui.processStudySignal.connect(self.processor.processStudy)
		self.gui.beforeExitSignal.connect(self.onBeforeExit)

	@QtCore.Slot()
	def onBeforeExit(self):
		self.gui.saveSettings(self.settings)
		self.processor.saveSettings(self.settings)

	def showGUI(self):
		self.gui.show()

if __name__ == '__main__':
	qtApp = QtWidgets.QApplication(sys.argv)

	try:
		app = MRIMAApp()
		app.showGUI()
	except Exception as ex:
		import traceback
		print(traceback.format_exc())
		#print("Exception:", ex)
		sys.exit()

	qtApp.exec_()

	#debug
	from Profiler import Profiler
	Profiler.getInstance().print()
