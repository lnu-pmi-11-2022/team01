#include "structures/maze/maze.h"

using namespace std;

// Main function.
int main(int argc, char** argv) {
  // Define the default maze width, height and checkpoints percentage.
  unsigned int mazeWidth = DEFAULT_MAZE_WIDTH;
  unsigned int mazeHeight = DEFAULT_MAZE_HEIGHT;
  unsigned int checkpointsPercentage = DEFAULT_CHECKPOINTS_PERCENTAGE;

  // Check if the number of arguments is correct.
  if (argc == 4) {
    // Get the maze width.
    mazeWidth = stoi(argv[1]);

    // Get the maze height.
    mazeHeight = stoi(argv[2]);

    // Get the number of checkpoints.
    checkpointsPercentage = stoi(argv[3]);
  }

  // Create the maze.
  Maze maze(mazeWidth, mazeHeight, checkpointsPercentage);

  // Visualize the maze generation.
  maze.visualizeMazeGeneration(MAZE_GENERATION_VISUALIZATION_MIN_DURATION_MS);

  return 0;
}
