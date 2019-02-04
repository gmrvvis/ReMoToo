#include <iostream>

#ifndef REMO_USE_WEBSTREAMER
#define REMO_USE_WEBSTREAMER
#endif

#include <flow/FlowDeviceToWebStream.h>
#include <pipeline/ImageConverter.h>
#include <media/MediaDesktop.h>
#include <media/MediaWebStreamer.h>
#include <util/Utils.h>

#include <IO/WebstreamerInputProcessor.h>

#include <webstreamer/console_logger.hpp>
#include <webstreamer/file_logger.hpp>
#include <webstreamer/stop_watch.hpp>
#include <webstreamer/webstreamer.hpp>

#include "XDOTOOLInputHandler.h"

//int main(int argc, char *argv[])
int main( void )
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
  std::unique_ptr < remo::Media >
    im = std::unique_ptr < remo::MediaDesktop > ( new remo::MediaDesktop ( desktopWidth, desktopHeight ));
  std::unique_ptr < remo::Stream >
    is = std::unique_ptr < remo::StreamDeviceIn > ( new remo::StreamDeviceIn
                                                      ( im.get ( )));

  //Define the output media and stream (define the dimensions here if default dont work).
  std::unique_ptr < remo::Media > om =
    std::unique_ptr < remo::MediaWebStreamer > ( new remo::MediaWebStreamer ( ));

	//remo::WSInputProcessor webstreamerInputProcessor;
  remo::WebstreamerInputProcessor webstreamerInputProcessor_;
	webstreamerInputProcessor_.setScreenSize(desktopWidth, desktopHeight);
	remo::MediaWebStreamer * mws = static_cast<remo::MediaWebStreamer*>(om.get());
	mws->setInputProcessor(webstreamerInputProcessor_);
	// TODO: Get rid of static accessors
	remo::InputManager::getInstance()
		.registerInputHandler<desktopstreamer::inputhandler::XDOTOOLInputHandler>();

  std::unique_ptr < remo::Stream >
    os = std::unique_ptr < remo::StreamWebStreamer > ( new remo::StreamWebStreamer ( om.get ( )));

  //Define the flow and process
  remo::FlowDeviceToWebStream f ( is.get ( ), os.get ( ));
  f.processStreams ( );

  return 0;
}
