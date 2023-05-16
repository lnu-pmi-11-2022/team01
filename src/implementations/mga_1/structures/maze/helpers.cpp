#include "maze.h"

// Method that checks if a cell is valid wall.
bool Maze::isValidWall(int x, int y) {
  return x >= 0 && y >= 0 && x < width && y < height && finalMaze[y][x] == WALL_ID;
}

// Method that checks if a cell is valid path.
bool Maze::isValidPath(int x, int y) {
  return x >= 0 && y >= 0 && x < width && y < height && finalMaze[y][x] != WALL_ID;
}

// Method that gets all checkpoints from the maze.
vector<Cell> Maze::getCheckpoints() {
  // Declare the checkpoints vector.
  vector<Cell> checkpoints;

  // Get all checkpoints from the maze.
  for (int i = 0; i < finalMaze.size(); i++) {
    for (int j = 0; j < finalMaze[0].size(); j++) {
      if (finalMaze[i][j] == CHECKPOINT_ID) {
        checkpoints.emplace_back(j, i);
      }
    }
  }

  // Return the checkpoints vector.
  return checkpoints;
}

// Method that returns the checkpoint ID.
int Maze::getCheckpointId(Cell checkpoint) {
  // Get all checkpoints from the maze.
  vector<Cell> checkpoints = getCheckpoints();

  // Find the checkpoint ID.
  for (int i = 0; i < checkpoints.size(); i++) {
    if (checkpoints[i].y == checkpoint.y && checkpoints[i].x == checkpoint.x) {
      return i;
    }
  }

  // Return -1 if the checkpoint was not found.
  return -1;
}

// Method that gets the number of cells of a given type.
unsigned int Maze::getTheNumberOfCells(MazeCellTypeIds type) {
  // Get the number of cells.
  unsigned int cellsCount = 0;
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      // Check if the cell is of the given type.
      if (finalMaze[y][x] == type) {
        // Increment the number of cells.
        cellsCount++;
      }

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }
  }

  // Return the number of cells.
  return cellsCount;
}

// Method that gets the name of solving algorithm.
string Maze::getSolvingAlgorithmName() {
  // Define the name of the solving algorithm.
  string solvingAlgorithmName;

  // Find the name of the solving algorithm.
  for (auto & algorithm : SUPPORTED_SOLVING_ALGORITHMS) {
      if (algorithm.first == solvingAlgorithm) {
        solvingAlgorithmName = algorithm.second;
        break;
      }
  }

  // Return the name of the solving algorithm.
  return solvingAlgorithmName;
}

