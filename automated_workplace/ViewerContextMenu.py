from PySide2 import QtCore, QtGui, QtWidgets

from Protocol import Protocol
from Dataset import Dataset
from Entity import Slice, Study
from ViewMode import ViewMode

class CheckableAction(QtWidgets.QAction):
	actionTriggeredSignal = QtCore.Signal(bool, object)

	def __init__(self, parent, text, obj):
		super().__init__(text, parent)
		self.obj = obj
		self.setCheckable(True)
		self.triggered.connect(self.onTriggered)

	def onTriggered(self, checked):
		self.actionTriggeredSignal.emit(checked, self.obj)

class CheckableActionsMenu(QtWidgets.QMenu):
	actionTriggeredSignal = QtCore.Signal(bool, object)

	def __init__(self, parent, text=None, policy=QtWidgets.QActionGroup.ExclusionPolicy.None_):
		super().__init__(text, parent)
		self.group = QtWidgets.QActionGroup(self)
		self.group.setExclusionPolicy(policy)

	def addAction(self, action):
		action.actionTriggeredSignal.connect(self.onActionTriggered)
		super().addAction(action)
		self.group.addAction(action)

	def addMenu(self, menu):
		menu.actionTriggeredSignal.connect(self.onActionTriggered)
		super().addMenu(menu)

	def actions(self):
		for action in self.group.actions():
			yield action

	@QtCore.Slot(bool, object)
	def onActionTriggered(self, state, obj):
		self.actionTriggeredSignal.emit(state, obj)

class ViewerContextMenu(CheckableActionsMenu):
	actionTriggeredSignal = QtCore.Signal(bool, object)

	def __init__(self, parent):
		super(ViewerContextMenu, self).__init__(parent, text=None)

		self.preprocessing_action = CheckableAction(self, 'Use preprocessed source', (ViewMode.USE_PREPROCESSED_SOURCE, None))
		self.addAction(self.preprocessing_action)

		self.mark_brain_action = CheckableAction(self, 'Mark brain', (ViewMode.MARK_BRAIN_AREA, None))
		self.mark_brain_action.setVisible(False)
		self.addAction(self.mark_brain_action)

		self.mark_ischemia_action = CheckableAction(self, 'Mark ischemia', (ViewMode.MARK_ISCHEMIA_AREA, None))
		self.mark_ischemia_action.setVisible(False)
		self.addAction(self.mark_ischemia_action)

		self.mark_msc_action = CheckableAction(self, 'Mark MSC', (ViewMode.MARK_MSC_AREA, None))
		self.mark_msc_action.setVisible(False)
		self.addAction(self.mark_msc_action)

		self.ref_msc_submenu = CheckableActionsMenu(self, "Mark MSC from", QtWidgets.QActionGroup.ExclusionPolicy.ExclusiveOptional)
		self.ref_msc_submenu.menuAction().setVisible(False)
		self.addMenu(self.ref_msc_submenu)

		self.ref_tracking_submenu = CheckableActionsMenu(self, "Track MSC from", QtWidgets.QActionGroup.ExclusionPolicy.ExclusiveOptional)
		self.ref_tracking_submenu.menuAction().setVisible(False)
		self.addMenu(self.ref_tracking_submenu)

	def clearContent(self):
		self.ref_tracking_submenu.clear()
		self.ref_tracking_submenu.menuAction().setVisible(False)
		self.ref_msc_submenu.clear()
		self.ref_msc_submenu.menuAction().setVisible(False)

	def setContent(self, dataset):
		self.clearContent()

		swi_protocols = [*dataset.getProtocolIndexes(Protocol.SWI)]
		if len(swi_protocols) == 0:
			return

		for swi_protocol_index in swi_protocols:
			for study_index in dataset.studiesWithProtocolIndex(swi_protocol_index):
				study_name = dataset.getStudyName(study_index)
				study = dataset.getStudy(study_index)

				ref_msc_action = CheckableAction(self.ref_msc_submenu, study_name, (ViewMode.MARK_MSC_FROM, study))
				self.ref_msc_submenu.addAction(ref_msc_action)

				ref_tracking_action = CheckableAction(self.ref_tracking_submenu, study_name, (ViewMode.TRACK_MSC_FROM, study))
				self.ref_tracking_submenu.addAction(ref_tracking_action)
		
	def updateContextMenu(self, selected_input):
		if selected_input == None or selected_input.protocol == None:
			self.mark_brain_action.setVisible(False)
			self.mark_ischemia_action.setVisible(False)
			self.mark_msc_action.setVisible(False)
			self.ref_msc_submenu.menuAction().setVisible(False)
			self.ref_tracking_submenu.menuAction().setVisible(False)
		else:
			isSWI = (selected_input.protocol == Protocol.SWI)
			self.mark_brain_action.setVisible(True)
			self.mark_ischemia_action.setVisible(not isSWI)
			self.mark_msc_action.setVisible(isSWI)

			if isinstance(selected_input, Study):
				if len(selected_input.ref_studies_list) == 0:
					self.ref_msc_submenu.menuAction().setVisible(False)
					self.ref_tracking_submenu.menuAction().setVisible(False)
				else:
					self.ref_msc_submenu.menuAction().setVisible(not isSWI)
					self.ref_tracking_submenu.menuAction().setVisible(isSWI)
					if isSWI:
						for action in self.ref_tracking_submenu.actions():
							_, study = action.obj
							action.setVisible(study.index in selected_input.ref_studies_list)
					else:
						for action in self.ref_msc_submenu.actions():
							_, study = action.obj
							action.setVisible(study.index in selected_input.ref_studies_list)

			elif isinstance(selected_input, Slice):
				self.ref_msc_submenu.menuAction().setVisible(False)
				self.ref_tracking_submenu.menuAction().setVisible(False)

	@QtCore.Slot(bool, object)
	def onActionTriggered(self, state, obj):
		self.actionTriggeredSignal.emit(state, obj)
