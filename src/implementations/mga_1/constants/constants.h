#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

using namespace std;

// Define default maze generation parameters.
const unsigned int DEFAULT_MAZE_WIDTH = 21;
const unsigned int DEFAULT_MAZE_HEIGHT = 21;
const unsigned int DEFAULT_CHECKPOINTS_PERCENTAGE = 10;

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

#endif
