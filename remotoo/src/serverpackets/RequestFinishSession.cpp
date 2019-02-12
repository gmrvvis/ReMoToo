#include "serverpackets/RequestFinishSession.h"

#include "Session.h"

#include <iostream>

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
      std::cout << "REMOTOO: Received kill signal" << std::endl;
      Session::getInstance ( ).finishSession ( );
    }
  }
}