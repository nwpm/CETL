#ifndef CETL_DLIST_H
#define CETL_DLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cetl_type cetl_type;
typedef struct cetl_dlist cetl_dlist;

cetl_dlist *cetl_dlist_create_empty(const cetl_type *type);
cetl_dlist *cetl_dlist_create_copy(const cetl_dlist *src_dlist);

cetl_dlist *cetl_dlist_push_back(cetl_dlist *dlist, const void *data);
cetl_dlist *cetl_dlist_pop_back(cetl_dlist *dlist);
cetl_dlist *cetl_dlist_push_front(cetl_dlist *dlist, const void *data);
cetl_dlist *cetl_dlist_pop_front(cetl_dlist *dlist);
cetl_dlist *cetl_dlist_insert(cetl_dlist *dlist, const void *data, size_t pos);
cetl_dlist *cetl_dlist_erase(cetl_dlist *dlist, size_t pos);
cetl_dlist *cetl_dlist_merge_two(const cetl_dlist *dlist1, const cetl_dlist *dlist2);
cetl_dlist *cetl_dlist_clear(cetl_dlist *dlist);
cetl_dlist *cetl_dlist_set(cetl_dlist *dlist, size_t pos, const void *data);

size_t cetl_dlist_size(const cetl_dlist *dlist);

bool cetl_dlist_is_empty(const cetl_dlist *dlist);

void *cetl_dlist_get(const cetl_dlist *dlist, size_t pos);
void cetl_dlist_free(cetl_dlist *dlist);
void cetl_dlist_free_nodes(cetl_dlist *dlist);

#endif // CETL_DLIST_H
