project (glbarcode LANGUAGES CXX)

#=======================================
# Sources
#=======================================
set (GLBARCODE_SOURCES
  Factory.cpp
  Barcode.cpp
  Barcode1dBase.cpp
  Barcode2dBase.cpp
  BarcodeCode39.cpp
  BarcodeCode39Ext.cpp
  BarcodeUpcBase.cpp
  BarcodeUpcA.cpp
  BarcodeEan13.cpp
  BarcodePostnet.cpp
  BarcodePostnet5.cpp
  BarcodePostnet9.cpp
  BarcodePostnet11.cpp
  BarcodeCepnet.cpp
  BarcodeOnecode.cpp
  BarcodeDataMatrix.cpp
  BarcodeQrcode.cpp
  DrawingPrimitives.cpp
  Renderer.cpp
  QtRenderer.cpp
)

#=====================================
# Target
#=====================================
add_library (glbarcode STATIC
  ${GLBARCODE_SOURCES}
)

target_compile_features (glbarcode
  PUBLIC cxx_std_11
)

target_include_directories (glbarcode
  PUBLIC ..
)

target_link_libraries (glbarcode
  Qt6::Widgets
)
