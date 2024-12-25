    #ifndef OPERATIONAL_SYSTEMS_MAIN_H
#define OPERATIONAL_SYSTEMS_MAIN_H

#include <memoryapi.h>
#include <windows.h>
#include <stdio.h>
#include <unistd.h>

//#include "mkk.h"
#include "allocators/buddy.h"

typedef Allocator *(alloc_create_func)(void *memory, size_t size);
typedef void *(alloc_alloc_func)(Allocator *allocator, size_t size);
typedef void (alloc_free_func)(Allocator *allocator, void *memory);
typedef void (alloc_destroy_func)(Allocator *allocator);

#endif //OPERATIONAL_SYSTEMS_MAIN_H
