set CD_PATH=%~dp0

set OPTSET=16

@REM pip3 install -U torch torchvision torchaudio
@REM pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
@REM pip3 install -U torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

@REM pip3 install -U ultralytics

cd ./runs/segment/cracks_exp/weights/

if exist "last.pt" (
del last.pt
)

if exist "best.pt" (
yolo export model=best.pt format=onnx imgsz=640,640 half=False opset=%OPTSET% dynamic=False batch=1
move best.onnx best_fp32.onnx

yolo export model=best.pt format=onnx imgsz=640,640 half=True opset=%OPTSET% dynamic=False device=0 batch=1
move best.onnx best_fp16.onnx

del best.pt
)

cd %CD_PATH%
