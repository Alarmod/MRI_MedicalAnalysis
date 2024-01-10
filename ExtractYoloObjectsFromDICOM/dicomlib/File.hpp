#ifndef FILE_HPP_INCLUDE_GUARD_85438743843
#define FILE_HPP_INCLUDE_GUARD_85438743843
#include "UID.hpp"
#include "UIDs.hpp"
#include "Exceptions.hpp"
#include "FileMetaInformation.hpp"
#include <fstream>


namespace dicom
{
	/*
		What operations do we actually want to expose to the end user?
		Basically,
		Read from an istream onto a dataset.
		Write from a dataset onto an ostream, appending appropriate meta info at begin...

		so, maybe...
		ReadFromStream(istream,dataset);


		and then

		WriteToStream(dataset,ostream);

		all of which implies that File should be a hidden (implementation only) class.

		One reason for keeping it visible would be so that users could go:

		File f(...);
		modify(f.data);
		f.Write(...);

		Alternatively, we could bundle all this functionality into DataSet:

		DataSet ds(istream);
		ds.Write(ostream);

		and then maybe we wouldn't even have a file object.
	*/

	//!Thrown if we can't open the file, it's corrupt.
	struct FileException:public dicom::exception
	{
		FileException(std::string Description):exception(Description){}
	};

	void ReadFileMetaFromStream(std::ifstream& In, DataSet& ds);

	void ReadFromStream(std::ifstream& In, DataSet& data,size_t max_number_of_byte_to_read=-1);

	void WriteToStream(const DataSet& data,std::ostream& Out,TS ts=TS(IMPL_VR_LE_TRANSFER_SYNTAX)/*::IMPL_VR_LE*/, bool Tiff=true);

	void Read(std::string FileName,DataSet& data,size_t max_number_of_byte_to_read=-1);
  void ReadW(std::wstring FileName, DataSet& data, size_t max_number_of_byte_to_read = -1);

	void Write(const DataSet& data, std::string FileName, TS ts=TS(IMPL_VR_LE_TRANSFER_SYNTAX), bool Tiff=true);
  void WriteW(const DataSet& data, std::wstring FileName, TS ts = TS(IMPL_VR_LE_TRANSFER_SYNTAX), bool Tiff = true);

}//namespace dicom

#endif //FILE_HPP_INCLUDE_GUARD_85438743843
