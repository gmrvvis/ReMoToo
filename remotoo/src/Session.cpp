#include "Session.h"

#include <stdlib.h>

#include "ReMoLON_Util/Config.h"

#include "serverpackets/RequestSessionInfo.h"
#include "serverpackets/RequestAllowAddress.h"
#include "serverpackets/RequestFinishSession.h"

#include "clientpackets/SessionInfo.h"

namespace remotoo
{
  Session Session::_INSTANCE;

  Session & Session::getInstance ( )
  {
    return _INSTANCE;
  }

  Session::Session ( )
   : _flow ( nullptr )
   , _xServerPid ( -1 )
  {

  }

  void Session::startConnection ( )
  {
    remolonUtil::Config cfg ( "./sessionClientConfig.cfg" );

    std::string serverAddress = cfg.getProperty ( "remolonServerAddress" );
		uint16_t serverPort = cfg.getIntProperty ( "remolonServerPort" );

    _client = std::unique_ptr < remolonUtil::RawClient > 
              ( new remolonUtil::RawClient ( serverAddress, serverPort ) );
    remolonUtil::RawClient * cPtr = _client.get ( );

    // Register receivable packets to be processed
    cPtr->registerReceivablePacket < serverpackets::RequestSessionInfo > ( );
    cPtr->registerReceivablePacket < serverpackets::RequestAllowAddress > ( );
    cPtr->registerReceivablePacket < serverpackets::RequestFinishSession > ( );

    // Connect to remolon
    cPtr->connect ( );

    // Identify the session
    remolonUtil::SendablePacketPtr
    sessionInfo = std::unique_ptr < clientpackets::SessionInfo > 
                  ( new clientpackets::SessionInfo ( ) );
    cPtr->sendPacket ( sessionInfo );
  }

  remolonUtil::RawClient * Session::getClient ( )
  {
    return _client.get ( );
  }

  void Session::setSessionName ( const std::string & sessionName_ )
  {
    _sessionName = sessionName_;
  }

  void Session::setUserOwnerName ( const std::string & userName_ )
  {
    _userName = userName_;
  }

  void Session::setStreamFlow ( remo::FlowDeviceToWebStream * flow_ )
  {
    _flow = flow_;
  }

  void Session::setXSessionPID ( int xPid_ )
  {
    _xServerPid = xPid_;
  }

  const std::string & Session::getSessionName ( )
  {
    return _sessionName;
  }

  const std::string & Session::getOwnerName ( )
  {
    return _userName;
  }

  void Session::finishSession ( )
  {
    _client.get ( )->close ( );

    if ( _flow )
    {
      _flow->finish ( );
    }

    if ( _xServerPid > 0 )
    {
      std::string closeCommand = "kill -9 " + std::to_string ( _xServerPid );
      system( closeCommand.c_str ( ) );
    }
  }
}