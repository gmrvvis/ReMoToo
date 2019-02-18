#include "serverpackets/RequestSessionInfo.h"

#include "Session.h"
#include "clientpackets/SessionInfo.h"

namespace remotoo
{
  namespace serverpackets
  {
    char RequestSessionInfo::getOpcode ( )
    {
      return 0x00;
    }

    void RequestSessionInfo::readImpl ( )
    {

    }

    void RequestSessionInfo::executePacketAction ( )
    {
      remolonUtil::RawClient * clnt = Session::getInstance ( ).getClient ( );

      remolonUtil::SendablePacketPtr info ( new clientpackets::SessionInfo ( ) );

      clnt->sendPacket ( info );
    }
  }
}