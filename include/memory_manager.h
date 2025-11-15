#ifndef MEMORY_MANAGER_H_
#define MEMORY_MANAGER_H_

#include "stdlib.h"

static const size_t HEAP_LIST_SIZE = 1024 * 4;

struct MemoryChunk
{
    int64_t size;
    MemoryChunk* next;
};

struct Memory
{
    void* heap_start;
    size_t heap_size;
    size_t current_offset;

    MemoryChunk* first_pointers[20];
};


void IncreaseHeap();

void* Allocate(size_t);
void* CleanAllocate(size_t);
void* ReAllocate(void*, size_t);

void Free(void* pointer);


#endif // MEMORY_MANAGER_H_