#ifndef CETL_COMMON_ITERATOR_H
#define CETL_COMMON_ITERATOR_H

#include "../types/cetl_types.h"

typedef enum _cetl_iter_category {
  CETL_FORWARD_ITERATOR,
  CETL_REVERSE_ITERATOR,
  CETL_MUTABLE_ITERATOR,
} _cetl_iter_category;

typedef struct cetl_iterator {

  _cetl_iter_category category; 
  cetl_cptr_t state;

  cetl_ptr_t (*get)(const struct cetl_iterator *);
  cetl_void_t (*next)(const struct cetl_iterator *);
  cetl_bool_t (*equal)(const struct cetl_iterator *,
                       const struct cetl_iterator *);

} cetl_iterator;

#endif
