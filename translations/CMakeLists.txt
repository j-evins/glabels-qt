#=======================================
# Translation files
#=======================================
# without this a "make clean" would delete the .ts files
set_directory_properties (PROPERTIES CLEAN_NO_CUSTOM 1)

include (TRANSLATION_FILES.txt)

#
# Update strings in C-locale translation files (lupdate+lrelease)
#
qt6_create_translation (glabels_C_qm_file
  ${CMAKE_SOURCE_DIR}/glabels
  ${CMAKE_SOURCE_DIR}/model
  ${CMAKE_SOURCE_DIR}/backends
  glabels_C.ts
  OPTIONS -no-obsolete -locations none
)
qt6_create_translation (templates_C_qm_file
  ${CMAKE_CURRENT_BINARY_DIR}/template-strings.h
  templates_C.ts
  OPTIONS -no-obsolete -locations none
)

#
# Generate QM files from all other language translation files (lrelease only)
#
qt6_add_translation (language_qm_files
  ${GLABELS_TS_FILES}
  ${TEMPLATES_TS_FILES}
)

add_custom_target (update_translations
  DEPENDS ${glabels_C_qm_file} ${templates_C_qm_file} ${language_qm_files}
)

# Add updating translations as a dependency for glabels-qt
add_dependencies (glabels-qt update_translations)


#=======================================
# XmlStrings utility
#=======================================
add_executable (XmlStrings WIN32
  XmlStrings.cpp
)

target_compile_features (Model
  PUBLIC cxx_std_11
)

target_link_libraries (XmlStrings
  Qt6::Core
  Qt6::Xml
)

#=======================================
# Extract translatable strings from XML
# template files.
#=======================================
# Use absolute locations of XML files
string (REGEX REPLACE "([^;]+)" "${CMAKE_SOURCE_DIR}/templates/\\1"
  xml_files "${template_files};${other_db_files}"
)

add_custom_command (
  OUTPUT template-strings.h
  COMMAND XmlStrings ${xml_files} > template-strings.h
  COMMENT "Extracting template strings."
  DEPENDS XmlStrings ${xml_files}
)

set_source_files_properties (template-strings.h PROPERTIES GENERATED TRUE)

add_custom_target (template-strings DEPENDS template-strings.h)


#=======================================
# Subdirectories
#=======================================


#=======================================
# Install
#=======================================
install (FILES ${glabels_C_qm_file}   DESTINATION share/glabels-qt/translations)
install (FILES ${templates_C_qm_file} DESTINATION share/glabels-qt/translations)
install (FILES ${language_qm_files}   DESTINATION share/glabels-qt/translations)
