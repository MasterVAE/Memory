#include <unistd.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>

#include "../include/memory_manager.h"

static int pow(int a, int b);

static Memory mem = {};

void IncreaseHeap()
{
    void* pointer = sbrk(HEAP_LIST_SIZE);

    if(!mem.heap_size)
    {
        mem.heap_start = pointer;
    }
    mem.heap_size += HEAP_LIST_SIZE;
}

void* Allocate(size_t size)
{
    if(size == 0) return NULL;
    if(mem.heap_size == 0) IncreaseHeap();

    int power = 6;
    for(; power < 21; power++)
    {
        if((size_t)pow(2, power) >= size) break;
    }

    if(power >= 20) return NULL;
    int memory_size = pow(2, power);

    MemoryChunk* currect_chunk = mem.first_pointers[power];
    while(currect_chunk)
    {
        if(currect_chunk->size < 0)
        {
            currect_chunk->size = -currect_chunk->size;
            return currect_chunk + 1;
        }
        currect_chunk = currect_chunk->next;
    }

    while (mem.heap_size - mem.current_offset < (size_t)memory_size + sizeof(MemoryChunk))
    {
        IncreaseHeap();
    }

    MemoryChunk* chunk = (MemoryChunk*)((char*)mem.heap_start + mem.current_offset);
    currect_chunk = mem.first_pointers[power];
    chunk->size = memory_size;
    chunk->next = NULL;

    mem.current_offset += (size_t)memory_size + sizeof(MemoryChunk);
    if(!mem.first_pointers[power])
    {
        mem.first_pointers[power] = chunk;
        return chunk + 1;
    }

    currect_chunk = mem.first_pointers[power];
    while(currect_chunk)
    {
        if(!currect_chunk->next)
        {
            currect_chunk->next = chunk;
            return chunk + 1;
        }

        currect_chunk = currect_chunk->next;
    }

    return NULL;
}

void* CleanAllocate(size_t size)
{
    char* allocated = (char*)Allocate(size);
    if(!allocated) return NULL;

    for(size_t i = 0; i < size; i++)
    {
        allocated[i] = 0;
    }

    return allocated;
}

void* ReAllocate(void* pointer, size_t size)
{
    if(!pointer)
    {
        return Allocate(size);
    }

    if(size == 0)
    {
        Free(pointer);
        return NULL;
    }

    MemoryChunk* chunk = (MemoryChunk*)pointer - 1;

    if(size <= (size_t)chunk->size) return pointer;

    void* new_pointer = Allocate(size);
    for(size_t i = 0; i < (size_t)chunk->size; i++)
    {
        *((char*)new_pointer + i) = *((char*)pointer + i);
    }
    Free(pointer);

    return new_pointer;
}

void Free(void* pointer)
{
    MemoryChunk* chunk = (MemoryChunk*)pointer - 1;

    if(chunk->size > 0)
    {
        chunk->size = -chunk->size;
        return;
    }
}

static int pow(int a, int b)
{
    int ans = 1;
    for(int i = 0; i < b; i++)
    {
        ans *= a;
    }

    return ans;
}