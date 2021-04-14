
cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

set(THIS parambin)
set(SRC_FILES
	src/parambin.cpp
	src/pwutils/pwstrings.cpp
)
set(INCLUDE_FILES
	include/parambin.hpp
	include/pwutils/pwstrings.h
)

add_library(${THIS} SHARED ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(${THIS} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include/pwutils
)

set(THIS_STATIC parambin_static)
add_library(${THIS_STATIC} STATIC ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(${THIS_STATIC} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/include/pwutils
)

include(GNUInstallDirs)
set_target_properties(${THIS} ${THIS_STATIC} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
	PUBLIC_HEADER "${INCLUDE_FILES}"
)

install(TARGETS ${THIS} ${THIS_STATIC}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


