cmake_minimum_required(VERSION 3.14)
project(LightBringer C CXX)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O4)

set(GL_LIB opengl32.lib)
set(GLFW3_LIB ${PROJECT_SOURCE_DIR}/lib/glfw3.lib)
configure_file(assets/teapot.obj assets/teapot.obj COPYONLY)
configure_file(assets/uv.jpg assets/uv.jpg COPYONLY)

aux_source_directory(src SOURCES)

add_executable(LightBringer ${SOURCES})
target_include_directories(LightBringer
    PUBLIC 
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(LightBringer
    ${GL_LIB}
    ${GLFW3_LIB}
)