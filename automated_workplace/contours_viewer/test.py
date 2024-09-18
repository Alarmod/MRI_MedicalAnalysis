# Based on https://github.com/Alarmod/MRI_MedicalAnalysis/blob/main/automated_workplace/MRIMAProcessor.py

import glob, os, numpy as np
import cv2
import pydicom

glob_res = glob.glob("./fire_and_smoke/images/*.png")

smoke_and_fireColorRGB = [255, 255, 0]

def draw_contours(rgb_image, data, color, thickness_value):
    ct_target, hierarchy_target = cv2.findContours(data, cv2.RETR_CCOMP, cv2.CHAIN_APPROX_NONE)
    if type(hierarchy_target) != type(None):
       for idx in range(len(hierarchy_target[0])): 
           if hierarchy_target[0][idx][3] == -1: 
              img_contours = cv2.drawContours(np.zeros(data.shape, np.uint8), ct_target, idx, 255, thickness_value)
              rgb_image[(img_contours == 255)] = color
           else: 
              for p in ct_target[idx][::5]: # set gap value
                  cv2.circle(rgb_image, p[0], thickness_value, color, -1)

for filepath in glob_res:
    print(f"Read {filepath}")
    base_name = os.path.basename(filepath)

    pixels = cv2.imread(filepath, cv2.IMREAD_COLOR)

    scale_factor = 512.0 / np.float64(pixels.shape[1])
    pixels = cv2.resize(pixels, (0, 0), fx=scale_factor, fy=scale_factor, interpolation=cv2.INTER_NEAREST)

    smoke_and_fire_mask= "./fire_and_smoke/masks/" + base_name
    smoke_and_fire_mask = cv2.imread(smoke_and_fire_mask, cv2.IMREAD_GRAYSCALE)
    smoke_and_fire_mask = cv2.resize(smoke_and_fire_mask, (0, 0), fx=scale_factor, fy=scale_factor, interpolation=cv2.INTER_NEAREST)

    smoke_and_fire_pixels_count = 0
    marked = (smoke_and_fire_mask == 255)
    smoke_and_fire_pixels_count = np.sum(marked)
    if smoke_and_fire_pixels_count > 0:
       draw_contours(pixels, smoke_and_fire_mask, smoke_and_fireColorRGB, 2)

    cv2.imwrite("./fire_and_smoke/result/" + base_name, pixels)
    cv2.imshow(base_name, pixels)

cv2.waitKey()