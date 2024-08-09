# Information on preliminary preparation for installation and using of the ExtractYoloObjectsFromDICOM utility
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

Ways of using:

1. Run _ExtractYoloObjectsFromDICOM.exe 1_ on original DICOM-dataset, that allows You to replace the original files with masks with expanded ones using dilate morphological operation. Original PNG-files with binary masks will be replaced, save original folders before!

Example of _ExtractYoloObjectsFromDICOM.exe 1_ execution result can be downloaded here: 
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. To generate training and testing datasets in Yolo format, run _ExtractYoloObjectsFromDICOM.exe 0_

3. You have also the ability to directly convert directories with IMA-files into YOLO format (but without TXT-data) with further test processing using an artificial neural network: 

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Script _convert_dir.py_ use installed [yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace) plugin.

# Information on preliminary preparation for installation and using of the "Automated Workplace" application
System requirements: Python 3.8-3.10

1. Install python libs with command _pip3 install -r requirements.txt_

2. Install ONNXRuntime libs for CPU or GPU: 
https://github.com/microsoft/onnxruntime/releases (version 1.18.0+)

3. Install [OpenCV 4.9.0+](https://opencv.org)

4. Confiure, build and install _yolo_segment_with_nanobind_ plugin:

In version 8.2.43 of the Ultralytics library, the order of obtaining segmentation masks was changed, the [developers abandoned the use of sigmoid function](https://github.com/ultralytics/ultralytics/pull/13939), which affected the results of neural networks trained on an earlier version of the library. If you retrain artificial neural networks with Ultralytics >8.2.42, change _#define USE_SIGMOID_FUNC true_ to _#define USE_SIGMOID_FUNC false_ in [yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp)

You must set actual path for ONNXRuntime and OpenCV, runtime version, compatible with your compiler. This directories with _bin_, _lib_ and _include_ subfolders. Settings must be set over _yolo_segment_with_nanobind/build.bat_ file. After configuration, the file or commands inside _build.bat_ must be launched (executed).

5. Configure path for system libs (_bin_ or _lib_ folders) over _boot.py_ file:

- actual path for ONNXRuntime and OpenCV (always required);

- actual path for [CUDA](https://developer.nvidia.com/cuda-toolkit) and [CUDNN](https://developer.nvidia.com/cudnn); this configuration requared if selected ONNXRuntime for GPU, for example, ONNXRuntime 1.18.0 requires presence [CUDA 11.8 or 12.4](https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html) and compatible drivers); tested with CUDNN version 8.9.7.29 and Nvidia driver version 528.24/555.85.

Manual to loading configuration files for artificial neural networks:

Load MRI_settings_ONNX.zip from

https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

and unpack it to resources folder (must be _resources/runs/segment/..._ structure)

Manual for GUI run: 

Configure [Classifier] section in _settings.ini_ file and run command

_python main.py_

Manual for speed test run: 

Configure [Classifier] section in _settings.ini_ file and run command 

_python test_processing_speed.py_

# Information on preliminary preparation for installation and using of the "yolo_8_segment_brain_ischemia_msk" application
System requirements: Python 3.8-3.10; Pytorch 2.2.2 or 2.3.1+

Libs installation and project files downloading:

1. Install libs

a) Pytorch 2.2.2 or 2.3.1+:

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu124_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu_

PyTorch 2.3.0 have bug that affects to Yolo 8 https://github.com/pytorch/pytorch/issues/121834

b) Ultralytics ==8.0.205 or (>8.1.5 and <8.2.43):

_pip3 install -U ultralytics==8.0.205_ or

_pip3 install -U ultralytics==8.1.6_ or

...

_pip3 install -U ultralytics==8.2.42_

In version 8.2.43 and more new of the Ultralytics library, the order of obtaining segmentation masks was changed, the [developers abandoned the use of sigmoid function](https://github.com/ultralytics/ultralytics/pull/13939), which affected the results of neural networks trained on an earlier version of the library, therefore it is recommended to either retrain our artificial neural networks, or use for tests Ultralytics library with version <8.2.43.

2. Load MRI_settings.zip and MRI_tests.zip: 

MRI_settings.zip --- https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing or  https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings.zip

MRI_tests.zip --- https://drive.google.com/file/d/15to8SyUL6DXNHhYwYQR9KJZ4bDlC3Yyq/view?usp=sharing

3. Unpack full runs directory with commands: 

_start /W /B winrar.exe x -ibck MRI_settings.zip_

_start /W /B winrar.exe x -ibck MRI_tests.zip_

Datasets for training can be loaded from 
https://drive.google.com/file/d/1SSYDAWpAw0Sbaz3dpHn_EH6ZuGZWt-LW/view?usp=drive_link
or from https://huggingface.co/datasets/alarmod/MRI/tree/main/train_and_test_data

(must be stored to _datasets_ folder and unpacked)

4. Configure settings.yaml (internal settings file of the _ultralytics_ library) if You already use Ultralytics in other projects
```
datasets_dir: [Path to _yolo_8_segment_brain_ischemia_msk\datasets_ folder]
weights_dir: weights
runs_dir: runs
```
For example, this file can be found here: _C:\Users\\%USERNAME%\AppData\Roaming\Ultralytics\settings.yaml_

Ways of using:

If Your GPU FP16 speed slow, change _global_half = True_ to _global_half = False_ in train.py and test.py.

1. Training:

_python train.py_

2. Testing: 

_python test.py default_validate_ (Yolo 8 default validation code, the results are far from what you get in Yolo predict)

_python test.py generate_data_ (validate with saving all data: contours, zones, cognitive visualization)

_python test.py internal_validate_ (modificated Yolo 8 validation code that closer to Yolo original prediction code)

Train and test on Nvidia RTX 16xx in FP16 mode may be slow (see https://github.com/ultralytics/yolov5/issues/7908)

Manual for exporting settings to ONNX:

To convert the settings of artificial neural networks into the ONNX format, you need run script _export.bat_

Export results can be loaded from
https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

MRI_settings_ONNX.zip can be unpacked with command

_start /W /B winrar.exe x -ibck MRI_settings_ONNX.zip_

Settings in original format will be saved in _orig_settings_ directory.


# Example of new variants for using the open library code
Instructions for using the open library code in third-party repositories:

1. Preparing the neural network model.
   
1.1. Converting the source data intended for training the ANN to the YOLO format (https://docs.ultralytics.com/datasets/segment/#ultralytics-yolo-format).

1.2. Training the neural network (see the example of the training script in the file https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/train.py).

1.3. Converting the settings of the trained neural network to the ONNX format (see the script https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/export.bat). The export parameters (the width and height of the internal input of the neural network) were obtained from the script https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py.

2. Using the yolo_segment_with_nanobind module (https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind). The Python API of the module includes the setGlobalThreadPoolSize function (the only parameter of the function specifies the number of computing threads used to implement multi-threaded processing in CPU mode) and a description of the YOLO class, which implements the following methods:

- constructor with parameters:
   
o "name" – model name;

o "net_path" – path to the neural network settings;

o "use_gpu" – flag for connecting the graphics accelerator (True or False);

o "use_fp16" – flag for enabling half-precision computing mode (True or False);

o "net_width" – defines the width of the internal input of the neural network (according to the result of get_net_info.py);

o "net_height" – defines the height of the internal input of the neural network (according to the result of get_net_info.py);

o "cudaID" – the number of the graphics accelerator;

- the warmup method – the method for initializing the neural network model;
   
- the process method with parameters:
  
o "input" – input image (single-channel/three-channel);

o "rec_treshold" – recognition confidence threshold;

o "max_results" – the maximum number of returned results (areas of interest);

o "get_brain" – a flag that determines the return of a binary mask with one object with maximum confidence (with filling of internal areas);

o "erode_level" – the number of iterations of the image erosion operation used for post-processing the ANN output.

The process method returns the results of the ANN operation, the output size is the same as the size of the image supplied via the "input" parameter.

An example of using the yolo_segment_with_nanobind module is given in the script https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py.

Link to an example of a new use case of the open library code (segmentation of structural damage images in the form of road surface cracks): https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/cracks_segmentator.

# ///////////////////////////////////////////////

# Информация о предварительной подготовке к установке и использованию утилиты ExtractYoloObjectsFromDICOM
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

Способы использования:

1. Запустите _ExtractYoloObjectsFromDICOM.exe 1_ на оригинальном наборе данных DICOM, что позволит заменить исходные файлы с масками на расширенные с помощью морфологической операции расширения. Исходные PNG-файлы с бинарными масками будут заменены, предварительно сохраните исходные папки!

Пример результата выполнения _ExtractYoloObjectsFromDICOM.exe 1_ можно скачать здесь:
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. Для генерации обучающих и тестовых наборов данных в формате Yolo запустите _ExtractYoloObjectsFromDICOM.exe 0_

3. Также у вас есть возможность напрямую конвертировать каталоги с IMA-файлами в формат YOLO (но без TXT-данных) с дальнейшей тестовой обработкой с помощью искусственной нейронной сети:

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Скрипт _convert_dir.py_ использует установленный Плагин [yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace).

# Информация о предварительной подготовке к установке и использованию приложения "Automated Workplace"
Системные требования: Python 3.8-3.10

1. Установите библиотеки python командой _pip3 install -r requirements.txt_

2. Установите библиотеки ONNXRuntime для CPU или GPU:
https://github.com/microsoft/onnxruntime/releases (версия 1.18.0+)

3. Установите [OpenCV 4.9.0+](https://opencv.org)

4. Настройте, соберите и установите плагин _yolo_segment_with_nanobind_:

В версии 8.2.43 библиотеки Ultralytics изменен порядок получения масок сегментации, [разработчики отказались от использования сигмоидальной функции](https://github.com/ultralytics/ultralytics/pull/13939), что повлияло на результаты нейронных сетей, обученных на более ранней версии библиотеки. Если вы переобучаете искусственные нейронные сети с помощью Ultralytics >8.2.42, измените _#define USE_SIGMOID_FUNC true_ на _#define USE_SIGMOID_FUNC false_ в [yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp)

Вы должны указать фактический путь для ONNXRuntime и OpenCV, версию среды выполнения, совместимую с вашим компилятором. Это каталоги с подпапками _bin_, _lib_ и _include_. Настройки должны быть установлены через файл _yolo_segment_with_nanobind/build.bat_. После настройки: файл или команды внутри _build.bat_ должны быть запущены (выполнены).

5. Настройте путь для системных библиотек (папки _bin_ или _lib_) через файл _boot.py_:

- фактический путь для ONNXRuntime и OpenCV (всегда требуется);

- фактический путь для [CUDA](https://developer.nvidia.com/cuda-toolkit) и [CUDNN](https://developer.nvidia.com/cudnn); эта настройка требуется, если выбран ONNXRuntime для GPU, например, ONNXRuntime 1.18.0 требует наличия [CUDA 11.8 или 12.4](https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html) и совместимых драйверов); протестировано с CUDNN версии 8.9.7.29 и драйвером Nvidia версии 528.24/555.85.

Руководство по загрузке файлов конфигурации для искусственных нейронных сетей:

Загрузите MRI_settings_ONNX.zip из

https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
или
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

и распакуйте его в папку ресурсов (должна быть структура _resources/runs/segment/..._)

Руководство по запуску GUI:

Настройте раздел [Classifier] в файле _settings.ini_ и выполните команду

_python main.py_

Руководство по запуску теста скорости:

Настройте раздел [Classifier] в файле _settings.ini_ и выполните команду

_python test_processing_speed.py_

# Информация о предварительной подготовке к установке и использованию приложения "yolo_8_segment_brain_ischemia_msk"
Системные требования: Python 3.8-3.10; Pytorch 2.2.2 или 2.3.1+

Установка библиотек и загрузка файлов проекта:

1. Установка библиотек

a) Pytorch 2.2.2 или 2.3.1+:

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118_ или

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121_ или

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu124_ или

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu_

В PyTorch 2.3.0 есть ошибка, которая влияет на Yolo 8 https://github.com/pytorch/pytorch/issues/121834

б) Ultralytics ==8.0.205 или (>8.1.5 и <8.2.43):

_pip3 install -U ultralytics==8.0.205_ или

_pip3 install -U ultralytics==8.1.6_ или

...

_pip3 install -U ultralytics==8.2.42_

В версии 8.2.43 и более новых библиотеки Ultralytics был изменен порядок получения масок сегментации, [разработчики отказались от использования сигмоидальной функции](https://github.com/ultralytics/ultralytics/pull/13939), что повлияло на результаты нейронных сетей, обученных на более ранней версии библиотеки, поэтому рекомендуется либо переобучить наши искусственные нейронные сети, либо использовать для тестов библиотеку Ultralytics версии <8.2.43.

2. Загрузите MRI_settings.zip и MRI_tests.zip:

MRI_settings.zip --- https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing или https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings.zip

MRI_tests.zip --- https://drive.google.com/file/d/15to8SyUL6DXNHhYwYQR9KJZ4bDlC3Yyq/view?usp=sharing

3. Распакуйте полный каталог запусков с помощью команд:

_start /W /B winrar.exe x -ibck MRI_settings.zip_

_start /W /B winrar.exe x -ibck MRI_tests.zip_

Наборы данных для обучения можно загрузить с
https://drive.google.com/file/d/1SSYDAWpAw0Sbaz3dpHn_EH6ZuGZWt-LW/view?usp=drive_link
или с https://huggingface.co/datasets/alarmod/MRI/tree/main/train_and_test_data

(должны быть сохранены в папке _datasets_ и распакованы)

4. Настройте settings.yaml (внутренний файл настроек библиотеки _ultralytics_), если вы уже используете Ultralytics в других проектах
```
datasets_dir: [Путь к папке _yolo_8_segment_brain_ischemia_msk\datasets_]
weights_dir: weights
runs_dir: runnings
```
Например, этот файл можно найти здесь: _C:\Users\\%USERNAME%\AppData\Roaming\Ultralytics\settings.yaml_

Способы использования:

Если скорость вашего GPU в режиме FP16 низкая, измените _global_half = True_ на _global_half = False_ в train.py и test.py.

1. Обучение:

_python train.py_

2. Тестирование:

_python test.py default_validate_ (код проверки Yolo 8 по умолчанию, результаты далеки от того, что вы получаете в Yolo predict)

_python test.py generate_data_ (проверка с сохранением всех данных: контуры, зоны, когнитивная визуализация)

_python test.py internal_validate_ (модифицированный код проверки Yolo 8, который ближе к оригинальному коду прогнозирования Yolo)

Обучение и тестирование на Nvidia RTX 16xx в режиме FP16 может быть медленным (см. https://github.com/ultralytics/yolov5/issues/7908)

Руководство по экспорту настроек в ONNX:

Чтобы преобразовать настройки искусственных нейронных сетей в формат ONNX, необходимо запустить скрипт _export.bat_

Результаты экспорта можно загрузить из
https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
или
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

MRI_settings_ONNX.zip можно распаковать с помощью команды

_start /W /B winrar.exe x -ibck MRI_settings_ONNX.zip_

Настройки в исходном формате будут сохранены в каталоге _orig_settings_.

# Пример новых вариантов использования кода открытой библиотеки
Инструкция по использованию кода открытой библиотеки в сторонних репозиториях:

1. Подготовка нейросетевой модели.
   
1.1. Конвертация исходных данных, предназначенных для обучения ИНС, в YOLO-формат (https://docs.ultralytics.com/datasets/segment/#ultralytics-yolo-format).

1.2. Обучение нейронной сети (см. пример обучающего скрипта в файле https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/train.py).

1.3. Конвертация настроек обученной нейронной сети в формат ONNX (см. скрипт https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/export.bat). Параметры экспорта (ширина и высота внутреннего входа нейронной сети) получены из скрипта https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py.

2. Применение модуля yolo_segment_with_nanobind (https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind). Python-API модуля включает функцию setGlobalThreadPoolSize (единственный параметр функции задает количество вычислительных потоков, используемых для реализации многопоточной обработки в CPU-режиме) и описание класса YOLO, реализующего следующие методы:
   
- конструктор с параметрами:
  
o "name" – имя модели;

o "net_path" – путь к настройкам нейронной сети; 

o "use_gpu" – флаг подключения графического ускорителя (True или False);

o "use_fp16" – флаг включения режима вычислений с половинной точностью (True или False); 

o "net_width" – определяет ширину внутреннего входа нейронной сети (в соответствии с результатом работы get_net_info.py); 

o "net_height" – определяет высоту внутреннего входа нейронной сети (в соответствии с результатом работы get_net_info.py);

o "cudaID" – номер графического ускорителя; 

- метод warmup – метод инициализации нейросетевой модели;

- метод process с параметрами:

o "input" – входное изображение (одноканальное/трехканальное);

o "rec_treshold" – порог уверенности распознавания;

o "max_results" – максимальное количество возвращаемых результатов (зон интереса);

o "get_brain" – флаг, определяющий возврат бинарной маски с одним объектом с максимальной уверенностью (с заливкой внутренних областей);

o "erode_level" – количество итераций операции эрозии изображения, применяемых для постобработки выхода ИНС.

Метод process возвращает результаты работы ИНС, размер выхода совпадает с размером изображения, подаваемого через параметр "input".

Пример использования модуля yolo_segment_with_nanobind приведен в скрипте https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py.

Ссылка на пример нового варианта использования кода открытой библиотеки (сегментация на изображениях структурных повреждений в виде трещин дорожного покрытия): https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/cracks_segmentator.
