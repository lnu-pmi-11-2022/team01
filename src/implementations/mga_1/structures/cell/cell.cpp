#include "cell.h"

// Constructor.
Cell::Cell(int _x, int _y) : x(_x), y(_y) {}

// Overload of the == operator.
bool Cell::operator==(const Cell& cell) const {
  return x == cell.x && y == cell.y;
}

// Overload of the != operator.
bool Cell::operator!=(const Cell& cell) const {
  return !(*this == cell);
}
