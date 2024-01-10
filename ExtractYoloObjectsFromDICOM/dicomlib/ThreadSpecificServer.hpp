#ifndef THREAD_SPECIFIC_SERVER_HPP_INCLUDE_GUARD_32045735827708
#define THREAD_SPECIFIC_SERVER_HPP_INCLUDE_GUARD_32045735827708

#include "ServiceBase.hpp"
#include "Server.hpp"


namespace dicom
{
	//!	stuff in here isn't expected to be directly used by the end programmer
	namespace Implementation
	{

		//!Manages communications with one client connection.  Should be created on it's own thread.
		/*!
			Instances of this class are created by the threading framework and are responsible
			for managing ONE client connection. (Server is the 'managing' class that represents
			the state and parameters of the entire service).

			This class gets treated as a FUNCTOR by Server::Serve()
			This means it gets copied around quite a lot, so it shouldn't have
			too many data members.  It may be that we should introduce some kind
			of intermediate class between Server and ThreadSpecificServer.

			this should probably include a state variable as described in Part8, Table 9.2
		*/

        struct ThreadSpecificServer : public ServiceBase
		{
			Server& server_;

			Network::AcceptedSocket* socket_;
			virtual Network::Socket* GetSocket(){return socket_;}
			bool AssociationNegotiated_;

			//!This gets created by the ThreadedServer framework
			ThreadSpecificServer(Network::AcceptedSocket* socket,Server& s);

			//! Thread function - gets called by newly created thread.
			void operator()();

			//!this gets called when data is available on socket.
			void HandleData();

			void NegotiateAssociation();

			void Dispatch(DataSet& command);

			void ProcessRequest();

            bool InterogateAAssociateRQ(const primitive::AAssociateRQ& association_request);

		};

	}//namespace Implementation

}//namespace dicom
#endif//THREAD_SPECIFIC_SERVER_HPP_INCLUDE_GUARD_32045735827708
