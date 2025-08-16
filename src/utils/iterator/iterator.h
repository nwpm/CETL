#ifndef CETL_COMMON_ITERATOR_H
#define CETL_COMMON_ITERATOR_H

#include "../types/cetl_types.h"

typedef enum _cetl_iter_category{
  CETL_FORWARD_ITERATOR,
  CETL_REVERSE_ITERATOR,
  CETL_MUTABLE_ITERATOR,
} _cetl_iter_category;

typedef struct cetl_iterator {

  _cetl_iter_category category;
  const cetl_element *elem_type;
  cetl_ptr_t data;
  cetl_size_t size;
  cetl_size_t index;

} cetl_iterator;

#endif
