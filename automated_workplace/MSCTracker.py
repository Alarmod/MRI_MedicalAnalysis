import numpy as np
#import cv2
import cc3d
import pulp as pl

def distance(start, end):
	return np.sqrt(np.sum(np.square(start - end)))

def getXYZ(col, row, slice_number, voxel_size):
	return np.array([col, row, slice_number])*voxel_size

class ConnectedComponent:
	def __init__(self, xyz, volume):
		self.xyz = xyz 
		self.volume = int(volume)

	def __repr__(self):
		return "<"+str(self.xyz) + ", " + str(self.volume) + ">"

'''
#CCL via opencv (2d)
def getConnectedComponents(voxels, voxel_size):
	components = []
	for slice_number in range(voxels.shape[0]):
		num_labels, labels, stats, centroids = cv2.connectedComponentsWithStats(voxels[slice_number], 8, cv2.CV_32S)
		for label in range(1, num_labels):
			component = ConnectedComponent(getXYZ(centroids[label][0], centroids[label][1], slice_number, voxel_size),
						       stats[label][cv2.CC_STAT_AREA])
			components.append(component)
	return components
'''

#CCL via сс3d (3d)
def getConnectedComponents(voxels, voxel_size):
	components = []
	labels, num_labels = cc3d.connected_components(voxels, connectivity=26, return_N=True)
	stats = cc3d.statistics(labels)
	centroids = stats["centroids"]
	voxel_counts = stats["voxel_counts"]
	for label in range(1, num_labels+1):
		component = ConnectedComponent(getXYZ(centroids[label][2], centroids[label][1], centroids[label][0], voxel_size),
					       voxel_counts[label])
		components.append(component)
	return components


def solve_closed_tp(supply, demands, cost):
	supply_count = len(supply)
	demands_count = len(demands)

	routes = [(s,d) for s in range(supply_count) for d in range(demands_count)]

	amount = np.ndarray((supply_count, demands_count), dtype=pl.LpVariable)
	for (s, d) in routes:
		amount[s][d] = pl.LpVariable("Transfer_from_"+str(s)+"_to_"+str(d), lowBound=0)

	problem = pl.LpProblem("TransportationProblem", pl.LpMinimize)
	problem += pl.lpSum(amount[s][d]*cost[s][d] for (s,d) in routes)

	for d in range(demands_count):
		problem += pl.lpSum(amount[s][d] for s in range(supply_count)) == demands[d]

	for s in range(supply_count):
		problem += pl.lpSum(amount[s][d] for d in range(demands_count)) <= supply[s]

	problem.solve(pl.PULP_CBC_CMD(msg=False))

	result = np.zeros(amount.shape, dtype=np.int32)
	if pl.LpStatus[problem.status]:
		for _from, _to in np.ndindex(amount.shape):
			result[_from][_to] = amount[_from][_to].varValue

	return result

def solve_tp(sources, destinations):
	supply = [src.volume for src in sources]
	demands = [dst.volume for dst in destinations]

	src_total_amount = np.sum(supply)
	dst_total_amount = np.sum(demands)

	useDummySource = 0
	useDummyDestination = 0
	amount_diff = abs(src_total_amount - dst_total_amount)
	if amount_diff != 0:
		if src_total_amount > dst_total_amount:
			useDummyDestination = 1
		else:
			useDummySource = 1

	src_count = len(sources)
	dst_count = len(destinations)
	costs = np.zeros((src_count + useDummySource, dst_count + useDummyDestination), dtype=float)
	for s in range(src_count):
		for d in range(dst_count):
			costs[s][d] = distance(sources[s].xyz, destinations[d].xyz)
		
	max_cost = np.max(costs) + 1
	if useDummySource:
		costs[-1,:] = max_cost
		supply.append(amount_diff)
	if useDummyDestination:
		costs[:,-1] = max_cost
		demands.append(amount_diff)

	return solve_closed_tp(supply, demands, costs)


class Track:
	def __init__(self, start, end, amount):
		self.start = start
		self.end = end
		self.amount = amount

	def __repr__(self):
		return "<"+str(self.start) + ", " + str(self.end) + ", " + str(self.amount) + ">"

class MSCTracker:
	def __init__(self):
		pass

	def track(self, src_mask, dst_mask, voxel_size):
		src_components = getConnectedComponents(src_mask, voxel_size)
		dst_components = getConnectedComponents(dst_mask, voxel_size)

		result = solve_tp(src_components, dst_components)
		tracks = []

		for src_index in range(len(src_components)):
			for dst_index in range(len(dst_components)):
				if result[src_index][dst_index] != 0:
					tracks.append(Track(src_components[src_index].xyz, dst_components[dst_index].xyz, result[src_index][dst_index]))
		return tracks