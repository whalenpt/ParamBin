
# Usage #

First install the ParamBin library (see Installation instructions in the top-level README file).
Then, using CMake, in your CMakeLists.txt file all that is needed two lines of commands

1. find_package(ParamBin) # such that your program knows where ParamBin is installed 
2. target_link_library(target_name PRIVATE ParamBin::parambin) # for dynamic linking or
   target_link_library(target_name PRIVATE ParamBin::parambin_static) # for static linking

A simple example of ParamBin usage is in the main.cpp file. To build this example using
cmake, run the following from the command line
```bash
cmake -S . -B build
cd build
make
```
An executable called use_parambin is created in the build directory. 
To run this executable:
```bash
./use_parambin
```





