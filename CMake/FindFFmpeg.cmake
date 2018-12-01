#[[
* Copyright (c) 2018 CCS/UPM - GMRV/URJC.
*
* Authors: Juan Pedro Brito Méndez <juanpedro.brito@upm.es>
*
* This library is free software; you can redistribute it and/or modify it under
* the terms of the GNU Lesser General Public License version 3.0 as published
* by the Free Software Foundation.
*
* This library is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
* details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
]]

if (FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIR)

  set(FFMPEG_FOUND TRUE)

else ()

  if (FFMPEG_LOCAL_DIR)
    include_directories(SYSTEM "${FFMPEG_LOCAL_DIR}/include")
    set(CMAKE_LIBRARY_PATH "${FFMPEG_LOCAL_DIR}/lib" ${CMAKE_LIBRARY_PATH})
    set(ENV{PKG_CONFIG_PATH}  "${FFMPEG_LOCAL_DIR}/lib/pkgconfig")
  endif (FFMPEG_LOCAL_DIR)

  set (FFMPEG_PKGS_LIBS libavcodec     libavformat     libavutil     libavfilter     libavdevice     libswscale     libswresample     libx264     libx265     libvpx)
  set (FFMPEG_PKGS_MODS FFMPEG_AVCODEC FFMPEG_AVFORMAT FFMPEG_AVUTIL FFMPEG_AVFILTER FFMPEG_AVDEVICE FFMPEG_SWSCALE FFMPEG_SWRESAMPLE FFMPEG_X264 FFMPEG_X265 FFMPEG_VPX)

  find_package(PkgConfig)
  
  if (PKG_CONFIG_FOUND)
    list(LENGTH FFMPEG_PKGS_LIBS NUMLIBS)
    math(EXPR len "${NUMLIBS} - 1")
    foreach(iter RANGE ${len})
      list(GET FFMPEG_PKGS_LIBS ${iter} lib)
      list(GET FFMPEG_PKGS_MODS ${iter} mod)
      set ( PKG_MOD PKG_${mod} )
      pkg_check_modules(PKG_MOD ${lib})
    endforeach()
  endif (PKG_CONFIG_FOUND)

  set (FFMPEG_STD_HEADER_LOCATION /usr/include /usr/local/include /opt/local/include /sw/include)
  find_path(FFMPEG_AVCODEC_INCLUDE_DIR NAMES libavcodec/avcodec.h
  PATHS ${_FFMPEG_AVCODEC_INCLUDE_DIRS} "${FFMPEG_LOCAL_DIR}/include" /usr/include /usr/local/include /opt/local/include /sw/include
  PATH_SUFFIXES ffmpeg libav)

  set ( FFMPEG_STD_LIB_LOCATION /usr/lib /usr/local/lib /opt/local/lib /sw/lib /usr/lib/x86_64-linux-gnu /usr/freeware/lib /usr/csw/lib $ENV{FFMPEG_DIR} )
  set ( FFMPEG_LIBS_LIST avcodec avformat avutil avfilter avdevice swscale swresample x264 x265 vpx )

  #Compact version does not work!
  list(LENGTH FFMPEG_LIBS_LIST NUM_LIBS)
  math(EXPR leng2 "${NUM_LIBS} - 1")
  foreach(itr RANGE ${leng2})
    list(GET FFMPEG_LIBS_LIST ${itr} lib)
    list(GET FFMPEG_PKGS_MODS ${itr} libDir)
    set (libDirFull "PKG_${libDir}_LIBRARY_DIRS")

    find_library(FFMPEG_${lib} NAMES ${lib}
                 PATHS  ${libDirFull} "${FFMPEG_LOCAL_DIR}/lib" ${FFMPEG_STD_LIB_LOCATION}  )

    if ( NOT FFMPEG_${lib} STREQUAL "" )
      set(FFMPEG_${lib}_FOUND 1)
    endif( )

    #Append each lib
    list(APPEND FFMPEG_LIBRARIES ${FFMPEG_${lib}})
  endforeach()

  #Minimal libs for ReMo
  if ( FFMPEG_avcodec_FOUND AND FFMPEG_avformat_FOUND
       AND FFMPEG_avdevice_FOUND AND FFMPEG_avdevice_FOUND)
    set(FFMPEG_FOUND TRUE)
    set(FFMPEG_INCLUDE_DIR ${FFMPEG_AVCODEC_INCLUDE_DIR})
    message(STATUS "Libs Found: --->>>> ${FFMPEG_LIBRARIES} ")
  else()
    message(FATAL_ERROR "Could not find libavcodec or libavformat or libavutil or libavdevice")
  endif(FFMPEG_avcodec_FOUND AND FFMPEG_avformat_FOUND
       AND FFMPEG_avdevice_FOUND AND FFMPEG_avdevice_FOUND)
endif(FFMPEG_LIBRARIES AND FFMPEG_INCLUDE_DIR)
