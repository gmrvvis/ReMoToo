#ifndef __REMOTOO_CLIENTPACKETS_SESSIONINFO_H__
#define __REMOTOO_CLIENTPACKETS_SESSIONINFO_H__

#include "ReMoLON_Util/Packet.h"

namespace remotoo
{
  namespace clientpackets
  {
    class SessionInfo : public remolonUtil::SendablePacket
    {
      public:
        char getOpcode ( );
        void writeImpl ( );
    };
  }
}

#endif