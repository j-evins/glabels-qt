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
configure_file (Version.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/Version.hpp @ONLY)
configure_file (Config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/Config.hpp @ONLY)

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
  Handle.cpp
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
  Version.cpp
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
  Model.hpp
  ModelObject.hpp
  ModelBarcodeObject.hpp
  ModelBoxObject.hpp
  ModelEllipseObject.hpp
  ModelImageObject.hpp
  ModelLineObject.hpp
  ModelShapeObject.hpp
  ModelTextObject.hpp
  PageRenderer.hpp
  Settings.hpp
  Variables.hpp
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
  PUBLIC cxx_std_20
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


