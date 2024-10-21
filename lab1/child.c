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
        if (i == 3) {
            int j = 0;
            char buf[15];
            while (val > 0) {
                buf[j++] = val % 10 + '0';
                val /= 10;
            }
            print(buf);
            sum = 0;
        }
        ch = seek_char(STDIN_FILENO);
    }

    return 0;
}