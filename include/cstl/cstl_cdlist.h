#ifndef CSTL_CDLIST_H
#define CSTL_CDLIST_H

#include <stddef.h>
#include <stdbool.h>

#include "cstl_dlist.h"

typedef struct cstl_cdlist{
  
  size_t size;
  cstl_dlist *data;

} cstl_cdlist;

cstl_cdlist* cstl_cdlist_create_empty();
cstl_cdlist* cstl_cdlist_create(void *data, size_t elem_size);
cstl_cdlist* cstl_cdlist_create_copy(cstl_cdlist *cdl);

cstl_cdlist* cstl_cdlist_push_back(cstl_cdlist* cdl, void *data, size_t elem_size);
cstl_cdlist* cstl_cdlist_pop_back(cstl_cdlist *cdl);
cstl_cdlist* cstl_cdlist_push_front(cstl_cdlist *cdl, void *data, size_t elem_size);
cstl_cdlist* cstl_cdlist_pop_front(cstl_cdlist *cdl);
cstl_cdlist* cstl_cdlist_insert(cstl_cdlist *cdl, void *data, size_t elem_size, size_t pos);
cstl_cdlist* cstl_cdlist_erase(cstl_cdlist *cdl, size_t pos);
cstl_cdlist* cstl_cdlist_merge_two(cstl_cdlist *cdl1, cstl_cdlist *cdl2);
cstl_cdlist* cstl_cdlist_clear(cstl_cdlist *cdl);
cstl_cdlist* cstl_cdlist_set(cstl_cdlist *cdl);

size_t cstl_cdlist_size(cstl_cdlist *cdl);

bool cstl_cdlist_is_empty(cstl_cdlist *cdl);

void *cstl_cdlist_get(cstl_cdlist *cdl, size_t pos);
void cstl_cdlist_free(cstl_cdlist *cdl);
void cstl_cdlist_free_nodes(cstl_cdlist* cdl);

#endif // CSTL_CDLIST_H
