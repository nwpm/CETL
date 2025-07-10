#include "../../include/cstl/cstl_llist.h"
#include "../../include/cstl/cstl_stack.h"
#include "cstl_stack_internal.h"
#include <stdlib.h>

cstl_stack *cstl_stack_create_empty(const cstl_type *type) {

  if (type == NULL) {
    return NULL;
  }

  cstl_stack *stack = malloc(sizeof(cstl_stack));

  if (stack == NULL) {
    return NULL;
  }

  cstl_llist *llist = cstl_llist_create_empty(type);

  if (llist == NULL) {
    free(stack);
    return NULL;
  }

  stack->size = 0;
  stack->data = llist;

  return stack;
}

cstl_stack *cstl_stack_create_copy(const cstl_stack *src_stack) {

  if (src_stack == NULL) {
    return NULL;
  }

  cstl_stack *new_stack = cstl_stack_create_empty(src_stack->type);

  if (new_stack == NULL || cstl_stack_is_empty(src_stack)) {
    return new_stack;
  }

  cstl_llist *llist_copy = cstl_llist_create_copy(src_stack->data);

  new_stack->size = src_stack->size;
  new_stack->data = llist_copy;

  return new_stack;
}

cstl_stack *cstl_stack_push(cstl_stack *stack, const void *data) {

  if (cstl_llist_push_back(stack->data, data) == NULL) {
    return NULL;
  }

  stack->size++;

  return stack;
}

cstl_stack *cstl_stack_pop(cstl_stack *stack) {

  if (cstl_llist_pop_back(stack->data) == NULL) {
    return NULL;
  }

  stack->size--;

  return stack;
}

bool cstl_stack_is_empty(const cstl_stack *stack) {
  return cstl_llist_is_empty(stack->data);
}

void *cstl_stack_top(const cstl_stack *stack) {
  return cstl_llist_get(stack->data, stack->size - 1);
}

void cstl_stack_clear(cstl_stack *stack) {
  cstl_llist_clear(stack->data);
  stack->size = 0;
}

void cstl_stack_free(cstl_stack *s) {

  if (s == NULL) {
    return;
  }

  cstl_llist_free(s->data);
  free(s);
}
