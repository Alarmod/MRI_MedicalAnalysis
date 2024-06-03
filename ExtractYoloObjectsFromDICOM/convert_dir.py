import numpy as np
import cv2
import glob, os

from YoloModel import YoloModel

useGPU = True
useFP16 = False
fp_string = "fp32"

from yolo_segment_with_nanobind import yolo_segment_ext as yolo_inference
yolo_inference.setGlobalThreadPoolSize(4);

model1 = YoloModel("t2_brain_" + fp_string + "_model_1", "./runs/segment/t2_brain_512/weights/best_" + fp_string + ".onnx", input_width=512, input_height=384, useGPU=useGPU, useFP16=useFP16)
model2 = YoloModel("adc_brain_" + fp_string + "_model_2", "./runs/segment/adc_brain_512/weights/best_" + fp_string + ".onnx", input_width=512, input_height=352, useGPU=useGPU, useFP16=useFP16)
model3 = YoloModel("swi_brain_" + fp_string + "_model_3", "./runs/segment/swi_brain_512/weights/best_" + fp_string + ".onnx", input_width=512, input_height=352, useGPU=useGPU, useFP16=useFP16)

glob_res = glob.glob("./dataset/*/*/*.png")

for file in glob_res:
    if file.endswith(".png"):
       if not file.endswith("_brain.png"):
          img = cv2.imread(file, cv2.IMREAD_GRAYSCALE)

          res = None
          res_ok = False
          if "t2" in file:
             res = model1.predict(img, rec_treshold=0.15, max_results=1, get_brain=True, erode_level=0)
             res_ok = True
          elif "ADC" in file:
             res = model2.predict(img, rec_treshold=0.15, max_results=1, get_brain=True, erode_level=0)
             res_ok = True
          elif "SWI" in file:
             res = model3.predict(img, rec_treshold=0.15, max_results=1, get_brain=True, erode_level=0)
             res_ok = True

          if res_ok: 
             img_res_path = file.rsplit('.', 1)[0] + "_brain.png"
             cv2.imwrite(img_res_path, res) 
             print(f"Saved '{img_res_path}'")

del model1
model1 = None

del model2
model2 = None

del model3
model3 = None