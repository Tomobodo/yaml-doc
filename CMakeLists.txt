cmake_minimum_required(VERSION 3.20)

project(
  yaml-doc
  VERSION 0.1.0
  LANGUAGES CXX
)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(example)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_link_options(
    -lstdc++
  )
endif()
