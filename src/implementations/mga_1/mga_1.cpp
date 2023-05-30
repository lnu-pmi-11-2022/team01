#include "mga_1.h"

// Function that prompts the user for a parameter.
unsigned int promptForParameter(const string& parameterName, unsigned int minValue, unsigned int maxValue) {
  // Initialize the parameter value.
  unsigned int parameterValue = 0;

  // Prompt the user for the parameter value until it is valid.
  while (true) {
    cout << colorString("Enter the " + parameterName + ":", "green", "black", "bold") << "\n";
    cout << colorString("-->", "yellow", "black", "bold") << " ";

    // Check if input is valid.
    if (!(cin >> parameterValue)) {
      cout << "\n" << colorString("Invalid input. Please try again.", "red", "black", "bold") << "\n\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear and ignore any invalid input
    } else if (parameterValue < minValue || parameterValue > maxValue) {
      cout << "\n" << colorString("The " + parameterName + " must be between " + to_string(minValue) + " and " + to_string(maxValue) + "!", "red", "black", "bold") << "\n\n";
    } else {
      break;
    }
  }

  // Print a new line.
  cout << "\n";

  // Return the parameter value.
  return parameterValue;
}

// Function that prompts the user for a choice.
template <typename T>
T promptForChoice(const string& message, vector<pair<T, string>> choices) {
  // Initialize the choice.
  int choice = -1;

  // List the choices.
  cout << colorString(message, "green", "black", "bold") << "\n";
  for (int i = 0; i < choices.size(); i++) {
    cout << "  " << i + 1 << ". " << choices[i].second << "\n";
  }

  // Prompt the user for the choice until it is valid.
  while (choice < 1 || choice > choices.size()) {
    cout << colorString("-->", "yellow", "black", "bold") << " ";
    cin >> choice;

    // Handle invalid input.
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "\n" << colorString("Invalid input. Please try again.", "red", "black", "bold") << "\n";
      choice = -1;
    } else if (choice < 1 || choice > choices.size()) {
      cout << "\n" << colorString("Invalid choice. Please try again.", "red", "black", "bold") << "\n";
    }
  }

  // Print a new line.
  cout << "\n";

  // Return the choice.
  return choices[choice - 1].first;
}

// Function that runs the maze generation algorithm.
void mga1(const string& executablePath) {
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

  // Prompt the user to choose the checkpoint setting type and a value.
  unsigned int checkpointsValue = 0;
  auto checkpointSetting = promptForChoice<CheckpointSettingType>("Choose the checkpoint setting type:", SUPPORTED_CHECKPOINT_SETTING_TYPES);
  switch (checkpointSetting) {
    case CheckpointSettingType::NUMBER:
      // Prompt the user to enter the maze checkpoints number.
      checkpointsValue = promptForParameter("checkpoints number", MAZE_MIN_CHECKPOINTS_SETTING, MAZE_MAX_CHECKPOINTS_SETTING);
      break;
    case CheckpointSettingType::PERCENTAGE:
      // Prompt the user to enter the maze checkpoints percentage.
      checkpointsValue = promptForParameter("checkpoints percentage", MAZE_MIN_CHECKPOINTS_SETTING, MAZE_MAX_CHECKPOINTS_PERCENTAGE);
      break;
  }

  // If the checkpoints value is greater than 0, prompt the user to choose the maze solving algorithm.
  SupportedSolvingAlgorithms solvingAlgorithm = SupportedSolvingAlgorithms::NONE;
  if (checkpointsValue > 0) {
    // Prompt the user to choose the maze solving algorithm.
    solvingAlgorithm = promptForChoice<SupportedSolvingAlgorithms>("Choose the maze solving algorithm:", SUPPORTED_SOLVING_ALGORITHMS);
  }

  // If the checkpoints value exceeds the maximum allowed for the chosen algorithm, print a warning message and decrease the value to the maximum allowed.
  if ((solvingAlgorithm == SupportedSolvingAlgorithms::HELD_KARP || solvingAlgorithm == SupportedSolvingAlgorithms::HELD_KARP_PARALLEL) && checkpointSetting == CheckpointSettingType::NUMBER && checkpointsValue > MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP) {
    cout << colorString("The number of checkpoints was decreased from " + to_string(checkpointsValue) + " to the maximum allowed " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP) + " for this algorithm.", "white", "red", "bold") << "\n\n";
    checkpointsValue = MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP;
  }
  if (solvingAlgorithm == SupportedSolvingAlgorithms::BRUTE_FORCE && checkpointSetting == CheckpointSettingType::NUMBER && checkpointsValue > MAZE_MAX_CHECKPOINTS_NUMBER_BRUTE_FORCE) {
    cout << colorString("The number of checkpoints was decreased from " + to_string(checkpointsValue) + " to the maximum allowed " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_BRUTE_FORCE) + " for this algorithm.", "white", "red", "bold") << "\n\n";
    checkpointsValue = MAZE_MAX_CHECKPOINTS_NUMBER_BRUTE_FORCE;
  }

  // Create the maze.
  Maze maze(mazeWidth, mazeHeight, checkpointsValue, checkpointSetting, solvingAlgorithm, executablePath);

  // Visualize the maze generation.
  maze.visualizeMazeGeneration(MAZE_GENERATION_VISUALIZATION_MIN_DURATION_MS);

  // Wait for the user to press the "Enter" key.
  waitForEnter("\n" + colorString("Press the \"Enter\" key to continue to the main menu...", "green", "black", "bold"));
}
