# Copyright (C) 2025 Pedro López-Cabanillas
# SPDX-License-Identifier:  GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

project(RiffTree
    LANGUAGES CXX
    VERSION 1.0
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
    ${PROJECT_NAME}_en.ts
    ${PROJECT_NAME}_es.ts
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
    main.cpp
    mainwindow.cpp
    mainwindow.h
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

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

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

if (QT_VERSION VERSION_GREATER_EQUAL 6.0)
    qt_generate_deploy_app_script(
        TARGET ${PROJECT_NAME}
        OUTPUT_SCRIPT deploy_script
        NO_UNSUPPORTED_PLATFORM_ERROR
    )
    install(SCRIPT ${deploy_script})
endif()
