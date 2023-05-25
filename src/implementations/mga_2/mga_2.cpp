#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <queue>
#include <algorithm>
#include <utility>
#include <climits>
#include "../../helpers/helpers.h"

using namespace std;

#define ll long long

const ll DIMMASK = (1 << 23);

const int INF = 1000000007;

int dist[30][30];
int d[DIMMASK][25];
int N, res, reslast;

int m, n, maxpoints, matrix[300][300], indexes[1000], matrix2[300][300];

// new function to find the accessible cells
void ddfs(int ii, int jj) {
  if (ii - 1 >= 0 && matrix[ii - 1][jj] == 0) {
    matrix[ii - 1][jj] = 3;
    ddfs(ii - 1, jj);
  }
  if (jj - 1 >= 0 && matrix[ii][jj - 1] == 0) {
    matrix[ii][jj - 1] = 3;
    ddfs(ii, jj - 1);
  }
  if (ii + 1 < n && matrix[ii + 1][jj] == 0) {
    matrix[ii + 1][jj] = 3;
    ddfs(ii + 1, jj);
  }
  if (jj + 1 < m && matrix[ii][jj + 1] == 0) {
    matrix[ii][jj + 1] = 3;
    ddfs(ii, jj + 1);
  }
}

// generate the matrix
void generate_matrix(int n, int m) {
  auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  mt19937 generator(seed);
  uniform_int_distribution<int> distribution(1, 100);
  //generate random matrix
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (distribution(generator) >= 63) {
        matrix[i][j] = 1;
      } else matrix[i][j] = 0;
    }
  }
  //find accessible cells
  ddfs(0, 0);
  //fill non-accessible cells with obstacles
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 0)
        matrix[i][j] = 1;
    }
  }
  //fill accessible cells with empty cells
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 3)
        matrix[i][j] = 0;
    }
  }
}
//put checkpoints on the matrix
void put_checkpoints() {
  auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  mt19937 generator(seed);
  uniform_int_distribution<int> distribution(1, 500);
  //put obstacles on the matrix
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 0 && distribution(generator) >= 498 && maxpoints <= 20) {
        matrix[i][j] = 2;
        maxpoints++;
      }
    }
  }
}

//function to set a random start on the matrix on an empty cell
void set_start() {
  auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
  mt19937 generator(seed);
  uniform_int_distribution<int> distribution(1, 100);
  int i, j;
  do {
    i = distribution(generator) % n;
    j = distribution(generator) % m;
  } while (matrix[i][j] != 0);
  matrix[i][j] = 2;
}

int shortest_path(const pair<int, int> &start, const pair<int, int> &end, vector<pair<int, int>> &path) {
  // check if start and end cells are valid
  if (matrix[start.first][start.second] == 1 || matrix[end.first][end.second] == 1) {
    return -1;
  }

  // initialize the distance matrix and the visited matrix
  vector<vector<int>> dist(300, vector<int>(300, -1));
  vector<vector<bool>> visited(300, vector<bool>(300, false));

  // define the direction vectors for BFS
  const vector<int> dir_row = {-1, 0, 1, 0};
  const vector<int> dir_col = {0, 1, 0, -1};

  // initialize the queue with the start cell
  queue<pair<int, int>> q;
  q.push(start);
  dist[start.first][start.second] = 0;

  // perform BFS until the end cell is found or the queue is empty
  while (!q.empty()) {
    // dequeue a cell from the front of the queue
    pair<int, int> current = q.front();
    q.pop();

    // mark the current cell as visited
    visited[current.first][current.second] = true;

    // if we have reached the end cell, backtrack to find the shortest path
    if (current == end) {
      pair<int, int> current_cell = end;
      while (current_cell != start) {
        path.push_back(current_cell);
        int min_dist = INT_MAX;
        int min_index = -1;
        for (int i = 0; i < dir_row.size(); ++i) {
          int row = current_cell.first + dir_row[i];
          int col = current_cell.second + dir_col[i];
          if (row >= 0 && row < n && col >= 0 && col < m && dist[row][col] != -1 && dist[row][col] < min_dist) {
            min_dist = dist[row][col];
            min_index = i;
          }
        }
        current_cell.first += dir_row[min_index];
        current_cell.second += dir_col[min_index];
      }
      path.push_back(start);
      reverse(path.begin(), path.end());
      return dist[end.first][end.second];
    }

    // enqueue neighboring cells that are not obstacles and have not been visited
    for (int i = 0; i < dir_row.size(); ++i) {
      int row = current.first + dir_row[i];
      int col = current.second + dir_col[i];
      if (row >= 0 && row < n && col >= 0 && col < m && !visited[row][col] && matrix[row][col] != 1) {
        q.push(make_pair(row, col));
        visited[row][col] = true;
        dist[row][col] = dist[current.first][current.second] + 1;
      }
    }
  }

  // if we have exhausted all possible paths and still haven't reached the end cell, there is no path
  return -1;
}

bool inset(ll mask, ll v) {
  return mask & (1 << (v - 1));
}

ll removebit(ll mask, ll bit) {
  return mask & ~(1 << (bit - 1));
}

int ind = 1; //індекс масиву, в який записується шлях
//функція друку оптимальноо шляху. Отримує множину, і останній елемент на шляху
void printanswer(ll mask, ll last) {
  indexes[ind] = last; //записуємо останній елемент на шляху в масив
  ind++; //збільшуємо індекс масиву
  ll prevmask = removebit(mask, last); //яка множина була на попередньому кроці - без елемента last

  for (int i = 1; i <= N; i++)
    if (inset(prevmask, i)) //якщо елемент i присутній в множині prvmask
      if (d[mask][last] == d[prevmask][i] + dist[i][last]) { //перевірим чи він був оптимальним кінцевим в цій множині
        printanswer(prevmask, i); //якщо так, то викличемо друк результату для цієї множини і цього останнього елемента
        break;
      }
}

//function to print the matrix
void print_matrix() {
  if (matrix[n - 1][m - 1] != 1) {
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < m; ++j) {
        if (matrix[i][j] == 0) {
          cout << "  ";
        } else if (matrix[i][j] == 1) {
          cout << colorString("██", "white", "white", "bold");
        } else if (matrix[i][j] == 2) {
          cout << colorString("* ", "green", "default", "bold");
        } else if (matrix[i][j] == 3) {
          cout << colorString("ST", "white", "green", "bold");
        } else if (matrix[i][j] == 4) {
          cout << colorString("• ", "yellow", "default", "bold");
        }
      }
      cout << endl;
    }
  }
}

void mga2() {
  clearConsole();

  // Reinitialize the dist array
  for (int i = 0; i < 30; i++) {
    for (int j = 0; j < 30; j++) {
      dist[i][j] = 0;
    }
  }

// Reinitialize the d array
  for (int i = 0; i < DIMMASK; i++) {
    for (int j = 0; j < 25; j++) {
      d[i][j] = 0;
    }
  }

// Reinitialize N, res, and reslast
  N = 0;
  res = 0;
  reslast = 0;

// Reinitialize m, n, maxpoints, and matrix
  m = 0;
  n = 0;
  maxpoints = 0;
  for (int i = 0; i < 300; i++) {
    for (int j = 0; j < 300; j++) {
      matrix[i][j] = 0;
    }
  }

// Reinitialize indexes and matrix2
  for (int i = 0; i < 1000; i++) {
    indexes[i] = 0;
  }
  for (int i = 0; i < 300; i++) {
    for (int j = 0; j < 300; j++) {
      matrix2[i][j] = 0;
    }
  }

  int number_of_generations = 0;
  cout<<"Enter the height of the maze: ";
  cin >> n;
  cout<<"Enter the width of the maze: ";
  cin >> m;

  cout << "Generating maze..." << endl;

  matrix[n - 1][m - 1] = 1;
  while (matrix[n - 1][m - 1] == 1) {
    int k;
    number_of_generations++;
    k = 0;//%61
    //n=50;
    //m=210;
    generate_matrix(n, m);
  }
  //set start point
  //set_start();
  //put checkpoints
  put_checkpoints();

  //find shortest path from start to all checkpoints
  vector<pair<int, int>> path;

  vector<pair<int, int>> myPoints;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix[i][j] == 2) {
        myPoints.push_back(make_pair(i, j));
      }
    }
  }

  int ii = 0;
  for (auto it : myPoints) {
    ii++;
    int jj = 0;
    for (auto it2 : myPoints) {
      if (it != it2) {
        jj++;
        dist[ii][jj] = shortest_path(it, it2, path);
      } else {
        jj++;
        dist[ii][jj] = 0;
      }
    }
  }

  N = ii;

  long long maxmask = (1 << N) - 1; //Отримуємо число з n одиничних бітів - максимальна маска


  for (ll mask = 0; mask <= maxmask; mask++) {
    for (ll last = 1; last <= N; last++) {
      d[mask][last] = INF; //по замовчуванню
      if (inset(mask, last)) { //якщо елемент last присутній в множині
        if (mask == (1 << (last - 1)))
          d[mask][last] = 0; //якщо множина лише з цього елемента, то відповідь = 0. База динаміки
        else {
          ll newmask = removebit(mask, last); //якою була множина до приєднання елемента last
          for (ll i = 1; i <= N; i++)
            if (inset(newmask, i)) //перебираємо всі елементи тієї множини, як останні на шляху
              d[mask][last] = min(d[mask][last],
                                  d[newmask][i]
                                      + dist[i][last]); //дивимось чи вігідніше нашу множину буде отримати відповідним переходом
        }
      }
    }
  }

  res = INF; //оптимальний результат

  //знаходимо на якому кінцевому елементів найоптимальніший результат
  for (ll v = 1; v <= N; v++)
    if (d[maxmask][v] < res) {
      res = d[maxmask][v];
      reslast = v;
    }

  printanswer(maxmask, reslast); //запускаємо функцію друку відповіді шляху

  matrix[myPoints[indexes[1] - 1].first][myPoints[indexes[1] - 1].second] = 3;

  clearConsole();

  cout << "Maze generated successfully!" << endl;
  cout << "Number of generations: " << number_of_generations << endl;
  waitForEnter(colorString("\nPress the \"Enter\" key to print the maze...", "green", "black", "bold"));
  clearConsole();

  print_matrix();

  waitForEnter(colorString("\nPress the \"Enter\" key twice to print the shortest path...", "green", "black", "bold"));
  clearConsole();

  cout << "Generating the shortest path..." << endl;

  for (int i = 1; i < ind - 1; i++) {
    vector<pair<int, int>> path2;

    //print the shortest path
    ll qwe = shortest_path(myPoints[indexes[i] - 1], myPoints[indexes[i + 1] - 1], path2);
    if (qwe == -1) {
      break;
    }

    //loop for printing the shortest path without start and end points
    for (ll j = 1; j < path2.size() - 1; j++) {
      matrix2[path2[j].first][path2[j].second] = 4;
    }

    path2.clear();
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (matrix2[i][j] == 4) {
        matrix[i][j] = 4;
      }
    }
  }
  clearConsole();
  print_matrix();

  //print the length of the shortest path
  cout << "\nShortest path length: " << res << endl;

  waitForEnter(colorString("\nPress the \"Enter\" key twice to continue...", "green", "black", "bold"));
}
