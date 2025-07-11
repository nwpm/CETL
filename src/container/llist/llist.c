#include "../../../include/cetl/cetl_llist.h"
#include "cetl_llist_internal.h"
#include <stdlib.h>
#include <string.h>

static _cetl_node *_cetl_llist_create_node(const void *data,
                                           const cetl_llist *llist) {

  _cetl_node *node = malloc(sizeof(_cetl_node));

  if (node == NULL) {
    return NULL;
  }

  node->data = malloc(llist->type->size);

  if(node->data == NULL){
    free(node);
    return NULL;
  }

  if (llist->type->ctor) {
    llist->type->ctor(node->data, data);
  } else {
    memcpy(node->data, data, llist->type->size);
  }

  node->next = NULL;

  return node;
}

static void _cetl_llist_free_node_data(const cetl_llist *llist, void *data) {
  if (llist->type && llist->type->dtor) {
    llist->type->dtor(data);
  } else {
    free(data);
  }
}

cetl_llist *cetl_llist_create_empty(const cetl_type *type) {

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

  if (new_llist == NULL || cetl_llist_is_empty(src_llist)) {
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

cetl_llist *cetl_llist_push_back(cetl_llist *llist, const void *data) {

  if (llist == NULL || data == NULL) {
    return NULL;
  }

  _cetl_node *new_node = _cetl_llist_create_node(data, llist);

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

cetl_llist *cetl_llist_push_front(cetl_llist *llist, const void *data) {

  if (llist == NULL || data == NULL) {
    return NULL;
  }

  _cetl_node *node = _cetl_llist_create_node(data, llist);

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

cetl_llist *cetl_llist_insert(cetl_llist *llist, const void *data, size_t pos) {

  if (llist == NULL || data == NULL || pos > llist->size) {
    return NULL;
  }

  if (pos == llist->size) {
    return cetl_llist_push_back(llist, data);
  } else if (pos == 0) {
    return cetl_llist_push_front(llist, data);
  }

  _cetl_node *node = _cetl_llist_create_node(data, llist);

  if (node == NULL) {
    return NULL;
  }

  _cetl_node *prev = llist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cetl_node *next = prev->next;

  prev->next = node;
  node->next = next;

  llist->size++;

  return llist;
}

cetl_llist *cetl_llist_erase(cetl_llist *llist, size_t pos) {

  if (llist == NULL || pos >= llist->size) {
    return NULL;
  }

  if (pos == 0) {
    return cetl_llist_pop_front(llist);
  } else if (pos == llist->size - 1) {
    return cetl_llist_pop_back(llist);
  }

  _cetl_node *prev = llist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
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

size_t cetl_llist_size(const cetl_llist *llist) { return llist->size; }

bool cetl_llist_is_empty(const cetl_llist *llist) {
  return llist && !llist->size;
}

void *cetl_llist_get(const cetl_llist *llist, size_t pos) {

  if (llist == NULL || pos >= llist->size) {
    return NULL;
  }

  _cetl_node *target = llist->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  return target->data;
}

cetl_llist *cetl_llist_set(cetl_llist *llist, size_t pos, const void *data) {

  if (llist == NULL || data == NULL || pos >= llist->size) {
    return NULL;
  }

  _cetl_node *target = llist->head;

  for (size_t i = 0; i < pos; ++i) {
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

void cetl_llist_free_nodes(cetl_llist *llist) {

  _cetl_node *current = llist->head;

  while (current) {
    _cetl_llist_free_node_data(llist, current->data);
    current = current->next;
  }

  free(current);
}

void cetl_llist_free(cetl_llist *l) {
  cetl_llist_free_nodes(l);
  free(l);
}
