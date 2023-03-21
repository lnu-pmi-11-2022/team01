#include <iostream>
#include <utility>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

using namespace std;

// Define command to clear the console
// Use "cls" for Windows and "clear" for Unix based systems
// const string CONSOLE_CLEAR_COMMAND = "cls";
const string CONSOLE_CLEAR_COMMAND = "clear";

// Define the matrix cell types IDs
const int PATH_ID = 0;
const int WALL_ID = 1;
const int CHECKPOINT_ID = 2;
const int PASSED_PATH_ID = 3;
const int PASSED_CHECKPOINT_ID = 4;
const int START_ID = 5;
const int END_ID = 6;

// Define the symbols used to represent the matrix cell types
const string WALL_SYMBOL = "██";
const string PATH_SYMBOL = "  ";
const string CHECKPOINT_SYMBOL = "* ";
const string PASSED_PATH_SYMBOL = "• ";
const string PASSED_CHECKPOINT_SYMBOL = "+ ";
const string START_SYMBOL = "> ";
const string END_SYMBOL = " <";

struct Cell {
  int x, y;

  Cell(int _x, int _y) : x(_x), y(_y) {}
};

struct Direction {
  int offsetX, offsetY;

  Direction(int _offsetX, int _offsetY) : offsetX(_offsetX), offsetY(_offsetY) {}
};

class Maze {
 private:
  unsigned int width;
  unsigned int height;
  unsigned int checkpointsPercentage;

  vector<vector<int>> finalMaze;
  vector<vector<vector<int>>> generationSteps;
  long long timePerformanceMs = 0;
  long long iterationsToGenerate = 0;

 public:
  Maze(unsigned int _width, unsigned int _height, unsigned int _checkpointsPercentage) {
    this->width = _width;
    this->height = _height;
    this->checkpointsPercentage = _checkpointsPercentage;
    generateMaze();
  }

  void generateMaze() {
    // Start the timer.
    auto startTime = chrono::high_resolution_clock::now();

    // Validate the input.
    validateInputParameters();

    // Print the maze generation parameters.
    cout << "Generating maze..." << "\n";
    cout << "- Width: " << width << "\n";
    cout << "- Height: " << height << "\n\n";

    // Seed the random number generator.
    srand(time(NULL));

    // Initialize the maze with walls.
    finalMaze = vector<vector<int>>(height, vector<int>(width, WALL_ID));

    // Define the random start position.
    int startX = rand() % (width / 2) * 2 + 1;
    int startY = rand() % (height / 2) * 2 + 1;

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
      int currentCellIndex = rand() % cells.size();
      Cell currentCell = cells[currentCellIndex];

      // Find all valid neighbors.
      vector<pair<int, int>> validNeighbors;
      for (const auto& move : moves) {
        int newX = currentCell.x + move.offsetX;
        int newY = currentCell.y + move.offsetY;

        // Check if the neighbor is valid.
        if (isValid(newX, newY)) {
          int betweenX = (currentCell.x + newX) / 2;
          int betweenY = (currentCell.y + newY) / 2;

          // If the cell between the current cell and the neighbor is a wall, then the neighbor is valid.
          if (finalMaze[betweenY][betweenX] == WALL_ID) {
            validNeighbors.emplace_back(newX, newY);
          }
        }

        // Increment the number of iterations to generate the maze.
        iterationsToGenerate++;
      }

      // Check if there are any valid neighbors.
      if (!validNeighbors.empty()) {
        // Choose a random neighbor.
        int randomNeighborIndex = rand() % validNeighbors.size();
        int newX = validNeighbors[randomNeighborIndex].first;
        int newY = validNeighbors[randomNeighborIndex].second;

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

    // Set the start and end positions.
    finalMaze[1][0] = START_ID;
    finalMaze[height - 2][width - 1] = END_ID;

    // Add the final maze state to the list of maze states.
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

  void distributeCheckpoints() {
    // Get the number of path cells.
    int pathCellsCount = 0;
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
        // Check if the cell is a path.
        if (finalMaze[y][x] == PATH_ID) {
          // Increment the number of path cells.
          pathCellsCount++;
        }

        // Increment the number of iterations to generate the maze.
        iterationsToGenerate++;
      }
    }

    // Calculate the number of checkpoints.
    int checkpointsCount = pathCellsCount * checkpointsPercentage / 100;

    // Distribute the checkpoints.
    int currentCheckpointsCount = 0;
    while (currentCheckpointsCount < checkpointsCount) {
      int x = rand() % width;
      int y = rand() % height;
      if (finalMaze[y][x] == PATH_ID) {
        finalMaze[y][x] = CHECKPOINT_ID;
        currentCheckpointsCount++;
      }
    }

    // Add the final maze state to the list of maze states.
    generationSteps.push_back(finalMaze);
  }

  void generateSolution() {
    // TODO: Implement the solution generation.

    // Notes:
    // - The solution must be the shortest path from the start to the end, and must pass through all the checkpoints.
    // - The passed path cells must be marked with the PASSED_PATH_ID value.
    // - The passed checkpoints must be marked with the PASSED_CHECKPOINT_ID value.
  }

  void filterSteps() {
    vector<vector<vector<int>>> filteredSteps;
    filteredSteps.push_back(generationSteps[0]);

    for (int i = 1; i < generationSteps.size(); i++) {
        if (generationSteps[i] != generationSteps[i-1]) {
          filteredSteps.push_back(generationSteps[i]);
        }
    }
    generationSteps = filteredSteps;
  }

  void validateInputParameters() {
    // Check if the width is more than or equal to 4.
    if (width < 4) {
      cout << "The width must at least 4. Exiting...\n";
      exit(1);
    }

    // Check if the height is more than or equal to 4.
    if (height < 4) {
      cout << "The height must at least 4. Exiting...\n";
      exit(1);
    }

    // Check if the width is odd.
    if (width % 2 == 0) {
      width++;
    }

    // Check if the height is odd.
    if (height % 2 == 0) {
      height++;
    }

    // Check if the checkpoints percentage is between 0 and 100.
    if (checkpointsPercentage < 0 || checkpointsPercentage > 100) {
      cout << "The checkpoints percentage must be between 0 and 100. Exiting...\n";
      exit(1);
    }
  }

  // Method that checks if a cell is valid.
  bool isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height && finalMaze[y][x] == WALL_ID;
  }

  // Method that visualizes the maze generation.
  void visualizeMazeGeneration(unsigned int visualizationDurationMs) {
    // Check if the visualization duration is not 0.
    if (visualizationDurationMs != 0) {
      // Prompt the user to visualize the maze generation.
      cout << "Maze has been generated!\n\n";
      cout << "Would you like to visualize the maze generation? (y/n):\n";
      string answer;
      cout << "--> ";
      cin >> answer;
      cout << "\n";

      // Check if the user wants to visualize the maze generation.
      if (answer != "y" && answer != "Y") {
        visualizationDurationMs = 0;
      }
    }

    // If the visualization duration is 0, then print the final maze state instantly.
    if (visualizationDurationMs == 0) {
      printMazeState(finalMaze);
      cout << "\nMaze generation completed!\n";
      cout << "Took " << timePerformanceMs << " ms and " << iterationsToGenerate << " iterations to generate.\n";
      return;
    }

    // Define the delay between each step.
    const unsigned int delay = visualizationDurationMs / generationSteps.size();

    // Set steps counter.
    unsigned int stepsCount = 0;

    // Set percentage.
    unsigned int percentage;

    // Loop through the maze states.
    for (const auto& step : generationSteps) {
      // Clear the console.
      system(CONSOLE_CLEAR_COMMAND.c_str());

      // Increment the steps counter.
      stepsCount++;

      // Calculate the percentage.
      percentage = (stepsCount * 100) / generationSteps.size();

      // Print the maze state.
      printMazeState(step);

      // Check if the maze generation is completed.
      if (percentage == 100) {
        cout << "\nMaze generation visualization completed!\n";
        cout << "Took " << timePerformanceMs << " ms and " << iterationsToGenerate << " iterations to generate.\n";
      } else {
        // Print the generation status.
        cout << "\nMaze is being visualized: " << percentage << "%\n";
        cout << "Step: " << stepsCount << " of " << generationSteps.size() << "\n";
        cout << "Time remaining: " << (delay * (generationSteps.size() - stepsCount)) / 1000 << " seconds.\n";
        cout << "\nPress CTRL + C to stop the visualization.\n";
      }

      // Sleep for the delay.
      this_thread::sleep_for(chrono::milliseconds(delay));
    }
  }

  // Method that prints the maze state.
  void printMazeState(const vector<vector<int>>& mazeState) {
    for (const auto& row : mazeState) {
      for (const auto& cell : row) {
        switch (cell) {
          case WALL_ID:
            cout << WALL_SYMBOL;
            break;
          case PATH_ID:
            cout << PATH_SYMBOL;
            break;
          case CHECKPOINT_ID:
            cout << CHECKPOINT_SYMBOL;
            break;
          case PASSED_CHECKPOINT_ID:
            cout << PASSED_CHECKPOINT_SYMBOL;
            break;
          case PASSED_PATH_ID:
            cout << PASSED_PATH_SYMBOL;
            break;
          case START_ID:
            cout << START_SYMBOL;
            break;
          case END_ID:
            cout << END_SYMBOL;
            break;
          default:
            cout << "E ";
        }
      }
      cout << "\n";
    }
  }
};

int main() {
  Maze maze(35, 35, 10);
  maze.visualizeMazeGeneration(5000);

  return 0;
}
