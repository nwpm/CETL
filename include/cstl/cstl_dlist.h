#ifndef CSTL_DLIST_H
#define CSTL_DLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cstl_type cstl_type;
typedef struct cstl_dlist cstl_dlist;

cstl_dlist *cstl_dlist_create_empty(cstl_type *t);
cstl_dlist *cstl_dlist_create_copy(cstl_dlist *dl);

cstl_dlist *cstl_dlist_push_back(cstl_dlist *dl, void *data);
cstl_dlist *cstl_dlist_pop_back(cstl_dlist *dl);
cstl_dlist *cstl_dlist_push_front(cstl_dlist *dl, void *data);
cstl_dlist *cstl_dlist_pop_front(cstl_dlist *dl);
cstl_dlist *cstl_dlist_insert(cstl_dlist *dl, void *data, size_t pos);
cstl_dlist *cstl_dlist_erase(cstl_dlist *dl, size_t pos);
cstl_dlist *cstl_dlist_merge_two(cstl_dlist *dl1, cstl_dlist *dl2);
cstl_dlist *cstl_dlist_clear(cstl_dlist *dl);
cstl_dlist *cstl_dlist_set(cstl_dlist *dl, size_t pos, void *data);

size_t cstl_dlist_size(cstl_dlist *dl);

bool cstl_dlist_is_empty(cstl_dlist *dl);

void *cstl_dlist_get(cstl_dlist *dl, size_t pos);
void cstl_dlist_free(cstl_dlist *dl);
void cstl_dlist_free_nodes(cstl_dlist *dl);

#endif // CSTL_DLIST_H
