# Copyright (C) 2025-2026 Pedro López-Cabanillas
# SPDX-License-Identifier:  GPL-3.0-or-later

cmake_minimum_required(VERSION 3.19)

project(RiffTreeGUI
    LANGUAGES CXX
    VERSION 1.0.1
    DESCRIPTION "A GUI application to show the tree structure of a Riff file with hex view"
    HOMEPAGE_URL "https://pedrolcl.github.io/RiffTreeGUI/"
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets LinguistTools)

set(TS_FILES
    translations/${PROJECT_NAME}_en.ts
    translations/${PROJECT_NAME}_es.ts
)

if (QT_VERSION VERSION_LESS 5.15.0)
    qt5_add_translation(QM_FILES ${TS_FILES})
else()
    qt_add_translation(QM_FILES ${TS_FILES})
endif()

include(TranslationUtils)
add_app_translations_resource(APP_RES ${QM_FILES})
add_qt_translations_resource(QT_RES en es)

add_executable(${PROJECT_NAME}
    aboutdialog.cpp
    aboutdialog.h
    main.cpp
    mainwindow.cpp
    mainwindow.h
    recentfileshelper.cpp
    recentfileshelper.h
    resources.qrc
    treeitem.cpp
    treeitem.h
    treemodel.cpp
    treemodel.h
# rifftree: https://github.com/jesustorresdev/rifftree (Apache 2.0 license)
    riff.h
# QHexView: https://github.com/Dax89/QHexView (MIT license)
    qhexview/include/QHexView/dialogs/hexfinddialog.h
    qhexview/include/QHexView/model/buffer/qdevicebuffer.h
    qhexview/include/QHexView/model/buffer/qhexbuffer.h
    qhexview/include/QHexView/model/buffer/qmappedfilebuffer.h
    qhexview/include/QHexView/model/buffer/qmemorybuffer.h
    qhexview/include/QHexView/model/buffer/qmemoryrefbuffer.h
    qhexview/include/QHexView/model/commands/hexviewcommand.h
    qhexview/include/QHexView/model/commands/insertcommand.h
    qhexview/include/QHexView/model/commands/removecommand.h
    qhexview/include/QHexView/model/commands/replacecommand.h
    qhexview/include/QHexView/model/qhexchanges.h
    qhexview/include/QHexView/model/qhexcursor.h
    qhexview/include/QHexView/model/qhexdelegate.h
    qhexview/include/QHexView/model/qhexdocument.h
    qhexview/include/QHexView/model/qhexmetadata.h
    qhexview/include/QHexView/model/qhexoptions.h
    qhexview/include/QHexView/model/qhexutils.h
    qhexview/include/QHexView/qhexview.h
    qhexview/src/dialogs/hexfinddialog.cpp
    qhexview/src/model/buffer/qdevicebuffer.cpp
    qhexview/src/model/buffer/qhexbuffer.cpp
    qhexview/src/model/buffer/qmappedfilebuffer.cpp
    qhexview/src/model/buffer/qmemorybuffer.cpp
    qhexview/src/model/buffer/qmemoryrefbuffer.cpp
    qhexview/src/model/commands/hexviewcommand.cpp
    qhexview/src/model/commands/insertcommand.cpp
    qhexview/src/model/commands/removecommand.cpp
    qhexview/src/model/commands/replacecommand.cpp
    qhexview/src/model/qhexcursor.cpp
    qhexview/src/model/qhexdelegate.cpp
    qhexview/src/model/qhexdocument.cpp
    qhexview/src/model/qhexmetadata.cpp
    qhexview/src/model/qhexutils.cpp
    qhexview/src/qhexview.cpp
# embedded translations
    ${APP_RES}
    ${QT_RES}
)

if (WIN32)
    configure_file(${PROJECT_NAME}.rc.in ${PROJECT_NAME}.rc @ONLY)
    target_sources ( ${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.rc
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE )
endif()

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "RiffTree.icns")
    set(APP_ICON_PATH "${CMAKE_CURRENT_SOURCE_DIR}/images/RiffTree.icns")
    set_source_files_properties( ${APP_ICON_PATH} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources" )
    target_sources( ${PROJECT_NAME} PUBLIC ${APP_ICON_PATH} )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_ICON_FILE ${MACOSX_BUNDLE_ICON_FILE}
        MACOSX_BUNDLE_GUI_IDENTIFIER "io.github.pedrolcl.RiffTreeGUI"
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_COPYRIGHT "Copyright © 2025-2026 Pedro López-Cabanillas"
        MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_compile_definitions( ${PROJECT_NAME} PRIVATE
    VERSION=${PROJECT_VERSION}
    PROGRAM=${PROJECT_NAME}
    QHEXVIEW_ENABLE_DIALOGS
)

target_include_directories( ${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/qhexview/include
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

if (UNIX AND NOT APPLE)
    install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/images/RiffTree.png
             DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/256x256/apps )
    install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.desktop
             DESTINATION ${CMAKE_INSTALL_DATADIR}/applications )
    install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.metainfo.xml
             DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo
             RENAME io.github.pedrolcl.${PROJECT_NAME}.appdata.xml )
endif()

if (QT_VERSION VERSION_GREATER_EQUAL 6.3)
    if(MSVC)
        qt_generate_deploy_app_script(
            TARGET ${PROJECT_NAME}
            OUTPUT_SCRIPT deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR
            NO_COMPILER_RUNTIME
            NO_TRANSLATIONS
        )
        include ( InstallRequiredSystemLibraries )
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        qt_generate_deploy_app_script(
            TARGET ${PROJECT_NAME}
            OUTPUT_SCRIPT deploy_script
            INCLUDE_PLUGINS xcb wayland
            NO_UNSUPPORTED_PLATFORM_ERROR
            NO_TRANSLATIONS
        )
    else()
        qt_generate_deploy_app_script(
            TARGET ${PROJECT_NAME}
            OUTPUT_SCRIPT deploy_script
            NO_UNSUPPORTED_PLATFORM_ERROR
            NO_TRANSLATIONS
        )
    endif()
    if (DEFINED deploy_script)
        install(SCRIPT ${deploy_script})
    endif()
    if (UNIX AND NOT APPLE AND DEFINED WORKDIR)
        # AppImage
        install( CODE
          "include(${CMAKE_SOURCE_DIR}/cmake/CreateAppImage.cmake)
           create_appimage(
                EXE \"${PROJECT_NAME}\"
                ICON \"RiffTree.png\"
                DESKTOP \"${PROJECT_NAME}.desktop\"
                WORKDIR \"${WORKDIR}\"
                APPDIR \"${PROJECT_NAME}-v${PROJECT_VERSION}-${CMAKE_SYSTEM_PROCESSOR}.AppDir\"
                ARCH \"${CMAKE_SYSTEM_PROCESSOR}\"
           )"
        )
    endif()
endif()

if (DEFINED ENV{GITHUB_OUTPUT})
    file(APPEND
        "$ENV{GITHUB_OUTPUT}"
        "config_version=${PROJECT_VERSION}\n"
    )
endif()

# CPack support
set ( CPACK_SYSTEM_NAME ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR} )
set ( CPACK_PACKAGE_DESCRIPTION_SUMMARY "RiffTree GUI" )
set ( CPACK_PACKAGE_VENDOR "https://pedrolcl.github.io/RiffTreeGUI" )
set ( CPACK_PACKAGE_DESCRIPTION_FILE "${PROJECT_SOURCE_DIR}/README.md" )
set ( CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE" )
set ( CPACK_PACKAGE_VERSION "v${PROJECT_VERSION}" )
set ( CPACK_PACKAGE_EXECUTABLES "RiffTreeGUI" "RiffTree GUI Viewer" )

# source packages
set ( CPACK_SOURCE_IGNORE_FILES "/.git/;/.qtcreator/;/build/;.user;~$;${CPACK_SOURCE_IGNORE_FILES}" )
set ( CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${PROJECT_VERSION}" )
set ( CPACK_SOURCE_STRIP_FILES OFF )

# binary packages
set ( CPACK_PACKAGE_NAME ${PROJECT_NAME} )
set ( CPACK_STRIP_FILES ON )

if(APPLE)
    set (CPACK_SYSTEM_NAME "macOS")
    set (CPACK_PRE_BUILD_SCRIPTS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/ThinFrameworks.cmake")
    set (CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${PROJECT_VERSION}-macOS-${CMAKE_SYSTEM_PROCESSOR}")
    set (CPACK_BINARY_ZIP ON)
    set (CPACK_BINARY_TGZ OFF)
    set (CPACK_BINARY_STGZ OFF)
endif()

include ( CPack )
