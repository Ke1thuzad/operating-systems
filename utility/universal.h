#ifndef OPERATIONAL_SYSTEMS_UNIVERSAL_H
#define OPERATIONAL_SYSTEMS_UNIVERSAL_H

#include <unistd.h>

char seek_char(int fd);
char seek_char_nl(int fd);
//char seek_char_nl(int fd, int* is_newline);
int len(const char* str);
char getchr();
char getchr_fd(int fd);
void print(char* str);
int chrtoint(char x, int* num);
int parse_int(char* str, int* res);
int absolute(int x);

#endif //OPERATIONAL_SYSTEMS_UNIVERSAL_H
