#ifndef TWO_WAY_dlist_H
#define TWO_WAY_dlist_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node{
  
  int32_t data;
  struct node* next;
  struct node* prev;

} node;

typedef struct dlist{
  
  size_t size;
  node* head;
  node* tail;

} dlist;

node* dlist_create_node(int32_t data);

dlist* dlist_create(dlist *l);
dlist* dlist_create_init(dlist *l, int32_t data);
dlist* dlist_push_back(dlist* l, int32_t data);
dlist* dlist_pop_back(dlist *l);
dlist* dlist_push_front(dlist *l, int32_t data);
dlist* dlist_pop_front(dlist *l);
dlist* dlist_insert(dlist *l, node* n, size_t pos);
dlist* dlist_erase(dlist *l, size_t pos);
dlist* dlist_merge_two(dlist *l1, dlist *l2);

bool dlist_is_empty(dlist *l);

void dlist_print_data(dlist *l);
void dlist_free_nodes(dlist* l);


#endif //TWO_WAY_dlist_H
