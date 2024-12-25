#include "buddy.h"

size_t next_power_of_two(size_t size) {
    size_t power = 1;
    while (power < size) {
        power <<= 1;
    }
    return power;
}

size_t get_level(size_t size) {
    size_t level = 0;
    while (size > MIN_BLOCK_SIZE && level <= MAX_LEVEL) {
        size >>= 1;
        level++;
    }
    return level;
}

size_t get_block_size(size_t level) {
    return MIN_BLOCK_SIZE << level;
}

Allocator* allocator_create(void* memory, size_t size) {
    if (!memory || size == 0) return NULL;

    size_t aligned_size = next_power_of_two(size);
    Allocator *allocator = (Allocator*)memory;
    allocator->memory = (void*)((char*)memory + sizeof(Allocator));
    allocator->size = aligned_size - sizeof(Allocator);

    for (size_t i = 0; i <= MAX_LEVEL; i++) {
        allocator->free_lists[i] = NULL;
    }

    size_t max_level = get_level(allocator->size);
    allocator->free_lists[max_level] = (Block*)allocator->memory;
    allocator->free_lists[max_level]->address = allocator->memory;
    allocator->free_lists[max_level]->next = NULL;

    return allocator;
}

void* allocator_alloc(Allocator *allocator, size_t size) {
    if (!allocator || size == 0 || size > allocator->size)
        return NULL;

    size_t required_size = next_power_of_two(size);
    size_t level = get_level(required_size);

    if (level > MAX_LEVEL)
        return NULL;

    for (size_t l = level; l <= MAX_LEVEL; l++) {
        if (allocator->free_lists[l]) {
            Block *block = allocator->free_lists[l];
            allocator->free_lists[l] = block->next;

            while (l > level) {
                size_t block_size = get_block_size(l);
                void *lower = block->address;
                void *upper = (char*)block->address + block_size / 2;

                Block *buddy = (Block*)upper;
                buddy->address = upper;
                buddy->next = allocator->free_lists[l - 1];
                allocator->free_lists[l - 1] = buddy;

                block->next = allocator->free_lists[l - 1];
                allocator->free_lists[l - 1] = block;

                l--;
                block = allocator->free_lists[l];
                allocator->free_lists[l] = block->next;
            }

            return block->address;
        }
    }

    return NULL;
}

void allocator_free(Allocator *allocator, void *memory) {
    if (!allocator || !memory)
        return;

    size_t offset = (size_t)memory - (size_t)allocator->memory;
    size_t level = 0;
    size_t block_size = get_block_size(0);

    while (offset >= block_size && level <= MAX_LEVEL) {
        block_size <<= 1;
        level++;
    }

    Block *block = (Block*)memory;
    block->address = memory;
    block->next = allocator->free_lists[level];
    allocator->free_lists[level] = block;

    while (level < MAX_LEVEL) {
        size_t buddy_level = level;
        size_t buddy_block_size = get_block_size(buddy_level);
        size_t buddy_offset = offset ^ (buddy_block_size / 2);
        void *buddy_address = (void*)((size_t)allocator->memory + buddy_offset);

        Block **prev_ptr = &(allocator->free_lists[buddy_level]);
        Block *buddy = allocator->free_lists[buddy_level];
        while (buddy && buddy->address != buddy_address) {
            prev_ptr = &(buddy->next);
            buddy = buddy->next;
        }

        if (buddy) {
            *prev_ptr = buddy->next;

            if ((size_t)memory < (size_t)buddy_address) {
                block->address = memory;
                block->next = allocator->free_lists[level + 1];
                allocator->free_lists[level + 1] = block;
            } else {
                block->address = buddy_address;
                block->next = allocator->free_lists[level + 1];
                allocator->free_lists[level + 1] = block;
            }
            level++;
        } else {
            break;
        }
    }
}

void allocator_destroy(Allocator *allocator) {
    if (!allocator)
        return;
}