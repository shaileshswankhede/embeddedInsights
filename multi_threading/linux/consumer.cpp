// Example code demonstrating interprocess synchronization with named semaphore
// BUffer allocated on shared memory. Run this program as separate process

#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_LOCK "/sem_lock"
#define SHM_NAME "/shm0"

#define BUFF_SIZE 10

int main6()
{
  int item;
  int consumed = 0;

  // Open named semaphore
  sem_t *empty = sem_open(SEM_EMPTY, 0);
  sem_t *full = sem_open(SEM_FULL, 0);
  sem_t *lock = sem_open(SEM_LOCK, 0);

  // Open existing shared memory with read only attribute
  int fd = shm_open(SHM_NAME, O_RDONLY, 0666);
  // map this memory to virtual address space of this (calling) process
  int *buff = (int *)mmap(NULL, BUFF_SIZE * sizeof(int), PROT_READ, MAP_SHARED, fd, 0);
  if (!(lock && full && empty && buff))
  {
    printf("couldn't open semaphore or shared memory\n");
    return 0;
  }

  while (1)
  {
    sem_wait(full);
    sem_wait(lock);
    item = buff[consumed++ % BUFF_SIZE];
    printf("consumed %d items: %d\n", consumed, item);
    sem_post(lock);
    sem_post(empty);
    sleep(2);
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
