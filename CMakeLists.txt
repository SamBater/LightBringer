cmake_minimum_required(VERSION 3.14)
project(LightBringer C CXX)
set(CMAKE_CXX_STANDARD 14)

add_subdirectory(glm)

LINK_DIRECTORIES(lib)
set(EXTRA_LIBS opengl32 glfw3dll)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)

add_executable(LightBringer ${SOURCES} ${HEADERS} ${HEADERS_CORE} include/Core/Pipeline/Clipper.cpp include/Core/Texture/CubeMap.cpp include/Core/Texture/CubeMap.h include/Core/Shader/SkyBoxShader.cpp include/Core/Shader/SkyBoxShader.h)

configure_file(lib/glfw3.dll ${CMAKE_BINARY_DIR}/glfw3.dll COPYONLY)

target_include_directories(LightBringer
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        glm
)

target_link_libraries(LightBringer
    ${EXTRA_LIBS}
        glm::glm
)

# 复制Assets文件
add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"
)