from PySide2 import QtCore, QtGui, QtWidgets

class MainWindow(QtWidgets.QMainWindow):
	class StatusBarEventFilter(QtCore.QObject):
		def eventFilter(self, watched: QtCore.QObject, event: QtCore.QEvent) -> bool:
			if isinstance(event, QtGui.QStatusTipEvent):
				return True
			return super().eventFilter(watched, event)

	scanFolderSignal = QtCore.Signal(str)
	beforeExitSignal = QtCore.Signal()

	def __init__(self, title, *args, **kwargs):
		super(MainWindow, self).__init__(*args, **kwargs)
		self.setWindowTitle(title)
		self.initUI()
        
	def initUI(self):
		self.__createMenuBar()
		self.__createMDIArea()

	def __createMenuBar(self):
		self.__scanFolderAction = QtWidgets.QAction('Scan Folder...', self)
		self.__scanFolderAction.triggered.connect(self.__onScanFolder)

		exitAction = QtWidgets.QAction('&Exit', self)
		exitAction.triggered.connect(self.close)

		menubar = self.menuBar()
		fileMenu = menubar.addMenu('&File')
		fileMenu.addAction(self.__scanFolderAction)
		fileMenu.addSeparator()
		fileMenu.addAction(exitAction)

		menubar.installEventFilter(MainWindow.StatusBarEventFilter(self))

	def __createMDIArea(self):
		self.__mdiArea = QtWidgets.QMdiArea()
		self.__mdiArea.setHorizontalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
		self.__mdiArea.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAsNeeded)
		self.setCentralWidget(self.__mdiArea)

	def addSubwindow(self, subwindow):
		self.__mdiArea.addSubWindow(subwindow)
		subwindow.setWindowFlags(QtCore.Qt.WindowTitleHint)

	def setStatusBarText(self, text):
		self.statusBar().showMessage(text)

	def __center(self):
		centerPoint = QtGui.QScreen.availableGeometry(QtWidgets.QApplication.primaryScreen()).center()
		fg = self.frameGeometry()
		fg.moveCenter(centerPoint)
		return fg.topLeft()

	def setDisabled(self, state):
		self.__scanFolderAction.setDisabled(state)

	def __onScanFolder(self):
		path = QtWidgets.QFileDialog.getExistingDirectory(parent=None, directory=QtCore.QDir.currentPath())
		if path != "":
			self.scanFolderSignal.emit(path)

	def loadSettings(self, settings):
		settings.beginGroup("MainWindow")
		pos = settings.value('pos', self.__center())
		size = settings.value('size', QtCore.QSize(1024, 768))
		settings.endGroup()
		self.move(pos)
		self.resize(size)

	def saveSettings(self, settings):
		settings.beginGroup("MainWindow")
		settings.setValue('pos', self.pos())
		settings.setValue('size', self.size())
		settings.endGroup()

	def closeEvent(self, event):
		reply = QtWidgets.QMessageBox.question(self, 'Message', "Are you sure to quit?", QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No, QtWidgets.QMessageBox.No)
		if reply == QtWidgets.QMessageBox.Yes:
			self.beforeExitSignal.emit()
			self.__mdiArea.closeAllSubWindows()
			event.accept()
		else:
			event.ignore()
