#include "maze.h"

// Method that prints the maze state.
void Maze::printMazeState(const vector<vector<unsigned int>>& mazeState) {
  string output;
  for (const auto& row : mazeState) {
    for (const auto& cell : row) {
      switch (cell) {
        case WALL_ID:
          output += colorString(WALL_SYMBOL, "white", "white", "default");
          break;
        case PATH_ID:
          output += colorString(PATH_SYMBOL, "green", "black", "default");
          break;
        case CHECKPOINT_ID:
          output += colorString(CHECKPOINT_SYMBOL, "yellow", "black", "bold");
          break;
        case PASSED_CHECKPOINT_ID:
          output += colorString(PASSED_CHECKPOINT_SYMBOL, "white", "green", "bold");
          break;
        case PASSED_PATH_ID:
          output += colorString(PASSED_PATH_SYMBOL, "green", "black", "default");
          break;
        case START_ID:
          output += colorString(START_SYMBOL, "white", "green", "bold");
          break;
        default:
          output += "E ";
      }
    }
    output += "\n";
  }
  fwrite(output.c_str(), sizeof(char), output.size(), stdout);
}
