#include "two_way_list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

node* dlist_create_node(int32_t data){

  node* new_node = (node*)malloc(sizeof(node));

  if(new_node == NULL){
    return NULL;
  }

  new_node->data = data;
  new_node->next = NULL;
  new_node->prev = NULL;

  return new_node;
}

dlist* dlist_create(dlist* l){
  node* head_node = dlist_create_node(0);

  l->head = head_node;
  l->tail = head_node;
  l->size = 1;

  return l;
}

dlist* dlist_create_init(dlist* l, int32_t data){
  node* head_node = dlist_create_node(data);

  l->head = head_node;
  l->tail = head_node;
  l->size = 1;

  return l;
}

dlist* dlist_push_back(dlist *l, int32_t data){
  node* new_node = dlist_create_node(data);

  new_node->prev = l->tail;

  l->tail->next = new_node;
  l->tail = new_node;

  l->size++;

  return l;
}

dlist* dlist_push_front(dlist* l, int32_t data){

  node* new_node = dlist_create_node(data);

  new_node->next = l->head;

  l->head->prev = new_node;
  l->head = new_node;

  l->size++;

  return l;
}

dlist* dlist_pop_back(dlist* l){

  if(l->size == 0){
    return NULL;
  }

  node* tmp = l->tail->prev;

  free(l->tail);

  if(tmp){
    tmp->next = NULL;
    l->tail = tmp;
  }else{
    l->head = NULL;
    l->tail = NULL;
  }

  l->size--;

  return l;
}

dlist* dlist_pop_front(dlist* l){

  if(l->size == 0){
    return NULL;
  }

  node *tmp = l->head->next;

  free(l->head);

  if(tmp){
    l->head = tmp;
    l->head->prev = NULL;
  }else{
    l->head = NULL;
    l->tail = NULL;
  }

  l->size--;

  return l;
}

dlist* dlist_insert(dlist* l, node* n, size_t pos){

  if(pos > l->size){
    return NULL;
  }

  if(pos == 0){
    n->next = l->head;
    n->prev = NULL;

    l->head->prev = n;
    l->head = n;

    l->size++;

    return l;
  }

  if(pos == l->size){
    n->next = NULL;
    n->prev = l->tail;

    l->tail->next = n;
    l->tail = n;

    l->size++;

    return l;
  }

  node* next = l->head;

  for(size_t i = 0; i < pos; ++i){
    next = next->next;
  }

  node* prev = next->prev;

  n->next = next;
  n->prev = prev;

  next->prev = n;
  prev->next = n;

  l->size++;

  return l;
}

dlist* dlist_erase(dlist* l, size_t pos){

  if(pos >= l->size){
    return NULL;
  }

  if(pos == 0){
    return dlist_pop_front(l);
  }

  if(pos == l->size - 1){
    return dlist_pop_back(l);
  }

  node* current = l->head;

  for(size_t i = 0; i < pos; ++i){
    current = current->next;
  }

  node* prev = current->prev;
  node* next = current->next;

  node* tmp = current;

  prev->next = next;
  next->prev = prev;

  free(tmp);

  l->size--;

  return l;
}

dlist* dlist_merge_two(dlist* l1, dlist* l2){
  l1->tail->next = l2->head;
  l1->size += l2->size;
  return l1;
}

bool dlist_is_empty(dlist *l){
  return !l->size;
}

void dlist_print_data(dlist *l){

  node* current = l->head;

  while(current != NULL){
    printf("%d\n", current->data);
    current = current->next;
  }

}

void dlist_free_nodes(dlist *l){

  node* current = l->head;
  node* tmp = NULL;

  while(current != NULL){
    tmp = current->next;
    free(current);
    current = tmp;
  }

}

int main(void){

  dlist* l = (dlist*)malloc(sizeof(dlist));

  dlist_create_init(l, 1);
  dlist_push_back(l, 99);

  dlist_print_data(l);
  printf("=======\n");

  node* n = dlist_create_node(12);

  dlist_insert(l, n, 2);

  dlist_print_data(l);
  printf("=======\n");

  dlist_erase(l, 1);

  dlist_print_data(l);
  printf("=======\n");

  dlist_free_nodes(l);

  free(l);

  return 0;
}


