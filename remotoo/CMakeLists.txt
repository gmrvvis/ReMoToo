include_directories( ${WEBSTREAMER_INCLUDE_DIRS} )
include_directories( ${REMO_INCLUDE_DIRS} )

#TODO: ${REMOLON_UTIL_INCLUDE_DIRS} dows not work, so I force to found the exact directory
include_directories( ${REMOLON_UTIL_INCLUDE_DIRS} )
include_directories( ${CMAKE_SOURCE_DIR}/ReMoLON/ReMoLON_Util/include )

include_directories( ${PROJECT_SOURCE_DIR}
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/flow
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/media
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/stream
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/pipeline
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/util
                     ${PROJECT_SOURCE_DIR}/ReMo/ReMo/util/IO
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/src
                     ${PROJECT_BINARY_DIR} 
)

set( REMOTOO_SOURCES  src/XdoInputHandler.cpp
                      src/ComboKeyHandler.cpp
                      src/PacketHandler.cpp
                      src/Clipboard.cpp
                      src/HttpActionHandler.cpp
                      src/XdoWrapper.cpp
                      src/Session.cpp
                      src/main.cpp
                      src/clientpackets/AllowAddressResult.cpp
                      src/clientpackets/SessionInfo.cpp
                      src/serverpackets/RequestAllowAddress.cpp
                      src/serverpackets/RequestFinishSession.cpp
                      src/serverpackets/RequestSessionInfo.cpp
)

set( REMOTOO_HEADERS  src/XdoInputHandler.h
                      src/ComboKeyHandler.h
                      src/PacketHandler.h
                      src/Clipboard.h
                      src/HttpActionHandler.h
                      src/Session.h
                      src/XdoWrapper.h
                      src/clientpackets/AllowAddressResult.h
                      src/clientpackets/SessionInfo.h
                      src/serverpackets/RequestAllowAddress.h
                      src/serverpackets/RequestFinishSession.h
                      src/serverpackets/RequestSessionInfo.h
)

set( REMOTOO_NAMESPACE remotoo )
set( REMOTOO_INCLUDE_NAMES ReMoToo )

set( REMOTOO_LINK_LIBRARIES ReMo
                            xdo
                            remolon_util )

common_application( remotoo )

# copy configuration file to binary directory
configure_file(${CMAKE_SOURCE_DIR}/sessionClientConfig.cfg ${CMAKE_CURRENT_BINARY_DIR}/sessionClientConfig.cfg COPYONLY)
