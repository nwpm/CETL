#include "../../../include/cetl/cetl_llist.h"
#include "../../../include/cetl/cetl_stack.h"
#include "../../utils/element/cetl_element.h"
#include "../../utils/iterator/iterator.h"

#include <stdlib.h>

struct cetl_stack {

  cetl_size_t size;
  struct cetl_llist *data;
  const cetl_element *type;
};

cetl_stack *cetl_stack_create_empty(const cetl_element *type) {

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

typedef struct _cetl_stack_iter_state {

  const cetl_stack *container;
  cetl_ptr_t llist_it;

} _cetl_stack_iter_state;

static cetl_ptr_t _cetl_stack_iter_get(const cetl_iterator *it) {

  const _cetl_stack_iter_state *state = (_cetl_stack_iter_state *)it->state;
  cetl_iterator* llist_it = (cetl_iterator*)state->llist_it;
  return llist_it->get(llist_it);
}

static cetl_void_t _cetl_stack_iter_next(const cetl_iterator *it) {

  _cetl_stack_iter_state *state = (_cetl_stack_iter_state *)it->state;
  cetl_iterator* llist_it = (cetl_iterator*)state->llist_it;
  llist_it->next(llist_it);
}

static cetl_bool_t _cetl_stack_iter_equal(const cetl_iterator *a,
                                          const cetl_iterator *b) {

  const _cetl_stack_iter_state *state_a = (_cetl_stack_iter_state *)a->state;
  const _cetl_stack_iter_state *state_b = (_cetl_stack_iter_state *)b->state;

  if ((state_a->container == state_b->container) &&
      (state_a->llist_it == state_b->llist_it)) {
    return CETL_TRUE;
  }

  return CETL_FALSE;
}

cetl_iterator *cetl_stack_iter_begin(const cetl_stack *stack) {

  _cetl_stack_iter_state *state = malloc(sizeof(_cetl_stack_iter_state));

  if (!state || (stack->size == 0))
    return NULL;

  cetl_iterator *llist_it = cetl_llist_iter_begin(stack->data);

  if (!llist_it)
    return NULL;

  state->container = stack;
  state->llist_it = llist_it;

  cetl_iterator *stack_it = malloc(sizeof(cetl_iterator));

  if (!stack_it)
    return NULL;

  stack_it->category = CETL_FORWARD_ITERATOR;
  stack_it->state = state;
  stack_it->get = _cetl_stack_iter_get;
  stack_it->next = _cetl_stack_iter_next;
  stack_it->equal = _cetl_stack_iter_equal;

  return stack_it;
}

cetl_iterator *cetl_stack_iter_end(const cetl_stack *stack) {

  _cetl_stack_iter_state *state = malloc(sizeof(_cetl_stack_iter_state));

  if (!state || (stack->size == 0))
    return NULL;

  cetl_iterator *llist_it = cetl_llist_iter_end(stack->data);

  if (!llist_it)
    return NULL;

  state->container = stack;
  state->llist_it = llist_it;

  cetl_iterator *stack_it = malloc(sizeof(cetl_iterator));

  if (!stack_it)
    return NULL;

  stack_it->category = CETL_FORWARD_ITERATOR;
  stack_it->state = state;
  stack_it->get = _cetl_stack_iter_get;
  stack_it->next = _cetl_stack_iter_next;
  stack_it->equal = _cetl_stack_iter_equal;

  return stack_it;
}

static cetl_cptr_t _cetl_stack_iter_cget(const cetl_const_iterator *it) {

  const _cetl_stack_iter_state *state = (_cetl_stack_iter_state *)it->state;
  const cetl_const_iterator* llist_it = (cetl_const_iterator*)state->llist_it;
  return llist_it->get(llist_it);
}

static cetl_void_t _cetl_stack_iter_cnext(const cetl_const_iterator *it) {

  _cetl_stack_iter_state *state = (_cetl_stack_iter_state *)it->state;
  cetl_const_iterator* llist_it = (cetl_const_iterator*)state->llist_it;
  llist_it->next(llist_it);
}

static cetl_bool_t _cetl_stack_iter_cequal(const cetl_const_iterator *a,
                                          const cetl_const_iterator *b) {

  const _cetl_stack_iter_state *state_a = (_cetl_stack_iter_state *)a->state;
  const _cetl_stack_iter_state *state_b = (_cetl_stack_iter_state *)b->state;

  if ((state_a->container == state_b->container) &&
      (state_a->llist_it == state_b->llist_it)) {
    return CETL_TRUE;
  }

  return CETL_FALSE;
}

cetl_const_iterator *cetl_stack_iter_cbegin(const cetl_stack *stack) {

  _cetl_stack_iter_state *state = malloc(sizeof(_cetl_stack_iter_state));

  if (!state || (stack->size == 0))
    return NULL;

  cetl_const_iterator *llist_it = cetl_llist_iter_cbegin(stack->data);

  if (!llist_it)
    return NULL;

  state->container = stack;
  state->llist_it = llist_it;

  cetl_const_iterator *stack_it = malloc(sizeof(cetl_const_iterator));

  if (!stack_it)
    return NULL;

  stack_it->category = CETL_CONST_ITERATOR;
  stack_it->state = state;
  stack_it->get = _cetl_stack_iter_cget;
  stack_it->next = _cetl_stack_iter_cnext;
  stack_it->equal = _cetl_stack_iter_cequal;

  return stack_it;
}

cetl_const_iterator *cetl_stack_iter_cend(const cetl_stack *stack) {

  _cetl_stack_iter_state *state = malloc(sizeof(_cetl_stack_iter_state));

  if (!state || (stack->size == 0))
    return NULL;

  cetl_const_iterator *llist_it = cetl_llist_iter_cend(stack->data);

  if (!llist_it)
    return NULL;

  state->container = stack;
  state->llist_it = llist_it;

  cetl_const_iterator *stack_it = malloc(sizeof(cetl_iterator));

  if (!stack_it)
    return NULL;

  stack_it->category = CETL_CONST_ITERATOR;
  stack_it->state = state;
  stack_it->get = _cetl_stack_iter_cget;
  stack_it->next = _cetl_stack_iter_cnext;
  stack_it->equal = _cetl_stack_iter_cequal;

  return stack_it;
}

cetl_void_t cetl_stack_iter_free(cetl_iterator *it) {

  if (!it)
    return;

  _cetl_stack_iter_state *state = (_cetl_stack_iter_state *)it->state;

  cetl_llist_iter_free(state->llist_it);
  free(state);
  free(it);
}

cetl_void_t cetl_stack_free(cetl_stack *s) {

  if (s == NULL) {
    return;
  }

  cetl_llist_free(s->data);
  free(s);
}
