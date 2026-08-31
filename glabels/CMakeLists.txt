project (glabels LANGUAGES CXX)

#=======================================
# Sources
#=======================================
set (glabels_sources
  main.cpp
  AboutDialog.cpp
  AppearanceModel.cpp
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
  MergeTableModel.cpp
  MergeView.cpp
  MiniPreviewPixmap.cpp
  NotebookUtil.cpp
  ObjectEditor.cpp
  PaletteUtil.cpp
  PreferencesDialog.cpp
  PrinterMonitor.cpp
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
  AboutDialog.hpp
  AppearanceModel.hpp
  BarcodeMenu.hpp
  BarcodeMenuButton.hpp
  BarcodeMenuItem.hpp
  ColorButton.hpp
  ColorHistory.hpp
  ColorPaletteDialog.hpp
  ColorPaletteItem.hpp
  EditVariableDialog.hpp
  FieldButton.hpp
  File.hpp
  LabelEditor.hpp
  MainWindow.hpp
  MergeTableModel.hpp
  MergeView.hpp
  ObjectEditor.hpp
  PreferencesDialog.hpp
  PrinterMonitor.hpp
  PrintView.hpp
  PropertiesView.hpp
  Preview.hpp
  ReportBugDialog.hpp
  SelectProductDialog.hpp
  SimplePreview.hpp
  StartupView.hpp
  TemplateDesigner.hpp
  TemplatePicker.hpp
  UndoRedoModel.hpp
  VariablesView.hpp
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
# macOS Resources
#=====================================
if (APPLE)
  # Generate .icns from SVG at build time
  set (ICNS_OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/macos/glabels.icns")
  set (ICNS_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/macos/generate-icns.sh")
  set (ICNS_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/icons/glabels-flat/128x128/apps/glabels.svg")

  add_custom_command (
    OUTPUT "${ICNS_OUTPUT}"
    COMMAND "${ICNS_SCRIPT}"
    DEPENDS "${ICNS_SOURCE}" "${ICNS_SCRIPT}"
    COMMENT "Generating macOS app icon..."
    VERBATIM
  )

  set (glabels_macos_resources
    "${ICNS_OUTPUT}"
  )
  set_source_files_properties (${glabels_macos_resources} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources"
    GENERATED TRUE
  )
endif ()

#=====================================
# Target
#=====================================
add_executable (glabels-qt WIN32 MACOSX_BUNDLE
  ${glabels_sources}
  ${glabels_moc_sources}
  ${glabels_qrc_sources}
  ${glabels_forms_headers}
  ${glabels_win_rc}
  ${glabels_macos_resources}
)

target_compile_features (glabels-qt
  PUBLIC cxx_std_20
)

target_include_directories (glabels-qt
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries (glabels-qt
  Model
  Qt6::Concurrent
  Qt6::Widgets
)

#=======================================
# macOS Bundle Configuration
#=======================================
if (APPLE)
    set_target_properties (glabels-qt PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "gLabels"
        MACOSX_BUNDLE_EXECUTABLE_NAME "glabels-qt"
        MACOSX_BUNDLE_GUI_IDENTIFIER "org.glabels.glabels-qt"
        MACOSX_BUNDLE_BUNDLE_VERSION "${CMAKE_PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}"
        MACOSX_BUNDLE_ICON_FILE "glabels.icns"
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/macos/Info.plist.in"
    )
endif ()

#=======================================
# Install
#=======================================
if (APPLE)
    install (TARGETS glabels-qt BUNDLE DESTINATION .)
else ()
    install (TARGETS glabels-qt RUNTIME DESTINATION bin)
endif ()

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

#
# macOS Runtime
#
if (APPLE)

    # Find macdeployqt
    get_target_property (QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component (QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    find_program (MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${QT_BIN_DIR}")

    # Copy templates into the app bundle Resources folder
    add_custom_command (TARGET glabels-qt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/templates"
            "$<TARGET_BUNDLE_DIR:glabels-qt>/Contents/Resources/templates"
        COMMENT "Copying templates to app bundle..."
    )

    # Copy translations into the app bundle Resources folder
    add_custom_command (TARGET glabels-qt POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_BINARY_DIR}/translations"
            "$<TARGET_BUNDLE_DIR:glabels-qt>/Contents/Resources/translations"
        COMMENT "Copying translations to app bundle..."
    )

    if (MACDEPLOYQT_EXECUTABLE)
        # Run macdeployqt after build to bundle Qt frameworks
        # Note: This creates a self-contained .app bundle
        add_custom_command (TARGET glabels-qt POST_BUILD
            COMMAND "${MACDEPLOYQT_EXECUTABLE}"
                "$<TARGET_BUNDLE_DIR:glabels-qt>"
                -always-overwrite
            COMMENT "Running macdeployqt to bundle Qt frameworks..."
        )

        # Re-sign the app after macdeployqt (required on macOS to fix invalidated signature)
        add_custom_command (TARGET glabels-qt POST_BUILD
            COMMAND codesign --force --deep --sign - "$<TARGET_BUNDLE_DIR:glabels-qt>"
            COMMENT "Re-signing app bundle..."
        )
    else ()
        message (WARNING "macdeployqt not found - Qt frameworks will not be bundled automatically")
    endif ()

endif (APPLE)
