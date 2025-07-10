#ifndef CSTL_DLIST_INTERNAL_H
#define CSTL_DLIST_INTERNAL_H

#include <stddef.h>
#include "../type/type.h"

typedef struct _cstl_dnode {

  void *data;
  struct _cstl_dnode *next;
  struct _cstl_dnode *prev;

} _cstl_dnode;

struct cstl_dlist {

  size_t size;
  _cstl_dnode *head;
  _cstl_dnode *tail;
  const cstl_type *type;

};

#endif // CSTL_DLIST_INTERNAL_H

