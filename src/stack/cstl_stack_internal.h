#ifndef CSTL_STACK_INTERNAL_H
#define CSTL_STACK_INTERNAL_H

#include "../type/type.h"
#include <stddef.h>

typedef struct cstl_llist cstl_llist;

struct cstl_stack {

  size_t size;
  struct cstl_llist *data;
  cstl_type *type;
};

#endif // CSTL_STACK_INTERNAL_H
