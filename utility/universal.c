#include "universal.h"

char seek_char(int fd) {
    char cur = getchr_fd(fd);
    while(cur > 0) {
        if (cur > ' ') {
            return cur;
        }
        cur = getchr_fd(fd);
    }
    return -1;
}

char seek_char_nl(int fd) {
//    *is_newline = 0;
    char cur = getchr_fd(fd);
    while(cur > 0) {
        if (cur == '\n' || cur > ' ') {
//            if (cur == '\n')
//                *is_newline = 1;
            return cur;
        }
        cur = getchr_fd(fd);
    }
    return -1;
}

int len(const char *str) {
    int i = 0;
    while(str[i++] > 0);
    return i - 1;
}

char getchr() {
    char ch;
    read(STDIN_FILENO, &ch, sizeof(char));
    return ch;
}

char getchr_fd(int fd) {
    char ch;
    read(fd, &ch, sizeof(char));
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

    while (str[i] != '\0') {
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