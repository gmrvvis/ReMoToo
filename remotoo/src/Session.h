#ifndef __REMOTOO_SESSION_H__
#define __REMOTOO_SESSION_H__

#include <memory>

#ifndef REMO_USE_WEBSTREAMER
#define REMO_USE_WEBSTREAMER
#endif

#include <ReMo/flow/FlowDeviceToWebStream.h>
#include <ReMoLON_Util/Client.h>

namespace remotoo
{
  class Session
  {
    public:
      static Session & getInstance ( );

      void startConnection ( );
      remolonUtil::RawClient * getClient ( );
      void setSessionName ( const std::string & sessionName_ );
      void setUserOwnerName ( const std::string & userName_ );
      void setStreamFlow ( remo::FlowDeviceToWebStream * flow_ );
      void setXSessionPID ( int xPid_ );

      const std::string & getSessionName ( );
      const std::string & getOwnerName ( );

      void finishSession ( );
    private:
      static Session _INSTANCE;

      Session ( );

      remo::FlowDeviceToWebStream * _flow;

      std::unique_ptr < remolonUtil::RawClient > _client;

      std::string _sessionName;
      std::string _userName;
      int _xServerPid;
  };
}

#endif
