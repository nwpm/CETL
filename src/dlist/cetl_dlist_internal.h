#ifndef CETL_DLIST_INTERNAL_H
#define CETL_DLIST_INTERNAL_H

#include <stddef.h>
#include "../type/type.h"

typedef struct _cetl_dnode {

  void *data;
  struct _cetl_dnode *next;
  struct _cetl_dnode *prev;

} _cetl_dnode;

struct cetl_dlist {

  size_t size;
  _cetl_dnode *head;
  _cetl_dnode *tail;
  const cetl_type *type;

};

#endif // CETL_DLIST_INTERNAL_H

