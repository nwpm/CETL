#ifndef CSTL_QUEUE_INTERNAL_H
#define CSTL_QUEUE_INTERNAL_H

#include "../llist/cstl_llist_internal.h"
#include <stddef.h>

struct cstl_queue {

  size_t size;
  struct cstl_llist *data;

};

#endif // CSTL_QUEUE_INTERNAL_H
