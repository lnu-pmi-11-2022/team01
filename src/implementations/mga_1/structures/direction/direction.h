#ifndef DIRECTION_H
#define DIRECTION_H

// Structure that represents a cell direction.
struct Direction {
  // Offset in the X and Y axis.
  int offsetX, offsetY;

  // Constructor.
  Direction(int _offsetX, int _offsetY);
};

#endif
