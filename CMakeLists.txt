cmake_minimum_required(VERSION 3.14)
project(LightBringer C CXX)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O4)

# 复制Assets文件
configure_file(assets/teapot.obj assets/teapot.obj COPYONLY)
configure_file(assets/monkey.obj assets/monkey.obj COPYONLY)
configure_file(assets/cube.obj assets/cube.obj COPYONLY)
configure_file(assets/sphere.obj assets/sphere.obj COPYONLY)
configure_file(assets/uv.jpg assets/uv.jpg COPYONLY)
configure_file(assets/brick_wall.png assets/brick_wall.png COPYONLY)
configure_file(lib/glfw3.dll glfw3.dll COPYONLY)


add_subdirectory(glm)

LINK_DIRECTORIES(lib)
set(EXTRA_LIBS opengl32 glfw3dll)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)

add_executable(LightBringer ${SOURCES} ${HEADERS} ${HEADERS_CORE})

target_include_directories(LightBringer
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        glm
)

target_link_libraries(LightBringer
    ${EXTRA_LIBS}
        glm::glm
)