#ifndef __DESKTOPSTREAMER_COMTHREAD_H__
#define __DESKTOPSTREAMER_COMTHREAD_H__

#include <thread>
#include <memory>

#include <Poco/Net/SecureStreamSocket.h>

namespace desktopstreamer
{
	typedef std::unique_ptr<Poco::Net::SecureStreamSocket> SocketPtr;
 
	class ComThread
	{
		private:
			SocketPtr socket;
			std::thread worker;

			std::string streamName;

			int * shutDownFlag;

			bool running;
		public:
			ComThread(const std::string & name, int * sdf);
			~ComThread();

			void start();
			void shutDown();

			bool isRunning();
			Poco::Net::SecureStreamSocket * getSocket();
			const std::string & getStreamName();
			void shutDownApp();
	};
}

#endif
