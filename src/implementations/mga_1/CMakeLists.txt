cmake_minimum_required(VERSION 3.24)
project(mga_1)

set(CMAKE_CXX_STANDARD 17)

add_executable(mga_1 main.cpp)

add_library(maze structures/maze/common.cpp structures/maze/generation.cpp structures/maze/solution.cpp structures/maze/output.cpp structures/maze/helpers.cpp)
add_library(cell structures/cell/cell.cpp)
add_library(direction structures/direction/direction.cpp)
add_library(helpers helpers/helpers.cpp)

target_link_libraries(mga_1 maze cell direction helpers)

target_include_directories(mga_1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

