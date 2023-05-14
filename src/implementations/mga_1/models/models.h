#ifndef MODELS_H
#define MODELS_H

using namespace std;

// Define the matrix cell types IDs.
enum MazeCellTypeIds {
  PATH_ID = 0,
  WALL_ID = 1,
  CHECKPOINT_ID = 2,
  PASSED_PATH_ID = 3,
  PASSED_CHECKPOINT_ID = 4,
  CURRENT_POSITION_ID = 5,
  START_ID = 6,
  END_ID = 7
};

#endif
