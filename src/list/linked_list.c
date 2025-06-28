#include "../../include/cstl/cstl_list.h"
#include <stdlib.h>

cstl_node *cstl_list_create_node(void *data) {

  if (data == NULL) {
    return NULL;
  }

  cstl_node *new_cstl_node = malloc(sizeof(cstl_node));

  if (new_cstl_node == NULL) {
    return NULL;
  }

  new_cstl_node->data = data;
  new_cstl_node->next = NULL;

  return new_cstl_node;
}

cstl_list *cstl_list_create_empty() {

  cstl_list *l = malloc(sizeof(cstl_list));

  if (l == NULL) {
    return NULL;
  }

  l->head = NULL;
  l->tail = NULL;
  l->size = 0;

  return l;
}

cstl_list *cstl_list_create(void *data, size_t elem_size) {

  if (data == NULL) {
    return NULL;
  }

  cstl_list *l = cstl_list_create_empty();

  if (l == NULL) {
    return NULL;
  }

  cstl_node *head = cstl_list_create_node(data);

  if (head == NULL) {
    return NULL;
  }

  l->elem_size = elem_size;
  l->head = head;
  l->tail = head;
  l->size = 1;

  return l;
}

cstl_list *cstl_list_push_back(cstl_list *l, void *data) {

  if (l == NULL || data == NULL) {
    return NULL;
  }

  cstl_node *new_cstl_node = cstl_list_create_node(data);

  if (new_cstl_node == NULL) {
    return NULL;
  }

  cstl_node *end = l->tail;

  l->tail = new_cstl_node;

  end->next = l->tail;

  l->size++;

  return l;
}

cstl_list *cstl_list_pop_back(cstl_list *l) {

  if (l == NULL) {
    return NULL;
  }

  if (l->size == 0) {
    return l;
  }

  if (l->size == 1) {
    free(l->tail);
    l->size = 0;
    return l;
  }

  cstl_node *current = l->head;

  while (current->next->next != NULL) {
    current = current->next;
  }

  free(l->tail);

  current->next = NULL;
  l->tail = current;

  l->size--;

  return l;
}

cstl_list *cstl_list_push_front(cstl_list *l, void *data) {

  if (l == NULL || data == NULL) {
    return NULL;
  }

  cstl_node *new_cstl_node = cstl_list_create_node(data);

  if (new_cstl_node == NULL) {
    return NULL;
  }

  new_cstl_node->next = l->head;
  l->head = new_cstl_node;

  l->size++;

  return l;
}

cstl_list *cstl_list_pop_front(cstl_list *l) {

  if (l == NULL) {
    return NULL;
  }

  if (l->size == 0) {
    return l;
  }

  cstl_node *tmp_head = l->head;

  l->head = NULL;
  free(tmp_head);

  l->size--;

  return l;
}

cstl_list *cstl_list_insert(cstl_list *l, void *data, size_t pos) {

  if (pos > l->size) {
    return NULL;
  }

  cstl_node *n = cstl_list_create_node(data);

  if (n == NULL) {
    return NULL;
  }

  if (pos == 0) {
    n->next = l->head;
    l->head = n;
    return l;
  }

  if (pos == l->size) {
    l->tail->next = n;
    l->tail = n;
    return l;
  }

  cstl_node *prev = l->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  cstl_node *next = prev->next;

  prev->next = n;
  n->next = next;

  l->size++;

  return l;
}

cstl_list *cstl_list_erase(cstl_list *l, size_t pos) {

  if (pos >= l->size) {
    return NULL;
  }

  if (pos == 0) {
    return cstl_list_pop_front(l);
  }

  if (pos == l->size - 1) {
    return cstl_list_pop_back(l);
  }

  cstl_node *prev = l->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  cstl_node *next = prev->next->next;

  free(prev->next);
  prev->next = next;

  l->size--;

  return l;
}

cstl_list *cstl_list_merge_two(cstl_list *l1, cstl_list *l2) {
  l1->tail->next = l2->head;
  l1->size += l2->size;
  return l1;
}

cstl_list *cstl_list_clear(cstl_list *l) {

  if (l == NULL) {
    return NULL;
  }

  cstl_list_free_nodes(l);
  l->size = 0;

  return l;
}

size_t cstl_list_size(cstl_list *l) { return l->size; }

bool cstl_list_is_empty(cstl_list *l) { return !l->size; }

void *cstl_list_get(cstl_list *l, size_t pos){

  if(l == NULL || pos >= l->size){
    return NULL;
  }

  cstl_node *current = l->head;

  for (size_t i = 0; i < pos; ++i) {
    current = current->next;
  }

  return current->data;
}

cstl_list *cstl_list_set(cstl_list *l, size_t pos, void *data){

  if(l == NULL || data == NULL || pos >= l->size){
    return NULL;
  }

  cstl_node *current = l->head;

  for (size_t i = 0; i < pos; ++i) {
    current = current->next;
  }

  current->data = data;

  return l;
}

void cstl_list_free_nodes(cstl_list *l) {

  cstl_node *current = l->head;
  cstl_node *next = NULL;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}
