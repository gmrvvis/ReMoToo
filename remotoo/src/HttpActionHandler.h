#ifndef __REMOTOO_HTTPACTIONHANDLER_H__
#define __REMOTOO_HTTPACTIONHANDLER_H__

#include <webstreamer/custom_http_action_handler.hpp>

#include <unordered_map>
#include <string>
#include <stdexcept>

namespace remotoo
{
  class ActionHandler
  {
    public:
      ActionHandler ( const std::string & action_ );
      virtual ~ActionHandler ( );

      const std::string & getAction ( );

      virtual void handleAction ( Poco::Net::HTTPServerRequest & request_, 
                                  Poco::Net::HTTPServerResponse & response_ ) = 0;
    private:
      std::string _action;
  };

  class RequestServerClipboard : public ActionHandler
  {
    public:
      RequestServerClipboard ( );

      void handleAction ( Poco::Net::HTTPServerRequest & request_, 
                          Poco::Net::HTTPServerResponse & response_ );
  };

  class HttpActionHandler : public webstreamer::AbstractHTTPActionHandler
  {
    public:
      HttpActionHandler ( );

      template < class T >
      void registerActionHandler ( )
      {
        if ( !std::is_base_of < ActionHandler, T >::value )
        {
          std::string message = "Attempted to register a non "
                                "ActionHandler-derived delegate to "
                                "handle incomming http requests";
          throw std::runtime_error ( message.c_str ( ) );
        }

        std::unique_ptr < ActionHandler > newHandler = std::make_unique < T > ( );
        ActionHandler * rawHandler = newHandler.get ( );
        _actionsDelegates [ rawHandler->getAction ( ) ] = std::move ( newHandler );
      }

      bool handleActionRequest ( Poco::Net::HTTPServerRequest & request_, 
                                 Poco::Net::HTTPServerResponse & response_ );

    private:
      std::unordered_map < std::string, std::unique_ptr < ActionHandler > > _actionsDelegates;
  };
}

#endif