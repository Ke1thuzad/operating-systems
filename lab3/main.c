#include <sys/wait.h>
#include "main.h"

#define SHARED_MEM_NAME "/shared_memory_example"
#define SHARED_MEM_SIZE 4096

int main(int argc, char** argv) {
    char ch = seek_char(STDIN_FILENO);

    Array filename;
    create_arr(5, &filename);
    read_value(STDIN_FILENO, &filename, ch);

    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        return throw_err(SHARED_MEMORY_ERROR);
    }

    if (ftruncate(shm_fd, SHARED_MEM_SIZE) == -1) {
        shm_unlink(SHARED_MEM_NAME);
        return throw_err(SHARED_MEMORY_ERROR);
    }

    char *shared_mem = mmap(NULL, SHARED_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_mem == MAP_FAILED) {
        shm_unlink(SHARED_MEM_NAME);
        return throw_err(MEMORY_NOT_ALLOCATED);
    }

    pid_t pid;
    if ((pid = fork()) < 0) {
        shm_unlink(SHARED_MEM_NAME);
        munmap(shared_mem, SHARED_MEM_SIZE);
        return throw_err(FORK_ERROR);
    }

    if (pid == 0) {
        int inpfd = open(filename.val, O_RDONLY);
        destroy(&filename);
        if (inpfd == -1) {
            exit(FILE_ERROR);
        }

        dup2(inpfd, STDIN_FILENO);
        close(inpfd);

        char* args[] = {"./child", NULL};
        if (execvp(args[0], args) == -1) {
            exit(FORK_ERROR);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);

        print(shared_mem);

        close(shm_fd);
        shm_unlink(SHARED_MEM_NAME);
        munmap(shared_mem, SHARED_MEM_SIZE);
        destroy(&filename);
    }

    exit(0);
}

int read_value(int fd, Array* result, char first) {
    if (first != -1)
        append(result, first);

    char character;
    while ((character = getchr_fd(fd)) > ' ') {
        append(result, character);
    }

    return 0;
}