#ifndef CSTL_VECTOR_INTERNAL_H
#define CSTL_VECTOR_INTERNAL_H

#include <stddef.h>

typedef void* (*cstl_copy_func)(const void *data);
typedef void (*cstl_free_func)(void *data);

struct cstl_vector {
  size_t size;
  size_t capacity;
  size_t elem_size;
  void *data;
};

#endif // CSTL_VECTOR_INTERNAL_H

