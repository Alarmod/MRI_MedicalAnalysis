import os, cv2, numpy as np

def LetterBoxInfo(img, new_shape=(640,640), auto=False, scaleFill=False, scaleup=True, center=True, stride=32):
        shape = img.shape[:2]  # current shape [height, width]
        if isinstance(new_shape, int):
            new_shape = (new_shape, new_shape)

        # Scale ratio (new / old)
        r = min(new_shape[0] / shape[0], new_shape[1] / shape[1])
        if not scaleup:  # only scale down, do not scale up (for better val mAP)
            r = min(r, 1.0)

        # Compute padding
        ratio = r, r  # width, height ratios
        new_unpad = int(round(shape[1] * r)), int(round(shape[0] * r))
        dw, dh = new_shape[1] - new_unpad[0], new_shape[0] - new_unpad[1]  # wh padding
        if auto:  # minimum rectangle
            dw, dh = np.mod(dw, stride), np.mod(dh, stride)  # wh padding
        elif scaleFill:  # stretch
            dw, dh = 0.0, 0.0
            new_unpad = (new_shape[1], new_shape[0])
            ratio = new_shape[1] / shape[1], new_shape[0] / shape[0]  # width, height ratios

        if center:
            dw /= 2  # divide padding into 2 sides
            dh /= 2

        if shape[::-1] != new_unpad:  # resize
            img = cv2.resize(img, new_unpad, interpolation=cv2.INTER_LINEAR)
        top, bottom = int(round(dh - 0.1)) if center else 0, int(round(dh + 0.1))
        left, right = int(round(dw - 0.1)) if center else 0, int(round(dw + 0.1))
        img = cv2.copyMakeBorder(
            img, top, bottom, left, right, cv2.BORDER_CONSTANT, value=(114, 114, 114)
        )  # add border

        return img

brain_and_ischemia_imgsz = 512
msk_imgsz=1280

directory = "./datasets/t2_data/brain/train/images/"
img = cv2.imread(directory + os.listdir(directory)[0], cv2.IMREAD_GRAYSCALE)
img = LetterBoxInfo(img, new_shape=brain_and_ischemia_imgsz, auto=True)
print(f"T2 brain shape: {img.shape}")

directory = "./datasets/t2_data/ischemia/train/images/"
img = cv2.imread(directory + os.listdir(directory)[0], cv2.IMREAD_GRAYSCALE)
img = LetterBoxInfo(img, new_shape=brain_and_ischemia_imgsz, auto=True)
print(f"T2 ischemia shape: {img.shape}\n")

directory = "./datasets/adc_data/brain/train/images/"
img = cv2.imread(directory + os.listdir(directory)[0], cv2.IMREAD_GRAYSCALE)
img = LetterBoxInfo(img, new_shape=brain_and_ischemia_imgsz, auto=True)
print(f"ADC brain shape: {img.shape}")

directory = "./datasets/adc_data/ischemia/train/images/"
img = cv2.imread(directory + os.listdir(directory)[0], cv2.IMREAD_GRAYSCALE)
img = LetterBoxInfo(img, new_shape=brain_and_ischemia_imgsz, auto=True)
print(f"ADC ischemia shape: {img.shape}\n")

directory = "./datasets/swi_data/brain/train/images/"
img = cv2.imread(directory + os.listdir(directory)[0], cv2.IMREAD_GRAYSCALE)
img = LetterBoxInfo(img, new_shape=brain_and_ischemia_imgsz, auto=True)
print(f"SWI brain shape: {img.shape}")

directory = "./datasets/swi_data/msc/train/images/"
img = cv2.imread(directory + os.listdir(directory)[0], cv2.IMREAD_GRAYSCALE)
img = LetterBoxInfo(img, new_shape=msk_imgsz, auto=True)
print(f"SWI MSC shape: {img.shape}\n")
