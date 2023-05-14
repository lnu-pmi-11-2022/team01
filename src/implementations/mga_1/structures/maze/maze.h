#ifndef MAZE_H
#define MAZE_H

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include "../cell/cell.h"
#include "../path/path.h"
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
  unsigned int minPathLength = 0;
  unsigned int requestedNumberOfCheckpoints = 0;
  unsigned int actualNumberOfCheckpoints = 0;

 public:
  // Constructor.
  Maze(unsigned int _width, unsigned int _height, unsigned int _checkpointsPercentage);

  // Method that generates the maze.
  void generateMaze();

  // Method that randomly distributes checkpoints on the maze.
  unsigned int distributeCheckpoints();

  // Method that generates the solution.
  void generateSolution();

  // Method that returns the shortest path between each pair of checkpoints.
  vector<Path> findShortestPathsBetweenEachPairOfCheckpoints();

  // Method that returns the shortest path between two cells.
  Path findShortestPathBetweenCells(Cell startCell, Cell endCell);

  // Method that creates an adjacency matrix from a list of paths.
  vector<vector<double>> createAdjacencyMatrix(const vector<Path>& paths);

  // Method that implements the traveling salesman problem algorithm.
  vector<Cell> tspAlgorithm(vector<vector<double>> adjacencyMatrix);

  // Method that constructs the final path from the order of the checkpoints and the paths.
  Path constructFinalPath(vector<Cell> checkpointsOrder, const vector<Path>& paths);

  // Method that visualizes the maze generation.
  void visualizeMazeGeneration(unsigned int minVisualizationDurationMs);

  // Method that prints the maze state.
  static void printMazeState(const vector<vector<unsigned int>>& mazeState, bool printAsIDs = PRINT_MAZE_AS_IDS);

  // Method that filters out the steps where anything is not changing.
  void filterSteps();

  // Method that checks if the given input parameters are valid.
  void validateInputParameters();

  // Method that checks if a cell is valid wall.
  bool isValidWall(int x, int y);

  // Method that checks if a cell is valid path.
  bool isValidPath(int x, int y);

  // Method that gets all checkpoints from the maze.
  vector<Cell> getCheckpoints();

  // Method that gets the checkpoint ID.
  int getCheckpointId(Cell checkpoint);

  // Method that gets the number of cells of a given type.
  unsigned int getTheNumberOfCells(MazeCellTypeIds type);
};

#endif
