# Automated Workplace
Automated Workplace – open source workplace for medical researchers intelligent support and high performance MRI-data processing. It designed for the increasing equipment of medical researchers.

# Requirements
1. Install ONNXRuntime libs for CPU or GPU: 
https://github.com/microsoft/onnxruntime/releases

2. Confiure and install _yolo_segment_with_nanobind_ plugin:

You must set actual path for ONNXRuntime, OpenCV and runtime version, compatible with your compiler
_yolo_segment_with_nanobind\build.bat_

3. Configure path for system libs:

- actual path for ONNXRuntime and OpenCV (always required)

- actual path for CUDA and CUDNN (optional)

This settings must be set over
_boot.py_

4. Install python libs:
_pip3 install -r requirements.txt_

# Installation
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
