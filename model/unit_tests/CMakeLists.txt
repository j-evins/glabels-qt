if (Qt6Test_FOUND)

  #=======================================
  # Test SubstitutionField class
  #=======================================
  qt6_wrap_cpp (TestSubstitutionField_moc_sources TestSubstitutionField.h)
  add_executable (TestSubstitutionField TestSubstitutionField.cpp ${TestSubstitutionField_moc_sources})
  target_link_libraries (TestSubstitutionField Model Qt6::Test)
  add_test (NAME SubstitutionField COMMAND TestSubstitutionField)

  #=======================================
  # Test XmlUtil class
  #=======================================
  qt6_wrap_cpp (TestXmlUtil_moc_sources TestXmlUtil.h)
  add_executable (TestXmlUtil TestXmlUtil.cpp ${TestXmlUtil_moc_sources})
  target_link_libraries (TestXmlUtil Model Qt6::Test)
  add_test (NAME XmlUtil COMMAND TestXmlUtil)

  #=======================================
  # Test XmlLabelCreator/Parser classes
  #=======================================
  qt6_wrap_cpp (TestXmlLabel_moc_sources TestXmlLabel.h)
  add_executable (TestXmlLabel TestXmlLabel.cpp ${TestXmlLabel_moc_sources})
  target_link_libraries (TestXmlLabel Model Qt6::Test)
  add_test (NAME XmlLabel COMMAND TestXmlLabel)

  #=======================================
  # Test ColorNode class
  #=======================================
  qt6_wrap_cpp (TestColorNode_moc_sources TestColorNode.h)
  add_executable (TestColorNode TestColorNode.cpp ${TestColorNode_moc_sources})
  target_link_libraries (TestColorNode Model Qt6::Test)
  add_test (NAME ColorNode COMMAND TestColorNode)

  #=======================================
  # Test FileUtil class
  #=======================================
  qt6_wrap_cpp (TestFileUtil_moc_sources TestFileUtil.h)
  add_executable (TestFileUtil TestFileUtil.cpp ${TestFileUtil_moc_sources})
  target_link_libraries (TestFileUtil Model Qt6::Test)
  add_test (NAME FileUtil COMMAND TestFileUtil)

  #=======================================
  # Test Merge classes
  #=======================================
  qt6_wrap_cpp (TestMerge_moc_sources TestMerge.h)
  add_executable (TestMerge TestMerge.cpp ${TestMerge_moc_sources})
  target_link_libraries (TestMerge Model Qt6::Test)
  add_test (NAME Merge COMMAND TestMerge)

  #=======================================
  # Test Model class
  #=======================================
  qt6_wrap_cpp (TestModel_moc_sources TestModel.h)
  add_executable (TestModel TestModel.cpp ${TestModel_moc_sources})
  target_link_libraries (TestModel Model Qt6::Test)
  add_test (NAME Model COMMAND TestModel)

  #=======================================
  # Test ModelImageObject class
  #=======================================
  qt6_wrap_cpp (TestModelImageObject_moc_sources TestModelImageObject.h)
  add_executable (TestModelImageObject TestModelImageObject.cpp ${TestModelImageObject_moc_sources})
  target_link_libraries (TestModelImageObject Model Qt6::Test)
  add_test (NAME ModelImageObject COMMAND TestModelImageObject)

  #=======================================
  # Test RawText class
  #=======================================
  qt6_wrap_cpp (TestRawText_moc_sources TestRawText.h)
  add_executable (TestRawText TestRawText.cpp ${TestRawText_moc_sources})
  target_link_libraries (TestRawText Model Qt6::Test)
  add_test (NAME RawText COMMAND TestRawText)

  #=======================================
  # Test TextNode class
  #=======================================
  qt6_wrap_cpp (TestTextNode_moc_sources TestTextNode.h)
  add_executable (TestTextNode TestTextNode.cpp ${TestTextNode_moc_sources})
  target_link_libraries (TestTextNode Model Qt6::Test)
  add_test (NAME TextNode COMMAND TestTextNode)

  #=======================================
  # Test Variable class
  #=======================================
  qt6_wrap_cpp (TestVariable_moc_sources TestVariable.h)
  add_executable (TestVariable TestVariable.cpp ${TestVariable_moc_sources})
  target_link_libraries (TestVariable Model Qt6::Test)
  add_test (NAME Variable COMMAND TestVariable)

  #=======================================
  # Test Variables class
  #=======================================
  qt6_wrap_cpp (TestVariables_moc_sources TestVariables.h)
  add_executable (TestVariables TestVariables.cpp ${TestVariables_moc_sources})
  target_link_libraries (TestVariables Model Qt6::Test)
  add_test (NAME Variables COMMAND TestVariables)

endif (Qt6Test_FOUND)
