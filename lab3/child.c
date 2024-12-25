#include "main.h"

#define SHARED_MEM_NAME "/shared_memory_example"
#define SHARED_MEM_SIZE 4096

int main() {
    int shm_fd = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        exit(1);
    }

    char *shared_memory = mmap(NULL, SHARED_MEM_SIZE,
                               PROT_READ | PROT_WRITE,
                               MAP_SHARED, shm_fd, 0);

    if (shared_memory == MAP_FAILED) {
        exit(1);
    }

    char ch = seek_char(STDIN_FILENO);
    char str[16] = "";
    int val, i = 0, sum = 0;
    int nl = 0;
    int write_pos = 0;

    while (ch != -1) {
        if (nl || ch == '\n') {
            if (i > 0) {
                int j = 0, sign = 1;
                char buf[16] = "";

                if (sum < 0) {
                    sum *= -1;
                    sign = -1;
                }

                do {
                    buf[j++] = sum % 10 + '0';
                    sum /= 10;
                } while (sum > 0);

                if (j == 0) j++;
                buf[j] = '\0';

                int size = len(buf);
                for (int k = 0; k < size / 2; ++k) {
                    char temp = buf[k];
                    buf[k] = buf[size - k - 1];
                    buf[size - k - 1] = temp;
                }

                if (sign == -1) {
                    shared_memory[write_pos++] = '-';
                }

                for (int k = 0; k < size; k++) {
                    shared_memory[write_pos++] = buf[k];
                }

                shared_memory[write_pos++] = '\n';
            }

            sum = 0;
            i = 0;
            nl = 0;
        } else {
            i++;
            if (nread_value(STDIN_FILENO, str, 15, ch, &nl) == -1)
                break;
            parse_int(str, &val);
            sum += val;
        }

        if (!nl) {
            str[0] = '\0';
            ch = seek_char_nl(STDIN_FILENO);
        }
    }

    shared_memory[write_pos] = '\0';

    munmap(shared_memory, SHARED_MEM_SIZE);
    close(shm_fd);

    return 0;
}

int nread_value(int fd, char *result, int n, char first, int *is_newline) {
    *is_newline = 0;
    int i = 0;
    if (first != -1)
        result[i++] = first;

    char character;
    for (; i < n; ++i) {
        character = getchr_fd(fd);
        if (character == -1)
            return -1;
        if (character <= ' ') {
            result[i] = '\0';
            if (character == '\n')
                *is_newline = 1;
            return 0;
        }
        result[i] = character;
    }
    result[i] = '\0';
    return 0;
}