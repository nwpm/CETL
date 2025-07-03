#ifndef CSTL_LLIST_H
#define CSTL_LLIST_H

#include <stdbool.h>
#include <stddef.h>

struct cstl_llist;
typedef struct cstl_llist cstl_llist;

cstl_llist *cstl_llist_create_empty();
cstl_llist *cstl_llist_create(void *data, size_t elem_size);
cstl_llist *cstl_llist_create_copy(cstl_llist *l);

cstl_llist *cstl_llist_push_back(cstl_llist *l, void *data, size_t elem_size);
cstl_llist *cstl_llist_pop_back(cstl_llist *l);
cstl_llist *cstl_llist_push_front(cstl_llist *l, void *data, size_t elem_size);
cstl_llist *cstl_llist_pop_front(cstl_llist *l);
cstl_llist *cstl_llist_insert(cstl_llist *l, void *data, size_t pos, size_t elem_size);
cstl_llist *cstl_llist_erase(cstl_llist *l, size_t pos);
cstl_llist *cstl_llist_merge_two(cstl_llist *l1, cstl_llist *l2);
cstl_llist *cstl_llist_clear(cstl_llist *l);
cstl_llist *cstl_llist_set(cstl_llist *l, size_t pos, void *data);

size_t cstl_llist_size(cstl_llist *l);

bool cstl_llist_is_empty(cstl_llist *l);

void *cstl_llist_get(cstl_llist *l, size_t pos);

void cstl_llist_free(cstl_llist *l);
void cstl_llist_free_nodes(cstl_llist *l);

#endif // CSTL_LLIST_H
