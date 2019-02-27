#ifndef __REMOTOO_SERVERPACKETS_REQUESTSESSIONINFO_H__
#define __REMOTOO_SERVERPACKETS_REQUESTSESSIONINFO_H__

#include <ReMoLON_Util/Packet.h>

namespace remotoo
{
  namespace serverpackets
  {
    class RequestSessionInfo : public remolonUtil::ReceivablePacket
    {
      public:
        char getOpcode ( );
        void readImpl ( );
        void executePacketAction ( );
    };
  }
}

#endif