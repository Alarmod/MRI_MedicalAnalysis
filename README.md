# MRI_MedicalAnalysis
MRI_MedicalAnalysis – open source library for medical researchers intelligent support based on cognitive graphics, artificial neural networks and high performance MRI-data processing

# Description of the final product:
The MRI_MedicalAnalysis library with an open software code is designed for the increasing equipment of medical researchers at the expense of a number of built-in software and instrumental modules and data processing means of Magnetic Resonance Imaging (MRI), providing intellectual support for making decisions of medical researchers. This library should include adapted data preliminary processing algorithms, original algorithms deep processing and visualization of data, configured artificial neural networks, software implementation of research modules. The library should provide:
- functional completeness – the presence of methods, algorithms and methods that provide the sufficiency of the functional support of the complex in solving the problems of processing and cognitive visualization of multidimensional biomedical data;
- parallel processing – ensuring the mode of work close to real time, for the account of proposed methods of automated processing acceleration in the presence relevant hardware resources (CPU and GPU).

# A library with an open software code should have following functionality:
- preliminary data processing: filtering of medical images;
- highlighting and analysis of areas of interest on a medical data;
- recognition of target objects and situations;
- using Artificial Neural Networks (ANN) and metrics to situations recognition;
- 2D- and 3D-visualization of the Regions Of Interest (ROI);
- tracking and forecasting of dynamic situations and processes.

# Quantitative parameters (characteristics, performance performance indicators), determining the execution of the library of their functions:
- accuracy of the isolation and measurement of the size of zones of interest of about 85%;
- completeness and accuracy of recognition of the class of highlighted areas of interest – no worse than 90%;
- time of the full examination cycle of the patient according to MRI – no more than 1 minutes on the Intel Core i3 8300 processor or its analogue (now we are use Nvidia GTX 1080 Ti and Nvidia RTX 3060 to achieve the specified indicators).

# Output reactions (signals, information data, recommendations, etc.), provided a program library as a result of the performance of its functions:
- cognitive 2D- and 3D-visualization of the output data;
- data measurements, speed and ROI volume calculation for medical researchers;
- forecast characteristics of the studied processes.
   
# The library will contain the following software-realized modules:
- preprocessing module: leveling brightness, normalization; selection (purpose) area of the brain; wave algorithm for the allocation of objects from the background; subtraction images and filtering of the result by brightness; highlighting texture features; filtering objects by applying a window of a special shape;
- module of segmentation and recognition: convolutional neural networks (now it based on [Yolo 8](https://github.com/ultralytics/ultralytics/blob/main/docs/en/models/yolov8.md) neural network; Metric Euclid Mahalanobis (excluded now, as the results of experiments showed low quality of segmentation);
- visualization module: 2D- and 3D-visualization of the brain; ROI 2D- and 3D-visualization; highlighting the trend of the Bezier curve; tracking of accumulations of Mesenchymal Stem Cells (MSC); cognitive data display; combined visualization of the results.
  
# Used programming languages
C++ and Python

# System requirements
Windows 10-11, Linux

Python 3.8-3.11

# Place of the work
Intellectual Management Laboratory at Ailamazyan Program Systems Institute of the Russian Academy of Sciences (https://icontrol.psiras.ru/)

# Acknowledgments 
This project got fund from the [Foundation for Assistance to Small Innovative Enterprises (FASIE)](https://fasie.ru)
