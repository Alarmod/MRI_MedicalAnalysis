# ExtractYoloObjectsFromDICOM
ExtractYoloObjectsFromDICOM - open source console tool for medical researchers intelligent support and high performance MRI-data processing. It designed for training and testing datasets convertation from IMA-files (DICOM) and binary masks (PNG) to Yolo format - grayscale images (PNG) and textual labels (TXT).

# Compilation
Project configured for compilation in Visual Studio 2022 with v141 platform toolset. Application based on 

-- modificated Flann library: 

_git clone https://github.com/Alarmod/flann_

-- original Dicomlib library:

_git clone https://github.com/Alarmod/dicomlib_

-- OpenCV library (version 3.1 are used):

_https://opencv.org_

For correct execution You must copy opencv_world310.dll from _opencv\build\x64\vc14\bin_ directory to folder with ExtractYoloObjectsFromDICOM.exe

-- BOOST library (version 1.82 are used): 

_https://www.boost.org_

# Ways of using
1. Run _ExtractYoloObjectsFromDICOM.exe 1_ on original DICOM-dataset, that allows You to replace the original files with masks with expanded ones using dilate morphological operation. Original PNG-files with binary masks will be replaced, save original folders before!

Example of _ExtractYoloObjectsFromDICOM.exe 1_ execution can be downloaded here: 
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. To generate training and testing datasets in Yolo format, run _ExtractYoloObjectsFromDICOM.exe 0_

3. You have also the ability to directly convert directories with IMA-files into YOLO format (but without TXT-data) with further test processing using an artificial neural network: 

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Script _convert_dir.py_ use installed [yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace) module.

# Used programming languages
C++

# ///////////////////////////////////////////////

# ExtractYoloObjectsFromDICOM
ExtractYoloObjectsFromDICOM - консольная программа с открытым исходным кодом для интеллектуальной поддержки врачей-исследователей и высокопроизводительной обработки данных МРТ. Она предназначена для обучения и тестирования преобразования наборов данных из файлов IMA (DICOM) и бинарных масок (PNG) в формат Yolo - изображения в оттенках серого (PNG) и текстовые метки (TXT).

# Компиляция
Проект настроен для компиляции в Visual Studio 2022 с набором инструментов платформы v141. Приложение основано на

-- модифицированной библиотеке Flann:

_git clone https://github.com/Alarmod/flann_

-- оригинальной библиотеке Dicomlib:

_git clone https://github.com/Alarmod/dicomlib_

-- библиотеке OpenCV (используется версия 3.1):

_https://opencv.org_

Для корректного выполнения необходимо скопировать opencv_world310.dll из каталога _opencv\build\x64\vc14\bin_ в папку с ExtractYoloObjectsFromDICOM.exe

-- библиотеке BOOST (используется версия 1.82):

_https://www.boost.org_

# Способы использования
1. Запустите _ExtractYoloObjectsFromDICOM.exe 1_ на оригинальном наборе данных DICOM, что позволит заменить исходные файлы с масками на расширенные с помощью морфологической операции расширения. Исходные PNG-файлы с бинарными масками будут заменены, предварительно сохраните исходные папки!

Пример результата выполнения _ExtractYoloObjectsFromDICOM.exe 1_ можно скачать здесь:
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. Для генерации обучающих и тестовых наборов данных в формате Yolo запустите _ExtractYoloObjectsFromDICOM.exe 0_

3. Также у вас есть возможность напрямую конвертировать каталоги с IMA-файлами в формат YOLO (но без TXT-данных) с дальнейшей тестовой обработкой с помощью искусственной нейронной сети:

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Скрипт _convert_dir.py_ использует установленный модуль [yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace).

# Используемые языки программирования
C++
