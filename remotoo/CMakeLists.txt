include_directories( ${WEBSTREAMER_INCLUDE_DIRS} )
include_directories( ${REMO_INCLUDE_DIRS} )

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

set( REMOTOO_SOURCES  src/XDOTOOLInputHandler.cpp
                      src/ComboKeyHandler.cpp
                      src/ComThread.cpp
                      src/PacketHandler.cpp
                      src/main.cpp
)

set( REMOTOO_HEADERS  src/XDOTOOLInputHandler.h
                      src/ComboKeyHandler.h
                      src/ComThread.h
                      src/PacketHandler.h
)

set( REMOTOO_NAMESPACE remotoo )
set( REMOTOO_INCLUDE_NAMES ReMoToo )

set( REMOTOO_LINK_LIBRARIES Poco::NetSSL
                            ReMo )

common_application( remotoo )
