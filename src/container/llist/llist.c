#include "../../../include/cetl/cetl_llist.h"
#include "../../utils/element/cetl_element.h"
#include "../../utils/iterator/iterator.h"

#include <stdlib.h>
#include <string.h>

typedef struct _cetl_node {

  cetl_ptr_t data;
  struct _cetl_node *next;

} _cetl_node;

struct cetl_llist {

  cetl_size_t size;
  _cetl_node *head;
  _cetl_node *tail;
  const cetl_element *type;
};

static _cetl_node *_cetl_llist_create_node(const cetl_ptr_t data,
                                           const cetl_element *type) {

  _cetl_node *node = malloc(sizeof(_cetl_node));

  if (node == NULL) {
    return NULL;
  }

  node->data = malloc(type->size);

  if (node->data == NULL) {
    free(node);
    return NULL;
  }

  if (type->ctor) {
    type->ctor(node->data, data);
  } else {
    memcpy(node->data, data, type->size);
  }

  node->next = NULL;

  return node;
}

static void _cetl_llist_free_node_data(const cetl_llist *llist,
                                       cetl_ptr_t data) {
  if (llist->type && llist->type->dtor) {
    llist->type->dtor(data);
  } else {
    free(data);
  }
}

cetl_llist *cetl_llist_create_empty(const cetl_element *type) {

  if (type == NULL) {
    return NULL;
  }

  cetl_llist *llist = malloc(sizeof(cetl_llist));

  if (llist == NULL) {
    return NULL;
  }

  llist->head = NULL;
  llist->tail = NULL;
  llist->type = type;
  llist->size = 0;

  return llist;
}

cetl_llist *cetl_llist_create_copy(const cetl_llist *src_llist) {

  if (src_llist == NULL) {
    return NULL;
  }

  cetl_llist *new_llist = cetl_llist_create_empty(src_llist->type);

  if (new_llist == NULL || (src_llist->size == 0)) {
    return new_llist;
  }

  new_llist->type = src_llist->type;

  _cetl_node *current = src_llist->head;

  while (current) {
    if (cetl_llist_push_back(new_llist, current->data) == NULL) {
      cetl_llist_free_nodes(new_llist);
      free(new_llist);
      return NULL;
    }
    current = current->next;
  }

  return new_llist;
}

cetl_llist *cetl_llist_push_back(cetl_llist *llist, const cetl_ptr_t data) {

  if (llist == NULL || data == NULL) {
    return NULL;
  }

  _cetl_node *new_node = _cetl_llist_create_node(data, llist->type);

  if (new_node == NULL) {
    return NULL;
  }

  if (llist->size == 0) {
    llist->head = new_node;
    llist->tail = new_node;
    llist->size++;
    return llist;
  }

  _cetl_node *last_node = llist->tail;
  llist->tail = new_node;
  last_node->next = llist->tail;

  llist->size++;

  return llist;
}

cetl_llist *cetl_llist_pop_back(cetl_llist *llist) {

  if (llist == NULL || llist->size == 0) {
    return llist;
  }

  _cetl_node *to_delete = llist->tail;

  if (llist->size == 1) {
    llist->head = NULL;
    llist->tail = NULL;
  } else {

    _cetl_node *current = llist->head;

    while (current->next->next != NULL) {
      current = current->next;
    }
    current->next = NULL;
    llist->tail = current;
  }

  _cetl_llist_free_node_data(llist, to_delete->data);

  free(to_delete);
  llist->size--;

  return llist;
}

cetl_llist *cetl_llist_push_front(cetl_llist *llist, const cetl_ptr_t data) {

  if (llist == NULL || data == NULL) {
    return NULL;
  }

  _cetl_node *node = _cetl_llist_create_node(data, llist->type);

  if (node == NULL) {
    return NULL;
  }

  if (llist->size == 0) {
    llist->head = node;
    llist->tail = node;
  } else {
    node->next = llist->head;
    llist->head = node;
  }

  llist->size++;

  return llist;
}

cetl_llist *cetl_llist_pop_front(cetl_llist *llist) {

  if (llist == NULL || llist->size == 0) {
    return llist;
  }

  _cetl_node *to_delete = llist->head;

  if (llist->size == 1) {
    llist->head = NULL;
    llist->tail = NULL;
  } else {
    llist->head = llist->head->next;
  }

  _cetl_llist_free_node_data(llist, llist->head->data);
  free(to_delete);

  llist->size--;

  return llist;
}

cetl_llist *cetl_llist_insert(cetl_llist *llist, const cetl_ptr_t data,
                              cetl_size_t pos) {

  if (llist == NULL || data == NULL || pos > llist->size) {
    return NULL;
  }

  if (pos == llist->size) {
    return cetl_llist_push_back(llist, data);
  } else if (pos == 0) {
    return cetl_llist_push_front(llist, data);
  }

  _cetl_node *node = _cetl_llist_create_node(data, llist->type);

  if (node == NULL) {
    return NULL;
  }

  _cetl_node *prev = llist->head;

  for (cetl_size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cetl_node *next = prev->next;

  prev->next = node;
  node->next = next;

  llist->size++;

  return llist;
}

cetl_llist *cetl_llist_erase(cetl_llist *llist, cetl_size_t pos) {

  if (llist == NULL || pos >= llist->size) {
    return NULL;
  }

  if (pos == 0) {
    return cetl_llist_pop_front(llist);
  } else if (pos == llist->size - 1) {
    return cetl_llist_pop_back(llist);
  }

  _cetl_node *prev = llist->head;

  for (cetl_size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cetl_node *next = prev->next->next;

  _cetl_llist_free_node_data(llist, prev->next->data);
  free(prev->next);

  prev->next = next;

  llist->size--;

  return llist;
}

cetl_llist *cetl_llist_merge_two(const cetl_llist *llist1,
                                 const cetl_llist *llist2) {

  if (llist1 == NULL || llist2 == NULL) {
    return NULL;
  }

  if (cetl_llist_is_empty(llist1)) {
    return cetl_llist_create_copy(llist2);
  } else if (cetl_llist_is_empty(llist2)) {
    return cetl_llist_create_copy(llist1);
  }

  cetl_llist *copy_llist1 = cetl_llist_create_copy(llist1);
  cetl_llist *copy_llist2 = cetl_llist_create_copy(llist2);

  copy_llist1->tail->next = copy_llist2->head;
  copy_llist1->tail = copy_llist2->tail;
  copy_llist1->size += copy_llist2->size;

  cetl_llist_free(copy_llist2);

  return copy_llist1;
}

cetl_llist *cetl_llist_clear(cetl_llist *llist) {

  if (llist == NULL) {
    return NULL;
  }

  cetl_llist_free_nodes(llist);
  llist->head = NULL;
  llist->tail = NULL;
  llist->size = 0;

  return llist;
}

cetl_size_t cetl_llist_size(const cetl_llist *llist) { return llist->size; }

cetl_bool_t cetl_llist_is_empty(const cetl_llist *llist) {
  return llist && !llist->size;
}

cetl_ptr_t cetl_llist_get(const cetl_llist *llist, cetl_size_t pos) {

  if (llist == NULL || pos >= llist->size) {
    return NULL;
  }

  _cetl_node *target = llist->head;

  for (cetl_size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  return target->data;
}

cetl_llist *cetl_llist_set(cetl_llist *llist, cetl_size_t pos,
                           const cetl_ptr_t data) {

  if (llist == NULL || data == NULL || pos >= llist->size) {
    return NULL;
  }

  _cetl_node *target = llist->head;

  for (cetl_size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  _cetl_llist_free_node_data(llist, target->data);

  if (llist->type && llist->type->ctor) {
    llist->type->ctor(target->data, data);
  } else {
    memcpy(target->data, data, llist->type->size);
  }

  return llist;
}

typedef struct _cetl_llist_iter_state {

  const cetl_llist *container;
  _cetl_node *current_node;

} _cetl_llist_iter_state;

static cetl_ptr_t _cetl_llist_iter_get(const cetl_iterator *it) {

  const _cetl_llist_iter_state *state = (_cetl_llist_iter_state *)it->state;
  return state->current_node;
}

static cetl_void_t _cetl_llist_iter_next(const cetl_iterator *it) {

  _cetl_llist_iter_state *state = (_cetl_llist_iter_state *)it->state;
  state->current_node = state->current_node->next;
}

static cetl_bool_t _cetl_llist_iter_equal(const cetl_iterator *a,
                                          const cetl_iterator *b) {

  const _cetl_llist_iter_state *state_a = (_cetl_llist_iter_state *)a->state;
  const _cetl_llist_iter_state *state_b = (_cetl_llist_iter_state *)b->state;

  if ((state_a->container == state_b->container) &&
      (state_a->current_node == state_b->current_node)) {
    return CETL_TRUE;
  }

  return CETL_FALSE;
}

cetl_iterator *cetl_llist_iter_begin(const cetl_llist *llist) {

  _cetl_llist_iter_state *state = malloc(sizeof(_cetl_llist_iter_state));

  if (!state || (llist->size == 0))
    return NULL;

  state->container = llist;
  state->current_node = llist->head;

  cetl_iterator *it = malloc(sizeof(cetl_iterator));

  if (!it)
    return NULL;

  it->category = CETL_FORWARD_ITERATOR;
  it->state = state;
  it->get = _cetl_llist_iter_get;
  it->next = _cetl_llist_iter_next;
  it->equal = _cetl_llist_iter_equal;

  return it;
}

cetl_iterator *cetl_llist_iter_end(const cetl_llist *llist) {

  _cetl_llist_iter_state *state = malloc(sizeof(_cetl_llist_iter_state));

  if (!state || (llist->size == 0))
    return NULL;

  state->container = llist;
  state->current_node = llist->tail;

  cetl_iterator *it = malloc(sizeof(cetl_iterator));

  if (!it)
    return NULL;

  it->category = CETL_FORWARD_ITERATOR;
  it->state = state;
  it->get = _cetl_llist_iter_get;
  it->next = _cetl_llist_iter_next;
  it->equal = _cetl_llist_iter_equal;

  return it;
}

cetl_void_t cetl_llist_iter_free(cetl_iterator *it) {

  if (!it)
    return;

  free(it->state);
  free(it);
}

cetl_void_t cetl_llist_free_nodes(cetl_llist *llist) {

  _cetl_node *current = llist->head;

  for (cetl_size_t i = 0; i < llist->size; ++i) {
    _cetl_node *tmp = current;
    _cetl_llist_free_node_data(llist, current->data);
    current = current->next;
    free(tmp);
  }
}

cetl_void_t cetl_llist_free(cetl_llist *l) {
  cetl_llist_free_nodes(l);
  free(l);
}
