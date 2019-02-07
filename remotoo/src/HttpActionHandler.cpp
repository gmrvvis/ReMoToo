#include "HttpActionHandler.h"

#ifdef __REMOTOO_CLIPBOARD_SUPPORT__
#include "clipboard.h"
#else
#include <iostream>
#endif

#include "XdoWrapper.h"

namespace remotoo
{
  ActionHandler::ActionHandler ( const std::string & action_ )
    : _action ( action_ )
  {
  }

  ActionHandler::~ActionHandler ( )
  {

  }

  const std::string & ActionHandler::getAction ( )
  {
    return _action;
  }

  // -----------------------------------------------------------

  RequestServerClipboard::RequestServerClipboard ( )
    : ActionHandler ( "RequestServerClipboard" )
  {
  }

  void RequestServerClipboard::handleAction ( Poco::Net::HTTPServerRequest & request_, 
                      Poco::Net::HTTPServerResponse & response_ )
  {
    #ifdef __REMOTOO_CLIPBOARD_SUPPORT__
      XdoWrapper::getInstance ( ).key ( "Control+c" );

			std::string content = clipboard::getInstance ( ).getClipboardContent ( );

			std::string xmlResponse = 
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
        "<clipboard>"
        "<content>";

      xmlResponse += content;

      xmlResponse +=
        "</content>"
        "</clipboard>";

      response_.setStatus ( Poco::Net::HTTPResponse::HTTP_OK );
      response_.send ( ) << xmlResponse;
    #else
      std::cout << "No support for clipboard request: " << request_.getURI ( ) << std::endl;
      response_.setStatus ( Poco::Net::HTTPResponse::HTTP_NOT_FOUND );
      response_.send();
    #endif
  }

  // ----------------------------------------------------------

  HttpActionHandler::HttpActionHandler ( )
  {
    registerActionHandler< RequestServerClipboard > ( );
  }

  bool 
  HttpActionHandler::handleActionRequest ( Poco::Net::HTTPServerRequest & request_, 
                                           Poco::Net::HTTPServerResponse & response_ )
  {
    std::string action = request_.getURI ( );

		if( action.find_first_of ( "/" ) == 0 )
		{
			action = action.substr ( 1, action.length ( ) );
    }

    auto it = _actionsDelegates.find ( action );
    if( it == _actionsDelegates.end ( ) )
    {
      return false;
    }

    it->second.get ( )->handleAction ( request_, response_ );

    return true;
  }
}