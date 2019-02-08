#include <iostream>

#ifndef REMO_USE_WEBSTREAMER
#define REMO_USE_WEBSTREAMER
#endif

#include <flow/FlowDeviceToWebStream.h>
#include <pipeline/ImageConverter.h>
#include <media/MediaDesktop.h>
#include <media/MediaWebStreamer.h>
#include <util/Utils.h>

#include <webstreamer/console_logger.hpp>
#include <webstreamer/file_logger.hpp>
#include <webstreamer/stop_watch.hpp>
#include <webstreamer/webstreamer.hpp>
#include <webstreamer/custom_http_action_handler.hpp>
#include <webstreamer/custom_packet_handler.hpp>
#include <webstreamer/access_manager.hpp>

#include "XdoInputHandler.h"
#include "PacketHandler.h"
#include "HttpActionHandler.h"
#include "Session.h"

//
//int main( void )
int main(int argc, char *argv[])
{
	// ====================================================================
  // Web streamer set up
  using webstreamer::ConsoleLogger;
  using webstreamer::CreateImmortalLogListener;
  using webstreamer::FileLogger;
  using webstreamer::LogLevel;

  int webPort = -1, webSockPort = -1, webRtcPort = -1;

  webstreamer::AccessManager::getInstance ( ).setAccessControlEnabled ( false );

  // remotoo <session name> <user owner> <user IP> <webPort> <webSockPort> <webRtcPort>
  if ( argc >= 7)
  {
    std::string sessionName ( argv [ 1 ] );
    std::string userName ( argv [ 2 ] );
    std::string userIP ( argv [ 3 ] );
    webPort = std::stoi ( argv [ 4 ] ); 
    webSockPort = std::stoi ( argv [ 5 ] );
    webRtcPort = std::stoi ( argv [ 6 ] );

    remotoo::Session::getInstance ( ).setSessionName ( sessionName );
    remotoo::Session::getInstance ( ).setUserOwnerName ( userName );
    webstreamer::AccessManager::getInstance ( ).allowAddress ( userIP );
  }

  remotoo::Session::getInstance ( ).startConnection ( );

  CreateImmortalLogListener<ConsoleLogger>(LogLevel::WARNING);

	const unsigned int desktopWidth = 1280;
	const unsigned int desktopHeight = 720;

  //Define the input media and stream
  std::unique_ptr < remo::Media >
    im = std::unique_ptr < remo::MediaDesktop > ( new remo::MediaDesktop ( desktopWidth, desktopHeight ));

  // Configure to capture display 4.0
  remo::MediaDesktop * mediaDesktopPtr = static_cast < remo::MediaDesktop * > ( im.get ( ) );
  mediaDesktopPtr->setDesktopConfigAsString ( ":4.0" );

  std::unique_ptr < remo::Stream >
    is = std::unique_ptr < remo::StreamDeviceIn > ( new remo::StreamDeviceIn
                                                      ( im.get ( )));

  //Define the output media and stream (define the dimensions here if default dont work).
  std::unique_ptr < remo::Media > om =
    std::unique_ptr < remo::MediaWebStreamer > ( new remo::MediaWebStreamer ( webPort, webSockPort, webRtcPort ));

	//remo::WSInputProcessor webstreamerInputProcessor;
  remotoo::XdoInputHandler handler;
	//handler.setScreenSize(desktopWidth, desktopHeight);
	remo::MediaWebStreamer * mws = static_cast<remo::MediaWebStreamer*>(om.get());
	mws->setInputProcessor(handler);
	
  webstreamer::PacketHandlerManager::getInstance ( )
    .registerPacketHandler < remotoo::PacketHandler > ( );

  webstreamer::ActionHandlerManager::getInstance ( )
    .registerActionHandler < remotoo::HttpActionHandler > ( );
	

  std::unique_ptr < remo::Stream >
    os = std::unique_ptr < remo::StreamWebStreamer > ( new remo::StreamWebStreamer ( om.get ( )));

  //Define the flow and process
  remo::FlowDeviceToWebStream f ( is.get ( ), os.get ( ));
  f.processStreams ( );

  return 0;
}