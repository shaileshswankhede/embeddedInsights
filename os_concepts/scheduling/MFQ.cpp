// Multiple queues with decreasing order of priority and increasing quantum time
// Processes are added to queue and processed in FCFS.
// If process is taking more time, it is bumped to higher order queue
// Idea is even though you don't get to run often, you get higher quantum of time when you get CPUI.
// This scheme favours processes that can quickly enter and exit

#include <stdio.h>
#include <stdlib.h>

#define NUM_QUEUES 3

typedef struct Process
{
  int pid;
  int burst_time;
  int remaining_time;
  int queue_level;
  struct Process *next;
} Process;

typedef struct Queue
{
  Process *front;
  Process *rear;
} Queue;

Queue *createQueue()
{
  Queue *q = (Queue *)malloc(sizeof(Queue));
  q->front = q->rear = NULL;
  return q;
}

void enqueue(Queue *q, Process *p)
{
  if (q->rear == NULL)
  {
    q->front = q->rear = p;
  }
  else
  {
    q->rear->next = p;
    q->rear = p;
  }
  p->next = NULL;
}

Process *dequeue(Queue *q)
{
  if (q->front == NULL)
  {
    return NULL;
  }
  Process *temp = q->front;
  q->front = q->front->next;
  if (q->front == NULL)
  {
    q->rear = NULL;
  }
  return temp;
}

Process *createProcess(int pid, int burst_time)
{
  Process *p = (Process *)malloc(sizeof(Process));
  p->pid = pid;
  p->burst_time = burst_time;
  p->remaining_time = burst_time;
  p->queue_level = 0;
  p->next = NULL;
  return p;
}

void executeProcess(Process *p, int time_quantum)
{
  if (p->remaining_time > time_quantum)
  {
    p->remaining_time -= time_quantum;
  }
  else
  {
    p->remaining_time = 0;
  }
}

int main_mfq()
{
  Queue *queues[NUM_QUEUES];
  for (int i = 0; i < NUM_QUEUES; i++)
  {
    queues[i] = createQueue();
  }

  Process *p1 = createProcess(1, 10);
  Process *p2 = createProcess(2, 6);
  Process *p3 = createProcess(3, 8);
  Process *p4 = createProcess(4, 4);

  enqueue(queues[0], p1);
  enqueue(queues[0], p2);
  enqueue(queues[0], p3);
  enqueue(queues[0], p4);

  while (1)
  {
    int all_empty = 1;
    for (int i = 0; i < NUM_QUEUES; i++)
    {
      if (queues[i]->front != NULL)
      {
        all_empty = 0;
        Process *current = dequeue(queues[i]);
        executeProcess(current, 1 << current->queue_level);

        printf("Executing Process %d from Queue %d with %d remaining time\n", current->pid, i, current->remaining_time);

        if (current->remaining_time > 0)
        {
          if (i < NUM_QUEUES - 1)
          {
            current->queue_level++;
            enqueue(queues[i + 1], current);
          }
          else
          {
            enqueue(queues[i], current);
          }
        }
        else
        {
          free(current);
        }
        break;
      }
    }
    if (all_empty)
    {
      break;
    }
  }

  for (int i = 0; i < NUM_QUEUES; i++)
  {
    free(queues[i]);
  }

  return 0;
}
