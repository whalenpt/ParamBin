cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)

project(use_parambin)

message("CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message("CMAKE_INSTALL_LIBDIR: ${CMAKE_INSTALL_LIBDIR}")
message("CMAKE_SYSTEM_PREFIX_PATH: ${CMAKE_SYSTEM_PREFIX_PATH}")

find_library(PARAMBIN_LIB parambin_static)
find_library(PWUTILS_LIB pwutils_static)
if(WIN32)
	set(WIN_INSTALL_PREFIX "C:/Program Files (x86)")
	set(PARAMBIN_LIBDIR "${WIN_INSTALL_PREFIX}/ParamBin/lib")
	message("PARAMBIN_LIBDIR: ${PARAMBIN_LIBDIR}")
	find_library(PARAMBIN_LIB parambin_static PATHS ${PARAMBIN_LIBDIR})
	find_library(PWUTILS_LIB pwutils_static PATHS ${PARAMBIN_LIBDIR})
else()
	find_library(PARAMBIN_LIB parambin_static PATHS ${CMAKE_INSTALL_LIBDIR})
	find_library(PWUTILS_LIB pwutils_static PATHS ${CMAKE_INSTALL_LIBDIR})
endif()

message(STATUS "PARAMBIN_LIB: ${PARAMBIN_LIB}")
message(STATUS "PWUTILS_LIB: ${PWUTILS_LIB}")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE 
	${PARAMBIN_LIB}
	${PWUTILS_LIB}
)
target_include_directories(${PROJECT_NAME} PUBLIC
	/usr/local/include
	/usr/local/include/pwutils
)






