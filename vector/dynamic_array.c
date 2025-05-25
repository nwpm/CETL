#include "dynamic_array.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dynamic_int32_array *arr_create(dynamic_int32_array *arr, size_t arr_capacity) {

  int32_t *ptr = (int32_t *)malloc(arr_capacity * sizeof(int32_t));

  if (ptr == NULL) {
    return NULL;
  }

  arr->size = 0;
  arr->ptr = ptr;
  arr->capacity = arr_capacity;

  return arr;
}

dynamic_int32_array *arr_create_init(dynamic_int32_array *arr,
                                     size_t arr_capacity, int32_t init_val) {

  int32_t *ptr = (int32_t *)malloc(arr_capacity * sizeof(int32_t));

  if (ptr == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < arr_capacity; ++i) {
    ptr[i] = init_val;
  }

  arr->capacity = arr_capacity;
  arr->ptr = ptr;
  arr->size = arr_capacity;

  return arr;
}

dynamic_int32_array *arr_resize(dynamic_int32_array *arr, size_t new_capacity) {

  if (new_capacity == arr->capacity) {
    return arr;
  }

  int32_t *n_ptr = (int32_t *)malloc(new_capacity * sizeof(int32_t));

  arr->size = (arr->size >= new_capacity) ? new_capacity : arr->size;
  arr->capacity = new_capacity;

  memmove(n_ptr, arr->ptr, arr->size * sizeof(int32_t));

  free(arr->ptr);
  arr->ptr = n_ptr;

  return arr;
}

dynamic_int32_array *arr_push_back(dynamic_int32_array *arr, int32_t val) {

  if (arr->size < arr->capacity) {
    arr->ptr[arr->size++] = val;
    return arr;
  }

  arr_resize(arr, arr->capacity * 2);
  arr->ptr[arr->size++] = val;

  return arr;
}

dynamic_int32_array *arr_pop_back(dynamic_int32_array *arr) {

  if (arr->size == 0) {
    return arr;
  }

  arr->size--;

  return arr;
}

bool arr_is_empty_arr(dynamic_int32_array *arr) {
  return (arr->size > 0) ? false : true;
}

void arr_print(dynamic_int32_array *arr) {
  for (size_t i = 0; i < arr->size; ++i) {
    printf("%d\n", arr->ptr[i]);
  }
}

dynamic_int32_array *arr_shrink_to_fit(dynamic_int32_array *arr) {

  if (arr->size == arr->capacity) {
    return arr;
  }

  return arr_resize(arr, arr->size);
}

dynamic_int32_array *arr_clear(dynamic_int32_array *arr) {
  arr_free(arr);
  arr->ptr = NULL;
  arr->size = 0;
  return arr;
}

dynamic_int32_array *arr_insert(dynamic_int32_array *arr, int32_t val,
                                size_t pos) {
  if (pos > arr->size) {
    return NULL;
  }

  if (arr->size + 1 > arr->capacity) {
    arr_resize(arr, arr->capacity * 2);
  }

  /*
    for (size_t i = arr->size; i > pos; --i) {
      arr->ptr[i] = arr->ptr[i - 1];
    }
  */

  memmove(arr->ptr + pos + 1, arr->ptr + pos,
          (arr->size - pos) * sizeof(int32_t));

  arr->ptr[pos] = val;
  arr->size++;

  return arr;
}

dynamic_int32_array *arr_erase(dynamic_int32_array *arr, size_t pos) {

  if (arr->size == 0 || pos >= arr->size) {
    return NULL;
  }

  /*
    for (size_t i = pos; i < arr->size - 1; ++i) {
      arr->ptr[i] = arr->ptr[i + 1];
    }
  */

  memmove(arr->ptr + pos - 1, arr->ptr + pos,
          (arr->size - pos) * sizeof(int32_t));

  arr->size--;

  return arr;
}

dynamic_int32_array *arr_insert_range(dynamic_int32_array *arr, int32_t *range,
                                      size_t range_size, size_t pos) {

  if (pos > arr->size) {
    return NULL;
  }

  size_t new_size = arr->size + range_size;

  if (new_size > arr->capacity) {
    arr->capacity = new_size;
  }

  memmove(arr->ptr + pos + range_size, arr->ptr + pos,
          range_size * sizeof(int32_t));

  memcpy(arr->ptr + pos, range, range_size * sizeof(int32_t));

  arr->size += range_size;

  return arr;
}

dynamic_int32_array *arr_erase_range(dynamic_int32_array *arr, size_t pos,
                                     size_t len) {

  if (pos > arr->size) {
    return NULL;
  }

  memmove(arr->ptr + pos, arr->ptr + len,
          (arr->size - pos - len) * sizeof(int32_t));

  arr->size -= len;

  return arr;
}

void arr_free(dynamic_int32_array *arr) { free(arr->ptr); }

int main(void) {

  dynamic_int32_array arr;
  dynamic_int32_array *p_arr = &arr;

  arr_create(p_arr, 10);
  arr_push_back(p_arr, 1);
  arr_push_back(p_arr, 2);
  arr_push_back(p_arr, 3);

  arr_print(p_arr);
  printf("\n");

  int32_t range[3] = {5, 5, 5};

  arr_insert_range(p_arr, range, 3, 0);

  arr_print(p_arr);
  printf("\n");

  arr_erase_range(p_arr, 0, 2);

  arr_print(p_arr);
  printf("\n");

  arr_free(p_arr);

  return 0;
}
