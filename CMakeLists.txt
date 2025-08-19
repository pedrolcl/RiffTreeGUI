# Copyright (C) 2025 Pedro López-Cabanillas
# SPDX-License-Identifier:  GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

project(RiffTree
    LANGUAGES CXX
    VERSION 1.0
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

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
    qhexview/model/buffer/qdevicebuffer.cpp
    qhexview/model/buffer/qdevicebuffer.h
    qhexview/model/buffer/qhexbuffer.cpp
    qhexview/model/buffer/qhexbuffer.h
    qhexview/model/buffer/qmappedfilebuffer.cpp
    qhexview/model/buffer/qmappedfilebuffer.h
    qhexview/model/buffer/qmemorybuffer.cpp
    qhexview/model/buffer/qmemorybuffer.h
    qhexview/model/buffer/qmemoryrefbuffer.cpp
    qhexview/model/buffer/qmemoryrefbuffer.h
    qhexview/model/commands/hexviewcommand.cpp
    qhexview/model/commands/hexviewcommand.h
    qhexview/model/commands/insertcommand.cpp
    qhexview/model/commands/insertcommand.h
    qhexview/model/commands/removecommand.cpp
    qhexview/model/commands/removecommand.h
    qhexview/model/commands/replacecommand.cpp
    qhexview/model/commands/replacecommand.h
    qhexview/model/qhexchanges.h
    qhexview/model/qhexcursor.cpp
    qhexview/model/qhexcursor.h
    qhexview/model/qhexdelegate.cpp
    qhexview/model/qhexdelegate.h
    qhexview/model/qhexdocument.cpp
    qhexview/model/qhexdocument.h
    qhexview/model/qhexmetadata.cpp
    qhexview/model/qhexmetadata.h
    qhexview/model/qhexoptions.h
    qhexview/model/qhexutils.cpp
    qhexview/model/qhexutils.h
    qhexview/qhexview.cpp
    qhexview/qhexview.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

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
