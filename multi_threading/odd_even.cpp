#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t m;
pthread_cond_t cond;
int count = 0;

void *printOdd(void *arg)
{
  int *max = (int *)arg;
  while (count < *max)
  {
    pthread_mutex_lock(&m);
    while (count % 2 == 0) // using while to handle spurious wakeup
    {
      // thread releases mutex here and blocks (goes to sleep) on cond
      pthread_cond_wait(&cond, &m);
      // mutex acquired again when call is returned (by call to pthread_cond_signal from other thread)
    }
    printf("%d ", count++);
    // only one of the thread waiting on cond gets unblocked depending on scheduling policy.
    // pthread_cond_broadcast signals should be used to unblock all
    // pthread_cond_signal or pthread_cond_broadcast doesn't automatically release mutex
    // as it doesn't have any reference to mutex
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&m);
  }
  return NULL;
}

void *printEven(void *arg)
{
  int *max = (int *)arg;
  while (count < *max)
  {
    pthread_mutex_lock(&m);
    while (count % 2)
    {
      pthread_cond_wait(&cond, &m);
    }
    printf("%d ", count++);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&m);
  }
  return NULL;
}

int main1()
{
  pthread_t t1, t2;
  int max = 100;
  pthread_create(&t1, NULL, printOdd, &max);
  pthread_create(&t2, NULL, printEven, &max);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  return 0;
}