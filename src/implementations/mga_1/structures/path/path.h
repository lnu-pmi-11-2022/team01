#ifndef PATH_H
#define PATH_H

#include <vector>
#include "../cell/cell.h"

using namespace std;

// Structure that represents a path between two checkpoints.
struct Path {
  // The vector of cells that form the path.
  vector<Cell> path;

  // The length of the path.
  double length;

  // The checkpoints that form the path.
  vector<Cell> checkpoints;

  // Constructor.
  Path(vector<Cell> _path, double _length, vector<Cell> _checkpoints);
};

#endif
