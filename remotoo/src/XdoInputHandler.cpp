#include "XdoInputHandler.h"

#include <iostream>
#include <regex>

#include "ComboKeyHandler.h"
#include "XdoWrapper.h"

namespace remotoo
{
  XdoInputHandler::XdoInputHandler ( )
    : _lastKeyPressed ( "" )
  {
  }

  XdoInputHandler::~XdoInputHandler()
  {
  }

  void XdoInputHandler::ProcessMouseInput ( const webstreamer::MouseEvent & evt_ )
  {
    int screenX = static_cast<int> ( evt_.x ( ) * _screenWidth );
    int screenY = static_cast<int> ( evt_.y ( ) * _screenHeight );

    // Mirror user's mouse movement
    XdoWrapper::getInstance ( ).mouseMove ( screenX, screenY );

    int button = static_cast < int > ( evt_.button ( ) ) + 1;
    webstreamer::MouseAction action = evt_.action ( );

    if ( button > 3 )
    {
      std::cerr << "inputhandler: Mouse button " << button << " not implemented yet!" << std::endl;
      return;
    }

    switch ( action )
    {
      case webstreamer::MouseAction::BUTTON_DOWN:
        XdoWrapper::getInstance ( ).mouseDown ( button ); 
        break;
      case webstreamer::MouseAction::BUTTON_UP:
        XdoWrapper::getInstance ( ).mouseUp ( button );
        break;
      case webstreamer::MouseAction::DOUBLE_CLICK:
        XdoWrapper::getInstance ( ).doubleClick ( button );
        break;
      case webstreamer::MouseAction::MOVE:
        break;
    }
  }

  void XdoInputHandler::ProcessKeyboardInput ( const webstreamer::KeyboardEvent & evt_ )
  {
    if ( evt_.key ( ).empty ( ) )
    {
      return;
    }

    std::string key = evt_.key ( );
    
    webstreamer::KeyboardAction action = evt_.action ( );

    switch ( action )
    {
      case webstreamer::KeyboardAction::KEY_DOWN:
      case webstreamer::KeyboardAction::KEY_PRESS:
        if ( _lastKeyPressed != key )
        {
          if ( _lastKeyPressed != "" )
          {
            XdoWrapper::getInstance ( ).keyUp ( _lastKeyPressed );
            _lastKeyPressed = "";
          }
          if ( !ComboKeyHandler::getInstance ( ).evaluateKeyPress ( key ) )
          {
            XdoWrapper::getInstance ( ).keyDown ( key );
            _lastKeyPressed = key;
          }
        }
        break;
      case webstreamer::KeyboardAction::KEY_UP:
        ComboKeyHandler::getInstance ( ).evaluateKeyUp ( key );
        if ( _lastKeyPressed != "" )
        {
          XdoWrapper::getInstance ( ).keyUp ( _lastKeyPressed );
          _lastKeyPressed = "";
        }
        break;
    }
  }
}