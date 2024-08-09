import warnings
warnings.simplefilter(action='ignore', category=FutureWarning)

import os
os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"

from ultralytics import YOLO

import torch

global_half=True

# NaN tensor values problem for GTX16xx users (no problem on other devices)
# https://github.com/ultralytics/yolov5/issues/7908
# При использовании видеокарт Nvidia GTX 16xx с активированной опцией global_half=True необходимо отключить поддержку CUDNN в PyTorch, установив torch.backends.cudnn.enabled=False, 
# запуск CUDNN в режиме бенчмарка с помощью опции torch.backends.cudnn.benchmark=True не гарантирует корректной работы
if global_half:
   if torch.cuda.is_available():
      if torch.cuda.device_count() > 0: 
         dev_name = torch.cuda.get_device_name(torch.cuda.current_device())
         if 'GeForce GTX 16' in dev_name:
            print("Activated NaN fix (see https://github.com/ultralytics/yolov5/issues/7908)")
            torch.backends.cudnn.enabled=False

def func(model, name_val, data_val, imgsz_val, batch_val, lr0_val, epochs_val, patience_val, mod): 
   results = model.train(
             name=name_val, 
             data=data_val, 
             imgsz=imgsz_val, 
             batch=batch_val, 
             lr0=lr0_val, 
             lrf=0.01, 
             mask_ratio=1, 
             overlap_mask=False, 
             half=global_half, 
             optimizer="AdamW", 
             single_cls=True, 
             cos_lr=True, 
             dnn=False, 
             amp=True, 
             device="0", 
             epochs=epochs_val, 
             patience=patience_val, 
             workers=0, 
             augment=False, 
             cache=True, 
             nms=False, 
             seed=777, 
             deterministic=True, 
             hsv_h=0.0, hsv_s=0.0, hsv_v=0.02, 
             perspective=0.0, flipud=0, fliplr=0, mosaic=0.0, mixup=0.0, copy_paste=0.0, 
             degrees=7.5 if mod else 0.0, translate=0.025 if mod else 0.0, scale=0.2 if mod else 0.0, shear=0.025 if mod else 0.0, 
             rect=True
   )

if __name__ == '__main__': 
   torch.multiprocessing.freeze_support()

   cracks_imgsz=640

   model = YOLO("yolov8n-seg.yaml", task="segment")
   func(model, "cracks_exp", "crack-seg.yaml", cracks_imgsz, 30, 0.001, 100, 20, True)
