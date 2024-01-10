# ExtractYoloObjectsFromDICOM
ExtractYoloObjectsFromDICOM – open source console tool for medical researchers intelligent support and high performance MRI-data processing

# Description:
The ExtractYoloObjectsFromDICOM – windows console application that designed for training and testing datasets convertation from IMA-files (DICOM) and binary masks (PNG) and to grayscale images (PNG) and textual labels (TXT).

# Compilation
Project configured for compilation in Visual Studio 2022 with v141 platform toolset. Application based on 

-- modificated Flann library https://github.com/flann-lib/flann

# Ways of using
1. Run ExtractYoloObjectsFromDICOM.exe with parameter 1, which allows you to replace the original files with masks with expanded ones using dilate morphological operation

2. To generate training contours, run ExtractYoloObjectsFromDICOM.exe with parameter 0

# Used programming languages
C++

# System requirements
Windows 10-11
