#include "ComThread.h"

#include <Poco/Net/SocketAddress.h>
#include <stdlib.h>
#include <pwd.h>

namespace desktopstreamer
{
	ComThread::ComThread(const std::string & name, int * sdf)
		: streamName(name)
		, shutDownFlag(sdf)
	{

	}

	ComThread::~ComThread()
	{

	}

	void ComThread::start()
	{
		running = true;

		Poco::Net::SocketAddress sa("127.0.0.1", 6000);
		socket = std::make_unique<Poco::Net::SecureStreamSocket>(sa);
		ComThread * thisComThread = this;
	
		worker = std::thread
		(
			[thisComThread]
			{
				Poco::Net::SecureStreamSocket * s = thisComThread->getSocket();

				uid_t uid = geteuid();
				struct passwd * pw = getpwuid(uid);
				std::string user (pw->pw_name);

				std::string init = "init;" + user + ";" + thisComThread->getStreamName();

				s->sendBytes(init.data(), (int)init.size());

				char buf[0xff];
				int received;

				while(thisComThread->isRunning())
				{
					received = s->receiveBytes(buf, 0xff);
					if(received > 0)
					{
						std::string packet (buf, received);
		
						if(packet == "shutdown")
						{
							thisComThread->shutDownApp();
							thisComThread->shutDown();
						}
					}
					else // Connection failed
					{
						thisComThread->shutDownApp();
						thisComThread->shutDown();
					}
				}
			}
		);

	}

	void ComThread::shutDown()
	{
		running = false;
	}

	bool ComThread::isRunning()
	{
		return running;
	}
		
	Poco::Net::SecureStreamSocket * ComThread::getSocket()
	{
		return socket.get();
	}

	const std::string & ComThread::getStreamName()
	{
		return streamName;
	}

	void ComThread::shutDownApp()
	{
		*shutDownFlag = 1;
	}
}
