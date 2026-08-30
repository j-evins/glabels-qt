if (Qt6Test_FOUND)

  #=======================================
  # Test VCardSimpleParser class
  #=======================================
  qt6_wrap_cpp (TestVCardSimpleParser_moc_sources TestVCardSimpleParser.hpp)
  add_executable (TestVCardSimpleParser TestVCardSimpleParser.cpp ${TestVCardSimpleParser_moc_sources})
  target_link_libraries (TestVCardSimpleParser Model Qt6::Test)
  add_test (NAME VCardSimpleParser COMMAND TestVCardSimpleParser)

endif (Qt6Test_FOUND)
