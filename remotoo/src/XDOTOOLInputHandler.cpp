#include "XDOTOOLInputHandler.h"

#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <regex>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdarg>
#include <stdlib.h>

#include "ComboKeyHandler.h"

namespace desktopstreamer
{
	namespace inputhandler
	{
		XDOTOOLInputHandler::XDOTOOLInputHandler()
			:lastKeyPressed("")
		{

		}

		XDOTOOLInputHandler::~XDOTOOLInputHandler()
		{

		}

		void XDOTOOLInputHandler::handleMouseInput(
			int screenX, 
			int screenY,
			MouseInputButton btn,
			MouseInputState btnState)
		{

			moveMouse(screenX, screenY);

			// Handle clicks
			int button = static_cast<int>(btn) + 1;

			if(button > 3)
			{
				std::cout << "XDOTOOLInputHandler: Mouse button " << button << " not implemented yet!" << std::endl;
				return;
			}

			switch(btnState)
			{
				case inputhandler::MouseInputState::MIS_BUTTON_DOWN:
					mouseButtonDown(button);
					break;
				case inputhandler::MouseInputState::MIS_BUTTON_UP:
					mouseButtonUp(button);
					break;
				case inputhandler::MouseInputState::MIS_BUTTON_DBL_CLICK:
					mouseDblClick(button);
					break;
				default:
					std::cout<<"Unknown button received ... remove dummy configuration!"<<std::endl;
			}
		}

		void XDOTOOLInputHandler::handleKeyInput(
			const std::string & key,
			KeyInputState keyState)
		{
			// Prevent injection
			const std::regex match("^[a-zA-Z0-9_]*");
			if(!std::regex_match(key, match))
			{
				return;
			}

			std::string cmd = "";

			switch(keyState)
			{
				case KeyInputState::KIS_KEY_DOWN:
				case KeyInputState::KIS_KEY_PRESS:
					// If we are holding another key, stop pressing
					// (Combos and sequences are parsed on client)
					if(lastKeyPressed != key)
					{
						if(lastKeyPressed != "")
						{
							keyUp(lastKeyPressed);
							lastKeyPressed = "";
						}
						if(!ComboKeyHandler::getInstance().evaluateKeyPress(key))
						{
							keyDown(key);
							lastKeyPressed = key;
						}
					}
					break;
				case KeyInputState::KIS_KEY_UP:
					ComboKeyHandler::getInstance().evaluateKeyUp(key);
					if(lastKeyPressed != "")
					{
						keyUp(lastKeyPressed);
						lastKeyPressed = "";
					}
					break;
				default:
					std::cout<<"Unknown key received ... remove dummy configuration!"<<std::endl;
			}
		}

		void XDOTOOLInputHandler::moveMouse(int x, int y)
		{
			int child = fork();
			if(child == 0)
			{
				execl("/usr/bin/xdotool", "xdotool", "mousemove", std::to_string(x).c_str(), std::to_string(y).c_str(), (char*)0);
			}
			else if (child > 0)
			{
				int status;
				waitpid(child, &status, 0);
			}
		}

		void XDOTOOLInputHandler::mouseButtonDown(int button)
		{
			int child = fork();
			if(child == 0)
			{
				execl("/usr/bin/xdotool", "xdotool", "mousedown", std::to_string(button).c_str(), (char*)0);
			}
			else if (child > 0)
			{
				int status;
				waitpid(child, &status, 0);
			}
		}

		void XDOTOOLInputHandler::mouseButtonUp(int button)
		{
			int child = fork();
			if(child == 0)
			{
				execl("/usr/bin/xdotool", "xdotool", "mouseup", std::to_string(button).c_str(), (char*)0);
			}
			else if (child > 0)
			{
				int status;
				waitpid(child, &status, 0);
			}
		}

		void XDOTOOLInputHandler::mouseDblClick(int button)
		{
			int child = fork();
			if(child == 0)
			{
				execl("/usr/bin/xdotool", "xdotool", "click", "--repeat", "2", std::to_string(button).c_str(), (char*)0);
			}
			else if (child > 0)
			{
				int status;
				waitpid(child, &status, 0);
			}
		}

		void XDOTOOLInputHandler::keyDown(std::string key)
		{
			int child = fork();
			if(child == 0)
			{
				execl("/usr/bin/xdotool", "xdotool", "keydown", key.c_str(), (char*)0);
			}
			else if (child > 0)
			{
				int status;
				waitpid(child, &status, 0);
			}
		}

		void XDOTOOLInputHandler::keyUp(std::string key)
		{
			int child = fork();
			if(child == 0)
			{
				execl("/usr/bin/xdotool", "xdotool", "keyup", key.c_str(), (char*)0);
			}
			else if (child > 0)
			{
				int status;
				waitpid(child, &status, 0);
			}
		}
	}
}
