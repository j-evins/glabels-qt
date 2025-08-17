project (glabels LANGUAGES CXX)

#=======================================
# Sources
#=======================================
set (glabels_sources
  main.cpp
  AboutDialog.cpp
  BarcodeMenu.cpp
  BarcodeMenuButton.cpp
  BarcodeMenuItem.cpp
  ColorButton.cpp
  ColorHistory.cpp
  ColorPaletteDialog.cpp
  ColorPaletteItem.cpp
  ColorSwatch.cpp
  Cursors.cpp
  EditVariableDialog.cpp
  FieldButton.cpp
  File.cpp
  Help.cpp
  LabelEditor.cpp
  MainWindow.cpp
  MergeView.cpp
  MiniPreviewPixmap.cpp
  NotebookUtil.cpp
  ObjectEditor.cpp
  PreferencesDialog.cpp
  PrintView.cpp
  PropertiesView.cpp
  Preview.cpp
  PreviewOverlayItem.cpp
  ReportBugDialog.cpp
  RollTemplatePath.cpp
  SelectProductDialog.cpp
  SimplePreview.cpp
  StartupView.cpp
  TemplateDesigner.cpp
  TemplatePicker.cpp
  TemplatePickerItem.cpp
  UndoRedoModel.cpp
  VariablesView.cpp
)

set (glabels_qobject_headers
  AboutDialog.h
  BarcodeMenu.h
  BarcodeMenuButton.h
  BarcodeMenuItem.h
  ColorButton.h
  ColorHistory.h
  ColorPaletteDialog.h
  ColorPaletteItem.h
  EditVariableDialog.h
  FieldButton.h
  File.h
  LabelEditor.h
  MainWindow.h
  MergeView.h
  ObjectEditor.h
  PreferencesDialog.h
  PrintView.h
  PropertiesView.h
  Preview.h
  ReportBugDialog.h
  SelectProductDialog.h
  SimplePreview.h
  StartupView.h
  TemplateDesigner.h
  TemplatePicker.h
  UndoRedoModel.h
  VariablesView.h
)

set (glabels_forms
  ui/AboutDialog.ui
  ui/EditVariableDialog.ui
  ui/MergeView.ui
  ui/ObjectEditor.ui
  ui/PreferencesDialog.ui
  ui/PrintView.ui
  ui/PropertiesView.ui
  ui/ReportBugDialog.ui
  ui/SelectProductDialog.ui
  ui/StartupView.ui
  ui/TemplateDesignerIntroPage.ui
  ui/TemplateDesignerNamePage.ui
  ui/TemplateDesignerPageSizePage.ui
  ui/TemplateDesignerShapePage.ui
  ui/TemplateDesignerRectPage.ui
  ui/TemplateDesignerRoundPage.ui
  ui/TemplateDesignerEllipsePage.ui
  ui/TemplateDesignerCdPage.ui
  ui/TemplateDesignerPathPage.ui
  ui/TemplateDesignerContinuousPage.ui
  ui/TemplateDesignerNLayoutsPage.ui
  ui/TemplateDesignerOneLayoutPage.ui
  ui/TemplateDesignerTwoLayoutPage.ui
  ui/TemplateDesignerApplyPage.ui
  ui/VariablesView.ui
)

set (glabels_resource_files
  cursors.qrc
  icons.qrc
  images.qrc
)

qt6_wrap_cpp (glabels_moc_sources ${glabels_qobject_headers})
qt6_wrap_ui (glabels_forms_headers ${glabels_forms})
qt6_add_resources (glabels_qrc_sources ${glabels_resource_files})

if (WIN32)
  # Windows resource file
  set (glabels_win_rc glabels.rc)
endif ()

#=====================================
# Target
#=====================================
add_executable (glabels-qt WIN32
  ${glabels_sources}
  ${glabels_moc_sources}
  ${glabels_qrc_sources}
  ${glabels_forms_headers}
  ${glabels_win_rc}
)

target_compile_features (glabels-qt
  PUBLIC cxx_std_11
)

target_include_directories (glabels-qt
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries (glabels-qt
  Model
  Qt6::Widgets
)

#=======================================
# Install
#=======================================
install (TARGETS glabels-qt RUNTIME DESTINATION bin)

install (FILES icons/glabels-flat/scalable/apps/glabels.svg DESTINATION share/icons/hicolor/scalable/apps)
install (FILES icons/glabels-flat/16x16/apps/glabels.svg    DESTINATION share/icons/hicolor/16x16/apps)
install (FILES icons/glabels-flat/22x22/apps/glabels.svg    DESTINATION share/icons/hicolor/22x22/apps)
install (FILES icons/glabels-flat/32x32/apps/glabels.svg    DESTINATION share/icons/hicolor/32x32/apps)
install (FILES icons/glabels-flat/48x48/apps/glabels.svg    DESTINATION share/icons/hicolor/48x48/apps)

install (FILES icons/mimetypes/scalable/x-glabels-project.svg DESTINATION share/icons/hicolor/scalable/mimetypes)
install (FILES icons/mimetypes/16x16/x-glabels-project.svg    DESTINATION share/icons/hicolor/16x16/mimetypes)
install (FILES icons/mimetypes/22x22/x-glabels-project.svg    DESTINATION share/icons/hicolor/22x22/mimetypes)
install (FILES icons/mimetypes/24x24/x-glabels-project.svg    DESTINATION share/icons/hicolor/24x24/mimetypes)

#
# Windows Runtime
#
if (WIN32)

    find_program (WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")

    #
    # Visual Studio toolchain
    #
    if (MSVC)
        # Run windeployqt immediately after build
        add_custom_command (TARGET glabels-qt POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                env PATH="${QT_BIN_DIR}" "${WINDEPLOYQT_EXECUTABLE}"
                    --verbose 0
                    --no-compiler-runtime
                    --no-opengl-sw
                    \"$<TARGET_FILE:glabels-qt>\"
        )

      # Install files staged by windeployqt
        install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ DESTINATION bin
                 FILES_MATCHING PATTERN *.dll PATTERN *.qm)

        # Install necessary system libraries
        set (CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
        include (InstallRequiredSystemLibraries)
    endif (MSVC)

    #
    # MSYS/MINGW toolchain
    #
    if (MINGW)
        # Run windeployqt immediately after build
        add_custom_command (TARGET glabels-qt POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                env PATH="${QT_BIN_DIR}" "${WINDEPLOYQT_EXECUTABLE}"
                    --verbose 0
                    --release
                    --no-compiler-runtime
                    --no-opengl-sw
                    \"$<TARGET_FILE:glabels-qt>\"
        )

        # Install files staged by windeployqt
        install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/ DESTINATION bin
                 FILES_MATCHING PATTERN *.dll PATTERN *.qm)

        # Install necessary system libraries
        install (FILES
            ${QT_BIN_DIR}/libgcc_s_dw2-1.dll
            ${QT_BIN_DIR}/libstdc++-6.dll
            ${QT_BIN_DIR}/libwinpthread-1.dll
            DESTINATION bin
        )
        if (ZLIB_FOUND)
          install (FILES ${MINGW_BIN_DIR}/zlib1.dll DESTINATION bin)
        endif ()
    endif (MINGW)

endif (WIN32)
