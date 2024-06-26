import os
os.environ["KMP_DUPLICATE_LIB_OK"]="TRUE"

from ultralytics import YOLO

import torch

global_half = True

def func(model, name_val, data_val, imgsz_val, batch_val, lr0_val, epochs_val, patience_val, mod): 
   results = model.train(
             name=name_val, 
             data=data_val, 
             imgsz=imgsz_val, 
             batch=batch_val, 
             lr0=lr0_val, 
             lrf=0.001, 
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

   reinit_data = 1

   brain_and_ischemia_imgsz=512
   msk_imgsz=1280

   model = YOLO("yolov8x-seg.yaml", task="segment")#.load(".\\runs\\segment\\t2_brain_" + str(brain_and_ischemia_imgsz) + "_trans\\weights\\best.pt")
   func(model, "t2_brain_" + str(brain_and_ischemia_imgsz), "data_t2_brain.yaml", brain_and_ischemia_imgsz, 12, 0.0001, 2100, 100 // reinit_data, True)

   model = YOLO(model="yolov8x-seg.yaml", task="segment")#.load(".\\runs\\segment\\t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented_trans\\weights\\best.pt")
   func(model, "t2_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented", "data_t2_ischemia.yaml", brain_and_ischemia_imgsz, 12, 0.0001, 2100, 100 // reinit_data, True)

   model = YOLO(model="yolov8x-seg.yaml", task="segment")#.load(".\\runs\\segment\\adc_brain_" + str(brain_and_ischemia_imgsz) + "_trans\\weights\\best.pt")
   func(model, "adc_brain_" + str(brain_and_ischemia_imgsz), "data_adc_brain.yaml", brain_and_ischemia_imgsz, 12, 0.0001, 2100, 100 // reinit_data, True)

   model = YOLO(model="yolov8x-seg.yaml", task="segment")#.load(".\\runs\\segment\\adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented_trans\\weights\\best.pt")
   func(model, "adc_ischemia_" + str(brain_and_ischemia_imgsz) + "_augmented", "data_adc_ischemia.yaml", brain_and_ischemia_imgsz, 12, 0.0001, 2100, 100 // reinit_data, True)

   model = YOLO(model="yolov8x-seg.yaml", task="segment")#.load(".\\runs\\segment\\swi_brain_" + str(brain_and_ischemia_imgsz) + "_trans\\weights\\best.pt")
   func(model, "swi_brain_" + str(brain_and_ischemia_imgsz), "data_swi_brain.yaml", brain_and_ischemia_imgsz, 12, 0.0001, 2100, 100 // reinit_data, True)

   model = YOLO(model="yolov8-mod.yaml", task="segment")#.load(".\\runs\\segment\\swi_msc_mod_" + str(msk_imgsz) + "_augmented_trans\\weights\\best.pt")
   func(model, "swi_msc_mod_" + str(msk_imgsz) + "_augmented", "data_swi_msc.yaml", msk_imgsz, 3, 0.0001, 2100, 100 // reinit_data, True)
