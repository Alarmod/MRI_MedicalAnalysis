from PySide2 import QtCore, QtGui, QtWidgets

from MDIChildSubWindow import MDIChildSubWindow
from Entity import Image, Volume

def clearLayout(layout):
	if layout is not None:
		while layout.count():
			item = layout.takeAt(0)
			widget = item.widget()
			if widget is not None:
				widget.deleteLater()
			else:
				clearLayout(item.layout())

class InfoSubWindow(MDIChildSubWindow):
	def __init__(self, title):
		super(InfoSubWindow, self).__init__(title)
		self.initUI()

	def initUI(self):
		self.grid = QtWidgets.QGridLayout()
		self.setLayout(self.grid)

		self.scroll_wrapper = QtWidgets.QWidget()
		self.scroll_wrapper.setLayout(self.grid)

		self.scroll = QtWidgets.QScrollArea()
		self.scroll.setWidget(self.scroll_wrapper)
		self.scroll.setWidgetResizable(True)

		self.setWidget(self.scroll)

	def minimumSizeHint(self):
		return QtCore.QSize(256, 128)

	def loadSettings(self, settings):
		settings.beginGroup("Info")
		pos = settings.value('pos', QtCore.QPoint(0,0))
		size = settings.value('size', QtCore.QSize(256,256))
		settings.endGroup()
		self.move(pos)
		self.resize(size)

	def saveSettings(self, settings):
		settings.beginGroup("Info")
		settings.setValue('pos', self.pos())
		settings.setValue('size', self.size())
		settings.endGroup()

	def clearContent(self):
		clearLayout(self.grid)

	def setContent(self, entity):
		if isinstance(entity, Image) or isinstance(entity, Volume):
			if entity.info == None:
				return
			i = 0
			for name, value in entity.info.items():
				self.grid.addWidget(QtWidgets.QLabel(str(name)), i, 0, alignment=QtCore.Qt.AlignCenter)
				self.grid.addWidget(QtWidgets.QLabel(str(value)), i, 1, alignment=QtCore.Qt.AlignCenter)
				i = i + 1
