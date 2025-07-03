#include "../../include/cstl/cstl_cdlist.h"

cstl_cdlist *cstl_cdlist_create_empty() {

  cstl_cdlist *cdl = malloc(sizeof(cstl_cdlist));

  if(cdl == NULL){
    return NULL;
  }

  cstl_dlist *dl = cstl_dlist_create_empty();

  if(dl == NULL){
    free(cdl);
    return NULL;
  }

  cdl->size = 0;
  cdl->data = dl;

  return cdl;
}

cstl_cdlist *cstl_cdlist_create(void *data, size_t elem_size) {

  if(data == NULL || elem_size == 0){
    return NULL;
  }

  cstl_cdlist *cdl = cstl_cdlist_create_empty();

  if(cdl == NULL){
    return NULL;
  }

  cstl_dlist_push_back(cdl->data, data, elem_size);

  cdl->size = 1;

  cdl->data->tail->next = cdl->data->head;

  return cdl;
}

cstl_cdlist *cstl_cdlist_push_back(cstl_cdlist *l, int32_t data) {

  node *new_node = cstl_cdlist_create_node(data);

  l->tail->next = new_node;
  new_node->next = l->head;
  l->tail = new_node;

  l->size++;

  return l;
}

cstl_cdlist *cstl_cdlist_push_front(cstl_cdlist *l, int32_t data) {

  node *new_node = cstl_cdlist_create_node(data);

  new_node->next = l->head;
  l->tail->next = new_node;
  l->head = new_node;

  l->size++;

  return l;
}

cstl_cdlist *cstl_cdlist_pop_back(cstl_cdlist *l) {

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

cstl_cdlist* cstl_cdlist_pop_front(cstl_cdlist *l){

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

cstl_cdlist* cstl_cdlist_insert(cstl_cdlist* l, node* n, size_t pos){

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

cstl_cdlist* cstl_cdlist_erase(cstl_cdlist* l, size_t pos){
  
  if(pos >= l->size){
    return NULL;
  }

  if(pos == 0){
    return cstl_cdlist_pop_front(l);
  }

  if(pos == l->size - 1){
    return cstl_cdlist_pop_back(l);
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


void cstl_cdlist_print_data(cstl_cdlist *l) {

  if (l->size == 0) {
    return;
  }

  node *current = l->head;

  for (size_t i = 0; i < l->size; ++i) {
    printf("%d\n", current->data);
    current = current->next;
  }
}

void cstl_cdlist_free_nodes(cstl_cdlist *l) {

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
