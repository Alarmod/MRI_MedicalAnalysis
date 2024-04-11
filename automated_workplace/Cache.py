import threading
import cachetools
import pydicom

class CachedData:
	def __init__(self, filename):
		self.filename = filename
		self.protocol = None
		self.ds = None
		self.preprocessed = None
		self.brain_mask = None
		self.ischemia_mask = None
		self.msc_mask = None

@cachetools.cached(cache=cachetools.FIFOCache(maxsize=512), key=lambda filename: filename, lock=threading.RLock())
def get_cached_data(filename):
	cached_data = CachedData(filename)
	cached_data.ds = pydicom.dcmread(filename)
	cached_data.protocol = cached_data.ds.ProtocolName
	if cached_data.protocol == "ep2d_diff_tra_14b_1mm":
		cached_data.protocol = "ep2d_diff_tra_14b"
	return cached_data
