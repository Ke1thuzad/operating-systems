#include "main.h"

int main(int argc, char **argv) {
    if (argc != 2)
        return throw_err(INCORRECT_ARGUMENTS);

    int A_size = 1000000;

    int A[A_size];

    srand(time(NULL));

    for (int i = 0; i < A_size; ++i) {
        A[i] = rand() % 1000000 + 1000;
    }

    int thread_count = atoi(argv[1]);

    if (thread_count <= 0 || thread_count > 15)
        return throw_err(INCORRECT_ARGUMENTS);

    int batch_size = A_size / thread_count;

    pthread_t threads[thread_count];
    Data *thread_data[thread_count];

    for (int i = 0; i < thread_count; ++i) {
        Data *data = malloc(sizeof(Data));
        data->size = batch_size;
        data->array = A + (batch_size * i);

        if (i == thread_count - 1)
            data->size += A_size % thread_count;

        pthread_create(threads + i, NULL, max_min_arr, data);
        thread_data[i] = data;
    }

    int min = INT_MAX, max = INT_MIN;

    for (int i = 0; i < thread_count; ++i) {
        int *returned;
        pthread_join(threads[i], (void**) &returned);
        free(thread_data[i]);

        if (!returned) {
            throw_err(MEMORY_NOT_ALLOCATED);
            continue;
        }

        if (returned[0] < min)
            min = returned[0];
        if (returned[1] > max)
            max = returned[1];

        free(returned);
    }

    printf("Min: %d\nMax: %d\n", min, max);

    return 0;
}

void *max_min_arr(void *x) {
    Data *data_ptr = (Data *)x;
    int min = INT_MAX, max = INT_MIN;

    for (int i = 0; i < data_ptr->size; ++i) {
        int value = data_ptr->array[i];

        if (value < min)
            min = value;
        if (value > max)
            max = value;
    }

    int *res = (int *) malloc(sizeof(int) * 2);
    if (!res)
        return NULL;

    res[0] = min;
    res[1] = max;

    return res;
}
