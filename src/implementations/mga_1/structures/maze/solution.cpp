#include "maze.h"

// Method that generates the solution.
void Maze::generateSolution() {
  // Get the shortest paths between each pair of checkpoints.
  vector<Path> paths = findShortestPathsBetweenEachPairOfCheckpoints();

  // Create the adjacency matrix.
  vector<vector<double>> matrix = createAdjacencyMatrix(paths);

  // Apply the traveling salesman problem algorithm.
  vector<Cell> tspResult = tspAlgorithm(matrix);

  // Construct the final path.
  Path finalPath = constructFinalPath(tspResult, paths);

  // Set the minimum path length.
  minPathLength = (unsigned int)finalPath.length;

  // Get the checkpoints.
  vector<Cell> checkpoints = getCheckpoints();

  // Iterate over the path cells.
  for (int i = 0; i < finalPath.path.size(); i++) {
    // Get the current cell.
    Cell currentCell = finalPath.path[i];

    // If the current cell is not the last cell.
    if (i != finalPath.path.size() - 1) {
      // Mark the current cell.
      finalMaze[currentCell.y][currentCell.x] = CURRENT_POSITION_ID;
    } else {
      // Mark the current cell.
      finalMaze[currentCell.y][currentCell.x] = END_ID;
    }

    // If the current cell is not the first cell.
    if (i > 0) {
      // Get the previous cell.
      Cell previousCell = finalPath.path[i - 1];

      // Check if the previous cell was a checkpoint.
      bool isCheckpoint = false;
      for (auto & checkpoint : checkpoints) {
        if (previousCell.x == checkpoint.x && previousCell.y == checkpoint.y) {
          isCheckpoint = true;
          break;
        }

        // Increment the number of iterations to generate the maze.
        iterationsTookToGenerate++;
      }

      // Mark the current cell.
      if (i - 1 == 0) {
        // Mark the start cell.
        finalMaze[previousCell.y][previousCell.x] = START_ID;
      } else if (i - 1 == finalPath.path.size() - 1) {
        // Mark the end cell.
        finalMaze[previousCell.y][previousCell.x] = END_ID;
      } else if (isCheckpoint) {
        // Mark the checkpoint cell.
        finalMaze[previousCell.y][previousCell.x] = PASSED_CHECKPOINT_ID;
      } else {
        // Mark the path cell.
        finalMaze[previousCell.y][previousCell.x] = PASSED_PATH_ID;
      }
    }

    // Add the current maze state to the list of maze states.
    generationSteps.push_back(finalMaze);

    // Increment the number of iterations to generate the maze.
    iterationsTookToGenerate++;
  }
}

// Method that returns the shortest path between each pair of checkpoints.
vector<Path> Maze::findShortestPathsBetweenEachPairOfCheckpoints() {
  // Get all the checkpoints.
  vector<Cell> checkpoints = getCheckpoints();

  // Declare a vector to store all the paths.
  vector<Path> paths;

  // Find the shortest path between each pair of checkpoints.
  for (int i = 0; i < checkpoints.size() - 1; i++) {
    for (int j = i + 1; j < checkpoints.size(); j++) {
      Path path = findShortestPathBetweenCells(checkpoints[i], checkpoints[j]);
      paths.push_back(path);

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }
  }

  // Return the paths.
  return paths;
}

// Method that returns the shortest path between two cells.
Path Maze::findShortestPathBetweenCells(Cell startCell, Cell endCell) {
  // Declare a path to store the result.
  vector<Cell> path;

  // Declare a variable to store the length of the path.
  double length = 0;

  // Declare a flag to indicate if the end cell was found.
  bool isFound = false;

  // Create a 2D array to store the distance from the start cell to each visited cell.
  vector<vector<double>> distances(height, vector<double>(width, INFINITY));

  // Create a 2D array to store the previous cell in the shortest path to each visited cell.
  vector<vector<Cell>> previous(height, vector<Cell>(width, {-1, -1}));

  // Initialize the distance from the start cell to itself to 0.
  distances[startCell.y][startCell.x] = 0;

  // Create a queue for BFS and add the start cell to it.
  queue<Cell> cells;
  cells.push(startCell);

  // Perform BFS until the end cell is found or the queue is empty.
  while (!cells.empty()) {
    Cell current = cells.front();
    cells.pop();

    // Stop BFS if the end cell is found.
    if (current.y == endCell.y && current.x == endCell.x) {
      isFound = true;
      break;
    }

    // Check the neighbors of the current cell.
    vector<Cell> neighbors = {
        {current.x, current.y - 1},
        {current.x, current.y + 1},
        {current.x - 1, current.y},
        {current.x + 1, current.y},
    };

    // Iterate over the neighbors.
    for (Cell neighbor : neighbors) {
      // Check if the neighbor is a valid cell and can be visited.
      if (isValidPath(neighbor.x, neighbor.y)) {
        // Calculate the distance to the neighbor cell.
        double newDistance = distances[current.y][current.x] + 1;

        // Check if the new distance is shorter than the previous distance to the neighbor cell.
        if (newDistance < distances[neighbor.y][neighbor.x]) {
          distances[neighbor.y][neighbor.x] = newDistance;
          previous[neighbor.y][neighbor.x] = current;
          cells.push(neighbor);
        }
      }

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }
  }

  // Construct the path and calculate its length if the end cell was found.
  if (isFound) {
    Cell current = endCell;
    while (current.y != startCell.y || current.x != startCell.x) {
      path.insert(path.begin(), current);
      length++;
      current = previous[current.y][current.x];

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }
    path.insert(path.begin(), current);
  }

  // Return the result.
  return {path, length, {startCell, endCell}};
}

// Method that creates an adjacency matrix from a list of paths.
vector<vector<double>> Maze::createAdjacencyMatrix(const vector<Path>& paths) {
  // Get all the checkpoints.
  vector<Cell> checkpoints = getCheckpoints();

  // Define a matrix to store the adjacency matrix.
  vector<vector<double>> matrix(checkpoints.size(), vector<double>(checkpoints.size(), 0));

  // Fill the matrix with the lengths of the paths.
  for (Path p : paths) {
    int i = getCheckpointId(p.checkpoints[0]);
    int j = getCheckpointId(p.checkpoints[1]);
    matrix[i][j] = p.length;
    matrix[j][i] = p.length;

    // Increment the number of iterations to generate the maze.
    iterationsTookToGenerate++;
  }

  // Return the matrix.
  return matrix;
}

// Method that implements the traveling salesman problem algorithm.
// TODO: Rewrite using Held-Karp algorithm.
vector<Cell> Maze::tspAlgorithm(vector<vector<double>> adjacencyMatrix) {
  // Get all the checkpoints.
  vector<Cell> checkpoints = getCheckpoints();

  // Get the number of checkpoints.
  unsigned int numberOfCheckpoints = adjacencyMatrix.size();

  // Create a vector to store the order of the checkpoints and initialize it.
  vector<int> order(numberOfCheckpoints);
  for (int i = 0; i < numberOfCheckpoints; i++) {
    order[i] = i;
  }

  // Define a variable to store the shortest length and the order of the checkpoints.
  double shortestLength = INFINITY;
  vector<int> shortestOrder;

  // While there are more permutations of the order of the checkpoints.
  do {
    // Calculate the length of the current order.
    double length = 0;

    // Set a flag to indicate if the current order is valid.
    bool isValid = true;

    // Set the current checkpoint to the first checkpoint in the order.
    int currentCheckpoint = order[0];

    // Iterate over the rest of the checkpoints in the order.
    for (int i = 1; i < numberOfCheckpoints; i++) {
      // Get the next checkpoint.
      int nextCheckpoint = order[i];

      // Check if the path between the current and the next checkpoint exists.
      if (adjacencyMatrix[currentCheckpoint][nextCheckpoint] == 0) {
        isValid = false;
        break;
      }

      // Add the length of the path between the current and the next checkpoint to the total length.
      length += adjacencyMatrix[currentCheckpoint][nextCheckpoint];

      // Set the next checkpoint as the current checkpoint.
      currentCheckpoint = nextCheckpoint;

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }

    // Check if the current order is valid and if the path between the last and the first checkpoint exists.
    if (isValid && adjacencyMatrix[currentCheckpoint][order[0]] != 0) {
      // Add the length of the path between the last and the first checkpoint to the total length.
      length += adjacencyMatrix[currentCheckpoint][order[0]];

      // Check if the current order is the shortest order.
      if (length < shortestLength) {
        shortestLength = length;
        shortestOrder = order;
      }
    }
  } while (next_permutation(order.begin() + 1, order.end()));

  // Construct the final order of the checkpoints.
  vector<Cell> result;

  // Reserve the size of the result vector.
  result.reserve(shortestOrder.size());

  // Add the checkpoints in the shortest order to the result vector.
  for (int i : shortestOrder) {
    result.push_back(checkpoints[i]);

    // Increment the number of iterations to generate the maze.
    iterationsTookToGenerate++;
  }

  // Return the result.
  return result;
}

// Method that constructs the final path from the order of the checkpoints and the paths.
Path Maze::constructFinalPath(vector<Cell> checkpointsOrder, const vector<Path>& paths) {
  // Define a vector to store the final path.
  vector<Cell> finalPath;

  // Iterate over the checkpoints in the order.
  for (int i = 0; i < checkpointsOrder.size() - 1; i++) {
    // Get the current and the next checkpoint.
    Cell checkpoint1 = checkpointsOrder[i];
    Cell checkpoint2 = checkpointsOrder[i+1];

    // Iterate over the paths.
    for (Path path : paths) {
      // Check if the path has the same checkpoints as the current and the next checkpoint.
      if ((path.checkpoints[0] == checkpoint1 && path.checkpoints[1] == checkpoint2) || (path.checkpoints[0] == checkpoint2 && path.checkpoints[1] == checkpoint1)) {
        // Add the path to the final path.
        if (path.checkpoints[0] == checkpoint1) {
          finalPath.push_back(path.checkpoints[0]);
          for (Cell cell : path.path) {
            finalPath.push_back(cell);

            // Increment the number of iterations to generate the maze.
            iterationsTookToGenerate++;
          }
          finalPath.push_back(path.checkpoints[1]);
        } else {
          finalPath.push_back(path.checkpoints[1]);
          for (int j = (int)path.path.size() - 1; j >= 0; j--) {
            finalPath.push_back(path.path[j]);

            // Increment the number of iterations to generate the maze.
            iterationsTookToGenerate++;
          }
          finalPath.push_back(path.checkpoints[0]);
        }

        // Stop iterating over the paths.
        break;
      }
    }
  }

  // Filter out duplicate consecutive cells
  vector<Cell> filteredFinalPath;
  for (int i = 0; i < finalPath.size(); i++) {
    if (i == 0 || finalPath[i] != finalPath[i-1]) {
      filteredFinalPath.push_back(finalPath[i]);
    }

    // Increment the number of iterations to generate the maze.
    iterationsTookToGenerate++;
  }

  // Prevent the path from continuing after the last checkpoint.
  vector<Cell> markedCheckpoints;
  for (int i = 0; i < filteredFinalPath.size() - 1; i++) {
    if (markedCheckpoints.size() == actualNumberOfCheckpoints) {
      // Remove the current and the next cells from the final path.
      filteredFinalPath.erase(filteredFinalPath.begin() + i, filteredFinalPath.end() - 1);

      // Stop iterating over the final path.
      break;
    }

    // Check if the current cell is a checkpoint.
    if (finalMaze[filteredFinalPath[i].y][filteredFinalPath[i].y] == CHECKPOINT_ID) {
      // Check if the current checkpoint was already added before.
        bool isMarked = false;
        for (Cell cell : markedCheckpoints) {
          if (cell == filteredFinalPath[i]) {
            isMarked = true;
            break;
          }

          // Increment the number of iterations to generate the maze.
          iterationsTookToGenerate++;
        }

        // If the current checkpoint was not added before, add it to the marked checkpoints.
        if (!isMarked) {
          markedCheckpoints.push_back(filteredFinalPath[i]);
        }
    }

    // Increment the number of iterations to generate the maze.
    iterationsTookToGenerate++;
  }

  // Return the final path.
  return {filteredFinalPath, (double)(filteredFinalPath.size() - 1), checkpointsOrder};
}
