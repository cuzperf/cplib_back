file(GLOB_RECURSE FORMAT_HEADER_FILES "${CMAKE_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE FORMAT_HEADER_FILES "${CMAKE_SOURCE_DIR}/tests/*.h")

file(GLOB_RECURSE FORMAT_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE FORMAT_SOURCE_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

if (CMAKE_VERBOSE_MAKEFILE)
  message("Formatted Header Files:")
  foreach(HEADER ${FORMAT_HEADER_FILES})
    message(STATUS "  ${HEADER}")
  endforeach()

  message("Formatted Source Files:")
  foreach(SOURCE ${FORMAT_SOURCE_FILES})
    message(STATUS "  ${SOURCE}")
  endforeach()
endif()

add_custom_target(
    clang-format
    COMMAND clang-format
    -i
    ${FORMAT_HEADER_FILES}
    ${FORMAT_SOURCE_FILES}
)
