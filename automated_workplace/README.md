# Automated Workplace
Automated Workplace – open source workplace for medical researchers intelligent support and high performance MRI-data processing. It designed for the increasing equipment of medical researchers.

# Requirements and system libs
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

# Loading configuration files for artificial neural networks
Load MRI_settings_ONNX.zip from

https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

and unpack it to resources folder (must be _resources/runs/segment/..._ structure)

# Manual for GUI run: 
Configure [Classifier] section in _settings.ini_ file and run command

_python main.py_

# Manual for speed test run: 
Configure [Classifier] section in _settings.ini_ file and run command 

_python test_processing_speed.py_

# System requirements
Python 3.8-3.10
