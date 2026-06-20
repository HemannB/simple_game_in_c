cmake_minimum_required(VERSION 3.16)
project(tetris C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output binary to build/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Find SDL2
find_package(SDL2 REQUIRED)

add_executable(tetris
        src/main.c
        src/grid.c
        src/tetromino.c
)

target_include_directories(tetris PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(tetris PRIVATE ${SDL2_LIBRARIES})

# Warnings
target_compile_options(tetris PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
    $<$<C_COMPILER_ID:MSVC>:/W4>
)