#include <iostream>

#include <flow/flowDeviceToWebStream.h>
#include <pipeline/ImageConverter.h>
#include <media/mediaDesktop.h>
#include <media/mediaWebStreamer.h>
#include <util/Utils.h>
#include <IO/WSInputProcessor.h>

#include <webstreamer/console_logger.hpp>
#include <webstreamer/file_logger.hpp>
#include <webstreamer/stop_watch.hpp>
#include <webstreamer/webstreamer.hpp>

#include "XDOTOOLInputHandler.h"

/*
#define SCREEN_SIZE_X 1920
#define SCREEN_SIZE_Y 1080
*/

int main(int argc, char *argv[])
{
	// ====================================================================
  // Web streamer set up
  using webstreamer::ConsoleLogger;
  using webstreamer::CreateImmortalLogListener;
  using webstreamer::FileLogger;
  using webstreamer::LogLevel;

  CreateImmortalLogListener<ConsoleLogger>(LogLevel::WARNING);

	const unsigned int desktopWidth = 1280;
	const unsigned int desktopHeight = 720;

  //Define the input media and stream
  std::unique_ptr < remo::media >
    im = std::unique_ptr < remo::mediaDesktop > ( new remo::mediaDesktop ( desktopWidth, desktopHeight ));
  std::unique_ptr < remo::stream >
    is = std::unique_ptr < remo::streamDeviceIn > ( new remo::streamDeviceIn
                                                      ( im.get ( )));

  //Define the output media and stream (define the dimensions here if default dont work).
  std::unique_ptr < remo::media > om =
    std::unique_ptr < remo::mediaWebStreamer > ( new remo::mediaWebStreamer ( ));

	remo::WSInputProcessor webstreamerInputProcessor;
	webstreamerInputProcessor.setScreenSize(desktopWidth, desktopHeight);
	remo::mediaWebStreamer * mws = static_cast<remo::mediaWebStreamer*>(om.get());
	mws->setInputProcessor(webstreamerInputProcessor);
	// TODO: Get rid of static accessors
	remo::InputManager::getInstance()
		.registerInputHandler<desktopstreamer::inputhandler::XDOTOOLInputHandler>();

  std::unique_ptr < remo::stream >
    os = std::unique_ptr < remo::streamWebStreamer > ( new remo::streamWebStreamer ( om.get ( )));

  //Define the flow and process
  remo::flowDeviceToWebStream f ( is.get ( ), os.get ( ));
  f.processStreams ( );

  return 0;
}

