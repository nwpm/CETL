#ifndef DYNAMIC_vec_H
#define DYNAMIC_vec_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct cstl_vector {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void *data;
} cstl_vector;

cstl_vector *vec_create(size_t vec_capacity, size_t elem_size);
cstl_vector *vec_create_copy(cstl_vector *v);
cstl_vector *vec_create_filled(size_t vec_capacity, size_t elem_size,
                          void *init_val);

void *vec_front(cstl_vector *v);
void *vec_back(cstl_vector *v);
void *vec_get(cstl_vector *v, size_t index);
void *vec_data(cstl_vector *v);

int vec_set(cstl_vector *v, size_t index, void *elem_val);

size_t vec_size(cstl_vector *v);
size_t vec_capacity(cstl_vector *v);

cstl_vector *vec_resize(cstl_vector *v, size_t new_capacity);
cstl_vector *vec_push_back(cstl_vector *v, void *elem_val);
cstl_vector *vec_pop_back(cstl_vector *v);
cstl_vector *vec_shrink_to_fit(cstl_vector *v);
cstl_vector *vec_clear(cstl_vector *v);
cstl_vector *vec_insert(cstl_vector *v, size_t index, void *elem_val);
cstl_vector *vec_erase(cstl_vector *v, size_t pos);
cstl_vector *vec_insert_range(cstl_vector *v, void *range, size_t range_size,
                         size_t index);
cstl_vector *vec_erase_range(cstl_vector *v, size_t index, size_t len);

bool vec_is_empty(cstl_vector *v);

void vec_free(cstl_vector *v);

#endif // DYNAMIC_vec_H
