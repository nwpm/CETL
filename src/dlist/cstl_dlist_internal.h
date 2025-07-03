#ifndef CSTL_DLIST_INTERNAL_H
#define CSTL_DLIST_INTERNAL_H

#include <stddef.h>

typedef struct _cstl_dnode {

  void *data;
  struct _cstl_dnode *next;
  struct _cstl_dnode *prev;

} _cstl_dnode;

struct cstl_dlist {

  size_t elem_size;
  size_t size;
  _cstl_dnode *head;
  _cstl_dnode *tail;

};

_cstl_dnode *_cstl_dlist_create_dnode(void *data, size_t elem_size);

#endif // CSTL_DLIST_INTERNAL_H

