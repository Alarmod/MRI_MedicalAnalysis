from PySide2 import QtCore, QtGui, QtWidgets

#? ,Возможно это лишняя сущность, слишком мало функциональной нагрузки
class MDIChildSubWindow(QtWidgets.QMdiSubWindow):
	def __init__(self, title):
		super(MDIChildSubWindow, self).__init__()
		self.setAttribute(QtCore.Qt.WA_DeleteOnClose, True)
		self.setWindowTitle(title)
