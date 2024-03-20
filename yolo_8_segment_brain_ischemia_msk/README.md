# yolo_8_segment_brain_ischemia_msk
yolo_8_segment_brain_ischemia_msk - open source console application that designed for training and testing artificial neural networks (Yolo 8) on the tasks of identifying a brain region, localizing areas of ischemic damage, searching for clusters of mesenchymal stem cells

# Libs installation and project files downloading
1. Install libs

_pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118_

_pip3 install -U ultralytics==8.1.6_

Supported ultralytics 8.0.205 and >= 8.1.6

Scale bug in ultralytics 8.0.206-8.1.5 fixed: https://github.com/ultralytics/ultralytics/commit/1a04cd38e7285969c0e4d827f4fbe52c834798ed

2. Load MRI_settings.zip and MRI_tests.zip: 

MRI_settings.zip --- https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing
MRI_tests.zip --- https://drive.google.com/file/d/15to8SyUL6DXNHhYwYQR9KJZ4bDlC3Yyq/view?usp=sharing

3. Load datasets.zip from 

https://drive.google.com/file/d/1CCXe6R8FJgIIdjx0RhxEqnEWAvPkyqbh/view?usp=sharing
or from https://huggingface.co/datasets/alarmod/MRI/tree/main/train_and_test_data (this files must be stored to datasets folder and unpacked)

4. Unpack create runs directory with command: 

_start /W /B winrar.exe x -ibck MRI_settings.zip_
_start /W /B winrar.exe x -ibck MRI_tests.zip_

5. Unpack datasets with command: 

_start /W /B winrar.exe x -ibck datasets.zip_


# Ways of using
1. Training:
_python train.py_

3. Testing: 
_python train.py default_validate_
(Yolo 8 default validation code)

_python train.py generate_data_
(validate with saving all data: contours, zones, cognitive visualization)

_python train.py internal_validate_
(modificated Yolo 8 validation code that closer to Yolo prediction code)

# Used programming languages
Python 3.9 and more new

# System requirements
Linux / Windows 10-11
