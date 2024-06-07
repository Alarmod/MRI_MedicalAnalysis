from PySide2 import QtCore, QtGui, QtWidgets

from MRIMAProcessor import MRIMAProcessor
from ViewMode import ViewMode
from Cache import get_cached_data
from Profiler import Profiler

#функция тестирования
def test(test_message, processor, dataset, study_protocol, view_mode):
	print(test_message)

	#дропнем всю статистику
	Profiler.getInstance().clear()
	#дропнем все закешированные данные (будет читать файлы с диска)
	get_cached_data.cache_clear()

	#для всех исследований имеющих указанный протокол
	for study_id in dataset.studiesWithProtocolName(study_protocol):
		#получим информацию об исследовании
		study = dataset.getStudy(study_id)
		#вызовем обработку (аналогично тому что делается в связке GUI<->AsyncMRIMAProcessor)
		_ = processor.processStudy(study, view_mode)

	#напечатаем статистику
	Profiler.getInstance().print()
		
	#и тут тоже все почистим
	Profiler.getInstance().clear()
	get_cached_data.cache_clear()

if __name__ == '__main__':
	#загрузим файл с настройками
	settings = QtCore.QSettings("settings.ini", QtCore.QSettings.IniFormat)

	#создадим processor (используем синхронное АПИ, работает чуть медленнее чем вариант в интерфейсе, скорее всего, из за того что я полностью очищаю кэш)
	processor = MRIMAProcessor()

	#проинициализируем его через конфиг
	processor.loadSettings(settings)

	#запросим у процесора dataset
	dataset = processor.scanFolder("./data")

	#напечатаем статистику про scanFolder
	Profiler.getInstance().print()

	#режимы отображения (набор флагов должен быть корректный, этим занимается GUI, всего "базовых" режимов у нас три)
	view_mode_brain = ViewMode() #получать только маску мозга
	view_mode_brain.setFlag(ViewMode.MARK_BRAIN_AREA)

	view_mode_brain_ischemia = ViewMode() #получать маску мозга и маску ишемии
	view_mode_brain_ischemia.setFlag(ViewMode.MARK_BRAIN_AREA)
	view_mode_brain_ischemia.setFlag(ViewMode.MARK_ISCHEMIA_AREA)

	view_mode_brain_msc = ViewMode() #получать маску мозга и маску скоплений МСК
	view_mode_brain_msc.setFlag(ViewMode.MARK_BRAIN_AREA)
	view_mode_brain_msc.setFlag(ViewMode.MARK_MSC_AREA)

	#протестируем на всех доступных тестовых исследованиях с заданным протоколом и режимом
	test("Testing: ADC brain", processor, dataset, "ep2d_diff_tra_14b", view_mode_brain)
	test("Testing: T2 brain", processor, dataset, "t2_tse_tra_fs", view_mode_brain)
	test("Testing: SWI brain", processor, dataset, "swi_tra", view_mode_brain)

	test("Testing: ADC brain & ischemia", processor, dataset, "ep2d_diff_tra_14b", view_mode_brain_ischemia)
	test("Testing: T2 brain & ischemia", processor, dataset, "t2_tse_tra_fs", view_mode_brain_ischemia)
	test("Testing: SWI brain & msc", processor, dataset, "swi_tra", view_mode_brain_msc)
