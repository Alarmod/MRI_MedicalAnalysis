# Automated Workplace
Automated Workplace – open source workplace for medical researchers intelligent support and high performance MRI-data processing. It designed for the increasing equipment of medical researchers.

# Requirements and system libs
1. Install python libs with command _pip3 install -r requirements.txt_

2. Install ONNXRuntime libs for CPU or GPU: 
https://github.com/microsoft/onnxruntime/releases

3. Confiure, build and install _yolo_segment_with_nanobind_ plugin:

You must set actual path for ONNXRuntime and OpenCV, runtime version, compatible with your compiler. This directories with _bin_, _lib_ and _include_ subfolders. Settings must be set over _yolo_segment_with_nanobind\build.bat_ file. After configuration, the file or commands inside must be launched (executed).

4. Configure path for system libs (_bin_ or _lib_ folders with dll/so-files):

- actual path for ONNXRuntime and OpenCV (always required)

- actual path for [CUDA](https://developer.nvidia.com/cuda-toolkit) and [CUDNN](https://developer.nvidia.com/cudnn) (optional, for example, ONNXRuntime 1.18.0 requires presence [CUDA 11.8 or 12.4](https://onnxruntime.ai/docs/execution-providers/CUDA-ExecutionProvider.html))

This settings must be set over _boot.py_ file.

# Loading configuration files for artificial neural networks
Load MRI_settings_ONNX.zip from

https://drive.google.com/file/d/16cPNC6zRKNaNQz6J40XU74XuVZYVBzWi/view?usp=drive_link
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings_ONNX.zip

and unpack it to resources folder (must be _resources/runs/segment/..._ structure)

# Manual for run: 
_python main.py_

# System requirements
Windows 10-11, Linux

Python 3.8-3.10

ONNXRuntime 1.18.0+
