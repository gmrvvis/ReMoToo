#ifndef __REMOTOO_SERVERPACKETS_REQUESTALLOWADDRESS_H__
#define __REMOTOO_SERVERPACKETS_REQUESTALLOWADDRESS_H__

#include <ReMoLON_Util/Packet.h>

namespace remotoo
{
  namespace serverpackets
  {
    class RequestAllowAddress : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
      private:
        std::string _newAddress;
    };
  }
}

#endif