# Copyright (c) 2026 Pedro López-Cabanillas
function(CREATE_APPIMAGE)
    set(args "EXE;ICON;DESKTOP;WORKDIR;APPDIR;ARCH")
    cmake_parse_arguments(ARGS "" "${args}" "" ${ARGN})

    set(_archs "aarch64;armhf;i686;x86_64")
    if (NOT ${ARGS_ARCH} IN_LIST _archs)
        message(FATAL_ERROR "not supported architecture: ${ARGS_ARCH}")
    endif()

    if (NOT IS_DIRECTORY ${ARGS_WORKDIR})
        message(FATAL_ERROR "invalid working directory: ${ARGS_WORKDIR}")
    endif()

    cmake_path(SET _appdir NORMALIZE "${ARGS_WORKDIR}/${ARGS_APPDIR}")
    if (NOT IS_DIRECTORY ${_appdir})
        message(FATAL_ERROR "invalid AppDir: ${ARGS_APPDIR}")
    endif()

    set(_exe "usr/bin/${ARGS_EXE}")
    if (NOT IS_EXECUTABLE ${_appdir}/${_exe})
        message(FATAL_ERROR "invalid executable: ${ARGS_EXE}")
    endif()

    set(_icon "usr/share/icons/hicolor/256x256/apps/${ARGS_ICON}")
    if (NOT IS_READABLE ${_appdir}/${_icon})
        message(FATAL_ERROR "icon file not readable: ${ARGS_ICON}")
    endif()

    set(_desktop "usr/share/applications/${ARGS_DESKTOP}")
    if (NOT IS_READABLE ${_appdir}/${_desktop})
        message(FATAL_ERROR ".desktop file not readable: ${ARGS_DESKTOP}")
    endif()

     # download AppImageTool if needed
    set(_ait_path "${ARGS_WORKDIR}/appimagetool-${ARGS_ARCH}.AppImage")
    if (NOT IS_EXECUTABLE "${_ait_path}")
        set(_expected_hash)
        if (${ARGS_ARCH} STREQUAL "aarch64")
            set(_expected_hash "f0837e7448a0c1e4e650a93bb3e85802546e60654ef287576f46c71c126a9158")
        elseif (${ARGS_ARCH} STREQUAL "armhf")
            set(_expected_hash "42b61cba5495d8aaf418a5c9a015a49b85ad92efabcbd3c341f1540440e4e23d")
        elseif (${ARGS_ARCH} STREQUAL "i686")
            set(_expected_hash "7ad9ff47c203aae0149b18f6df9e3018b2e2f470ea644a0413e3ded39e9e3bdb")
        elseif (${ARGS_ARCH} STREQUAL "x86_64")
            set(_expected_hash "ed4ce84f0d9caff66f50bcca6ff6f35aae54ce8135408b3fa33abfc3cb384eb0")
        endif()
        set(_ait_url "https://github.com/AppImage/appimagetool/releases/download/1.9.1/appimagetool-${ARGS_ARCH}.AppImage")
        message(STATUS "downloading AppImageTool from github")
        file(DOWNLOAD ${_ait_url} "${_ait_path}" EXPECTED_HASH SHA256=${_expected_hash})
        file(CHMOD "${_ait_path}" FILE_PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    endif()

    # AppImage name
    string(REPLACE ".AppDir" ".AppImage" _output_name "${_appdir}")

    # create the symlink that will launch the App
    file(CREATE_LINK "${_exe}" "${_appdir}/AppRun" SYMBOLIC)
    
    # icon
    file(CREATE_LINK "${_icon}" "${_appdir}/${ARGS_ICON}" SYMBOLIC)

    # symlink the .desktop file
    file(CREATE_LINK "${_desktop}" "${_appdir}/${ARGS_DESKTOP}" SYMBOLIC)

    # Invoke AppImageTool
    execute_process(COMMAND ${_ait_path} -n ${_appdir} ${_output_name})
endfunction()
