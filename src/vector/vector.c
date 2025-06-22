#include "../../include/cstl/vector.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

cstl_vector *vec_create(size_t vec_capacity, size_t elem_size) {

  if (vec_capacity <= 0 || elem_size <= 0) {
    return NULL;
  }

  cstl_vector *v = malloc(sizeof(cstl_vector));

  if (v == NULL) {
    return NULL;
  }

  v->size = 0;
  v->elem_size = elem_size;
  v->data = NULL;
  v->capacity = vec_capacity;

  return v;
}

cstl_vector *vec_create_copy(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  cstl_vector *v_copy = vec_create(v->capacity, v->elem_size);

  v_copy->size = v->size;
  void *data_copy = NULL;

  if (v->data != NULL) {
    data_copy = malloc(v_copy->size * v_copy->elem_size);
    memcpy(data_copy, v->data, v->size * v->elem_size);
  }

  v_copy->data = data_copy;
  return v_copy;
}

cstl_vector *vec_create_filled(size_t vec_capacity, size_t elem_size,
                               void *init_val) {

  cstl_vector *v = vec_create(vec_capacity, elem_size);

  if (v == NULL) {
    return NULL;
  }

  void *data = malloc(elem_size * vec_capacity);

  if (data == NULL) {
    free(v);
    return NULL;
  }

  for (size_t i = 0; i < vec_capacity; ++i) {
    void *elem_ptr = (char *)data + i * elem_size;
    memcpy(elem_ptr, init_val, elem_size);
  }

  v->data = data;
  v->capacity = vec_capacity;
  v->size = vec_capacity;

  return v;
}

size_t vec_size(cstl_vector *v) { return v->size; }

void *vec_front(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  return v->data;
}

void *vec_back(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  return (char *)v->data + (v->size - 1) * v->elem_size;
}

void *vec_get(cstl_vector *v, size_t index) {

  if (v == NULL || index >= v->size) {
    return NULL;
  }

  return (char *)v->data + index * v->elem_size;
}

int vec_set(cstl_vector *v, size_t index, void *elem_val) {

  if (v == NULL) {
    return -1;
  }

  if (index >= v->size) {
    return -2;
  }

  void *dest = (char *)v->data + index * v->elem_size;
  memcpy(dest, elem_val, v->elem_size);
  return 0;
}

cstl_vector *vec_resize(cstl_vector *v, size_t new_capacity) {

  if (v == NULL) {
    return NULL;
  }

  if (new_capacity == v->capacity) {
    return v;
  }

  void *n_ptr = malloc(new_capacity * v->elem_size);

  v->size = (v->size >= new_capacity) ? new_capacity : v->size;
  v->capacity = new_capacity;

  memmove(n_ptr, v->data, v->size * v->elem_size);

  free(v->data);
  v->data = n_ptr;

  return v;
}

cstl_vector *vec_push_back(cstl_vector *v, void *elem_val) {

  if (v == NULL || elem_val == NULL) {
    return NULL;
  }

  if (v->size == v->capacity) {
    size_t new_capacity = (v->capacity > 0) ? v->capacity * 2 : 1;
    if (vec_resize(v, new_capacity) == NULL) {
      return NULL;
    }
  }

  vec_set(v, v->size, elem_val);
  v->size++;

  return v;
}

cstl_vector *vec_pop_back(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  if (v->size == 0) {
    return v;
  }

  v->size--;

  return v;
}

bool vec_is_empty_arr(cstl_vector *arr) {
  return (arr->size > 0) ? false : true;
}

cstl_vector *vec_shrink_to_fit(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  if (v->size == v->capacity) {
    return v;
  }

  return vec_resize(v, v->size);
}

cstl_vector *vec_clear(cstl_vector *v) {
  vec_free(v);
  v->data = NULL;
  v->size = 0;
  return v;
}

cstl_vector *vec_insert(cstl_vector *v, size_t index, void *elem_val) {

  if (v == NULL || elem_val == NULL || index > v->size) {
    return NULL;
  }

  if (v->size + 1 > v->capacity) {
    size_t new_capacity = (v->capacity > 0) ? v->capacity * 2 : 1;
    vec_resize(v, new_capacity);
  }

  void *dest = (char *)v->data + (index + 1) * v->elem_size;
  void *src = (char *)v->data + index * v->elem_size;
  size_t move_size = (v->size - index) * v->elem_size;
  memmove(dest, src, move_size);

  return v;
}

cstl_vector *vec_erase(cstl_vector *v, size_t index) {

  if (v == NULL || v->size == 0 || index >= v->size) {
    return NULL;
  }

  void *dest = (char *)v->data + index * v->elem_size;
  void *src = (char *)v->data + (index + 1) * v->elem_size;
  size_t move_size = (v->size - index) * v->elem_size;
  memmove(dest, src, move_size);

  v->size--;

  return v;
}

cstl_vector *vec_insert_range(cstl_vector *v, void *range, size_t range_size,
                              size_t index) {

  if (v == NULL || range == NULL || index > v->size) {
    return NULL;
  }

  size_t new_size = v->size + range_size;

  if (new_size > v->capacity) {
    size_t new_capacity = (v->capacity > 0) ? v->capacity * 2 : 1;
    vec_resize(v, new_capacity);
  }

  void *dest = (char *)v->data + (index + range_size) * v->elem_size;
  void *src = (char *)v->data + index * v->elem_size;
  size_t move_size = (v->size - index) * v->elem_size;
  memmove(dest, src, move_size);

  void *dest_r = (char *)v->data + index * v->elem_size;
  memcpy(dest_r, range, range_size);

  v->size += range_size;

  return v;
}

cstl_vector *vec_erase_range(cstl_vector *v, size_t index, size_t len) {

  if (v == NULL || index > v->size) {
    return NULL;
  }

  void *dest = (char *)v->data + index * v->elem_size;
  void *src = (char *)v->data + len * v->elem_size;
  size_t move_size = (v->size - index - len) * v->elem_size;
  memmove(dest, src, move_size);

  v->size -= len;

  return v;
}

void vec_free(cstl_vector *v) { free(v->data); }
