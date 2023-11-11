project(${PROJECT_NAME}-tests)

add_executable(${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME} PRIVATE
  main.cpp
)

if (NOT TARGET doctest)
  add_subdirectory(${CMAKE_SOURCE_DIR}/libs/doctest doctest)
endif()

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  yaml-doc
  doctest
)
