#include <stdio.h>
#include <string.h>

typedef struct
{
  char a;  // 1 byte
  char *b; // pointer (size depends on the machine: 4 bytes on 32-bit, 8 bytes on 64-bit)
  int c;   // 4 byte
} myStruct;

int main1()
{
  printf("sizeof myStruct %lu alignment %lu\n", sizeof(myStruct), __alignof(myStruct));
  // on 32bit machine
  // a|P|P|P|b|b|b|b|c|c|c|c --> 12 bytes
  // sizeof pointer = 4 --> b needs alignment of 4 btes so padding of 3 bytes after char a
  // on 64bit machine
  // a|P|P|P|P|P|P|P|b|b|b|b|b|b|b|b|c|c|c|c|P|P|P|P  --> 24 bytes
  // sizeof pointer = 8 --> b needs alignment of 8 bytes so padding of 7 bytes after char a
  // Last padding after c to make entire structure aligned to 8 bytes so that every member in struct array follows alignment
  // For example without final padding, b will be only 4 bytes aligned for 2nd struct element
  // P represents padding

  return 0;
}