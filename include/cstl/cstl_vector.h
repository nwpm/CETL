#ifndef CSTL_VECTOR_H
#define CSTL_VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct cstl_vector {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void *data;
} cstl_vector;

cstl_vector *cstl_vec_create(size_t cstl_vec_capacity, size_t elem_size);
cstl_vector *cstl_vec_create_copy(cstl_vector *v);
cstl_vector *cstl_vec_create_filled(size_t cstl_vec_capacity, size_t elem_size,
                          void *init_val);
//TODO: delete? because it useless
cstl_vector *cstl_vec_create_empty();

void *cstl_vec_front(cstl_vector *v);
void *cstl_vec_back(cstl_vector *v);
void *cstl_vec_get(cstl_vector *v, size_t index);
void *cstl_vec_data(cstl_vector *v);

int cstl_vec_set(cstl_vector *v, size_t index, void *elem_val);

size_t cstl_vec_size(cstl_vector *v);
size_t cstl_vec_capacity(cstl_vector *v);

cstl_vector *cstl_vec_resize(cstl_vector *v, size_t new_capacity);
cstl_vector *cstl_vec_push_back(cstl_vector *v, void *elem_val);
cstl_vector *cstl_vec_pop_back(cstl_vector *v);
cstl_vector *cstl_vec_shrink_to_fit(cstl_vector *v);
cstl_vector *cstl_vec_clear(cstl_vector *v);
cstl_vector *cstl_vec_insert(cstl_vector *v, size_t index, void *elem_val);
cstl_vector *cstl_vec_erase(cstl_vector *v, size_t pos);
cstl_vector *cstl_vec_insert_range(cstl_vector *v, void *range, size_t range_size,
                         size_t index);
cstl_vector *cstl_vec_erase_range(cstl_vector *v, size_t index, size_t len);

bool cstl_vec_is_empty(cstl_vector *v);

void cstl_vec_free(cstl_vector *v);

#endif // CSTL_VECTOR_H
