import asyncio
from time import perf_counter

class Counter:
	def __init__(self):
		self.count = 0
		self.totalTime = 0
		self.lastStartTime = 0

	def start(self):
		self.lastStartTime = perf_counter()
	
	def stop(self):
		self.count += 1
		self.totalTime += perf_counter() - self.lastStartTime

	def __str__(self):
		return f"Calls: {self.count}\tTotal time: {self.totalTime:.06f}\tAvg time: {(self.totalTime / self.count):.06f}"

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
		self.counters.get(tag).stop()

	def print(self):
		for key, value in self.counters.items():
			print(value, f" Tag: \"{key}\"")


#decorator
def profile(func):
	def wrapper(*args, **kwargs):
		Profiler.getInstance().start(func.__name__)
		retval = func(*args, **kwargs)
		Profiler.getInstance().stop(func.__name__)
		return retval

	return wrapper
