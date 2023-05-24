#include "maze.h"

// Method that generates the maze.
void Maze::generateMaze() {
  // Validate the input.
  validateInputParameters();

  // Set the generation timestamp.
  generationTimestamp = time(nullptr);

  // Print the maze generation parameters.
  cout << colorString("Maze parameters:", "yellow", "black", "bold") << "\n";
  cout << "  - Width: " << width << "\n";
  cout << "  - Height: " << height << "\n";
  switch (checkpointSettingType) {
    case CheckpointSettingType::NUMBER:
      cout << "  - Number of checkpoints: " << checkpointsValue << "\n";
      break;
    case CheckpointSettingType::PERCENTAGE:
      cout << "  - Checkpoints percentage: " << checkpointsValue << "%" << "\n";
      break;
  }
  cout << "  - Solving algorithm: " << getSolvingAlgorithmName() << "\n\n";

  // Wait for user input.
  waitForEnter(colorString("Press the \"Enter\" key to start the maze generation...", "green", "black", "bold"));

  // Print a warning if the maze is large.
  if (width * height > MAZE_WARNING_THRESHOLD_AREA) {
    cout << "\n" << colorString("WARNING: The maze is large, so the generation may take a while.", "red", "black", "bold") << "\n";
  }

  // Start playing the background audio.
  SoundBuffer buffer;
  buffer.loadFromFile(getFilePath(executablePath, GENERATION_BG_AUDIO_FILE_PATH));

  // Create a sound and play it in a loop in a separate thread
  Sound sound(buffer);
  sound.setLoop(true);
  atomic<bool> stopFlag(false);
  thread soundThread([&]() {
    while (!stopFlag.load()) {
      sound.play();
      while (sound.getStatus() == Sound::Playing && !stopFlag.load()) {
        // Wait for the sound to finish playing or for stopFlag to be set
      }
      sound.stop();
    }
  });

  // Print the maze generation header.
  cout << "\n" << colorString("Generating maze...", "yellow", "black", "bold") << "\n";

  // Start the timer.
  auto startTime = chrono::high_resolution_clock::now();
  auto stepStartTime = chrono::high_resolution_clock::now();

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
      if (isValidWall(newX, newY)) {
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
  cout << colorString("DONE!", "green", "black", "bold");
  unsigned long long timePerformance = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - stepStartTime).count();
  cout << colorString(" (Took " + millisecondsToTimeString(timePerformance) + ")", "white", "black", "bold") << "\n\n";
  stepStartTime = chrono::high_resolution_clock::now();

  // Check if there are any checkpoints.
  unsigned int numberOfCheckpoints = 0;
  if (checkpointsValue != 0) {
    // Distribute the checkpoints.
    cout << colorString("Distributing the checkpoints...", "yellow", "black", "bold") << "\n";
    numberOfCheckpoints = distributeCheckpoints();
    cout << colorString("DONE! (" + to_string(numberOfCheckpoints) + " checkpoints)", "green", "black", "bold");
    timePerformance = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - stepStartTime).count();
    cout << colorString(" (Took " + millisecondsToTimeString(timePerformance) + ")", "white", "black", "bold") << "\n\n";

    if (numberOfCheckpoints < requestedNumberOfCheckpoints) {
      cout << colorString("The number of checkpoints was decreased from " + to_string(requestedNumberOfCheckpoints) + " to the maximum allowed " + to_string(numberOfCheckpoints) + " for this maze.", "white", "red", "bold") << "\n\n";
    }

    if (requestedNumberOfCheckpoints == 1) {
      cout << colorString("The number of checkpoints was increased from 1, which is unsupported, to 2.", "white", "red", "bold") << "\n\n";
    }
  }

  // Check if there are any checkpoints.
  if (numberOfCheckpoints > 0 && solvingAlgorithm != SupportedSolvingAlgorithms::NONE) {
    // Generate the solution.
    generateSolution();
  }

  // Filter out the steps where the path is not changing.
  stepStartTime = chrono::high_resolution_clock::now();
  cout << colorString("Filtering frames...", "yellow", "black", "bold") << "\n";
  filterSteps();
  cout << colorString("DONE!", "green", "black", "bold");
  timePerformance = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - stepStartTime).count();
  cout << colorString(" (Took " + millisecondsToTimeString(timePerformance) + ")", "white", "black", "bold") << "\n\n";

  // Stop the timer.
  auto endTime = chrono::high_resolution_clock::now();

  // Calculate the time performance.
  timePerformanceMs = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();

  // Stop the audio and join the audio thread
  stopFlag.store(true);
  soundThread.join();
}

// Method that randomly distributes checkpoints on the maze.
unsigned int Maze::distributeCheckpoints() {
  // Get the number of path cells.
  unsigned int pathCellsCount = getTheNumberOfCells(PATH_ID);

  // Calculate the requested number of checkpoints.
  switch (checkpointSettingType) {
    case CheckpointSettingType::NUMBER:
      requestedNumberOfCheckpoints = checkpointsValue;
      break;
    case CheckpointSettingType::PERCENTAGE:
      requestedNumberOfCheckpoints = max((int)(pathCellsCount * checkpointsValue / 100), (int)MAZE_MIN_CHECKPOINTS_NUMBER);
      break;
    default:
      break;
  }

  // Get the maximum number of checkpoints.
  unsigned int maxNumberOfCheckpoints;
  if (solvingAlgorithm == SupportedSolvingAlgorithms::HELD_KARP_PARALLEL || solvingAlgorithm == SupportedSolvingAlgorithms::HELD_KARP) {
    maxNumberOfCheckpoints = min(MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP, pathCellsCount);
  } else if (solvingAlgorithm == SupportedSolvingAlgorithms::BRUTE_FORCE) {
    maxNumberOfCheckpoints = min(MAZE_MAX_CHECKPOINTS_NUMBER_BRUTE_FORCE, pathCellsCount);
  } else {
    maxNumberOfCheckpoints = pathCellsCount;
  }

  // Calculate the number of checkpoints.
  unsigned int checkpointsCount = min(requestedNumberOfCheckpoints, maxNumberOfCheckpoints);

  // Prevent the number of checkpoints from being 1.
  if (checkpointsCount == 1) {
    checkpointsCount = 2;
  }

  // Distribute the checkpoints.
  unsigned int currentCheckpointsCount = 0;
  while (currentCheckpointsCount < checkpointsCount) {
    unsigned int x = randomGenerator() % width;
    unsigned int y = randomGenerator() % height;
    if (finalMaze[y][x] == PATH_ID) {
      finalMaze[y][x] = CHECKPOINT_ID;
      currentCheckpointsCount++;

      // Add the maze state to the list of maze states.
      generationSteps.push_back(finalMaze);
    }
  }

  // Set the actual number of checkpoints.
  actualNumberOfCheckpoints = checkpointsCount;

  // Return the number of checkpoints.
  return checkpointsCount;
}
