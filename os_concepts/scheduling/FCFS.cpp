#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "process.h"

// FCFS is non preemptive scheduling algorithm and follows FIFO.
// CPU is assigned to processes in the order processes appear and request

int sortByArrival(const void *a, const void *b)
{
  Process *p1 = (Process *)a;
  Process *p2 = (Process *)b;
  return p1->arrival < p2->arrival ? -1 : (p1->arrival > p2->arrival ? 1 : 0);
}

void runFCFS(Process p[], int num)
{

  // sort processes by arrival time
  qsort(p, num, sizeof(p[0]), sortByArrival);

  uint32_t start = p[0].arrival;
  uint32_t totalTat = 0;
  for (int i = 0; i < num; i++)
  {
    if (i > 0)
    {
      // update start
      start = p[i - 1].arrival + p[i - 1].tat;
      if (p[i].arrival > start)
        start = p[i].arrival;
    }
    p[i].wait = start - p[i].arrival;
    p[i].tat = p[i].wait + p[i].service;

    totalTat += p[i].tat;
  }
  printf("avg tat for FCFS processes %.2f\n", (float)totalTat / num);
}
