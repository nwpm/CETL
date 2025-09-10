#ifndef CETL_VECTOR_H
#define CETL_VECTOR_H

#include "../../src/utils/types/cetl_types.h"

#define CETL_ITER_GET_AS(it, type) ((type*)((it)->get(it)))

// Create
cetl_vector *cetl_vec_create_empty(const struct cetl_element *type);
cetl_vector *cetl_vec_create_copy(const cetl_vector *src_vec);

// Operations
cetl_vector *cetl_vec_resize(cetl_vector *vec, cetl_size_t new_capacity);
cetl_vector *cetl_vec_push_back(cetl_vector *vec, cetl_cptr_t new_val);
cetl_vector *cetl_vec_pop_back(cetl_vector *vec);
cetl_vector *cetl_vec_shrink_to_fit(cetl_vector *vec);
cetl_vector *cetl_vec_clear(cetl_vector *vec);
cetl_vector *cetl_vec_insert(cetl_vector *vec, cetl_size_t pos,
                             cetl_cptr_t new_val);
cetl_vector *cetl_vec_erase(cetl_vector *vec, cetl_size_t pos);
cetl_vector *cetl_vec_insert_range(cetl_vector *vec, cetl_cptr_t range,
                                   cetl_size_t range_size, cetl_size_t pos);
cetl_vector *cetl_vec_erase_range(cetl_vector *vec, cetl_size_t pos,
                                  cetl_size_t len);

const struct cetl_element* cetl_vec_get_type(const cetl_vector* vec);

// Element access
cetl_ptr_t cetl_vec_front(const cetl_vector *vec);
cetl_ptr_t cetl_vec_back(const cetl_vector *vec);
cetl_ptr_t cetl_vec_get(const cetl_vector *vec, cetl_size_t pos);
cetl_ptr_t cetl_vec_data(const cetl_vector *vec);

cetl_result_t cetl_vec_set(cetl_vector *vec, cetl_size_t pos,
                           cetl_cptr_t new_val);

// Size
cetl_size_t cetl_vec_size(const cetl_vector *vec);
cetl_size_t cetl_vec_capacity(const cetl_vector *vec);
cetl_bool_t cetl_vec_is_empty(const cetl_vector *vec);

// Iterators
cetl_iterator *cetl_vec_iter_begin(const cetl_vector* vec);
cetl_iterator *cetl_vec_iter_end(const cetl_vector* vec);

cetl_const_iterator *cetl_vec_iter_cbegin(const cetl_vector* vec);
cetl_const_iterator *cetl_vec_iter_cend(const cetl_vector* vec);

// Free
cetl_void_t cetl_vec_iter_free(cetl_iterator *it);
cetl_void_t cetl_vec_iter_cfree(cetl_const_iterator *it);
cetl_void_t cetl_vec_free(cetl_vector *vec);

#endif // CETL_VECTOR_H
