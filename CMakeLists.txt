cmake_minimum_required ( VERSION 3.2 )
project ( ReMoToo VERSION 0.0.1)
set( ReMoToo_VERSION_ABI 1 )

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wno-deprecated-declarations")

if( "${PROJECT_SOURCE_DIR}" STREQUAL "${PROJECT_BINARY_DIR}" )
 message( FATAL "no in source building allowed." )
endif( )

#Prepared for common in the future
if (NOT WEBSTREAMER_DIR )
  message (FATAL "ReMoToo requires WebStreamer! Please set the variable WEBSTREAMER_DIR = webstreamer build path con configuration time.")

  list( APPEND CMAKE_MODULE_PATH

   ${PROJECT_SOURCE_DIR}/CMake
   ${CMAKE_SOURCE_DIR}/CMake/common )

  if( NOT EXISTS ${CMAKE_SOURCE_DIR}/CMake/common/Common.cmake )
   message( FATAL_ERROR "CMake/common missing, run: git submodule update --init")
  endif( )

  include( GitExternal )

  set( REMOTOO_DESCRIPTION "ReMoToo: System service to stream the desktop." )
  set( REMOTOO_LICENSE LGPL )
  set( COMMON_PROJECT_DOMAIN es.gmrv )

  include( Common )

  common_find_package(Threads SYSTEM REQUIRED)
  common_find_package(FFmpeg SYSTEM REQUIRED)

  common_find_package(ReMo REQUIRED)

  common_find_package_post( )

  set(DOCS README.md LICENSE.txt)
  install(FILES ${DOCS} DESTINATION share/ReMoToo COMPONENT dev)

  set(DOXYGEN_MAINPAGE_MD README.md)
  set(DOXYGEN_EXTRA_INPUT ${PROJECT_SOURCE_DIR}/README.md)
  include(DoxygenRule)

  include(CPackConfig)
  include(CTest)
else ( NOT WEBSTREAMER_DIR )

  set (CMAKE_PREFIX_PATH "${WEBSTREAMER_DIR}" ${CMAKE_PREFIX_PATH})
  find_package( webstreamer REQUIRED )
  link_directories ( ${WEBSTREAMER_LIB_DIRS} )
  add_definitions ( -DWEBSTREAMER )

  add_subdirectory(ReMo)
endif ( NOT WEBSTREAMER_DIR )

add_subdirectory(remotoo)
