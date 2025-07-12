#include "../../../include/cetl/cetl_vector.h"
#include "cetl_vector_internal.h"

#include <stdlib.h>
#include <string.h>

static cetl_void_t _cetl_vec_free_data(cetl_vector *vec) {

  if (vec->type->dtor) {
    for (cetl_size_t i = 0; i < vec->size; ++i) {
      cetl_ptr_t current = (cetl_byte_t *)vec->data + i * vec->type->size;
      vec->type->dtor(current);
    }
  }

  free(vec->data);
}

cetl_vector *cetl_vec_create_empty(const cetl_type *type) {

  if (type == NULL) {
    return NULL;
  }

  cetl_vector *vec = malloc(sizeof(cetl_vector));

  if (vec == NULL) {
    return NULL;
  }

  cetl_byte_t* alloc_data = malloc(CETL_VEC_START_CAPACITY * type->size);

  if (alloc_data == NULL) {
    free(vec);
    return NULL;
  }

  vec->size = 0;
  vec->data = alloc_data;
  vec->capacity = CETL_VEC_START_CAPACITY;
  vec->type = type;

  return vec;
}

cetl_vector *cetl_vec_create_copy(const cetl_vector *src_vec) {

  if (src_vec == NULL) {
    return NULL;
  }

  cetl_vector *new_vec = cetl_vec_create_empty(src_vec->type);

  if (new_vec == NULL || cetl_vec_is_empty(src_vec)) {
    return new_vec;
  }

  cetl_vec_resize(new_vec, src_vec->capacity);

  cetl_byte_t* src_elem = NULL;

  for (cetl_size_t i = 0; i < src_vec->size; ++i) {
    src_elem = src_vec->data + i * src_vec->type->size;
    cetl_vec_push_back(new_vec, src_elem);
  }

  return new_vec;
}

cetl_size_t cetl_vec_size(const cetl_vector *vec) { return vec->size; }

cetl_size_t cetl_vec_capacity(const cetl_vector *vec) { return vec->capacity; }

cetl_ptr_t cetl_vec_front(const cetl_vector *vec) {

  if (vec == NULL || vec->size == 0 || vec->data == NULL) {
    return NULL;
  }

  return vec->data;
}

cetl_ptr_t cetl_vec_back(const cetl_vector *vec) {

  if (vec == NULL || vec->size == 0 || vec->data == NULL) {
    return NULL;
  }

  return vec->data + (vec->size - 1) * vec->type->size;
}

cetl_ptr_t cetl_vec_get(const cetl_vector *vec, cetl_size_t pos) {

  if (vec == NULL || pos >= vec->size || vec->size == 0 || vec->data == NULL) {
    return NULL;
  }

  return vec->data + pos * vec->type->size;
}

cetl_ptr_t cetl_vec_data(const cetl_vector *vec) {

  if (vec == NULL) {
    return NULL;
  }

  return vec->data;
}

cetl_result_t cetl_vec_set(cetl_vector *vec, cetl_size_t pos,
                           cetl_cptr_t new_val) {

  if (vec == NULL || vec->data == NULL || new_val == NULL) {
    return -1;
  }

  if (pos >= vec->size) {
    return -2;
  }

  cetl_byte_t* dest = vec->data + pos * vec->type->size;

  if (vec->type->ctor) {
    vec->type->dtor(dest);
    vec->type->ctor(dest, new_val);
  } else {
    memcpy(dest, new_val, vec->type->size);
  }

  return 0;
}

cetl_vector *cetl_vec_resize(cetl_vector *vec, cetl_size_t new_capacity) {

  if (vec == NULL) {
    return NULL;
  }

  if (new_capacity == vec->capacity) {
    return vec;
  }

  cetl_byte_t* new_ptr =
      (new_capacity == 0) ? NULL : malloc(new_capacity * vec->type->size);

  if (vec->size > new_capacity) {
    if (vec->type->dtor) {
      for (cetl_size_t i = new_capacity; i < vec->size; ++i) {
        cetl_byte_t* current = vec->data + i * vec->type->size;
        vec->type->dtor(current);
      }
    }

    vec->size = new_capacity;
  }

  vec->capacity = new_capacity;

  memmove(new_ptr, vec->data, vec->size * vec->type->size);

  free(vec->data);
  vec->data = new_ptr;

  return vec;
}

cetl_vector *cetl_vec_push_back(cetl_vector *vec, cetl_cptr_t new_val) {

  if (vec == NULL || new_val == NULL) {
    return NULL;
  }

  if (vec->size == vec->capacity) {
    cetl_size_t new_capacity = (vec->capacity > 0)
                                   ? vec->capacity * CETL_VEC_GROW_RATE
                                   : CETL_VEC_START_CAPACITY;
    if (cetl_vec_resize(vec, new_capacity) == NULL) {
      return NULL;
    }
  }

  cetl_byte_t* dest = vec->data + vec->size * vec->type->size;

  if (vec->type->ctor) {
    vec->type->ctor(dest, new_val);
  } else {
    memcpy(dest, new_val, vec->type->size);
  }

  vec->size++;

  return vec;
}

cetl_vector *cetl_vec_pop_back(cetl_vector *vec) {

  if (vec == NULL) {
    return NULL;
  }

  if (vec->size == 0) {
    return vec;
  }

  if (vec->type->dtor) {
    cetl_ptr_t data = cetl_vec_back(vec);
    vec->type->dtor(data);
  }

  vec->size--;

  return vec;
}

cetl_bool_t cetl_vec_is_empty(const cetl_vector *vec) { return !vec->size; }

cetl_vector *cetl_vec_shrink_to_fit(cetl_vector *vec) {

  if (vec == NULL || vec->size == vec->capacity) {
    return vec;
  }

  return cetl_vec_resize(vec, vec->size);
}

cetl_vector *cetl_vec_clear(cetl_vector *vec) {

  if (vec == NULL) {
    return NULL;
  }

  _cetl_vec_free_data(vec);
  vec->data = NULL;
  vec->size = 0;
  return vec;
}

cetl_vector *cetl_vec_insert(cetl_vector *vec, cetl_size_t pos,
                             cetl_cptr_t new_val) {

  if (vec == NULL || new_val == NULL || pos > vec->size) {
    return NULL;
  }

  if (vec->size + 1 > vec->capacity) {
    cetl_size_t new_capacity = (vec->capacity > 0)
                                   ? vec->capacity * CETL_VEC_GROW_RATE
                                   : CETL_VEC_START_CAPACITY;
    cetl_vec_resize(vec, new_capacity);
  }

  if (pos == vec->size) {
    return cetl_vec_push_back(vec, new_val);
  }

  cetl_byte_t* move_next = vec->data + (pos + 1) * vec->type->size;
  cetl_byte_t* move_from = vec->data + pos * vec->type->size;
  cetl_size_t move_size = (vec->size - pos) * vec->type->size;
  memmove(move_next, move_from, move_size);

  cetl_byte_t* insert_to = vec->data + pos * vec->type->size;

  if (vec->type->ctor) {
    vec->type->ctor(insert_to, new_val);
  } else {
    memcpy(insert_to, new_val, vec->type->size);
  }

  vec->size++;

  return vec;
}

cetl_vector *cetl_vec_erase(cetl_vector *vec, cetl_size_t index) {

  if (vec == NULL || vec->size == 0 || index >= vec->size) {
    return NULL;
  }

  if (index == vec->size - 1) {
    cetl_vec_pop_back(vec);
    return vec;
  }

  cetl_byte_t* dest = vec->data + index * vec->type->size;
  cetl_byte_t* src = vec->data + (index + 1) * vec->size;
  cetl_size_t move_size = (vec->size - index - 1) * vec->size;

  if (vec->type->dtor) {
    vec->type->dtor(dest);
  }

  memmove(dest, src, move_size);

  vec->size--;

  return vec;
}

cetl_vector *cetl_vec_insert_range(cetl_vector *vec, cetl_cptr_t range,
                                   cetl_size_t range_size, cetl_size_t pos) {

  if (vec == NULL || range == NULL || pos > vec->size) {
    return NULL;
  }

  if (range_size == 0) {
    return vec;
  }

  cetl_size_t new_size = vec->size + range_size;

  if (new_size > vec->capacity) {

    cetl_size_t new_capacity = vec->capacity;

    while (new_capacity < new_size) {
      new_capacity = (new_capacity > 0) ? new_capacity * CETL_VEC_GROW_RATE
                                        : CETL_VEC_START_CAPACITY;
    }

    if (cetl_vec_resize(vec, new_capacity) == NULL) {
      return NULL;
    }
  }

  cetl_byte_t* dest = vec->data + (pos + range_size) * vec->type->size;
  cetl_byte_t* src = vec->data + pos * vec->type->size;
  cetl_size_t move_size = (vec->size - pos) * vec->type->size;
  memmove(dest, src, move_size);

  if (vec->type->ctor) {

    cetl_size_t j = 0;
    for (cetl_size_t i = pos; i < pos + range_size; ++i) {
      cetl_byte_t* current = vec->data + i * vec->type->size;
      const cetl_byte_t* range_data = (cetl_byte_t*)range + (j++) * vec->type->size;
      vec->type->ctor(current, range_data);
    }

  } else {
    cetl_byte_t* dest_r = vec->data + pos * vec->type->size;
    memcpy(dest_r, range, range_size * vec->type->size);
  }

  vec->size = new_size;

  return vec;
}

cetl_vector *cetl_vec_erase_range(cetl_vector *vec, cetl_size_t pos,
                                  cetl_size_t len) {

  if (vec == NULL || pos > vec->size || pos + len > vec->size) {
    return NULL;
  }

  if (len == 0) {
    return vec;
  }

  if (vec->type->dtor) {
    for (cetl_size_t i = pos; i < (pos + len); ++i) {
      cetl_byte_t* current = vec->data + i * vec->type->size;
      vec->type->dtor(current);
    }
  }

  cetl_byte_t* dest = vec->data + pos * vec->type->size;
  cetl_byte_t* src = vec->data + (pos + len) * vec->type->size;
  cetl_size_t move_size = (pos + len == vec->size) ? 0 : len;
  memmove(dest, src, move_size);

  vec->size -= len;

  return vec;
}

cetl_void_t cetl_vec_free(cetl_vector *vec) {

  if (vec == NULL) {
    return;
  }

  _cetl_vec_free_data(vec);
  free(vec);
}
