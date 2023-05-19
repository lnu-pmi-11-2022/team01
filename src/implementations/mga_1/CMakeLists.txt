cmake_minimum_required(VERSION 3.24)
project(mga_1)

set(CMAKE_CXX_STANDARD 17)

# SFML is required for this implementation

# To install SFML on Ubuntu, run the following command:
# sudo apt-get install libsfml-dev

# To install SFML on Windows, download the binaries from the official website:
# https://www.sfml-dev.org/download/sfml/2.5.1/
# and add the SFML_ROOT environment variable to the path of the SFML directory
# (e.g. C:\SFML-2.5.1)

# To install SFML on Mac, run the following command:
# brew install sfml

find_package(SFML 2 COMPONENTS audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
endif()

add_library(mga_1 mga_1.cpp)
add_library(maze structures/maze/common.cpp structures/maze/generation.cpp structures/maze/solution.cpp structures/maze/output.cpp structures/maze/helpers.cpp)
add_library(cell structures/cell/cell.cpp)
add_library(path structures/path/path.cpp)
add_library(direction structures/direction/direction.cpp)

target_link_libraries(mga_1 maze cell path direction sfml-audio)

target_include_directories(mga_1 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
