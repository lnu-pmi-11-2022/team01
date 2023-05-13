#include "maze.h"

// Constructor.
Maze::Maze(unsigned int _width, unsigned int _height, unsigned int _checkpointsPercentage) {
  this->width = _width;
  this->height = _height;
  this->checkpointsPercentage = _checkpointsPercentage;
  generateMaze();
}

// Method that visualizes the maze generation.
void Maze::visualizeMazeGeneration(unsigned int minVisualizationDurationMs) {
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
      cout << generateProgressBarString(percentage, width * 2) << "\n\n";
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
