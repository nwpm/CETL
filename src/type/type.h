#ifndef CSTL_ELEMENT_H
#define CSTL_ELEMENT_H

#include <stddef.h>

typedef struct cstl_type{

  size_t size;
  void *(*ctor)(void* dest, const void* data);
  void (*dtor)(void *data);
  int (*cmp)(const void* a, const void* b);

} cstl_type;

#endif
