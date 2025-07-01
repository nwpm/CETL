#ifndef CSTL_DLIST_H
#define CSTL_DLIST_H

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _cstl_dnode{
  
  void *data;
  struct _cstl_dnode* next;
  struct _cstl_dnode* prev;

} _cstl_dnode;

typedef struct cstl_dlist{
  
  size_t elem_size;
  size_t size;
  _cstl_dnode* head;
  _cstl_dnode* tail;

} cstl_dlist;

_cstl_dnode* _cstl_dlist_create_dnode(void *data, size_t elem_size);

cstl_dlist* cstl_dlist_create_empty();
cstl_dlist* cstl_dlist_create(void *data, size_t elem_size);
cstl_dlist* cstl_dlist_create_copy(cstl_dlist *dl);

cstl_dlist* cstl_dlist_push_back(cstl_dlist* dl, void *data, size_t elem_size);
cstl_dlist* cstl_dlist_pop_back(cstl_dlist *dl);
cstl_dlist* cstl_dlist_push_front(cstl_dlist *dl, void *data, size_t elem_size);
cstl_dlist* cstl_dlist_pop_front(cstl_dlist *dl);
cstl_dlist* cstl_dlist_insert(cstl_dlist *dl, void* data, size_t pos, size_t elem_size);
cstl_dlist* cstl_dlist_erase(cstl_dlist *dl, size_t pos);
cstl_dlist* cstl_dlist_merge_two(cstl_dlist *dl1, cstl_dlist *dl2);

//TODO:
cstl_dlist* cstl_dlist_clear(cstl_dlist *dl);
cstl_dlist* cstl_dlist_set(cstl_dlist *dl, size_t pos, void *data);
size_t cstl_dlist_size(cstl_dlist *dl);
void *cstl_dlist_get(cstl_dlist *dl, size_t pos);
void cstl_dlist_free(cstl_dlist *dl);
//

bool cstl_dlist_is_empty(cstl_dlist *dl);

void cstl_dlist_free_nodes(cstl_dlist* dl);

#endif //CSTL_DLIST_H
