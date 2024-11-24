#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_ITEMS 10
int buff[MAX_ITEMS];

pthread_mutex_t mutex;
sem_t empty, full;
int produced, consumed = 0;

void *producer(void *arg)
{
  int item;
  while (1)
  {
    sem_wait(&empty);
    pthread_mutex_lock(&mutex);
    item = rand() % 100;
    buff[produced++ % MAX_ITEMS] = item;
    printf("produced %d item: %d\n", produced, item);
    fflush(stdout);
    item = (item + 1) % MAX_ITEMS;
    pthread_mutex_unlock(&mutex);
    sem_post(&full);
    usleep(100000);
  }
}

void *consumer(void *arg)
{
  while (1)
  {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    int item = buff[consumed++ % MAX_ITEMS];
    printf("consumed %d item: %d\n", consumed, item);
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    usleep(200000);
  }
}

int main2()
{
  pthread_t tid0, tid1;
  sem_init(&empty, 0, MAX_ITEMS);
  sem_init(&full, 0, 0);
  pthread_create(&tid0, NULL, producer, NULL);
  pthread_create(&tid1, NULL, consumer, NULL);
  pthread_join(tid0, NULL);
  pthread_join(tid1, NULL);
  sem_destroy(&empty);
  sem_destroy(&full);
  return 0;
}