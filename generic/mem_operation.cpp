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
  else
  {
    while(len--)
    {
      *((uint8_t *)dst) = *((uint8_t *)src);
      dst = (void *)((uint8_t *)dst + 1);
      src = (void *)((uint8_t *)src + 1);
    }
  }
  return dstOrig;
}

void *faster_memcopy(void *dst, const void *src, size_t len)
{
  // reading word from address not aligned to word size is slower
  // instead copy first few bytes till address gets aligned to word boundary
  // if both source and destination are unaligned by same number of bytes with respect to Word coundary
  // then simple bytecopy of extra unaligned bytes should work. Otherwise we need merge trick
  void *dstOrig = dst;
  uintptr_t dstP = (uintptr_t)dst;
  uintptr_t srcP = (uintptr_t)src;
  // check if source and destination are unaligned by number of bytes with respect to Word coundary
  uint8_t wordBytes = sizeof(uint32_t);
  if((srcP & (wordBytes - 1)) == (dstP & (wordBytes - 1)))
  {
    // copy few bytes to align dst to word boundary
    if (len >= wordBytes)
    {
      uint8_t bytesToCopy = (-dstP) % wordBytes;
      len = len - bytesToCopy;
      while (bytesToCopy--)
      {
        *((uint8_t*)dst) = *((uint8_t*)src);
        dst = (void*)((uint8_t*)dst + 1);
        src = (void*)((uint8_t*)src + 1);
      }
    }
    fast_memcpy(dst, src, len);
  }
  else
  {
    // Reference: https://book-of-gehn.github.io/articles/2024/09/21/Behind-memcpy-memmove.html
    // dst and source NOT unaligned by same num of bytes
    // first copy few bytes such that dst gets aligned. Then read current and next word, do necessary alignement to get effective word
    // and write to aligned dst
    if (len >= wordBytes)
    {
      uint8_t bytesToCopy = (-dstP) % wordBytes;
      len = len - bytesToCopy;
      while (bytesToCopy--)
      {
        *((uint8_t*)dst) = *((uint8_t*)src);
        dst = (void*)((uint8_t*)dst + 1);
        src = (void*)((uint8_t*)src + 1);
      }
      //now dst is aligned on word boundary
      if(len > wordBytes)
      {
        srcP = (uintptr_t)src;
        uint8_t sh1 = srcP % wordBytes;
        uint8_t sh2 = wordBytes - sh1;
        uint32_t *lWord = (uint32_t*)(srcP - sh1);
        uint32_t *rWord = (uint32_t*)(srcP + sh2);
        uint32_t srcWord = ((*lWord) << sh1) | ((*rWord) >> sh2);
        *((uint32_t*)dst) = *((uint32_t*)src);
        dst = (void*)((uint32_t*)dst + 1);
        src = (void*)((uint32_t*)src + 1);
        len -= wordBytes;
        fast_memcpy(dst, src, len);
      }
      else
      {
        // bytecopy remaining few bytes
        while (len--)
        {
          *((uint8_t*)dst) = *((uint8_t*)src);
          dst = (void*)((uint8_t*)dst + 1);
          src = (void*)((uint8_t*)src + 1);
        }
      }
    }
  }
  return dstOrig;
}

void *my_memmove(void *dst, const void *src, size_t len)
{
  uint8_t *dstPtr = (uint8_t *)dst;
  const uint8_t *srcPtr = (const uint8_t *)src;
  uintptr_t dstP = (uintptr_t)dstPtr;
  uintptr_t srcP = (uintptr_t)srcPtr;

  // simple efficient memcpy can be used if addresses are non overlapping
  // safe way to check is via pointer subtraction than using dstP >= srcP + len as it may overflow
  // also direct comparing pointer is undefined behavior if pointer doesn't point to same memory object. Thus address comparision recommended
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
  memcpy(buffer0 + 2, buffer0, 13);         // result depends on how memcpy is implemented. Expected result if memcpy copying large data in one chunk like page even though address overlapping
  my_memcpy(buffer1 + 2, buffer1, 13);      // unexpected result with bytecopy implementation of memcpy because of overlap and dst addr > src addr
  my_memmove(buffer2 + 2, buffer2, 13);     // expected result with memove implementation, where direction of copy determined based on dst addr and src addr
  fast_memcpy(buffer3 + 2, buffer3, 13);    // fast memcpy using block copy. Result varies compared to bytecopy because direct chunk copied
  faster_memcopy(buffer4, buffer4 + 2, 13); // even faster memcpy leveraging address alignement for data access optimization

  printf("Result with memcpy: %s\n", buffer0);
  printf("Result with my_memcpy: %s\n", buffer1);
  printf("Result with my_memmove: %s\n", buffer2);
  printf("Result with fast memcpy: %s\n", buffer3);
  printf("Result with faster memcpy: %s\n", buffer4);
  return 0;
}