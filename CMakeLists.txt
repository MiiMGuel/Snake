cmake_minimum_required(VERSION 3.30.0)

project(
    Snake
    VERSION 1.0
    DESCRIPTION "idk"
    LANGUAGES C CXX
)

add_subdirectory(lib/raylib)

add_executable(snake)

target_sources(
    snake
    PRIVATE
    ${PROJECT_SOURCE_DIR}/src/snake.cpp
)

target_include_directories(
    snake
    PRIVATE 
    ${PROJECT_SOURCE_DIR}/src 
)

target_link_libraries(
    snake 
    PRIVATE
    raylib
)