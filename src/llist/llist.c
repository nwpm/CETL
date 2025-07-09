#include "../../include/cstl/cstl_llist.h"
#include "cstl_llist_internal.h"
#include <stdlib.h>
#include <string.h>

static _cstl_node *_cstl_llist_create_node(const void *data, cstl_llist *llist) {

  if (data == NULL) {
    return NULL;
  }

  _cstl_node *node = malloc(sizeof(_cstl_node));

  if (node == NULL) {
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

static void _cstl_llist_free_node_data(cstl_llist *llist, void *data) {
  if (llist->type && llist->type->dtor) {
    llist->type->dtor(data);
  } else {
    free(data);
  }
}

cstl_llist *cstl_llist_create_empty(cstl_type *type) {

  cstl_llist *llist = malloc(sizeof(cstl_llist));

  if (llist == NULL) {
    return NULL;
  }

  llist->head = NULL;
  llist->tail = NULL;
  llist->type = type;
  llist->size = 0;

  return llist;
}

cstl_llist *cstl_llist_create_copy(cstl_llist *llist) {

  if (llist == NULL) {
    return NULL;
  }

  cstl_llist *llist_copy = cstl_llist_create_empty(llist->type);

  if (llist_copy == NULL) {
    return NULL;
  }

  if (cstl_llist_is_empty(llist)) {
    return llist_copy;
  }

  llist_copy->type = llist->type;

  _cstl_node *current = llist->head;

  while (current) {
    cstl_llist *res = cstl_llist_push_back(llist_copy, current->data);
    current = current->next;

    if (res == NULL) {
      cstl_llist_free_nodes(llist_copy);
      free(llist_copy);
      return NULL;
    }
  }

  return llist_copy;
}

cstl_llist *cstl_llist_push_back(cstl_llist *llist, const void *data) {

  if (llist == NULL || data == NULL) {
    return NULL;
  }

  _cstl_node *node = _cstl_llist_create_node(data, llist);

  if (node == NULL) {
    return NULL;
  }

  if (llist->size == 0) {
    llist->head = node;
    llist->tail = node;
    llist->size++;
    return llist;
  }

  _cstl_node *end = llist->tail;
  llist->tail = node;
  end->next = llist->tail;

  llist->size++;

  return llist;
}

cstl_llist *cstl_llist_pop_back(cstl_llist *llist) {

  if (llist == NULL || llist->size == 0) {
    return llist;
  }

  _cstl_node *to_delete = llist->tail;

  if (llist->size == 1) {
    llist->head = NULL;
    llist->tail = NULL;
  } else {

    _cstl_node *current = llist->head;

    while (current->next->next != NULL) {
      current = current->next;
    }
    current->next = NULL;
    llist->tail = current;
  }

  _cstl_llist_free_node_data(llist, to_delete->data);

  free(to_delete);
  llist->size--;

  return llist;
}

cstl_llist *cstl_llist_push_front(cstl_llist *llist, const void *data) {

  if (llist == NULL || data == NULL) {
    return NULL;
  }

  _cstl_node *node = _cstl_llist_create_node(data, llist);

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

cstl_llist *cstl_llist_pop_front(cstl_llist *llist) {

  if (llist == NULL || llist->size == 0) {
    return llist;
  }

  _cstl_node *to_delete = llist->head;

  if (llist->size == 1) {
    llist->head = NULL;
    llist->tail = NULL;
  } else {
    llist->head = llist->head->next;
  }

  _cstl_llist_free_node_data(llist, llist->head->data);
  free(to_delete);

  llist->size--;

  return llist;
}

cstl_llist *cstl_llist_insert(cstl_llist *llist, const void *data, size_t pos) {

  if (llist == NULL || data == NULL || pos > llist->size) {
    return NULL;
  }

  if (pos == llist->size) {
    return cstl_llist_push_back(llist, data);
  }else if (pos == 0) {
    return cstl_llist_push_front(llist, data);
  }

  _cstl_node *node = _cstl_llist_create_node(data, llist);

  if (node == NULL) {
    return NULL;
  }

  _cstl_node *prev = llist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cstl_node *next = prev->next;

  prev->next = node;
  node->next = next;

  llist->size++;

  return llist;
}

cstl_llist *cstl_llist_erase(cstl_llist *llist, size_t pos) {

  if (llist == NULL || pos >= llist->size) {
    return NULL;
  }

  if (pos == 0) {
    return cstl_llist_pop_front(llist);
  }else if (pos == llist->size - 1) {
    return cstl_llist_pop_back(llist);
  }

  _cstl_node *prev = llist->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cstl_node *next = prev->next->next;

  _cstl_llist_free_node_data(llist, prev->next->data);
  free(prev->next);

  prev->next = next;

  llist->size--;

  return llist;
}

cstl_llist *cstl_llist_merge_two(cstl_llist *llist1, cstl_llist *llist2) {

  if (llist1 == NULL || llist2 == NULL) {
    return NULL;
  }

  if (cstl_llist_is_empty(llist1)) {
    return cstl_llist_create_copy(llist2);
  }else if (cstl_llist_is_empty(llist2)) {
    return cstl_llist_create_copy(llist1);
  }

  cstl_llist *copy_llist1 = cstl_llist_create_copy(llist1);
  cstl_llist *copy_llist2 = cstl_llist_create_copy(llist2);

  copy_llist1->tail->next = copy_llist2->head;
  copy_llist1->tail = copy_llist2->tail;
  copy_llist1->size += copy_llist2->size;

  cstl_llist_free(copy_llist2);

  return copy_llist1;
}

cstl_llist *cstl_llist_clear(cstl_llist *llist) {

  if (llist == NULL) {
    return NULL;
  }

  cstl_llist_free_nodes(llist);
  llist->head = NULL;
  llist->tail = NULL;
  llist->size = 0;

  return llist;
}

size_t cstl_llist_size(cstl_llist *llist) { return llist->size; }

bool cstl_llist_is_empty(cstl_llist *llist) { return llist && !llist->size; }

void *cstl_llist_get(cstl_llist *llist, size_t pos) {

  if (llist == NULL || pos >= llist->size) {
    return NULL;
  }

  _cstl_node *target = llist->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  return target->data;
}

cstl_llist *cstl_llist_set(cstl_llist *llist, size_t pos, const void *data) {

  if (llist == NULL || data == NULL || pos >= llist->size) {
    return NULL;
  }

  _cstl_node *target = llist->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  _cstl_llist_free_node_data(llist, target->data);

  if(llist->type && llist->type->ctor){
    llist->type->ctor(target->data, data);
  }else{
    memcpy(target->data, data, llist->type->size);
  }

  return llist;
}

void cstl_llist_free_nodes(cstl_llist *llist) {

  _cstl_node *current = llist->head;

  while (current) {
    _cstl_llist_free_node_data(llist, current->data);
    free(current);
    current = current->next;
  }
}

void cstl_llist_free(cstl_llist *l) {
  cstl_llist_free_nodes(l);
  free(l);
}
