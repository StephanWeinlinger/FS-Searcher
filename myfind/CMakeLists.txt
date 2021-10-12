# CMakeList.txt : CMake project for myfind, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (myfind "myfind.cpp" "myfind.cpp")

# TODO: Add tests and install targets if needed.

set_property(TARGET "myfind" PROPERTY CXX_STANDARD 17)