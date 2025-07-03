#ifndef CSTL_LLIST_INTERNAL_H
#define CSTL_LLIST_INTERNAL_H

#include <stddef.h>

typedef struct _cstl_node {

  void *data;
  struct _cstl_node *next;

} _cstl_node;

struct cstl_llist {

  size_t elem_size;
  size_t size;
  _cstl_node *head;
  _cstl_node *tail;

};

_cstl_node *_cstl_llist_create_node(void *data, size_t elem_size);

#endif // CSTL_LLIST_INTERNAL_H

