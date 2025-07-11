#ifndef CETL_ITERATOR_H
#define CETL_ITERATOR_H

typedef enum _cetl_iterator_type{

  CETL_ITER_MODE_FORWARD,
  CETL_ITER_MODE_BIDIRECTIONAL,
  CETL_ITER_MODE_REVERSE,
  CSTL_ITER_MODE_MUTABLE,
  CETL_ITER_MODE_CONST,

} _cetl_iterator_type;

struct _cetl_iterator{

  void *(*next)(struct _cetl_iterator *itor);
  void (*has_next)(struct _cetl_iterator *itor);
  void (*destroy)(struct _cetl_iterator *itor);

  void *(*get)(struct _cetl_iterator *itor);
  void (*set)(struct _cetl_iterator *itor);

  void *(*item)(struct _cetl_iterator *itor);
  _cetl_iterator_type type;
};

#endif // CETL_ITERATOR_H
