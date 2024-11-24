#include <stdio.h>
#include <stdlib.h>

// fibonacci of nth number using dynamic programming - bottom up approach
int fibonaaci(int n)
{
  if (n <= 1)
    return n;

  int *fibArr = (int *)malloc((n + 1) * sizeof(int));
  fibArr[0] = 0;
  fibArr[1] = 1;
  for (int idx = 2; idx <= n; idx++)
  {
    fibArr[idx] = fibArr[idx - 1] + fibArr[idx - 2];
  }
  return fibArr[n];
}

int main1()
{
  int num = 6;
  printf("fib at %dth position = %d\n", num, fibonaaci(num));
  return 0;
}