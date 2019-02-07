#ifndef __REMOTOO_XDOINPUTHANDLER_H__
#define __REMOTOO_XDOINPUTHANDLER_H__

#ifndef REMO_USE_WEBSTREAMER
#define REMO_USE_WEBSTREAMER
#endif

#include <IO/WebstreamerInputProcessor.h>

#include <string>

namespace remotoo
{
  class XdoInputHandler : public remo::WebstreamerInputProcessor
  {
    public:
      XdoInputHandler ( );
      ~XdoInputHandler ( );

      void ProcessMouseInput ( const webstreamer::MouseEvent & me_ ) override;
      void ProcessKeyboardInput ( const webstreamer::KeyboardEvent & ke_ ) override;
      
    private:
      std::string _lastKeyPressed;
  };
}

#endif