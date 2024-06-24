from PySide2 import QtCore, QtGui, QtWidgets
import pandas as pd

from Protocol import Protocol, get_protocol_name
from Cache import get_cached_data
from Entity import Slice, Study

def files(root_dir, mask):
	filenameFilter = [mask]
	it = QtCore.QDirIterator(root_dir, filenameFilter, QtCore.QDir.Files, QtCore.QDirIterator.Subdirectories)
	while it.hasNext():
		it.next()
		yield it.filePath()

class Dataset:
	def __init__(self):
		self.__slices = None
		self.__studies = None
		self.__protocols = None 

	def __getSlices(self, path):
		columns = ["protocol_id","PatientID","StudyID","StudyDescription","StudyDate","StudyTime","Filename","SliceLocation", "slice_number"]
		slice_info = []

		for filename in files(path, "*.ima"):
			cached_data = get_cached_data(filename)
			slice_info.append([cached_data.protocol, cached_data.ds.PatientID, cached_data.ds.StudyID, cached_data.ds.StudyDescription, cached_data.ds.StudyDate, cached_data.ds.StudyTime, filename, cached_data.ds.SliceLocation, None])

		self.__slices = pd.DataFrame(slice_info, columns=columns).sort_values("SliceLocation", ascending=True)

	def __getStudies(self):
		columns = ["protocol_id","PatientID","StudyID","StudyDescription","StudyDate","StudyTime","slices_idx_list", "ref_studies_list"]
		key = ["protocol_id","PatientID","StudyID","StudyDescription","StudyDate","StudyTime"]
		studies_info = []
		for group_key, group in self.__slices.groupby(key):
			unique = group.drop_duplicates("SliceLocation", keep='last')
			indexes = unique.index.to_numpy()
			studies_info.append([*group_key, indexes, None])

			slice_number = 0
			for index in indexes:
				self.__slices.loc[index, "slice_number"] = slice_number
				slice_number = slice_number + 1

		self.__studies = pd.DataFrame(studies_info, columns=columns)

	def __getProtocols(self):
		columns = ["protocol_id","studies_idx_list"]
		key = ["protocol_id"]
		protocols_info = []
		for group_key, group in self.__studies.groupby(key):
			protocols_info.append([*group_key, group.index.to_numpy()])
		self.__protocols = pd.DataFrame(protocols_info, columns=columns)

	def __getRefStudies(self, study_index):
		swi_protocol_index = self.getProtocolIndex(Protocol.SWI)
		if swi_protocol_index == None:
			return []

		study_slice_count = len(self.__studies.loc[study_index]["slices_idx_list"])

		for ref_study_index in self.__protocols.loc[swi_protocol_index]["studies_idx_list"]:
			if study_index == ref_study_index:
				continue
			if len(self.__studies.loc[ref_study_index]["slices_idx_list"]) == study_slice_count:
				yield ref_study_index

	def scanFolder(self, path):
		self.__getSlices(path)
		self.__getStudies()
		self.__getProtocols()

		for study_index in self.studies():
			self.__studies.at[study_index, "ref_studies_list"] = [*self.__getRefStudies(study_index)]

	def protocols(self):
		for index in range(len(self.__protocols)):
			yield index

	def studies(self):
		for index in range(len(self.__studies)):
			yield index

	def slices(self):
		for index in range(len(self.__slices)):
			yield index

	def getProtocolIndex(self, protocol_id):
		for index, protocol in self.__protocols.iterrows():
			if protocol["protocol_id"] == protocol_id:
				return index
		return None

	def getProtocolName(self, protocol_index):
		return get_protocol_name(self.__protocols.loc[protocol_index]["protocol_id"])

	def getStudyName(self, study_index):
		study = self.__studies.loc[study_index]
		return str(study["PatientID"]) + "/" + str(study["StudyDescription"])

	def getSliceName(self, slice_index):
		return str("Slice " + str(self.__slices.loc[slice_index]["slice_number"]))

	def studiesWithProtocolIndex(self, protocol_index):
		for study_index in self.__protocols.loc[protocol_index]["studies_idx_list"]:
			yield study_index

	def slicesWithStudyIndex(self, study_index):
		for slice_index in self.__studies.loc[study_index]["slices_idx_list"]:
			yield slice_index

	def refStudies(self, study_index):
		return self.__studies.loc[study_index]["ref_studies_list"]

	def getStudy(self, study_index):
		study_info = self.__studies.loc[study_index]
		return Study(study_index, study_info["protocol_id"], [self.__slices.loc[slice_index]["Filename"] for slice_index in study_info["slices_idx_list"]], study_info["ref_studies_list"], study_info["StudyDate"])

	def getSlice(self, slice_index):
		slice_info = self.__slices.loc[slice_index]
		return Slice(slice_index, slice_info["protocol_id"], slice_info["Filename"])
