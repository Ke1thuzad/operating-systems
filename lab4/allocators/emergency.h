#ifndef OPERATIONAL_SYSTEMS_EMERGENCY_H
#define OPERATIONAL_SYSTEMS_EMERGENCY_H

#include <memoryapi.h>

typedef struct Allocator {
    void *memory;
    size_t size;
} Allocator;


Allocator* allocator_create(void *memory, size_t size);

void* allocator_alloc(Allocator *allocator, size_t size);

void allocator_free(Allocator *allocator, void *memory);

void allocator_destroy(Allocator *allocator);

#endif //OPERATIONAL_SYSTEMS_EMERGENCY_H
