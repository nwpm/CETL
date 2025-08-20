#ifndef CETL_STATE_H
#define CETL_STATE_H

typedef enum _cetl_state_e {

  CETL_STATE_OK             = 1,

  CETL_STATE_ERROR          = 2,
  CETL_STATE_NULL_PARAMETER = 3,

} _cetl_state_e;

typedef enum _cetl_iter_category {
  CETL_FORWARD_ITERATOR,
  CETL_REVERSE_ITERATOR,
  CETL_CONST_ITERATOR,
} _cetl_iter_category;

#endif // CETL_STATE_H
