#include "Allocator.h"

#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

Allocator*
createAllocator(size_t size) {
    Allocator* allocator = (Allocator*) malloc(sizeof(Allocator));
    allocator->size = size;
    allocator->memory = malloc(size + 4096*2);
    uintptr_t r = (uintptr_t)allocator->memory;
    r &= 0xfffff000;
    r += 4096;
    allocator->memory = (void*)r;
    mprotect(allocator->memory, size,
            PROT_READ | PROT_WRITE | PROT_EXEC);
    allocator->base = allocator->memory;
    return allocator;
}

void*
reserveFrom(Allocator* allocator, size_t size)
{
    if (allocator->base + size <= allocator->base + allocator->size) {
        void* tmp = allocator->base;
        allocator->base+=size;
        return tmp;
    }
    fprintf(stderr, "Couldn't allocate memory\n");
    exit(4);
}