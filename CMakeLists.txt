cmake_minimum_required (VERSION 3.22)

###############################################################################
# gLabels Label Designer Project
###############################################################################
project (glabels-qt VERSION "3.99.0")

#
# Path for locally defined cmake modules
#
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


#=======================================
# Version Information
#=======================================
set (WEBSITE  "glabels.org")
set (BUG_WEBSITE  "https://github.com/j-evins/glabels-qt/issues")

execute_process(
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND git symbolic-ref --short HEAD
  RESULT_VARIABLE BRANCH_VALID
  OUTPUT_VARIABLE BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (NOT ${BRANCH_VALID} STREQUAL "0")
  set (BRANCH "Unkonwn")
endif ()
  
execute_process(
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND git rev-list --count ${BRANCH}
  RESULT_VARIABLE COMMIT_COUNT_VALID
  OUTPUT_VARIABLE COMMIT_COUNT
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (NOT ${COMMIT_COUNT_VALID} STREQUAL "0")
  set (COMMIT_COUNT "?")
endif ()
  
execute_process(
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND git log -1 --format=%h
  RESULT_VARIABLE COMMIT_HASH_VALID
  OUTPUT_VARIABLE COMMIT_HASH
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (NOT ${COMMIT_HASH_VALID} STREQUAL "0")
  set (COMMIT_HASH "exported")
endif ()
  
execute_process(
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND git log -1 --format=%cd --date=short
  RESULT_VARIABLE COMMIT_DATE_VALID
  OUTPUT_VARIABLE COMMIT_DATE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
if (NOT ${COMMIT_DATE_VALID} STREQUAL "0")
  string (TIMESTAMP COMMIT_DATE %Y-%m-%d)
endif ()

# Uncomment for snapshots, comment for releases
set(VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}-${BRANCH}${COMMIT_COUNT}")

# Uncomment for releases, comment for snapshots
#set(VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

set(LONG_VERSION_STRING "${VERSION_STRING} (${COMMIT_HASH} ${COMMIT_DATE})")

# Auto-generate version file
configure_file (VERSION.txt.in VERSION.txt @ONLY)


#=======================================
# Packaging Information
#=======================================
set (CPACK_PACKAGE_VENDOR ${WEBSITE})

set (CPACK_PACKAGE_NAME glabels)

set (CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set (CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set (CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set (CPACK_PACKAGE_VERSION_TWEAK ${COMMIT_HASH})

set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "gLabels Label Designer")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set (CPACK_PACKAGE_EXECUTABLES "glabels-qt;gLabels")
set (CPACK_CREATE_DESKTOP_LINKS "glabels-qt")

set (CPACK_NSIS_DISPLAY_NAME "gLabels")
set (CPACK_NSIS_URL_INFO_ABOUT "http://${WEBSITE}")
set (CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/glabels/windows-icon/glabels.ico")
set (CPACK_NSIS_MODIFY_PATH "ON")

include (CPack)


#=======================================
# Dependencies
#=======================================
if (MINGW)
   # Locate MINGW directories
   string (REPLACE "/g++.exe" "" MINGW_BIN_DIR  ${CMAKE_CXX_COMPILER})
   string (REPLACE "/bin"     "" MINGW_BASE_DIR ${MINGW_BIN_DIR})

   # Make sure we can find MINGW libraries, e.g. zlib
   set (CMAKE_PREFIX_PATH ${MINGW_BASE_DIR} )
endif ()

find_package (Qt6 6.2 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Xml Svg LinguistTools Test)

if (WIN32)
   # Locate Qt directories
   execute_process (COMMAND qtpaths --install-prefix OUTPUT_VARIABLE QT_BASE_DIR OUTPUT_STRIP_TRAILING_WHITESPACE)
   set (QT_BIN_DIR ${QT_BASE_DIR}/bin)
endif ()

#
# Optional dependencies
#
find_package (ZLIB 1.2 QUIET)
find_package (GnuBarcode 0.98 QUIET)
find_package (LibQrencode 3.4 QUIET)
find_package (LibZint 2.15 QUIET)
# Unit testing support
find_package (Qt5Test 5.6 QUIET)


#=======================================
# Global compiler options
#=======================================
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
   #
   # Uncomment to compile everything with aggressively pedantic options
   # (not recommended -- only for testing -- also not portable)
   #
   #add_compile_options("-Wall" "-Wextra" "-Wpedantic" "-Wno-unused-parameter" "-Werror")

   # Uncomment to always compile with debug symbols
   add_compile_options("-g")
endif ()


#=======================================
# Unit Testing
#=======================================
if (Qt6Test_FOUND)
  enable_testing ()
endif ()

#=======================================
# Subdirectories
#=======================================
add_subdirectory (glbarcode)
add_subdirectory (backends)
add_subdirectory (model)
add_subdirectory (glabels)
add_subdirectory (glabels-batch)
add_subdirectory (templates)
add_subdirectory (user-docs)
add_subdirectory (translations)
add_subdirectory (data)


#=======================================
# Print configuration information
#=======================================
message (STATUS "")
message (STATUS "Project name ............ " ${CMAKE_PROJECT_NAME})
message (STATUS "Project version ......... " ${LONG_VERSION_STRING})
message (STATUS "Host system ............. " ${CMAKE_HOST_SYSTEM} " " ${CMAKE_HOST_SYSTEM_PROCESSOR})
message (STATUS "Target system ........... " ${CMAKE_SYSTEM} " " ${CMAKE_SYSTEM_PROCESSOR})
message (STATUS "Installation prefix ..... " ${CMAKE_INSTALL_PREFIX})
message (STATUS "Source code location .... " ${glabels_SOURCE_DIR})
message (STATUS "CMake version ........... " ${CMAKE_VERSION})
message (STATUS "C++ Compiler ............ " ${CMAKE_CXX_COMPILER_ID} " " ${CMAKE_CXX_COMPILER} " " ${CMAKE_CXX_COMPILER_VERSION})
message (STATUS "Qt version .............. " ${Qt6Core_VERSION})

if (ZLIB_FOUND)
  message (STATUS "zlib (optional).......... " ${ZLIB_VERSION_STRING})
else (ZLIB_FOUND)
  message (STATUS "zlib (optional).......... No.")
endif (ZLIB_FOUND)

if (GNUBARCODE_FOUND)
  message (STATUS "GNU Barcode (optional)... " ${GNUBARCODE_VERSION_STRING})
else (GNUBARCODE_FOUND)
  message (STATUS "GNU Barcode (optional)... No.")
endif (GNUBARCODE_FOUND)

if (LIBQRENCODE_FOUND)
  message (STATUS "qrencode (optional)...... " ${LIBQRENCODE_VERSION_STRING})
else (LIBQRENCODE_FOUND)
  message (STATUS "qrencode (optional)...... No.")
endif (LIBQRENCODE_FOUND)

if (LIBZINT_FOUND)
  message (STATUS "libzint (optional)....... " ${LIBZINT_VERSION_STRING})
else (LIBZINT_FOUND)
  message (STATUS "libzint (optional)....... No.")
endif (LIBZINT_FOUND)

if (MSVC)
   message (STATUS "MSVC Qt location ........ " ${QT_BASE_DIR})
endif (MSVC)

if (MINGW)
   message (STATUS "MinGW location .......... " ${MINGW_BASE_DIR})
   message (STATUS "MinGW Qt location ....... " ${QT_BASE_DIR})
endif (MINGW)

message (STATUS "")

#
# Uncomment to print all CMake variables
#
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#   message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
  
