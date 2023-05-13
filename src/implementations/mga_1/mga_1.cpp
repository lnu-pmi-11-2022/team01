#include "mga_1.h"
#include "constants/constants.h"

// Function that prompts the user for a parameter.
unsigned int promptForParameter(const string& parameterName, unsigned int minValue, unsigned int maxValue) {
  // Initialize the parameter value.
  bool valid = false;
  unsigned int parameterValue;

  // Prompt the user for the parameter value until it is valid.
  while (!valid) {
    cout << colorString("Enter the " + parameterName + ":", "green", "black", "bold") << "\n";
    cout << colorString("--> ", "yellow", "black", "bold");
    cin >> parameterValue;

    // Validate the maze width.
    if (parameterValue < minValue || parameterValue > maxValue) {
      cout << "\n" << colorString("The " + parameterName + " must be between " + to_string(minValue) + " and " + to_string(maxValue) + "!", "red", "black", "bold") << "\n\n";
    } else {
      valid = true;
    }
  }

  // Print a new line.
  cout << "\n";

  // Return the parameter value.
  return parameterValue;
}

// Function that runs the maze generation algorithm.
void mga1() {
  // Clear the console.
  clearConsole();

  // Print the header.
  cout << colorString(
      " ██████   ██████   █████████    █████████             ████ \n"
      " ░██████ ██████   ███░░░░░███  ███░░░░░███           ░░███ \n"
      " ░███░█████░███  ███     ░░░  ░███    ░███            ░███ \n"
      " ░███░░███ ░███ ░███          ░███████████            ░███ \n"
      " ░███ ░░░  ░███ ░███    █████ ░███░░░░░███            ░███ \n"
      " ░███      ░███ ░░███  ░░███  ░███    ░███            ░███ \n"
      " █████     █████ ░░█████████  █████   █████ █████████ █████\n"
      " ░░░░░     ░░░░░   ░░░░░░░░░  ░░░░░   ░░░░░ ░░░░░░░░░ ░░░░░","green", "black", "bold") << "\n\n";
  cout << colorString("WELCOME TO THE \"MGA_1\" MAZE GENERATION ALGORITHM!", "green", "black", "bold") << "\n\n";
  cout << colorString("Here is the quick outline of the algorithm:", "yellow", "black", "bold") << "\n";
  cout << "    1. The maze starts as a grid of walls.\n"
          "    2. A single cell is randomly chosen as the starting point and marked as the path.\n"
          "        2.1. All neighboring cells of the starting cell that are not yet part of the path are considered valid neighbor cells. \n"
          "    3. One valid neighbor cell is randomly selected.\n"
          "    4. The wall between the current cell and the selected neighbor cell is removed, adding the neighbor cell to the path.\n"
          "    5. The neighbor cell is then added to the list of cells to be expanded.\n"
          "    6. The process continues till there are no expandable cells left. \n"
          "        6.1. This completes the basic maze generation.\n"
          "    7. Checkpoint cells are then added within the maze. \n"
          "    8. The shortest path from the starting point to the exit is found and marked as the solution path.\n\n";

  // Prompt the user to enter the maze width.
  unsigned int mazeWidth = promptForParameter("maze width", MAZE_MIN_WIDTH, MAZE_MAX_WIDTH);

  // Prompt the user to enter the maze height.
  unsigned int mazeHeight = promptForParameter("maze height", MAZE_MIN_HEIGHT, MAZE_MAX_HEIGHT);

  // Prompt the user to enter the maze checkpoints percentage.
  unsigned int checkpointsPercentage = promptForParameter("maze checkpoints percentage", MAZE_MIN_CHECKPOINTS_PERCENTAGE, MAZE_MAX_CHECKPOINTS_PERCENTAGE);

  // Create the maze.
  Maze maze(mazeWidth, mazeHeight, checkpointsPercentage);

  // Visualize the maze generation.
  maze.visualizeMazeGeneration(MAZE_GENERATION_VISUALIZATION_MIN_DURATION_MS);

  // Wait for the user to press the "Enter" key.
  waitForEnter("\n" + colorString("Press the \"Enter\" key to continue to the main menu...", "green", "black", "bold"));
}
