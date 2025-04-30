project (Model LANGUAGES CXX)

#=======================================
# Handle optional dependencies
#=======================================
if (${ZLIB_FOUND})
  add_definitions (-DHAVE_ZLIB=1)
  set (OPTIONAL_ZLIB ZLIB::ZLIB)
else ()
  set (OPTIONAL_ZLIB "")
endif ()

#=======================================
# Sources
#=======================================
# Auto-generated files
configure_file (Version.h.in ${CMAKE_CURRENT_BINARY_DIR}/Version.h @ONLY)
configure_file (Config.h.in ${CMAKE_CURRENT_BINARY_DIR}/Config.h @ONLY)

set (Model_sources
  Category.cpp
  ColorNode.cpp
  DataCache.cpp
  Db.cpp
  Distance.cpp
  FileUtil.cpp
  Frame.cpp
  FrameCd.cpp
  FrameContinuous.cpp
  FrameEllipse.cpp
  FramePath.cpp
  FrameRect.cpp
  FrameRound.cpp
  Handles.cpp
  Layout.cpp
  Markup.cpp
  Model.cpp
  ModelObject.cpp
  ModelBarcodeObject.cpp
  ModelBoxObject.cpp
  ModelEllipseObject.cpp
  ModelImageObject.cpp
  ModelLineObject.cpp
  ModelShapeObject.cpp
  ModelTextObject.cpp
  Outline.cpp
  PageRenderer.cpp
  Paper.cpp
  ParserState.cpp
  Point.cpp
  RawText.cpp
  Region.cpp
  Settings.cpp
  Size.cpp
  StrUtil.cpp
  SubstitutionField.cpp
  Template.cpp
  TextNode.cpp
  Units.cpp
  Variable.cpp
  Variables.cpp
  Vendor.cpp
  XmlCategoryParser.cpp
  XmlLabelCreator.cpp
  XmlLabelParser.cpp
  XmlLabelParser_3.cpp
  XmlPaperParser.cpp
  XmlTemplateCreator.cpp
  XmlTemplateParser.cpp
  XmlUtil.cpp
  XmlVendorParser.cpp
)

set (Model_qobject_headers
  Model.h
  ModelObject.h
  ModelBarcodeObject.h
  ModelBoxObject.h
  ModelEllipseObject.h
  ModelImageObject.h
  ModelLineObject.h
  ModelShapeObject.h
  ModelTextObject.h
  PageRenderer.h
  Settings.h
  Variables.h
)

qt6_wrap_cpp (Model_moc_sources ${Model_qobject_headers})

#=====================================
# Target
#=====================================
add_library (Model STATIC
  ${Model_sources}
  ${Model_moc_sources}
)

target_compile_features (Model
  PUBLIC cxx_std_11
)

target_include_directories (Model
  PUBLIC .. ${CMAKE_CURRENT_BINARY_DIR}/..
  PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries (Model
  Barcode
  Merge
  Qt6::Core
  Qt6::PrintSupport
  Qt6::Xml
  Qt6::Svg
  ${OPTIONAL_ZLIB}
)

#=======================================
# Subdirectories
#=======================================
add_subdirectory (unit_tests)


