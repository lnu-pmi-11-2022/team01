#include "path.h"

#include <utility>

// Constructor.
Path::Path(vector<Cell> _path, double _length, vector<Cell> _checkpoints) : path(std::move(_path)), length(_length), checkpoints(std::move(_checkpoints)) {}
