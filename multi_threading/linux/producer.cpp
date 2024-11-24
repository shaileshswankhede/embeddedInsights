// Example code demonstrating interprocess synchronization with named semaphore
// BUffer allocated on shared memory. Run this program as separate process

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_LOCK "/sem_lock"
#define SHM_NAME "/shm0"

#define BUFF_SIZE 10

int main()
{
  // create named semaphore
  sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0644, BUFF_SIZE);
  sem_t *full = sem_open(SEM_FULL, O_CREAT, 0644, 0);
  sem_t *lock = sem_open(SEM_LOCK, O_CREAT, 0644, 1);

  // create named shared memory
  int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
  // truncate fd to required size
  ftruncate(fd, BUFF_SIZE * sizeof(int));
  // map this memory in virtual address space of this (calling) process
  int *buff = (int *)mmap(NULL, BUFF_SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  int item;
  int produced = 0;

  while (1)
  {
    sem_wait(empty);
    sem_wait(lock);
    item = rand() % 100;
    buff[produced++ % BUFF_SIZE] = item;
    printf("Produced %d items: current: %d\n", produced, item);
    sem_post(lock);
    sem_post(full);
    sleep(1);
  }

  // release all resources
  sem_close(empty);
  sem_close(full);
  sem_close(lock);
  sem_unlink(SEM_EMPTY);
  sem_unlink(SEM_FULL);
  sem_unlink(SEM_LOCK);

  close(fd);
  shm_unlink(SHM_NAME);

  return 0;
}
