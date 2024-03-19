from PySide2 import QtCore, QtGui, QtWidgets

from MDIChildSubWindow import MDIChildSubWindow
from Dataset import Dataset
from Entity import Slice, Study

class TreeWidgetItem(QtWidgets.QTreeWidgetItem):
	def __init__(self, parent, text, obj):
		super(TreeWidgetItem, self).__init__(parent)
		self.obj = obj
		self.setText(0, text)

class TreeWidget(QtWidgets.QTreeWidget):
	def __init__(self, *args, **kwargs):
		super(TreeWidget, self).__init__(*args, **kwargs)
		self.disabled = False

	def keyPressEvent(self, event):
		if self.disabled == False:
			QtWidgets.QTreeWidget.keyPressEvent(self, event)

	def mousePressEvent(self, event):
		if self.disabled == False:
			QtWidgets.QTreeWidget.mousePressEvent(self, event)

	def mouseMoveEvent(self, event):
		if self.disabled == False:
			QtWidgets.QTreeWidget.mouseMoveEvent(self, event)

	def setDisabled(self, state):
		self.disabled = state

class DatasetViewSubWindow(MDIChildSubWindow):
	inputSelectedSignal = QtCore.Signal(object)

	def __init__(self, title):
		super(DatasetViewSubWindow, self).__init__(title)
		self.initUI()

	def initUI(self):
		self.tree = TreeWidget()
		self.tree.header().hide()
		self.setWidget(self.tree)
		self.tree.selectionModel().selectionChanged.connect(self.onSelectionChanged)

	def minimumSizeHint(self):
		return QtCore.QSize(256, 256)

	def loadSettings(self, settings):
		settings.beginGroup("Studies")
		pos = settings.value('pos', QtCore.QPoint(0,0))
		size = settings.value('size', QtCore.QSize(256,256))
		settings.endGroup()
		self.move(pos)
		self.resize(size)

	def saveSettings(self, settings):
		settings.beginGroup("Studies")
		settings.setValue('pos', self.pos())
		settings.setValue('size', self.size())
		settings.endGroup()

	def setDisabled(self, state):
		self.tree.setDisabled(state)

	def setContent(self, dataset):
		for protocol_id in dataset.protocols():
			protocol_node = TreeWidgetItem(self.tree, dataset.getProtocolName(protocol_id), None)
			for study_id in dataset.studiesWithProtocolID(protocol_id):
				study_node = TreeWidgetItem(protocol_node, dataset.getStudyName(study_id), dataset.getStudy(study_id))
				for slice_id in dataset.slicesWithStudyID(study_id):
					slice_node = TreeWidgetItem(study_node, dataset.getSliceName(slice_id), dataset.getSlice(slice_id))

	def clearContent(self):
		self.tree.clear()

	@QtCore.Slot(QtCore.QItemSelection, QtCore.QItemSelection)
	def onSelectionChanged(self, selected, deselected):
		it = self.tree.selectedItems()
		if len(it) != 1: 
			return
		self.inputSelectedSignal.emit(it[0].obj)
