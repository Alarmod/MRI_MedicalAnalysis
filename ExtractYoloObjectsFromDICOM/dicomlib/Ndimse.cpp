/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/
#include <string>
#include "Ndimse.hpp"
#include "Types.hpp"

#include "ImplementationUID.hpp"
#include "ServiceBase.hpp"


#ifdef THIS_ISNT_IMPLEMENTED_YET



/*
	we really need to write a test application
	that makes use of this code.

	Until then there's not much point in mucking around with
	this as we have no way of testing it.
*/
namespace dicom
{


	void HandleNCreate(NHandlerFunction handler,ServiceBase& pdu, const DataSet& command, const UID& classUID)
//	bool NCreateSCP::handle(ServiceBase& pdu, const DataSet& rqCmd, const UID& classUID)
	{
		UINT16 msgID = 0;
		command(TAG_MSG_ID)>>msgID;

		UINT16 dstype = 0;
		command(TAG_DATA_SET_TYPE)>>dstype;


		UID instUID (command.Get<UID>(TAG_AFF_SOP_INST_UID));

		if(instUID.str().size()==0)//this should never happen 
		{
			char buffer[64];
			instUID = makeUID(buffer);
		}

		DataSet rqData;
		if (dstype != DataSetStatus::NO_DATA_SET)
		{
			if (pdu.Read(rqData) == false)
				return false;
		}

		UINT16 stat = Status::SUCCESS;
		DataSet rspData;

		//if (m_pHandler)
		//	stat = m_pHandler->handle(pdu/*.GetSocketfd()*/, &rspData, instUID, command, rqData);

		handler(pdu,rspData,command,data);


		NCreateRSP rspCmd(msgID, classUID, instUID, stat,
			rspData.empty() ? DataSetStatus::NO_DATA_SET : DataSetStatus::YES_DATA_SET);

		pdu.WriteCommand(rspCmd, classUID);

		if(!rspData.empty())
			pdu.WriteDataSet(rspData, classUID) ;
		
		//return true;
	}

//	bool NSetSCP::handle(ServiceBase& pdu, const DataSet& rqCmd, const UID& classUID)
	void HandleNSetSCP(NHandlerFunction handler, ServiceBase& pdu, const DataSet& command, const UID& classUID);
	{
		UINT16 msgID = 0;

		command(TAG_MSG_ID)>>msgID;

		UINT16 dstype = 0;

		command(TAG_DATA_SET_TYPE)>>dstype;

		UID instUID = command.Get<UID>(TAG_REQ_SOP_INST_UID);

		if (dstype == DataSetStatus::NO_DATA_SET)
			throw exception("No data set!");

		DataSet rqData;
		if (pdu.Read(rqData) == false)
			return false;//should throw 

		UINT16 stat = Status::SUCCESS;

		DataSet rspData;
		//if (m_pHandler)
		//	stat = m_pHandler->handle(pdu/*.GetSocketfd()*/, &rspData, instUID, command, rqData);
		handler(pdu,command,rqData);

		NSetRSP rspCmd(msgID, classUID, stat,
			rspData.empty() ? DataSetStatus::NO_DATA_SET : DataSetStatus::YES_DATA_SET);

		pdu.WriteCommand(rspCmd, classUID); 
		if(!rspData.empty() )
			pdu.WriteDataSet(rspData, classUID);
		return true;
	}

	NCreateSCU::NCreateSCU(const UID& classUID)
		: m_classUID(classUID)
	{
	}

	void NCreateSCU::writeRQ(ServiceBase& pdu, const UID& instUID, const DataSet& data)
	{
		NCreateRQ rqCmd(uniq16odd(), m_classUID, instUID, DataSetStatus::YES_DATA_SET);
		pdu.WriteCommand(rqCmd, m_classUID);
		pdu.WriteDataSet(data, m_classUID);
	}

	void NCreateSCU::writeRQ(ServiceBase& pdu, const DataSet& data)
	{
		NCreateRQ rqCmd(uniq16odd(), m_classUID, DataSetStatus::YES_DATA_SET);
		pdu.WriteCommand(rqCmd, m_classUID);
		pdu.WriteDataSet(data, m_classUID);
	}

	void NCreateSCU::writeRQ(ServiceBase& pdu, const UID& instUID)
	{
		NCreateRQ rqCmd(uniq16odd(), m_classUID, instUID, DataSetStatus::NO_DATA_SET);
		pdu.WriteCommand(rqCmd, m_classUID);
	}

	void NCreateSCU::writeRQ(ServiceBase& pdu)
	{
		NCreateRQ rqCmd(uniq16odd(), m_classUID, DataSetStatus::NO_DATA_SET);
		pdu.WriteCommand(rqCmd, m_classUID);
	}

	bool NCreateSCU::readRSP(UINT16& status, DataSet& data, ServiceBase& pdu)
	{
		DataSet response;
		return readRSP(status, response, data, pdu);
	}

	bool NCreateSCU::readRSP(UINT16& status, DataSet& response, DataSet& data, ServiceBase& pdu)
	{
		UINT16 dstype = 0;
		pdu.Read(response);
		response(TAG_DATA_SET_TYPE)>>dstype;
		response(TAG_STATUS)>>status;
		if(dstype==DataSetStatus::NO_DATA_SET)
			return true;
		else
			return pdu.Read(data);


		//return pdu.Read(*rsp_p) != false &&
		//	rsp_p->getSafeUS(&dstype, TAG_DATA_SET_TYPE) &&
		//	rsp_p->getSafeUS(stat_p, TAG_STATUS) &&
		//	(dstype != DataSetStatus::NO_DATA_SET ? pdu.Read(*data_p) != false : true);
	}

	NSetSCU::NSetSCU(const UID& classUID)
		: m_classUID(classUID)
	{
	}

	void NSetSCU::writeRQ(ServiceBase& pdu, const UID& instUID, const DataSet& data)
	{
		NSetRQ rqCmd(uniq16odd(), m_classUID, instUID);
		pdu.WriteCommand(rqCmd, m_classUID);
		pdu.WriteDataSet(data, m_classUID);
	}

	bool NSetSCU::readRSP(UINT16& status, DataSet& data, ServiceBase& pdu)
	{
		DataSet response;
		return readRSP(status, response, data, pdu);
	}

	bool NSetSCU::readRSP(UINT16& status, DataSet& response, DataSet& data, ServiceBase& pdu)
	{
		UINT16 dstype = 0;

		pdu.Read(response);
		response(TAG_DATA_SET_TYPE)>>dstype;
		response(TAG_STATUS)>>status;
		if(dstype==DataSetStatus::NO_DATA_SET)
			return true;
		else
			return pdu.Read(data);
		//return pdu.Read(*rsp_p) != false &&
		//	rsp_p->getSafeUS(&dstype, TAG_DATA_SET_TYPE) &&
		//	rsp_p->getSafeUS(stat_p, TAG_STATUS) &&
		//	(dstype != DataSetStatus::NO_DATA_SET ? pdu.Read(*data_p) != false : true);
	}
}//namespace dicom
#endif //THIS_ISNT_IMPLEMENTED_YET
