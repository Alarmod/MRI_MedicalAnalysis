from PySide2 import QtCore, QtGui, QtWidgets
from Entity import Image

class View2DWidget(QtWidgets.QLabel):
	def __init__(self, parent=None):
		super().__init__(parent=parent)
		self.setAlignment(QtCore.Qt.AlignCenter)
		self.currentImage = None

	def __setPixmap(self, size):
		shape = self.currentImage.shape
		qimage = QtGui.QImage(self.currentImage, shape[1], shape[0], 3 * shape[1], QtGui.QImage.Format_RGB888)
		self.setPixmap(QtGui.QPixmap(qimage).scaled(size, QtCore.Qt.KeepAspectRatio, QtCore.Qt.FastTransformation))

	def setContent(self, image):
		self.currentImage = image.rgb_image
		if type(self.currentImage) != type(None):
			self.__setPixmap(self.size())
		else:
			super().clear()

	def clearContent(self):
		self.currentImage = None
		super().clear()

	def resizeEvent(self, event : QtGui.QResizeEvent): 
		if type(self.currentImage) != type(None):
			self.__setPixmap(event.size())
		super().resizeEvent(event)
