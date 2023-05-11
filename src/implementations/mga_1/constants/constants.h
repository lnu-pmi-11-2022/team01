#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

using namespace std;

// Define constraints.
const unsigned int MAZE_MIN_WIDTH = 4;
const unsigned int MAZE_MAX_WIDTH = INT_MAX;
const unsigned int MAZE_MIN_HEIGHT = 4;
const unsigned int MAZE_MAX_HEIGHT = INT_MAX;
const unsigned int MAZE_MIN_CHECKPOINTS_PERCENTAGE = 0;
const unsigned int MAZE_MAX_CHECKPOINTS_PERCENTAGE = 100;
const unsigned int MAZE_WARNING_THRESHOLD_AREA = 10000;

// Define the symbols used to represent the matrix cell types.
const string WALL_SYMBOL = "██";
const string PATH_SYMBOL = "  ";
const string CHECKPOINT_SYMBOL = "* ";
const string PASSED_PATH_SYMBOL = "• ";
const string PASSED_CHECKPOINT_SYMBOL = "+ ";
const string START_SYMBOL = "ST";

// Define other constants.
const unsigned int ESTIMATED_TIME_UPDATE_INTERVAL_STEPS = 10;
const unsigned int MAZE_GENERATION_VISUALIZATION_MIN_DURATION_MS = 5000;

#endif
