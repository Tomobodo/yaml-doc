cmake_minimum_required(VERSION 3.20)

project(
  yaml-doc
  VERSION 0.1.0
  LANGUAGES CXX
)

set(YAML_DOC_DIR ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/libyaml)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(example)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_link_options(
    -lstdc++
  )
endif()
