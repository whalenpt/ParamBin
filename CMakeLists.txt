cmake_minimum_required(VERSION 3.19.1)
set(CMAKE_CXX_STANDARD 17)

set(THIS ParamBin)
project(${THIS} VERSION 0.0.2 DESCRIPTION "Container for parameters")

add_subdirectory(ParamBin)
add_subdirectory(tests)







