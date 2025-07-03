#include "cstl_dlist_internal.h"
#include "../../include/cstl/cstl_dlist.h"
#include <stdlib.h>
#include <string.h>

_cstl_dnode *_cstl_dlist_create_dnode(void *data, size_t elem_size) {

  if (data == NULL) {
    return NULL;
  }

  _cstl_dnode *dnode = malloc(sizeof(_cstl_dnode));

  if (dnode == NULL) {
    return NULL;
  }

  void *data_copy = malloc(elem_size);

  if (data_copy == NULL) {
    free(dnode);
    return NULL;
  }

  memcpy(data_copy, data, elem_size);

  dnode->data = data_copy;
  dnode->next = NULL;
  dnode->prev = NULL;

  return dnode;
}

cstl_dlist *cstl_dlist_create_empty() {

  cstl_dlist *dl = malloc(sizeof(cstl_dlist));

  if (dl == NULL) {
    return NULL;
  }

  dl->head = NULL;
  dl->tail = NULL;
  dl->elem_size = 0;
  dl->size = 0;

  return dl;
}

cstl_dlist *cstl_dlist_create(void *data, size_t elem_size) {

  if (data == NULL || elem_size == 0) {
    return NULL;
  }

  cstl_dlist *dl = cstl_dlist_create_empty();

  if (dl == NULL) {
    return NULL;
  }

  _cstl_dnode *head = _cstl_dlist_create_dnode(data, elem_size);

  if (head == NULL) {
    return NULL;
  }

  dl->elem_size = elem_size;
  dl->head = head;
  dl->tail = head;
  dl->size = 1;

  return dl;
}

cstl_dlist *cstl_dlist_create_copy(cstl_dlist *dl) {

  if (dl == NULL) {
    return NULL;
  }

  cstl_dlist *dl_copy = cstl_dlist_create_empty();

  if (dl_copy == NULL) {
    return NULL;
  }

  if (cstl_dlist_is_empty(dl)) {
    return dl_copy;
  }

  // NOTE: remove elem_size init lower?

  dl_copy->elem_size = dl->elem_size;

  _cstl_dnode *current = dl->head;

  while (current) {
    cstl_dlist *res =
        cstl_dlist_push_back(dl_copy, current->data, dl->elem_size);
    current = current->next;

    if (res == NULL) {
      cstl_dlist_free_nodes(dl_copy);
      free(dl_copy);
      return NULL;
    }
  }

  return dl_copy;
}

cstl_dlist *cstl_dlist_push_back(cstl_dlist *dl, void *data, size_t elem_size) {

  if (dl == NULL || data == NULL || elem_size == 0) {
    return NULL;
  }

  _cstl_dnode *dnode = _cstl_dlist_create_dnode(data, elem_size);

  if (dnode == NULL) {
    return NULL;
  }

  if (dl->size == 0) {
    dl->elem_size = elem_size;
    dl->head = dnode;
    dl->tail = dnode;
    dl->size++;
    return dl;
  }

  dnode->prev = dl->tail;

  dl->tail->next = dnode;
  dl->tail = dnode;

  dl->size++;

  return dl;
}

cstl_dlist *cstl_dlist_push_front(cstl_dlist *dl, void *data,
                                  size_t elem_size) {

  if (dl == NULL || data == NULL || elem_size == 0) {
    return NULL;
  }

  _cstl_dnode *dnode = _cstl_dlist_create_dnode(data, elem_size);

  if (dl->size == 0) {
    dl->elem_size = elem_size;
    dl->head = dnode;
    dl->tail = dnode;
    dl->size++;
    return dl;
  }

  dnode->next = dl->head;

  dl->head->prev = dnode;
  dl->head = dnode;

  dl->size++;

  return dl;
}

cstl_dlist *cstl_dlist_pop_back(cstl_dlist *dl) {

  if (dl == NULL) {
    return NULL;
  }

  if (dl->size == 0) {
    return dl;
  }

  _cstl_dnode *current = dl->tail->prev;

  free(dl->tail->data);
  free(dl->tail);

  if (current) {
    current->next = NULL;
    dl->tail = current;
  } else {
    dl->head = NULL;
    dl->tail = NULL;
  }

  dl->size--;

  return dl;
}

cstl_dlist *cstl_dlist_pop_front(cstl_dlist *dl) {

  if (dl == NULL) {
    return NULL;
  }

  if (dl->size == 0) {
    return dl;
  }

  _cstl_dnode *current = dl->head->next;

  free(dl->head->data);
  free(dl->head);

  if (current) {
    dl->head = current;
    dl->head->prev = NULL;
  } else {
    dl->head = NULL;
    dl->tail = NULL;
  }

  dl->size--;

  return dl;
}

cstl_dlist *cstl_dlist_insert(cstl_dlist *dl, void *data, size_t pos,
                              size_t elem_size) {

  if (dl == NULL || data == NULL || pos > dl->size) {
    return NULL;
  }

  if (pos == dl->size) {
    return cstl_dlist_push_back(dl, data, elem_size);
  }

  if (pos == 0) {
    return cstl_dlist_push_front(dl, data, elem_size);
  }

  _cstl_dnode *dnode = _cstl_dlist_create_dnode(data, elem_size);

  if (dnode == NULL) {
    return NULL;
  }

  _cstl_dnode *ins_next = dl->head;

  for (size_t i = 0; i < pos; ++i) {
    ins_next = ins_next->next;
  }

  _cstl_dnode *ins_prev = ins_next->prev;

  dnode->next = ins_next;
  dnode->prev = ins_prev;

  ins_next->prev = dnode;
  ins_prev->next = dnode;

  dl->size++;

  return dl;
}

cstl_dlist *cstl_dlist_erase(cstl_dlist *dl, size_t pos) {

  if (dl == NULL || pos >= dl->size) {
    return NULL;
  }

  if (pos == 0) {
    return cstl_dlist_pop_front(dl);
  }

  if (pos == dl->size - 1) {
    return cstl_dlist_pop_back(dl);
  }

  _cstl_dnode *del_node = dl->head;

  for (size_t i = 0; i < pos; ++i) {
    del_node = del_node->next;
  }

  _cstl_dnode *del_prev = del_node->prev;
  _cstl_dnode *del_next = del_node->next;

  del_prev->next = del_next;
  del_next->prev = del_prev;

  free(del_node->data);
  free(del_node);

  dl->size--;

  return dl;
}

cstl_dlist *cstl_dlist_merge_two(cstl_dlist *dl1, cstl_dlist *dl2) {

  if (dl1 == NULL || dl2 == NULL) {
    return NULL;
  }

  cstl_dlist *c_dl1 = cstl_dlist_create_copy(dl1);
  cstl_dlist *c_dl2 = cstl_dlist_create_copy(dl2);

  if (cstl_dlist_is_empty(c_dl1)) {
    free(c_dl1);
    return c_dl2;
  }

  if (cstl_dlist_is_empty(c_dl2)) {
    free(c_dl2);
    return c_dl1;
  }

  c_dl1->tail->next = c_dl2->head;
  c_dl2->head->prev = c_dl1->tail;
  c_dl1->tail = c_dl2->tail;
  c_dl1->size += c_dl2->size;

  free(c_dl2);

  return c_dl1;
}

cstl_dlist *cstl_dlist_clear(cstl_dlist *dl) {

  if (dl == NULL) {
    return NULL;
  }

  cstl_dlist_free_nodes(dl);

  dl->head = NULL;
  dl->tail = NULL;
  dl->size = 0;

  return dl;
}

cstl_dlist *cstl_dlist_set(cstl_dlist *dl, size_t pos, void *data) {

  if (dl == NULL || data == NULL || pos >= dl->size) {
    return NULL;
  }

  _cstl_dnode *target = dl->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  void *data_copy = malloc(dl->elem_size);

  if (data_copy == NULL) {
    return NULL;
  }

  memcpy(data_copy, data, dl->elem_size);

  free(target->data);
  target->data = data_copy;

  return dl;
}

size_t cstl_dlist_size(cstl_dlist *dl) { return dl->size; }

void *cstl_dlist_get(cstl_dlist *dl, size_t pos) {

  if (dl == NULL || pos >= dl->size) {
    return NULL;
  }

  _cstl_dnode *target = dl->head;

  for (size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  return target->data;
}

bool cstl_dlist_is_empty(cstl_dlist *dl) { return dl && !dl->size; }

void cstl_dlist_free_nodes(cstl_dlist *dl) {

  _cstl_dnode *current = dl->head;
  _cstl_dnode *next = NULL;

  while (current) {
    next = current->next;
    free(current->data);
    free(current);
    current = next;
  }
}

void cstl_dlist_free(cstl_dlist *dl) {
  cstl_dlist_free_nodes(dl);
  free(dl);
}
