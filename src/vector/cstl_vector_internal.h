#ifndef CSTL_VECTOR_INTERNAL_H
#define CSTL_VECTOR_INTERNAL_H

#include "../type/type.h"
#include <stddef.h>

#define CSTL_VEC_START_CAPACITY ((size_t)8)
#define CSTL_VEC_GROW_RATE 2

struct cstl_vector {
  size_t size;
  size_t capacity;
  void *data;
  cstl_type *type;
};

#endif // CSTL_VECTOR_INTERNAL_H
