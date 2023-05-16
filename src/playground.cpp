#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfloat>
#include <ciso646>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

#include <ccomplex>
#include <cfenv>
#include <cinttypes>
#include <cstdbool>
#include <cstdint>
#include <ctgmath>
#include <cwchar>
#include <cwctype>

// C++
#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <ostream>
#include <vector>

using namespace std;





// function to solve TSP using Held-Karp algorithm
vector<int> tsp_hk(vector<vector<double>> adjacencyMatrix) {
  const unsigned int checkpointsNumber = adjacencyMatrix.size(); // number of cities

  double memoizationMatrix[checkpointsNumber][1 << checkpointsNumber]; // memoization table
  int parent[checkpointsNumber][1 << checkpointsNumber]; // to reconstruct the path


  // set initial values for memoization table
  for (int i = 0; i < checkpointsNumber; i++) {
    for (int j = 0; j < (1 << checkpointsNumber); j++) {
      memoizationMatrix[i][j] = -1;
      parent[i][j] = -1;
    }
  }

  // base case for dynamic programming
  memoizationMatrix[0][1] = 0;

  // calculate shortest path for all subsets of cities
  for (int mask = 1; mask < (1 << checkpointsNumber); mask++) {
    for (int i = 0; i < checkpointsNumber; i++) {
      if (!(mask & (1 << i))) continue;
      for (int j = 0; j < checkpointsNumber; j++) {
        if (mask & (1 << j) || i == j) continue;
        int new_mask = mask | (1 << j);
        if (memoizationMatrix[j][new_mask] == -1 || memoizationMatrix[j][new_mask] > memoizationMatrix[i][mask] + adjacencyMatrix[i][j]) {
          memoizationMatrix[j][new_mask] = memoizationMatrix[i][mask] + adjacencyMatrix[i][j];
          parent[j][new_mask] = i;
        }
      }
    }
  }

  // find the end of the shortest path
  int end = 0;
  for (int i = 1; i < checkpointsNumber; i++) {
    if (memoizationMatrix[i][(1 << checkpointsNumber) - 1] == -1) continue;
    if (memoizationMatrix[i][(1 << checkpointsNumber) - 1] + adjacencyMatrix[i][0] < memoizationMatrix[end][(1 << checkpointsNumber) - 1] + adjacencyMatrix[end][0]) {
      end = i;
    }
  }

  // construct the path using the parent array
  vector<int> path;
  int mask = (1 << checkpointsNumber) - 1;
  while (parent[end][mask] != -1) {
    path.push_back(end);
    int new_mask = mask ^ (1 << end);
    end = parent[end][mask];
    mask = new_mask;
  }
  path.push_back(end);
  path.push_back(0);
  reverse(path.begin(), path.end());

  return path;
}

// main function to test the algorithm
int main() {
  vector<vector<double>> matrix = {
      {0,4,30,28,22,26,17,21,28,28,23,23},
      {4,0,26,24,18,22,13,17,24,24,19,19},
      {30,26,0,10,32,36,27,31,38,38,33,33},
      {28,24,10,0,30,34,25,29,36,36,31,31},
      {22,18,32,30,0,16,5,3,18,10,5,13},
      {26,22,36,34,16,0,11,15,2,22,17,13},
      {17,13,27,25,5,11,0,4,13,11,6,8},
      {21,17,31,29,3,15,4,0,17,7,2,12},
      {28,24,38,36,18,2,13,17,0,24,19,15},
      {28,24,38,36,10,22,11,7,24,0,5,19},
      {23,19,33,31,5,17,6,2,19,5,0,14},
      {23,19,33,31,13,13,8,12,15,19,14,0}
  };

  // solve TSP and print the result
  vector<int> result = tsp_hk(matrix);
  cout << "Shortest Path: ";
  for (int i : result) {
    cout << i << " ";
  }

  return 0;
}
