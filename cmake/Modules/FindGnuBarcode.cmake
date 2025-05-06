# - Try to find the GNU Barcode library
# Once done this will define
#
#  GNUBARCODE_FOUND - System has GNU Barcode
#  GNUBARCODE_INCLUDE_DIRS - The GNU Barcode include directory
#  GNUBARCODE_LIBRARIES - The libraries needed to use GNU Barcode
#  GNUBARCODE_DEFINITIONS - Definitions needed to use GNU Barcode
#  GNUBARCODE_VERSION_STRING - the version of GNU Barcode found

set (GNUBARCODE_DEFINITIONS "")

find_path (GNUBARCODE_INCLUDE_DIR NAMES barcode.h PATH_SUFFIXES barcode)

find_library (GNUBARCODE_LIBRARY NAMES barcode )

if (GNUBARCODE_LIBRARY AND GNUBARCODE_INCLUDE_DIR)
  
  set (GNUBARCODE_INCLUDE_DIRS ${GNUBARCODE_INCLUDE_DIR})
  set (GNUBARCODE_LIBRARIES ${GNUBARCODE_LIBRARY})
  set (GNUBARCODE_DEFINITIONS "")

  if (NOT TARGET GNU::BARCODE)
    
    add_library (GNU::BARCODE UNKNOWN IMPORTED)
    set_target_properties (GNU::BARCODE PROPERTIES
      INTERFACE_COMPILE_DEFINITIONS     "${GNUBARCODE_DEFINITIONS}"
      INTERFACE_INCLUDE_DIRECTORIES     "${GNUBARCODE_INCLUDE_DIRS}"
      IMPORTED_LINK_INTERFACE_LANGUAGES "C"
      IMPORTED_LOCATION                 "${GNUBARCODE_LIBRARY}"
    )
    
  endif ()

endif ()

if (GNUBARCODE_INCLUDE_DIR AND EXISTS "${GNUBARCODE_INCLUDE_DIR}/barcode.h")
  file (STRINGS "${GNUBARCODE_INCLUDE_DIR}/barcode.h" BARCODE_H REGEX "^#define BARCODE_VERSION *\"[^\"]*\"")
  string (REGEX REPLACE "^.*VERSION *\"([^\"]*)\"" "\\1" GNUBARCODE_VERSION_STRING "${BARCODE_H}")
  if (NOT "${BARCODE_H}")
    set(GNUBARCODE_VERSION_STRING "0.99")  # ubuntu v0.99 may have barcode.h, but no version string
  endif()
endif()

# handle the QUIETLY and REQUIRED arguments and set GNUBARCODE_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GnuBarcode
                                  REQUIRED_VARS GNUBARCODE_LIBRARY GNUBARCODE_INCLUDE_DIR
                                  VERSION_VAR GNUBARCODE_VERSION_STRING)

mark_as_advanced(GNUBARCODE_INCLUDE_DIR GNUBARCODE_LIBRARY)
