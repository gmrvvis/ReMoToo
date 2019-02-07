#include "clientpackets/SessionInfo.h"

#include "Session.h"

namespace remotoo
{
  namespace clientpackets
  {
    char SessionInfo::getOpcode ( )
    {
      return 0x00;
    }

    void SessionInfo::writeImpl ( )
    {
      const std::string & session = Session::getInstance ( ).getSessionName ( );
      const std::string & user = Session::getInstance ( ).getOwnerName ( );

      writeString ( session );
      writeString ( user );
    }
  }
}