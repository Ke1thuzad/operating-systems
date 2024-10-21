#ifndef OPERATIONAL_SYSTEMS_UNIVERSAL_H
#define OPERATIONAL_SYSTEMS_UNIVERSAL_H

#include <unistd.h>

char seek_char(int fd);
int len(const char* str);
char getchr();
char getchr_fd(int fd);
void print(char* str);
int chrtoint(char x, int* num);
int parse_int(char* str, int* res);

#endif //OPERATIONAL_SYSTEMS_UNIVERSAL_H
