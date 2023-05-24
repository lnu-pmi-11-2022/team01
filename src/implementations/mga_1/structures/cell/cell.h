#ifndef CELL_H
#define CELL_H

// Structure that represents a cell.
struct Cell {
  // Position in the X and Y axis.
  int x, y;

  // Constructor.  
  Cell(int _x, int _y);

  // Overload of the == operator.
  bool operator==(const Cell& cell) const;

  // Overload of the != operator.
  bool operator!=(const Cell& cell) const;
};

#endif
