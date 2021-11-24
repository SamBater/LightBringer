cmake_minimum_required(VERSION 3.14)
project(LightBringer C CXX)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-O4)

# 复制Assets文件
configure_file(assets/teapot.obj assets/teapot.obj COPYONLY)
configure_file(assets/monkey.obj assets/monkey.obj COPYONLY)
configure_file(assets/cube.obj assets/cube.obj COPYONLY)
configure_file(assets/uv.jpg assets/uv.jpg COPYONLY)

LINK_DIRECTORIES(lib)
set(EXTRA_LIBS opengl32 glfw3)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCES)


file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/math/*.h)
file(GLOB HEADERS_CORE ${PROJECT_SOURCE_DIR}/include/Core/*/*.h)


source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS})
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${HEADERS_CORE})



add_executable(LightBringer ${SOURCES} ${HEADERS} ${HEADERS_CORE} )
target_include_directories(LightBringer
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(LightBringer
    ${EXTRA_LIBS}
)