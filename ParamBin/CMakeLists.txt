
cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES
	src/parambin.cpp
	src/pwutils/pwstrings.cpp
)
set(INCLUDE_FILES
	include/parambin.hpp
	include/pwutils/pwstrings.h
)

add_library(parambin SHARED ${SRC_FILES} ${INCLUDE_FILES})
add_library(ParamBin::parambin ALIAS parambin)
target_include_directories(parambin PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/pwutils>
	$<INSTALL_INTERFACE:include/pwutils>
)

add_library(parambin_static STATIC ${SRC_FILES} ${INCLUDE_FILES})
add_library(ParamBin::parambin_static ALIAS parambin_static)
target_include_directories(parambin_static PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/pwutils>
	$<INSTALL_INTERFACE:include/pwutils>
)


set_target_properties(parambin parambin_static PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
	PUBLIC_HEADER "${INCLUDE_FILES}"
)

include(GNUInstallDirs)
install(TARGETS parambin parambin_static
	EXPORT ParamBinTargets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BIDDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(EXPORT ParamBinTargets
	FILE ParamBinTargets.cmake
	NAMESPACE ParamBin::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ParamBin
)

include(CMakePackageConfigHelpers)

configure_package_config_file( 
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/ParamBinConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/cmake/ParamBinConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ParamBin
	PATH_VARS CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/cmake/ParamBinConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/ParamBinConfig.cmake
	      ${CMAKE_CURRENT_BINARY_DIR}/cmake/ParamBinConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ParamBin
)



