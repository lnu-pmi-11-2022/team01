#include "maze.h"

// Constructor.
Maze::Maze(unsigned int _width, unsigned int _height, unsigned int _checkpointsValue, CheckpointSettingType _checkpointSettingType, SupportedSolvingAlgorithms _solvingAlgorithm, string _executablePath) {
  this->width = _width;
  this->height = _height;
  this->checkpointsValue = _checkpointsValue;
  this->checkpointSettingType = _checkpointSettingType;
  this->solvingAlgorithm = _solvingAlgorithm;
  this->executablePath = std::move(_executablePath);
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

    // Prompt user whether to save the maze report.
    saveMazeReport();

    // Prompt user whether to save the maze generation steps as JSON.
    saveMazeGenerationStepsAsJson();

    return;
  }

  // Start playing the background audio.
  SoundBuffer buffer;
  buffer.loadFromFile(getFilePath(executablePath, VISUALIZATION_BG_AUDIO_FILE_PATH));

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

  // Stop the audio and join the audio thread
  stopFlag.store(true);
  soundThread.join();

  // Prompt user whether to save the maze report.
  saveMazeReport();

  // Prompt user whether to save the maze generation steps as JSON.
  saveMazeGenerationStepsAsJson();
}

// Method that saves the maze report to a file.
void Maze::saveMazeReport() {
  // Prompt user whether to save the maze report.
  cout << "\n" << colorString("Would you like to save the maze report to a file?", "yellow", "black", "bold") << " (" << colorString("Y", "green", "default", "bold") << "/" << colorString("N", "red", "default", "bold") << "):\n";
  string answer;
  cout << colorString("-->", "yellow", "black", "bold") << " ";
  cin >> answer;

  // Check if the user wants to save the maze report.
  if (answer != "y" && answer != "Y") {
    return;
  }

  // Generate the file name from the current time.
  tm *ltm = localtime(&generationTimestamp);
  string fileName = "_report_" + to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + "-" + to_string(ltm->tm_min) + "-" + to_string(ltm->tm_sec) + ".txt";
  string filePath = executablePath + fileName;

  // Create the file.
  ofstream file;
  file.open(filePath);

  // Write the maze report to the file.
  file << generateMazeReportFile();

  // Close the file.
  file.close();

  // Print the success message.
  cout << "\n" << colorString("Maze report saved successfully!", "green", "black", "bold") << "\n";

  // Print the file path.
  cout << "File path: " << colorString(filePath, "yellow", "black", "bold") << "\n";
}

// Method that saves the maze generation steps as a JSON to a file.
void Maze::saveMazeGenerationStepsAsJson() {
  // Prompt user whether to save the maze generation steps as a JSON.
  cout << "\n" << colorString("Would you like to save the maze visualization steps as a JSON to a file?", "yellow", "black", "bold") << " (" << colorString("Y", "green", "default", "bold") << "/" << colorString("N", "red", "default", "bold") << "):\n";
  string answer;
  cout << colorString("-->", "yellow", "black", "bold") << " ";
  cin >> answer;

  // Check if the user wants to save the maze generation steps as a JSON.
  if (answer != "y" && answer != "Y") {
    return;
  }

  // Generate the file name from the current time.
  tm *ltm = localtime(&generationTimestamp);
  string fileName = "_json_" + to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + "-" + to_string(ltm->tm_min) + "-" + to_string(ltm->tm_sec) + ".json";
  string filePath = executablePath + fileName;
  string minifiedFileName = "_json_" + to_string(1900 + ltm->tm_year) + "-" + to_string(1 + ltm->tm_mon) + "-" + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + "-" + to_string(ltm->tm_min) + "-" + to_string(ltm->tm_sec) + ".min.json";
  string minifiedFilePath = executablePath + minifiedFileName;

  // Create the file.
  ofstream file;
  file.open(filePath);

  // Write the maze generation steps to the file.
  file << generateMazeGenerationStepsFile(false);

  // Close the file.
  file.close();

  // Create the minified file.
  ofstream minifiedFile;
  minifiedFile.open(minifiedFilePath);

  // Write the maze generation steps to the minified file.
  minifiedFile << generateMazeGenerationStepsFile(true);

  // Close the minified file.
  minifiedFile.close();

  // Print the success message.
  cout << "\n" << colorString("Maze generation steps saved successfully!", "green", "black", "bold") << "\n";

  // Print the file paths.
  cout << "File path: " << colorString(filePath, "yellow", "black", "bold") << "\n";
  cout << "Minified file path: " << colorString(minifiedFilePath, "yellow", "black", "bold") << "\n";
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
