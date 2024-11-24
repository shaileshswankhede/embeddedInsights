#include <vector>
#include <stdio.h>

using namespace std;

vector<vector<int>> getPascalTriangle(int rows)
{
  vector<vector<int>> matrix;
  for (int row = 0; row < rows; row++)
  {
    vector<int> rowVec;
    for (int i = 0; i <= row; i++)
    {
      if (i == 0 || i == row)
      {
        rowVec.push_back(1);
      }
      else
      {
        rowVec.push_back(matrix[row - 1][i - 1] + matrix[row - 1][i]);
      }
    }
    matrix.push_back(rowVec);
  }
  return matrix;
}

int main()
{
  int n = 5;
  vector<vector<int>> mat = getPascalTriangle(n);
  for (int row = 0; row < mat.size(); row++)
  {
    for (int i = 0; i < mat[row].size(); i++)
    {
      printf("%d ", mat[row][i]);
    }
    printf("\n");
  }

  return 0;
}