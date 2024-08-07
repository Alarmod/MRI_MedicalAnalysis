# MRI_MedicalAnalysis
MRI_MedicalAnalysis – open source library for medical researchers intelligent support based on cognitive graphics, artificial neural networks and high performance MRI-data processing

# Description of the final product:
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
- conducting research in the field of cell therapy, namely research on the processes of migration and homing of stem cells in the ischemic brain.
Main potential consumers:
- hospitals;
- medical and research institutions;
- medical diagnostic centers.
  
# Used programming languages
C++ and Python

# System requirements
Python 3.8-3.10

# Place of the work
Intellectual Management Laboratory at Ailamazyan Program Systems Institute of the Russian Academy of Sciences (https://icontrol.psiras.ru/)

# Acknowledgments 
This project got fund from the [Foundation for Assistance to Small Innovative Enterprises (FASIE)](https://fasie.ru)

# ///////////////////////////////////////////////

# MRI_MedicalAnalysis
MRI_MedicalAnalysis – библиотека с открытым исходным кодом для интеллектуальной поддержки врачей-исследователей на основе когнитивной графики, искусственных нейронных сетей и высокопроизводительной обработки МРТ-данных

# Описание конечного продукта:
Библиотека MRI_MedicalAnalysis с открытым программным кодом предназначена для расширения оснащения врачей-исследователей за счет ряда встроенных программно-аппаратных модулей и средств обработки данных магнитно-резонансной томографии (МРТ), обеспечивающих интеллектуальную поддержку принятия решений врачами-исследователями. В состав библиотеки входят адаптированные алгоритмы предварительной обработки данных, оригинальные алгоритмы глубокой обработки и визуализации данных, настраиваемые искусственные нейронные сети, высокоскоростная программная реализация исследовательских модулей. Библиотека обеспечивает
- функциональную полноту – наличие методов, алгоритмов и способов, обеспечивающих достаточность функциональной поддержки комплекса при решении задач обработки и когнитивной визуализации многомерных биомедицинских данных;
- высокая скорость обработки – обеспечение режима работы, близкого к реальному времени, за счет предлагаемых методов автоматизированного ускорения обработки при наличии соответствующих аппаратных ресурсов (CPU и GPU).

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
- проведение исследований в области клеточной терапии, а именно исследований процессов миграции и хоуминга стволовых клеток в ишемизированном мозге.
Основные возможные потребители:
- больницы,
- медицинские и научно-исследовательские учреждения,
- медицинские диагностические центры.

# Используемые языки программирования
C++ и Python

# Системные требования
Python 3.8-3.10

# Место работы
Лаборатория интеллектуального управления Института программных систем им. А.К. Айламазяна РАН (https://icontrol.psiras.ru/)

# Благодарности
Проект профинансирован [Фондом содействия развитию малых форм предприятий в научно-технической сфере (FASIE)](https://fasie.ru)
