#include "../../include/cstl/cstl_queue.h"
#include <stddef.h>

cstl_queue *cstl_queue_create_empty() {

  cstl_queue *q = malloc(sizeof(cstl_queue));

  if (q == NULL) {
    return NULL;
  }

  cstl_llist *llist = cstl_llist_create_empty();

  if (llist == NULL) {
    return NULL;
  }

  q->size = 0;
  q->data = llist;

  return q;
}

cstl_queue *cstl_queue_create(void *data, size_t elem_size) {

  if (data == NULL || elem_size == 0) {
    return NULL;
  }

  cstl_queue *q = cstl_queue_create_empty();

  if (q == NULL) {
    return NULL;
  }

  cstl_llist_push_back(q->data, data, elem_size);

  q->size = 1;

  return q;
}

cstl_queue *cstl_queue_create_copy(cstl_queue *q) {

  if (q == NULL) {
    return NULL;
  }

  cstl_queue *c_q = cstl_queue_create_empty();

  if (c_q == NULL) {
    return NULL;
  }

  _cstl_node *current = q->data->head;

  while (current) {
    cstl_queue *res = cstl_queue_push(c_q, current->data, q->data->elem_size);
    current = current->next;

    if (res == NULL) {
      cstl_queue_free(c_q);
      return NULL;
    }
  }

  return c_q;
}

void *cstl_queue_front(cstl_queue *q) {

  if (q == NULL || q->size == 0) {
    return NULL;
  }

  return cstl_llist_get(q->data, 0);
}

void *cstl_queue_back(cstl_queue *q) {

  if (q == NULL || q->size == 0) {
    return NULL;
  }

  return cstl_llist_get(q->data, q->size - 1);
}

cstl_queue *cstl_queue_push(cstl_queue *q, void *data, size_t elem_size) {

  if (q == NULL || data == NULL || elem_size == 0) {
    return NULL;
  }

  cstl_llist_push_back(q->data, data, elem_size);
  q->size++;

  return q;
}

cstl_queue *cstl_queue_pop(cstl_queue *q) {

  if (q == NULL) {
    return NULL;
  }

  cstl_llist_pop_front(q->data);
  q->size--;

  return q;
}

size_t cstl_queue_size(cstl_queue *q) { return q->size; }

bool cstl_queue_is_empty(cstl_queue *q) { return q && !q->size; }

void cstl_queue_free(cstl_queue *q) {

  cstl_llist_free_nodes(q->data);
  free(q->data);
  free(q);
}
