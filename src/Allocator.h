#ifndef __ALLOCATOR__
#define __ALLOCATOR__

#include <stdint.h>
#include <string.h>

typedef struct {
    void* base;
    void* memory;
    uint32_t size;
} Allocator;

Allocator*
createAllocator(size_t size);

void*
reserveFrom(Allocator* allocator, size_t size);

#endif