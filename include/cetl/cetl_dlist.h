#ifndef CETL_DLIST_H
#define CETL_DLIST_H

#include "../../src/utils/types/cetl_types.h"

// Create
cetl_dlist *cetl_dlist_create_empty(const struct cetl_element *type);
cetl_dlist *cetl_dlist_create_copy(const cetl_dlist *src_dlist);

// Operations
cetl_dlist *cetl_dlist_push_back(cetl_dlist *dlist, const cetl_ptr_t data);
cetl_dlist *cetl_dlist_pop_back(cetl_dlist *dlist);
cetl_dlist *cetl_dlist_push_front(cetl_dlist *dlist, const cetl_ptr_t data);
cetl_dlist *cetl_dlist_pop_front(cetl_dlist *dlist);
cetl_dlist *cetl_dlist_insert(cetl_dlist *dlist, const cetl_ptr_t data,
                              cetl_size_t pos);
cetl_dlist *cetl_dlist_erase(cetl_dlist *dlist, cetl_size_t pos);
cetl_dlist *cetl_dlist_merge_two(const cetl_dlist *dlist1,
                                 const cetl_dlist *dlist2);
cetl_dlist *cetl_dlist_clear(cetl_dlist *dlist);
cetl_dlist *cetl_dlist_set(cetl_dlist *dlist, cetl_size_t pos,
                           const cetl_ptr_t data);
cetl_ptr_t cetl_dlist_get(const cetl_dlist *dlist, cetl_size_t pos);

// Size
cetl_size_t cetl_dlist_size(const cetl_dlist *dlist);
cetl_bool_t cetl_dlist_is_empty(const cetl_dlist *dlist);

// Iterator
cetl_iterator *cetl_dlist_iter_begin(const cetl_dlist* dlist);
cetl_iterator *cetl_dlist_iter_end(const cetl_dlist* dlist);

cetl_const_iterator *cetl_dlist_iter_cbegin(const cetl_dlist* dlist);
cetl_const_iterator *cetl_dlist_iter_cend(const cetl_dlist* dlist);

// Free
cetl_void_t cetl_dlist_free(cetl_dlist *dlist);
cetl_void_t cetl_dlist_free_nodes(cetl_dlist *dlist);
cetl_void_t cetl_dlist_iter_free(cetl_iterator *it);
cetl_void_t cetl_dlist_iter_cfree(cetl_const_iterator *it);


#endif // CETL_DLIST_H
