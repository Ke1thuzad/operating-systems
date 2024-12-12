#include "universal.h"

char seek_char(int fd) {
    char cur;
    ssize_t bytes_read;
    while ((bytes_read = read(fd, &cur, sizeof(char))) > 0) {
        if (cur > ' ') {
            return cur;
        }
    }
    if (bytes_read == 0 || bytes_read == -1) {
        return -1;
    }
    return -1;
}

char seek_char_nl(int fd) {
    char cur;
    ssize_t bytes_read;
    while ((bytes_read = read(fd, &cur, sizeof(char))) > 0) {
        if (cur == '\n' || cur > ' ') {
            return cur;
        }
    }

    if (bytes_read == 0 || bytes_read == -1) {
        return -1;
    }

    return -1;
}

int len(const char *str) {
    int i = 0;

    while (str[i] != '\0') {
        i++;
    }

    return i;
}

char getchr() {
    char ch;
    ssize_t bytes_read = read(STDIN_FILENO, &ch, sizeof(char));

    if (bytes_read <= 0) {
        return -1;
    }

    return ch;
}

char getchr_fd(int fd) {
    char ch;
    ssize_t bytes_read = read(fd, &ch, sizeof(char));

    if (bytes_read <= 0) {
        return -1;
    }

    return ch;
}

void print(char *str) {
    write(STDOUT_FILENO, str, len(str));
}

int chrtoint(char x, int* num) {
    int a = x - '0';
    if (a < 0 || a > 9) return 1;
    *num = a;
    return 0;
}

int parse_int(char* str, int* res) {
    int i = 0, result = 0, sign = 1;

    if (str[0] == '-') i++, sign = -1;

    while (str[i] > 0) {
        int temp, err = chrtoint(str[i], &temp);
        if (err)
            return err;
        result *= 10;
        result += temp;
        i++;
    }

    *res = sign * result;
    return 0;
}

int absolute(int x) {
    if (x < 0)
        return -x;
    return x;
}