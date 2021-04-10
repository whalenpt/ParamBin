
cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

set(THIS parambin)

set(SRC_FILES
	src/parambin.cpp
)
set(INCLUDE_FILES
	include/parambin.hpp
)

add_library(${THIS} SHARED ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(${THIS} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/Utils/include
)
target_link_libraries(${THIS} pwutils)

include(GNUInstallDirs)
set_target_properties(${THIS} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
	PUBLIC_HEADER "${INCLUDE_FILES}"
)

set(THIS_STATIC parambin_static)
add_library(${THIS_STATIC} STATIC ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(${THIS_STATIC} PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/Utils/include
)

target_link_libraries(${THIS_STATIC} pwutils_static)

set_target_properties(${THIS_STATIC} PROPERTIES
	VERSION ${PROJECT_VERSION}
	SOVERSION 0
	PUBLIC_HEADER "${INCLUDE_FILES}"
)

install(TARGETS ${THIS_STATIC}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)


