#include "../../include/cstl/cstl_stack.h"
#include <stddef.h>
#include <stdint.h>

cstl_stack *cstl_stack_create_empty() {

  cstl_stack *s = malloc(sizeof(cstl_stack));

  if (s == NULL) {
    return NULL;
  }

  cstl_list *l = cstl_list_create_empty();

  if (l == NULL) {
    return NULL;
  }

  s->size = 0;
  s->data = l;

  return s;
}

cstl_stack *cstl_stack_create(void *data, size_t elem_size) {

  if (data == NULL || elem_size == 0) {
    return NULL;
  }

  cstl_stack *s = malloc(sizeof(cstl_stack));

  if (s == NULL) {
    return NULL;
  }

  cstl_list *l = cstl_list_create(data, elem_size);

  if (l == NULL) {
    return NULL;
  }

  s->size = 1;
  s->data = l;

  return s;
}

cstl_stack *cstl_stack_create_copy(cstl_stack *s) {

  if (s == NULL) {
    return NULL;
  }

  cstl_stack *res = cstl_stack_create_empty();

  res->size = s->size;

  _cstl_node *current = s->data->head;
  size_t elem_size = s->data->elem_size;

  while(current){
    cstl_list_push_back(res->data, current->data, elem_size);
    current = current->next;
  }

  return res;
}

cstl_stack *cstl_stack_push(cstl_stack *s, void *data, size_t elem_size) {

  if (s == NULL || data == NULL) {
    return NULL;
  }

  cstl_list_push_back(s->data, data, elem_size);
  s->size++;

  return s;
}

cstl_stack *cstl_stack_pop(cstl_stack *s) {

  if (s == NULL || s->size == 0) {
    return NULL;
  }

  cstl_list_pop_back(s->data);
  s->size--;

  return s;
}

bool cstl_stack_is_empty(cstl_stack *s){
  return cstl_list_is_empty(s->data);
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

  cstl_list_clear(s->data);
  s->size = 0;
}

void cstl_stack_free(cstl_stack *s) {

  if (s == NULL) {
    return;
  }

  cstl_list_free(s->data);
  free(s);
}
