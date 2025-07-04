#ifndef CSTL_VECTOR_INTERNAL_H
#define CSTL_VECTOR_INTERNAL_H

#include "../type/type.h"
#include <stddef.h>

#define CSTL_VEC_START_CAPACITY 8

typedef struct cstl_vector {
  size_t size;
  size_t capacity;
  void *data;
  cstl_type *type;
} cstl_vector;

#endif // CSTL_VECTOR_INTERNAL_H
