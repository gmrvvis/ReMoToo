#include "clientpackets/AllowAddressResult.h"

namespace remotoo
{
  namespace clientpackets
  {
    AllowAddressResult::AllowAddressResult ( const std::string & address_, int status_ )
     : _address ( address_ )
     , _status ( status_ )
    {
    }

    char AllowAddressResult::getOpcode ( )
    {
      return 0x01;
    }

    void AllowAddressResult::writeImpl ( )
    {
      writeString ( _address );
      writeInt ( _status );
    }
  }
}