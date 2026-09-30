
# Optional user-docs target, requires Sphinx
add_custom_target (user-docs DEPENDS manpages qthelp-manual)

#=======================================
# Rebuild man pages
#=======================================
set (MAN_SOURCES
  man/glabels-qt.rst
  man/glabels-batch-qt.rst
)

foreach (_man_source ${MAN_SOURCES})
  string (REGEX REPLACE "([^.]*)\.rst" "_build/\\1.1" _man_page ${_man_source})
  list (APPEND MAN_PAGES ${_man_page})
endforeach ()

add_custom_command (
  OUTPUT ${MAN_PAGES}
  COMMAND ${SPHINX_EXECUTABLE} -M man ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/_build
  DEPENDS ${MAN_SOURCES} 
  COMMENT "Generating man pages."
)

add_custom_target (manpages DEPENDS ${MAN_PAGES})

#=======================================
# Rebuild qthelp user manual
#=======================================
set (QTHELP_FILES
  _build/qthelp/gLabels.qch
  _build/qthelp/gLabels.qhc
)

find_program (QHELPGENERATOR_EXECUTABLE
  NAMES qhelpgenerator
  PATHS /usr/lib/qt6/libexec
  NO_DEFAULT_PATH
)

add_custom_target (qthelp-manual
  ${SPHINX_EXECUTABLE} -M qthelp ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/_build
  COMMAND ${QHELPGENERATOR_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/_build/qthelp/gLabels.qhcp
  BYPRODUCTS ${QTHELP_FILES}
  COMMENT "Generating qthelp documentation."
)

#=======================================
# Rebuild simple HTML user manual
#=======================================
add_custom_target (html-manual
  ${SPHINX_EXECUTABLE} -M html ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/_build
  COMMENT "Generating HTML documentation."
)

#=======================================
# Install
#=======================================
install (FILES ${MAN_PAGES} DESTINATION share/man/man1)
install (FILES ${QTHELP_FILES} DESTINATION share/glabels-qt/doc)
