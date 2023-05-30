#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <climits>
#include "../models/models.h"

using namespace std;

// Define constraints.
const unsigned int MAZE_MIN_WIDTH = 4;
const unsigned int MAZE_MAX_WIDTH = INT_MAX;
const unsigned int MAZE_MIN_HEIGHT = 4;
const unsigned int MAZE_MAX_HEIGHT = INT_MAX;
const unsigned int MAZE_MAX_CHECKPOINTS_PERCENTAGE = 100;
const unsigned int MAZE_WARNING_THRESHOLD_AREA = 10000;
const unsigned int MAZE_MIN_CHECKPOINTS_NUMBER = 2;
const unsigned int MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP = 23;
const unsigned int MAZE_MAX_CHECKPOINTS_NUMBER_BRUTE_FORCE = 12;
const unsigned int MAZE_MIN_CHECKPOINTS_SETTING = 0;
const unsigned int MAZE_MAX_CHECKPOINTS_SETTING = INT_MAX;

// Define the symbols used to represent the matrix cell types.
const string WALL_SYMBOL = "██";
const string PATH_SYMBOL = "  ";
const string CHECKPOINT_SYMBOL = "* ";
const string PASSED_PATH_SYMBOL = "• ";
const string CURRENT_POSITION_SYMBOL = "• ";
const string PASSED_CHECKPOINT_SYMBOL = "+ ";
const string START_SYMBOL = "ST";
const string END_SYMBOL = "EN";

// Define other constants.
const unsigned int ESTIMATED_TIME_UPDATE_INTERVAL_STEPS = 10;
const unsigned int MAZE_GENERATION_VISUALIZATION_MIN_DURATION_MS = 5000;
const bool PRINT_MAZE_AS_IDS = false;
const string GENERATION_BG_AUDIO_FILE_PATH = "assets/generation.wav";
const string VISUALIZATION_BG_AUDIO_FILE_PATH = "assets/visualization.wav";

// Define the supported solving algorithms.
const vector<pair<SupportedSolvingAlgorithms, string>> SUPPORTED_SOLVING_ALGORITHMS = {
  { SupportedSolvingAlgorithms::HELD_KARP_PARALLEL, colorString("Held-Karp - Multithreading", "yellow", "default", "underline") + " (the fastest; non-heuristic; up to " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP) + " checkpoints)" },
  { SupportedSolvingAlgorithms::HELD_KARP, colorString("Held-Karp - Single Thread", "yellow", "default", "underline") +  " (fast; non-heuristic; up to " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP) + " checkpoints)" },
  { SupportedSolvingAlgorithms::BRUTE_FORCE, colorString("Brute Force", "yellow", "default", "underline") + " (slow; non-heuristic; up to " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_BRUTE_FORCE) + " checkpoints)" },
  { SupportedSolvingAlgorithms::NONE, colorString("None", "yellow", "default", "underline") + " (just distribute checkpoints)" },
};
const vector<pair<SupportedSolvingAlgorithms, string>> SUPPORTED_SOLVING_ALGORITHMS_NO_COLOR_STRINGS = {
  { SupportedSolvingAlgorithms::HELD_KARP_PARALLEL, "Held-Karp - Multithreading (the fastest; non-heuristic; up to " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP) + " checkpoints)" },
  { SupportedSolvingAlgorithms::HELD_KARP, "Held-Karp - Single Thread (fast; non-heuristic; up to " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_HELD_KARP) + " checkpoints)" },
  { SupportedSolvingAlgorithms::BRUTE_FORCE, "Brute Force (slow; non-heuristic; up to " + to_string(MAZE_MAX_CHECKPOINTS_NUMBER_BRUTE_FORCE) + " checkpoints)" },
  { SupportedSolvingAlgorithms::NONE, "None (just distribute checkpoints)" },
};

// Define the supported checkpoint setting types.
const vector<pair<CheckpointSettingType, string>> SUPPORTED_CHECKPOINT_SETTING_TYPES = {
  { CheckpointSettingType::NUMBER, colorString("Number", "yellow", "default", "underline") },
  { CheckpointSettingType::PERCENTAGE, colorString("Percentage", "yellow", "default", "underline") },
};

#endif
