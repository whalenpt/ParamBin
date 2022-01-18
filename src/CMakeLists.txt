
cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES
	parambin.cpp
	parambinstrings.cpp
)
set(INCL_DIR "${ParamBin_SOURCE_DIR}/include/ParamBin")

set(INCLUDE_FILES
    ${INCL_DIR}/parambinstrings.h
    ${INCL_DIR}/parambin.hpp
)

add_library(parambin SHARED ${SRC_FILES} ${INCLUDE_FILES})
add_library(ParamBin::parambin ALIAS parambin)
target_include_directories(parambin PUBLIC 
    $<BUILD_INTERFACE:${ParamBin_SOURCE_DIR}/include/ParamBin>
	$<INSTALL_INTERFACE:include>
)

add_library(parambin_static STATIC ${SRC_FILES} ${INCLUDE_FILES})
add_library(ParamBin::parambin_static ALIAS parambin_static)
target_include_directories(parambin_static PUBLIC 
    $<BUILD_INTERFACE:${ParamBin_SOURCE_DIR}/include/ParamBin>
	$<INSTALL_INTERFACE:include>
)


set_target_properties(parambin parambin_static PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
)

install(DIRECTORY ${INCL_DIR}
	DESTINATION include
)

install(TARGETS parambin parambin_static
	EXPORT ParamBinTargets
	LIBRARY DESTINATION lib 
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
)

install(EXPORT ParamBinTargets
	FILE ParamBinTargets.cmake
	NAMESPACE ParamBin::
	DESTINATION lib/cmake/ParamBin
)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

configure_package_config_file( 
    ${ParamBin_SOURCE_DIR}/cmake/ParamBinConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ParamBinConfig.cmake
	INSTALL_DESTINATION lib/cmake/ParamBin
	PATH_VARS CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    ParamBinConfigVersion.cmake
	VERSION ${ParamBin_VERSION}
	COMPATIBILITY AnyNewerVersion
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/ParamBinConfig.cmake
	      ${CMAKE_CURRENT_BINARY_DIR}/ParamBinConfigVersion.cmake
	DESTINATION lib/cmake/ParamBin
)



