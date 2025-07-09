#include "cstl_stack_internal.h"
#include "../../include/cstl/cstl_stack.h"
#include "../../include/cstl/cstl_llist.h"
#include <stdlib.h>

cstl_stack *cstl_stack_create_empty(cstl_type *t) {

  if(t == NULL){
    return NULL;
  }

  cstl_stack *s = malloc(sizeof(cstl_stack));

  if (s == NULL) {
    return NULL;
  }

  cstl_llist *l = cstl_llist_create_empty(t);

  if (l == NULL) {
    return NULL;
  }

  s->size = 0;
  s->data = l;

  return s;
}

cstl_stack *cstl_stack_create_copy(cstl_stack *s) {

  if (s == NULL) {
    return NULL;
  }

  cstl_stack *res = cstl_stack_create_empty(s->type);

  if(res == NULL){
    return NULL;
  }

  //TODO: make copy procces with iterator

  return res;
}

cstl_stack *cstl_stack_push(cstl_stack *s, void *data, size_t elem_size) {

  if (s == NULL || data == NULL) {
    return NULL;
  }

  cstl_llist_push_back(s->data, data, elem_size);
  s->size++;

  return s;
}

cstl_stack *cstl_stack_pop(cstl_stack *s) {

  if (s == NULL || s->size == 0) {
    return NULL;
  }

  cstl_llist_pop_back(s->data);
  s->size--;

  return s;
}

bool cstl_stack_is_empty(cstl_stack *s){
  return cstl_llist_is_empty(s->data);
}

void *cstl_stack_top(cstl_stack *s) {

  if (s == NULL || s->size == 0) {
    return NULL;
  }

  return s->data->tail->data;
}

void cstl_stack_clear(cstl_stack *s){

  if(s == NULL){
    return;
  }

  cstl_llist_clear(s->data);
  s->size = 0;
}

void cstl_stack_free(cstl_stack *s) {

  if (s == NULL) {
    return;
  }

  cstl_llist_free(s->data);
  free(s);
}
