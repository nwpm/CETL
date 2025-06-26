#include "../../include/cstl/cstl_vector.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// TODO: rename to create empty
cstl_vector *cstl_vec_create(size_t vec_capacity, size_t elem_size) {

  if (vec_capacity <= 0 || elem_size <= 0) {
    return NULL;
  }

  cstl_vector *v = malloc(sizeof(cstl_vector));

  if (v == NULL) {
    return NULL;
  }

  void *data = malloc(vec_capacity * elem_size);

  if (data == NULL) {
    free(v);
    return NULL;
  }

  v->size = 0;
  v->elem_size = elem_size;
  v->data = data;
  v->capacity = vec_capacity;

  return v;
}

// TODO: delete
cstl_vector *cstl_vec_create_empty() {

  cstl_vector *v = malloc(sizeof(cstl_vector));

  if (v == NULL) {
    return NULL;
  }

  v->size = 0;
  v->elem_size = 0;
  v->data = NULL;
  v->capacity = 0;

  return v;
}

cstl_vector *cstl_vec_create_copy(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  cstl_vector *v_copy = cstl_vec_create(v->capacity, v->elem_size);

  v_copy->size = v->size;

  if (v->data != NULL) {
    memcpy(v_copy->data, v->data, v->size * v->elem_size);
  } else {
    cstl_vec_free(v_copy);
    v_copy->data = NULL;
  }

  return v_copy;
}

cstl_vector *cstl_vec_create_filled(size_t vec_capacity, size_t elem_size,
                                    void *init_val) {

  cstl_vector *v = cstl_vec_create(vec_capacity, elem_size);

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

size_t cstl_vec_size(cstl_vector *v) { 
  return v->size; 
}

size_t cstl_vec_capacity(cstl_vector *v) { 
  return v->capacity; 
}

void *cstl_vec_front(cstl_vector *v) {

  if (v == NULL || v->size == 0 || v->data == NULL) {
    return NULL;
  }

  return v->data;
}

void *cstl_vec_back(cstl_vector *v) {

  if (v == NULL || v->size == 0 || v->data == NULL) {
    return NULL;
  }

  return (char *)v->data + (v->size - 1) * v->elem_size;
}

void *cstl_vec_get(cstl_vector *v, size_t index) {

  if (v == NULL || index >= v->size || v->size == 0 || v->data == NULL) {
    return NULL;
  }

  return (char *)v->data + index * v->elem_size;
}

void *cstl_vec_data(cstl_vector *v) { 
  
  if(v == NULL){
    return NULL;
  }

  return v->data; 
}

int cstl_vec_set(cstl_vector *v, size_t index, void *elem_val) {

  if (v == NULL || v->data == NULL || elem_val == NULL) {
    return -1;
  }

  if (index >= v->size) {
    return -2;
  }

  void *dest = (char *)v->data + index * v->elem_size;
  memcpy(dest, elem_val, v->elem_size);
  return 0;
}

cstl_vector *cstl_vec_resize(cstl_vector *v, size_t new_capacity) {

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

cstl_vector *cstl_vec_push_back(cstl_vector *v, void *elem_val) {

  if (v == NULL || elem_val == NULL) {
    return NULL;
  }

  if (v->size == v->capacity) {
    size_t new_capacity = (v->capacity > 0) ? v->capacity * 2 : 1;
    if (cstl_vec_resize(v, new_capacity) == NULL) {
      return NULL;
    }
  }

  void *dest = v->data;

  if (v->size != 0) {
    dest = (char *)v->data + v->size * v->elem_size;
  }

  memcpy(dest, elem_val, v->elem_size);

  v->size++;

  return v;
}

cstl_vector *cstl_vec_pop_back(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  if (v->size == 0) {
    return v;
  }

  v->size--;

  return v;
}

bool cstl_vec_is_empty(cstl_vector *v) {
  return !v->size;
}

cstl_vector *cstl_vec_shrink_to_fit(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  if (v->size == v->capacity) {
    return v;
  }

  return cstl_vec_resize(v, v->size);
}

cstl_vector *cstl_vec_clear(cstl_vector *v) {
  cstl_vec_free(v);
  v->data = NULL;
  v->size = 0;
  return v;
}

cstl_vector *cstl_vec_insert(cstl_vector *v, size_t index, void *elem_val) {

  if (v == NULL || elem_val == NULL || index > v->size) {
    return NULL;
  }

  if (v->size + 1 > v->capacity) {
    size_t new_capacity = (v->capacity > 0) ? v->capacity * 2 : 1;
    cstl_vec_resize(v, new_capacity);
  }

  if (index == v->size) {
    cstl_vec_push_back(v, elem_val);
    return v;
  }

  void *dest = (char *)v->data + (index + 1) * v->elem_size;
  void *src = (char *)v->data + index * v->elem_size;
  size_t move_size = (v->size - index) * v->elem_size;
  memmove(dest, src, move_size);

  void *dest_insert = (char *)v->data + index * v->elem_size;
  memcpy(dest_insert, elem_val, v->elem_size);

  v->size++;

  return v;
}

cstl_vector *cstl_vec_erase(cstl_vector *v, size_t index) {

  if (v == NULL || v->size == 0 || index >= v->size) {
    return NULL;
  }

  if(index == v->size - 1){
    cstl_vec_pop_back(v);
    return v;
  }

  void *dest = (char *)v->data + index * v->elem_size;
  void *src = (char *)v->data + (index + 1) * v->elem_size;
  size_t move_size = (v->size - index - 1) * v->elem_size;
  memmove(dest, src, move_size);

  v->size--;

  return v;
}

cstl_vector *cstl_vec_insert_range(cstl_vector *v, void *range,
                                   size_t range_size, size_t index) {

  if (v == NULL || range == NULL || index > v->size) {
    return NULL;
  }

  if(range_size == 0){
    return v;
  }

  size_t new_size = v->size + range_size;

  if (new_size > v->capacity) {

    size_t new_capacity = v->capacity;

    while(new_capacity < new_size){
      new_capacity = (new_capacity > 0) ? new_capacity * 2 : 1;
    }

    if(cstl_vec_resize(v, new_capacity) == NULL){
      return NULL;
    }

  }

  void *dest = (char *)v->data + (index + range_size) * v->elem_size;
  void *src = (char *)v->data + index * v->elem_size;
  size_t move_size = (v->size - index) * v->elem_size;
  memmove(dest, src, move_size);

  void *dest_r = (char *)v->data + index * v->elem_size;
  memcpy(dest_r, range, range_size * v->elem_size);

  v->size = new_size;

  return v;
}

cstl_vector *cstl_vec_erase_range(cstl_vector *v, size_t index, size_t len) {

  if (v == NULL || index > v->size || index + len > v->size) {
    return NULL;
  }

  if(len == 0){
    return v;
  }

  if(index + len == v->size){
    v->size -= len;
    return v;
  }

  void *dest = (char *)v->data + index * v->elem_size;
  void *src = (char *)v->data + len * v->elem_size;
  size_t move_size = (v->size - index - len) * v->elem_size;
  memmove(dest, src, move_size);

  v->size -= len;

  return v;
}

void cstl_vec_free(cstl_vector *v) { free(v->data); }
