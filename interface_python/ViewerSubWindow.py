from PySide2 import QtCore, QtGui, QtWidgets

from MDIChildSubWindow import MDIChildSubWindow

from View2DWidget import View2DWidget
from View3DWidget import View3DWidget
from ViewerContextMenu import ViewerContextMenu

from Dataset import Dataset
from Entity import Image, Volume

class ViewerSubWindow(MDIChildSubWindow):
	contextMenuActionTriggeredSignal = QtCore.Signal(bool, object)

	def __init__(self, title):
		super(ViewerSubWindow, self).__init__(title)
		self.initUI()
		self.contextMenuDisabled = False

	def initUI(self):
		self.views = QtWidgets.QStackedWidget()
		self.view2d_widget = View2DWidget()
		self.view2d_index = self.views.addWidget(self.view2d_widget)
		self.view3d_widget = View3DWidget()
		self.view3d_index = self.views.addWidget(self.view3d_widget)
		self.setWidget(self.views)

		self.contextMenu = ViewerContextMenu(self)
		self.contextMenu.actionTriggeredSignal.connect(self.onContextMenuActionTriggered)

		self.view3DBackgroundColorRGB = [0,0,0]

	def minimumSizeHint(self):
		return QtCore.QSize(256, 256)

	def switchTo2DView(self):
		self.views.setCurrentIndex(self.view2d_index)
		
	def switchTo3DView(self):
		self.views.setCurrentIndex(self.view3d_index)

	def loadSettings(self, settings):
		settings.beginGroup("Viewer")
		pos = settings.value('pos', QtCore.QPoint(0,0))
		size = settings.value('size', QtCore.QSize(256,256))
		self.view3DBackgroundColorRGB = list(map(float, settings.value('View3DBackgroundColorRGB', self.view3DBackgroundColorRGB)))
		settings.endGroup()

		self.move(pos)
		self.resize(size)
		self.view3d_widget.setBackgroundColor(self.view3DBackgroundColorRGB)

	def saveSettings(self, settings):
		settings.beginGroup("Viewer")
		settings.setValue('pos', self.pos())
		settings.setValue('size', self.size())
		settings.setValue('View3DBackgroundColorRGB', self.view3DBackgroundColorRGB)
		settings.endGroup()

	def setDisabled(self, state):
		#self.view2d_widget.setDisabled(state)
		self.view3d_widget.setDisabled(state)
		self.contextMenuDisabled = state

	def setContent(self, content, reset_camera=False):
		if isinstance(content, Dataset):
			self.contextMenu.setContent(content)
		if isinstance(content, Image):
			self.view2d_widget.setContent(content)
			self.switchTo2DView()
		elif isinstance(content, Volume):
			self.view3d_widget.setContent(content)
			if reset_camera: self.view3d_widget.resetCamera()
			self.switchTo3DView()

	def updateContextMenu(self, selected_input):
		self.contextMenu.updateContextMenu(selected_input)

	def clearContent(self, view2d=True, view3d=True, contextMenu=True):
		if view2d: self.view2d_widget.clearContent()
		if view3d: self.view3d_widget.clearContent()
		if contextMenu: self.contextMenu.clearContent()

	def contextMenuEvent(self, event):
		if self.contextMenuDisabled == False:
			self.contextMenu.exec_(event.globalPos())

	@QtCore.Slot(bool, object)
	def onContextMenuActionTriggered(self, state, obj):
		self.contextMenuActionTriggeredSignal.emit(state, obj)
