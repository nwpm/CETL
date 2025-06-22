#include "cycle_list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

node *clist_create_node(int32_t data) {

  node *n = (node *)malloc(sizeof(node));

  n->data = data;
  n->next = NULL;

  return n;
}

clist *clist_create(clist *l) {

  node *n = clist_create_node(0);

  l->head = n;
  l->tail = n;
  l->size = 1;

  l->tail->next = l->head;

  return l;
}

clist *clist_create_init(clist *l, int32_t data) {

  node *n = clist_create_node(data);

  l->head = n;
  l->tail = n;
  l->size = 1;

  l->tail->next = l->head;

  return l;
}

clist *clist_push_back(clist *l, int32_t data) {

  node *new_node = clist_create_node(data);

  l->tail->next = new_node;
  new_node->next = l->head;
  l->tail = new_node;

  l->size++;

  return l;
}

clist *clist_push_front(clist *l, int32_t data) {

  node *new_node = clist_create_node(data);

  new_node->next = l->head;
  l->tail->next = new_node;
  l->head = new_node;

  l->size++;

  return l;
}

clist *clist_pop_back(clist *l) {

  if (l->size == 0) {
    return NULL;
  }

  if (l->size == 1) {
    free(l->tail);
    l->size = 0;
    return l;
  }

  node *current = l->head;

  while (current->next != l->tail) {
    current = current->next;
  }

  free(l->tail);
  l->tail = current;
  l->tail->next = l->head;

  l->size--;

  return l;
}

clist* clist_pop_front(clist *l){

  if(l->size == 0){
    return NULL;
  }

  if(l->size == 1){
    free(l->tail);
    l->size = 0;
    return l;
  }

  node *tmp = l->head->next;

  free(l->head);

  l->tail->next = tmp;
  l->head = tmp;

  l->size--;

  return l;
}

clist* clist_insert(clist* l, node* n, size_t pos){

  if(pos > l->size){
    return NULL;
  }

  if(pos == 0){
    n->next = l->head;
    l->tail->next = n;
    l->head = n;
    return l;
  }

  if(pos == l->size){
    l->tail->next = n;
    n->next = l->head;
    l->tail = n;
    return l;
  }

  node* prev = l->head;

  for(size_t i = 0; i < pos - 1; ++i){
    prev = prev->next;
  }

  node* next = prev->next;

  prev->next = n;
  n->next = next;

  l->size++;

  return l;
}

clist* clist_erase(clist* l, size_t pos){
  
  if(pos >= l->size){
    return NULL;
  }

  if(pos == 0){
    return clist_pop_front(l);
  }

  if(pos == l->size - 1){
    return clist_pop_back(l);
  }

  node *prev = l->head;

  for(size_t i = 0; i < pos - 1; ++i){
    prev = prev->next;
  }

  node* next = prev->next->next;

  free(prev->next);

  prev->next = next;

  l->size--;

  return l;
}


void clist_print_data(clist *l) {

  if (l->size == 0) {
    return;
  }

  node *current = l->head;

  for (size_t i = 0; i < l->size; ++i) {
    printf("%d\n", current->data);
    current = current->next;
  }
}

void clist_free_nodes(clist *l) {

  if (l->size == 0) {
    return;
  }

  if (l->size == 1) {
    free(l->head);
    l->size = 0;
    return;
  }

  node *current = l->head->next;
  node *tmp = NULL;

  for (size_t i = 1; i < l->size; ++i) {
    tmp = current->next;
    free(current);
    current = tmp;
  }

  free(current);
}

int main(void) {

  clist *l = (clist *)malloc(sizeof(clist));

  clist_create_init(l, 10);

  clist_push_back(l, 11);
  clist_push_front(l, 9);

  node *n = clist_create_node(2);

  clist_insert(l, n, 2);

  clist_erase(l, 1);

  clist_print_data(l);

  clist_free_nodes(l);
  free(l);

  return 0;
}
