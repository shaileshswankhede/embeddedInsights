#define HASH_TABLE_SIZE 17          // prime number greater than 16
int hashTable[HASH_TABLE_SIZE] = {0};

int hashFunc(int num)
{
    return num % HASH_TABLE_SIZE;
}

int getHashTableEntry(int num)
{
    int idx = hashFunc(num);
    if (hashTable[idx] == -1)   // no hash collision
    {
        hashTable[idx] = num;
        return idx;
    }
    else    // hash collision. Find next entry in hash table
    {
        int origIdx = idx;
        for (int next = idx; next != origIdx; next = (next+1) % HASH_TABLE_SIZE)
        {
            if (hashTable[next] == -1)  // found next free entry
            {
                hashTable[next] = num;
                return next;
            }
        }
    }
}

void initHash()
{
    memset(hashTable, 0xFF, sizeof(hashTable));     // initialize all elements to -1
}