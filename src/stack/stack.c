#include "stack.h"
#include <stdint.h>
#include <stdio.h>

stack *stack_create(void) {

  stack *s = (stack *)malloc(sizeof(stack));

  if (s == NULL) {
    return NULL;
  }

  list *l = (list *)malloc(sizeof(list));

  if (l == NULL) {
    return NULL;
  }

  list_create(l);

  s->data_ptr = l;
  s->size = l->size;

  return s;
}

stack *stack_create_init(int32_t value) {

  stack *s = (stack *)malloc(sizeof(stack));

  if (s == NULL) {
    return NULL;
  }

  list *l = (list *)malloc(sizeof(list));

  if (l == NULL) {
    return NULL;
  }

  list_create_init(l, value);

  s->data_ptr = l;
  s->size = l->size;

  return s;
}

stack *stack_push(stack *s, int32_t value) {

  list_push_back(s->data_ptr, value);
  s->size = s->data_ptr->size;

  return s;
}

stack *stack_pop(stack *s) {

  if (s->size == 0) {
    return NULL;
  }

  list_pop_back(s->data_ptr);
  s->size = s->data_ptr->size;

  return s;
}

int32_t stack_top(stack *s) {
  if (s->size == 0) {
    return -1; //???
  }
  return s->data_ptr->tail->data;
}

void stack_print_data(stack *s) { list_print_data(s->data_ptr); }

void stack_free(stack *s) {

  list_free_nodes(s->data_ptr);
  free(s->data_ptr);
  free(s);

}

int main(void) {

  stack *s = stack_create_init(2);

  stack_push(s, 3);
  stack_pop(s);

  stack_print_data(s);

  stack_free(s);

  return 0;
}
