#ifndef CETL_QUEUE_INTERNAL_H
#define CETL_QUEUE_INTERNAL_H

#include "../../prefix/cetl_ptypes.h"
#include "../type/cetl_type.h"
#include <stddef.h>

struct cetl_llist;

struct cetl_queue {

  cetl_size_t size;
  struct cetl_llist *data;
  const struct cetl_type *type;
};

#endif // CETL_QUEUE_INTERNAL_H
