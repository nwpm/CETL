#ifndef CETL_ELEMENT_H
#define CETL_ELEMENT_H

#include <stddef.h>

struct cetl_type{

  size_t size;
  void *(*ctor)(void* dest, const void* data);
  void (*dtor)(void *data);
  int (*cmp)(const void* a, const void* b);

};

#endif
