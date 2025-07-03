#include "../../include/cstl/cstl_vector.h"
#include "cstl_vector_internal.h"

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

cstl_vector *cstl_vec_create(const void *data, size_t elem_size) {

  if (data == NULL || elem_size == 0) {
    return NULL;
  }

  cstl_vector *v = cstl_vec_create_empty();

  if (v == NULL) {
    return NULL;
  }

  return cstl_vec_push_back(v, data, elem_size);
}

cstl_vector *cstl_vec_create_copy(cstl_vector *v) {

  if (v == NULL) {
    return NULL;
  }

  cstl_vector *v_copy = cstl_vec_create_empty();

  v_copy->size = v->size;
  v_copy->capacity = v->capacity;

  if (v->data != NULL) {
    memcpy(v_copy->data, v->data, v->size * v->elem_size);
  }

  return v_copy;
}

size_t cstl_vec_size(cstl_vector *v) { return v->size; }

size_t cstl_vec_capacity(cstl_vector *v) { return v->capacity; }

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

  if (v == NULL) {
    return NULL;
  }

  return v->data;
}

int cstl_vec_set(cstl_vector *v, size_t index, const void *elem_val) {

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

cstl_vector *cstl_vec_push_back(cstl_vector *v, const void *data, size_t elem_size) {

  if (v == NULL || data == NULL || elem_size == 0) {
    return NULL;
  }

  if (v->size == v->capacity) {
    size_t new_capacity = (v->capacity > 0) ? v->capacity * 2 : 1;
    if (cstl_vec_resize(v, new_capacity) == NULL) {
      return NULL;
    }
  }

  void *data_copy = _cstl_vec_deep_copy(data, elem_size);

  if (data_copy == NULL) {
    return NULL;
  }

  void *dest = v->data;

  if (v->size != 0) {
    dest = (char *)v->data + v->size * elem_size;
  }

  memcpy(dest, data_copy, elem_size);

  // TODO: need better option for elem_size
  v->elem_size = elem_size;
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

bool cstl_vec_is_empty(cstl_vector *v) { return !v->size; }

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

cstl_vector *cstl_vec_insert(cstl_vector *v, size_t pos, const void *data, size_t elem_size) {

  if (v == NULL || data == NULL || pos > v->size) {
    return NULL;
  }

  if (v->size + 1 > v->capacity) {
    size_t new_capacity = (v->capacity > 0) ? v->capacity * 2 : 1;
    cstl_vec_resize(v, new_capacity);
  }

  if (pos == v->size) {
    return cstl_vec_push_back(v, data, elem_size);
  }

  void *dest = (char *)v->data + (pos + 1) * v->elem_size;
  void *src = (char *)v->data + pos * v->elem_size;
  size_t move_size = (v->size - pos) * v->elem_size;
  memmove(dest, src, move_size);

  void *data_copy = _cstl_vec_deep_copy(data, elem_size);
  
  if(data_copy == NULL){
    return NULL;
  }

  void *dest_insert = (char *)v->data + pos * v->elem_size;
  memcpy(dest_insert, data, v->elem_size);

  v->size++;

  return v;
}

cstl_vector *cstl_vec_erase(cstl_vector *v, size_t index) {

  if (v == NULL || v->size == 0 || index >= v->size) {
    return NULL;
  }

  if (index == v->size - 1) {
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

cstl_vector *cstl_vec_insert_range(cstl_vector *v, const void *range,
                                   size_t range_size, size_t index) {

  if (v == NULL || range == NULL || index > v->size) {
    return NULL;
  }

  if (range_size == 0) {
    return v;
  }

  size_t new_size = v->size + range_size;

  if (new_size > v->capacity) {

    size_t new_capacity = v->capacity;

    while (new_capacity < new_size) {
      new_capacity = (new_capacity > 0) ? new_capacity * 2 : 1;
    }

    if (cstl_vec_resize(v, new_capacity) == NULL) {
      return NULL;
    }
  }

  void *dest = (char *)v->data + (index + range_size) * v->elem_size;
  void *src = (char *)v->data + index * v->elem_size;
  size_t move_size = (v->size - index) * v->elem_size;
  memmove(dest, src, move_size);

  void *range_copy = _cstl_vec_deep_copy(range, range_size);

  if(range_copy == NULL){
    return NULL;
  }

  void *dest_r = (char *)v->data + index * v->elem_size;
  memcpy(dest_r, range_copy, range_size * v->elem_size);

  v->size = new_size;

  return v;
}

cstl_vector *cstl_vec_erase_range(cstl_vector *v, size_t index, size_t len) {

  if (v == NULL || index > v->size || index + len > v->size) {
    return NULL;
  }

  if (len == 0) {
    return v;
  }

  if (index + len == v->size) {
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
