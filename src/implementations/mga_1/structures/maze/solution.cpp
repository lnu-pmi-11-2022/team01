#include "maze.h"

// Method that generates the solution.
void Maze::generateSolution() {
  // Set the time point for benchmarking.
  auto stepStartTime = chrono::high_resolution_clock::now();

  // Get the shortest paths between each pair of checkpoints.
  cout << colorString("Finding the shortest paths between each pair of checkpoints...", "yellow", "black", "bold") << "\n";
  vector<Path> paths = findShortestPathsBetweenEachPairOfCheckpoints();
  cout << colorString("DONE!", "green", "black", "bold");
  unsigned long long timePerformance = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - stepStartTime).count();
  cout << colorString(" (Took " + millisecondsToTimeString(timePerformance) + ")", "white", "black", "bold") << "\n\n";
  stepStartTime = chrono::high_resolution_clock::now();

  // Create the adjacency matrix.
  cout << colorString("Creating the adjacency matrix...", "yellow", "black", "bold") << "\n";
  vector<vector<double>> matrix = createAdjacencyMatrix(paths);
  cout << colorString("DONE!", "green", "black", "bold");
  timePerformance = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - stepStartTime).count();
  cout << colorString(" (Took " + millisecondsToTimeString(timePerformance) + ")", "white", "black", "bold") << "\n\n";
  stepStartTime = chrono::high_resolution_clock::now();

  // Apply the chosen traveling salesman problem solving algorithm.
  cout << colorString("Applying the chosen TSP solving algorithm...", "yellow", "black", "bold") << "\n";
  vector<Cell> tspResult;
  switch (solvingAlgorithm) {
    case SupportedSolvingAlgorithms::HELD_KARP_PARALLEL:
      tspResult = tspHeldKarpParallel(matrix);
      break;
    case SupportedSolvingAlgorithms::HELD_KARP:
      tspResult = tspHeldKarp(matrix);
      break;
    case SupportedSolvingAlgorithms::BRUTE_FORCE:
      tspResult = tspBruteForce(matrix);
      break;
    default:
      break;
  }
  cout << colorString("DONE!", "green", "black", "bold");
  timePerformance = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - stepStartTime).count();
  cout << colorString(" (Took " + millisecondsToTimeString(timePerformance) + ")", "white", "black", "bold") << "\n\n";
  stepStartTime = chrono::high_resolution_clock::now();

  // Construct the final path.
  cout << colorString("Constructing the final path...", "yellow", "black", "bold") << "\n";
  Path finalPath = constructFinalPath(tspResult, paths);
  cout << colorString("DONE!", "green", "black", "bold");
  timePerformance = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - stepStartTime).count();
  cout << colorString(" (Took " + millisecondsToTimeString(timePerformance) + ")", "white", "black", "bold") << "\n\n";

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
      for (auto &checkpoint : checkpoints) {
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
vector<vector<double>> Maze::createAdjacencyMatrix(const vector<Path> &paths) {
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

// Method that implements the traveling salesman problem using Held-Karp (dynamic programming) algorithm and runs it in a single thread.
vector<Cell> Maze::tspHeldKarp(vector<vector<double>> adjacencyMatrix) {
  // Get all the checkpoints and their quantity.
  const vector<Cell> checkpoints = getCheckpoints();
  const unsigned int checkpointsQuantity = adjacencyMatrix.size();

  // Define a memoization matrix.
  auto** memoizationMatrix = new double*[checkpointsQuantity];
  for (int i = 0; i < checkpointsQuantity; i++) {
    memoizationMatrix[i] = new double[1 << checkpointsQuantity];
  }

  // Define a matrix to reconstruct the path.
  auto** parentMatrix = new int*[checkpointsQuantity];
  for (int i = 0; i < checkpointsQuantity; i++) {
    parentMatrix[i] = new int[1 << checkpointsQuantity];
  }

  // Iterate over all the checkpoints and find the shortest path starting from each checkpoint.
  vector<Cell> shortestPath;
  double shortestPathLength = numeric_limits<double>::max();
  for (int startPoint = 0; startPoint < checkpointsQuantity; startPoint++) {
    // Initialize the memoization table with default values.
    for (int i = 0; i < checkpointsQuantity; i++) {
      for (int j = 0; j < (1 << checkpointsQuantity); j++) {
        memoizationMatrix[i][j] = -1;
        parentMatrix[i][j] = -1;
      }
    }

    // Set the path that starts and ends at the current checkpoint.
    memoizationMatrix[startPoint][1 << startPoint] = 0;

    // Iterate over all the possible subsets of checkpoints.
    for (int mask = 1; mask < (1 << checkpointsQuantity); mask++) {
      // Iterate over all the checkpoints.
      for (int i = 0; i < checkpointsQuantity; i++) {
        // If the current checkpoint is not in the current subset, skip it.
        if (!(mask & (1 << i))) continue;

        // Iterate over all the checkpoints.
        for (int j = 0; j < checkpointsQuantity; j++) {
          // If the current checkpoint is in the current subset or is the same as the current checkpoint, skip it.
          if (mask & (1 << j) || i == j) continue;

          // Calculate the new mask.
          int newMask = mask | (1 << j);

          // Check if the new mask is valid and if the path between the current and the next checkpoint exists.
          if (memoizationMatrix[j][newMask] == -1 || memoizationMatrix[j][newMask] > memoizationMatrix[i][mask] + adjacencyMatrix[i][j]) {
            // Update the memoization table and the parent matrix.
            memoizationMatrix[j][newMask] = memoizationMatrix[i][mask] + adjacencyMatrix[i][j];
            parentMatrix[j][newMask] = i;
          }
        }
      }
    }

    // Find the shortest path that starts and ends at the current checkpoint.
    for (int i = 0; i < checkpointsQuantity; i++) {
      if (i == startPoint) continue;
      if (shortestPathLength > memoizationMatrix[i][(1 << checkpointsQuantity) - 1] + adjacencyMatrix[i][startPoint]) {
        shortestPathLength = memoizationMatrix[i][(1 << checkpointsQuantity) - 1] + adjacencyMatrix[i][startPoint];
        shortestPath.clear();

        // Reconstruct the shortest path.
        int currentCheckpoint = i;
        int currentMask = (1 << checkpointsQuantity) - 1;
        while (currentCheckpoint != -1) {
          shortestPath.push_back(checkpoints[currentCheckpoint]);
          int parentCheckpoint = parentMatrix[currentCheckpoint][currentMask];
          int parentMask = currentMask ^ (1 << currentCheckpoint);
          currentCheckpoint = parentCheckpoint;
          currentMask = parentMask;
        }
        reverse(shortestPath.begin(), shortestPath.end());
      }
    }
  }

  // Delete the memoization matrix and the parent matrix.
  for (int i = 0; i < checkpointsQuantity; i++) {
    delete[] memoizationMatrix[i];
    delete[] parentMatrix[i];
  }
  delete[] memoizationMatrix;
  delete[] parentMatrix;

  return shortestPath;
}

// Method that implements the traveling salesman problem using Held-Karp (dynamic programming) algorithm and runs it in multiple threads.
vector<Cell> Maze::tspHeldKarpParallel(vector<vector<double>> adjacencyMatrix) {
  // Get all the checkpoints and their quantity.
  const vector<Cell> checkpoints = getCheckpoints();
  const unsigned int checkpointsQuantity = adjacencyMatrix.size();

  // Display the stats of the threads.
  unsigned int numThreadsAvailable = thread::hardware_concurrency();
  cout << "  - Number of threads available: " << numThreadsAvailable << "\n";
  cout << "  - Number of threads to be used: " << checkpointsQuantity << "\n";

  // Define a vector to store all the threads.
  vector<thread> threads;

  // Define a mutex to lock the shortest path and shortest path length.
  mutex shortestPathMutex;
  mutex shortestPathLengthMutex;

  // Define variables to store the shortest path and shortest path length.
  vector<Cell> shortestPath;
  double shortestPathLength = numeric_limits<double>::max();

  // Define a function to be run by each thread.
  auto threadFunction = [&](int startPoint) {
    // Define a memoization matrix.
    auto** memoizationMatrix = new double*[checkpointsQuantity];
    for (int i = 0; i < checkpointsQuantity; i++) {
      memoizationMatrix[i] = new double[1 << checkpointsQuantity];
    }

    // Define a matrix to reconstruct the path.
    auto** parentMatrix = new int*[checkpointsQuantity];
    for (int i = 0; i < checkpointsQuantity; i++) {
      parentMatrix[i] = new int[1 << checkpointsQuantity];
    }

    // Initialize the memoization table with default values.
    for (int i = 0; i < checkpointsQuantity; i++) {
      for (int j = 0; j < (1 << checkpointsQuantity); j++) {
        memoizationMatrix[i][j] = -1;
        parentMatrix[i][j] = -1;
      }
    }

    // Set the path that starts and ends at the current checkpoint.
    memoizationMatrix[startPoint][1 << startPoint] = 0;

    // Iterate over all the possible subsets of checkpoints.
    for (int mask = 1; mask < (1 << checkpointsQuantity); mask++) {
      // Iterate over all the checkpoints.
      for (int i = 0; i < checkpointsQuantity; i++) {
        // If the current checkpoint is not in the current subset, skip it.
        if (!(mask & (1 << i))) continue;

        // Iterate over all the checkpoints.
        for (int j = 0; j < checkpointsQuantity; j++) {
          // If the current checkpoint is in the current subset or is the same as the current checkpoint, skip it.
          if (mask & (1 << j) || i == j) continue;

          // Calculate the new mask.
          int newMask = mask | (1 << j);

          // Check if the new mask is valid and if the path between the current and the next checkpoint exists.
          if (memoizationMatrix[j][newMask] == -1 || memoizationMatrix[j][newMask] > memoizationMatrix[i][mask] + adjacencyMatrix[i][j]) {
            // Update the memoization table and the parent matrix.
            memoizationMatrix[j][newMask] = memoizationMatrix[i][mask] + adjacencyMatrix[i][j];
            parentMatrix[j][newMask] = i;
          }
        }
      }
    }

    // Find the shortest path that starts and ends at the current checkpoint.
    vector<Cell> currentShortestPath;
    double currentShortestPathLength = numeric_limits<double>::max();
    for (int i = 0; i < checkpointsQuantity; i++) {
      if (i == startPoint) continue;
      if (currentShortestPathLength > memoizationMatrix[i][(1 << checkpointsQuantity) - 1] + adjacencyMatrix[i][startPoint]) {
        currentShortestPathLength = memoizationMatrix[i][(1 << checkpointsQuantity) - 1] + adjacencyMatrix[i][startPoint];
        currentShortestPath.clear();

        // Reconstruct the shortest path.
        int currentCheckpoint = i;
        int currentMask = (1 << checkpointsQuantity) - 1;
        while (currentCheckpoint != -1) {
          currentShortestPath.push_back(checkpoints[currentCheckpoint]);
          int parentCheckpoint = parentMatrix[currentCheckpoint][currentMask];
          int parentMask = currentMask ^ (1 << currentCheckpoint);
          currentCheckpoint = parentCheckpoint;
          currentMask = parentMask;
        }
        reverse(currentShortestPath.begin(), currentShortestPath.end());
      }
    }

    // Lock the shortest path and shortest path length and update them if the current path is shorter.
    shortestPathMutex.lock();
    if (currentShortestPathLength < shortestPathLength) {
      shortestPathLength = currentShortestPathLength;
      shortestPath = currentShortestPath;
    }
    shortestPathMutex.unlock();

    // Delete the memoization matrix and the parent matrix.
    for (int i = 0; i < checkpointsQuantity; i++) {
      delete[] memoizationMatrix[i];
      delete[] parentMatrix[i];
    }
    delete[] memoizationMatrix;
    delete[] parentMatrix;
  };

  // Start a thread for each starting point.
  threads.reserve(checkpointsQuantity);
  for (int startPoint = 0; startPoint < checkpointsQuantity; startPoint++) {
    threads.emplace_back(threadFunction, startPoint);
  }

  // Wait for all the threads to finish.
  for (auto& thread : threads) {
    thread.join();
  }

  return shortestPath;
}

// Method that implements the traveling salesman problem using brute force algorithm.
vector<Cell> Maze::tspBruteForce(vector<vector<double>> adjacencyMatrix) {
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
      // Check if the current order is the shortest order.
      if (length < shortestLength) {
        shortestLength = length;
        shortestOrder = order;
      }
    }
  } while (next_permutation(order.begin(), order.end()));

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
Path Maze::constructFinalPath(vector<Cell> checkpointsOrder, const vector<Path> &paths) {
  // Define a vector to store the final path.
  vector<Cell> finalPath;

  // Iterate over the checkpoints in the order.
  for (int i = 0; i < checkpointsOrder.size() - 1; i++) {
    // Get the current and the next checkpoint.
    Cell checkpoint1 = checkpointsOrder[i];
    Cell checkpoint2 = checkpointsOrder[i + 1];

    // Iterate over the paths.
    for (Path path : paths) {
      // Check if the path has the same checkpoints as the current and the next checkpoint.
      if ((path.checkpoints[0] == checkpoint1 && path.checkpoints[1] == checkpoint2)
          || (path.checkpoints[0] == checkpoint2 && path.checkpoints[1] == checkpoint1)) {
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
          for (int j = (int) path.path.size() - 1; j >= 0; j--) {
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
    if (i == 0 || finalPath[i] != finalPath[i - 1]) {
      filteredFinalPath.push_back(finalPath[i]);
    }

    // Increment the number of iterations to generate the maze.
    iterationsTookToGenerate++;
  }

  // Filter out cells when the path passed through all the checkpoints.
  vector<Cell> passedCheckpoints;
  for (int i = 0; i < filteredFinalPath.size(); i++) {
    // Get the current cell.
    Cell currentCell = filteredFinalPath[i];

    // Check if the current cell is a checkpoint and if it was not passed before.
    if (find(checkpointsOrder.begin(), checkpointsOrder.end(), currentCell) != checkpointsOrder.end()
        && find(passedCheckpoints.begin(), passedCheckpoints.end(), currentCell) == passedCheckpoints.end()) {
      // Add the current cell to the passed checkpoints.
      passedCheckpoints.push_back(currentCell);
    }

    // Check if the path passed through all the checkpoints.
    if (passedCheckpoints.size() == checkpointsOrder.size()) {
      // Erase the cells after the current cell and break from the loop.
      filteredFinalPath.erase(filteredFinalPath.begin() + i + 1, filteredFinalPath.end());
      break;
    }
  }

  // Return the final path.
  return {filteredFinalPath, (double) (filteredFinalPath.size() - 1), checkpointsOrder};
}
