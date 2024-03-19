/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/
#include <algorithm>
#include <iostream>
#include "Cdimse.hpp"
#include "ServiceBase.hpp"

#include "Dumper.hpp"
/*
	This file needs a lot of cleaning up work.
*/
/*
	I'm a bit worried that the 'Group Length' field
	never seems to get inserted, as the standard seems
	to require for all messages.  Am I missing something
	here?
*/

using std::for_each;
using std::cout;
using std::endl;
using std::ios;
using std::find;
using std::string;

namespace dicom
{

	/*!
		Simply write back a success response.
		See Part 8, table 9.1-5
	*/
	void HandleCEcho(ServiceBase& pdu, const DataSet& command,const UID& classUID)
	{
		UINT16 msgID;
		command(TAG_MSG_ID)>>msgID;
		CommandSet::CEchoRSP response(msgID,classUID);
		pdu.WriteCommand(response,classUID);
	}

	void HandleCStore(CStoreFunction handler, ServiceBase& pdu, const DataSet& command, const UID& classUID)
	{
		UINT16 msgID,data_set_status;
		command(TAG_MSG_ID)>>msgID;
		command(TAG_DATA_SET_TYPE)>>data_set_status;
		if(data_set_status==DataSetStatus::NO_DATA_SET)
			throw exception("No data set!");
		DataSet data;
		pdu.Read(data);//the TransferSyntax is determined internally by pdu. -Sam

		handler(pdu,command,data);//this should indicate failure via a throw...

		UID instuid;
		data(TAG_SOP_INST_UID)>>instuid;
		CommandSet::CStoreRSP response(msgID,classUID,instuid,Status::SUCCESS);
		pdu.WriteCommand(response,classUID);
	}

	/*
		Part 7, Section 9.1.2.2 describes this procedure...
					also table 9.3-3
		Part 4, Section C.3.4 has additional information.
	*/
	void HandleCFind(CFindFunction handler,ServiceBase& pdu, const DataSet& command, const UID& classUID)
	{
#ifdef _DEBUG
		cout  << "HandleCFind:" << endl << command;
#endif
		UINT16 msgID,data_set_status;
		command(TAG_MSG_ID)>>msgID;
		command(TAG_DATA_SET_TYPE)>>data_set_status;
		if(data_set_status==DataSetStatus::NO_DATA_SET)
			throw exception("No data set");
		DataSet request_data;
		pdu.Read(request_data);

		Sequence Matches;

		//the user-defined callback does the actual matching...
		handler(pdu,request_data,Matches);

		//now we send back all found matches.
		for(Sequence::iterator I=Matches.begin();I!=Matches.end();I++)
		{

			CommandSet::CFindRSP response(msgID,classUID,Status::PENDING,DataSetStatus::YES_DATA_SET);
			pdu.WriteCommand(response,classUID);
			pdu.WriteDataSet(*I,classUID);
		}

		CommandSet::CFindRSP response(msgID,classUID,Status::SUCCESS,DataSetStatus::NO_DATA_SET);
		pdu.WriteCommand(response,classUID);
	}



	/*
		C-GET is only maintained in
		the standard for backwards compatability.  If we're going to implement it, I
		think we need to figure out the client-side behaviour first.
	*/


	void CGetSCP::handle(ServiceBase& pdu, const DataSet& rqCmd, const UID& classUID)
	{
		//TODO
		throw NotYetImplemented();
	}

	void HandleCMove(CMoveFunction handler,ServiceBase& pdu,
		const DataSet& command, const UID& classUID)
	{
		UINT16 data_set_status;
		command(TAG_DATA_SET_TYPE)>>data_set_status;
		if(data_set_status==DataSetStatus::NO_DATA_SET)
			throw exception("No data set");
		DataSet request_data;
		pdu.Read(request_data);

		//The rest part of implementation involves design of server and should be 
		//implemented in serve. -Sam
		handler(pdu,command,request_data);
	}



	CEchoSCU::CEchoSCU(ServiceBase& service)
	: SCU(service,VERIFICATION_SOP_CLASS)
	{
	}

	void CEchoSCU::writeRQ()
	{
		CommandSet::CEchoRQ rq(uniq16odd(), classUID_);
		service_.WriteCommand(rq, classUID_) ;
	}

	void CEchoSCU::readRSP(UINT16& status)
	{
		DataSet response;
		readRSP(status, response);
	}

	void CEchoSCU::readRSP(UINT16& status, DataSet& response)
	{
		service_.Read(response);
		response(TAG_STATUS)>>status;
	}

	CStoreSCU::CStoreSCU(ServiceBase& service,const UID& classUID)
	: SCU(service,classUID)
	{
	}

	void CStoreSCU::writeRQ(const UID& instUID, const DataSet& data,/*TS ts,*/ UINT16 priority)
	{
		CommandSet::CStoreRQ rq(uniq16odd(), classUID_, instUID, priority);
		service_.WriteCommand(rq, classUID_);
		service_.WriteDataSet(data, classUID_/*,ts*/);
	}

	void CStoreSCU::readRSP(UINT16& status)//maybe status should be a return value?TODO
	{
		DataSet response;
		readRSP(status, response);
	}

	void CStoreSCU::readRSP(UINT16& status, DataSet& response)
	{
		service_.Read(response);
		response(TAG_STATUS) >> status;
	}
//I'd prefer:
/*
		DataSet CStoreSCU::readRSP();
*/

	CFindSCU::CFindSCU(ServiceBase& service,const UID& classUID)
	: SCU(service,classUID)
	{
	}

	void CFindSCU::writeRQ(const DataSet& data, UINT16 priority)
	{
		CommandSet::CFindRQ rq(uniq16odd(), classUID_, priority);
		service_.WriteCommand(rq, classUID_);
		service_.WriteDataSet(data, classUID_);
	}

	void CFindSCU::readRSP(UINT16& status, DataSet&  data)
	{
		DataSet response;
		readRSP(status, response, data);
	}


	//All the ::readRSP functions from here on are identical: please
	//amalgamate!

	void CFindSCU::readRSP(UINT16& status, DataSet& response, DataSet&  data)
	{
		UINT16 dstype = 0;

		service_.Read(response);
		response(TAG_DATA_SET_TYPE)	>>	dstype;
		response(TAG_STATUS)		>>	status;
		if(dstype!=DataSetStatus::NO_DATA_SET)
			service_.Read(data);

	}

	CGetSCU::CGetSCU(ServiceBase& service,const UID& classUID)
	: SCU(service,classUID)
	{
	}

	void CGetSCU::writeRQ(const DataSet& data, UINT16 priority)
	{
		CommandSet::CGetRQ rq(uniq16odd(), classUID_, priority);
		service_.WriteCommand(rq, classUID_);
		service_.WriteDataSet(data, classUID_);
	}

	void CGetSCU::readRSP(UINT16& status, DataSet&  data)
	{
		DataSet rsp;
		readRSP(status, rsp, data);
	}

	void CGetSCU::readRSP(UINT16& status, DataSet& response, DataSet&  data)
	{
		UINT16 dstype = 0;
		service_.Read(response);
		response(TAG_DATA_SET_TYPE)	>>	dstype;
		response(TAG_STATUS)		>>	status;
		if(dstype!=DataSetStatus::NO_DATA_SET)
			service_.Read(data);

	}

	CMoveSCU::CMoveSCU(ServiceBase& service,const UID& classUID)
	: SCU(service,classUID)
	{
	}

	void CMoveSCU::writeRQ(const string& destAET,
							const DataSet& data, UINT16 priority)
	{
		CommandSet::CMoveRQ rq(uniq16odd(), classUID_, destAET, priority);
		service_.WriteCommand(rq, classUID_);
		service_.WriteDataSet(data, classUID_);
	}

/*
	Now I'm not happy about these extra readRSP members, one of them
	is superfluous
*/

	void CMoveSCU::readRSP(UINT16& status, DataSet&  data)
	{
		DataSet response;
		readRSP(status, response, data);
	}

	void CMoveSCU::readRSP(UINT16& status, DataSet& response, DataSet&  data)
	{
		UINT16 dstype = 0;

		service_.Read(response);
		response(TAG_DATA_SET_TYPE)	>>	dstype;
		response(TAG_STATUS)		>>	status;
		if(dstype!=DataSetStatus::NO_DATA_SET)
			service_.Read(data);

	}
}//namespace dicom
