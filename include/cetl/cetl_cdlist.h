#ifndef CETL_CDLIST_H
#define CETL_CDLIST_H

#include "../../src/utils/types/cetl_types.h"

// Create
cetl_cdlist *cetl_cdlist_create_empty(const struct cetl_element *type);
cetl_cdlist *cetl_cdlist_create_copy(const cetl_cdlist *src_cdlist);

// Operations
cetl_cdlist *cetl_cdlist_push_back(cetl_cdlist *cdlist, const cetl_ptr_t data);
cetl_cdlist *cetl_cdlist_pop_back(cetl_cdlist *cdlist);
cetl_cdlist *cetl_cdlist_push_front(cetl_cdlist *cdlist, const cetl_ptr_t data);
cetl_cdlist *cetl_cdlist_pop_front(cetl_cdlist *cdlist);
cetl_cdlist *cetl_cdlist_insert(cetl_cdlist *cdlist, const cetl_ptr_t data,
                                cetl_size_t pos);
cetl_cdlist *cetl_cdlist_erase(cetl_cdlist *cdlist, cetl_size_t pos);
cetl_cdlist *cetl_cdlist_merge_two(const cetl_cdlist *cdlist1,
                                   const cetl_cdlist *cdlist2);
cetl_cdlist *cetl_cdlist_clear(cetl_cdlist *cdlist);
cetl_cdlist *cetl_cdlist_set(cetl_cdlist *cdlist, cetl_size_t pos,
                             const cetl_ptr_t data);
cetl_ptr_t cetl_cdlist_get(const cetl_cdlist *cdlist, cetl_size_t pos);

// Size
cetl_size_t cetl_cdlist_size(const cetl_cdlist *cdl);
cetl_bool_t cetl_cdlist_is_empty(const cetl_cdlist *cdl);

// Iterator
cetl_iterator *cetl_cdlist_iter_begin(const cetl_cdlist* cdlist);
cetl_iterator *cetl_cdlist_iter_end(const cetl_cdlist* cdlist);

cetl_const_iterator *cetl_cdlist_iter_cbegin(const cetl_cdlist *cdlist);
cetl_const_iterator *cetl_cdlist_iter_cend(const cetl_cdlist *cdlist);

// Free
cetl_void_t cetl_cdlist_free(cetl_cdlist *cdl);
cetl_void_t cetl_cdlist_free_nodes(cetl_cdlist *cdl);
cetl_void_t cetl_cdlist_iter_free(cetl_iterator* it);
cetl_void_t cetl_cdlist_iter_cfree(cetl_const_iterator* it);


#endif // CETL_CDLIST_H
