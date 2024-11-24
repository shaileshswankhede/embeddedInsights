#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/wait.h>

#define MAX_ARGS 10

bool getCommand(char *argv[])
{
  char *line = NULL;
  size_t len = 0;
  if (getline(&line, &len, stdin) != -1)
  {
    char *arg = strtok(line, " ");
    int idx = 0;
    while (arg != NULL)
    {
      argv[idx] = arg;
      arg = strtok(NULL, " ");
      idx++;
    }
    argv[idx - 1][strlen(argv[idx - 1]) - 1] = '\0'; // remove newline from last entered arg
    argv[idx] = NULL;                                // args must be null terminated for exec call
    return true;
  }
  return false;
}

void shellTest()
{
  int pid;
  // char *const argv[] = {(char *)"ls", (char *)"-l", NULL};
  // execvp(argv[0], argv);

  char *line = NULL;
  size_t len = 0;
  char *argv[MAX_ARGS];
  int status;

  printf("\nEnter command to test\n");

  while (getCommand(&argv[0]))
  {
    // exec call replaces content of calling process with exec program's memory
    // thus it is important to call it in separate child process
    if ((pid = fork()) == 0)
    {
      execvp(argv[0], argv);
    }
    waitpid(pid, &status, WNOHANG);
  }
}