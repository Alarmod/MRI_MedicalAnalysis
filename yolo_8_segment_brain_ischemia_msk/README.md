# yolo_8_segment_brain_ischemia_msk
yolo_8_segment_brain_ischemia_msk - open source console application that designed for training and testing artificial neural networks (Yolo 8) on the tasks of identifying a brain region, localizing areas of ischemic damage, searching for clusters of mesenchymal stem cells

# Libs installation and project files downloading
1. Install libs

a) Pytorch 2.2.2 or 2.3.1+:

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121_ or

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cpu_

PyTorch 2.3.0 have bug that affects to Yolo 8 https://github.com/pytorch/pytorch/issues/121834

b) Ultralytics ==8.0.205 or (>8.1.5 and <8.2.43):

_pip3 install -U ultralytics==8.0.205_ or

_pip3 install -U ultralytics==8.1.6_ or

...

_pip3 install -U ultralytics==8.2.42_

Scale bug in ultralytics 8.0.206-8.1.5 fixed by authors this project: https://github.com/ultralytics/ultralytics/commit/1a04cd38e7285969c0e4d827f4fbe52c834798ed

In version 8.2.43 of the Ultralytics library, the order of obtaining segmentation masks was changed, the [developers abandoned the use of sigmoid function](https://github.com/ultralytics/ultralytics/pull/13939), which affected the results of neural networks trained on an earlier version of the library, therefore it is recommended to either retrain artificial neural networks, or use for tests library with version <8.2.43.

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

# Ways of using
If Your GPU FP16 speed slow, change _global_half = True_ to _global_half = False_ in train.py and test.py.

1. Training:

_python train.py_

2. Testing: 

_python test.py default_validate_ (Yolo 8 default validation code, the results are far from what you get in Yolo predict)

_python test.py generate_data_ (validate with saving all data: contours, zones, cognitive visualization)

_python test.py internal_validate_ (modificated Yolo 8 validation code that closer to Yolo original prediction code)

# Export settings to ONNX
To convert the settings of artificial neural networks into the ONNX format, you need run script _export.bat_

Export results can be loaded from
https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

MRI_settings_ONNX.zip can be unpacked with command

_start /W /B winrar.exe x -ibck MRI_settings_ONNX.zip_

Settings in original format will be saved in _orig_settings_ directory.

# System requirements
Python 3.8-3.10

Pytorch 2.2.2 or 2.3.1+
