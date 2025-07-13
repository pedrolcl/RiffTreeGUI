# Copyright (C) 2025 Pedro López-Cabanillas
# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier:  GPL-3.0-or-later

cmake_minimum_required(VERSION 3.16)

project(RiffTree
    LANGUAGES CXX
    VERSION 1.0
)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets) #Test

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME}
    main.cpp
    riff.h
    treeitem.cpp treeitem.h
    treemodel.cpp treemodel.h
    mainwindow.h mainwindow.cpp
# QHexView: https://github.com/Dax89/QHexView (MIT license)
    qhexview/model/buffer/qdevicebuffer.h
    qhexview/model/buffer/qhexbuffer.h
    qhexview/model/buffer/qmappedfilebuffer.h
    qhexview/model/buffer/qmemorybuffer.h
    qhexview/model/buffer/qmemoryrefbuffer.h
    qhexview/model/commands/hexcommand.h
    qhexview/model/commands/insertcommand.h
    qhexview/model/commands/removecommand.h
    qhexview/model/commands/replacecommand.h
    qhexview/model/commands/replacecommand.h
    qhexview/model/qhexcursor.h
    qhexview/model/qhexdelegate.h
    qhexview/model/qhexdocument.h
    qhexview/model/qhexmetadata.h
    qhexview/model/qhexoptions.h
    qhexview/model/qhexutils.h
    qhexview/qhexview.h
    qhexview/model/commands/hexcommand.cpp
    qhexview/model/commands/insertcommand.cpp
    qhexview/model/commands/removecommand.cpp
    qhexview/model/commands/replacecommand.cpp
    qhexview/model/buffer/qdevicebuffer.cpp
    qhexview/model/buffer/qhexbuffer.cpp
    qhexview/model/buffer/qmemorybuffer.cpp
    qhexview/model/buffer/qmemoryrefbuffer.cpp
    qhexview/model/buffer/qmappedfilebuffer.cpp
    qhexview/model/qhexdelegate.cpp
    qhexview/model/qhexutils.cpp
    qhexview/model/qhexcursor.cpp
    qhexview/model/qhexmetadata.cpp
    qhexview/model/qhexdocument.cpp
    qhexview/qhexview.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets)

install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# include(CTest)

# qt_add_executable(${PROJECT_NAME}_tester
#     test.cpp
#     treeitem.cpp treeitem.h
#     treemodel.cpp treemodel.h)

# target_link_libraries(${PROJECT_NAME}_tester PRIVATE
#     Qt6::Core
#     Qt6::Test
# )

# if(ANDROID)
#     target_link_libraries(${PROJECT_NAME}_tester PRIVATE
#         Qt6::Gui
#     )
# endif()

# qt_add_resources(${PROJECT_NAME}_tester "${PROJECT_NAME}_tester"
#     PREFIX
#         "/"
#     FILES
#         ${${PROJECT_NAME}_resource_files}
# )

# add_test(NAME ${PROJECT_NAME}_tester
#          COMMAND ${PROJECT_NAME}_tester)
