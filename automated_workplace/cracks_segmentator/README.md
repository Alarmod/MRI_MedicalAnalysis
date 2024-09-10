# Description of data and launch order:

- _00_train.py_ - script for training the neural network that performing segmentation, training time is about 3.5 hours on Nvidia GTX 3060

- _01_convert_settings.bat_ - script for converting neural network settings to ONNX format

- _02_test.py_ - script for testing the neural network using the [../yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind) module

- _data.yaml_ - file with a description of the dataset that used for training and testing

Based on [Crack-Seg dataset](https://docs.ultralytics.com/datasets/segment/crack-seg/) that has been [slightly modified](https://github.com/ultralytics/ultralytics/pull/15736) by the developers of this open source software library.

# ///////////////////////////////////////////////

# Описание данных и порядок запуска: 

- _00_train.py_ - скрипт для обучения нейронной сети, осуществляющей сегментацию, время обучения - порядка 3.5 часов на Nvidia GTX 3060

- _01_convert_settings.bat_ - скрипт для конвертации настроек нейросети в формат ONNX

- _02_test.py_ - скрипт для тестирования нейросети с использованием модуля [../yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind)

- _data.yaml_ - файл с описанием датасета, используемого для обучения и тестирования

На основе [датасета Crack-Seg](https://docs.ultralytics.com/datasets/segment/crack-seg/), который был [незначительно модифицирован](https://github.com/ultralytics/ultralytics/pull/15736) разработчиками настоящей открытой программной библиотеки.

<img src="https://github.com/ultralytics/docs/releases/download/0/crack-segmentation-sample.avif">
