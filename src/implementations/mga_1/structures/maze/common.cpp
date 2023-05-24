#include "maze.h"

// Constructor.
Maze::Maze(unsigned int _width, unsigned int _height, unsigned int _checkpointsValue, CheckpointSettingType _checkpointSettingType, SupportedSolvingAlgorithms _solvingAlgorithm) {
  this->width = _width;
  this->height = _height;
  this->checkpointsValue = _checkpointsValue;
  this->checkpointSettingType = _checkpointSettingType;
  this->solvingAlgorithm = _solvingAlgorithm;
  generateMaze();
}

// Method that visualizes the maze generation.
void Maze::visualizeMazeGeneration(unsigned int minVisualizationDurationMs) {
  // Prompt the user to visualize the maze generation.
  cout << colorString("Maze has been generated in ", "green", "black", "bold") << colorString(millisecondsToTimeString(timePerformanceMs), "green", "black", "bold") << "!\n\n";
  cout << colorString("Would you like to visualize the maze generation?", "yellow", "black", "bold") << " (" << colorString("Y", "green", "default", "bold") << "/" << colorString("N", "red", "default", "bold") << "):\n";
  string answer;
  cout << colorString("-->", "yellow", "black", "bold") << " ";
  cin >> answer;
  cout << "\n";

  // Check if the user wants to visualize the maze generation.
  if (answer != "y" && answer != "Y") {
    clearConsole();
    printMazeState(finalMaze);

    cout << "\n" << colorString("Maze generation completed!", "green", "black", "bold") << "\n\n";

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

    // Print the maze generation statistics.
    if (minPathLength > 0 || actualNumberOfCheckpoints > 0) {
      cout << colorString("Maze statistics:", "yellow", "black", "bold") << "\n";
    }
    if (minPathLength > 0) {
      cout << "  - Minimum path length: " << minPathLength << " cells.\n";
    }
    if (actualNumberOfCheckpoints > 0) {
      cout << "  - Number of checkpoints: " << actualNumberOfCheckpoints << ".\n\n";
    }
    cout << "Took " << millisecondsToTimeString(timePerformanceMs) << " (" << splitNumberIntoBlocks(iterationsTookToGenerate) << " iterations) to generate.\n";
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
      cout << "\n" << colorString("Maze generation visualization completed!", "green", "black", "bold") << "\n\n";

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

      // Print the maze generation statistics.

      // Print the maze generation statistics.
      if (minPathLength > 0 || actualNumberOfCheckpoints > 0) {
        cout << colorString("Maze statistics:", "yellow", "black", "bold") << "\n";
      }
      if (minPathLength > 0) {
        cout << "  - Minimum path length: " << minPathLength << " cells.\n";
      }
      if (actualNumberOfCheckpoints > 0) {
        cout << "  - Number of checkpoints: " << actualNumberOfCheckpoints << ".\n\n";
      }
      cout << "Took " << millisecondsToTimeString(timePerformanceMs) << " (" << splitNumberIntoBlocks(iterationsTookToGenerate) << " iterations) to generate.\n";
    } else {
      // Print the generation status.
      cout << "\n" << colorString("Maze generation is being visualized: ", "yellow", "black", "bold") << colorString(to_string(percentage), "yellow", "black", "bold") << colorString("%", "yellow", "black", "bold") << "\n";
      cout << generateProgressBarString(percentage, width * 2) << "\n\n";
      cout << "Step: " << stepsCount << " of " << generationSteps.size() << "\n";
      if (estimatedTimeLeftMs > 0) {
        cout << "Estimated time remaining: " << millisecondsToTimeString(estimatedTimeLeftMs) << ".\n\n";
      } else {
        cout << "Estimated time remaining: Calculating...\n\n";
      }
      cout << colorString("Press CTRL + C to stop the visualization.", "red", "black", "bold") << "\n";
    }

    // Sleep for the delay.
    if (minVisualizationDurationMs > 0) {
      this_thread::sleep_for(chrono::milliseconds(delay));
    }
  }
}

// Method that filters out the steps where anything is not changing.
void Maze::filterSteps() {
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
void Maze::validateInputParameters() {
  // Check if the width is odd.
  if (width % 2 == 0) {
    width++;
  }

  // Check if the height is odd.
  if (height % 2 == 0) {
    height++;
  }
}
