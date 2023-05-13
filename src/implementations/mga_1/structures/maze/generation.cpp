#include "maze.h"

// Method that generates the maze.
void Maze::generateMaze() {
  // Validate the input.
  validateInputParameters();

  // Print the maze generation parameters.
  cout << colorString("Maze parameters:", "yellow", "black", "bold") << "\n";
  cout << "    - Width: " << width << "\n";
  cout << "    - Height: " << height << "\n";
  cout << "    - Checkpoints percentage: " << checkpointsPercentage << "%" << "\n\n";

  // Wait for user input.
  cout << colorString("Press enter key to start the maze generation...", "green", "black", "bold");
  waitForEnter("");

  // Print the maze generation header.
  cout << "\n" << colorString("Generating maze...", "yellow", "black", "bold");

  // Print a warning if the maze is large.
  if (width * height > MAZE_WARNING_THRESHOLD_AREA) {
    cout << "\n" << colorString("WARNING: The maze is large, so the generation may take a while.", "red", "black", "bold");
  }
  cout << "\n\n";

  // Start the timer.
  auto startTime = chrono::high_resolution_clock::now();

  // Initialize the maze with walls.
  finalMaze = vector<vector<unsigned int>>(height, vector<unsigned int>(width, WALL_ID));

  // Define the random start position.
  unsigned int startX = randomGenerator() % (width / 2) * 2 + 1;
  unsigned int startY = randomGenerator() % (height / 2) * 2 + 1;

  // Set the start position to path.
  finalMaze[startY][startX] = PATH_ID;

  // Create a list of available cells.
  vector<Cell> cells;
  cells.emplace_back(startX, startY);

  // Define the possible moves.
  Direction RIGHT = Direction(0, 2);
  Direction DOWN = Direction(2, 0);
  Direction LEFT = Direction(0, -2);
  Direction UP = Direction(-2, 0);
  vector<Direction> moves = {RIGHT, DOWN, LEFT, UP};

  // Generate the maze.
  while (!cells.empty()) {
    // Pick a random cell.
    unsigned int currentCellIndex = randomGenerator() % cells.size();
    Cell currentCell = cells[currentCellIndex];

    // Find all valid neighbors.
    vector<pair<unsigned int, unsigned int>> validNeighbors;
    for (const auto& move : moves) {
      int newX = int(currentCell.x) + move.offsetX;
      int newY = int(currentCell.y) + move.offsetY;

      // Check if the neighbor is valid.
      if (isValid(newX, newY)) {
        unsigned int betweenX = (currentCell.x + newX) / 2;
        unsigned int betweenY = (currentCell.y + newY) / 2;

        // If the cell between the current cell and the neighbor is a wall, then the neighbor is valid.
        if (finalMaze[betweenY][betweenX] == WALL_ID) {
          validNeighbors.emplace_back(newX, newY);
        }
      }

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }

    // Check if there are any valid neighbors.
    if (!validNeighbors.empty()) {
      // Choose a random neighbor.
      unsigned int randomNeighborIndex = randomGenerator() % validNeighbors.size();
      unsigned int newX = validNeighbors[randomNeighborIndex].first;
      unsigned int newY = validNeighbors[randomNeighborIndex].second;

      // Remove the wall between the current cell and the chosen neighbor.
      finalMaze[(currentCell.y + newY) / 2][(currentCell.x + newX) / 2] = PATH_ID;

      // Set the chosen neighbor to path.
      finalMaze[newY][newX] = PATH_ID;

      // Add the chosen neighbor to the list of available cells.
      cells.emplace_back(newX, newY);
    } else {
      // If there are no valid neighbors, remove the current cell from the list of available cells.
      cells.erase(cells.begin() + currentCellIndex);
    }

    // Add the current maze state to the list of maze states.
    generationSteps.push_back(finalMaze);
  }

  // Find a random cell with PATH_ID and set a start position to it.
  bool isFound = false;
  while (!isFound) {
    const unsigned int x  = randomGenerator() % width;
    const unsigned int y = randomGenerator() % height;
    if (finalMaze[y][x] == PATH_ID) {
      finalMaze[y][x] = START_ID;
      isFound = true;
    }
  }
  generationSteps.push_back(finalMaze);

  // Distribute the checkpoints.
  distributeCheckpoints();

  // Generate the solution.
  generateSolution();

  // Filter out the steps where the path is not changing.
  filterSteps();

  // Stop the timer.
  auto endTime = chrono::high_resolution_clock::now();

  // Calculate the time performance.
  timePerformanceMs = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
}

// Method that randomly distributes checkpoints on the maze.
void Maze::distributeCheckpoints() {
  // Get the number of path cells.
  unsigned int pathCellsCount = 0;
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      // Check if the cell is a path.
      if (finalMaze[y][x] == PATH_ID) {
        // Increment the number of path cells.
        pathCellsCount++;
      }

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }
  }

  // Calculate the number of checkpoints.
  unsigned int checkpointsCount = pathCellsCount * checkpointsPercentage / 100;

  // Distribute the checkpoints.
  unsigned int currentCheckpointsCount = 0;
  while (currentCheckpointsCount < checkpointsCount) {
    unsigned int x = randomGenerator() % width;
    unsigned int y = randomGenerator() % height;
    if (finalMaze[y][x] == PATH_ID) {
      finalMaze[y][x] = CHECKPOINT_ID;
      currentCheckpointsCount++;
    }
  }

  // Add the final maze state to the list of maze states.
  generationSteps.push_back(finalMaze);
}
