#include "main.h"

static alloc_create_func *alloc_create;
static alloc_alloc_func *alloc_alloc;
static alloc_free_func *alloc_free;
static alloc_destroy_func *alloc_destroy;

int main(int argc, char **argv) {

    HMODULE library = LoadLibrary(argv[1]);

    if (argc < 1 || library == NULL) {
        library = LoadLibrary("./libemergency.dll");

        write(STDERR_FILENO, "Emergency library has been loaded\n", 35);
    }

    if (!library) {
        write(STDERR_FILENO, "Error with loading a library has occurred\n", 43);

        return 1;
    }

    alloc_create = (alloc_create_func *) GetProcAddress(library, "allocator_create");
    if (!alloc_create) {
        write(STDERR_FILENO, "Error with importing allocator_create has occurred\n", 52);
    }

    alloc_alloc = (alloc_alloc_func *) GetProcAddress(library, "allocator_alloc");
    if (!alloc_alloc) {
        write(STDERR_FILENO, "Error with importing allocator_alloc has occurred\n", 51);
    }

    alloc_free = (alloc_free_func *) GetProcAddress(library, "allocator_free");
    if (!alloc_free) {
        write(STDERR_FILENO, "Error with importing allocator_free has occurred\n", 50);
    }

    alloc_destroy = (alloc_destroy_func *) GetProcAddress(library, "allocator_destroy");
    if (!alloc_destroy) {
        write(STDERR_FILENO, "Error with importing allocator_destroy has occurred\n", 53);
    }

    void *memory = VirtualAlloc(NULL, 1024 * 1024 * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);  // 4 MB

    Allocator *allocator = alloc_create(memory, 1024 * 20);
    if (!allocator) {
        VirtualFree(memory, 0, MEM_RELEASE);
        FreeLibrary(library);
        return 2;
    }

    char *allocated_mem = (char *) alloc_alloc(allocator, 530);
    if (!allocated_mem) {
        alloc_destroy(allocator);
        VirtualFree(memory, 0, MEM_RELEASE);
        FreeLibrary(library);
        return 3;
    }

    char *allocated_mem2 = (char *) alloc_alloc(allocator, 32);
    if (!allocated_mem2) {
        alloc_destroy(allocator);
        VirtualFree(memory, 0, MEM_RELEASE);
        FreeLibrary(library);
        return 3;
    }

    allocated_mem[0] = '1';
    allocated_mem[1] = '\0';
    allocated_mem2[1] = '\0';
    allocated_mem2[0] = 'l';

    write(STDOUT_FILENO, allocated_mem, 2);
    write(STDOUT_FILENO, allocated_mem2, 2);

    alloc_free(allocator, allocated_mem2);
    alloc_free(allocator, allocated_mem);

    alloc_destroy(allocator);

    FreeLibrary(library);

    VirtualFree(memory, 0, MEM_RELEASE);

    return 0;
}
