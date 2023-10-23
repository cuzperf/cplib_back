project(test)
enable_testing()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_options(/WX)
endif()

add_subdirectory(single_alone_test)

include_directories("${CMAKE_SOURCE_DIR}/3rdparty/googletest/googletest/include")
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/src")

# unit_test
add_executable(unit_test
  ${CMAKE_SOURCE_DIR}/3rdparty/googletest/googletest/src/gtest_main.cc
  unit_test/math/basic_test.cpp
  unit_test/util.cpp
  # unit_test/*.h
  test_util.h
)
target_link_libraries(unit_test gtest gtest_main cp_math)
set_target_properties(unit_test
  PROPERTIES
    CXX_STANDARD 17
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# smoke_test
