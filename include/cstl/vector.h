#ifndef DYNAMIC_vec_H
#define DYNAMIC_vec_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct vector {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void *data;
} vector;

vector *vec_create(size_t vec_capacity, size_t elem_size);
vector *vec_create_copy(vector *v);
vector *vec_create_filled(size_t vec_capacity, size_t elem_size,
                          void *init_val);

void *vec_front(vector *v);
void *vec_back(vector *v);
void *vec_get(vector *v, size_t index);
void *vec_data(vector *v);

int vec_set(vector *v, size_t index, void *elem_val);

size_t vec_size(vector *v);
size_t vec_capacity(vector *v);

vector *vec_resize(vector *v, size_t new_capacity);
vector *vec_push_back(vector *v, void *elem_val);
vector *vec_pop_back(vector *v);
vector *vec_shrink_to_fit(vector *v);
vector *vec_clear(vector *v);
vector *vec_insert(vector *v, size_t index, void *elem_val);
vector *vec_erase(vector *v, size_t pos);
vector *vec_insert_range(vector *v, void *range, size_t range_size,
                         size_t index);
vector *vec_erase_range(vector *v, size_t index, size_t len);

bool vec_is_empty(vector *v);

void vec_free(vector *v);

#endif // DYNAMIC_vec_H
