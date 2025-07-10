#include "../../include/cstl/cstl_llist.h"
#include "../../include/cstl/cstl_queue.h"
#include "cstl_queue_internal.h"
#include <stdlib.h>


typedef struct cstl_llist cstl_llist;

cstl_queue *cstl_queue_create_empty(const cstl_type *type) {

  if (type == NULL) {
    return NULL;
  }

  cstl_queue *queue = malloc(sizeof(cstl_queue));

  if (queue == NULL) {
    return NULL;
  }

  cstl_llist *llist = cstl_llist_create_empty(type);

  if (llist == NULL) {
    free(queue);
    return NULL;
  }

  queue->size = 0;
  queue->data = llist;

  return queue;
}

cstl_queue *cstl_queue_create_copy(const cstl_queue *src_queue) {

  if (src_queue == NULL) {
    return NULL;
  }

  cstl_queue *new_queue = cstl_queue_create_empty(src_queue->type);

  if (new_queue == NULL || cstl_queue_is_empty(src_queue)) {
    return new_queue;
  }

  cstl_llist *llist_copy = cstl_llist_create_copy(src_queue->data);

  if (llist_copy == NULL) {
    cstl_queue_free(new_queue);
    return NULL;
  }

  new_queue->size = src_queue->size;
  new_queue->data = llist_copy;

  return new_queue;
}

void *cstl_queue_front(const cstl_queue *queue) {
  return cstl_llist_get(queue->data, 0);
}

void *cstl_queue_back(const cstl_queue *queue) {
  return cstl_llist_get(queue->data, queue->size - 1);
}

cstl_queue *cstl_queue_push(cstl_queue *queue, const void *data) {

  if(cstl_llist_push_back(queue->data, data) == NULL){
    return NULL;
  }
  
  queue->size++;

  return queue;
}

cstl_queue *cstl_queue_pop(cstl_queue *queue) {

  if (cstl_llist_pop_front(queue->data) == NULL) {
    return NULL;
  }

  queue->size--;

  return queue;
}

size_t cstl_queue_size(const cstl_queue *queue) { return queue->size; }

bool cstl_queue_is_empty(const cstl_queue *queue) { return queue && !queue->size; }

void cstl_queue_swap(cstl_queue **queue1, cstl_queue **queue2){

  if(queue1 == NULL || queue2 == NULL){
    return;
  }

  cstl_queue *tmp = *queue1;
  *queue1 = *queue2;
  *queue2 = tmp;

}

void cstl_queue_free(cstl_queue *queue) {

  if(queue == NULL){
    return;
  }

  cstl_llist_free(queue->data);
  free(queue);
}
