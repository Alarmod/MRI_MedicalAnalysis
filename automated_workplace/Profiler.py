import threading
from time import perf_counter

class Counter:
	def __init__(self):
		self.count = 0
		self.maxTime = 0
		self.totalTime = 0
		self.lastStartTime = 0

	def start(self):
		self.lastStartTime = perf_counter()
	
	def stop(self):
		delta = perf_counter() - self.lastStartTime
		self.count += 1
		self.totalTime += delta
		self.maxTime = max(self.maxTime, delta)

	def __str__(self):
		if self.count:
			return f"Calls: {self.count}\tTotal time: {self.totalTime:.06f}\tMax time: {self.maxTime:.03f}\tAvg time: {(self.totalTime / self.count):.03f}"
		return ""

class Profiler:
	__instance = None

	@staticmethod
	def getInstance():
		if Profiler.__instance == None:
			Profiler.__instance = Profiler()
		return Profiler.__instance

	def __init__(self):
		self.counters = {}

	def start(self, tag):
		counter = self.counters.get(tag)
		if counter is None:
			counter = Counter()
			self.counters[tag] = counter
		counter.start()

	def stop(self, tag):
		counter = self.counters.get(tag)
		if counter:
			counter.stop()

	def clear(self):
		self.counters = {}

	def print(self):
		for key, value in sorted(self.counters.items(), key=lambda x: x[1].totalTime, reverse=True):
			print(value, f" Tag: \"{key}\"")


#decorator
def profile(func):
	def wrapper(*args, **kwargs):
		tag = threading.current_thread().name + "/" + func.__name__
		Profiler.getInstance().start(tag)
		retval = func(*args, **kwargs)
		Profiler.getInstance().stop(tag)
		return retval

	return wrapper
