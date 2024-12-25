#include "mkk.h"


Allocator *allocator_create(void *const memory, const size_t size) {
    Allocator *new_allocator = (Allocator *) VirtualAlloc(NULL, sizeof(Allocator), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!new_allocator)
        return NULL;

    size_t k_sizes = size / PAGESIZE + (size % PAGESIZE != 0);
    new_allocator->page_count = k_sizes;

    new_allocator->kmemsizes = (Page *) VirtualAlloc(NULL, sizeof(Page) * k_sizes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    new_allocator->kmemsizes[0].start_addr = memory;
    new_allocator->kmemsizes[0].page_size = PAGESIZE;
    if (size / PAGESIZE <= 0)
        new_allocator->kmemsizes[0].page_size = size % PAGESIZE;

    new_allocator->kmemsizes[0].frag_size = 0;

    new_allocator->free_page = (FreeList *) VirtualAlloc(NULL, sizeof(FreeList), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    new_allocator->free_page->page = &new_allocator->kmemsizes[0];

    FreeList *freeList = new_allocator->free_page;

    for (int i = 1; i < k_sizes; ++i) {
        new_allocator->kmemsizes[i].page_size = PAGESIZE;
        if (i == k_sizes - 1 && size % PAGESIZE > 0) {
            new_allocator->kmemsizes[i].page_size = size % PAGESIZE;
        }
        new_allocator->kmemsizes[i].start_addr = new_allocator->kmemsizes[i - 1].start_addr + PAGESIZE;
        new_allocator->kmemsizes[i].frag_size = 0;


        freeList->next = (FreeList *) VirtualAlloc(NULL, sizeof(FreeList), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        freeList = freeList->next;
        freeList->page = &new_allocator->kmemsizes[i];
    }

    return new_allocator;
}

void *small_alloc(Allocator *const allocator, const size_t size) {
    unsigned char order = NDX(size);

    void *allocated_chunk;

    // If free cells of this size exist, then use them
    if (allocator->freelistarr[order]) {
        allocated_chunk = allocator->freelistarr[order]->val;

        Buffer *residue = allocator->freelistarr[order];

        allocator->freelistarr[order] = allocator->freelistarr[order]->next;

        VirtualFree(residue, sizeof(Buffer), MEM_DECOMMIT);

        return allocated_chunk;
    }
    // Else alloc page for this size

    if (!allocator->free_page)
        return NULL;

    Page *free_page = allocator->free_page->page;
    allocator->free_page = allocator->free_page->next;

    free_page->frag_size = 1 << (MIN_ORDER + order);

    int fragments = free_page->page_size / free_page->frag_size;

    allocator->freelistarr[order] = (Buffer *) VirtualAlloc(NULL, sizeof(Buffer), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    Buffer *current_buf = allocator->freelistarr[order];

    allocated_chunk = free_page->start_addr;

    current_buf->val = free_page->start_addr + free_page->frag_size;

    // Add newly allocated fragments to the pool
    for (int i = 2; i < fragments; ++i) {
        current_buf->next = (Buffer *) VirtualAlloc(NULL, sizeof(Buffer), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        current_buf = current_buf->next;

        current_buf->val = free_page->start_addr + free_page->frag_size * i;
    }

    return allocated_chunk;
}

void *large_alloc(Allocator *const allocator, const size_t size) {
    // Check if contiguous buffers are ok (first fit)

    size_t need_pages = size / PAGESIZE + (size % PAGESIZE != 0);

    if (need_pages > allocator->page_count)
        return NULL;

    FreeList *cur = allocator->free_page, *prev = NULL;

    int available = 0;
    Page *available_start = NULL;

    while (cur) {

        if (cur->next && cur->next->page->start_addr - cur->page->page_size == cur->page->start_addr) {
            available++;

            if (!available_start)
                available_start = cur->page;
        } else {
            available_start = NULL;
            available = 0;
        }

        if (available >= need_pages)
            break;

        if (!available)
            prev = cur;

        cur = cur->next;
    }

    if (available >= need_pages) {
        Page *allocated_page = available_start;
        for (int i = 0; i < need_pages; ++i) {
            allocated_page->frag_size = -1;
            allocated_page = (Page *)((char*)allocated_page->start_addr + allocated_page->page_size);
        }

        if (prev)
            prev->next = cur ? cur->next : NULL;
        else
            allocator->free_page = cur ? cur->next : NULL;

        VirtualFree(cur, sizeof(FreeList), MEM_DECOMMIT);
        return available_start->start_addr;
    }

    return NULL;
}

void *allocator_alloc(Allocator *const allocator, const size_t size) {
    if (size <= PAGESIZE / 2) {
        return small_alloc(allocator, size);
    }

    return large_alloc(allocator, size);
}

void allocator_free(Allocator *const allocator, void *const memory) {
    size_t memdiff = (memory - allocator->kmemsizes[0].start_addr);

    size_t page_id = memdiff / PAGESIZE;
    if (page_id >= allocator->page_count)
        return;

    Page *page = &allocator->kmemsizes[page_id];

    int order = NDX(page->frag_size);

    if (page->frag_size < PAGESIZE) {
        Buffer *new_free_buffer = (Buffer *) VirtualAlloc(NULL, sizeof(Buffer), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
        if (!new_free_buffer)
            return;

        new_free_buffer->val = memory;

        new_free_buffer->next = allocator->freelistarr[order];
        allocator->freelistarr[order] = new_free_buffer;
    } else {
        page->frag_size = 0;

        // Free contiguous pages

        if (page_id >= allocator->page_count - 1)
            return;

        int i = 1;
        Page *cur = &allocator->kmemsizes[page_id + i], *prev = page;

        while (cur->frag_size == -1) {
            cur->start_addr = prev->start_addr + prev->page_size;
            cur->frag_size = 0;

            prev = cur;

            i++;
            cur = &allocator->kmemsizes[page_id + i];
        }
    }

}

void destroy_buffer(Buffer *start) {
    Buffer *cur = start, *prev;
    while (cur) {
        prev = cur;
        cur = cur->next;

        VirtualFree(prev, sizeof(Buffer), MEM_DECOMMIT);
    }
}

void destroy_free_pages_list(FreeList *start) {
    FreeList *cur = start, *prev;
    while (cur) {
        prev = cur;
        cur = cur->next;

        VirtualFree(prev, sizeof(FreeList), MEM_DECOMMIT);
    }
}

void allocator_destroy(Allocator *const allocator) {
    for (int i = 0; i < MAX_ORDER - MIN_ORDER; ++i)
        destroy_buffer(allocator->freelistarr[i]);

    destroy_free_pages_list(allocator->free_page);

    VirtualFree(allocator->kmemsizes, 0, MEM_RELEASE);
    VirtualFree(allocator, 0, MEM_RELEASE);
}
