#include "./linked_list.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

node *list_create_node(int32_t data) {

  node *new_node = (node *)malloc(sizeof(node));

  if (new_node == NULL) {
    return NULL;
  }

  new_node->data = data;
  new_node->next = NULL;

  return new_node;
}

list *list_create(list *l) {

  node *head = list_create_node(0);

  if (head == NULL) {
    return NULL;
  }

  l->head = head;
  l->tail = head;
  l->size++;

  return l;
}

list *list_create_init(list *l, int32_t data) {

  node *head = list_create_node(data);

  if (head == NULL) {
    return NULL;
  }

  l->head = head;
  l->tail = head;
  l->size++;

  return l;
}

list *list_push_back(list *l, int32_t data) {

  node *new_node = list_create_node(data);

  if (new_node == NULL) {
    return NULL;
  }

  node *end = l->tail;

  l->tail = new_node;

  end->next = l->tail;

  l->size++;

  return l;
}

list *list_pop_back(list *l) {

  if (l->size == 0) {
    return l;
  }

  if (l->size == 1) {
    free(l->tail);
    l->size = 0;
    return l;
  }

  node *current = l->head;

  while (current->next->next != NULL) {
    current = current->next;
  }

  free(l->tail);

  current->next = NULL;
  l->tail = current;

  l->size--;

  return l;
}

list *list_push_front(list *l, int32_t data) {

  node *new_node = list_create_node(data);

  if (new_node == NULL) {
    return NULL;
  }

  new_node->next = l->head;
  l->head = new_node;

  l->size++;

  return l;
}

list *list_pop_front(list *l) {

  if (l->size == 0) {
    return l;
  }

  node *tmp_head = l->head;

  l->head = l->head->next;

  free(tmp_head);

  l->size--;

  return l;
}

list *list_insert(list *l, node *n, size_t pos) {

  if (pos > l->size) {
    return NULL;
  }

  if (pos == 0) {
    n->next = l->head;
    l->head = n;
    return l;
  }

  if (pos == l->size) {
    n->next = NULL;
    l->tail->next = n;
    l->tail = n;
    return l;
  }

  node *prev = l->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  node *next = prev->next;

  prev->next = n;
  n->next = next;

  l->size++;

  return l;
}

list *list_erase(list *l, size_t pos) {

  if (pos >= l->size) {
    return NULL;
  }

  if (pos == 0) {
    return list_pop_front(l);
  }

  if (pos == l->size - 1) {
    return list_pop_back(l);
  }

  node *prev = l->head;

  for (size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  node *next = prev->next->next;

  free(prev->next);
  prev->next = next;

  l->size--;

  return l;
}

list *list_merge_two(list *l1, list *l2) {
  l1->tail->next = l2->head;
  l1->size += l2->size;
  return l1;
}

bool list_is_empty(list *l) { return !l->size; }

void list_print_data(list *l) {

  node *current = l->head;

  while (current != NULL) {
    printf("%d\n", current->data);
    current = current->next;
  }
}

void list_free(list *l) {

  node *current = l->head;
  node *next = NULL;

  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
}

int main(void) {

  list *p_l = (list *)malloc(sizeof(list));

  list_create_init(p_l, 10);
  list_push_back(p_l, 9);
  list_print_data(p_l);

  printf("--------\n");

  list_pop_back(p_l);
  list_print_data(p_l);

  printf("--------\n");

  list_push_front(p_l, 11);
  list_print_data(p_l);

  printf("--------\n");

  node *n = (node *)malloc(sizeof(node));
  n->data = 22;

  list_insert(p_l, n, 2);
  list_print_data(p_l);

  printf("--------\n");

  list_erase(p_l, 2);
  list_print_data(p_l);

  printf("--------\n");

  list_free(p_l);

  free(p_l);

  return 0;
}
