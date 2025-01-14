// Page replacement LRU code in C. Key checking in page table is through manual walkthrough
// maintain index of least recently used page
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PAGE_TABLE_SIZE 4

typedef struct pageTable
{
    int32_t pageNumber;
    int32_t frameNumber;
} pageTable_t;

pageTable_t pages[PAGE_TABLE_SIZE];

int32_t lastUsed[PAGE_TABLE_SIZE];  // to track when page is referenced. Lowest number indicates least recently used page
static uint32_t numPageFaults = 0;

static uint32_t getNewFrame()
{
    // OS finds free space in physical memory and return corresponding frame number
    return rand() % 1024;
}

static uint32_t getFrame(uint32_t pageNumber)
{
    static uint32_t refIdx = 0;
    for (uint32_t idx = 0; idx < PAGE_TABLE_SIZE; idx++)
    {
        if (pageNumber == pages[idx].pageNumber)
        {
            // page found in page table
            lastUsed[idx] = refIdx++;
            return pages[idx].frameNumber;
        }
    }

    // page not found in page table. Assign new entry in page table at lruIdx
    numPageFaults++;
    // find least recently used page in pagetable
    int32_t lruIdx = -1;
    int32_t minTime = INT32_MAX;
    for (uint32_t idx = 0; idx < PAGE_TABLE_SIZE; idx++)
    {
        if (pages[idx].frameNumber == -1)
        {
            // empty frame available at idx
            lruIdx = idx;
            break;
        }
        if (lastUsed[idx] < minTime)
        {
            minTime = lastUsed[idx];
            lruIdx = idx;
        }
    }

    // replace LRU page
    pages[lruIdx].pageNumber = pageNumber;
    pages[lruIdx].frameNumber = getNewFrame();
    lastUsed[lruIdx] = refIdx++;
    return pages[lruIdx].frameNumber;
}

int main()
{
    srand(time(NULL));

    // reset all page table entries to -1
    for (uint32_t idx = 0; idx < PAGE_TABLE_SIZE; idx++)
    {
        pages[idx].pageNumber = -1;
        pages[idx].frameNumber = -1;
    }
    memset(lastUsed, 0xff, sizeof(lastUsed));

    int pageIdx[] = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2 };
    uint8_t numEntries = sizeof(pageIdx) / sizeof(pageIdx[0]);
    for(uint8_t idx = 0; idx < numEntries; idx++)
    {
        uint32_t frameNumber = getFrame(pageIdx[idx]);
        printf("received frame %d. Page faults so far: %d\n", frameNumber, numPageFaults);
    }

    return 0;
}
