#ifndef CETL_ITERATOR_H
#define CETL_ITERATOR_H

#include "../../prefix/cetl_prefix.h"

typedef enum _cetl_iterator_type{

  CETL_ITER_MODE_FORWARD,
  CETL_ITER_MODE_BIDIRECTIONAL,
  CETL_ITER_MODE_REVERSE,
  CSTL_ITER_MODE_MUTABLE,
  CETL_ITER_MODE_CONST,

} _cetl_iterator_type;

struct _cetl_iterator_t{

  cetl_ptr_t (*next)(struct _cetl_iterator *itor);
  cetl_result_t (*has_next)(struct _cetl_iterator *itor);
  cetl_result_t (*destroy)(struct _cetl_iterator *itor);

  cetl_ptr_t (*get)(struct _cetl_iterator *itor);
  cetl_result_t (*set)(struct _cetl_iterator *itor);

  cetl_ptr_t (*item)(struct _cetl_iterator *itor);
  _cetl_iterator_type type;
};

#endif // CETL_ITERATOR_H
