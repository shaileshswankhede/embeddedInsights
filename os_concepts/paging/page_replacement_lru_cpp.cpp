// Page replacement LRU code using hashmap and list

#include <unordered_map>
#include <list>
#include <stdlib.h>
#include <time.h>

#define PAGE_TABLE_SIZE 4

using namespace std;

list<pair<int, int>> refPages;     // pages when refrenced are put at front. Last page in list refers to least recently used 
unordered_map<int, list<pair<int, int>>::iterator> pageTable;   // maintains map of page number to reference to correct node in refPages
static uint32_t numPageFaults = 0;

static uint32_t getNewFrame()
{
    // OS finds free space in physical memory and return corresponding frame number
    return rand() % 1024;
}

static uint32_t getFrame(uint32_t pageNumber)
{
    if(pageTable.find(pageNumber) == pageTable.end())
    {
        // page not found in page table. Get new frame and add it to front of refPages to indicate recently referred pages
        numPageFaults++;
        uint32_t frame = getNewFrame();
        refPages.push_front({pageNumber, frame});
        pageTable[pageNumber] = refPages.begin();

        if(refPages.size() > PAGE_TABLE_SIZE)
        {
            // evict last page (least recently used) page from the page table
            auto lastPage = refPages.back();
            pageTable.erase(lastPage.first);
            refPages.pop_back();
        }
        return frame;
    }
    else
    {
        // Page already exist in pagetable. Bring this page number to front of refPages list
        auto it = pageTable[pageNumber];
        refPages.splice(refPages.begin(), refPages, it);
        return it->second;
    }
}

int main1()
{
    srand(time(NULL));
    int pageIdx[] = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2 };
    uint8_t numEntries = sizeof(pageIdx) / sizeof(pageIdx[0]);
    for(uint8_t idx = 0; idx < numEntries; idx++)
    {
        uint32_t frameNumber = getFrame(pageIdx[idx]);
        printf("received frame %d. Page faults so far: %d\n", frameNumber, numPageFaults);
    }
    return 0;
}
