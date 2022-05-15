project(Engine)

option(USE_SOLUTION_FOLDERS  ON)
option(GROUP_BY_EXPLORER ON)
set_target_properties(uninstall PROPERTIES FOLDER "CMakePredefinedTargets")
set(EXTRA_LIBS opengl32 glfw glm::glm imgui stb json11)

file(GLOB_RECURSE HEADER_FILES *.h)
aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_FILES)

add_library(Engine ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries(Engine
    ${EXTRA_LIBS}
)

configure_file(Predefined.h.in ${PROJECT_SOURCE_DIR}/Predefined.h)
