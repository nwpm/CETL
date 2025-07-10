#ifndef CSTL_LLIST_INTERNAL_H
#define CSTL_LLIST_INTERNAL_H

#include <stddef.h>
#include "../type/type.h"

typedef struct _cstl_node {

  void *data;
  struct _cstl_node *next;

} _cstl_node;

struct cstl_llist {

  size_t size;
  _cstl_node *head;
  _cstl_node *tail;
  const cstl_type* type;

};

#endif // CSTL_LLIST_INTERNAL_H

