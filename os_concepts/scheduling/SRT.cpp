// This is preemptive version of SJF
#include <stdio.h>
#include "process.h"

int getShortestProcess(Process p[], int num, int currTime)
{
  int idx = -1;
  int shortestTime = INT32_MAX;
  for (int i = 0; i < num; i++)
  {
    if ((p[i].arrival < currTime) && (p[i].rt != 0) && (p[i].rt < shortestTime))
    {
      shortestTime = p[i].rt;
      idx = i;
    }
  }
  return idx;
}

void findWaitingTime(Process p[], int num)
{
  int numCompleted = 0;
  int currTime = 0;
  while (numCompleted < num)
  {
    // find process with shortest remaining time that arrived before current time
    int schIdx = getShortestProcess(p, num, currTime);
    if (schIdx == -1)
    {
      currTime++;
      continue;
    }

    // reduce remaining time of scheduled process
    p[schIdx].rt--;

    // measure all times if a process is completed
    if (p[schIdx].rt <= 0)
    {
      numCompleted++;
      p[schIdx].tat = currTime - p[schIdx].arrival;
      p[schIdx].wait = p[schIdx].tat - p[schIdx].service;
    }
    currTime++;
  }
}

void runSRT(Process p[], int num)
{
  findWaitingTime(p, num);

  int totalTat = 0;
  for (int i = 0; i < num; i++)
  {
    totalTat += p[i].tat;
  }
  printf("avg TAT for %d processes = %.2f\n", num, totalTat * 1.0F / num);
}