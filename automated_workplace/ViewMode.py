class ViewMode:
	USE_PREPROCESSED_SOURCE = 1
	MARK_BRAIN_AREA = 2
	MARK_ISCHEMIA_AREA = 4
	MARK_MSC_AREA = 8
	MARK_MSC_FROM = 16
	TRACK_MSC_FROM = 32

	def __init__(self):
		self.flags = 0
		self.ref_msc_study = None
		self.ref_tracking_study = None

	def setFlag(self, flag, ref_study=None):
		self.flags = self.flags | flag
		if flag == ViewMode.MARK_MSC_FROM:
			self.ref_msc_study = ref_study
		if flag == ViewMode.TRACK_MSC_FROM:
			self.ref_tracking_study = ref_study

	def dropFlag(self, flag):
		self.flags = self.flags & ~flag
		if flag == ViewMode.MARK_MSC_FROM:
			self.ref_msc_study = None
		if flag == ViewMode.TRACK_MSC_FROM:
			self.ref_tracking_study = None
		
