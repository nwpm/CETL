#ifndef CETL_CDLIST_H
#define CETL_CDLIST_H

#include <stddef.h>
#include <stdbool.h>

#include "cetl_dlist.h"

typedef struct cetl_cdlist{
  
  size_t size;
  cetl_dlist *data;

} cetl_cdlist;

cetl_cdlist* cetl_cdlist_create_empty();
cetl_cdlist* cetl_cdlist_create(void *data, size_t elem_size);
cetl_cdlist* cetl_cdlist_create_copy(cetl_cdlist *cdl);

cetl_cdlist* cetl_cdlist_push_back(cetl_cdlist* cdl, void *data, size_t elem_size);
cetl_cdlist* cetl_cdlist_pop_back(cetl_cdlist *cdl);
cetl_cdlist* cetl_cdlist_push_front(cetl_cdlist *cdl, void *data, size_t elem_size);
cetl_cdlist* cetl_cdlist_pop_front(cetl_cdlist *cdl);
cetl_cdlist* cetl_cdlist_insert(cetl_cdlist *cdl, void *data, size_t elem_size, size_t pos);
cetl_cdlist* cetl_cdlist_erase(cetl_cdlist *cdl, size_t pos);
cetl_cdlist* cetl_cdlist_merge_two(cetl_cdlist *cdl1, cetl_cdlist *cdl2);
cetl_cdlist* cetl_cdlist_clear(cetl_cdlist *cdl);
cetl_cdlist* cetl_cdlist_set(cetl_cdlist *cdl);

size_t cetl_cdlist_size(cetl_cdlist *cdl);

bool cetl_cdlist_is_empty(cetl_cdlist *cdl);

void *cetl_cdlist_get(cetl_cdlist *cdl, size_t pos);
void cetl_cdlist_free(cetl_cdlist *cdl);
void cetl_cdlist_free_nodes(cetl_cdlist* cdl);

#endif // CETL_CDLIST_H
