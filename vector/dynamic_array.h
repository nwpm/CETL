#ifndef DYNAMIC_ARR_H
#define DYNAMIC_ARR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
  size_t size;
  size_t capacity;
  int32_t *ptr;
} dynamic_int32_array;

dynamic_int32_array *arr_create(dynamic_int32_array *arr, size_t arr_capacity);
dynamic_int32_array *arr_create_init(dynamic_int32_array *arr,
                                     size_t arr_capacity, int32_t init_val);

dynamic_int32_array *arr_resize(dynamic_int32_array *arr, size_t new_capacity);
dynamic_int32_array *arr_push_back(dynamic_int32_array *arr, int32_t val);
dynamic_int32_array *arr_pop_back(dynamic_int32_array *arr);
dynamic_int32_array* arr_shrink_to_fit(dynamic_int32_array *arr);
dynamic_int32_array* arr_clear(dynamic_int32_array *arr);
dynamic_int32_array* arr_insert(dynamic_int32_array *arr, int32_t val, size_t pos);
dynamic_int32_array* arr_erase(dynamic_int32_array *arr, size_t pos);
dynamic_int32_array* arr_insert_range(dynamic_int32_array *arr, int32_t *range, size_t range_size, size_t pos);
dynamic_int32_array* arr_erase_range(dynamic_int32_array *arr, size_t pos, size_t len);

bool arr_is_empty_arr(dynamic_int32_array *arr);

void arr_print(dynamic_int32_array *arr);

void arr_free(dynamic_int32_array *arr);

#endif // DYNAMIC_ARR_H
