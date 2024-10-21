#include "array.h"

int create_arr(unsigned int length, Array* arr) {

    arr->capacity = length;
    arr->length = 0;
    arr->val = (char*) malloc(length);


    if (!arr->val) return throw_err(MEMORY_NOT_ALLOCATED);
    arr->val[0] = '\0';

    return 0;
}

int append(Array* arr, char value) {
    if (arr->length >= (arr->capacity - 2)) {
        int err = extend(arr);
        if (err)
            return err;
    }

    arr->val[arr->length++] = value;
    arr->val[arr->length] = '\0';
    return 0;
}

int resize(Array* arr, int size_delta) {
    char* new_addr = (char*) realloc(arr->val, arr->capacity + size_delta);

    if (!new_addr) {
        free(arr->val);
        return throw_err(MEMORY_NOT_ALLOCATED);
    }

    arr->val = new_addr;
    arr->capacity += size_delta;
    return 0;
}

// Double array length.
int extend(Array* arr) {
    return resize(arr, arr->capacity);
}

void destroy(Array* arr) {
    if (arr->val)
        free(arr->val);
    arr->val = NULL;
    arr->length = 0;
    arr->capacity = 0;
}
//
//int copy(Array* dst, Array* src) {
//    destroy(dst);
//    dst->val = (char*) calloc(src->capacity, sizeof(char));
//    if (!dst->val) {
//        return throw_err(MEMORY_NOT_ALLOCATED);
//    }
//    dst->length = src->length;
//    dst->capacity = src->capacity;
//
//    char* tempdst = dst->val, *tempsrc = src->val;
//    while ((*dst->val++ = *src->val++));
//    dst->val = tempdst;
//    src->val = tempsrc;
//    return 0;
//}
//
//void print_arr(const Array arr) {
//    for (int i = 0; i < arr.length; ++i) {
//        printf("%c ", arr.val[i]);
//    }
//    printf("\n");
//}
//
//void reverse_print_arr(const Array arr) {
//    for (int i = arr.length - 1; i >= 0; --i) {
//        printf("%u", arr.val[i]);
//    }
//}
//
//int reverse(Array* arr) {
//    int size = arr->length;
//    for (int i = 0; i < size / 2; ++i) {
//        char temp = arr->val[i];
//        arr->val[i] = arr->val[size - i - 1];
//        arr->val[size - i - 1] = temp;
//    }
//
//    return 0;
//}
//
//int value_to_arr(unsigned int value, Array* result) {
//    if (result)
//        destroy(result);
//    int err = create_arr(10, result);
//    if (err)
//        return err;
//    while (value > 0) {
//        err = append(result, value % 10 + '0');
//        if (err) {
//            destroy(result);
//            return err;
//        }
//        value /= 10;
//    }
//    reverse(result);
//    return 0;
//}
//
//int str_to_arr(char* str, Array* result) {
//    if (result)
//        destroy(result);
//    int err = create_arr(10, result), i = 0;
//    if (err)
//        return err;
//    while (str[i]) {
//        err = append(result, str[i++]);
//        if (err)
//            return err;
//    }
//    return 0;
//
//}
//
//void arr_to_value(Array arr, unsigned int* result) {
//    *result = 0;
//    for (int i = 0; i < arr.length; ++i) {
//        *result *= 10;
//        *result += arr.val[i] - '0';
//    }
//}
//
//int add(const Array A, unsigned int B, Array* result) {
//    if (result)
//        destroy(result);
//    int err = create_arr(5, result);
//    if (err)
//        return err;
//
//    int i = 0, sum = 0, shift = 0;
//
//    while(B > 0) {
//        if (A.val[i])
//            sum = A.val[i++] - '0' + B % 10 + shift;
//        else
//            sum = B % 10 + shift;
//        shift = sum / 10;
//        append(result, sum % 10 + '0');
//        B /= 10;
//    }
//
//    while (shift > 0) {
//        append(result, shift % 10 + '0');
//        shift /= 10;
//    }
//    return 0;
//}
//
//int add_arrays(const Array A, const Array B, Array* result) {
//    if (result)
//        destroy(result);
//    int err = create_arr(5, result);
//    if (err)
//        return err;
//
//    int i = 0, sum = 0, shift = 0;
//
//    while(A.val[i] && B.val[i]) {
//        sum = A.val[i] - '0' + B.val[i] - '0' + shift;
//        shift = sum / 10;
//        append(result, sum % 10 + '0');
//        i++;
//    }
//    int j = i;
//    while (A.val[i]) {
//        sum = A.val[i] - '0' + shift;
//        shift = sum / 10;
//        append(result, sum % 10 + '0');
//        i++;
//    }
//    while (B.val[j]) {
//        sum = B.val[j] - '0' + shift;
//        shift = sum / 10;
//        append(result, sum % 10 + '0');
//        j++;
//    }
//
//    while (shift > 0) {
//        append(result, shift % 10 + '0');
//        shift /= 10;
//    }
//    return 0;
//}
//
//int multiply(const Array A, float B, Array* result) {
//    if (result)
//        destroy(result);
//    int err = create_arr(5, result);
//    if (err) {
//        destroy(result);
//        return err;
//    }
//    int shift = 0;
//
//    for (int i = 0; i < A.length; i++) {
//        int product = (A.val[i] - '0') * B + shift;
//        shift = product / 10;
//        err = append(result, product % 10 + '0');
//        if (err) {
//            destroy(result);
//            return err;
//        }
//    }
//
//    while (shift > 0) {
//        err = append(result, shift % 10 + '0');
//        if (err) {
//            destroy(result);
//            return err;
//        }
//        shift /= 10;
//    }
//
//
//    return 0;
//}
//
//// Concatenate B to A.
//int concat(Array* A, Array B) {
//    for (int i = 0; i < B.length; ++i) {
//        int err = append(A, B.val[i]);
//        if (err)
//            return err;
//    }
//
//    return 0;
//}
//
//// Concatenate B to A.
//int concat_str(Array* A, char* B) {
//    int i = 0;
//    while (B[i] != '\0') {
//        int err = append(A, B[i++]);
//        if (err)
//            return err;
//    }
//
//    return 0;
//}