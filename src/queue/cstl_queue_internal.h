#ifndef CSTL_QUEUE_INTERNAL_H
#define CSTL_QUEUE_INTERNAL_H

#include "../type/type.h"
#include <stddef.h>

typedef struct cstl_llist cstl_llist;

struct cstl_queue {

  size_t size;
  struct cstl_llist *data;
  cstl_type *type;
};

#endif // CSTL_QUEUE_INTERNAL_H
