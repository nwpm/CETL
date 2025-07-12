#ifndef CETL_VECTOR_INTERNAL_H
#define CETL_VECTOR_INTERNAL_H

#include "../../prefix/cetl_prefix.h"
#include "../type/cetl_type.h"
#include <stddef.h>

#define CETL_VEC_START_CAPACITY ((cetl_size_t)8)
#define CETL_VEC_GROW_RATE 2

struct cetl_vector {
  cetl_size_t size;
  cetl_size_t capacity;
  cetl_byte_t* data;
  const cetl_type *type;
};

#endif // CETL_VECTOR_INTERNAL_H
