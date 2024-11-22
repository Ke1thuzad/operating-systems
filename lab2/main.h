#ifndef OPERATIONAL_SYSTEMS_MAIN_H
#define OPERATIONAL_SYSTEMS_MAIN_H

#include "../utility/error_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

typedef struct Data {
    int size;
    int *array;
} Data;

void *max_min_arr(void *x);

#endif //OPERATIONAL_SYSTEMS_MAIN_H
