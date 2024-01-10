#ifndef SERVER_HPP_INCLUDE_GUARD_26510884
#define SERVER_HPP_INCLUDE_GUARD_26510884

#ifdef _WIN32
#pragma warning(disable:4275) //boost threads is creating this warning, discussion on the mailing list indicates that it's safe to turn off.
#endif

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#if defined( __unix__)
#include <signal.h>
#endif

#include <set>

#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include "socket/Socket.hpp"
#include "Cdimse.hpp"
#include "ServiceBase.hpp"
#include "DataSet.hpp"
#include "aarq.hpp"
namespace dicom
{
	struct TerminateServerThread : public std::exception{};
/*
	The alternative to setting up all these callbacks would be to use
	inheritance and virtual functions.
*/

#if defined(_MSC_VER)//see comments in Cdimse.hpp for following syntax.
	typedef boost::function1<bool,const std::string&> StringCheckFunction;
	typedef boost::function2<bool,const std::string&,const std::string> StringCheckFunction2;
	typedef boost::function1<void,const primitive::AAssociateRQ&> AssociationNegotiatedFunction;
	typedef boost::function1<void,void> AssociationTerminatedFunction;
#else
	typedef boost::function1<bool,const std::string&> StringCheckFunction;
	typedef boost::function2<bool,const std::string&,const std::string> StringCheckFunction2;
	typedef boost::function<void()> AssociationTerminatedFunction;
	typedef boost::function<void(const primitive::AAssociateRQ&)> AssociationNegotiatedFunction;
#endif


	//!Thrown if we don't have a handler function for the requested service class.
	/*!
		This implies a client has requested an operation that wasn't agreed upon at
		association negotiation.
	*/
	struct NoAvailableHandler:public dicom::exception
	{
		NoAvailableHandler():dicom::exception("No available handler."){}
	};


	//!Facilitates writing DICOM services
	/*!
		This class is responsible for listening on a socket for incoming
		TCP/IP connections.  On receiving a connection, it spawns an instance
		of ThreadSpecificServer in a new thread to handle that connection, and
		continues listening.  This way you can write a dicom service provider
		without having to manually implement any threading or socket code!

		All you need to do is to tell Server which function to call on receipt
		of a given dicom message, using AddHandler().

		This class should probably be a singleton, as I don't know what the
		behaviour would be if you create more than one.

		My goal is to make this the only class that needs to make use of
		mutexes - i.e. be responsible for all data that is shared between
		threads.

		Basic strategy is to have every PUBLIC member function mutex protected.


	*/
    class Server: boost::noncopyable
	{

		//!Only used if Server runs in a new thread.
		/*!
			Server can be run either in calling thread or in a new thread. If
			it is run in a new thread, the following member handles that thread.
			(in either case, it will spawn a thread for every incoming connection.)
		*/
		boost::thread* ServerThread_;

		//!do we want a mutex for every member, or just one?
		boost::mutex mutex_;

		boost::mutex killflag_mutex;

		//!When this is set to true, the Server will stop accepting new connections and eventually terminate.
		bool KillFlag;

		/*
			mutexes on stdout and stderr. Note that these won't help
			if you start more than one instance of Server.  Maybe
			we should make them static?
		*/

		static boost::mutex cerr_mutex;
		static boost::mutex cout_mutex;


		/*
			The following expressions associate abstract syntaxes, (e.g.
			UID_MAMMO_PRES_IMAGE_STORAGE_SOP_CLASS), with functions or functors.
			On receiving a given command, the appropriate handler function will
			be called.  THis allows the user to specify a handler, is type safe,
			and (I think) is less messy than using virtual functions.  Hopefully
			it will make thread-safety easier too.
			(This is a CALL BACK system.)

			This uses the boost::function library to encapsulate functions-as-objects
		*/


		std::map<UID,HandlerFunction> Handlers_;

		/*
			Would it be cleaner to explicitly specify CMoveHandlers_, CGetHandlers_ etc,
			i.e. one container for each C-DIMSE and N-DIMSE message?
		*/

		std::map<UID,CFindFunction> FindHandlers_;

		//!Will be called to validate Local AETs.
		/*!
			Must be set by SetCheckLocalAETFunction() before calling Serve()
		*/
		StringCheckFunction CheckLocalAET;

		//!Will be called to validate Remote AETs.
		/*!
			Must be set by SetCheckRemoteAETFunction() before calling Serve()
		*/
		StringCheckFunction2 CheckRemoteAET;

		//!Mutex for AET check functions.
		/*!
			 Probably not really needed, as they should only be set before we start serving.
		*/
		boost::mutex AETMutex_;

		//!returns true if a client has requested a connection, false if Kill flag has been raised, otherwise blocks.
		bool ClientConnectionPending(Network::Socket* pSocket);

        
        //!Our own thread collection type.
        /*!
            We don't use boost::thread_group here, because we need to be able to clean up threads as we go 
            along, not just at the end of the program.
            This in turn requires us to carefully manage a boolean 'thread finished' flag, that is set in the client
            thread and detected in the main thread.


        */

        typedef std::map<boost::shared_ptr<boost::thread>,bool> ThreadGroup;
        
        //!Each thread owns an open socket connection to a client
        ThreadGroup clientThreads_;

        //!Does housekeeping work on clientThreads_
        void threadCleanup(bool cleanAll);


	public:


        //!signal to server object that current thread is free to be cleaned up
        void allDone();


		//!Logging facilities.
		/*!
			By supplying a class derived from Logger to SetLogger(), you can provide
			custom logging facilities.  (For example, in scippy we provide a class that redirects
			all log messages to a database table.)
		*/
		struct Logger
		{
			//!Defaults to writing to cerr
			virtual void LogError				(std::string Error);
			//!Defaults to writing to cout
			virtual void LogMessage				(std::string Message);
			//!Defaults to zero
			virtual void  AssociationNegotiated	(const primitive::AAssociateRQ& request){}
			//!Defaults to nothing
			virtual void AssociationTerminated	(){}

		}DefaultLogger_;

	private:
		Logger* CurrentLogger_;
	public:

		//!Call this to replace the default logger with a custom one.
		void SetLogger(Logger* logger);

		//!Pipes error message to current logger.
		void LogError(std::string Error);

		//!Pipes message to current logger
		void LogMessage(std::string Message);

		void SetCheckLocalAETCallback(StringCheckFunction f);
		void SetCheckRemoteAETCallback(StringCheckFunction2 f);

		void AddHandler(const UID& uid,HandlerFunction f);
		void AddFindHandler(const UID& uid,CFindFunction Handler);

		HandlerFunction GetHandler(const UID& uid);
		CFindFunction GetFindHandler(const UID& uid);

		bool IsAcceptableRemoteApplicationTitle		(const std::string& Title,std::string ip);
		bool IsAcceptableLocalApplicationTitle		(const std::string& Title);
		bool IsAcceptableApplicationContext			(const UID& uid);
		bool IsAcceptableAbstractSyntax				(const UID& uid);


		//for callback mechanism

		//it's possible we should make these protected members only accesible to
		//ThreadSpecificServer, using the friend keyword.
		void AssociationNegotiated					(const primitive::AAssociateRQ& request);
		void AssociationTerminated					();

		bool CanHandleTransferSyntax				(primitive::TransferSyntax &);

		//these next two seem poorly thought out.
		void GetImplementationClass					(primitive::ImplementationClass &);
		void GetImplementationVersion				(primitive::ImplementationVersion &);

		//!Constructor
		Server();

		//!Destructor.
		virtual ~Server();

		//!Does not return until SIG_TERM is received.
		void Serve(short Port);

		//!alias for Serve()
		void ServeInThisThread(short Port){Serve(Port);}

		//!Only call this once.
		void ServeInNewThread(short port);

		//!Only call this subsequent to calling ServeInNewThread;
		void Stop();

		//!Warn all threads to terminate cleanly
		void RaiseKillFlag();

		//!Has RaiseKillFlag been called
		bool KillFlagRaised();

	};
}//namespace dicom
#endif //SERVER_HPP_INCLUDE_GUARD_26510884
