#ifndef CETL_COMMON_ITERATOR_H
#define CETL_COMMON_ITERATOR_H

#include "../../prefix/cetl_types.h"

typedef struct cetl_common_iter {

  cetl_ptr_t data;
  cetl_size_t size;
  cetl_size_t index;
  cetl_size_t elem_size;

} cetl_common_iter;

#endif
