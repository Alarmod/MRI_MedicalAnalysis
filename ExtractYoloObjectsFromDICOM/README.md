# ExtractYoloObjectsFromDICOM
ExtractYoloObjectsFromDICOM - open source console tool for medical researchers intelligent support and high performance MRI-data processing. It designed for training and testing datasets convertation from IMA-files (DICOM) and binary masks (PNG) to Yolo format - grayscale images (PNG) and textual labels (TXT).

# Compilation
Project configured for compilation in Visual Studio 2022 with v141 platform toolset. Application based on 

-- modificated Flann library: 

_git clone https://github.com/Alarmod/flann_

-- original Dicomlib library:

_git clone https://github.com/Alarmod/dicomlib_

-- OpenCV library (version 3.1 are used):

_https://opencv.org_

For correct execution You must copy opencv_world310.dll to x64\Release folder

-- BOOST library (version 1.82 are used): 

_https://www.boost.org_

# Ways of using
1. Run _ExtractYoloObjectsFromDICOM.exe 1_ on original DICOM-dataset, that allows You to replace the original files with masks with expanded ones using dilate morphological operation. Original PNG-files with binary masks will be replaced, save original folders before!

Example of _ExtractYoloObjectsFromDICOM.exe 1_ execution can be downloaded here: 
https://drive.google.com/file/d/1SJ1TNAfcWX5mexRBFR_jyzMFN-3xsurI/view?usp=sharing

2. To generate training and testing datasets in Yolo format, run _ExtractYoloObjectsFromDICOM.exe 0_

3. You have also the ability to directly convert directories with IMA-files into YOLO format, with further processing using an artificial neural network: 

_ExtractYoloObjectsFromDICOM.exe convert ./dataset/_

_python convert_dir.py_

Script _convert_dir.py_ use installed [yolo_segment_with_nanobind](https://github.com/Alarmod/MRI_MedicalAnalysis/tree/main/automated_workplace) plugin.

# Used programming languages
C++
