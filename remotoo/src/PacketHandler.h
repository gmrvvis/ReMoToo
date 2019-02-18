#ifndef __REMOTOO_PACKETHANDLER_H__
#define __REMOTOO_PACKETHANDLER_H__

#include "webstreamer/custom_packet_handler.hpp"

#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace remotoo
{
  class ReceivePacket
  {
    public:
      ReceivePacket ( const std::uint8_t & opcode_ );
      virtual ~ReceivePacket ( );
      const std::uint8_t & getOpcode ( );
      virtual webstreamer::Event::Ptr parseData ( const void * data_, 
                                                  const std::size_t & sizeBytes_ ) = 0;
      virtual void doAction ( webstreamer::Client & client_, 
                              webstreamer::Event::Ptr & event_ ) = 0;
    private:
      std::uint8_t _opcode;
  };

  // ===============================================================================

  class RequestPasteFromClient : public ReceivePacket
  {
    public:
      RequestPasteFromClient ( );
      webstreamer::Event::Ptr parseData ( const void * data_, 
                                          const std::size_t & sizeBytes_ );
      void doAction ( webstreamer::Client & client_, 
                      webstreamer::Event::Ptr & event_ );

      class RequestPasteFromClientEvent : public webstreamer::Event
      {
        public:
          RequestPasteFromClientEvent ( const std::string & content_ );

          std::string _clipContent;
      };
  };

  // ===============================================================================

  // ===============================================================================
  // ===============================================================================

  class PacketHandler : public webstreamer::AbstractPacketHandler
  {
    public:
      PacketHandler ( );

      template < class T >
      void registerPacket ( )
      {
        if( !std::is_base_of < ReceivePacket, T >::value )
        {
          const std::string message = "PacketHandler: attempted to register "
                                      "a non Packet-derived packet handler";
          throw std::runtime_error ( message.c_str ( ) );
        }

        std::unique_ptr < ReceivePacket > newPacket ( new T ( ) );
        _packets [ newPacket.get ( )->getOpcode ( ) ] = std::move ( newPacket );
      }

      webstreamer::Event::Ptr decodePacket ( const void * data_, std::size_t sizeBytes_ );
      bool handlePacket ( webstreamer::Client & client_, webstreamer::Event::Ptr & event_ );
    private:
      std::unordered_map < std::uint8_t, std::unique_ptr < ReceivePacket > > _packets;
  };
}

#endif
