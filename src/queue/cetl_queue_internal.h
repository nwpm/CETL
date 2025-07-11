#ifndef CETL_QUEUE_INTERNAL_H
#define CETL_QUEUE_INTERNAL_H

#include "../type/type.h"
#include <stddef.h>

typedef struct cetl_llist cetl_llist;

struct cetl_queue {

  size_t size;
  struct cetl_llist *data;
  cetl_type *type;
};

#endif // CETL_QUEUE_INTERNAL_H
