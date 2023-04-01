#include <iostream>
#include <utility>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <map>
#include <random>

using namespace std;

// Define default maze generation parameters.
const unsigned int DEFAULT_MAZE_WIDTH = 21;
const unsigned int DEFAULT_MAZE_HEIGHT = 21;
const unsigned int DEFAULT_CHECKPOINTS_PERCENTAGE = 10;

// Define the matrix cell types IDs.
enum MazeCellTypeIds {
  PATH_ID = 0,
  WALL_ID = 1,
  CHECKPOINT_ID = 2,
  PASSED_PATH_ID = 3,
  PASSED_CHECKPOINT_ID = 4,
  START_ID = 5
};

// Define the symbols used to represent the matrix cell types.
const string WALL_SYMBOL = "██";
const string PATH_SYMBOL = "  ";
const string CHECKPOINT_SYMBOL = "* ";
const string PASSED_PATH_SYMBOL = "• ";
const string PASSED_CHECKPOINT_SYMBOL = "+ ";
const string START_SYMBOL = "ST";

// Define progress bar symbols.
const string PROGRESS_BAR_FULL_SYMBOL = "█";
const string PROGRESS_BAR_EMPTY_SYMBOL = "░";

// Define other constants.
const unsigned int ESTIMATED_TIME_UPDATE_INTERVAL_STEPS = 10;
const unsigned int MAZE_GENERATION_VISUALIZATION_MIN_DURATION_MS = 5000;

// Random number generator.
unsigned int randomGenerator() {
  // Create a Mersenne Twister random number generator seeded with current time.
  mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());

  // Create a uniform distribution to generate integers between 0 and RAND_MAX.
  uniform_int_distribution<int> dist(0, RAND_MAX);

  // Return a random number.
  return dist(gen);
}

// Structure that represents a cell.
struct Cell {
  // Position in the X and Y axis.
  unsigned int x, y;

  // Constructor.
  Cell(int _x, int _y) : x(_x), y(_y) {}
};

// Structure that represents a cell direction.
struct Direction {
  // Offset in the X and Y axis.
  int offsetX, offsetY;

  // Constructor.
  Direction(int _offsetX, int _offsetY) : offsetX(_offsetX), offsetY(_offsetY) {}
};

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
  long long timePerformanceMs = 0;
  long long iterationsTookToGenerate = 0;
  long long estimatedTimeLeftMs = 0;

 public:
  // Constructor.
  Maze(unsigned int _width, unsigned int _height, unsigned int _checkpointsPercentage) {
    this->width = _width;
    this->height = _height;
    this->checkpointsPercentage = _checkpointsPercentage;
    generateMaze();
  }

  // Method that generates the maze.
  void generateMaze() {
    // Clear the console.
    clearConsole();

    // Validate the input.
    validateInputParameters();

    // Print the maze generation parameters.
    cout << "Maze parameters:" << "\n";
    cout << "- Width: " << width << "\n";
    cout << "- Height: " << height << "\n";
    cout << "- Checkpoints percentage: " << checkpointsPercentage << "%" << "\n\n";

    // Wait for user input.
    cout << "Press enter key to start the maze generation..." << "\n";
    cin.get();

    // Print the maze generation header.
    cout << "Generating maze..." << "\n\n";

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
  void distributeCheckpoints() {
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

  // Method that generates the solution.
  void generateSolution() {
    // TODO: Implement the solution generation.

    // Notes:
    // - The solution must be the shortest path from the start to the end, and must pass through all the checkpoints.
    // - The passed path cells must be marked with the PASSED_PATH_ID value.
    // - The passed checkpoints must be marked with the PASSED_CHECKPOINT_ID value.
  }

  // Method that filters out the steps where anything is not changing.
  void filterSteps() {
    vector<vector<vector<unsigned int>>> filteredSteps;
    filteredSteps.push_back(generationSteps[0]);

    for (unsigned int i = 1; i < generationSteps.size(); i++) {
        if (generationSteps[i] != generationSteps[i-1]) {
          filteredSteps.push_back(generationSteps[i]);
        }
    }
    generationSteps = filteredSteps;
  }

  // Method that checks if the given input parameters are valid.
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
  void visualizeMazeGeneration(unsigned int minVisualizationDurationMs) {
    // Prompt the user to visualize the maze generation.
    cout << "Maze has been generated in " << millisecondsToTimeString(timePerformanceMs) << "!\n\n";
    cout << "Would you like to visualize the maze generation? (Y/N):\n";
    string answer;
    cout << "--> ";
    cin >> answer;
    cout << "\n";

    // Check if the user wants to visualize the maze generation.
    if (answer != "y" && answer != "Y") {
      printMazeState(finalMaze);
      cout << "\nMaze generation completed!\n";
      cout << "Took " << millisecondsToTimeString(timePerformanceMs) << " (" << iterationsTookToGenerate << " iterations) to generate.\n";
      return;
    }

    // Clear the console.
    clearConsole();

    // Define the delay between each step.
    const unsigned int delay = minVisualizationDurationMs / generationSteps.size();

    // Set steps counter.
    unsigned int stepsCount = 0;

    // Set percentage.
    unsigned int percentage;

    // Set the starting time point.
    auto timeToVisualizePercentageOfStepsStartMs = chrono::high_resolution_clock::now();

    // Set the ending time point.
    auto timeToVisualizePercentageOfStepsEndMs = chrono::high_resolution_clock::now();

    // Set default time remaining.
    estimatedTimeLeftMs = -1;

    // Loop through the maze states.
    for (const auto& step : generationSteps) {
      // Clear the console.
      clearConsole();

      // Increment the steps counter.
      stepsCount++;

      // Check if the steps counter is a multiple of the update each steps.
      if (stepsCount % ESTIMATED_TIME_UPDATE_INTERVAL_STEPS == 0) {
          // Stop the timer.
          timeToVisualizePercentageOfStepsEndMs = chrono::high_resolution_clock::now();

          // Calculate steps left.
          const unsigned int stepsLeft = generationSteps.size() - stepsCount;

          // Update the estimated time left.
          estimatedTimeLeftMs = chrono::duration_cast<chrono::milliseconds>(
              timeToVisualizePercentageOfStepsEndMs - timeToVisualizePercentageOfStepsStartMs
              ).count() * stepsLeft / ESTIMATED_TIME_UPDATE_INTERVAL_STEPS;

          // Set the starting time point.
          timeToVisualizePercentageOfStepsStartMs = chrono::high_resolution_clock::now();
      }

      // Calculate the percentage.
      percentage = (stepsCount * 100) / generationSteps.size();

      // Print the maze state.
      printMazeState(step);

      // Check if the maze generation is completed.
      if (percentage == 100) {
        cout << "\nMaze generation visualization completed!\n\n";
        cout << "Took " << millisecondsToTimeString(timePerformanceMs) << " (" << iterationsTookToGenerate << " iterations) to generate.\n";
      } else {
        // Print the generation status.
        cout << "\nMaze generation is being visualized: " << percentage << "%\n";
        cout << generateProgressBarString(percentage) << "\n\n";
        cout << "Step: " << stepsCount << " of " << generationSteps.size() << "\n";
        if (estimatedTimeLeftMs > 0) {
            cout << "Estimated time remaining: " << millisecondsToTimeString(estimatedTimeLeftMs) << ".\n\n";
            } else {
            cout << "Estimated time remaining: Calculating...\n\n";
        }
        cout << "Press CTRL + C to stop the visualization.\n";
      }

      // Sleep for the delay.
      if (minVisualizationDurationMs > 0) {
        this_thread::sleep_for(chrono::milliseconds(delay));
      }
    }
  }

  // Method that prints the maze state.
  static void printMazeState(const vector<vector<unsigned int>>& mazeState) {
    string output;
    for (const auto& row : mazeState) {
      for (const auto& cell : row) {
        switch (cell) {
          case WALL_ID:
            output += colorString(WALL_SYMBOL, "white", "white", "default");
            break;
          case PATH_ID:
            output += colorString(PATH_SYMBOL, "green", "black", "default");
            break;
          case CHECKPOINT_ID:
            output += colorString(CHECKPOINT_SYMBOL, "yellow", "black", "bold");
            break;
          case PASSED_CHECKPOINT_ID:
            output += colorString(PASSED_CHECKPOINT_SYMBOL, "white", "green", "bold");
            break;
          case PASSED_PATH_ID:
            output += colorString(PASSED_PATH_SYMBOL, "green", "black", "default");
            break;
          case START_ID:
            output += colorString(START_SYMBOL, "white", "green", "bold");
            break;
          default:
            output += "E ";
        }
      }
      output += "\n";
    }
    fwrite(output.c_str(), sizeof(char), output.size(), stdout);
  }

  // Methods that colors the string.
  static string colorString(
      const string& str,
      const string& color = "default",
      const string& backgroundColor = "default",
      const string& style = "default"
  ) {
    // Define prefix and suffix.
    string prefix = "\033[";
    string suffix = "\033[0m";

    // Define the color codes.
    map<string, string> colors = {
      {"black", "30"},
      {"red", "31"},
      {"green", "32"},
      {"yellow", "33"},
      {"blue", "34"},
      {"magenta", "35"},
      {"cyan", "36"},
      {"white", "37"}
    };

    // Define the background color codes.
    map<string, string> backgroundColors = {
      {"black", "40"},
      {"red", "41"},
      {"green", "42"},
      {"yellow", "43"},
      {"blue", "44"},
      {"magenta", "45"},
      {"cyan", "46"},
      {"white", "47"}
    };

    // Define the style codes.
    map<string, string> styles = {
      {"bold", "1"},
      {"underline", "4"},
      {"blink", "5"},
      {"invert", "7"}
    };

    // Define the codes vector.
    vector<string> codes;

    // Check if the color is valid.
    if (colors.find(color) != colors.end()) {
      codes.push_back(colors[color]);
    }

    // Check if the background color is valid.
    if (backgroundColors.find(backgroundColor) != backgroundColors.end()) {
      codes.push_back(backgroundColors[backgroundColor]);
    }

    // Check if the style is valid.
    if (styles.find(style) != styles.end()) {
      codes.push_back(styles[style]);
    }

    // Check if there are no codes.
    if (codes.empty()) {
      return str;
    }

    // Join the codes, prefix and suffix.
    string output = prefix;
    for (unsigned int i = 0; i < codes.size(); i++) {
      output += codes[i];
      if (i != codes.size() - 1) {
        output += ";";
      }
    }
    output += "m" + str + suffix;

    // Return the output.
    return output;
  }

  // Method that clears the console.
  static void clearConsole() {
    #if _WIN32
        system("cls");
    #else
        cout << "\033[2J\033[1;1H";
    #endif
  }

  // Method that converts milliseconds to time string.
  static string millisecondsToTimeString(unsigned long long milliseconds) {
    // Define the time string.
    string timeString;

    // Calculate the hours.
    unsigned int hours = int(milliseconds / 3600000);

    // Calculate the minutes.
    unsigned int minutes = int((milliseconds % 3600000) / 60000);

    // Calculate the seconds.
    unsigned int seconds = int(((milliseconds % 3600000) % 60000) / 1000);

    // Calculate the milliseconds.
    unsigned int ms = int(((milliseconds % 3600000) % 60000) % 1000);

    // Check if the hours, minutes and seconds are 0.
    if (hours == 0 && minutes == 0 && seconds == 0) {
      return to_string(ms) + "ms";
    }

    // Check if the hours are greater than 0.
    if (hours > 0) {
      timeString += to_string(hours) + "h ";
    }

    // Check if the minutes are greater than 0.
    if (minutes > 0) {
      timeString += to_string(minutes) + "m ";
    }

    // Check if the seconds are greater than 0.
    if (seconds > 0) {
      // Get first two digits of the milliseconds.
      string msStr = to_string(ms);
      if (msStr.size() == 1) {
        msStr = "0" + msStr;
      } else if (msStr.size() > 2) {
        msStr = msStr.substr(0, 2);
      }

      // Add the seconds and milliseconds to the time string.
      timeString += to_string(seconds) + "." + msStr + "s ";
    }

    // Trim last space.
    if (timeString[timeString.size() - 1] == ' ') {
      timeString = timeString.substr(0, timeString.size() - 1);
    }

    // Return the time string.
    return timeString;
  }

  // Method that generates a progress bar string.
  [[nodiscard]] string generateProgressBarString(unsigned int percentage) const {
    // Define the progress bar string.
    string progressBarString;

    // Check if the percentage is greater than 100.
    if (percentage > 100) {
      percentage = 100;
    }

    // Calculate max number of blocks.
    unsigned int maxNumberOfBlocks = width * 2;

    // Calculate the number of blocks.
    unsigned int numberOfBlocks = int(maxNumberOfBlocks * (percentage / 100.0));

    // Calculate the number of empty blocks.
    unsigned int numberOfEmptyBlocks = maxNumberOfBlocks - numberOfBlocks;

    // Add the blocks to the progress bar string.
    for (unsigned int i = 0; i < numberOfBlocks; i++) {
      progressBarString += PROGRESS_BAR_FULL_SYMBOL;
    }

    // Add the empty blocks to the progress bar string.
    for (unsigned int i = 0; i < numberOfEmptyBlocks; i++) {
      progressBarString += PROGRESS_BAR_EMPTY_SYMBOL;
    }

    // Return the progress bar string.
    return progressBarString;
  }
};

// Main function.
int main(int argc, char** argv) {
  // Define the default maze width, height and checkpoints percentage.
  unsigned int mazeWidth = DEFAULT_MAZE_WIDTH;
  unsigned int mazeHeight = DEFAULT_MAZE_HEIGHT;
  unsigned int checkpointsPercentage = DEFAULT_CHECKPOINTS_PERCENTAGE;

  // Check if the number of arguments is correct.
  if (argc == 4) {
    // Get the maze width.
    mazeWidth = stoi(argv[1]);

    // Get the maze height.
    mazeHeight = stoi(argv[2]);

    // Get the number of checkpoints.
    checkpointsPercentage = stoi(argv[3]);
  }

  // Create the maze.
  Maze maze(mazeWidth, mazeHeight, checkpointsPercentage);

  // Visualize the maze generation.
  maze.visualizeMazeGeneration(MAZE_GENERATION_VISUALIZATION_MIN_DURATION_MS);

  return 0;
}
