#ifndef CETL_STACK_INTERNAL_H
#define CETL_STACK_INTERNAL_H

#include "../type/type.h"
#include <stddef.h>

typedef struct cetl_llist cetl_llist;

struct cetl_stack {

  size_t size;
  struct cetl_llist *data;
  cetl_type *type;
};

#endif // CETL_STACK_INTERNAL_H
