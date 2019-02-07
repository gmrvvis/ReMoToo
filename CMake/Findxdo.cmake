find_path(XDO_INCLUDE_DIR xdo.h)

find_library(XDO_LIBRARY_DIR NAMES xdo)

if ( XDO_LIBRARY_DIR )
	set(XDO_FOUND true)
else ( XDO_LIBRARY_DIR )
  set(XDO_FOUND false)
endif ( XDO_LIBRARY_DIR )

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(XDO REQUIRED_VARS XDO_LIBRARY_DIR XDO_INCLUDE_DIR)
