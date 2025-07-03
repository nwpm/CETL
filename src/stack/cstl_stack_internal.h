#ifndef CSTL_STACK_INTERNAL_H
#define CSTL_STACK_INTERNAL_H

#include <stddef.h>

#include "../llist/cstl_llist_internal.h"

struct cstl_stack {

  size_t size;
  struct cstl_llist *data;

};

#endif // CSTL_STACK_INTERNAL_H

