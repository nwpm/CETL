#include "../../include/cetl/cetl_cdlist.h"

cetl_cdlist *cetl_cdlist_create_empty() {

  cetl_cdlist *cdl = malloc(sizeof(cetl_cdlist));

  if(cdl == NULL){
    return NULL;
  }

  cetl_dlist *dl = cetl_dlist_create_empty();

  if(dl == NULL){
    free(cdl);
    return NULL;
  }

  cdl->size = 0;
  cdl->data = dl;

  return cdl;
}

cetl_cdlist *cetl_cdlist_create(void *data, size_t elem_size) {

  if(data == NULL || elem_size == 0){
    return NULL;
  }

  cetl_cdlist *cdl = cetl_cdlist_create_empty();

  if(cdl == NULL){
    return NULL;
  }

  cetl_dlist_push_back(cdl->data, data, elem_size);

  cdl->size = 1;

  cdl->data->tail->next = cdl->data->head;

  return cdl;
}

cetl_cdlist *cetl_cdlist_push_back(cetl_cdlist *l, int32_t data) {

  node *new_node = cetl_cdlist_create_node(data);

  l->tail->next = new_node;
  new_node->next = l->head;
  l->tail = new_node;

  l->size++;

  return l;
}

cetl_cdlist *cetl_cdlist_push_front(cetl_cdlist *l, int32_t data) {

  node *new_node = cetl_cdlist_create_node(data);

  new_node->next = l->head;
  l->tail->next = new_node;
  l->head = new_node;

  l->size++;

  return l;
}

cetl_cdlist *cetl_cdlist_pop_back(cetl_cdlist *l) {

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

cetl_cdlist* cetl_cdlist_pop_front(cetl_cdlist *l){

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

cetl_cdlist* cetl_cdlist_insert(cetl_cdlist* l, node* n, size_t pos){

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

cetl_cdlist* cetl_cdlist_erase(cetl_cdlist* l, size_t pos){
  
  if(pos >= l->size){
    return NULL;
  }

  if(pos == 0){
    return cetl_cdlist_pop_front(l);
  }

  if(pos == l->size - 1){
    return cetl_cdlist_pop_back(l);
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


void cetl_cdlist_print_data(cetl_cdlist *l) {

  if (l->size == 0) {
    return;
  }

  node *current = l->head;

  for (size_t i = 0; i < l->size; ++i) {
    printf("%d\n", current->data);
    current = current->next;
  }
}

void cetl_cdlist_free_nodes(cetl_cdlist *l) {

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
