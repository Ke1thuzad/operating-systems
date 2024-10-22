#include <stdlib.h>
#include "main.h"

int main(int argc, char** argv) {

    int pipe_desc[2];

    if (pipe(pipe_desc) < 0) {
        return throw_err(PIPE_ERROR);
    }

    char ch = seek_char(STDIN_FILENO);

    Array filename;
    create_arr(5, &filename);
    read_value(STDIN_FILENO, &filename, ch);

    pid_t pid;
    if ((pid = fork()) < 0) {
        return throw_err(FORK_ERROR);
    }

    if (pid == 0) {

        int inpfd = open(filename.val, O_RDONLY);
        destroy(&filename);
        if (inpfd == -1)
            return throw_err(FILE_ERROR);

        dup2(inpfd, STDIN_FILENO);
        dup2(pipe_desc[1], STDOUT_FILENO);

        close(pipe_desc[0]);
        close(pipe_desc[1]);
        close(inpfd);

        char* args[] = {"./child", NULL};
        if (execvp(args[0], args))
            return throw_err(FORK_ERROR);
    } else {
        close(pipe_desc[1]);
    }

    while (1) {
        char buff[100] = "";
        if(!read(pipe_desc[0], buff, 100))
            break;

        print(buff);
        print("\n");
        buff[0] = '\0';
    }

    return 0;

}

int read_value(int fd, Array* result, char first) {
    if (first)
        append(result, first);

    char character = getchr_fd(fd);
    while (character > ' ') {
        append(result, (char)character);
        character = getchr_fd(fd);
    }

    return 0;
}