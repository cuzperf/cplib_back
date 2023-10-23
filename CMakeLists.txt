cmake_minimum_required(VERSION 3.16)

project(
  cplib
  VERSION 0.0
  DESCRIPTION "C++ Lib for Programming Contest"
  LANGUAGES C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_SOURCE_DIR}/cmake/clang-format.cmake)

# add test
add_subdirectory(3rdparty/googletest)
add_subdirectory(src)
add_subdirectory(tests)
