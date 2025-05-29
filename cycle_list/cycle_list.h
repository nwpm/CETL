#ifndef CYCLE_clist_H
#define CYCLE_clist_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node{
  
  int32_t data;
  struct node* next;

} node;

typedef struct clist{
  
  size_t size;
  node* head;
  node* tail;

} clist;

node* clist_create_node(int32_t data);

clist* clist_create(clist *l);
clist* clist_create_init(clist *l, int32_t data);
clist* clist_push_back(clist* l, int32_t data);
clist* clist_pop_back(clist *l);
clist* clist_push_front(clist *l, int32_t data);
clist* clist_pop_front(clist *l);
clist* clist_insert(clist *l, node* n, size_t pos);
clist* clist_erase(clist *l, size_t pos);
clist* clist_merge_two(clist *l1, clist *l2);

bool clist_is_empty(clist *l);

void clist_print_data(clist *l);
void clist_free_nodes(clist* l);

#endif // CYCLE_clist_H
