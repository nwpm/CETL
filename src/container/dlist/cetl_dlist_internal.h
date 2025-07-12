#ifndef CETL_DLIST_INTERNAL_H
#define CETL_DLIST_INTERNAL_H

#include "../../prefix/cetl_ptypes.h"
#include "../type/cetl_type.h"
#include <stddef.h>

typedef struct _cetl_dnode {

  cetl_ptr_t data;
  struct _cetl_dnode *next;
  struct _cetl_dnode *prev;

} _cetl_dnode;

struct cetl_dlist {

  cetl_size_t size;
  _cetl_dnode *head;
  _cetl_dnode *tail;
  const cetl_type *type;
};

#endif // CETL_DLIST_INTERNAL_H
