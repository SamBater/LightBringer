project(imgui)

file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/*.h)
aux_source_directory(${PROJECT_SOURCE_DIR}/ SOURCES)

add_library(imgui
  ${HEADERS}
  ${SOURCES}
)

target_include_directories(imgui
  PUBLIC
  ${PROJECT_SOURCE_DIR}
)

target_link_libraries(imgui
  glfw
)