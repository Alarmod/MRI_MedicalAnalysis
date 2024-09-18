# Information on preliminary preparation for installation and using of the "ExtractYoloObjectsFromDICOM" utility
Project configured for compilation in Visual Studio 2022 Community Edition (or more powerfull version). Application based on 

-- modificated Flann library: 

_git clone https://github.com/Alarmod/flann_

-- original Dicomlib library:

_git clone https://github.com/Alarmod/dicomlib_

-- OpenCV library (version 4.9.0 are used):

_https://opencv.org_

For correct execution You must copy opencv_world490.dll from _opencv\build\x64\vc16\bin_ directory to folder with ExtractYoloObjectsFromDICOM.exe or add this directory to _PATH_ enviroment variable.

-- BOOST library (version 1.86 are used): 

_https://www.boost.org_

Ways of using:

1. Run _ExtractYoloObjectsFromDICOM.exe 1_ on [original DICOM-dataset](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/dataset/orig_dicom_dataset), that allows You to replace the original files with masks with expanded ones using dilate morphological operation. Original PNG-files with binary masks will be replaced, save original folders before!

Example of _ExtractYoloObjectsFromDICOM.exe 1_ execution result can be downloaded here: 
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. To generate training and testing datasets in Yolo format, run _ExtractYoloObjectsFromDICOM.exe 0_

3. You have also the ability to directly convert directories with IMA-files into YOLO format (but without TXT-data) with further test processing using an artificial neural network: 

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Script [ExtractYoloObjectsFromDICOM/convert_dir.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py) use installed [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind) module. The path to the system libraries (bin or lib folders) should be configured similarly to what was done with the [automated_workplace/boot.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py) file (see further manual for "Automated Workplace" application). However, the capabilities of such conversion are limited by the fact that the utilities and conversion scripts are tied to the names of subdirectories stored in the example folder [dataset](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/ExtractYoloObjectsFromDICOM/dataset), which is done to automatically recognize the type of DICOM data without processing the corresponding internal tags.

# Information on preliminary preparation for installation and using of the "Automated Workplace" application
System requirements: Python 3.8-3.10

1. Install python libs with command _pip3 install -r requirements.txt_

2. Install ONNXRuntime libs for CPU or GPU: 
https://github.com/microsoft/onnxruntime/releases (version 1.18.0+)

3. Install [OpenCV 4.9.0+](https://opencv.org)

4. Confiure, build and install [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind) module:

In version 8.2.43 of the Ultralytics library, the order of obtaining segmentation masks was changed, the [developers abandoned the use of sigmoid function](https://github.com/ultralytics/ultralytics/pull/13939), which affected the results of neural networks trained on an earlier version of the library. If You retrain artificial neural networks with Ultralytics >8.2.42, change _#define USE_SIGMOID_FUNC true_ to _#define USE_SIGMOID_FUNC false_ in [automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp)

You must set actual path for ONNXRuntime and OpenCV, runtime version, compatible with Your compiler. This directories with _bin_, _lib_ and _include_ subfolders. Settings must be set over [automated_workplace/yolo_segment_with_nanobind/build.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/build.bat) file. After configuration, the file or commands inside [automated_workplace/yolo_segment_with_nanobind/build.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/build.bat) must be launched (executed).

5. Configure path for system libs (_bin_ or _lib_ folders) over file [automated_workplace/boot.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py):

- actual path for ONNXRuntime and OpenCV (always required);

- actual path for [CUDA](https://developer.nvidia.com/cuda-toolkit) and [CUDNN](https://developer.nvidia.com/cudnn); this configuration requared if selected ONNXRuntime for GPU, for example, ONNXRuntime 1.18.0 requires presence [CUDA 11.8.0 or 12.4.0](https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html) and compatible drivers); tested with CUDNN version 8.9.7.29 and Nvidia driver version 528.24/555.85.

Manual to loading configuration files for artificial neural networks:

Load _MRI_settings_ONNX.zip_ from

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
System requirements: Python 3.8-3.10; PyTorch 2.2.2 or 2.3.1+

Libs installation and project files downloading:

1. Install libs

a) PyTorch 2.2.2 or 2.3.1+:

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu124_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu_

Warning: 

- PyTorch 2.3.0 have [bug](https://github.com/pytorch/pytorch/issues/121834) that affects to Yolo 8.

- CPU inference is broken in PyTorch 2.4.0 (see https://github.com/ultralytics/ultralytics/issues/15049). Use PyTorch 2.4.1 or an [older version](https://pytorch.org/get-started/previous-versions/), such as PyTorch 2.3.1.

- It is recommended to train and test already trained neural networks on GPU, primarily because Ultralytics works slowly on CPU. If there is no graphics accelerator, You need to install the CPU build of PyTorch, otherwise You may encounter a BEX64 exception on the nvdxgdmal64.dll_unloaded event. An erroneously installed version of PyTorch can be removed using the command _pip3 uninstall torch torchvision torchaudio_.

b) Ultralytics ==8.0.205 or (>8.1.5 and <8.2.43):

_pip3 install -U ultralytics==8.0.205_ or

_pip3 install -U ultralytics==8.1.6_ or

...

_pip3 install -U ultralytics==8.2.42_

In version 8.2.43 and more new versions of the Ultralytics library, the order of obtaining segmentation masks was changed, the [developers abandoned the use of sigmoid function](https://github.com/ultralytics/ultralytics/pull/13939), which affected the results of neural networks trained on an earlier version of the library, therefore it is recommended to either retrain our artificial neural networks, or use for tests Ultralytics library with version <8.2.43.

2. Load _MRI_settings.zip_ and _MRI_tests.zip_: 

_MRI_settings.zip_ --- https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing or  https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings.zip

_MRI_tests.zip_ --- https://drive.google.com/file/d/15to8SyUL6DXNHhYwYQR9KJZ4bDlC3Yyq/view?usp=sharing

3. Unpack full _runs_ directory with commands: 

_powershell -command "Expand-Archive -Force MRI_settings.zip ./"_

_powershell -command "Expand-Archive -Force MRI_tests.zip ./"_

4. Datasets for training can be loaded from 
https://drive.google.com/file/d/1SSYDAWpAw0Sbaz3dpHn_EH6ZuGZWt-LW/view?usp=drive_link
or from https://huggingface.co/datasets/alarmod/MRI/tree/main/train_and_test_data

(must be stored to _datasets_ folder and unpacked)

5. Configure _settings.yaml_ (internal settings file of the Ultralytics library) if You already use Ultralytics in other projects
```
datasets_dir: [Path to _yolo_8_segment_brain_ischemia_msk/datasets_ folder]
weights_dir: weights
runs_dir: runs
```
For example, this file can be found here: _C:\Users\\%USERNAME%\AppData\Roaming\Ultralytics\settings.yaml_

Ways of using:

If Your GPU FP16 speed slow, change _global_half = True_ to _global_half = False_ in train.py and test.py.

1. Training:

_python train.py_

2. Testing: 

_python test.py default_validate_ (Yolo 8 default validation code, the results are far from what You get in Yolo predict)

_python test.py generate_data_ (validate with saving all data: contours, zones, cognitive visualization)

_python test.py internal_validate_ (modificated Yolo 8 validation code that closer to Yolo original prediction code)

Train and test on Nvidia RTX 16xx in FP16 mode may be slow (see https://github.com/ultralytics/yolov5/issues/7908)

Manual for exporting settings to ONNX format:

To convert the settings of artificial neural networks into the ONNX format, You need run script [yolo_8_segment_brain_ischemia_msk/export.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/export.bat).

Export results can be loaded from
https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

_MRI_settings_ONNX.zip_ can be unpacked with command

_powershell -command "Expand-Archive -Force MRI_settings_ONNX.zip ./"_

Settings in original format will be saved in _orig_settings_ directory.

# Example 1 of new variant for using the open source library code
Instructions for using the open library code in third-party repositories:

1. Preparing the neural network model.
   
   1.1. Converting the source data intended for training the ANN to the YOLO format (https://docs.ultralytics.com/datasets/segment/#ultralytics-yolo-format).

   The library code is optimized for Yolo 8 with using rectangular input (training and using with the _rect=True_ parameter). It is necessary that the images included in the training dataset have the same aspect ratio, otherwise training using batches will be ineffective (due to the fact that for each batch using independently alignment of incoming data). To correct datasets that do not meet the specified requirement, it is recommended to use the LetterBox function (see [C++ code](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp) that run with the _autoShape=True_ parameter or [Python code](https://github.com/ultralytics/yolov5/blob/master/utils/augmentations.py) that run with the _auto=True_ parameter).

   1.2. Training the neural network (see the example of the training script in the file [yolo_8_segment_brain_ischemia_msk/train.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/train.py)).

   1.3. Converting the settings of the trained neural network to the ONNX format (see the script [yolo_8_segment_brain_ischemia_msk/export.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/export.bat)). The export parameters (the width and height of the internal input of the neural network) must be obtained from the script [yolo_8_segment_brain_ischemia_msk/get_net_info.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py).

2. Using the [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind) module. The Python API of the module includes the setGlobalThreadPoolSize function (the only parameter of the function specifies the number of computing threads used to implement multi-threaded processing in CPU mode) and a description of the YOLO class, which implements the following methods:

- constructor with parameters:
   
  - "name" – model name;

  - "net_path" – path to the neural network settings;

  - "use_gpu" – flag for connecting the graphics accelerator (True or False);

  - "use_fp16" – flag for enabling half-precision computing mode (True or False);

  - "net_width" – defines the width of the internal input of the neural network (according to the result of [yolo_8_segment_brain_ischemia_msk/get_net_info.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py));

  - "net_height" – defines the height of the internal input of the neural network (according to the result of [yolo_8_segment_brain_ischemia_msk/get_net_info.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py));

  - "cudaID" – the number of the graphics accelerator;

- the "warmup" method – the method for initializing the neural network model;
   
- the "process" method with parameters:
  
  - "input" – input image (single-channel/three-channel);

  - "rec_treshold" – recognition confidence threshold;

  - "max_results" – the maximum number of returned results (areas of interest);

  - "get_brain" – a flag that determines the return of a binary mask with one object with maximum confidence (with filling of internal areas);

  - "erode_level" – the number of iterations of the image erosion operation used for post-processing the ANN output.

The "process" method returns the results of the ANN operation, the output size is the same as the size of the image supplied via the "input" parameter.

An example of using the [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind) module is given in the script [ExtractYoloObjectsFromDICOM/convert_dir.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py).

Link to an example of a new use case of the open library code (segmentation of structural damage images in the form of road surface cracks): [automated_workplace/cracks_segmentator](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/cracks_segmentator). The path to the system libraries (bin or lib folders) should be configured in [automated_workplace/cracks_segmentator/02_test.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/cracks_segmentator/02_test.py) similarly to what was done with the [automated_workplace/boot.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py) file (see manual for "Automated Workplace" application). The _settings.yaml_ file should have the following settings:
```
datasets_dir: [Path to _automated_workplace/cracks_segmentator/datasets/crack-seg_ folder]
weights_dir: weights
runs_dir: runs
```

# Example 2 of new variant for using the open source library code
An example of cognitive contour visualization of fire and smoke sources is posted in the repository: https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/contours_viewer

# ///////////////////////////////////////////////

# Информация о предварительной подготовке к установке и использованию утилиты "ExtractYoloObjectsFromDICOM"
Проект настроен для компиляции в Visual Studio 2022 Community Edition (или в версии с большими возможностями). Приложение основано на

-- модифицированной библиотеке Flann:

_git clone https://github.com/Alarmod/flann_

-- оригинальной библиотеке Dicomlib:

_git clone https://github.com/Alarmod/dicomlib_

-- библиотеке OpenCV (используется версия 4.9.0):

_https://opencv.org_

Для корректного выполнения необходимо скопировать opencv_world490.dll из каталога _opencv\build\x64\vc16\bin_ в папку с ExtractYoloObjectsFromDICOM.exe или добавьте этот каталог в переменную среды _PATH_.

-- библиотеке BOOST (используется версия 1.86):

_https://www.boost.org_

Способы использования:

1. Запустите _ExtractYoloObjectsFromDICOM.exe 1_ на [оригинальном наборе данных DICOM](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/dataset/orig_dicom_dataset), что позволит заменить исходные файлы с масками на расширенные с помощью морфологической операции расширения. Исходные PNG-файлы с бинарными масками будут заменены, предварительно сохраните исходные папки!

Пример результата выполнения _ExtractYoloObjectsFromDICOM.exe 1_ можно скачать здесь:
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. Для генерации обучающих и тестовых наборов данных в формате Yolo запустите _ExtractYoloObjectsFromDICOM.exe 0_

3. Также у вас есть возможность напрямую конвертировать каталоги с IMA-файлами в формат YOLO (но без TXT-данных) с дальнейшей тестовой обработкой с помощью искусственной нейронной сети:

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Скрипт [ExtractYoloObjectsFromDICOM/convert_dir.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py) использует установленный модуль [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind). Путь к системным библиотекам (папки bin или lib) должен быть настроен аналично тому, как было сделано с файлом [automated_workplace/boot.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py) (см. далее руководство к приложению "Automated Workplace"). Однако, возможности такой конвертации ограничены тем, что утилиты и скрипты конвертации привязаны к именам поддиректорий, хранимых в папке-примере [dataset](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/ExtractYoloObjectsFromDICOM/dataset), что сделано для автоматического распознавания типа DICOM-данных без обработки соответствующих внутренних тэгов.

# Информация о предварительной подготовке к установке и использованию приложения "Automated Workplace"
Системные требования: Python 3.8-3.10

1. Установите библиотеки python командой _pip3 install -r requirements.txt_

2. Установите библиотеки ONNXRuntime для CPU или GPU:
https://github.com/microsoft/onnxruntime/releases (версия 1.18.0+)

3. Установите [OpenCV 4.9.0+](https://opencv.org)

4. Настройте, соберите и установите модуль [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind):

В версии 8.2.43 библиотеки Ultralytics изменен порядок получения масок сегментации, [разработчики отказались от использования сигмоидальной функции](https://github.com/ultralytics/ultralytics/pull/13939), что повлияло на результаты нейронных сетей, обученных на более ранней версии библиотеки. Если вы переобучаете искусственные нейронные сети с помощью Ultralytics >8.2.42, измените _#define USE_SIGMOID_FUNC true_ на _#define USE_SIGMOID_FUNC false_ в [automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp)

Вы должны указать фактический путь для ONNXRuntime и OpenCV, версию среды выполнения, совместимую с вашим компилятором. Это каталоги с подпапками _bin_, _lib_ и _include_. Настройки должны быть установлены через файл [automated_workplace/yolo_segment_with_nanobind/build.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/build.bat). После настройки: файл или команды внутри [automated_workplace/yolo_segment_with_nanobind/build.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/build.bat) должны быть запущены (выполнены).

5. Настройте путь для системных библиотек (папки _bin_ или _lib_) через файл [automated_workplace/boot.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py):

- фактический путь для ONNXRuntime и OpenCV (всегда требуется);

- фактический путь для [CUDA](https://developer.nvidia.com/cuda-toolkit) и [CUDNN](https://developer.nvidia.com/cudnn); эта настройка требуется, если выбран ONNXRuntime для GPU, например, ONNXRuntime 1.18.0 требует наличия [CUDA 11.8.0 или 12.4.0](https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html) и совместимых драйверов); протестировано с CUDNN версии 8.9.7.29 и драйвером Nvidia версии 528.24/555.85.

Руководство по загрузке файлов конфигурации для искусственных нейронных сетей:

Загрузите _MRI_settings_ONNX.zip_ из

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

а) PyTorch 2.2.2 или 2.3.1+:

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118_ или

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121_ или

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu124_ или

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu_

Внимание: 
- PyTorch 2.3.0 имеет [ошибку](https://github.com/pytorch/pytorch/issues/121834), которая влияет на Yolo 8.

- Инференс на CPU в PyTorch 2.4.0 сломан (см. https://github.com/ultralytics/ultralytics/issues/15049). Используйте PyTorch 2.4.1 или [более старую версию](https://pytorch.org/get-started/previous-versions/), например, PyTorch 2.3.1.

- Рекомендуется выполнять обучение и тестирование уже обученных нейросетей на GPU, в первую очередь - из-за медленной работы Ultralytics на CPU. Если же графический ускоритель отсутствует, необходимо ставить именно CPU-сборку PyTorch, иначе Вы можете столкнуться с исключением BEX64 на событии nvdxgdmal64.dll_unloaded. Ошибочно поставленную версию PyTorch можно удалить с помощью команды _pip3 uninstall torch torchvision torchaudio_.

б) Ultralytics ==8.0.205 или (>8.1.5 и <8.2.43):

_pip3 install -U ultralytics==8.0.205_ или

_pip3 install -U ultralytics==8.1.6_ или

...

_pip3 install -U ultralytics==8.2.42_

В версии 8.2.43 и более новых версиях библиотеки Ultralytics был изменен порядок получения масок сегментации, [разработчики отказались от использования сигмоидальной функции](https://github.com/ultralytics/ultralytics/pull/13939), что повлияло на результаты нейронных сетей, обученных на более ранней версии библиотеки, поэтому рекомендуется либо переобучить наши искусственные нейронные сети, либо использовать для тестов библиотеку Ultralytics версии <8.2.43.

2. Загрузите _MRI_settings.zip_ и _MRI_tests.zip_:

_MRI_settings.zip_ --- https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing или https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings.zip

_MRI_tests.zip_ --- https://drive.google.com/file/d/15to8SyUL6DXNHhYwYQR9KJZ4bDlC3Yyq/view?usp=sharing

3. Распакуйте полный каталог _runs_ с помощью команд:

_powershell -command "Expand-Archive -Force MRI_settings.zip ./"_

_powershell -command "Expand-Archive -Force MRI_tests.zip ./"_

4. Наборы данных для обучения можно загрузить с
https://drive.google.com/file/d/1SSYDAWpAw0Sbaz3dpHn_EH6ZuGZWt-LW/view?usp=drive_link
или с https://huggingface.co/datasets/alarmod/MRI/tree/main/train_and_test_data

(должны быть сохранены в папке _datasets_ и распакованы)

5. Настройте _settings.yaml_ (внутренний файл настроек библиотеки Ultralytics), если вы уже используете Ultralytics в других проектах
```
datasets_dir: [Путь к папке _yolo_8_segment_brain_ischemia_msk/datasets_]
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

Руководство по экспорту настроек в формат ONNX:

Чтобы преобразовать настройки искусственных нейронных сетей в формат ONNX, необходимо запустить скрипт [yolo_8_segment_brain_ischemia_msk/export.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/export.bat).

Результаты экспорта можно загрузить из
https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
или
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

_MRI_settings_ONNX.zip_ можно распаковать с помощью команды

_powershell -command "Expand-Archive -Force MRI_settings_ONNX.zip ./"_

Настройки в исходном формате будут сохранены в каталоге _orig_settings_.

# Пример 1 нового варианта использования кода открытой библиотеки
Инструкция по использованию кода открытой библиотеки в сторонних репозиториях:

1. Подготовка нейросетевой модели.
   
   1.1. Конвертация исходных данных, предназначенных для обучения ИНС, в YOLO-формат (https://docs.ultralytics.com/datasets/segment/#ultralytics-yolo-format).

   Код библиотеки оптимизирован для работы Yolo 8 с применением прямоугольного входа (обучение и использование с параметром _rect=True_). Необходимо, чтобы изображения, включаемые в обучающую выборку, были с одним и тем же соотношением сторон, иначе обучение с использованием батчей будет неэффективным (из-за того, что для каждого батча выравнивавание поступающих данных осуществляется независимо). Для коррекции датасетов, не отвечающих указанному требованию, рекомендуется использовать функцию LetterBox (см. [С++-код](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/yolo_segment_with_nanobind/yolo_segment_with_nanobind.cpp), запускаемый с параметром _autoShape=True_, или [Python-код](https://github.com/ultralytics/yolov5/blob/master/utils/augmentations.py), запускаемый с параметром _auto=True_).

   1.2. Обучение нейронной сети (см. пример обучающего скрипта в файле [yolo_8_segment_brain_ischemia_msk/train.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/train.py)).

   1.3. Конвертация настроек обученной нейронной сети в формат ONNX (см. скрипт [yolo_8_segment_brain_ischemia_msk/export.bat](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/export.bat)). Параметры экспорта (ширина и высота внутреннего входа нейронной сети) должны быть получены из скрипта [yolo_8_segment_brain_ischemia_msk/get_net_info.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py).

2. Применение модуля [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind). Python-API модуля включает функцию setGlobalThreadPoolSize (единственный параметр функции задает количество вычислительных потоков, используемых для реализации многопоточной обработки в CPU-режиме) и описание класса YOLO, реализующего следующие методы:
   
- конструктор с параметрами:
  
  - "name" – имя модели;

  - "net_path" – путь к настройкам нейронной сети; 

  - "use_gpu" – флаг подключения графического ускорителя (True или False);

  - "use_fp16" – флаг включения режима вычислений с половинной точностью (True или False); 

  - "net_width" – определяет ширину внутреннего входа нейронной сети (в соответствии с результатом работы [yolo_8_segment_brain_ischemia_msk/get_net_info.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py)); 

  - "net_height" – определяет высоту внутреннего входа нейронной сети (в соответствии с результатом работы [yolo_8_segment_brain_ischemia_msk/get_net_info.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/yolo_8_segment_brain_ischemia_msk/get_net_info.py));

  - "cudaID" – номер графического ускорителя; 

- метод "warmup" – метод инициализации нейросетевой модели;

- метод "process" с параметрами:

  - "input" – входное изображение (одноканальное/трехканальное);

  - "rec_treshold" – порог уверенности распознавания;

  - "max_results" – максимальное количество возвращаемых результатов (зон интереса);

  - "get_brain" – флаг, определяющий возврат бинарной маски с одним объектом с максимальной уверенностью (с заливкой внутренних областей);

  - "erode_level" – количество итераций операции эрозии изображения, применяемых для постобработки выхода ИНС.

Метод "process" возвращает результаты работы ИНС, размер выхода совпадает с размером изображения, подаваемого через параметр "input".

Пример использования модуля [automated_workplace/yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/yolo_segment_with_nanobind) приведен в скрипте [ExtractYoloObjectsFromDICOM/convert_dir.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py).

Ссылка на пример нового варианта использования кода открытой библиотеки (сегментация на изображениях структурных повреждений в виде трещин дорожного покрытия): [automated_workplace/cracks_segmentator](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/cracks_segmentator). Путь к системным библиотекам (папки bin или lib) должен быть настроен в файле [automated_workplace/cracks_segmentator/02_test.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/cracks_segmentator/02_test.py) аналично тому, как было сделано с файлом [automated_workplace/boot.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/boot.py) (см. руководство к приложению "Automated Workplace"). В файле _settings.yaml_ должны быть следующие настройки: 
```
datasets_dir: [Path to _automated_workplace/cracks_segmentator/datasets/crack-seg_ folder]
weights_dir: weights
runs_dir: runs
```

# Пример 2 нового варианта использования кода открытой библиотеки
Пример когнитивной контурной визуализации очагов огня и дыма размещен в репозитории: https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace/contours_viewer
