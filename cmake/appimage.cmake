# Copyright (c) 2026 Pedro López-Cabanillas
function(make_appimage)
    set(optional)
    set(args EXE ICON DESKTOP WORKDIR APPDIR PROCESSOR)
    set(list_args ASSETS)
    cmake_parse_arguments(
        PARSE_ARGV 0
        ARGS
        "${optional}"
        "${args}"
        "${list_args}"
    )

    if(${ARGS_UNPARSED_ARGUMENTS})
        message(WARNING "Unparsed arguments: ${ARGS_UNPARSED_ARGUMENTS}")
    endif()

    # download AppImageTool if needed
    SET(AIT_PATH "${ARGS_WORKDIR}/appimagetool-${ARGS_PROCESSOR}.AppImage" CACHE INTERNAL "appimagetool program")
    if (NOT EXISTS "${AIT_PATH}")
        set(AIT_URL "https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-${ARGS_PROCESSOR}.AppImage")
        file(DOWNLOAD ${AIT_URL} "${AIT_PATH}")
        execute_process(COMMAND chmod +x "${AIT_PATH}")
    endif()

    # AppImage name
    string(REPLACE ".AppDir" ".AppImage" OUTPUT_NAME "${ARGS_APPDIR}")

    # copy assets to appdir
    file(COPY ${ARGS_ASSETS} DESTINATION "${ARGS_APPDIR}")

    # create the symlink that will launch the App
    execute_process(COMMAND ln -s "${ARGS_EXE}" "${ARGS_APPDIR}/AppRun")
    
    # icon thumbnail
    execute_process(COMMAND ln -s "${ARGS_ICON}" "${ARGS_APPDIR}/.DirIcon")

    # icon
    execute_process(COMMAND ln -s "${ARGS_ICON}" "${ARGS_APPDIR}/")

    # symlink the .desktop file
    execute_process(COMMAND ln -s "${ARGS_DESKTOP}" "${ARGS_APPDIR}/")

    # Invoke AppImageTool
    execute_process(COMMAND ${AIT_PATH} -n ${ARGS_APPDIR} ${OUTPUT_NAME})
endfunction()
