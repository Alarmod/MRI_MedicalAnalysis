from PySide2 import QtCore, QtGui, QtWidgets
import pydicom
import pandas as pd

from Entity import Slice, Study

def files(root_dir, mask):
	filenameFilter = [mask]
	it = QtCore.QDirIterator(root_dir, filenameFilter, QtCore.QDir.Files, QtCore.QDirIterator.Subdirectories)
	while it.hasNext():
		it.next()
		yield it.filePath()

def loadDICOMFile(filename):
	return pydicom.dcmread(filename)

class Dataset:
	def __init__(self):
		self.__slices = None
		self.__studies = None
		self.__protocols = None 

	def __getSlices(self, path):
		columns = ["ProtocolName","PatientID","StudyID","StudyDescription","StudyDate","StudyTime","Filename","SliceLocation", "slice_number"]
		slice_info = []
		for filename in files(path, "*.ima"):
			ds = loadDICOMFile(filename)
			protocol_name = ds.ProtocolName
			if protocol_name == "ep2d_diff_tra_14b_1mm":
				protocol_name = "ep2d_diff_tra_14b"
			slice_info.append([protocol_name, ds.PatientID, ds.StudyID, ds.StudyDescription, ds.StudyDate, ds.StudyTime, filename, ds.SliceLocation, None])

		self.__slices = pd.DataFrame(slice_info, columns=columns).sort_values("SliceLocation", ascending=True)

	def __getStudies(self):
		columns = ["ProtocolName","PatientID","StudyID","StudyDescription","StudyDate","StudyTime","slices_idx_list", "ref_studies_list"]
		key = ["ProtocolName","PatientID","StudyID","StudyDescription","StudyDate","StudyTime"]
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
		columns = ["ProtocolName","studies_idx_list"]
		key = ["ProtocolName"]
		protocols_info = []
		for group_key, group in self.__studies.groupby(key):
			protocols_info.append([*group_key, group.index.to_numpy()])
		self.__protocols = pd.DataFrame(protocols_info, columns=columns)

	def __getRefStudies(self, target_study_id):
		swi_protocol_id = self.getProtocolID("swi_tra")
		if swi_protocol_id == None:
			return []

		target_study = self.__studies.loc[target_study_id]
		target_study_slice_count = len(target_study["slices_idx_list"])

		for study_id in self.__protocols.loc[swi_protocol_id]["studies_idx_list"]:
			if study_id == target_study_id:
				continue
			study = self.__studies.loc[study_id]
			if len(study["slices_idx_list"]) == target_study_slice_count:
				yield study_id

	def scanFolder(self, path):
		self.__getSlices(path)
		self.__getStudies()
		self.__getProtocols()

		for index, study in self.__studies.iterrows():
			study["ref_studies_list"] = [*self.__getRefStudies(index)]


	def protocols(self):
		for index in range(len(self.__protocols)):
			yield index

	def getProtocolID(self, protocol_name):
		for index, protocol in self.__protocols.iterrows():
			if protocol["ProtocolName"] == protocol_name:
				return index
		return None

	def getProtocolName(self, protocol_id):
		return self.__protocols.loc[protocol_id]["ProtocolName"]

	def studies(self):
		for index in range(len(self.__studies)):
			yield index

	def getStudyName(self, study_id):
		study = self.__studies.loc[study_id]
		return str(study["PatientID"]) + "/" + str(study["StudyDescription"])

	def studiesWithProtocolID(self, parent_protocol_id):
		for study_id in self.__protocols.loc[parent_protocol_id]["studies_idx_list"]:
			yield study_id

	def studiesWithProtocolName(self, protocol_name):
		protocol_id = self.getProtocolID(protocol_name)
		if protocol_id == None:
			return []
		return self.studiesWithProtocolID(protocol_id)

	def refStudies(self, target_study_id):
		return self.__studies.loc[target_study_id]["ref_studies_list"]

	def slices(self):
		for index in range(len(self.__slices)):
			yield index

	def getSliceName(self, slice_id):
		return str("Slice " + str(self.__slices.loc[slice_id]["slice_number"]))

	def slicesWithStudyID(self, parent_study_id):
		for slice_id in self.__studies.loc[parent_study_id]["slices_idx_list"]:
			yield slice_id

	def getSlice(self, slice_id):
		slice_info = self.__slices.loc[slice_id]
		return Slice(slice_id, slice_info["ProtocolName"], slice_info["Filename"])

	def getStudy(self, study_id):
		study_info = self.__studies.loc[study_id]
		return Study(study_id, study_info["ProtocolName"], [self.__slices.loc[slice_id]["Filename"] for slice_id in study_info["slices_idx_list"]], study_info["ref_studies_list"], study_info["StudyDate"])
