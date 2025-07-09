#include "cstl_dlist_internal.h"
#include "../../include/cstl/cstl_dlist.h"
#include <stdlib.h>
#include <string.h>

static _cstl_dnode *_cstl_dlist_create_dnode(const void *data, cstl_dlist *dlist) {

  if (data == NULL) {
    return NULL;
  }

  _cstl_dnode *dnode = malloc(sizeof(_cstl_dnode));

  if (dnode == NULL) {
    return NULL;
  }

  if(dlist->type->ctor){
    dlist->type->ctor(dnode->data, data);
  }else{
    memcpy(dnode->data, data, dlist->type->size);
  }

  dnode->next = NULL;
  dnode->prev = NULL;

  return dnode;
}

static void _cstl_dlist_free_node_data(cstl_dlist *dlist, void *data){
  if(dlist->type->dtor){
    dlist->type->dtor(data);
  }else{
    free(data);
  }
}

cstl_dlist *cstl_dlist_create_empty(cstl_type *type) {

  cstl_dlist *dlist = malloc(sizeof(cstl_dlist));

  if (dlist == NULL) {
    return NULL;
  }

  dlist->head = NULL;
  dlist->tail = NULL;
  dlist->type = type;
  dlist->size = 0;

  return dlist;
}

cstl_dlist *cstl_dlist_create_copy(cstl_dlist *dlist) {

  if (dlist == NULL) {
    return NULL;
  }

  cstl_dlist *dlist_copy = cstl_dlist_create_empty(dlist->type);

  if (dlist_copy == NULL || cstl_dlist_is_empty(dlist)) {
    return dlist_copy;
  }

  _cstl_dnode *current = dlist->head;

  while (current) {
    cstl_dlist *res =
        cstl_dlist_push_back(dlist_copy, current->data);
    current = current->next;

    if (res == NULL) {
      cstl_dlist_free_nodes(dlist_copy);
      free(dlist_copy);
      return NULL;
    }
  }

  return dlist_copy;
}

cstl_dlist *cstl_dlist_push_back(cstl_dlist *dlist, void *data) {

  if (dlist == NULL || data == NULL) {
    return NULL;
  }

  _cstl_dnode *dnode = _cstl_dlist_create_dnode(data, dlist);

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

cstl_dlist *cstl_dlist_push_front(cstl_dlist *dlist, void *data) {

  if (dlist == NULL || data == NULL) {
    return NULL;
  }

  _cstl_dnode *dnode = _cstl_dlist_create_dnode(data, dlist);

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

cstl_dlist *cstl_dlist_pop_back(cstl_dlist *dlist) {

  if (dlist == NULL || dlist->size == 0) {
    return dlist;
  }

  _cstl_dnode *new_tail = dlist->tail->prev;

  _cstl_dlist_free_node_data(dlist, dlist->tail->data);
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

cstl_dlist *cstl_dlist_pop_front(cstl_dlist *dlist) {

  if (dlist == NULL || dlist->size == 0) {
    return dlist;
  }

  _cstl_dnode *new_head = dlist->head->next;

  _cstl_dlist_free_node_data(dlist, dlist->head->data);
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

cstl_dlist *cstl_dlist_insert(cstl_dlist *dlist, void *data, size_t pos) {

  if (dlist == NULL || data == NULL || pos > dlist->size) {
    return NULL;
  }

  if (pos == dlist->size) {
    return cstl_dlist_push_back(dlist, data);
  }else if (pos == 0) {
    return cstl_dlist_push_front(dlist, data);
  }

  _cstl_dnode *dnode = _cstl_dlist_create_dnode(data, dlist);

  if (dnode == NULL) {
    return NULL;
  }

  _cstl_dnode *prev = dlist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cstl_dnode *next = prev->next;

  dnode->next = next;
  dnode->prev = prev;

  prev->next = dnode;
  next->prev = dnode;

  dlist->size++;

  return dlist;
}

cstl_dlist *cstl_dlist_erase(cstl_dlist *dlist, size_t pos) {

  if (dlist == NULL || pos >= dlist->size) {
    return NULL;
  }

  if (pos == 0) {
    return cstl_dlist_pop_front(dlist);
  }else if (pos == dlist->size - 1) {
    return cstl_dlist_pop_back(dlist);
  }

  _cstl_dnode *prev = dlist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cstl_dnode *next = prev->next->next;

  _cstl_dlist_free_node_data(dlist, prev->next->data);
  free(prev->next);

  prev->next = next;
  next->prev = prev;

  dlist->size--;

  return dlist;
}

cstl_dlist *cstl_dlist_merge_two(cstl_dlist *dlist1, cstl_dlist *dlist2) {

  if (dlist1 == NULL || dlist2 == NULL) {
    return NULL;
  }

  if (cstl_dlist_is_empty(dlist1)) {
    return cstl_dlist_create_copy(dlist2);
  }else if (cstl_dlist_is_empty(dlist2)) {
    return cstl_dlist_create_copy(dlist1);
  }

  cstl_dlist *copy_dlist1 = cstl_dlist_create_copy(dlist1);
  cstl_dlist *copy_dlist2 = cstl_dlist_create_copy(dlist2);

  copy_dlist1->tail->next = copy_dlist2->head;
  copy_dlist2->head->prev = copy_dlist1->tail;
  copy_dlist1->tail = copy_dlist2->tail;
  copy_dlist1->size += copy_dlist2->size;

  cstl_dlist_free(copy_dlist2);

  return copy_dlist1;
}

cstl_dlist *cstl_dlist_clear(cstl_dlist *dlist) {

  if (dlist == NULL) {
    return NULL;
  }

  cstl_dlist_free_nodes(dlist);

  dlist->head = NULL;
  dlist->tail = NULL;
  dlist->size = 0;

  return dlist;
}

cstl_dlist *cstl_dlist_set(cstl_dlist *dlist, size_t pos, void *data) {

  if (dlist == NULL || data == NULL || pos >= dlist->size) {
    return NULL;
  }

  _cstl_dnode *target = dlist->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  _cstl_dlist_free_node_data(dlist, target->data);

  if(dlist->type && dlist->type->ctor){
    dlist->type->ctor(target->data, data);
  }else{
    memcpy(target->data, data, dlist->type->size);
  }

  return dlist;
}

size_t cstl_dlist_size(cstl_dlist *dlist) { return dlist->size; }

void *cstl_dlist_get(cstl_dlist *dlist, size_t pos) {

  if (dlist == NULL || pos >= dlist->size) {
    return NULL;
  }

  _cstl_dnode *target = dlist->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  return target->data;
}

bool cstl_dlist_is_empty(cstl_dlist *dlist) { return dlist && !dlist->size; }

void cstl_dlist_free_nodes(cstl_dlist *dlist) {

  _cstl_dnode *current = dlist->head;

  while (current) {
    _cstl_dlist_free_node_data(dlist, current->data);
    free(current);
    current = current->next;
  }
}

void cstl_dlist_free(cstl_dlist *dlist) {
  cstl_dlist_free_nodes(dlist);
  free(dlist);
}
