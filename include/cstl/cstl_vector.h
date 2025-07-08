#ifndef CSTL_VECTOR_H
#define CSTL_VECTOR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cstl_type cstl_type;
typedef struct cstl_vector cstl_vector;

cstl_vector *cstl_vec_create_empty(cstl_type *type);
cstl_vector *cstl_vec_create_copy(cstl_vector *vec);

cstl_vector *cstl_vec_resize(cstl_vector *vec, size_t new_capacity);
cstl_vector *cstl_vec_push_back(cstl_vector *vec, const void *data);
cstl_vector *cstl_vec_pop_back(cstl_vector *vec);
cstl_vector *cstl_vec_shrink_to_fit(cstl_vector *vec);
cstl_vector *cstl_vec_clear(cstl_vector *vec);
cstl_vector *cstl_vec_insert(cstl_vector *vec, size_t pos, const void *data);
cstl_vector *cstl_vec_erase(cstl_vector *vec, size_t pos);
cstl_vector *cstl_vec_insert_range(cstl_vector *vec, const void *range,
                                   size_t range_size, size_t pos);
cstl_vector *cstl_vec_erase_range(cstl_vector *vec, size_t pos, size_t len);

void *cstl_vec_front(cstl_vector *vec);
void *cstl_vec_back(cstl_vector *vec);
void *cstl_vec_get(cstl_vector *vec, size_t pos);
void *cstl_vec_data(cstl_vector *vec);

int cstl_vec_set(cstl_vector *vec, size_t pos, const void *set_data);

size_t cstl_vec_size(cstl_vector *vec);
size_t cstl_vec_capacity(cstl_vector *vec);

bool cstl_vec_is_empty(cstl_vector *vec);

void _cstl_vec_free_data(cstl_vector *vec);
void cstl_vec_free(cstl_vector *vec);

#endif // CSTL_VECTOR_H
