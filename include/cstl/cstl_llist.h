#ifndef CSTL_LLIST_H
#define CSTL_LLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cstl_type cstl_type;
typedef struct cstl_llist cstl_llist;

cstl_llist *cstl_llist_create_empty(const cstl_type *type);
cstl_llist *cstl_llist_create_copy(const cstl_llist *src_llist);

cstl_llist *cstl_llist_push_back(cstl_llist *llist, const void *data);
cstl_llist *cstl_llist_pop_back(cstl_llist *llist);
cstl_llist *cstl_llist_push_front(cstl_llist *llist, const void *data);
cstl_llist *cstl_llist_pop_front(cstl_llist *llist);
cstl_llist *cstl_llist_insert(cstl_llist *llist, const void *data, size_t pos);
cstl_llist *cstl_llist_erase(cstl_llist *llist, size_t pos);
cstl_llist *cstl_llist_merge_two(const cstl_llist *llist1, const cstl_llist *llist2);
cstl_llist *cstl_llist_clear(cstl_llist *llist);
cstl_llist *cstl_llist_set(cstl_llist *llist, size_t pos, const void *data);

size_t cstl_llist_size(const cstl_llist *llist);

bool cstl_llist_is_empty(const cstl_llist *llist);

void *cstl_llist_get(const cstl_llist *llist, size_t pos);

void cstl_llist_free(cstl_llist *llist);
void cstl_llist_free_nodes(cstl_llist *llist);

#endif // CSTL_LLIST_H
