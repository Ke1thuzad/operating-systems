#include "main.h"

int main() {
    char ch = seek_char(STDIN_FILENO);
    char str[15];
    int val, i = 0, sum = 0;
    while (ch != -1) {
        i++;
        nread_value(STDIN_FILENO, str, 15, ch);
        parse_int(str, &val);
        sum += val;
        if (i % 3 == 0) {
            int j = 0;
            char buf[15];
            while (sum > 0) {
                buf[j++] = sum % 10 + '0';
                sum /= 10;
            }
            buf[j] = '\0';
            print(buf);
            sum = 0;
        }
        ch = seek_char(STDIN_FILENO);
    }

    return 0;
}

int nread_value(int fd, char* result, int n, char first) {
    int i = 0;
    if (first)
        result[i++] = first;

    char character = getchr_fd(fd);
    for (; i < n; ++i) {
        if (character <= ' ')
            return 0;
        result[i] = character;
    }

    return 0;
}