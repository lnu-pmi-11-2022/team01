#include "maze.h"

// Method that checks if a cell is valid wall.
bool Maze::isValidWall(int x, int y) {
  return x >= 0 && y >= 0 && x < width && y < height && finalMaze[y][x] == WALL_ID;
}

// Method that checks if a cell is valid path.
bool Maze::isValidPath(int x, int y) {
  return x >= 0 && y >= 0 && x < width && y < height && finalMaze[y][x] != WALL_ID;
}

// Method that gets all checkpoints from the maze.
vector<Cell> Maze::getCheckpoints() {
  // Declare the checkpoints vector.
  vector<Cell> checkpoints;

  // Get all checkpoints from the maze.
  for (int i = 0; i < finalMaze.size(); i++) {
    for (int j = 0; j < finalMaze[0].size(); j++) {
      if (finalMaze[i][j] == CHECKPOINT_ID) {
        checkpoints.emplace_back(j, i);
      }
    }
  }

  // Return the checkpoints vector.
  return checkpoints;
}

// Method that returns the checkpoint ID.
int Maze::getCheckpointId(Cell checkpoint) {
  // Get all checkpoints from the maze.
  vector<Cell> checkpoints = getCheckpoints();

  // Find the checkpoint ID.
  for (int i = 0; i < checkpoints.size(); i++) {
    if (checkpoints[i].y == checkpoint.y && checkpoints[i].x == checkpoint.x) {
      return i;
    }
  }

  // Return -1 if the checkpoint was not found.
  return -1;
}

// Method that gets the number of cells of a given type.
unsigned int Maze::getTheNumberOfCells(MazeCellTypeIds type) {
  // Get the number of cells.
  unsigned int cellsCount = 0;
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      // Check if the cell is of the given type.
      if (finalMaze[y][x] == type) {
        // Increment the number of cells.
        cellsCount++;
      }

      // Increment the number of iterations to generate the maze.
      iterationsTookToGenerate++;
    }
  }

  // Return the number of cells.
  return cellsCount;
}

// Method that gets the name of solving algorithm.
string Maze::getSolvingAlgorithmName(bool noColors) {
  // Define the name of the solving algorithm.
  string solvingAlgorithmName;

  // Define the solving algorithms.
  vector<pair<SupportedSolvingAlgorithms, string>> solvingAlgorithms = noColors ? SUPPORTED_SOLVING_ALGORITHMS_NO_COLOR_STRINGS : SUPPORTED_SOLVING_ALGORITHMS;

  // Find the name of the solving algorithm.
  for (auto & algorithm : solvingAlgorithms) {
      if (algorithm.first == solvingAlgorithm) {
        solvingAlgorithmName = algorithm.second;
        break;
      }
  }

  // Return the name of the solving algorithm.
  return solvingAlgorithmName;
}

// Method that generates the maze report file.
string Maze::generateMazeReportFile() {
  // Declare the report.
  ostringstream report;

  // Append the maze visualization.
  report << "Maze visualization:\n";
  report << printMazeState(finalMaze, false, true, true) << "\n";

  // Append the raw maze.
  report << "Raw maze:\n";
  report << printMazeState(finalMaze, true, true) << "\n";

  // Append the maze parameters.
  report << "Maze parameters:\n";
  report << "  - Width: " << width << "\n";
  report << "  - Height: " << height << "\n";
  switch (checkpointSettingType) {
    case CheckpointSettingType::NUMBER:
      report << "  - Number of checkpoints: " << checkpointsValue << "\n";
      break;
    case CheckpointSettingType::PERCENTAGE:
      report << "  - Checkpoints percentage: " << checkpointsValue << "%" << "\n";
      break;
  }
  report << "  - Solving algorithm: " << getSolvingAlgorithmName(true) << "\n\n";

  // Append the maze generation statistics.
  if (minPathLength > 0 || actualNumberOfCheckpoints > 0) {
    report << "Maze statistics:\n";
  }
  if (minPathLength > 0) {
    report << "  - Minimum path length: " << minPathLength << " cells.\n";
  }
  if (actualNumberOfCheckpoints > 0) {
    report << "  - Number of checkpoints: " << actualNumberOfCheckpoints << ".\n\n";
  }
  report << "Took " << millisecondsToTimeString(timePerformanceMs) << " (" << splitNumberIntoBlocks(iterationsTookToGenerate) << " iterations) to generate.\n";

  // Return the report.
  return report.str();
}

// Method that generates the maze generation steps file.
string Maze::generateMazeGenerationStepsFile(bool minified) {
    // Declare the report.
    ostringstream report;

    // Append the JSON array start.
    report << "[\n";

    // Iterate over the steps.
    for (auto & step : generationSteps) {
      // Append the JSON array start.
      report << "  [\n";

      // Iterate over the rows.
      for (auto & row : step) {
        // Append the JSON array start.
        report << "    [";

        // Iterate over the cells.
        for (auto & cell : row) {
          // Append the cell.
          report << cell;

          // Check if it is the last cell.
          if (&cell != &row.back()) {
              // Append the comma.
              report << ", ";
          }
        }

        // Check if it is the last row.
        if (&row != &step.back()) {
            // Append the JSON array end.
            report << "], \n";
        } else {
            // Append the JSON array end.
            report << "]\n";
        }
      }

      // Check if it is the last step.
      if (&step != &generationSteps.back()) {
          // Append the JSON array end.
          report << "  ], \n";
      } else {
          // Append the JSON array end.
          report << "  ]\n";
      }
    }

    // Append the JSON array end.
    report << "]";

    // Get the report string.
    string reportString = report.str();

    // Remove all spaces and line breaks if the report should be minified.
    if (minified) {
      reportString.erase(remove_if(reportString.begin(), reportString.end(), ::isspace), reportString.end());
    }

    // Return the report.
    return reportString;
}
