import numpy as np
import cv2

class VolumeCombiner:
	def __init__(self, output_mask, output_spacing, input_mask, input_spacing):
		self.output_shape = output_mask.shape

	def transform(self, mask):
		def resized(mask, w, h):
			mask = cv2.resize(mask, (w, h))
			return cv2.inRange(mask, 127, 255)

		if mask.shape == self.output_shape:
			return mask

		return np.stack([resized(mask[i], self.output_shape[2], self.output_shape[1]) for i in range(mask.shape[0])])