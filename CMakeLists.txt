cmake_minimum_required(VERSION 3.20)

project(
  yaml-doc
  VERSION 0.1.0
  LANGUAGES CXX
)

set(YAML_DOC_DIR ${PROJECT_SOURCE_DIR})
set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(tests EXCLUDE_FROM_ALL)
add_subdirectory(example EXCLUDE_FROM_ALL)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_link_options(
    -lstdc++
)
endif()

set(YAML_DOC_TESTS ${PROJECT_NAME}-tests)
enable_testing()
add_test(
  NAME ${YAML_DOC_TESTS}
  COMMAND ${PROJECT_BINARY_DIR}/tests/${YAML_DOC_TESTS}
  WORKING_DIRECTORY ${YAML_DOC_DIR}
)
