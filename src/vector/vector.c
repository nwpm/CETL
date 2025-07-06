#include "../../include/cstl/cstl_vector.h"
#include "cstl_vector_internal.h"

#include <stdlib.h>
#include <string.h>

cstl_vector *cstl_vec_create_empty(cstl_type *type) {

  if (type == NULL) {
    return NULL;
  }

  cstl_vector *vec = malloc(sizeof(cstl_vector));

  if (vec == NULL) {
    return NULL;
  }

  void *alloc_data = malloc(CSTL_VEC_START_CAPACITY * type->size);

  if (alloc_data == NULL) {
    free(vec);
    return NULL;
  }

  vec->size = 0;
  vec->data = alloc_data;
  vec->capacity = CSTL_VEC_START_CAPACITY;
  vec->type = type;

  return vec;
}

cstl_vector *cstl_vec_create_copy(cstl_vector *vec) {

  if (vec == NULL) {
    return NULL;
  }

  cstl_vector *vec_copy = cstl_vec_create_empty(vec->type);

  cstl_vec_resize(vec_copy, vec->capacity);

  void *current = NULL;

  for (size_t i = 0; i < vec->size; ++i) {
    current = (char *)vec->data + i * vec->type->size;
    cstl_vec_push_back(vec_copy, current);
  }

  return vec_copy;
}

size_t cstl_vec_size(cstl_vector *vec) { return vec->size; }

size_t cstl_vec_capacity(cstl_vector *vec) { return vec->capacity; }

void *cstl_vec_front(cstl_vector *vec) {

  if (vec == NULL || vec->size == 0 || vec->data == NULL) {
    return NULL;
  }

  return vec->data;
}

void *cstl_vec_back(cstl_vector *vec) {

  if (vec == NULL || vec->size == 0 || vec->data == NULL) {
    return NULL;
  }

  return (char *)vec->data + (vec->size - 1) * vec->type->size;
}

void *cstl_vec_get(cstl_vector *vec, size_t pos) {

  if (vec == NULL || pos >= vec->size || vec->size == 0 || vec->data == NULL) {
    return NULL;
  }

  return (char *)vec->data + pos * vec->type->size;
}

void *cstl_vec_data(cstl_vector *vec) {

  if (vec == NULL) {
    return NULL;
  }

  return vec->data;
}

int cstl_vec_set(cstl_vector *vec, size_t pos, const void *elem_val) {

  if (vec == NULL || vec->data == NULL || elem_val == NULL) {
    return -1;
  }

  if (pos >= vec->size) {
    return -2;
  }

  void *dest = (char *)vec->data + pos * vec->type->size;

  if (vec->type->ctor) {
    vec->type->dtor(dest);
    vec->type->ctor(dest, elem_val);
  } else {
    memcpy(dest, elem_val, vec->type->size);
  }

  return 0;
}

cstl_vector *cstl_vec_resize(cstl_vector *vec, size_t new_capacity) {

  if (vec == NULL) {
    return NULL;
  }

  if (new_capacity == vec->capacity) {
    return vec;
  }

  void *n_ptr = malloc(new_capacity * vec->type->size);

  if (vec->size > new_capacity) {

    for (size_t i = new_capacity; i < vec->size; ++i) {
      void *current = (char *)vec->data + i * vec->type->size;
      vec->type->dtor(current);
    }

    vec->size = new_capacity;
  }

  vec->capacity = new_capacity;

  memmove(n_ptr, vec->data, vec->size * vec->type->size);

  free(vec->data);
  vec->data = n_ptr;

  return vec;
}

cstl_vector *cstl_vec_push_back(cstl_vector *vec, const void *data) {

  if (vec == NULL || data == NULL) {
    return NULL;
  }

  if (vec->size == vec->capacity) {
    size_t new_capacity =
        (vec->capacity > 0) ? vec->capacity * 2 : CSTL_VEC_START_CAPACITY;
    if (cstl_vec_resize(vec, new_capacity) == NULL) {
      return NULL;
    }
  }

  void *dest = (char *)vec->data + vec->size * vec->type->size;

  if (vec->type->ctor) {
    vec->type->ctor(dest, data);
  } else {
    memcpy(dest, data, vec->type->size);
  }

  vec->size++;

  return vec;
}

cstl_vector *cstl_vec_pop_back(cstl_vector *vec) {

  if (vec == NULL) {
    return NULL;
  }

  if (vec->size == 0) {
    return vec;
  }

  if (vec->type->dtor) {
    void *data = (char *)vec->data + (vec->size - 1) * vec->type->size;
    vec->type->dtor(data);
  }

  vec->size--;

  return vec;
}

bool cstl_vec_is_empty(cstl_vector *vec) { return !vec->size; }

cstl_vector *cstl_vec_shrink_to_fit(cstl_vector *vec) {

  if (vec == NULL) {
    return NULL;
  }

  if (vec->size == vec->capacity) {
    return vec;
  }

  return cstl_vec_resize(vec, vec->size);
}

cstl_vector *cstl_vec_clear(cstl_vector *vec) {
  _cstl_vec_free_data(vec);
  vec->data = NULL;
  vec->size = 0;
  return vec;
}

cstl_vector *cstl_vec_insert(cstl_vector *vec, size_t pos, const void *data) {

  if (vec == NULL || data == NULL || pos > vec->size) {
    return NULL;
  }

  if (vec->size + 1 > vec->capacity) {
    size_t new_capacity = (vec->capacity > 0) ? vec->capacity * CSTL_VEC_GROW_RATE
                                            : CSTL_VEC_START_CAPACITY;
    cstl_vec_resize(vec, new_capacity);
  }

  if (pos == vec->size) {
    return cstl_vec_push_back(vec, data);
  }

  void *move_next = (char *)vec->data + (pos + 1) * vec->type->size;
  void *move_from = (char *)vec->data + pos * vec->type->size;
  size_t move_size = (vec->size - pos) * vec->type->size;
  memmove(move_next, move_from, move_size);

  void *insert_to = (char *)vec->data + pos * vec->type->size;

  if (vec->type->ctor) {
    vec->type->ctor(insert_to, data);
  } else {
    memcpy(insert_to, data, vec->type->size);
  }

  vec->size++;

  return vec;
}

cstl_vector *cstl_vec_erase(cstl_vector *vec, size_t index) {

  if (vec == NULL || vec->size == 0 || index >= vec->size) {
    return NULL;
  }

  if (index == vec->size - 1) {
    cstl_vec_pop_back(vec);
    return vec;
  }

  void *dest = (char *)vec->data + index * vec->type->size;
  void *src = (char *)vec->data + (index + 1) * vec->size;
  size_t move_size = (vec->size - index - 1) * vec->size;

  if (vec->type->dtor) {
    vec->type->dtor(dest);
  }

  memmove(dest, src, move_size);

  vec->size--;

  return vec;
}

cstl_vector *cstl_vec_insert_range(cstl_vector *vec, const void *range,
                                   size_t range_size, size_t pos) {

  if (vec == NULL || range == NULL || pos > vec->size) {
    return NULL;
  }

  if (range_size == 0) {
    return vec;
  }

  size_t new_size = vec->size + range_size;

  if (new_size > vec->capacity) {

    size_t new_capacity = vec->capacity;

    while (new_capacity < new_size) {
      new_capacity =
          (new_capacity > 0) ? new_capacity * 2 : CSTL_VEC_START_CAPACITY;
    }

    if (cstl_vec_resize(vec, new_capacity) == NULL) {
      return NULL;
    }
  }

  void *dest = (char *)vec->data + (pos + range_size) * vec->type->size;
  void *src = (char *)vec->data + pos * vec->type->size;
  size_t move_size = (vec->size - pos) * vec->type->size;
  memmove(dest, src, move_size);

  if (vec->type->ctor) {

    size_t j = 0;
    for (size_t i = pos; i < pos + range_size; ++i) {
      void *current = (char *)vec->data + i * vec->type->size;
      const void *range_data = (char *)range + (j++) * vec->type->size;
      vec->type->ctor(current, range_data);
    }

  } else {
    void *dest_r = (char *)vec->data + pos * vec->type->size;
    memcpy(dest_r, range, range_size * vec->type->size);
  }

  vec->size = new_size;

  return vec;
}

cstl_vector *cstl_vec_erase_range(cstl_vector *vec, size_t pos, size_t len) {

  if (vec == NULL || pos > vec->size || pos + len > vec->size) {
    return NULL;
  }

  if (len == 0) {
    return vec;
  }

  if (vec->type->dtor) {
    for (size_t i = pos; i < (pos + len); ++i) {
      void *current = (char *)vec->data + i * vec->type->size;
      vec->type->dtor(current);
    }
  }

  void *dest = (char *)vec->data + pos * vec->type->size;
  void *src = (char *)vec->data + (pos + len) * vec->type->size;
  size_t move_size = (pos + len == vec->size) ? 0 : len;
  memmove(dest, src, move_size);

  vec->size -= len;

  return vec;
}

void _cstl_vec_free_data(cstl_vector *vec) {

  if (vec == NULL) {
    return;
  }

  if (vec->type->dtor) {

    for (size_t i = 0; i < vec->size; ++i) {
      void *current = (char *)vec->data + i * vec->type->size;
      vec->type->dtor(current);
    }
  }

  free(vec->data);
}

void cstl_vec_free(cstl_vector *vec) {

  if (vec == NULL) {
    return;
  }

  _cstl_vec_free_data(vec);

  free(vec);
}
