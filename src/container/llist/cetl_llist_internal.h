#ifndef CETL_LLIST_INTERNAL_H
#define CETL_LLIST_INTERNAL_H

#include <stddef.h>
#include "../type/cetl_type.h"

typedef struct _cetl_node {

  void *data;
  struct _cetl_node *next;

} _cetl_node;

struct cetl_llist {

  size_t size;
  _cetl_node *head;
  _cetl_node *tail;
  const cetl_type* type;

};

#endif // CETL_LLIST_INTERNAL_H

