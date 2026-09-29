# FindLibUSB1.cmake
#
# Locate libusb-1.0 via pkg-config.
#
# Imported targets:
#   LibUSB1::LibUSB1   (when found)
#
# Result variables:
#   LibUSB1_FOUND
#   LibUSB1_VERSION
#   LibUSB1_INCLUDE_DIRS
#   LibUSB1_LIBRARIES

find_package(PkgConfig QUIET)

if(PkgConfig_FOUND)
    pkg_check_modules(PC_LIBUSB1 QUIET libusb-1.0)
endif()

find_path(LibUSB1_INCLUDE_DIR
    NAMES libusb.h
    HINTS ${PC_LIBUSB1_INCLUDEDIR}
    PATH_SUFFIXES libusb-1.0
)

find_library(LibUSB1_LIBRARY
    NAMES usb-1.0 usb
    HINTS ${PC_LIBUSB1_LIBDIR}
)

set(LibUSB1_VERSION "${PC_LIBUSB1_VERSION}")
set(LibUSB1_INCLUDE_DIRS "${LibUSB1_INCLUDE_DIR}")
set(LibUSB1_LIBRARIES    "${LibUSB1_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibUSB1
    REQUIRED_VARS LibUSB1_LIBRARY LibUSB1_INCLUDE_DIR
    VERSION_VAR   LibUSB1_VERSION
)
mark_as_advanced(LibUSB1_INCLUDE_DIR LibUSB1_LIBRARY)

if(LibUSB1_FOUND AND NOT TARGET LibUSB1::LibUSB1)
    add_library(LibUSB1::LibUSB1 UNKNOWN IMPORTED)
    set_target_properties(LibUSB1::LibUSB1 PROPERTIES
        IMPORTED_LOCATION             "${LibUSB1_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${LibUSB1_INCLUDE_DIR}"
    )
endif()
