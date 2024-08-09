import numpy as np
import cv2
import glob, os

#add dirs: 
#required - OpenCV, onnxruntime
#optional - cuda, cudnn
libs_path=["D:/libs/opencv-4_9_0/build/x64/vc16/bin", 
		"D:/libs/onnxruntime-win-x64-gpu-1.18.0/lib", 
		"D:/libs/cuda/11.8/bin", 
		"D:/libs/cudnn/8.9.7.29/bin"]

for path in libs_path:
	if os.path.exists(path):
		os.add_dll_directory(path)
	else:
		print(f"Warning: Path \'{path}\' not exists")

class YoloModel:
	def __init__(self, name, weights_file, input_width, input_height, useGPU, useFP16, cuda_id):
		self.name = name
		self.weights_file = weights_file
		self.input_width = input_width
		self.input_height = input_height
		self.useGPU = useGPU
		self.useFP16 = useFP16
		self.cuda_id = cuda_id

		self.__model = yolo_inference.YOLO(name=self.name, net_path=self.weights_file, use_gpu=self.useGPU, use_fp16=self.useFP16, net_width=self.input_width, net_height=self.input_height, cudaID=self.cuda_id)

	def warmup(self):
		print(f"Warming up model {self.name} (GPU={self.useGPU})")
		self.__model.warmup()

	def predict(self, input_image, **kwargs):
		return self.__model.process(input=input_image, **kwargs)

useGPU = True
useFP16 = False
fp_string = "fp32"

import yolo_segment_ext as yolo_inference
yolo_inference.setGlobalThreadPoolSize(4);

model1 = YoloModel("t2_brain_" + fp_string + "_model_1", "../automated_workplace/resources/runs/segment/t2_brain_512/weights/best_" + fp_string + ".onnx", input_width=512, input_height=384, useGPU=useGPU, useFP16=useFP16, cuda_id=0)
model2 = YoloModel("adc_brain_" + fp_string + "_model_2", "../automated_workplace/resources/runs/segment/adc_brain_512/weights/best_" + fp_string + ".onnx", input_width=512, input_height=352, useGPU=useGPU, useFP16=useFP16, cuda_id=0)
model3 = YoloModel("swi_brain_" + fp_string + "_model_3", "../automated_workplace/resources/runs/segment/swi_brain_512/weights/best_" + fp_string + ".onnx", input_width=512, input_height=352, useGPU=useGPU, useFP16=useFP16, cuda_id=0)

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