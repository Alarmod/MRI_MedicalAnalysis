/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/


#ifndef NDIMSE_HPP_INCLUDE_GUARD_5834956123
#define NDIMSE_HPP_INCLUDE_GUARD_5834956123

#include "DataSet.hpp"
#include "UID.hpp"
#include "ServiceBase.hpp"

/*
	Is inheritance the correct mechanism to use here, or 
	would composition be more appropriate.  I.e, are we dealing
	with 'IS A' or 'HAS A'?
*/


/*
	This needs the same treatment as CDIMSE - replacing virtual functions
	with function objects as our abstraction mechanism of choice.
*/

#ifdef THIS_ISNT_IMPLEMENTED_YET
namespace dicom
{


#if defined(_MSC_VER)//should also check the value of this, as MSVC v7.1 and above may not need this workaround.
	typedef boost::function3<void,ServiceBase&,const DataSet&,DataSet&>
		NHandlerFunction
#else
	typedef boost::function<void(ServiceBase&,const DataSet&,DataSet&)>
		NHandlerFunction;
#endif

			//handler(pdu,rspData,command,data);
	//class NHandler
	//{
	//public:
	//	virtual UINT16 handle(ServiceBase& pdu, dicom::DataSet* rspData_p, dicom::UID& instUID,
	//		const dicom::DataSet& rqCmd, const dicom::DataSet& rqData) = 0;
	//};

	//class NCreateSCP
	//{
	//	NHandler* m_pHandler;
	//public:
	//	NCreateSCP(NHandler* pHandler = 0) : m_pHandler(pHandler) {}
	//	bool handle(ServiceBase& pdu, const dicom::DataSet& rqCmd, const dicom::UID& classUID);
	//};

	void HandleNCreate(NHandlerFunction handler, ServiceBase& pdu,const DataSet& command, const UID& classUID);

	//class NSetSCP
	//{
	//	NHandler* m_pHandler;
	//public:
	//	NSetSCP(NHandler* pHandler = 0) : m_pHandler(pHandler) {}
	//	bool handle(ServiceBase& pdu, const dicom::DataSet& rqCmd, const dicom::UID& classUID);
	//};

	void HandleNSetSCP(NHandlerFunction handler, ServiceBase& pdu, const DataSet& command, const UID& classUID);

	class NCreateSCU  
	{
		const dicom::UID m_classUID;
	public:
		NCreateSCU(const dicom::UID& classUID);
		void writeRQ(ServiceBase& pdu, const dicom::UID& instUID, const dicom::DataSet& data);
		void writeRQ(ServiceBase& pdu, const dicom::UID& instUID);
		void writeRQ(ServiceBase& pdu, const dicom::DataSet& data);
		void writeRQ(ServiceBase& pdu);
		bool readRSP(UINT16& status, dicom::DataSet& data, ServiceBase& pdu);
		bool readRSP(UINT16& status, dicom::DataSet& response, dicom::DataSet& data, ServiceBase& pdu);
	};

	class NSetSCU  
	{
		const dicom::UID m_classUID;
	public:
		NSetSCU(const dicom::UID& classUID);
		void writeRQ(ServiceBase& pdu, const dicom::UID& instUID, const dicom::DataSet& data);
		bool readRSP(UINT16& status, dicom::DataSet& data, ServiceBase& pdu);
		bool readRSP(UINT16& status, dicom::DataSet& response, dicom::DataSet& data, ServiceBase& pdu);
	};
}//namespace dicom

#endif //THIS_ISNT_IMPLEMENTED_YET


#endif //NDIMSE_HPP_INCLUDE_GUARD_5834956123
