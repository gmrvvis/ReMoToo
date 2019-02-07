#include "XdoWrapper.h"

namespace remotoo
{
  XdoWrapper XdoWrapper::_INSTANCE;

  XdoWrapper & XdoWrapper::getInstance ( )
  {
    return _INSTANCE;
  }

  XdoWrapper::XdoWrapper ( )
  {
    _xdoHandler = xdo_new ( NULL );
  }

  XdoWrapper::~XdoWrapper ( )
  {
    if( _xdoHandler )
    {
      xdo_free ( _xdoHandler );
    }
  }

  void XdoWrapper::keyDown ( const std::string & keySeq_ )
  {
    xdo_send_keysequence_window_down ( _xdoHandler, CURRENTWINDOW, keySeq_.c_str ( ), 0 );
  }

  void XdoWrapper::keyUp ( const std::string & keySeq_ )
  {
    xdo_send_keysequence_window_up ( _xdoHandler, CURRENTWINDOW, keySeq_.c_str ( ), 0 );
  }

  void XdoWrapper::key ( const std::string & keySeq_ )
  {
    xdo_send_keysequence_window ( _xdoHandler, CURRENTWINDOW, keySeq_.c_str ( ), 0 );
  }

  void XdoWrapper::mouseDown ( int button_ )
  {
    xdo_mouse_down ( _xdoHandler, CURRENTWINDOW, button_ );
  }

  void XdoWrapper::mouseUp ( int button_ )
  {
    xdo_mouse_up ( _xdoHandler, CURRENTWINDOW, button_ );
  }

  void XdoWrapper::doubleClick ( int button_ )
  {
    xdo_click_window_multiple ( _xdoHandler, CURRENTWINDOW, button_, 2, 0 );
  }

  void XdoWrapper::mouseMove ( int x_, int y_ )
  {
    xdo_move_mouse ( _xdoHandler, x_, y_, 0 ); // 0 or 4? screen == display??
  }
}