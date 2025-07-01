#include "../../include/cstl/cstl_dlist.h"
#include <stddef.h>
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

  dnode->data = data;
  dnode->next = NULL;
  dnode->prev = NULL;

  return dnode;
}

cstl_dlist* cstl_dlist_create_empty(){

  cstl_dlist *dl = malloc(sizeof(cstl_dlist));

  if(dl == NULL){
    return NULL;
  }

  dl->head = NULL;
  dl->tail = NULL;
  dl->elem_size = 0;
  dl->size = 0;

  return dl;
}

cstl_dlist *cstl_dlist_create(void *data, size_t elem_size) {

  if(data == NULL || elem_size == 0){
    return NULL;
  }

  cstl_dlist *dl = cstl_dlist_create_empty();

  if(dl == NULL){
    return NULL;
  }

  _cstl_dnode *head = _cstl_dlist_create_dnode(data, elem_size);

  if(head == NULL){
    return NULL;
  }

  dl->elem_size = elem_size;
  dl->head = head;
  dl->tail = head;
  dl->size = 1;

  return dl;
}

cstl_dlist *cstl_dlist_create_copy(cstl_dlist *dl) {

  if(dl == NULL){
    return NULL;
  }

  cstl_dlist *dl_copy = cstl_dlist_create_empty();

  if(dl_copy == NULL){
    return NULL;
  }

  if(cstl_dlist_is_empty(dl)){
    return dl_copy;
  }

  //NOTE: remove elem_size init lower?

  dl_copy->elem_size = dl->elem_size;

  _cstl_dnode *current = dl->head;

  while(current){
    cstl_dlist *res = cstl_dlist_push_back(dl_copy, current->data, dl->elem_size);
    current = current->next;

    if(res == NULL){
      cstl_dlist_free_nodes(dl_copy);
      free(dl_copy);
      return NULL;
    }
  }

  return dl;
}

// NOTE: stop here
cstl_dlist *cstl_dlist_push_back(cstl_dlist *dl, void *data, size_t elem_size) {

  if(dl == NULL || data == NULL || elem_size == 0){
    return NULL;
  }

  _cstl_dnode *dnode = _cstl_dlist_create_dnode(data, elem_size);

  if(dnode == NULL){
    return NULL;
  }

  if(dl->size == 0){
    dl->elem_size = elem_size;
    dl->head = dnode;
    dl->tail = dnode;
    dl->size++;
    return dl;
  }

  new__cstl_dnode->prev = l->tail;

  l->tail->next = new__cstl_dnode;
  l->tail = new__cstl_dnode;

  l->size++;

  return l;
}

cstl_dlist *cstl_dlist_push_front(cstl_dlist *l, int32_t data) {

  _cstl_dnode *new__cstl_dnode = cstl_dlist_create__cstl_dnode(data);

  new__cstl_dnode->next = l->head;

  l->head->prev = new__cstl_dnode;
  l->head = new__cstl_dnode;

  l->size++;

  return l;
}

cstl_dlist *cstl_dlist_pop_back(cstl_dlist *l) {

  if (l->size == 0) {
    return NULL;
  }

  _cstl_dnode *tmp = l->tail->prev;

  free(l->tail);

  if (tmp) {
    tmp->next = NULL;
    l->tail = tmp;
  } else {
    l->head = NULL;
    l->tail = NULL;
  }

  l->size--;

  return l;
}

cstl_dlist *cstl_dlist_pop_front(cstl_dlist *l) {

  if (l->size == 0) {
    return NULL;
  }

  _cstl_dnode *tmp = l->head->next;

  free(l->head);

  if (tmp) {
    l->head = tmp;
    l->head->prev = NULL;
  } else {
    l->head = NULL;
    l->tail = NULL;
  }

  l->size--;

  return l;
}

cstl_dlist *cstl_dlist_insert(cstl_dlist *l, _cstl_dnode *n, size_t pos) {

  if (pos > l->size) {
    return NULL;
  }

  if (pos == 0) {
    n->next = l->head;
    n->prev = NULL;

    l->head->prev = n;
    l->head = n;

    l->size++;

    return l;
  }

  if (pos == l->size) {
    n->next = NULL;
    n->prev = l->tail;

    l->tail->next = n;
    l->tail = n;

    l->size++;

    return l;
  }

  _cstl_dnode *next = l->head;

  for (size_t i = 0; i < pos; ++i) {
    next = next->next;
  }

  _cstl_dnode *prev = next->prev;

  n->next = next;
  n->prev = prev;

  next->prev = n;
  prev->next = n;

  l->size++;

  return l;
}

cstl_dlist *cstl_dlist_erase(cstl_dlist *l, size_t pos) {

  if (pos >= l->size) {
    return NULL;
  }

  if (pos == 0) {
    return cstl_dlist_pop_front(l);
  }

  if (pos == l->size - 1) {
    return cstl_dlist_pop_back(l);
  }

  _cstl_dnode *current = l->head;

  for (size_t i = 0; i < pos; ++i) {
    current = current->next;
  }

  _cstl_dnode *prev = current->prev;
  _cstl_dnode *next = current->next;

  _cstl_dnode *tmp = current;

  prev->next = next;
  next->prev = prev;

  free(tmp);

  l->size--;

  return l;
}

cstl_dlist *cstl_dlist_merge_two(cstl_dlist *l1, cstl_dlist *l2) {
  l1->tail->next = l2->head;
  l1->size += l2->size;
  return l1;
}

bool cstl_dlist_is_empty(cstl_dlist *l) { return !l->size; }

void cstl_dlist_print_data(cstl_dlist *l) {

  _cstl_dnode *current = l->head;

  while (current != NULL) {
    printf("%d\n", current->data);
    current = current->next;
  }
}

void cstl_dlist_free__cstl_dnodes(cstl_dlist *l) {

  _cstl_dnode *current = l->head;
  _cstl_dnode *tmp = NULL;

  while (current != NULL) {
    tmp = current->next;
    free(current);
    current = tmp;
  }
}

int main(void) {

  cstl_dlist *l = (cstl_dlist *)malloc(sizeof(cstl_dlist));

  cstl_dlist_create_init(l, 1);
  cstl_dlist_push_back(l, 99);

  cstl_dlist_print_data(l);
  printf("=======\n");

  _cstl_dnode *n = cstl_dlist_create__cstl_dnode(12);

  cstl_dlist_insert(l, n, 2);

  cstl_dlist_print_data(l);
  printf("=======\n");

  cstl_dlist_erase(l, 1);

  cstl_dlist_print_data(l);
  printf("=======\n");

  cstl_dlist_free__cstl_dnodes(l);

  free(l);

  return 0;
}
