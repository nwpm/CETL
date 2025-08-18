#ifndef CETL_LLIST_H
#define CETL_LLIST_H

#include "../../src/utils/types/cetl_types.h"

// Create
cetl_llist *cetl_llist_create_empty(const struct cetl_element *type);
cetl_llist *cetl_llist_create_copy(const cetl_llist *src_llist);

// Operations
cetl_llist *cetl_llist_push_back(cetl_llist *llist, const cetl_ptr_t data);
cetl_llist *cetl_llist_pop_back(cetl_llist *llist);
cetl_llist *cetl_llist_push_front(cetl_llist *llist, const cetl_ptr_t data);
cetl_llist *cetl_llist_pop_front(cetl_llist *llist);
cetl_llist *cetl_llist_insert(cetl_llist *llist, const cetl_ptr_t data, cetl_size_t pos);
cetl_llist *cetl_llist_erase(cetl_llist *llist, cetl_size_t pos);
cetl_llist *cetl_llist_merge_two(const cetl_llist *llist1, const cetl_llist *llist2);
cetl_llist *cetl_llist_clear(cetl_llist *llist);
cetl_llist *cetl_llist_set(cetl_llist *llist, cetl_size_t pos, const cetl_ptr_t data);
cetl_ptr_t cetl_llist_get(const cetl_llist *llist, cetl_size_t pos);

// Size
cetl_size_t cetl_llist_size(const cetl_llist *llist);
cetl_bool_t cetl_llist_is_empty(const cetl_llist *llist);

// Iterator
cetl_iterator *cetl_llist_iter_begin(const cetl_llist* llist);
cetl_iterator *cetl_llist_iter_end(const cetl_llist* llist);

// Free
cetl_void_t cetl_llist_free(cetl_llist *llist);
cetl_void_t cetl_llist_free_nodes(cetl_llist *llist);

#endif // CETL_LLIST_H
