#ifndef OPERATIONAL_SYSTEMS_MKK_H
#define OPERATIONAL_SYSTEMS_MKK_H

#include <memoryapi.h>

#define MAX_ORDER 10
#define MIN_ORDER 3
#define PAGESIZE 1024

// 8 16 32 64 128 256 512
// 0 1  2  3  4   5   6

#define NDX(size) \
    (size) > 256 ? 6 : \
    (size) > 128 ? 5 : \
    (size) > 64  ? 4 : \
    (size) > 32  ? 3 : \
    (size) > 16  ? 2 : \
    (size) > 8   ? 1 : 0


typedef struct Page {
    short page_size;
    short frag_size;  // -1 - Contiguous, 0 - Free, 1+ - Allocated.
    void *start_addr;  // if frag_size == -1 - pointer to the first page.
} Page;

typedef struct Buffer {
    void *val;
    struct Buffer *next;
} Buffer;

typedef struct FreeList {
    Page *page;
    struct FreeList *next;
} FreeList;

typedef struct Allocator {
    Page *kmemsizes;
    size_t page_count;
    FreeList *free_page;
    Buffer *freelistarr[MAX_ORDER - MIN_ORDER];
} Allocator;

Allocator *allocator_create(void *memory, size_t size);

void *allocator_alloc(Allocator *allocator, size_t size);

void allocator_free(Allocator *allocator, void *memory);

void allocator_destroy(Allocator *allocator);

#endif //OPERATIONAL_SYSTEMS_MKK_H
