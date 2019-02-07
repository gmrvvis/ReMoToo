cmake_minimum_required ( VERSION 3.2 )
project ( ReMoToo VERSION 0.0.2 )
set( ReMoToo_VERSION_ABI 1 )

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Debug )
endif( NOT CMAKE_BUILD_TYPE )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fPIC -Wno-deprecated-declarations" )

if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
    message( FATAL "no in source building allowed." )
endif( )

if( NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake )
    message( FATAL_ERROR "CMake/common missing, run: git submodule update --init")
endif( )

list( APPEND CMAKE_MODULE_PATH
 ${PROJECT_SOURCE_DIR}/CMake
 ${CMAKE_SOURCE_DIR}/CMake/common )

include( GitExternal )

set( REMOTOO_DESCRIPTION "ReMoToo: ReMo Tools." )
set( REMOTOO_LICENSE LGPL )
set( COMMON_PROJECT_DOMAIN es.gmrv )

include( Common )

common_find_package( Poco REQUIRED COMPONENTS Foundation Net NetSSL )
common_find_package( webstreamer SYSTEM )
common_find_package( ReMo REQUIRED )
common_find_package( xdo REQUIRED )
common_find_package( remolonutil REQUIRED )
#common_find_package( clipboard REQUIRED )

if (NOT WEBSTREAMER_FOUND )
  message ( FATAL "ReMoToo requires WebStreamer! Please build ReMo and ReMoToo with this dependency." )
endif ( )

common_find_package_post( )

add_subdirectory( remotoo )

set( DOCS README.md LICENSE.txt )
install( FILES ${DOCS} DESTINATION share/ReMoToo COMPONENT dev )

set( DOXYGEN_MAINPAGE_MD README.md )
set( DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md )
include( DoxygenRule )

include( CPackConfig )
include( CTest )
