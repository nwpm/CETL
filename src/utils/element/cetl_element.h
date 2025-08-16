#ifndef CETL_ELEMENT_H
#define CETL_ELEMENT_H

#include "../types/cetl_types.h"

typedef struct cetl_element {
  cetl_size_t size;
  cetl_ptr_t (*ctor)(cetl_ptr_t dest, cetl_cptr_t data);
  cetl_void_t (*dtor)(cetl_ptr_t data);
  cetl_result_t (*cmp)(cetl_cptr_t a, cetl_cptr_t b);
} cetl_element;

#endif
