#include "../../include/cetl/cetl_dlist.h"
#include "cetl_dlist_internal.h"
#include <stdlib.h>
#include <string.h>

static _cetl_dnode *_cetl_dlist_create_dnode(const void *data,
                                             cetl_dlist *dlist) {

  _cetl_dnode *dnode = malloc(sizeof(_cetl_dnode));

  if (dnode == NULL) {
    return NULL;
  }

  if (dlist->type->ctor) {
    dlist->type->ctor(dnode->data, data);
  } else {
    memcpy(dnode->data, data, dlist->type->size);
  }

  dnode->next = NULL;
  dnode->prev = NULL;

  return dnode;
}

static void _cetl_dlist_free_node_data(cetl_dlist *dlist, void *data) {
  if (dlist->type->dtor) {
    dlist->type->dtor(data);
  } else {
    free(data);
  }
}

cetl_dlist *cetl_dlist_create_empty(const cetl_type *type) {

  cetl_dlist *dlist = malloc(sizeof(cetl_dlist));

  if (dlist == NULL) {
    return NULL;
  }

  dlist->head = NULL;
  dlist->tail = NULL;
  dlist->type = type;
  dlist->size = 0;

  return dlist;
}

cetl_dlist *cetl_dlist_create_copy(const cetl_dlist *src_dlist) {

  if (src_dlist == NULL) {
    return NULL;
  }

  cetl_dlist *new_dlist = cetl_dlist_create_empty(src_dlist->type);

  if (new_dlist == NULL || cetl_dlist_is_empty(src_dlist)) {
    return new_dlist;
  }

  _cetl_dnode *current = src_dlist->head;

  while (current) {
    if (cetl_dlist_push_back(new_dlist, current->data) == NULL) {
      cetl_dlist_free_nodes(new_dlist);
      free(new_dlist);
      return NULL;
    }
    current = current->next;
  }

  return new_dlist;
}

cetl_dlist *cetl_dlist_push_back(cetl_dlist *dlist, const void *data) {

  if (dlist == NULL || data == NULL) {
    return NULL;
  }

  _cetl_dnode *dnode = _cetl_dlist_create_dnode(data, dlist);

  if (dnode == NULL) {
    return NULL;
  }

  if (dlist->size == 0) {
    dlist->head = dnode;
    dlist->tail = dnode;
    dlist->size++;
    return dlist;
  }

  dnode->prev = dlist->tail;

  dlist->tail->next = dnode;
  dlist->tail = dnode;

  dlist->size++;

  return dlist;
}

cetl_dlist *cetl_dlist_push_front(cetl_dlist *dlist, const void *data) {

  if (dlist == NULL || data == NULL) {
    return NULL;
  }

  _cetl_dnode *dnode = _cetl_dlist_create_dnode(data, dlist);

  if (dlist->size == 0) {
    dlist->head = dnode;
    dlist->tail = dnode;
    dlist->size++;
    return dlist;
  }

  dnode->next = dlist->head;

  dlist->head->prev = dnode;
  dlist->head = dnode;

  dlist->size++;

  return dlist;
}

cetl_dlist *cetl_dlist_pop_back(cetl_dlist *dlist) {

  if (dlist == NULL || dlist->size == 0) {
    return dlist;
  }

  _cetl_dnode *new_tail = dlist->tail->prev;

  _cetl_dlist_free_node_data(dlist, dlist->tail->data);
  free(dlist->tail);

  if (new_tail) {
    new_tail->next = NULL;
    dlist->tail = new_tail;
  } else {
    dlist->head = NULL;
    dlist->tail = NULL;
  }

  dlist->size--;

  return dlist;
}

cetl_dlist *cetl_dlist_pop_front(cetl_dlist *dlist) {

  if (dlist == NULL || dlist->size == 0) {
    return dlist;
  }

  _cetl_dnode *new_head = dlist->head->next;

  _cetl_dlist_free_node_data(dlist, dlist->head->data);
  free(dlist->head);

  if (new_head) {
    dlist->head = new_head;
    dlist->head->prev = NULL;
  } else {
    dlist->head = NULL;
    dlist->tail = NULL;
  }

  dlist->size--;

  return dlist;
}

cetl_dlist *cetl_dlist_insert(cetl_dlist *dlist, const void *data, size_t pos) {

  if (dlist == NULL || data == NULL || pos > dlist->size) {
    return NULL;
  }

  if (pos == dlist->size) {
    return cetl_dlist_push_back(dlist, data);
  } else if (pos == 0) {
    return cetl_dlist_push_front(dlist, data);
  }

  _cetl_dnode *dnode = _cetl_dlist_create_dnode(data, dlist);

  if (dnode == NULL) {
    return NULL;
  }

  _cetl_dnode *prev = dlist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cetl_dnode *next = prev->next;

  dnode->next = next;
  dnode->prev = prev;

  prev->next = dnode;
  next->prev = dnode;

  dlist->size++;

  return dlist;
}

cetl_dlist *cetl_dlist_erase(cetl_dlist *dlist, size_t pos) {

  if (dlist == NULL || pos >= dlist->size) {
    return NULL;
  }

  if (pos == 0) {
    return cetl_dlist_pop_front(dlist);
  } else if (pos == dlist->size - 1) {
    return cetl_dlist_pop_back(dlist);
  }

  _cetl_dnode *prev = dlist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cetl_dnode *next = prev->next->next;

  _cetl_dlist_free_node_data(dlist, prev->next->data);
  free(prev->next);

  prev->next = next;
  next->prev = prev;

  dlist->size--;

  return dlist;
}

cetl_dlist *cetl_dlist_merge_two(const cetl_dlist *dlist1,
                                 const cetl_dlist *dlist2) {

  if (dlist1 == NULL || dlist2 == NULL) {
    return NULL;
  }

  if (cetl_dlist_is_empty(dlist1)) {
    return cetl_dlist_create_copy(dlist2);
  } else if (cetl_dlist_is_empty(dlist2)) {
    return cetl_dlist_create_copy(dlist1);
  }

  cetl_dlist *copy_dlist1 = cetl_dlist_create_copy(dlist1);
  cetl_dlist *copy_dlist2 = cetl_dlist_create_copy(dlist2);

  copy_dlist1->tail->next = copy_dlist2->head;
  copy_dlist2->head->prev = copy_dlist1->tail;
  copy_dlist1->tail = copy_dlist2->tail;
  copy_dlist1->size += copy_dlist2->size;

  cetl_dlist_free(copy_dlist2);

  return copy_dlist1;
}

cetl_dlist *cetl_dlist_clear(cetl_dlist *dlist) {

  if (dlist == NULL) {
    return NULL;
  }

  cetl_dlist_free_nodes(dlist);

  dlist->head = NULL;
  dlist->tail = NULL;
  dlist->size = 0;

  return dlist;
}

cetl_dlist *cetl_dlist_set(cetl_dlist *dlist, size_t pos, const void *data) {

  if (dlist == NULL || data == NULL || pos >= dlist->size) {
    return NULL;
  }

  _cetl_dnode *target = dlist->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  _cetl_dlist_free_node_data(dlist, target->data);

  if (dlist->type && dlist->type->ctor) {
    dlist->type->ctor(target->data, data);
  } else {
    memcpy(target->data, data, dlist->type->size);
  }

  return dlist;
}

size_t cetl_dlist_size(const cetl_dlist *dlist) { return dlist->size; }

void *cetl_dlist_get(const cetl_dlist *dlist, size_t pos) {

  if (dlist == NULL || pos >= dlist->size) {
    return NULL;
  }

  _cetl_dnode *target = dlist->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  return target->data;
}

bool cetl_dlist_is_empty(const cetl_dlist *dlist) { return dlist && !dlist->size; }

void cetl_dlist_free_nodes(cetl_dlist *dlist) {

  _cetl_dnode *current = dlist->head;

  while (current) {
    _cetl_dlist_free_node_data(dlist, current->data);
    free(current);
    current = current->next;
  }
}

void cetl_dlist_free(cetl_dlist *dlist) {
  cetl_dlist_free_nodes(dlist);
  free(dlist);
}
