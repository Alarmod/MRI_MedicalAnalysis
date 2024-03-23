from PySide2 import QtCore, QtGui, QtWidgets
import vtk

class VTKWidget(QtWidgets.QWidget):
	def __init__(self, parent=None, **kw):
		super().__init__(parent, **kw)

		self.setSizePolicy(QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding))
		self.setAttribute(QtCore.Qt.WidgetAttribute.WA_OpaquePaintEvent)
		self.setAttribute(QtCore.Qt.WidgetAttribute.WA_PaintOnScreen)

		self.__renderWindow = vtk.vtkRenderWindow()
		self.__renderWindow.SetWindowInfo(str(int(self.winId())))

		if self.parent():
			self.parent().destroyed.connect(self.close, QtCore.Qt.ConnectionType.DirectConnection)

	def getRenderWindow(self):
		return self.__renderWindow

	def start(self):
		self.__renderWindow.Start()

	def paintEngine(self):
		return None

	def paintEvent(self, event):
		self.__renderWindow.Render()

	def resizeEvent(self, event):
		self.__renderWindow.SetSize(self.width(), self.height())
		self.update()

	def closeEvent(self, event):
		self.__renderWindow.Finalize()

class VTKInteractiveWidget(VTKWidget):
	def __init__(self, parent=None, **kw):
		super().__init__(parent, **kw)

		self.__interactor = vtk.vtkGenericRenderWindowInteractor()
		self.__interactor.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())
		self.__interactor.SetRenderWindow(self.getRenderWindow())

		self.__activeButton = None
		self.__wheelDelta = 0

	def getInteractor(self):
		return self.__interactor

	#def start(self):
	#	self.__interactor.Start()

	def resizeEvent(self, event):
		self.__interactor.SetSize(self.width(), self.height())
		self.__interactor.ConfigureEvent()
		super().resizeEvent(event)

	def __setEventInformation(self, x, y, ctrl=0, shift=0, key='\0', repeat=0, keysum=None):
		self.__interactor.SetEventInformation(x, self.height()-y-1, ctrl, shift, key, repeat, keysum)

	def mousePressEvent(self, event):
		self.__activeButton = event.button()
		if self.__activeButton == QtCore.Qt.LeftButton:
			self.__setEventInformation(event.x(), event.y())
			self.__interactor.LeftButtonPressEvent()

	def mouseMoveEvent(self, event):
		if self.__activeButton == QtCore.Qt.LeftButton:
			self.__setEventInformation(event.x(), event.y())
			self.__interactor.MouseMoveEvent()

	def mouseReleaseEvent(self, event):
		if self.__activeButton == QtCore.Qt.LeftButton:
			self.__setEventInformation(event.x(), event.y())
			self.__interactor.LeftButtonReleaseEvent()
			self.__ActiveButton = None

	def wheelEvent(self, event):
		self.__wheelDelta += event.delta()

		if self.__wheelDelta >= 120:
			self.__interactor.MouseWheelForwardEvent()
			self.__wheelDelta = 0
		elif self.__wheelDelta <= -120:
			self.__interactor.MouseWheelBackwardEvent()
			self.__wheelDelta = 0
