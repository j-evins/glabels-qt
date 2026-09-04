if (Qt6Test_FOUND)

  #=======================================
  # Test RawVCard class
  #=======================================
  qt6_wrap_cpp (TestRawVCard_moc_sources TestRawVCard.hpp)
  add_executable (TestRawVCard TestRawVCard.cpp ${TestRawVCard_moc_sources})
  target_link_libraries (TestRawVCard Model Qt6::Test)
  add_test (NAME RawVCard COMMAND TestRawVCard)

  #=======================================
  # Test VCardContact class
  #=======================================
  qt6_wrap_cpp (TestVCardContact_moc_sources TestVCardContact.hpp)
  add_executable (TestVCardContact TestVCardContact.cpp ${TestVCardContact_moc_sources})
  target_link_libraries (TestVCardContact Model Qt6::Test)
  add_test (NAME VCardContact COMMAND TestVCardContact)

endif (Qt6Test_FOUND)
