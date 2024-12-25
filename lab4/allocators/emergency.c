#include "emergency.h"

Allocator* allocator_create(void *memory, size_t size) {
    Allocator *allocator = (Allocator*)VirtualAlloc(NULL, sizeof(Allocator), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!allocator) return NULL;

    allocator->memory = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!allocator->memory) {
        VirtualFree(allocator, 0, MEM_RELEASE);
        return NULL;
    }

    allocator->size = size;
    return allocator;
}

void allocator_destroy(Allocator *allocator) {
    if (allocator) {
        VirtualFree(allocator->memory, 0, MEM_RELEASE);
        VirtualFree(allocator, 0, MEM_RELEASE);
    }
}

void* allocator_alloc(Allocator *allocator, size_t size) {
    if (!allocator || size == 0 || size > allocator->size)
        return NULL;

    void *memory = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    return memory;
}

void allocator_free(Allocator *allocator, void *memory) {
    if (memory) {
        VirtualFree(memory, 0, MEM_RELEASE);
    }
}

