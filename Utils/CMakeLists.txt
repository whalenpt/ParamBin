cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

set(SRC_FILES
	src/pwdir.cpp
	src/pwmath.cpp
	src/pwstats.cpp
	src/pwdataio.cpp
	src/pwstrings.cpp
	src/pwthreads.cpp
)

set(INCLUDE_FILES
	include/pwutils/pwdir.h
	include/pwutils/pwexcept.h
	include/pwutils/pwmath.h
	include/pwutils/pwstats.h
	include/pwutils/pwstrings.h
	include/pwutils/pwthreads.h
	include/pwutils/pwconstants.h
	include/pwutils/pwdataio.hpp
)

set(THIS pwutils)
add_library(${THIS} SHARED ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(${THIS} PUBLIC include/pwutils)

install(TARGETS ${THIS} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include/pwutils)

set(THIS_STATIC pwutils_static)
add_library(${THIS_STATIC} STATIC ${SRC_FILES} ${INCLUDE_FILES})
target_include_directories(${THIS_STATIC} PUBLIC include/pwutils)

install(TARGETS ${THIS_STATIC} DESTINATION lib)
install(FILES ${INCLUDE_FILES} DESTINATION include/pwutils)
























