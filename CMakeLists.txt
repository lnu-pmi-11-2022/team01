cmake_minimum_required(VERSION 3.24)
project(team-01)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src/implementations/mga_1)

# Currently disabled due to compilation errors and OS compatibility issues
# add_subdirectory(src/implementations/mga_2)

add_library(helpers src/helpers/helpers.cpp)

add_executable(main src/main.cpp)
target_link_libraries(main mga_1 helpers)


add_executable(playground src/playground.cpp)
