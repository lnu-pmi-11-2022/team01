#include "maze.h"

// Method that checks if a cell is valid.
bool Maze::isValid(int x, int y) {
  return x >= 0 && y >= 0 && x < width && y < height && finalMaze[y][x] == WALL_ID;
}

