#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node{
  
  int32_t data;
  struct node* next;

} node;

typedef struct list{
  
  size_t size;
  node* head;
  node* tail;

} list;

node* list_create_node(int32_t data);

list* list_create(list *l);
list* list_create_init(list *l, int32_t data);
list* list_push_back(list* l, int32_t data);
list* list_pop_back(list *l);
list* list_push_front(list *l, int32_t data);
list* list_pop_front(list *l);
list* list_insert(list *l, node* n, size_t pos);
list* list_erase(list *l, size_t pos);
list* list_merge_two(list *l1, list *l2);

bool list_is_empty(list *l);

void list_print_data(list *l);
void list_free_nodes(list* l);

#endif // LINKED_LIST_H
