#ifndef __REMOTOO_CLIENTPACKETS_ALLOWADDRESSRESULT_H__
#define __REMOTOO_CLIENTPACKETS_ALLOWADDRESSRESULT_H__

#include "ReMoLON_Util/Packet.h"

namespace remotoo
{
  namespace clientpackets
  {
    class AllowAddressResult : public remolonUtil::SendablePacket
    {
      public:
        AllowAddressResult ( const std::string & address_, int result_ );
        char getOpcode ( );
        void writeImpl ( );
      private:
        std::string _address;
        int _status;
    };
  }
}

#endif