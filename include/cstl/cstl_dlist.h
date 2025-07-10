#ifndef CSTL_DLIST_H
#define CSTL_DLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cstl_type cstl_type;
typedef struct cstl_dlist cstl_dlist;

cstl_dlist *cstl_dlist_create_empty(const cstl_type *type);
cstl_dlist *cstl_dlist_create_copy(const cstl_dlist *src_dlist);

cstl_dlist *cstl_dlist_push_back(cstl_dlist *dlist, const void *data);
cstl_dlist *cstl_dlist_pop_back(cstl_dlist *dlist);
cstl_dlist *cstl_dlist_push_front(cstl_dlist *dlist, const void *data);
cstl_dlist *cstl_dlist_pop_front(cstl_dlist *dlist);
cstl_dlist *cstl_dlist_insert(cstl_dlist *dlist, const void *data, size_t pos);
cstl_dlist *cstl_dlist_erase(cstl_dlist *dlist, size_t pos);
cstl_dlist *cstl_dlist_merge_two(const cstl_dlist *dlist1, const cstl_dlist *dlist2);
cstl_dlist *cstl_dlist_clear(cstl_dlist *dlist);
cstl_dlist *cstl_dlist_set(cstl_dlist *dlist, size_t pos, const void *data);

size_t cstl_dlist_size(const cstl_dlist *dlist);

bool cstl_dlist_is_empty(const cstl_dlist *dlist);

void *cstl_dlist_get(const cstl_dlist *dlist, size_t pos);
void cstl_dlist_free(cstl_dlist *dlist);
void cstl_dlist_free_nodes(cstl_dlist *dlist);

#endif // CSTL_DLIST_H
