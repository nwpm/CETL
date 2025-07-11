#ifndef CETL_LLIST_H
#define CETL_LLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cetl_type cetl_type;
typedef struct cetl_llist cetl_llist;

cetl_llist *cetl_llist_create_empty(const cetl_type *type);
cetl_llist *cetl_llist_create_copy(const cetl_llist *src_llist);

cetl_llist *cetl_llist_push_back(cetl_llist *llist, const void *data);
cetl_llist *cetl_llist_pop_back(cetl_llist *llist);
cetl_llist *cetl_llist_push_front(cetl_llist *llist, const void *data);
cetl_llist *cetl_llist_pop_front(cetl_llist *llist);
cetl_llist *cetl_llist_insert(cetl_llist *llist, const void *data, size_t pos);
cetl_llist *cetl_llist_erase(cetl_llist *llist, size_t pos);
cetl_llist *cetl_llist_merge_two(const cetl_llist *llist1, const cetl_llist *llist2);
cetl_llist *cetl_llist_clear(cetl_llist *llist);
cetl_llist *cetl_llist_set(cetl_llist *llist, size_t pos, const void *data);

size_t cetl_llist_size(const cetl_llist *llist);

bool cetl_llist_is_empty(const cetl_llist *llist);

void *cetl_llist_get(const cetl_llist *llist, size_t pos);

void cetl_llist_free(cetl_llist *llist);
void cetl_llist_free_nodes(cetl_llist *llist);

#endif // CETL_LLIST_H
