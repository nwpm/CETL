#ifndef CETL_VECTOR_INTERNAL_H
#define CETL_VECTOR_INTERNAL_H

#include "../type/type.h"
#include <stddef.h>

#define cetl_VEC_START_CAPACITY ((size_t)8)
#define cetl_VEC_GROW_RATE 2

struct cetl_vector {
  size_t size;
  size_t capacity;
  void *data;
  const cetl_type *type;
};

#endif // CETL_VECTOR_INTERNAL_H
