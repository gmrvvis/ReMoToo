#ifndef __DESKTOPSTREAMER_PACKETHANDLER_H__
#define __DESKTOPSTREAMER_PACKETHANDLER_H__

#include "webstreamer/CustomPacketHandler.hpp"

#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace desktopstreamer
{
	namespace network
	{
		// ===============================================================================

		class Packet
		{
			private:
				std::uint8_t opcode;
			public:
				Packet(const std::uint8_t opcode);
				const std::uint8_t & getOpcode();
				virtual void parseData(const void * data, const std::size_t & sizeBytes);
				virtual void doAction();
		};

		// ===============================================================================

		class RequestPasteFromClient : public Packet
		{
			public:
				RequestPasteFromClient();
				void parseData(const void * data, const std::size_t & sizeBytes);
				void doAction();
		};

		// ===============================================================================

		class RequestCopyFromServer : public Packet
		{
			public:
				RequestCopyFromServer();
				void parseData(const void * data, const std::size_t & sizeBytes);
				void doAction();
		};

		// ===============================================================================

		class PacketHandler : public webstreamer::AbstractPacketHandler
		{
			private:
				std::unordered_map<std::uint8_t, std::unique_ptr<Packet>>
			public:
				bool handlePacket(std::uint8_t opcode, const void * data, const std::size_t & sizeBytes);
		};
	}
}

#endif
