add_library(${PROJECT_NAME})

add_subdirectory(exceptions)

target_sources(
  ${PROJECT_NAME} PRIVATE
  Doc.cpp
)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  ${PROJECT_SOURCE_DIR}/includes/YamlDoc
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  yaml
)
