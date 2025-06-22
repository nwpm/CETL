#include "../../include/cstl/vector.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vector *vec_create(size_t vec_capacity, size_t elem_size) {

  vector *v = malloc(sizeof(vector));

  if (v == NULL) {
    return NULL;
  }

  void *ptr = malloc(vec_capacity * elem_size);

  if (ptr == NULL) {
    free(v);
    return NULL;
  }

  v->size = 0;
  v->elem_size = elem_size;
  v->data = ptr;
  v->capacity = vec_capacity;

  return v;
}

vector *vec_create_copy(vector *v) {

  if (v == NULL) {
    return NULL;
  }

  vector *v_copy = vec_create(v->capacity, v->elem_size);

  v_copy->size = v->size;

  void *data_copy = malloc(v_copy->capacity * v_copy->elem_size);
  memcpy(data_copy, v->data, v->size * v->elem_size);

  v_copy->data = data_copy;
  return v_copy;
}

vector *vec_create_filled(size_t vec_capacity, size_t elem_size,
                          void *init_val) {

  vector *v = vec_create(vec_capacity, elem_size);

  if (v == NULL) {
    return NULL;
  }

  for (size_t i = 0; i < vec_capacity; ++i) {
    void *elem_ptr = (char *)v->data + i * elem_size;
    memcpy(elem_ptr, init_val, elem_size);
  }

  v->capacity = vec_capacity;
  v->size = vec_capacity;

  return v;
}

size_t vec_size(vector *v) {

  return v->size;
}

void *vec_front(vector *v) {

  if (v == NULL) {
    return NULL;
  }

  return v->data;
}

void *vec_back(vector *v) {

  if (v == NULL) {
    return NULL;
  }

  return (char *)v->data + (v->size - 1) * v->elem_size;
}

void *vec_get(vector *v, size_t index) {

  if (v == NULL || index >= v->size) {
    return NULL;
  }

  return (char *)v->data + index * v->elem_size;
}

int vec_set(vector *v, size_t index, void *elem_val) {

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

vector *vec_resize(vector *v, size_t new_capacity) {

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

vector *vec_push_back(vector *v, void *elem_val) {

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

vector *vec_pop_back(vector *v) {

  if (v == NULL) {
    return NULL;
  }

  if (v->size == 0) {
    return v;
  }

  v->size--;

  return v;
}

bool vec_is_empty_arr(vector *arr) { return (arr->size > 0) ? false : true; }

vector *vec_shrink_to_fit(vector *v) {

  if (v == NULL) {
    return NULL;
  }

  if (v->size == v->capacity) {
    return v;
  }

  return vec_resize(v, v->size);
}

vector *vec_clear(vector *v) {
  vec_free(v);
  v->data = NULL;
  v->size = 0;
  return v;
}

vector *vec_insert(vector *v, size_t index, void *elem_val) {

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

vector *vec_erase(vector *v, size_t index) {

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

vector *vec_insert_range(vector *v, void *range, size_t range_size,
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

vector *vec_erase_range(vector *v, size_t index, size_t len) {

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

void vec_free(vector *v) { free(v->data); }
