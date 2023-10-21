cmake_minimum_required(VERSION 3.19.7)  # enable Apple Silicon

project(
  cplib
  VERSION 0.0
  DESCRIPTION "C++ Lib for Programming Contest"
  LANGUAGES C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

# add test
add_subdirectory(tests)

include(${CMAKE_SOURCE_DIR}/cmake/clang-format.cmake)

add_subdirectory("${CMAKE_SOURCE_DIR}/3rdparty/googletest" "3rdparty/googletest")
