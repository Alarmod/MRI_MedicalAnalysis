from PySide2 import QtCore, QtGui, QtWidgets
import vtk

from VTKWidgets import VTKInteractiveWidget
from Entity import Volume

class View3DWidget(VTKInteractiveWidget):
	def __init__(self, parent=None, **kw):
		VTKInteractiveWidget.__init__(self, parent, **kw)

		self.__renderer = vtk.vtkRenderer()
		self.__renderer.GetActiveCamera().Azimuth(180)
		self.__renderer.GetActiveCamera().Roll(180)
		self.getRenderWindow().AddRenderer(self.__renderer)

	def setBackgroundColor(self, color):
		self.__renderer.SetBackground(color[0], color[1], color[2])

	def setDisabled(self, state):
		if state == True:
			self.getInteractor().Disable()
		else:
			self.getInteractor().Enable()

	def setContent(self, content):
		self.__renderer.RemoveAllViewProps()
		if (isinstance(content, Volume)):
			self.__renderer.AddVolume(content.vtk_volume)
			self.__renderer.AddActor(content.vtk_mesh)
		self.update()

	def clearContent(self):
		self.__renderer.RemoveAllViewProps()
		self.update()

	def resetCamera(self):
		self.__renderer.ResetCamera()
		self.update()
