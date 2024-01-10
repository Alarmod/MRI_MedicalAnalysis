/************************************************************************
*	DICOMLIB
*	Copyright 2003 Sunnybrook and Women's College Health Science Center
*	Implemented by Trevor Morgan  (morgan@sten.sunnybrook.utoronto.ca)
*
*	See LICENSE.txt for copyright and licensing info.
*************************************************************************/

#include "aarq.hpp"
#include "aaac.hpp"
#include "aarj.hpp"
#include "Dumper.hpp"
#include <sstream>

#include "ThreadSpecificServer.hpp"
using std::endl;
namespace dicom
{

	namespace Implementation
	{

		using namespace primitive;

		//!One of these is spawned on a new thread for each new connection being served.  See also class Server
		/*!
			We won't do association here, as the object is _constructed_ in the master
			thread, and we want to get it passed to the child thread as soon as possible.
			(See Server::Serve() to see how ThreadSpecificServer gets created and used)
		*/
		ThreadSpecificServer::ThreadSpecificServer(Network::AcceptedSocket* socket,Server& s)
		//:ServiceBase()
		:server_(s)
		,socket_(socket)
		,AssociationNegotiated_(false)
// 		,association_identifier_(0)
		{
		}

		/*!
			This will get called by the threading framework, in the context
			of a new thread.

			To provide monitoring capabilities, we might want to provide two callbacks:
				AssociationNegotiated()/AssociationClosed(), so that a dicom server can
				e.g. keep track of number of connections.  It should probably also provide
				an option to reject an association, eg if too many connections are
				present.
		*/
		void ThreadSpecificServer::operator()()
		{

			//Now we start up a message loop to handle incoming requests.
			try
			{
				while(!server_.KillFlagRaised())//do we need to inform the client if we're shutting down?
					if(socket_->MoreData(1))
						HandleData();
			}
			catch(TerminateServerThread)//this is expected
			{
				server_.AssociationTerminated();
			}
			catch(std::exception& e)//this is unexpected.
			{
				server_.LogError(e.what());
				server_.LogError("This connection will close.");
			}
			/*unfortunate to have this here, but the only way I can think of that
			closes sockets aggresively enough*/
			delete socket_;
		}//thread will be destroyed after function exits.

		void ThreadSpecificServer::HandleData()
		{
			if(!AssociationNegotiated_)
				NegotiateAssociation();
			else
				ProcessRequest();
		}

		/*!
			I think this should be calling a callback to let
			the application monitor connections.
		*/
		void ThreadSpecificServer::NegotiateAssociation()
		{
			AAssociateRQ& association_request=AAssociateRQ_;//BAD BAD BAD
			//AAssociateRQ association_request;
			association_request.Read(*socket_);


			if(InterogateAAssociateRQ(association_request))
			{
				AssociationNegotiated_=true;
				server_.AssociationNegotiated(association_request);

			}
			else
			{
				//should we go into some kind of waiting phase for the
				//client to shut down the association?

				throw FailedAssociation();//will get caught and cause the thread to end cleanly.
			}
		}


		void ThreadSpecificServer::Dispatch(DataSet& command)
		{

		/*
			notes.

			C-ECHO should always be handled by the library - it's behaviour doesn't
			change, so we don't need to support user overrides.
		*/

			/*lot's of implementation work still to be done here*/


			Command::Code cmd;
			UID classUID("");
			command(TAG_CMD_FIELD) >> cmd;


			/*
				Where is the following operation specified???
			*/
			if(command.find(TAG_AFF_SOP_CLASS_UID)!=command.end())
				command(TAG_AFF_SOP_CLASS_UID) >> classUID;
			else if(command.find(TAG_REQ_SOP_CLASS_UID)!=command.end())
				command(TAG_REQ_SOP_CLASS_UID) >> classUID;
			else
			{
				std::ostringstream os;
				os << "Unknown command: " << command;
				server_.LogError(os.str());
				throw dicom::exception(os.str());
			}

			/*
			cmd must be one of the command values specified in Tag.hpp
			*/

			/*
				This should be in a seperate 'Dispatch()' function
			*/
			switch(cmd)
			{

			case Command::C_ECHO_RQ:
				HandleCEcho(*this,command,classUID);
				break;

			case Command::C_STORE_RQ:
				{
					HandlerFunction handler=server_.GetHandler(classUID);
					HandleCStore(handler,*this,command,classUID);
					server_.LogMessage("Handled a C-STORE");
				}
				break;

			case Command::C_FIND_RQ:
				{
					CFindFunction handler=server_.GetFindHandler(classUID);
					HandleCFind(handler,*this,command,classUID);
				}
				break;
			case Command::C_MOVE_RQ:
			{
				HandlerFunction handler=server_.GetHandler(classUID);
				HandleCMove(handler,*this,command,classUID);
			}
			break;

			//case Command::C_CANCEL_RQ:
			//	return ;	// ignored by this version
// 			case Command::N_CREATE_RQ:
// 				return
// 				return NCreateSCP(getNHandler(m_mapNCreate, classUID)).handle(pdu, rq, classUID);
// 			case Command::N_SET_RQ:
// 				return NSetSCP(getNHandler(m
			default:
				throw dicom::exception("unsupported operation requested");
			}

		}

		void ThreadSpecificServer::ProcessRequest()
		{

			//read command, dispatch command.
			//watch for closing request.
			DataSet command;

			if(!Read(command))
			{

				//failure means that the association has been succesfully terminated.
				throw TerminateServerThread();//will gracefully close this serving thread.
			}
			else
			{
				//dispatch the command/data set.
				/* what if there are more???*/
				Dispatch(command);
				//don't we need to send back a response here?
			}
		}



		/*!
			Please re think this so the error handling is seperate from the
			rest of the implementation.
		*/

		bool ThreadSpecificServer::InterogateAAssociateRQ(const AAssociateRQ& association_request)
		{
			if(!server_.IsAcceptableRemoteApplicationTitle(association_request.CallingAppTitle_,socket_->get_remote_ip()))
			{
				AAssociateRJ Rejection (AAssociateRJ::REJECTED_PERMANENT,
					AAssociateRJ::DICOM_SERVICE_USER,
					AAssociateRJ::CALLING_AE_NOT_RECOGNIZED);
				server_.LogMessage("Bad Remote AET");
				Rejection.Write (*socket_);

				return ( false );
			}
			if(!server_.IsAcceptableLocalApplicationTitle(association_request.CalledAppTitle_))
			{
				AAssociateRJ Rejection (AAssociateRJ::REJECTED_PERMANENT,
					AAssociateRJ::DICOM_SERVICE_USER,
					AAssociateRJ::CALLED_AE_NOT_RECOGNIZED);
				server_.LogMessage("Bad Local AET");
				Rejection.Write (*socket_);
				return ( false );
			}
			if(!server_.IsAcceptableApplicationContext(association_request.AppContext_.UID_))
			{
				AAssociateRJ Rejection (AAssociateRJ::REJECTED_PERMANENT,
					AAssociateRJ::DICOM_SERVICE_USER,
					AAssociateRJ::APPLICATION_CONTEXT_NOT_SUPPORTED);
				server_.LogMessage("Bad Application Context");
				Rejection.Write ( *socket_);
				return ( false );
			}
			//ok, we got this far, so we'll send back an acceptance message.
			AAssociateAC Acceptance;
			// Transfer the information over to the A-ASSOCIATE-AC Class

			Acceptance.CalledAppTitle_=association_request.CalledAppTitle_;
			Acceptance.CallingAppTitle_=association_request.CallingAppTitle_;

			Acceptance.AppContext_ = association_request.AppContext_;
			Acceptance.UserInfo_ = association_request.UserInfo_;

			UINT32 ValidPresentationContexts;

			UserInformation				UserInfo;
			MaximumSubLength			MaxSubLength;

			// Now we need to find an acceptable presentation context

			size_t Index = 0;
			ValidPresentationContexts = 0;

			//client has proposed the following presentation contexts, we choose which we like
			std::vector<PresentationContext> ProposedPresentationContexts = (association_request.ProposedPresentationContexts_);



			while ( Index < ProposedPresentationContexts.size())
			{
				PresentationContextAccept	AcceptedPresentationContext;//Accept or reject, keep a record -Sam
				PresentationContext PresContext = ProposedPresentationContexts.at ( Index );

				size_t IndexTrn = 0;
				AcceptedPresentationContext.PresentationContextID_ = PresContext.ID_;	
				//client expects this (non-unique) identifier to be bounced back, see
				//part 8 table 9.13 and part 8 / 7.1.1.13

				//Presumed transfersyntax not supported, correct it otherwise -Sam
				AcceptedPresentationContext.Result_ = 4;
				while (IndexTrn < PresContext.TransferSyntaxes_.size())
				{
					TransferSyntax TrnSyntax = PresContext.TransferSyntaxes_.at(IndexTrn);

					//we always need to assign a TxSyntax here, even we reject it Table 9-18 -Sam
					AcceptedPresentationContext.TrnSyntax_.UID_ = TrnSyntax.UID_;
					if(server_.CanHandleTransferSyntax(TrnSyntax))
					{
						AcceptedPresentationContext.Result_ = 0;
						++ValidPresentationContexts;
						break;  //first come, first serve -Sam
					}
					++IndexTrn;
				}
				if(!server_.IsAcceptableAbstractSyntax(PresContext.AbsSyntax_.UID_))
					AcceptedPresentationContext.Result_ = 3;
				AcceptedPresentationContexts_.push_back ( AcceptedPresentationContext );//Accept or reject, keep a record -Sam

				++Index;
			}
			if(!ValidPresentationContexts)
			{//what does this bit mean?

				server_.LogError ("no valid pres context");
				Acceptance.Write (*socket_);
				return ( false );
			}
			MaxSubLength.Set(16384);	// we can do all DICOM can handle
			server_.GetImplementationClass(UserInfo.ImpClass_);
			server_.GetImplementationVersion(UserInfo.ImpVersion_);
			UserInfo.SetMax(MaxSubLength);

			Acceptance.PresContextAccepts_=AcceptedPresentationContexts_;//do this later outside of while loop -Sam
			Acceptance.SetUserInformation ( UserInfo );


			Acceptance.Write(*socket_);


			return ( true );	// yes, the communication should work out
		}

	}//	namespace Implementation
}//namespace dicom
