#ifndef CETL_CONST_ITERATOR_H
#define CETL_CONST_ITERATOR_H

#include "../types/cetl_types.h"
#include "../types/cetl_state.h"

struct cetl_const_iterator {

  _cetl_iter_category category; 
  cetl_ptr_t state;

  cetl_cptr_t (*get)(const struct cetl_const_iterator *);
  cetl_void_t (*next)(const struct cetl_const_iterator *);
  cetl_bool_t (*equal)(const struct cetl_const_iterator *,
                       const struct cetl_const_iterator *);

};

#endif
