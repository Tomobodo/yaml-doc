add_library(${PROJECT_NAME})

add_subdirectory(exceptions)

target_sources(
  ${PROJECT_NAME} PRIVATE
  Doc.cpp
)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  ${YAML_DOC_DIR}/include/yaml-doc
  ${YAML_DOC_DIR}/libs/libyaml/include
)

target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  yaml
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
)

