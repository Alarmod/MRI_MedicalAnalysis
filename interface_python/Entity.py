class Slice:
	def __init__(self, id, protocol, filename):
		self.id = id
		self.protocol = protocol
		self.filename = filename

class Study:
	def __init__(self, id, protocol, filename_list, ref_studies_list, date):
		self.id = id
		self.protocol = protocol
		self.filename_list = filename_list
		self.ref_studies_list = ref_studies_list
		self.date = date

class Image:
	def __init__(self, rgb_image, info):
		self.rgb_image = rgb_image
		self.info = info

class Volume:
	def __init__(self, vtk_volume, vtk_mesh, info):
		self.vtk_volume = vtk_volume
		self.vtk_mesh = vtk_mesh
		self.info = info
