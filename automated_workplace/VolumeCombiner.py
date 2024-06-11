import numpy as np
import cv2
'''
class VolumeCombiner:
	def __init__(self, target_mask, target_spacing, source_mask, source_spacing):
		self.target_shape = target_mask.shape

	def transform(self, source_mask):
		def resized(mask, w, h):
			mask = cv2.resize(mask, (w, h))
			return cv2.inRange(mask, 127, 255)

		if source_mask.shape == self.target_shape:
			return source_mask

		return np.stack([resized(source_mask[i], self.target_shape[2], self.target_shape[1]) for i in range(source_mask.shape[0])])
'''

#warp by brain area bboxes (top-center alignment)
class VolumeCombiner:
	def __init__(self, target_mask, target_spacing, source_mask, source_spacing):
		self.target_shape = target_mask.shape

		slice_count = self.target_shape[0]
		self.transformations = [None]*slice_count

		scale = [source_spacing[1] / target_spacing[1], source_spacing[0] / target_spacing[0]] # [xs,ys]

		for i in range(slice_count):
			target_bbox = cv2.boundingRect(target_mask[i]) #x,y,w,h
			source_bbox = cv2.boundingRect(source_mask[i]) #x,y,w,h
			source_bbox = np.multiply(source_bbox, [scale[0], scale[1], scale[0], scale[1]]) #scale bbox
			t_anchor_x = target_bbox[0] + target_bbox[2]/2
			s_anchor_x = source_bbox[0] + source_bbox[2]/2
			self.transformations[i] = np.array([[scale[0], 0 , round(t_anchor_x-s_anchor_x)], 
							    [0, scale[1], -(source_bbox[1]-target_bbox[1])]]).astype(np.float32)

	def transform(self, source_mask):
		def transformed(mask, transformation, w, h):
			mask = cv2.warpAffine(mask, transformation, (w, h))
			return cv2.inRange(mask, 127, 255)

		return np.stack([transformed(source_mask[i], self.transformations[i], self.target_shape[2], self.target_shape[1]) for i in range(source_mask.shape[0])])
