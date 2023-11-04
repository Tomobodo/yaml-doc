project(${PROJECT_NAME}-example)

add_executable(${PROJECT_NAME})

target_sources(
  ${PROJECT_NAME} PRIVATE
  main.cpp
)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/includes
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  yaml-doc
)
