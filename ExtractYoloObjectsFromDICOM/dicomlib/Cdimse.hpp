/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/

#ifndef CDIMSE_HPP_INCLUDE_GUARD_156306638534
#define CDIMSE_HPP_INCLUDE_GUARD_156306638534

#include "DataSet.hpp"
#include "UIDs.hpp"

#include <string>
#include <boost/function.hpp>
#include "ServiceBase.hpp"
#include "CommandSets.hpp"

namespace dicom
{

/*
	First we define the signature of callback functions that will be called
	by instances of ThreadSpecificServer on receiving CDIMSE commands, such as
	C-MOVE, C-FIND etc...

	A developer of a DICOM server must implement functions that match these
	signatures and register them with a Server object by calling Server::AddHandler(...)

	See comments in Server.hpp

	We use boost::function to specify function signatures.  Note that due to limitations
	in Microsoft Visual C++ 7.0, we need to provide two alternative syntaxes.

	We should probably create a Callbacks.hpp file for these typedefs.
*/



#if defined(_MSC_VER)														//MSVC v7.1 and above may not need this workaround.
		typedef boost::function3<void,ServiceBase&,const DataSet&,DataSet&>	//msvc needs this non-standard syntax.  See the boost::function tutorial for reasoning.
			HandlerFunction;												//see boost::function documentation for rationale of alternative syntax.
		typedef boost::function3<void,ServiceBase&,DataSet&,Sequence&>
			CFindFunction;
#else
		typedef boost::function<void(ServiceBase& ,const DataSet& , DataSet&)>
			HandlerFunction;
		typedef boost::function<void(ServiceBase&,DataSet&,Sequence&)>
			CFindFunction;
#endif

	typedef HandlerFunction CMoveFunction;
	typedef HandlerFunction CStoreFunction;
	typedef HandlerFunction CGetFunction;

	void HandleCEcho(ServiceBase& pdu, const DataSet& command,const UID& classUID);

	void HandleCStore(CStoreFunction handler, ServiceBase& pdu, const DataSet& command, const UID& classUID);

	void HandleCFind(CFindFunction  handler,ServiceBase& pdu, const DataSet& command, const UID& classUID);

	void HandleCMove(CMoveFunction handler, ServiceBase& pdu, const DataSet& command, const UID& classUID);

	void HandleCGet(CGetFunction handler, ServiceBase& pdu, const DataSet& command, const UID& classUID);

	class CGetSCP
	{
		HandlerFunction handler_;
 		Sequence m_sq;//is this needed???
	public:
		CGetSCP(HandlerFunction handler):handler_(handler){}
		void handle(ServiceBase& pdu, const DataSet& rqCmd, const UID& classUID);
	};


	/*
		These classes should probably have a common base (that could
		containt the UID object.)
	*/

	//!Service Class User.
	/*!
		Base class for the various Service Class Users
	*/
	class SCU
	{
	protected:
		ServiceBase& service_;
		const UID classUID_;
	public:
		SCU(ServiceBase& service,UID classUID):service_(service),classUID_(classUID){}
	};


	//!Part 4, Annex A

	class CEchoSCU  : public SCU
	{
	public:

		CEchoSCU(ServiceBase& service);//,const UID& classUID = VERIFICATION_SOP_CLASS);
		void writeRQ();
		void readRSP(UINT16& stat_p);
		void readRSP(UINT16& status, DataSet& response);
	};

	class CStoreSCU  : public SCU
	{
	public:
		CStoreSCU(ServiceBase& service,const UID& classUID);
		void writeRQ(const UID& instUID,
			const DataSet& data,/*TS ts,*/ UINT16 priority = Priority::MEDIUM);
		void readRSP(UINT16& status);
		void readRSP(UINT16& status, DataSet& response);
	};

	class CFindSCU  : public SCU
	{
	public:
		CFindSCU(ServiceBase& service,const UID& classUID);
		void writeRQ(const DataSet& data, UINT16 priority = Priority::MEDIUM);
		void readRSP(UINT16& status, DataSet&  data);
		void readRSP(UINT16& status, DataSet& response, DataSet&  data);
	};

	class CGetSCU  : public SCU
	{
	public:
		CGetSCU(ServiceBase& service,const UID& classUID);
		void writeRQ(const DataSet& data, UINT16 priority = Priority::MEDIUM);
		void readRSP(UINT16& status, DataSet&  data);
		void readRSP(UINT16& status, DataSet& response, DataSet&  data);
	};

	class CMoveSCU  : public SCU
	{
		//const std::string m_classUID;
	public:
		CMoveSCU(ServiceBase& service,const UID& classUID);
		void writeRQ(const std::string& destAET,
			const DataSet& data, UINT16 priority = Priority::MEDIUM);
		void  readRSP(UINT16& status, DataSet&  data);
		void readRSP(UINT16& status, DataSet& response, DataSet&  data);
	};
}//namespace dicom
#endif //CDIMSE_HPP_INCLUDE_GUARD_156306638534
