#include "PacketHandler.h"

#include <iostream>

#include "Clipboard.h"
#include "XdoWrapper.h"

namespace remotoo
{
  ReceivePacket::ReceivePacket ( const std::uint8_t & opcode_ )
    : _opcode ( opcode_ )
  {
  }

  ReceivePacket::~ReceivePacket ( )
  {
  }

  const std::uint8_t & ReceivePacket::getOpcode ( )
  {
    return _opcode;
  }

  // ===========================================================================

  RequestPasteFromClient::RequestPasteFromClient ( )
    : ReceivePacket ( 0x52 )
  {
  }
  
  webstreamer::Event::Ptr 
  RequestPasteFromClient::parseData ( const void * data_, 
                                      const std::size_t & sizeBytes_ )
  {
    if ( sizeBytes_ < 1 ) 
    {
      throw std::runtime_error( "Invalid event size" );
    }

    std::size_t size = sizeBytes_ - 1;

    const char * code_begin = static_cast < const char * > ( data_ ) + 1;
    const char * code_end = code_begin + size;

    std::string clipContent = "";
    clipContent.append ( code_begin, code_end );
    clipContent.push_back ( '\0' );

    return std::unique_ptr < RequestPasteFromClientEvent > 
           ( new RequestPasteFromClientEvent ( clipContent ) );
  }

  void RequestPasteFromClient::doAction ( webstreamer::Client & client_, 
                                          webstreamer::Event::Ptr & event_ )
  {
#ifdef __REMOTOO_CLIPBOARD_SUPPORT__
    if ( client_.OwnsInputToken ( ) )
    {
      RequestPasteFromClientEvent * 
        castedEvt = static_cast < RequestPasteFromClientEvent * > ( event_.get ( ) );

      Clipboard::getInstance ( ).setClipboardContent ( castedEvt->_clipContent );
      XdoWrapper::getInstance ( ).key ( "Control+v" );
    }
#else
    if ( client_.OwnsInputToken ( ) )
    {
      std::cout << "No support to handle event of type " << event_.get ( )->ToString ( ) << std::endl;
    }
#endif
  }

  RequestPasteFromClient::RequestPasteFromClientEvent::RequestPasteFromClientEvent ( const std::string & content_ )
    : Event ( static_cast < webstreamer::EventType > ( 0x52 ) )
    , _clipContent ( content_ )
  {
  }

  // ===========================================================================
  // ===========================================================================

  PacketHandler::PacketHandler()
  {
    registerPacket<RequestPasteFromClient>();
  }

  webstreamer::Event::Ptr PacketHandler::decodePacket ( const void * data_, 
                                                        std::size_t sizeBytes_ )
  {
    std::uint8_t op = *reinterpret_cast < const std::uint8_t * > ( data_ );

    auto it = _packets.find ( op );
    if ( it != _packets.end ( ) )
    {
      return it->second.get ( )->parseData ( data_, sizeBytes_ );
    }

    return std::unique_ptr < webstreamer::Event > 
           ( new webstreamer::Event ( webstreamer::EventType::UNKNOWN ) );
  }

  bool PacketHandler::handlePacket ( webstreamer::Client & client_, 
                                     webstreamer::Event::Ptr & event_ )
  {
    std::uint8_t op = static_cast < std::uint8_t > ( event_.get ( )->type ( ) );
    auto it = _packets.find ( op );
    if ( it != _packets.end ( ) )
    {
      it->second.get ( )->doAction ( client_, event_ );
      return true;
    }

    return false;
  }
}
