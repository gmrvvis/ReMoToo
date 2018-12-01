#ifndef __DESKTOPSTREAMER_INPUTHANDLER_XDOTOOLINPUTHANDLER_H__
#define __DESKTOPSTREAMER_INPUTHANDLER_XDOTOOLINPUTHANDLER_H__

#include <util/IO/Abstractinputhandler.h>

#include <string>

namespace desktopstreamer
{
	namespace inputhandler
	{
		using namespace remo::inputHandler;
		class XDOTOOLInputHandler : public AbstractInputHandler
		{
			private:
				std::string lastKeyPressed;
				std::string currentWindow;
			public:
				XDOTOOLInputHandler();
				~XDOTOOLInputHandler();

				void handleMouseInput(
					int screenX, 
					int screenY,
					MouseInputButton btn,
					MouseInputState btnState);

				void handleKeyInput(
					const std::string & key,
					KeyInputState keyState);
			private:
				void moveMouse(int x, int y);
				void mouseButtonDown(int button);
				void mouseButtonUp(int button);
				void mouseDblClick(int button);
				void keyDown(std::string key);
				void keyUp(std::string key);
		};
	}
}

#endif
