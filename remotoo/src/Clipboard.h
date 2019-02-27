#ifdef __REMOTOO_CLIPBOARD_SUPPORT__

#ifndef __REMOTOO_CLIPBOARD_H__
#define __REMOTOO_CLIPBOARD_H__

#include "libclipboard.h"

#include <string>

namespace remotoo
{
	class Clipboard
	{
    public:
      static Clipboard & getInstance ( );

      ~Clipboard ( );
		
			std::string getClipboardContent ( );
			void setClipboardContent ( const std::string & newContent_ );
    private:
      Clipboard ( );

      static Clipboard _INSTANCE;

      clipboard_c * _clipBoardHandle;
	};
}

#endif
#endif
