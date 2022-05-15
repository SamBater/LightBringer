cmake_minimum_required(VERSION 3.14)
project(LightBringer C CXX)
set(CMAKE_CXX_STANDARD 14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakePredefinedTargets")

set(OUTPUT_DIR ${PROJECT_SOURCE_DIR}/bin)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY OUTPUT_DIR)
set(ASSETS_DIR ${PROJECT_SOURCE_DIR}/bin/assets)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/bin>)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    add_compile_options("/openmp")
else()
    set(CMAKE_CXX_FLAGS "-fopenmp")
endif()

add_subdirectory(ThirdParty)
add_subdirectory(Engine)

file(GLOB_RECURSE ASSETS_FILES "assets/*")
add_library(assets INTERFACE ${ASSETS_FILES})
set_target_properties(assets PROPERTIES FOLDER "Engine")

set_target_properties(Engine PROPERTIES FOLDER "Engine")


add_executable(${PROJECT_NAME} Engine/main.cpp)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER App)

target_link_libraries(
    ${PROJECT_NAME}
    Engine
)

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_DIR})
endif()

# 复制Assets文件
add_custom_command(
        TARGET ${CMAKE_PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets"
)