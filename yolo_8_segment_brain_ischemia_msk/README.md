# yolo_8_segment_brain_ischemia_msk
yolo_8_segment_brain_ischemia_msk - open source console application that designed for training and testing artificial neural networks (Yolo 8) on the tasks of identifying a brain region, localizing areas of ischemic damage, searching for clusters of mesenchymal stem cells

# Libs installation and project files downloading
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

Scale bug in ultralytics 8.0.206-8.1.5 fixed by authors this project: https://github.com/ultralytics/ultralytics/commit/1a04cd38e7285969c0e4d827f4fbe52c834798ed

In version 8.2.43 and more new of the Ultralytics library, the order of obtaining segmentation masks was changed, the [developers abandoned the use of sigmoid function](https://github.com/ultralytics/ultralytics/pull/13939), which affected the results of neural networks trained on an earlier version of the library, therefore it is recommended to either retrain our artificial neural networks, or use for tests Ultralytics library with version <8.2.43.

2. Load MRI_settings.zip and MRI_tests.zip: 

MRI_settings.zip --- https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing or  https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings.zip

MRI_tests.zip --- https://drive.google.com/file/d/15to8SyUL6DXNHhYwYQR9KJZ4bDlC3Yyq/view?usp=sharing

3. Unpack full _runs_ directory with commands: 

_start /W /B winrar.exe x -ibck MRI_settings.zip_

_start /W /B winrar.exe x -ibck MRI_tests.zip_

4. Datasets for training can be loaded from 
https://drive.google.com/file/d/1SSYDAWpAw0Sbaz3dpHn_EH6ZuGZWt-LW/view?usp=drive_link
or from https://huggingface.co/datasets/alarmod/MRI/tree/main/train_and_test_data

(must be stored to _datasets_ folder and unpacked)

5. Configure settings.yaml (internal settings file of the _ultralytics_ library) if You already use Ultralytics in other projects
```
datasets_dir: [Path to _yolo_8_segment_brain_ischemia_msk/datasets_ folder]
weights_dir: weights
runs_dir: runs
```
For example, this file can be found here: _C:\Users\\%USERNAME%\AppData\Roaming\Ultralytics\settings.yaml_ (for Windows)

# Ways of using
If Your GPU FP16 speed slow, change _global_half = True_ to _global_half = False_ in train.py and test.py.

1. Training:

_python train.py_

2. Testing: 

_python test.py default_validate_ (Yolo 8 default validation code, the results are far from what You get in Yolo predict)

_python test.py generate_data_ (validate with saving all data: contours, zones, cognitive visualization)

_python test.py internal_validate_ (modificated Yolo 8 validation code that closer to Yolo original prediction code)

Train and test on Nvidia RTX 16xx in FP16 mode may be slow (see https://github.com/ultralytics/yolov5/issues/7908)

# Export settings to ONNX format
To convert the settings of artificial neural networks into the ONNX format, You need run script _export.bat_

Export results can be loaded from
https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

MRI_settings_ONNX.zip can be unpacked with command

_start /W /B winrar.exe x -ibck MRI_settings_ONNX.zip_

Settings in original format will be saved in _orig_settings_ directory.

# System requirements
Python 3.8-3.10

PyTorch 2.2.2 or 2.3.1+

# ///////////////////////////////////////////////

# yolo_8_segment_brain_ischemia_msk
yolo_8_segment_brain_ischemia_msk - консольное приложение с открытым исходным кодом, предназначенное для обучения и тестирования искусственных нейронных сетей (Yolo 8) на задачах определения области мозга, локализации зон ишемического повреждения, поиска скоплений мезенхимальных стволовых клеток

# Установка библиотек и загрузка файлов проекта
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

Ошибка масштабирования в ultralytics 8.0.206-8.1.5 исправлена ​​авторами этого проекта: https://github.com/ultralytics/ultralytics/commit/1a04cd38e7285969c0e4d827f4fbe52c834798ed

В версии 8.2.43 и более новых сборках библиотеки Ultralytics был изменен порядок получения масок сегментации, [разработчики отказались от использования сигмоидальной функции](https://github.com/ultralytics/ultralytics/pull/13939), что повлияло на результаты нейронных сетей, обученных на более ранней версии библиотеки, поэтому рекомендуется либо переобучить наши искусственные нейронные сети, либо использовать для тестов библиотеку Ultralytics версии <8.2.43.

2. Загрузите MRI_settings.zip и MRI_tests.zip:

MRI_settings.zip --- https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing или https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings.zip

MRI_tests.zip --- https://drive.google.com/file/d/15to8SyUL6DXNHhYwYQR9KJZ4bDlC3Yyq/view?usp=sharing

3. Распакуйте полный каталог _runs_ с помощью команд:

_start /W /B winrar.exe x -ibck MRI_settings.zip_

_start /W /B winrar.exe x -ibck MRI_tests.zip_

4.  Наборы данных для обучения можно загрузить с
https://drive.google.com/file/d/1SSYDAWpAw0Sbaz3dpHn_EH6ZuGZWt-LW/view?usp=drive_link
или с https://huggingface.co/datasets/alarmod/MRI/tree/main/train_and_test_data

(должны быть сохранены в папке _datasets_ и распакованы)

5. Настройте settings.yaml (внутренний файл настроек библиотеки _ultralytics_), если вы уже используете Ultralytics в других проектах
```
datasets_dir: [Путь к папке _yolo_8_segment_brain_ischemia_msk/datasets_]
weights_dir: weights
runs_dir: runnings
```
Например, этот файл можно найти здесь: _C:\Users\\%USERNAME%\AppData\Roaming\Ultralytics\settings.yaml_ (для Windows)

# Способы использования
Если скорость FP16 вашего GPU низкая, измените _global_half = True_ на _global_half = False_ в train.py и test.py.

1. Обучение:

_python train.py_

2. Тестирование:

_python test.py default_validate_ (код проверки Yolo 8 по умолчанию, результаты далеки от того, что вы получаете в Yolo predict)

_python test.py generate_data_ (проверка с сохранением всех данных: контуры, зоны, когнитивная визуализация)

_python test.py internal_validate_ (модифицированный код проверки Yolo 8, который ближе к оригинальному коду прогнозирования Yolo)

Обучение и тестирование на Nvidia RTX 16xx в режиме FP16 может быть медленным (см. https://github.com/ultralytics/yolov5/issues/7908)

# Экспорт настроек в формат ONNX
Чтобы преобразовать настройки искусственных нейронных сетей в формат ONNX, вам нужно запустить скрипт _export.bat_

Результаты экспорта можно загрузить из
https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
или
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

MRI_settings_ONNX.zip можно распаковать командой

_start /W /B winrar.exe x -ibck MRI_settings_ONNX.zip_

Настройки в исходном формате будут сохранены в каталоге _orig_settings_.

# Системные требования
Python 3.8-3.10

PyTorch 2.2.2 или 2.3.1+
