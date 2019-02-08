#include "Session.h"

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
  {

  }

  void Session::startConnection ( )
  {
    remolonUtil::Config cfg ( "./sessionClientConfig.cfg" );

    remolonUtil::TClientConfig clientConfig;
    clientConfig._serverAddress = cfg.getProperty ( "remolonServerAddress" );
		clientConfig._serverPort = cfg.getIntProperty ( "remolonServerPort" );

    _client = std::make_unique < remolonUtil::Client > ( clientConfig );
    remolonUtil::Client * cPtr = _client.get ( );

    // Register receivable packets to be processed
    cPtr->registerReceivablePacket < serverpackets::RequestSessionInfo > ( );
    cPtr->registerReceivablePacket < serverpackets::RequestAllowAddress > ( );
    cPtr->registerReceivablePacket < serverpackets::RequestFinishSession > ( );

    // Connect to remolon
    cPtr->connect ( );

    // Identify the session
    remolonUtil::SendablePacketPtr
    sessionInfo = std::make_unique < clientpackets::SessionInfo > ( );
    cPtr->sendPacket ( sessionInfo );
  }

  remolonUtil::Client & Session::getClient ( )
  {
    return *( _client.get ( ) );
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
    if ( _flow )
    {
      _flow->finish ( );
    }
  }
}