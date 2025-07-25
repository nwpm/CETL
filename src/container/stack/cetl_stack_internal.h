#ifndef CETL_STACK_INTERNAL_H
#define CETL_STACK_INTERNAL_H

#include "../../prefix/cetl_ptypes.h"
#include "../type/cetl_type.h"
#include <stddef.h>

struct cetl_llist;

struct cetl_stack {

  cetl_size_t size;
  struct cetl_llist *data;
  const struct cetl_type *type;
};

#endif // CETL_STACK_INTERNAL_H
