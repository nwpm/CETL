#include "../../../include/cetl/cetl_llist.h"
#include "../../../include/cetl/cetl_queue.h"
#include "../../utils/element/cetl_element.h"
#include "../../utils/iterator/iterator.h"

#include <stdlib.h>

struct cetl_queue {

  cetl_size_t size;
  struct cetl_llist *data;
  const struct cetl_element *type;
};

cetl_queue *cetl_queue_create_empty(const cetl_element *type) {

  if (type == NULL) {
    return NULL;
  }

  cetl_queue *queue = malloc(sizeof(cetl_queue));

  if (queue == NULL) {
    return NULL;
  }

  cetl_llist *llist = cetl_llist_create_empty(type);

  if (llist == NULL) {
    free(queue);
    return NULL;
  }

  queue->size = 0;
  queue->data = llist;

  return queue;
}

cetl_queue *cetl_queue_create_copy(const cetl_queue *src_queue) {

  if (src_queue == NULL) {
    return NULL;
  }

  cetl_queue *new_queue = cetl_queue_create_empty(src_queue->type);

  if (new_queue == NULL || cetl_queue_is_empty(src_queue)) {
    return new_queue;
  }

  cetl_llist *llist_copy = cetl_llist_create_copy(src_queue->data);

  if (llist_copy == NULL) {
    cetl_queue_free(new_queue);
    return NULL;
  }

  new_queue->size = src_queue->size;
  new_queue->data = llist_copy;

  return new_queue;
}

cetl_ptr_t cetl_queue_front(const cetl_queue *queue) {
  return cetl_llist_get(queue->data, 0);
}

cetl_ptr_t cetl_queue_back(const cetl_queue *queue) {
  return cetl_llist_get(queue->data, queue->size - 1);
}

cetl_queue *cetl_queue_push(cetl_queue *queue, const cetl_ptr_t data) {

  if(cetl_llist_push_back(queue->data, data) == NULL){
    return NULL;
  }
  
  queue->size++;

  return queue;
}

cetl_queue *cetl_queue_pop(cetl_queue *queue) {

  if (cetl_llist_pop_front(queue->data) == NULL) {
    return NULL;
  }

  queue->size--;

  return queue;
}

cetl_size_t cetl_queue_size(const cetl_queue *queue) { return queue->size; }

cetl_bool_t cetl_queue_is_empty(const cetl_queue *queue) { return queue && !queue->size; }

cetl_void_t cetl_queue_swap(cetl_queue **queue1, cetl_queue **queue2){

  if(queue1 == NULL || queue2 == NULL){
    return;
  }

  cetl_queue *tmp = *queue1;
  *queue1 = *queue2;
  *queue2 = tmp;

}

typedef struct _cetl_queue_iter_state {

  const cetl_queue *container;
  cetl_ptr_t llist_it;

} _cetl_queue_iter_state;

static cetl_ptr_t _cetl_queue_iter_get(const cetl_iterator *it) {

  const _cetl_queue_iter_state *state = (_cetl_queue_iter_state *)it->state;
  cetl_iterator *llist_it = (cetl_iterator*)state->llist_it;
  return llist_it->get(llist_it);
}

static cetl_void_t _cetl_queue_iter_next(const cetl_iterator *it) {

  _cetl_queue_iter_state *state = (_cetl_queue_iter_state *)it->state;
  cetl_iterator *llist_it = (cetl_iterator*)state->llist_it;
  llist_it->next(llist_it);
}

static cetl_bool_t _cetl_queue_iter_equal(const cetl_iterator *a,
                                          const cetl_iterator *b) {

  const _cetl_queue_iter_state *state_a = (_cetl_queue_iter_state *)a->state;
  const _cetl_queue_iter_state *state_b = (_cetl_queue_iter_state *)b->state;

  if ((state_a->container == state_b->container) &&
      (state_a->llist_it == state_b->llist_it)) {
    return CETL_TRUE;
  }

  return CETL_FALSE;
}

cetl_iterator *cetl_queue_iter_begin(const cetl_queue *queue) {

  _cetl_queue_iter_state *state = malloc(sizeof(_cetl_queue_iter_state));

  if (!state || (queue->size == 0))
    return NULL;

  cetl_iterator *llist_it = cetl_llist_iter_begin(queue->data);

  if (!llist_it)
    return NULL;

  state->container = queue;
  state->llist_it = llist_it;

  cetl_iterator *queue_it = malloc(sizeof(cetl_iterator));

  if (!queue_it)
    return NULL;

  queue_it->category = CETL_FORWARD_ITERATOR;
  queue_it->state = state;
  queue_it->get = _cetl_queue_iter_get;
  queue_it->next = _cetl_queue_iter_next;
  queue_it->equal = _cetl_queue_iter_equal;

  return queue_it;
}

cetl_iterator *cetl_queue_iter_end(const cetl_queue *queue) {

  _cetl_queue_iter_state *state = malloc(sizeof(_cetl_queue_iter_state));

  if (!state || (queue->size == 0))
    return NULL;

  cetl_iterator *llist_it = cetl_llist_iter_end(queue->data);

  if (!llist_it)
    return NULL;

  state->container = queue;
  state->llist_it = llist_it;

  cetl_iterator *queue_it = malloc(sizeof(cetl_iterator));

  if (!queue_it)
    return NULL;

  queue_it->category = CETL_FORWARD_ITERATOR;
  queue_it->state = state;
  queue_it->get = _cetl_queue_iter_get;
  queue_it->next = _cetl_queue_iter_next;
  queue_it->equal = _cetl_queue_iter_equal;

  return queue_it;
}

static cetl_cptr_t _cetl_queue_iter_cget(const cetl_const_iterator *it) {

  const _cetl_queue_iter_state *state = (_cetl_queue_iter_state *)it->state;
  const cetl_const_iterator* llist_it = (cetl_const_iterator*)state->llist_it;
  return llist_it->get(llist_it);
}

static cetl_void_t _cetl_queue_iter_cnext(const cetl_const_iterator *it) {

  _cetl_queue_iter_state *state = (_cetl_queue_iter_state *)it->state;
  cetl_const_iterator* llist_it = (cetl_const_iterator*)state->llist_it;
  llist_it->next(llist_it);
}

static cetl_bool_t _cetl_queue_iter_cequal(const cetl_const_iterator *a,
                                          const cetl_const_iterator *b) {

  const _cetl_queue_iter_state *state_a = (_cetl_queue_iter_state *)a->state;
  const _cetl_queue_iter_state *state_b = (_cetl_queue_iter_state *)b->state;

  if ((state_a->container == state_b->container) &&
      (state_a->llist_it == state_b->llist_it)) {
    return CETL_TRUE;
  }

  return CETL_FALSE;
}

cetl_const_iterator *cetl_queue_iter_cbegin(const cetl_queue *queue) {

  _cetl_queue_iter_state *state = malloc(sizeof(_cetl_queue_iter_state));

  if (!state || (queue->size == 0))
    return NULL;

  cetl_const_iterator *llist_it = cetl_llist_iter_cbegin(queue->data);

  if (!llist_it)
    return NULL;

  state->container = queue;
  state->llist_it = llist_it;

  cetl_const_iterator *queue_it = malloc(sizeof(cetl_const_iterator));

  if (!queue_it)
    return NULL;

  queue_it->category = CETL_CONST_ITERATOR;
  queue_it->state = state;
  queue_it->get = _cetl_queue_iter_cget;
  queue_it->next = _cetl_queue_iter_cnext;
  queue_it->equal = _cetl_queue_iter_cequal;

  return queue_it;
}

cetl_const_iterator *cetl_queue_iter_cend(const cetl_queue *queue) {

  _cetl_queue_iter_state *state = malloc(sizeof(_cetl_queue_iter_state));

  if (!state || (queue->size == 0))
    return NULL;

  cetl_const_iterator *llist_it = cetl_llist_iter_cend(queue->data);

  if (!llist_it)
    return NULL;

  state->container = queue;
  state->llist_it = llist_it;

  cetl_const_iterator *queue_it = malloc(sizeof(cetl_const_iterator));

  if (!queue_it)
    return NULL;

  queue_it->category = CETL_CONST_ITERATOR;
  queue_it->state = state;
  queue_it->get = _cetl_queue_iter_cget;
  queue_it->next = _cetl_queue_iter_cnext;
  queue_it->equal = _cetl_queue_iter_cequal;

  return queue_it;
}

cetl_void_t cetl_queue_iter_free(cetl_iterator *it) {

  if (!it)
    return;

  _cetl_queue_iter_state *state = (_cetl_queue_iter_state *)it->state;

  cetl_llist_iter_free(state->llist_it);
  free(state);
  free(it);
}

cetl_void_t cetl_queue_iter_cfree(cetl_const_iterator *it) {

  if (!it)
    return;

  _cetl_queue_iter_state *state = (_cetl_queue_iter_state *)it->state;

  cetl_llist_iter_free(state->llist_it);
  free(state);
  free(it);
}

cetl_void_t cetl_queue_free(cetl_queue *queue) {

  if(queue == NULL){
    return;
  }

  cetl_llist_free(queue->data);
  free(queue);
}
