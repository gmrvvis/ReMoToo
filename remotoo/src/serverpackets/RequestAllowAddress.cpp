#include "serverpackets/RequestAllowAddress.h"

#include <webstreamer/access_manager.hpp> 

namespace remotoo
{
  namespace serverpackets
  {
    char RequestAllowAddress::getOpcode ( )
    {
      return 0x01;
    }

    void RequestAllowAddress::readImpl ( )
    {
      _newAddress = readString ( );
    }

    void RequestAllowAddress::executePacketAction ( )
    {
      webstreamer::AccessManager::getInstance ( ).allowAddress ( _newAddress );
    }
  }
}