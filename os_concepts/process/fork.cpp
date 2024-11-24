#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void forkTest()
{
  int pid;
  // New process is created within fork() system call, so child starts from there and return value to variable pid
  // Each process has its own memory so each have its own pid
  // fork() call returns pid of child in parent process and 0 in child process
  // getpid() can be used to return actual pid in child process
  // sequence of execution can change based on context switch
  if ((pid = fork()) == 0)
  {
    printf("A: I am a child, my pid is %d\n", getpid());
    exit(0);
  }
  printf("B: I am the parent, my pid is %d\n", getpid());
  printf("C: I just created a child whose pid is %d\n", pid);
}