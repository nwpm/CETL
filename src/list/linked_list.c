#include "../../include/cstl/cstl_list.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

_cstl_node *cstl_list_create_node(void *data, size_t elem_size) {

  if (data == NULL) {
    return NULL;
  }

  _cstl_node *new__cstl_node = malloc(sizeof(_cstl_node));

  if (new__cstl_node == NULL) {
    return NULL;
  }

  void *data_copy = malloc(elem_size);

  if (data_copy == NULL) {
    free(new__cstl_node);
    return NULL;
  }

  memcpy(data_copy, data, elem_size);

  new__cstl_node->data = data_copy;
  new__cstl_node->next = NULL;

  return new__cstl_node;
}

cstl_list *cstl_list_create_empty() {

  cstl_list *l = malloc(sizeof(cstl_list));

  if (l == NULL) {
    return NULL;
  }

  l->head = NULL;
  l->tail = NULL;
  l->elem_size = 0;
  l->size = 0;

  return l;
}

cstl_list *cstl_list_create(void *data, size_t elem_size) {

  if (data == NULL || elem_size == 0) {
    return NULL;
  }

  cstl_list *l = cstl_list_create_empty();

  if (l == NULL) {
    return NULL;
  }

  _cstl_node *head = cstl_list_create_node(data, elem_size);

  if (head == NULL) {
    return NULL;
  }

  l->elem_size = elem_size;
  l->head = head;
  l->tail = head;
  l->size = 1;

  return l;
}

cstl_list *cstl_list_create_copy(cstl_list *l) {

  if (l == NULL) {
    return NULL;
  }

  cstl_list *l_copy = cstl_list_create_empty();

  if (l_copy == NULL) {
    return NULL;
  }

  if (cstl_list_is_empty(l)) {
    return l_copy;
  }

  l_copy->elem_size = l->elem_size;

  _cstl_node *current = l->head;

  while (current) {
    cstl_list *res = cstl_list_push_back(l_copy, current->data, l->elem_size);
    current = current->next;

    if (res == NULL) {
      cstl_list_free_nodes(l_copy);
      free(l_copy);
      return NULL;
    }
  }

  return l_copy;
}

cstl_list *cstl_list_push_back(cstl_list *l, void *data, size_t elem_size) {

  if (l == NULL || data == NULL || elem_size == 0) {
    return NULL;
  }

  _cstl_node *new__cstl_node = cstl_list_create_node(data, elem_size);

  if (new__cstl_node == NULL) {
    return NULL;
  }

  if (l->size == 0) {
    l->elem_size = elem_size;
    l->head = new__cstl_node;
    l->tail = new__cstl_node;
    l->size++;
    return l;
  }

  _cstl_node *end = l->tail;
  l->tail = new__cstl_node;
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
    free(l->tail->data);
    free(l->tail);
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
  }

  _cstl_node *current = l->head;

  while (current->next->next != NULL) {
    current = current->next;
  }

  free(l->tail->data);
  free(l->tail);

  current->next = NULL;
  l->tail = current;

  l->size--;

  return l;
}

cstl_list *cstl_list_push_front(cstl_list *l, void *data, size_t elem_size) {

  if (l == NULL || data == NULL || elem_size == 0) {
    return NULL;
  }

  _cstl_node *new__cstl_node = cstl_list_create_node(data, elem_size);

  if (new__cstl_node == NULL) {
    return NULL;
  }

  if (l->size == 0) {
    l->elem_size = elem_size;
    l->head = new__cstl_node;
    l->tail = new__cstl_node;
    l->size++;
    return l;
  }

  new__cstl_node->next = l->head;
  l->head = new__cstl_node;

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

  if (l->size == 1) {
    free(l->tail->data);
    free(l->tail);
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    return l;
  }

  _cstl_node *tmp_head = l->head;

  l->head = l->head->next;
  free(tmp_head->data);
  free(tmp_head);

  l->size--;

  return l;
}

cstl_list *cstl_list_insert(cstl_list *l, void *data, size_t pos,
                            size_t elem_size) {

  if (l == NULL || data == NULL || pos > l->size) {
    return NULL;
  }

  if (pos == l->size) {
    return cstl_list_push_back(l, data, elem_size);
  }

  if (pos == 0) {
    return cstl_list_push_front(l, data, elem_size);
  }

  _cstl_node *n = cstl_list_create_node(data, elem_size);

  if (n == NULL) {
    return NULL;
  }

  _cstl_node *prev = l->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cstl_node *next = prev->next;

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

  _cstl_node *prev = l->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cstl_node *next = prev->next->next;

  free(prev->next->data);
  free(prev->next);
  prev->next = next;

  l->size--;

  return l;
}

cstl_list *cstl_list_merge_two(cstl_list *l1, cstl_list *l2) {

  if (l1 == NULL || l2 == NULL) {
    return NULL;
  }

  cstl_list *c_l1 = cstl_list_create_copy(l1);
  cstl_list *c_l2 = cstl_list_create_copy(l2);

  if (cstl_list_is_empty(c_l1)) {
    free(c_l1);
    return c_l2;
  }

  if (cstl_list_is_empty(c_l2)) {
    free(c_l2);
    return c_l1;
  }

  c_l1->tail->next = c_l2->head;
  c_l1->tail = c_l2->tail;
  c_l1->size += c_l2->size;

  free(c_l2);

  return c_l1;
}

cstl_list *cstl_list_clear(cstl_list *l) {

  if (l == NULL) {
    return NULL;
  }

  cstl_list_free_nodes(l);
  l->head = NULL;
  l->tail = NULL;
  l->size = 0;

  return l;
}

size_t cstl_list_size(cstl_list *l) { return l->size; }

bool cstl_list_is_empty(cstl_list *l) {
  return !l->size && !l->elem_size && !l->head && !l->tail;
}

void *cstl_list_get(cstl_list *l, size_t pos) {

  if (l == NULL || pos >= l->size) {
    return NULL;
  }

  _cstl_node *current = l->head;

  for (size_t i = 0; i < pos; ++i) {
    current = current->next;
  }

  return current->data;
}

cstl_list *cstl_list_set(cstl_list *l, size_t pos, void *data) {

  if (l == NULL || data == NULL || pos >= l->size) {
    return NULL;
  }

  _cstl_node *current = l->head;

  for (size_t i = 0; i < pos; ++i) {
    current = current->next;
  }

  void *data_copy = malloc(l->elem_size);

  if (data_copy == NULL) {
    return NULL;
  }

  memcpy(data_copy, data, l->elem_size);

  free(current->data);
  current->data = data_copy;

  return l;
}

void cstl_list_free_nodes(cstl_list *l) {

  _cstl_node *current = l->head;
  _cstl_node *next = NULL;

  while (current != NULL) {
    next = current->next;
    free(current->data);
    free(current);
    current = next;
  }
}

void cstl_list_free(cstl_list *l){
  cstl_list_free_nodes(l);
  free(l);
}



