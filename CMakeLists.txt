cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

set(THIS parambin)
project(${THIS} VERSION 0.0.1 DESCRIPTION "Container for parameters")

set(SRC_FILES
	src/parambin.cpp
	src/siscales.cpp
)

set(INCLUDE_FILES
	include/parambin.hpp
	include/siscales.h
)

add_library(${THIS} SHARED ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(${THIS} PUBLIC include)
target_link_libraries(${THIS} pwutils)

include(GNUInstallDirs)
set_target_properties(${THIS} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
	PUBLIC_HEADER "${INCLUDE_FILES}"
)

install(TARGETS ${THIS}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)



