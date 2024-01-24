# ExtractYoloObjectsFromDICOM
ExtractYoloObjectsFromDICOM - open source console tool for medical researchers intelligent support and high performance MRI-data processing. It designed for training and testing datasets convertation from IMA-files (DICOM) and binary masks (PNG) to Yolo format - grayscale images (PNG) and textual labels (TXT).

# Compilation
Project configured for compilation in Visual Studio 2022 with v141 platform toolset. Application based on 

-- modificated Flann library https://github.com/flann-lib/flann

-- OpenCV library (version: 3.1) https://opencv.org

For correct execution You must copy opencv_world310.dll to x64\Release folder

# Ways of using
1. Run _ExtractYoloObjectsFromDICOM.exe 1_, which allows you to replace the original files with masks with expanded ones using dilate morphological operation. Original PNG file with binary mask will be replaced.

2. To generate training and testing datasets in Yolo format, run _ExtractYoloObjectsFromDICOM.exe 0_

# Used programming languages
C++

# System requirements
Windows 10-11
