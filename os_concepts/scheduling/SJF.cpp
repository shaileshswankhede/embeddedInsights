#include <bits/stdc++.h>
#include <stdlib.h>
#include "process.h"

using namespace std;

int cmpArrival(const void *a, const void *b)
{
  Process *p1 = (Process *)a;
  Process *p2 = (Process *)b;
  return p1->arrival < p2->arrival ? -1 : (p1->arrival > p2->arrival ? 1 : 0);
}

void runSJFS(Process p[], int num)
{
  qsort(p, num, sizeof(p[0]), cmpArrival);

  // wait queue to maintain process with min service time at top
  priority_queue<Process> pq;
  uint32_t currTime = p[0].arrival;
  uint32_t totalTat = 0;

  // push first process in wait queue
  pq.push(p[0]);
  p[0].arrival = -1; // set process arrival to -1 so that it doesn't enter queue again

  while (!pq.empty())
  {
    printf("Running Process P%d\n", pq.top().pid);

    // increase currTime with service time of currently executed process
    // and remove executed process from wait queue
    currTime += pq.top().service;
    totalTat += (currTime - pq.top().arrival);
    pq.pop();

    // add processes to queue while their arrival time is less than current time based on their service time
    for (int idx = 0; idx < num; idx++)
    {
      if ((p[idx].arrival <= currTime) && (p[idx].arrival != -1))
      {
        pq.push(p[idx]);
        p[idx].arrival = -1;
      }
    }
  }
  printf("avg TAT for %d processes = %.2f\n", num, totalTat * 1.0f / num);
}