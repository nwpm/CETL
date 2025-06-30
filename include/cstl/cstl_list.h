#ifndef CSTL_LIST_H
#define CSTL_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

// TODO: move all node part in internal header?
typedef struct _cstl_node {

  void *data;
  struct _cstl_node *next;

} _cstl_node;

typedef struct cstl_list {

  size_t elem_size;
  size_t size;
  _cstl_node *head;
  _cstl_node *tail;

} cstl_list;

_cstl_node *_cstl_list_create_node(void *data, size_t elem_size);

cstl_list *cstl_list_create_empty();
cstl_list *cstl_list_create(void *data, size_t elem_size);
cstl_list *cstl_list_create_copy(cstl_list *l);

cstl_list *cstl_list_push_back(cstl_list *l, void *data, size_t elem_size);
cstl_list *cstl_list_pop_back(cstl_list *l);
cstl_list *cstl_list_push_front(cstl_list *l, void *data, size_t elem_size);
cstl_list *cstl_list_pop_front(cstl_list *l);
cstl_list *cstl_list_insert(cstl_list *l, void *data, size_t pos, size_t elem_size);
cstl_list *cstl_list_erase(cstl_list *l, size_t pos);
cstl_list *cstl_list_merge_two(cstl_list *l1, cstl_list *l2);
cstl_list *cstl_list_clear(cstl_list *l);
cstl_list *cstl_list_set(cstl_list *l, size_t pos, void *data);

size_t cstl_list_size(cstl_list *l);

bool cstl_list_is_empty(cstl_list *l);

void *cstl_list_get(cstl_list *l, size_t pos);

void cstl_list_free(cstl_list *l);
void cstl_list_free_nodes(cstl_list *l);

#endif // CSTL_LIST_H
