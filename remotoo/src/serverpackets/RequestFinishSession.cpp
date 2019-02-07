#include "serverpackets/RequestFinishSession.h"

#include "Session.h"

namespace remotoo
{
  namespace serverpackets
  {
    char RequestFinishSession::getOpcode ( )
    {
      return 0x02;
    }

    void RequestFinishSession::readImpl ( )
    {

    }

    void RequestFinishSession::executePacketAction ( )
    {
      Session::getInstance ( ).finishSession ( );
    }
  }
}