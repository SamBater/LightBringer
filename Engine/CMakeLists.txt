project(Engine)

option(USE_SOLUTION_FOLDERS  ON)
option(GROUP_BY_EXPLORER ON)
set_target_properties(uninstall PROPERTIES FOLDER "CMakePredefinedTargets")
set(EXTRA_LIBS opengl32 glfw glm::glm imgui stb json11 tinyobjloader)

file(GLOB_RECURSE HEADER_FILES *.h)
aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_FILES)

add_library(Engine ${HEADER_FILES} ${SOURCE_FILES})

add_executable(LightBringer main.cpp)

target_link_libraries(Engine
    ${EXTRA_LIBS}
)

target_link_libraries(LightBringer
    Engine
)

set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/assets)
configure_file(Predefined.h.in ${PROJECT_SOURCE_DIR}/Predefined.h)

file(GLOB_RECURSE ASSETS_FILES "assets/*")
add_library(assets INTERFACE ${ASSETS_FILES})
set_target_properties(assets PROPERTIES FOLDER "Engine")

# 复制Assets文件
add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"
)
