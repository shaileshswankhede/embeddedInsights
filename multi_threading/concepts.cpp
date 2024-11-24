#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

pthread_mutex_t lock;

void *power(void *arg)
{
  int *in = (int *)arg;
  int *ret = (int *)malloc(sizeof(int));
  *ret = (*in) * (*in);
  printf("child thread %lu returning value = %d addr:%p\n", pthread_self(), *ret, ret);
  pthread_exit(ret);
  // return ret;       // This has same effect of pthread_exit(ret)
}

// This becomes zombie thread if it terminates and not joined
// leading to resource leak. OS maintains its state (TCB) for some possible future joing
void *fun(void *arg)
{
  int *ptr = (int *)arg;
  while (1)
  {
    printf("Spawned thread: %lu. I am alive\n", pthread_self());
    sleep(3);
    // This is undefined behaviour as thread trying to access main thread local data
    // main thread termination can lead to segmentation fault. Behaviour is OS specific
    // OS may chose to retain main thread stack. MingW windows crashes over here
    memset(ptr, 0x34, 100 * sizeof(int));
  }
}

void *leftShift(void *arg)
{
  int *val = (int *)arg;
  pthread_mutex_lock(&lock);
  *val = *val << 1;
  // thread which locked mutex should only release mutex. Otherwise undefined behaviour
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main()
{
  pthread_t tid0, tid1, tid2, tid3;
  int localArr[100];
  int input = 6;

  pthread_create(&tid0, NULL, fun, localArr);
  pthread_create(&tid1, NULL, power, &input);
  pthread_detach(tid0); // Here pthread library will release resource as soon as tid0 exits without waiting for joins

  void *output;
  sleep(3); // even though thread tid1 might have already exited, thread stack is preserved so as to get return value
  // pthread_detach(tid1); // calling pthread_detach immediately releases resources and thread stack will and thus return will no longer will be valid
  pthread_join(tid1, &output);
  printf("return value from tid1 --> %d addr: %p\n", (*(int *)output), output);
  printf("freeing up memory allocated in tid1 for storing result\n");
  free(output);

  int value = 5U;
  // two threads can share same text segment.
  // Only need to ensure proper synchronization mechanism to avoid race condition
  pthread_create(&tid2, NULL, leftShift, &value);
  pthread_create(&tid3, NULL, leftShift, &value);
  pthread_join(tid2, NULL);
  pthread_join(tid3, NULL);

  printf("updated value = %u\n", value);

  sleep(5);
  printf("main thread terminating\n");
  pthread_exit(0);
  printf("This should never print\n");
  // this return doesn't have any effect
  // Return without pthread_exit will terminate child thread
  return 0;
}