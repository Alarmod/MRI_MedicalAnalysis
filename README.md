# MRI_MedicalAnalysis
MRI_MedicalAnalysis – open source library for medical researchers intelligent support based on cognitive graphics, artificial neural networks and high performance MRI-data processing

# Description of the final product
The MRI_MedicalAnalysis library with an open software code is designed for the increasing equipment of medical researchers at the expense of a number of built-in software and instrumental modules and data processing means of Magnetic Resonance Imaging (MRI), providing intellectual support for making decisions of medical researchers. This library include adapted data preliminary processing algorithms, original algorithms deep processing and visualization of data, configured artificial neural networks, high speed software implementation of research modules. The library provide
- functional completeness – the presence of methods, algorithms and methods that provide the sufficiency of the functional support of the complex in solving the problems of processing and cognitive visualization of multidimensional biomedical data;
- high speed processing – ensuring the mode of work close to real time, for the account of proposed methods of automated processing acceleration in the presence relevant hardware resources (CPU and GPU).

# A library with an open software code have following functionality:
- preliminary data processing: filtering of medical images;
- highlighting and analysis of areas of interest on a medical data;
- recognition of target objects and situations;
- using Artificial Neural Networks (ANN) to situations recognition;
- 2D- and 3D-visualization of the Regions Of Interest (ROI);
- tracking and forecasting of dynamic situations and processes.

# Quantitative parameters (characteristics, performance performance indicators), determining the execution of the library of their functions:
- accuracy of the isolation and measurement of the size of zones of interest of about 85%;
- completeness and accuracy of recognition of the class of highlighted areas of interest – no worse than 90%;
- time of the full examination cycle according to MRI – no more than 1 minute on the Intel Core i3 8300 processor or its analogue.

# Output reactions (signals, information data, recommendations, etc.), provided a program library as a result of the performance of its functions:
- cognitive 2D- and 3D-visualization of the output data;
- data measurements, speed and ROI volume calculation for medical researchers;
- forecast characteristics of the studied processes.
   
# The library contain the following software-realized modules:
- preprocessing module: normalization of MRI-data;
- Python-module for training and testing artificial neural networks with [Yolo 8](https://github.com/ultralytics/ultralytics/blob/main/docs/en/models/yolov8.md) architecture;
- C++-module for high speed segmentation and classification, based on [Yolo 8](https://github.com/ultralytics/ultralytics/blob/main/docs/en/models/yolov8.md) architecture and [ONNXRuntime](https://onnxruntime.ai/) library (CPU and GPU supported);
- visualization module: 2D- and 3D-visualization of the brain; ROI 2D- and 3D-visualization; tracking of accumulations of Mesenchymal Stem Cells (MSC); combined visualization of the results.

# List of areas of applied use: 
- express diagnostics of the state of the brain;
- conducting scientific research on a set of diagnostically significant indicators in ischemic stroke of the brain;
- conducting research in the field of cell therapy, namely research on the processes of migration and homing of stem cells in the ischemic brain;
- isolation and measurement of stem cells in the brain labeled in a different way (e.g., using fluorescent dyes);
- detection of other regions of interest in the brain (e.g., neoplasms).

# Main potential consumers:
- hospitals;
- medical and research institutions;
- medical diagnostic centers.
  
# Used programming languages
C++ and Python

# Requirements
Minimum hardware requirements for the library:

1) a computer with an Intel Core i3 8300 CPU (according to the Technical Specifications) and at least 16 GB of RAM;
2) for training the neural network: an Nvidia GPU with at least 11 GB of memory and CUDA 11.8 support, such as an Nvidia GTX 1080 Ti, Nvidia RTX 3060, or Nvidia RTX 4070.
3) for using the trained neural network (when using a GPU to accelerate computations): an Nvidia GPU with at least 4 GB of memory and CUDA 11.8 support, such as an Nvidia GTX 1650.

Minimum operating system requirements: Windows 10 (x64).

General requirements for pre-installed software for compiling and running the developed library:

1) Visual Studio 2022 compiler with platform toolset version 14.1 installed;
2) CMake (3.26+);
3) modified Flann library (https://github.com/Alarmod/flann) (C++);
4) Dicomlib library (https://github.com/Alarmod/dicomlib) (C++);
5) OpenCV library (versions 3.1 and 4.9.0+) (C++);
6) Boost library (version 1.82) (C++);
7) python interpreter 3.8-3.10;
8) pytorch 2.2.2 or 2.3.1+ (Python);
9) ultralytics versions 8.0.205 or (>8.1.5 and <8.2.43) (Python);
10) ONNXRuntime library (version 1.18.0+) (C++);
11) CUDA library versions 11.8.0 or 12.4.0;
12) CUDDN library (version 8.9.7.29+).
13) PySide2 (Python);
14) vtk (Python);
15) pydicom (Python);
16) pandas (version >=2.0.3) (Python);
17) opencv-python (Python);
18) numpy (Python);
19) connected-components-3d (Python);
20) pulp (Python);
21) cachetools (Python);
22) nanobind (Python).
    
To use GPU resources in the ANN training process, it is recommended to install libraries in the following order: pytorch, ultralytics. 

The “yolo_segment_with_nanobind” module must be configured and compiled. 

The “boot.py” file must specify the paths to the bin and lib directories containing the dll files of the ONNXRuntime and OpenCV libraries, and optionally, the paths to the files for the CUDA and CUDNN libraries. Likewise, these paths must be written in [02_test.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/cracks_segmentator/02_test.py) and [convert_dir.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py).

# Place of the work
Intellectual Control Laboratory at Ailamazyan Program Systems Institute of the Russian Academy of Sciences (https://icontrol.psiras.ru/)

# Acknowledgments 
This project got fund from the [Foundation for Assistance to Small Innovative Enterprises (FASIE)](https://fasie.ru)

# ///////////////////////////////////////////////

# MRI_MedicalAnalysis
MRI_MedicalAnalysis – библиотека с открытым исходным кодом для интеллектуальной поддержки врачей-исследователей на основе когнитивной графики, искусственных нейронных сетей и высокопроизводительной обработки МРТ-данных

# Описание конечного продукта
Библиотека MRI_MedicalAnalysis с открытым программным кодом предназначена для расширения оснащения врачей-исследователей за счет ряда встроенных программно-аппаратных модулей и средств обработки данных магнитно-резонансной томографии (МРТ), обеспечивающих интеллектуальную поддержку принятия решений врачами-исследователями. В состав библиотеки входят адаптированные алгоритмы предварительной обработки данных, оригинальные алгоритмы глубокой обработки и визуализации данных, настраиваемые искусственные нейронные сети, высокоскоростная программная реализация исследовательских модулей. Библиотека обеспечивает
- функциональную полноту – наличие методов, алгоритмов и способов, обеспечивающих достаточность функциональной поддержки комплекса при решении задач обработки и когнитивной визуализации многомерных биомедицинских данных;
- высокую скорость обработки – обеспечение режима работы, близкого к реальному времени, за счет предлагаемых методов автоматизированного ускорения обработки при наличии соответствующих аппаратных ресурсов (CPU и GPU).

# Библиотека с открытым программным кодом имеет следующий функционал:
- предварительная обработка данных: фильтрация медицинских изображений;
- выделение и анализ областей интереса на медицинских данных;
- распознавание целевых объектов и ситуаций;
- использование искусственных нейронных сетей (ИНС) для распознавания ситуаций;
- 2D- и 3D-визуализация областей интереса (ROI);
- отслеживание и прогнозирование динамических ситуаций и процессов.

# Количественные параметры (характеристики, показатели эффективности), определяющие выполнение библиотекой своих функций:
- точность выделения и измерения размера зон интереса около 85%;
- полнота и точность распознавания класса выделенных областей интереса – не хуже 90%;
- время полного цикла обследования по данным МРТ – не более 1 минуты на процессоре Intel Core i3 8300 или его аналоге.

# Выходные реакции (сигналы, информационные данные, рекомендации и т.п.), предоставляемые программной библиотекой в ​​результате выполнения ее функций:
- когнитивная 2D- и 3D-визуализация выходных данных;
- измерение данных, расчет скорости и объема ROI для врачей-исследователей;
- прогнозные характеристики изучаемых процессов.

# Библиотека содержит следующие программно реализованные модули:
- модуль предварительной обработки: нормализация МРТ-данных;
- Python-модуль для обучения и тестирования искусственных нейронных сетей с архитектурой [Yolo 8](https://github.com/ultralytics/ultralytics/blob/main/docs/en/models/yolov8.md);
- C++-модуль для высокоскоростной сегментации и классификации, основанный на архитектуре [Yolo 8](https://github.com/ultralytics/ultralytics/blob/main/docs/en/models/yolov8.md) и библиотеке [ONNXRuntime](https://onnxruntime.ai/) (поддерживается CPU и GPU);
- модуль визуализации: 2D- и 3D-визуализация мозга; 2D- и 3D-визуализация ROI; отслеживание скоплений мезенхимальных стволовых клеток (МСК); комбинированная визуализация результатов.

# Перечень направлений прикладного использования:
- экспресс-диагностика состояния головного мозга;
- проведение научных исследований комплекса диагностически значимых показателей при ишемическом инсульте головного мозга;
- проведение исследований в области клеточной терапии, а именно исследований процессов миграции и хоуминга стволовых клеток в ишемизированном мозге;
- выделение и измерение стволовых клеток в головном мозге, помеченных иным образом (например, с помощью флюоресцентных красителей);
- диагностика зон интереса в головном мозге (например, новообразований).

# Основные возможные потребители:
- больницы,
- медицинские и научно-исследовательские учреждения,
- медицинские диагностические центры.

# Используемые языки программирования
C++ и Python

# Требования
Минимальные требования по аппаратному обеспечению библиотеки: 

1) компьютер с процессором Intel Core i3 8300 (согласно Техническому заданию) и объемом оперативной памяти не менее 16 гигабайт; 
2) для обучения нейронной сети: видеокарта Nvidia с объемом памяти не менее 11 гигабайт и с поддержкой CUDA 11.8, например, Nvidia GTX 1080 Ti, Nvidia RTX 3060 или Nvidia RTX 4070.
3) для использования обученной нейронной сети (при применении GPU для ускорения вычислений): видеокарта Nvidia с объемом памяти не менее 4 гигабайт и с поддержкой CUDA 11.8, например, видеокарта Nvidia GTX 1650.

Минимальные требования к операционному обеспечению: Windows 10 (x64).

Общие требования к предустановленному программному обеспечению для компиляции и запуска разработанной библиотеки:

1) компилятор Visual Studio 2022 с установленным набором инструментов платформы версии 14.1;
2) CMake (3.26+);
3) модифицированная библиотека Flann (https://github.com/Alarmod/flann) (C++);
4) библиотека Dicomlib (https://github.com/Alarmod/dicomlib) (C++);  
5) библиотека OpenCV (версии 3.1 и 4.9.0+) (C++);
6) библиотека Boost (версия 1.82) (C++);
7) интерпретатор python 3.8-3.10;
8) pytorch 2.2.2 или 2.3.1+ (Python);
9) ultralytics версий 8.0.205 или (>8.1.5 и <8.2.43) (Python);
10) библиотека ONNXRuntime (версия 1.18.0+) (C++);
11) библиотека CUDA версий 11.8.0 или 12.4.0;
12) библиотека CUDDN (версия 8.9.7.29+). 
13) PySide2 (Python);
14) vtk (Python);
15) pydicom (Python);
16) pandas (версия >=2.0.3) (Python);
17) opencv-python (Python);
18) numpy (Python);
19) connected-components-3d (Python);
20) pulp (Python);
21) cachetools (Python);
22) nanobind (Python).

Для использования ресурсов GPU в процессе обучения ИНС рекомендуется устанавливать библиотеки в следующем порядке: pytorch, ultralytics.

Должен быть сконфигурирован и скомпилирован модуль “yolo_segment_with_nanobind”. Через файл “boot.py” должны быть прописаны пути к bin- и lib-директориям, содержащим dll-файлы библиотек ONNXRuntime и OpenCV, опционально – пути к файлам для библиотек CUDA и CUDNN. Аналогично эти пути прописываются в [02_test.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/cracks_segmentator/02_test.py) и [convert_dir.py](https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/ExtractYoloObjectsFromDICOM/convert_dir.py).

# Место, где выполняется разработка
Лаборатория интеллектуального управления Института программных систем им. А.К. Айламазяна РАН (https://icontrol.psiras.ru/)

# Благодарности
Проект профинансирован [Фондом содействия развитию малых форм предприятий в научно-технической сфере (FASIE)](https://fasie.ru)
