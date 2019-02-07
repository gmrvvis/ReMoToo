#ifndef __REMOTOO_XDOWRAPPER_H__
#define __REMOTOO_XDOWRAPPER_H__

#include <string>

extern "C" {
#include <xdo.h>
}

namespace remotoo
{
  class XdoWrapper
  {
    public:
      static XdoWrapper & getInstance ( );

      ~XdoWrapper ( );

      void keyUp ( const std::string & keySeq_ );
      void keyDown ( const std::string & keySeq_ );
      void key ( const std::string & keySeq_ );

      void mouseMove ( int x_, int y_ );
      void mouseDown ( int button_ );
      void mouseUp ( int button_ );
      void doubleClick ( int button_ );

    private:
      XdoWrapper ( );

      static XdoWrapper _INSTANCE;

      xdo_t * _xdoHandler;
  };
}

#endif