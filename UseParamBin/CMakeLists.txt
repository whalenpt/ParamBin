cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 17)

project(use_parambin)

find_library(PARAMBIN_LIB parambin_static PATHS ${CMAKE_INSTALL_LIBDIR})
find_library(PWUTILS_LIB pwutils_static PATHS ${CMAKE_INSTALL_LIBDIR})
message(STATUS "PARAMBIN_LIB: ${PARAMBIN_LIB}")
message(STATUS "PWUTILS_LIB: ${PARAMBIN_LIB}")

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE 
	${PARAMBIN_LIB}
	${PWUTILS_LIB}
)

