project (glabels-batch LANGUAGES CXX)

#=======================================
# Sources
#=======================================
set (glabels-batch_sources
  main.cpp
)

#=====================================
# Target
#=====================================
add_executable (glabels-batch-qt
  ${glabels-batch_sources}
)

target_compile_features (glabels-batch-qt
  PUBLIC cxx_std_20
)

target_link_libraries (glabels-batch-qt
  Model
)

#=======================================
# Install
#=======================================
install (TARGETS glabels-batch-qt RUNTIME DESTINATION bin)

