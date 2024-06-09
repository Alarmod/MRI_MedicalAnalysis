from PySide2 import QtCore, QtGui, QtWidgets

from MRIMAProcessor import MRIMAProcessor
from ViewMode import ViewMode
from Cache import get_cached_data
from Profiler import Profiler

def test(test_message, processor, dataset, study_protocol, view_mode):
	print(test_message)

	Profiler.getInstance().clear()
	get_cached_data.cache_clear()

	for study_id in dataset.studiesWithProtocolName(study_protocol):
		study = dataset.getStudy(study_id)
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

	test("Testing: ADC brain", processor, dataset, "ep2d_diff_tra_14b", view_mode_brain)
	test("Testing: T2 brain", processor, dataset, "t2_tse_tra_fs", view_mode_brain)
	test("Testing: SWI brain", processor, dataset, "swi_tra", view_mode_brain)

	test("Testing: ADC brain & ischemia", processor, dataset, "ep2d_diff_tra_14b", view_mode_brain_ischemia)
	test("Testing: T2 brain & ischemia", processor, dataset, "t2_tse_tra_fs", view_mode_brain_ischemia)
	test("Testing: SWI brain & msc", processor, dataset, "swi_tra", view_mode_brain_msc)
