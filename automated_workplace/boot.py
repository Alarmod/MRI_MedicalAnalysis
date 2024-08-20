import os

#add dirs: 
#required - OpenCV, onnxruntime
#optional - cuda, cudnn
libs_path=["D:/libs/opencv-4_9_0/build/x64/vc16/bin", 
		"D:/libs/onnxruntime-win-x64-gpu-1.18.0/lib", 
		"D:/libs/cuda/11.8/bin", 
		"D:/libs/cudnn/8.9.7.29/bin"]

path_fix = []
for path in libs_path:
	if os.path.isdir(path):
		path_fix.append(path)
		os.add_dll_directory(path)
	else:
		print(f"Warning: Path \'{path}\' not exists")
os.environ["PATH"] = os.pathsep.join(path_fix) + os.pathsep + os.environ["PATH"]