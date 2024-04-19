# Automated Workplace
Automated Workplace – open source workplace for medical researchers intelligent support and high performance MRI-data processing. It designed for the increasing equipment of medical researchers.

# Requirements
For GPU: _pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118_

For CPU: _pip3 install -U torch torchvision torchaudio_

Supported torch >= 2.2.2

_pip3 install -r requirements.txt_

# Installation
Load MRI_settings.zip from

https://drive.google.com/file/d/15uKf4ayCEnhhBj73WTrgIHoAqWOzlhDh/view?usp=sharing
or
https://huggingface.co/alarmod/MRI_settings/blob/main/MRI_settings.zip

and unpack it to resources folder (must be _resources/runs/segment/..._ structure)

# Manual for run: 
_python main.py_

# System requirements
Windows 10-11, Linux

Python 3.8-3.10
