@REM Brain or Ischemia: 
@REM T2 [384x512], ADC [352x512], SWI [352x512]

@REM MSC:
@REM SWI [896x1280]

set CD_PATH=%~dp0

set OPTSET=16

@REM pip3 install torch torchvision torchaudio
@REM pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
@REM pip3 install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu121

pip3 install -U ultralytics

mkdir orig_settings
move %CD_PATH%runs %CD_PATH%orig_settings\\runs

xcopy %CD_PATH%orig_settings\\runs\\ %CD_PATH%runs\\ /T /E

cd %CD_PATH%orig_settings/runs/segment/adc_brain_512/weights
del best.onnx
yolo export model=best.pt format=onnx imgsz=352,512 half=False opset=%OPTSET% dynamic=False batch=1
move best.onnx %CD_PATH%runs/segment/adc_brain_512/weights/best_fp32.onnx

yolo export model=best.pt format=onnx imgsz=352,512 half=True opset=%OPTSET% dynamic=False device=0 batch=1
move best.onnx %CD_PATH%runs/segment/adc_brain_512/weights/best_fp16.onnx
cd ../../../../..


cd %CD_PATH%orig_settings/runs/segment/adc_ischemia_512_augmented/weights
del best.onnx
yolo export model=best.pt format=onnx imgsz=352,512 half=False opset=%OPTSET% dynamic=False batch=1
move best.onnx %CD_PATH%runs/segment/adc_ischemia_512_augmented/weights/best_fp32.onnx

yolo export model=best.pt format=onnx imgsz=352,512 half=True opset=%OPTSET% dynamic=False device=0 batch=1
move best.onnx %CD_PATH%runs/segment/adc_ischemia_512_augmented/weights/best_fp16.onnx
cd ../../../../..


cd %CD_PATH%orig_settings/runs/segment/swi_brain_512/weights
del best.onnx
yolo export model=best.pt format=onnx imgsz=352,512 half=False opset=%OPTSET% dynamic=False batch=1
move best.onnx %CD_PATH%runs/segment/swi_brain_512/weights/best_fp32.onnx

yolo export model=best.pt format=onnx imgsz=352,512 half=True opset=%OPTSET% dynamic=False device=0 batch=1
move best.onnx %CD_PATH%runs/segment/swi_brain_512/weights/best_fp16.onnx
cd ../../../../..


cd %CD_PATH%orig_settings/runs/segment/swi_msc_mod_1280_augmented/weights
del best.onnx
yolo export model=best.pt format=onnx imgsz=896,1280 half=False opset=%OPTSET% dynamic=False batch=1
move best.onnx %CD_PATH%runs/segment/swi_msc_mod_1280_augmented/weights/best_fp32.onnx

yolo export model=best.pt format=onnx imgsz=896,1280 half=True opset=%OPTSET% dynamic=False device=0 batch=1
move best.onnx %CD_PATH%runs/segment/swi_msc_mod_1280_augmented/weights/best_fp16.onnx
cd ../../../../..


cd %CD_PATH%orig_settings/runs/segment/t2_brain_512/weights
del best.onnx
yolo export model=best.pt format=onnx imgsz=384,512 half=False opset=%OPTSET% dynamic=False batch=1
move best.onnx %CD_PATH%runs/segment/t2_brain_512/weights/best_fp32.onnx

yolo export model=best.pt format=onnx imgsz=384,512 half=True opset=%OPTSET% dynamic=False device=0 batch=1
move best.onnx %CD_PATH%runs/segment/t2_brain_512/weights/best_fp16.onnx
cd ../../../../..


cd %CD_PATH%orig_settings/runs/segment/t2_ischemia_512_augmented/weights
del best.onnx
yolo export model=best.pt format=onnx imgsz=384,512 half=False opset=%OPTSET% dynamic=False batch=1
move best.onnx %CD_PATH%runs/segment/t2_ischemia_512_augmented/weights/best_fp32.onnx

yolo export model=best.pt format=onnx imgsz=384,512 half=True opset=%OPTSET% dynamic=False device=0 batch=1
move best.onnx %CD_PATH%runs/segment/t2_ischemia_512_augmented/weights/best_fp16.onnx
cd ../../../../..