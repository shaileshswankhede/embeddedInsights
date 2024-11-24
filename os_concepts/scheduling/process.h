#ifndef __PROCESS_H
#define __PROCESS_H

#include <stdint.h>

typedef struct __Process
{
  int32_t pid;
  uint32_t arrival; // time at which process arrived to queue
  uint32_t service; // time required to process
  uint32_t wait;    // waiting time for process to start running from arrival (running - ready)
  uint32_t tat;     // total turnaround time = waiting + service time
  uint32_t rt;      // remaining time for process

  bool operator<(const struct __Process &o) const
  {
    return service > o.service; // with priority queue > places min element at the top (reverse of comparator in sort)
  }
} Process;

void runFCFS(Process p[], int num);
void runSJFS(Process p[], int num);
void runRR(Process p[], int num);
void runSRT(Process p[], int num);

#endif