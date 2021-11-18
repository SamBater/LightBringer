cmake_minimum_required(VERSION 3.14)
project(OpenGL C CXX)
set(CMAKE_CXX_STANDARD 14)

list(APPEND EXTRA_LIBS
    "-lopengl32")

aux_source_directory(src SOURCES)

add_executable(OpenGL ${SOURCES})
target_include_directories(OpenGL
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
target_link_libraries(OpenGL
    ${EXTRA_LIBS}
${PROJECT_SOURCE_DIR}/lib/libglfw3dll.a
)