cmake_minimum_required(VERSION 3.24)
project(mga_1)

set(CMAKE_CXX_STANDARD 17)

add_library(mga_1 mga_1.cpp)
add_library(maze structures/maze/common.cpp structures/maze/generation.cpp structures/maze/solution.cpp structures/maze/output.cpp structures/maze/helpers.cpp)
add_library(cell structures/cell/cell.cpp)
add_library(path structures/path/path.cpp)
add_library(direction structures/direction/direction.cpp)

# SFML is required for this project.

# To install SFML on Ubuntu, run the following command:
# sudo apt-get install libsfml-dev

# To install SFML on Windows, download the binaries from the official website:
# https://www.sfml-dev.org/download/sfml/2.5.1/
# and add the SFML_ROOT environment variable to the path of the SFML directory
# (e.g. C:\SFML-2.5.1)

# To install SFML on Mac, run the following command:
# brew install sfml
find_package(SFML 2.5 COMPONENTS audio REQUIRED)

# If you are using Linux, you can comment out the following lines.
#set(SFML_INCLUDE_DIR /usr/include/SFML)
#set(SFML_LIBRARY_DIR /usr/lib/x86_64-linux-gnu)

# If you are using Windows, you can comment out the following lines.
#set(SFML_INCLUDE_DIR C:/SFML-2.5.1/include)
#set(SFML_LIBRARY_DIR C:/SFML-2.5.1/lib)

# If you are using MacOS and installed SFML using Homebrew, you can comment out the following lines.
set(SFML_INCLUDE_DIR /opt/homebrew/include)
set(SFML_LIBRARY_DIR /opt/homebrew/lib)

include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIBRARY_DIR})

target_link_libraries(mga_1 maze cell path direction sfml-audio)
