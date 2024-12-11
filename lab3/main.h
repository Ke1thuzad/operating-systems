#ifndef OPERATIONAL_SYSTEMS_MAIN_H
#define OPERATIONAL_SYSTEMS_MAIN_H

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "../utility/array.h"
#include "../utility/error_handler.h"
#include "../utility/universal.h"

int read_value(int fd, Array* result, char first);
int nread_value(int fd, char* result, int n, char first, int* is_newline);

#endif //OPERATIONAL_SYSTEMS_MAIN_H
