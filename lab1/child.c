#include "main.h"

int main() {
    char ch = seek_char(STDIN_FILENO);
    char str[15];
    int val, i = 0, sum = 0;
    int nl = 0;

    while (ch != -1) {
        if (nl || ch == '\n') {
            int j = 0, sign = 1;
            char buf[16] = "0";

            if (sum < 0) {
                sum *= -1;
                sign = -1;
            }

            while (sum > 0) {
                buf[j++] = sum % 10 + '0';
                sum /= 10;
            }
            if (j == 0)
                j++;

            buf[j] = '\0';

            int size = len(buf);
            for (int k = 0; k < size / 2; ++k) {
                char temp = buf[k];
                buf[k] = buf[size - k - 1];
                buf[size - k - 1] = temp;
            }
            if (sign == -1)
                print("-");

            print(buf);
            sum = 0;
            nl = 0;
        } else {
            i++;
            nread_value(STDIN_FILENO, str, 15, ch, &nl);
            parse_int(str, &val);
            sum += val;
        }

        if (!nl) {
            str[0] = '\0';
            ch = seek_char_nl(STDIN_FILENO);
        }
    }

    return 0;
}

int nread_value(int fd, char* result, int n, char first, int* is_newline) {
    *is_newline = 0;
    int i = 0;
    if (first)
        result[i++] = first;

    char character = getchr_fd(fd);
    for (; i < n; ++i) {
        if (character <= ' ') {
            result[i] = '\0';
            if (character == '\n')
                *is_newline = 1;
            return 0;
        }
        result[i] = character;
        character = getchr_fd(fd);
    }
    result[i] = '\0';

    return 0;
}