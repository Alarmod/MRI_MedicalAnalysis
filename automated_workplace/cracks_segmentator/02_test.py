import numpy as np, cv2, glob, os

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

import yolo_segment_ext as yolo_inference
yolo_inference.setGlobalThreadPoolSize(4)

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

# Select compute mode:
useGPU = True
useFP16 = True

if useGPU and useFP16: 
	fp_string = "fp16"
else:
	fp_string = "fp32"

model = YoloModel("cracks_model", "./runs/segment/cracks_exp/weights/best_" + fp_string + ".onnx", input_width=640, input_height=640, useGPU=useGPU, useFP16=useFP16, cuda_id=0)

glob_res = glob.glob("./datasets/crack-seg/test/images/*.jpg")
glob_res = glob_res[:3]

for file in glob_res:
	img = cv2.imread(file, cv2.IMREAD_COLOR)

	res = model.predict(img, rec_treshold=0.15, max_results=16, get_brain=False, erode_level=0)
	res = cv2.cvtColor(res, cv2.COLOR_GRAY2BGR)
	cv2.imshow(file, np.vstack((img, res)))

cv2.waitKey()

del model
model = None