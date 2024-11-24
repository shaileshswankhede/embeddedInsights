#include <string.h>
#include <stdio.h>
#include <stdint.h>

void *my_memcpy(void *__restrict dst, const void *__restrict src, size_t count)
{
  char *dstPtr = (char *)dst;
  const char *srcPtr = (const char *)src;
  while (count--)
  {
    *dstPtr++ = *srcPtr++;
  }
  return dst;
}

void *fast_memcpy(void *dst, const void *src, size_t len)
{
  void *dstOrig = dst;
  if (len > sizeof(long))
  {
    // do block copy with block size sizeof(long)
    size_t chunks = len / sizeof(long);
    while (chunks--)
    {
      // can't type cast and increment pointer in single instruction as typecast creates lvalue
      // and lvalue cannot be modified. Direct incrementing void* dst is undefined behavior
      *((uint64_t *)dst) = *((uint64_t *)src);
      dst = (void *)((uint64_t *)dst + 1);
      src = (void *)((uint64_t *)src + 1);
    }

    uint8_t trailBytes = len % sizeof(long);
    chunks = trailBytes / sizeof(uint32_t);
    if (chunks)
    {
      *((uint32_t *)dst) = *((uint32_t *)src);
      dst = (void *)((uint32_t *)dst + 1);
      src = (void *)((uint32_t *)src + 1);
      trailBytes -= sizeof(uint32_t);
    }

    chunks = trailBytes / sizeof(uint16_t);
    if (chunks)
    {
      *((uint16_t *)dst) = *((uint16_t *)src);
      dst = (void *)((uint16_t *)dst + 1);
      src = (void *)((uint16_t *)src + 1);
      trailBytes -= sizeof(uint16_t);
    }

    if (trailBytes)
    {
      *((uint8_t *)dst) = *((uint8_t *)src);
    }
  }
  return dstOrig;
}

void *fast_memmove(void *dst, const void *src, size_t len)
{
  uintptr_t dstP = (uintptr_t)dst;
  // copy few bytes to align dst to word boundary
  if (len >= sizeof(uint64_t))
  {
    len = len - ((-dstP) % sizeof(uint64_t));
  }
  //WIP
  return dst;
}

void *my_memmove(void *dst, const void *src, size_t len)
{
  uint8_t *dstPtr = (uint8_t *)dst;
  const uint8_t *srcPtr = (const uint8_t *)src;
  uintptr_t dstP = (uintptr_t)dstPtr;
  uintptr_t srcP = (uintptr_t)srcPtr;

  // simple efficient memcpy can be used if addresses are non overlapping
  // safe way to check is via pointer subtraction than using dstP >= srcP + len as it may overflow
  // if ((dstP > srcP && (dstP - srcP) >= len) ||
  //    (srcP > dstP) && (srcP - dstP) >= len)
  //{
  //  memcpy(dst, src, len);
  //}
  // bytecopy can be optimized to chunk copy. Need to consider alignement and trailing bytes
  if (dstP > srcP && (dstP - srcP) < len) // need back to front copying
  {
    dstPtr += len;
    srcPtr += len;
    while (len--)
    {
      *(--dstPtr) = *(--srcPtr);
    }
  }
  else // safe front to back copying
  {
    while (len--)
    {
      *dstPtr++ = *srcPtr++;
    }
  }
  return dst;
}

int main()
{
  char buffer0[16] = "123456781234567";
  char buffer1[16] = "123456781234567";
  char buffer2[16] = "123456781234567";
  char buffer3[16] = "123456781234567";
  char buffer4[16] = "123456781234567";
  // memcpy behaviour is implementation specific for overlapping region
  // For optimization it may copy block of data (like 8 bytes) in single instruction
  // and trailing data in descending order of block size
  memcpy(buffer0 + 2, buffer0, 13);
  my_memcpy(buffer1 + 2, buffer1, 13);
  my_memmove(buffer2 + 2, buffer2, 13);
  fast_memcpy(buffer3 + 2, buffer3, 13);
  fast_memmove(buffer4 + 2, buffer4, 13);

  printf("Result with memcpy: %s\n", buffer0);
  printf("Result with my_memcpy: %s\n", buffer1);
  printf("Result with my_memmove: %s\n", buffer2);
  printf("Result with fast memcpy: %s\n", buffer3);
  return 0;
}