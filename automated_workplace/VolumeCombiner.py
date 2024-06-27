import numpy as np
import cv2

#warp by brain area bboxes (top-center alignment)
def transform_volume(target_brain_mask, target_spacing, source_brain_mask, source_spacing, source_msc_mask):
	target_shape = target_brain_mask.shape

	slice_count = target_shape[0]
	transformations = [None]*slice_count
	scale = [source_spacing[0] / target_spacing[0], source_spacing[1] / target_spacing[1]] # [xs,ys]

	for i in range(slice_count):
		target_bbox = cv2.boundingRect(target_brain_mask[i]) #x,y,w,h
		source_bbox = cv2.boundingRect(source_brain_mask[i]) #x,y,w,h
		source_bbox = np.multiply(source_bbox, [scale[0], scale[1], scale[0], scale[1]]) #scale bbox
		t_anchor_x = target_bbox[0] + target_bbox[2]/2
		s_anchor_x = source_bbox[0] + source_bbox[2]/2
		transformations[i] = np.array([[scale[0], 0 , round(t_anchor_x-s_anchor_x)], 
						       [0, scale[1], -(source_bbox[1]-target_bbox[1])]]).astype(np.float32)

	def transformed(mask, transformation, w, h):
		mask = cv2.warpAffine(mask, transformation, (w, h))
		return cv2.inRange(mask, 127, 255)

	return np.stack([transformed(source_msc_mask[i], transformations[i], target_shape[2], target_shape[1]) for i in range(source_msc_mask.shape[0])])
