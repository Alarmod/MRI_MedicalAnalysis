from PySide2 import QtCore, QtGui, QtWidgets

from Protocol import Protocol
from Dataset import Dataset
from Cache import get_cached_data
from ViewMode import ViewMode
from MRIMAProcessor import MRIMAProcessor
from Profiler import Profiler

def test(test_message, processor, dataset, protocol_id, view_mode):
	print(test_message)

	Profiler.getInstance().clear()
	get_cached_data.cache_clear()

	for study_index in dataset.studiesWithProtocolIndex(dataset.getProtocolIndex(protocol_id)):
		study = dataset.getStudy(study_index)
		_ = processor.processStudy(study, view_mode)

	Profiler.getInstance().print()

if __name__ == '__main__':
	settings = QtCore.QSettings("settings.ini", QtCore.QSettings.IniFormat)

	processor = MRIMAProcessor()

	processor.loadSettings(settings)

	dataset = processor.scanFolder("./data")

	Profiler.getInstance().print()

	view_mode_brain = ViewMode()
	view_mode_brain.setFlag(ViewMode.MARK_BRAIN_AREA)

	view_mode_brain_ischemia = ViewMode()
	view_mode_brain_ischemia.setFlag(ViewMode.MARK_BRAIN_AREA)
	view_mode_brain_ischemia.setFlag(ViewMode.MARK_ISCHEMIA_AREA)

	view_mode_brain_msc = ViewMode()
	view_mode_brain_msc.setFlag(ViewMode.MARK_BRAIN_AREA)
	view_mode_brain_msc.setFlag(ViewMode.MARK_MSC_AREA)

	test("Testing: ADC brain", processor, dataset, Protocol.ADC, view_mode_brain)
	test("Testing: T2 brain", processor, dataset, Protocol.T2, view_mode_brain)
	test("Testing: SWI brain", processor, dataset, Protocol.SWI, view_mode_brain)

	test("Testing: ADC brain & ischemia", processor, dataset, Protocol.ADC, view_mode_brain_ischemia)
	test("Testing: T2 brain & ischemia", processor, dataset, Protocol.T2, view_mode_brain_ischemia)
	test("Testing: SWI brain & msc", processor, dataset, Protocol.SWI, view_mode_brain_msc)
