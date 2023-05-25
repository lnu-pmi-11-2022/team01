#include "maze.h"

// Method that prints the maze state.
string Maze::printMazeState(const vector<vector<unsigned int>>& mazeState, bool printAsIDs, bool noOutput, bool noColors) {
  string output;
  for (const auto& row : mazeState) {
    for (const auto& cell : row) {
      switch (cell) {
        case WALL_ID:
          if (printAsIDs) {
            output += to_string(WALL_ID);
          } else {
            if (noColors) {
              output += WALL_SYMBOL;
            } else {
              output += colorString(WALL_SYMBOL, "white", "white", "default");
            }
          }
          break;
        case PATH_ID:
          if (printAsIDs) {
            output += to_string(PATH_ID);
          } else {
            if (noColors) {
              output += PATH_SYMBOL;
            } else {
              output += colorString(PATH_SYMBOL, "green", "black", "default");
            }
          }
          break;
        case CHECKPOINT_ID:
          if (printAsIDs) {
            output += to_string(CHECKPOINT_ID);
          } else {
            if (noColors) {
              output += CHECKPOINT_SYMBOL;
            } else {
              output += colorString(CHECKPOINT_SYMBOL, "yellow", "black", "bold");
            }
          }
          break;
        case PASSED_CHECKPOINT_ID:
          if (printAsIDs) {
            output += to_string(PASSED_CHECKPOINT_ID);
          } else {
            if (noColors) {
              output += PASSED_CHECKPOINT_SYMBOL;
            } else {
              output += colorString(PASSED_CHECKPOINT_SYMBOL, "white", "green", "bold");
            }
          }
          break;
        case PASSED_PATH_ID:
          if (printAsIDs) {
            output += to_string(PASSED_PATH_ID);
          } else {
            if (noColors) {
              output += PASSED_PATH_SYMBOL;
            } else {
              output += colorString(PASSED_PATH_SYMBOL, "green", "black", "default");
            }
          }
          break;
        case CURRENT_POSITION_ID:
          if (printAsIDs) {
            output += to_string(CURRENT_POSITION_ID);
          } else {
            if (noColors) {
              output += CURRENT_POSITION_SYMBOL;
            } else {
              output += colorString(CURRENT_POSITION_SYMBOL, "green", "yellow", "default");
            }
          }
          break;
        case START_ID:
          if (printAsIDs) {
            output += to_string(START_ID);
          } else {
            if (noColors) {
              output += START_SYMBOL;
            } else {
              output += colorString(START_SYMBOL, "white", "green", "bold");
            }
          }
          break;
        case END_ID:
          if (printAsIDs) {
            output += to_string(END_ID);
          } else {
            if (noColors) {
              output += END_SYMBOL;
            } else {
              output += colorString(END_SYMBOL, "white", "red", "bold");
            }
          }
          break;
        default:
          output += "E ";
      }
    }
    output += "\n";
  }

  if (!noOutput) {
    fwrite(output.c_str(), sizeof(char), output.size(), stdout);
  }

  return output;
}
