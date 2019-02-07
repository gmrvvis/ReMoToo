#ifndef __REMOTOO_SESSION_H__
#define __REMOTOO_SESSION_H__

#include <memory>

#include <ReMoLON_Util/Client.h>

namespace remotoo
{
  class Session
  {
    public:
      static Session & getInstance ( );

      void startConnection ( );
      remolonUtil::Client & getClient ( );
      void setSessionName ( const std::string & sessionName_ );
      void setUserOwnerName ( const std::string & userName_ );

      const std::string & getSessionName ( );
      const std::string & getOwnerName ( );

      void finishSession ( );
    private:
      static Session _INSTANCE;

      std::unique_ptr < remolonUtil::Client > _client;

      std::string _sessionName;
      std::string _userName;
  };
}

#endif