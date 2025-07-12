#include "../../../include/cetl/cetl_llist.h"
#include "../../../include/cetl/cetl_stack.h"
#include "cetl_stack_internal.h"
#include <stdlib.h>

cetl_stack *cetl_stack_create_empty(const cetl_type *type) {

  if (type == NULL) {
    return NULL;
  }

  cetl_stack *stack = malloc(sizeof(cetl_stack));

  if (stack == NULL) {
    return NULL;
  }

  cetl_llist *llist = cetl_llist_create_empty(type);

  if (llist == NULL) {
    free(stack);
    return NULL;
  }

  stack->size = 0;
  stack->data = llist;

  return stack;
}

cetl_stack *cetl_stack_create_copy(const cetl_stack *src_stack) {

  if (src_stack == NULL) {
    return NULL;
  }

  cetl_stack *new_stack = cetl_stack_create_empty(src_stack->type);

  if (new_stack == NULL || cetl_stack_is_empty(src_stack)) {
    return new_stack;
  }

  cetl_llist *llist_copy = cetl_llist_create_copy(src_stack->data);

  new_stack->size = src_stack->size;
  new_stack->data = llist_copy;

  return new_stack;
}

cetl_stack *cetl_stack_push(cetl_stack *stack, const cetl_ptr_t data) {

  if (cetl_llist_push_back(stack->data, data) == NULL) {
    return NULL;
  }

  stack->size++;

  return stack;
}

cetl_stack *cetl_stack_pop(cetl_stack *stack) {

  if (cetl_llist_pop_back(stack->data) == NULL) {
    return NULL;
  }

  stack->size--;

  return stack;
}

cetl_bool_t cetl_stack_is_empty(const cetl_stack *stack) {
  return cetl_llist_is_empty(stack->data);
}

cetl_ptr_t cetl_stack_top(const cetl_stack *stack) {
  return cetl_llist_get(stack->data, stack->size - 1);
}

cetl_void_t cetl_stack_clear(cetl_stack *stack) {
  cetl_llist_clear(stack->data);
  stack->size = 0;
}

cetl_void_t cetl_stack_free(cetl_stack *s) {

  if (s == NULL) {
    return;
  }

  cetl_llist_free(s->data);
  free(s);
}
