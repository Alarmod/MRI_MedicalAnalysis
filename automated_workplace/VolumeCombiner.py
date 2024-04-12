import numpy as np
import cv2

class VolumeCombiner:
	def __init__(self, target_mask, target_spacing, source_mask, source_spacing):
		self.target_shape = target_mask.shape

	def transform(self, source_mask):
		def resized(mask, w, h):
			mask = cv2.resize(mask, (w, h))
			return cv2.inRange(mask, 127, 255)

		if source_mask.shape == self.target_shape:
			return source_mask

		return np.stack([resized(source_mask[i], self.output_shape[2], self.output_shape[1]) for i in range(source_mask.shape[0])])

'''
def get_bbox_points(bbox):
	return np.array([[bbox[0], bbox[1], 1], #top-left
			[bbox[0]+bbox[2], bbox[1], 1], #top-right
			[bbox[0]+bbox[2], bbox[1]+bbox[3], 1], #bottom-right
			[bbox[0], bbox[1]+bbox[3], 1]]) #bottom-left

def get_bbox_similarity_matrix_transform(target, source):
	target_points = get_bbox_points(cv2.boundingRect(target))
	source_points = get_bbox_points(cv2.boundingRect(source))
	M, residuals, rank, s = np.linalg.lstsq(source_points, target_points, rcond=None)
	return M.T

#warp by brain area bboxes
class VolumeCombiner:
	def __init__(self, target_mask, target_spacing, source_mask, source_spacing):
		self.target_shape = target_mask.shape

		slice_count = self.target_shape[0]
		self.transformations = [None]*slice_count
		for i in range(slice_count):
			self.transformations[i] = get_bbox_similarity_matrix_transform(target_mask[i], source_mask[i])

	def transform(self, source_mask):
		def transformed(mask, transformation, w, h):
			mask = cv2.warpPerspective(mask, transformation, (w, h))
			return cv2.inRange(mask, 127, 255)

		return np.stack([transformed(source_mask[i], self.transformations[i], self.target_shape[2], self.target_shape[1]) for i in range(source_mask.shape[0])])
'''
