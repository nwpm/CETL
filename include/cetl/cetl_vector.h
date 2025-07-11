#ifndef CETL_VECTOR_H
#define CETL_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cetl_type cetl_type;
typedef struct cetl_vector cetl_vector;

cetl_vector *cetl_vec_create_empty(const cetl_type *type);
cetl_vector *cetl_vec_create_copy(const cetl_vector *src_vec);

cetl_vector *cetl_vec_resize(cetl_vector *vec, size_t new_capacity);
cetl_vector *cetl_vec_push_back(cetl_vector *vec, const void *new_val);
cetl_vector *cetl_vec_pop_back(cetl_vector *vec);
cetl_vector *cetl_vec_shrink_to_fit(cetl_vector *vec);
cetl_vector *cetl_vec_clear(cetl_vector *vec);
cetl_vector *cetl_vec_insert(cetl_vector *vec, size_t pos, const void *new_val);
cetl_vector *cetl_vec_erase(cetl_vector *vec, size_t pos);
cetl_vector *cetl_vec_insert_range(cetl_vector *vec, const void *range,
                                   size_t range_size, size_t pos);
cetl_vector *cetl_vec_erase_range(cetl_vector *vec, size_t pos, size_t len);

void *cetl_vec_front(const cetl_vector *vec);
void *cetl_vec_back(const cetl_vector *vec);
void *cetl_vec_get(const cetl_vector *vec, size_t pos);
void *cetl_vec_data(const cetl_vector *vec);

int cetl_vec_set(cetl_vector *vec, size_t pos, const void *new_val);

size_t cetl_vec_size(const cetl_vector *vec);
size_t cetl_vec_capacity(const cetl_vector *vec);

bool cetl_vec_is_empty(const cetl_vector *vec);

void cetl_vec_free(cetl_vector *vec);

#endif // CETL_VECTOR_H
