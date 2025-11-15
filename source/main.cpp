#include <stdio.h>

#include "../include/memory_manager.h"

int main(void)
{
    int* test1 = (int*)Allocate(100);

    char** test2 = (char**)CleanAllocate(10);

    char** test3 = (char**)ReAllocate(test2, 70);

    printf("%p %p %p\n", test1, test2, test3);

    return 0;
}