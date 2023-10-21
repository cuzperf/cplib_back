add_executable(test_cxx98_for_interface test_cxx98_for_interface.cpp)
set_target_properties(test_cxx98_for_interface
  PROPERTIES
    CXX_STANDARD 98
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
target_include_directories(test_cxx98_for_interface INTERFACE ${CMAKE_SOURCE_DIR}/interface)
