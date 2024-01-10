# yolo_8_segment_brain_ischemia_msk
yolo_8_segment_brain_ischemia_msk - open source console tool MRI_MedicalAnalysis train process. It use Yolo 8 training code.

# Description:
The yolo_8_segment_brain_ischemia_msk - console application that designed for training and testing artificial neural networks on the tasks of identifying a brain region, localizing areas of ischemic damage, searching for clusters of mesenchymal stem cells

# Compilation
1. Install libs

pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118

pip3 install -U ultralytics==8.0.205

2. Load runs.zip from 

https://drive.google.com/file/d/1CH8bIjGHNiKi572CpZmvh6vhtk6dGN4F/view?usp=sharing

3. Load datasets.zip from 

https://drive.google.com/file/d/1CCXe6R8FJgIIdjx0RhxEqnEWAvPkyqbh/view?usp=sharing

4. Unpack runs with command: 

start /W /B winrar.exe x -ibck runs.zip

5. Unpack datasets with command: 

start /W /B winrar.exe x -ibck datasets.zip


# Ways of using
1. Training: python train.py

2. Testing: 
  python train.py default_validate
  (Yolo default validation code)

  python train.py generate_data
  (validate with saving all data: contours, zones, cognitive visualization)

  python train.py internal_validate
  (modicated Yolo validation code that close to prediction format)

# Used programming languages
Python

# System requirements
Linux / Windows 10-11
