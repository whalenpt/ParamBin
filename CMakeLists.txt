cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

project(ParamBin VERSION 0.0.3 DESCRIPTION "Container for parameters")

option(PARAMBIN_TEST "Build ParamBin tests" OFF)

add_subdirectory(src)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    if(PARAMBIN_TEST)
        enable_testing()
        set(INSTALL_GTEST OFF CACHE BOOL "Enable installation of googletest. (Projects embedding googletest may want to turn this OFF.)")
        if(WIN32)
            unset(CMAKE_CXX_STANDARD)
                    add_subdirectory(external/googletest)
            set(CMAKE_CXX_STANDARD 17)
        else()
            add_subdirectory(external/googletest)
        endif()
        add_subdirectory(test)
    endif()
endif()








