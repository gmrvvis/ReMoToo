#ifdef __REMOTOO_CLIPBOARD_SUPPORT__

#include "Clipboard.h"

#include <stdexcept>

namespace remotoo
{
	Clipboard Clipboard::_INSTANCE;

	Clipboard & Clipboard::getInstance ( )
	{
		return _INSTANCE;
	}

	Clipboard::Clipboard ( )
	{
		_clipBoardHandle = clipboard_new ( NULL );
	}

	Clipboard::~Clipboard ( )
	{
		clipboard_free ( _clipBoardHandle );
	}
	
	std::string Clipboard::getClipboardContent ( )
	{
		std::string result = "";
		try
		{
			int len;
			char * text = clipboard_text_ex ( _clipBoardHandle, &len, LCB_CLIPBOARD );
			result =  std::string ( text, len );
			free ( text );
		}
		catch ( const std::exception & e )
		{
		}

		return result;
	}
		
	void Clipboard::setClipboardContent ( const std::string & newContent_ )
	{
		clipboard_set_text_ex ( _clipBoardHandle, newContent_.c_str ( ), -1, LCB_CLIPBOARD );
	}
}

#endif
