#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include "../cell/cell.h"
#include "../direction/direction.h"
#include "../../../../helpers/helpers.h"
#include "../../constants/constants.h"
#include "../../models/models.h"

using namespace std;

// Class that represents the maze.
class Maze {
 private:
  // Maze generation parameters.
  unsigned int width;
  unsigned int height;
  unsigned int checkpointsPercentage;

  // Maze internal variables.
  vector<vector<unsigned int>> finalMaze;
  vector<vector<vector<unsigned int>>> generationSteps;

  // Maze generation statistics.
  long long timePerformanceMs = 0;
  long long iterationsTookToGenerate = 0;
  long long estimatedTimeLeftMs = 0;

 public:
  // Constructor.
  Maze(unsigned int _width, unsigned int _height, unsigned int _checkpointsPercentage);

  // Method that generates the maze.
  void generateMaze();

  // Method that randomly distributes checkpoints on the maze.
  void distributeCheckpoints();

  // Method that generates the solution.
  void generateSolution();

  // Method that visualizes the maze generation.
  void visualizeMazeGeneration(unsigned int minVisualizationDurationMs);

  // Method that prints the maze state.
  static void printMazeState(const vector<vector<unsigned int>>& mazeState);

  // Method that filters out the steps where anything is not changing.
  void filterSteps();

  // Method that checks if the given input parameters are valid.
  void validateInputParameters();

  // Method that checks if a cell is valid.
  bool isValid(int x, int y);
};

#endif
