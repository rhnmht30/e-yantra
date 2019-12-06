#include <iostream>
#include <climits>
#include <cstring>
using namespace std;

// M x N matrix
#define M 10
#define N 10
int s = 0;

// Check if it is possible to go to (x, y) from current position. The
// function returns false if the cell has value 0 or already visited
bool isSafe(int mat[M][N], int visited[M][N], int x, int y)
{
  if (mat[x][y] == 0 || visited[x][y])
    return false;

  return true;
}

// if not a valid position, return false
bool isValid(int x, int y)
{
  if (x < M && y < N && x >= 0 && y >= 0)
    return true;

  return false;
}

// Find Shortest Possible Route in a Matrix mat from source cell (0, 0)
// to destination cell (x, y)

// min_dist is passed by reference and stores length of longest path
// from source to destination found so far dist maintains length of
// path from source cell to current cell (i, j)

void findShortestPath(int mat[M][N], int visited[M][N], int i, int j,
                      int x, int y, int &min_dist, int dist)
{
  s++;
  // if destination is found, update min_dist
  if (i == x && j == y)

  {
    cout << "Pahuch gye";
    // cout << "\n";
    // for (int i = 0; i < M; i++)
    // {
    //   for (int j = 0; j < N; j++)
    //   {
    //     cout << visited[i][j] << " ";
    //   }
    //   cout << "\n";
    // }
    min_dist = min(dist, min_dist);
    return;
  }

  // set (i, j) cell as visited
  visited[i][j] = 1;

  cout << "(" << i << "," << j << ")";

  // go to bottom cell

  if (isValid(i + 1, j) && isSafe(mat, visited, i + 1, j))
  {
    if (i == 7 && j == 4)
    {
      cout << "bottom";
    }

    findShortestPath(mat, visited, i + 1, j, x, y, min_dist, dist + 1);
  }

  // go to right cell
  if (isValid(i, j + 1) && isSafe(mat, visited, i, j + 1))
  {
    if (i == 7 && j == 4)
    {
      cout << "right";
    }

    findShortestPath(mat, visited, i, j + 1, x, y, min_dist, dist + 1);
  }

  // cout << "no right(" << i << "," << j << ")should go up";
  // go to top cell
  if (isValid(i - 1, j) && isSafe(mat, visited, i - 1, j))
  {
    if (i == 7 && j == 4)
    {
      cout << "top";
    }
    findShortestPath(mat, visited, i - 1, j, x, y, min_dist, dist + 1);
  }

  // go to left cell
  if (isValid(i, j - 1) && isSafe(mat, visited, i, j - 1))
  {
    if (i == 7 && j == 4)
    {
      cout << "lefty";
    }

    findShortestPath(mat, visited, i, j - 1, x, y, min_dist, dist + 1);
  }

  // Backtrack - Remove (i, j) from visited matrix
  // visited[i][j] = 0;
}

// Find Shortest Path in Maze
int main()
{
  int mat[M][N] =
      {
          {1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
          {0, 1, 1, 1, 1, 1, 0, 1, 0, 1},
          {0, 0, 1, 0, 1, 1, 1, 0, 0, 1},
          {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
          {0, 0, 0, 1, 0, 0, 0, 1, 0, 1},
          {1, 0, 1, 1, 1, 0, 0, 1, 1, 0},
          {0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
          {0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
          {1, 1, 1, 1, 1, 0, 0, 1, 1, 1},
          {0, 0, 1, 0, 0, 1, 1, 0, 0, 1},
      };

  // construct a matrix to keep track of visited cells
  int visited[M][N];

  // initially all cells are unvisited
  memset(visited, 0, sizeof visited);

  int min_dist = INT_MAX;
  findShortestPath(mat, visited, 0, 0, 7, 5, min_dist, 0);
  cout << s;
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      cout << visited[i][j] << " ";
    }
    cout << "\n";
  }
  if (min_dist != INT_MAX)
    cout << "The shortest path from source to destination "
            "has length "
         << min_dist << "\n";
  else
    cout << "Destination can't be reached from given source";

  return 0;
}