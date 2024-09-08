# ExtractYoloObjectsFromDICOM
ExtractYoloObjectsFromDICOM - open source console tool for medical researchers intelligent support and high performance MRI-data processing. It designed for training and testing datasets convertation from IMA-files (DICOM) and binary masks (PNG) to Yolo format - grayscale images (PNG) and textual labels (TXT).

# Compilation
Project configured for compilation in Visual Studio 2022 Community Edition (or in more powerfull version) with v141 platform toolset. Application based on 

-- modificated Flann library: 

_git clone https://github.com/Alarmod/flann_

-- original Dicomlib library:

_git clone https://github.com/Alarmod/dicomlib_

-- OpenCV library (version 3.1 are used):

_https://opencv.org_

For correct execution You must copy opencv_world310.dll from _opencv\build\x64\vc14\bin_ directory to folder with ExtractYoloObjectsFromDICOM.exe or add this directory to _PATH_ enviroment variable.

-- BOOST library (version 1.82 are used): 

_https://www.boost.org_

# Ways of using
1. Run _ExtractYoloObjectsFromDICOM.exe 1_ on [original DICOM-dataset](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/dataset/orig_dicom_dataset), that allows You to replace the original files with masks with expanded ones using dilate morphological operation. Original PNG-files with binary masks will be replaced, save original folders before!

Example of _ExtractYoloObjectsFromDICOM.exe 1_ execution can be downloaded here: 
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. To generate training and testing datasets in Yolo format, run _ExtractYoloObjectsFromDICOM.exe 0_

3. You have also the ability to directly convert directories with IMA-files into YOLO format (but without TXT-data) with further test processing using an artificial neural network: 

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Script _convert_dir.py_ use installed [yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind) module. The path to the system libraries (bin or lib folders) should be configured similarly to what was done with the [_boot.py_](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py) file (see [manual](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace) for "Automated Workplace" application). However, the capabilities of such conversion are limited by the fact that the utilities and conversion scripts are tied to the names of subdirectories stored in the example folder [dataset](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/ExtractYoloObjectsFromDICOM/dataset), which is done to automatically recognize the type of DICOM data without processing the corresponding internal tags.

# Used programming languages
C++

# ///////////////////////////////////////////////

# ExtractYoloObjectsFromDICOM
ExtractYoloObjectsFromDICOM - консольная программа с открытым исходным кодом для интеллектуальной поддержки врачей-исследователей и высокопроизводительной обработки данных МРТ. Она предназначена для обучения и тестирования преобразования наборов данных из файлов IMA (DICOM) и бинарных масок (PNG) в формат Yolo - изображения в оттенках серого (PNG) и текстовые метки (TXT).

# Компиляция
Проект настроен для компиляции в Visual Studio 2022 Community Edition (или в версии с большими возможностями) с набором инструментов платформы v141. Приложение основано на

-- модифицированной библиотеке Flann:

_git clone https://github.com/Alarmod/flann_

-- оригинальной библиотеке Dicomlib:

_git clone https://github.com/Alarmod/dicomlib_

-- библиотеке OpenCV (используется версия 3.1):

_https://opencv.org_

Для корректного выполнения необходимо скопировать opencv_world310.dll из каталога _opencv\build\x64\vc14\bin_ в папку с ExtractYoloObjectsFromDICOM.exe или добавьте этот каталог в переменную среды _PATH_.

-- библиотеке BOOST (используется версия 1.82):

_https://www.boost.org_

# Способы использования
1. Запустите _ExtractYoloObjectsFromDICOM.exe 1_ на [оригинальном наборе данных DICOM](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/dataset/orig_dicom_dataset), что позволит заменить исходные файлы с масками на расширенные с помощью морфологической операции расширения. Исходные PNG-файлы с бинарными масками будут заменены, предварительно сохраните исходные папки!

Пример результата выполнения _ExtractYoloObjectsFromDICOM.exe 1_ можно скачать здесь:
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. Для генерации обучающих и тестовых наборов данных в формате Yolo запустите _ExtractYoloObjectsFromDICOM.exe 0_

3. Также у вас есть возможность напрямую конвертировать каталоги с IMA-файлами в формат YOLO (но без TXT-данных) с дальнейшей тестовой обработкой с помощью искусственной нейронной сети: 

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Скрипт _convert_dir.py_ использует установленный модуль [yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind). Путь к системным библиотекам (папки bin или lib) должен быть настроен аналично тому, как было сделано с файлом [_boot.py_](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py) (см. [руководство](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace) к приложению "Automated Workplace"). Однако, возможности такой конвертации ограничены тем, что утилиты и скрипты конвертации привязаны к именам поддиректорий, хранимых в папке-примере [dataset](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/ExtractYoloObjectsFromDICOM/dataset), что сделано для автоматического распознавания типа DICOM-данных без обработки соответствующих внутренних тэгов.

# Используемые языки программирования
C++
