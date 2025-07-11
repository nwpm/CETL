#include "../../../include/cetl/cetl_llist.h"
#include "../../../include/cetl/cetl_queue.h"
#include "cetl_queue_internal.h"
#include <stdlib.h>

cetl_queue *cetl_queue_create_empty(const cetl_type *type) {

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

void *cetl_queue_front(const cetl_queue *queue) {
  return cetl_llist_get(queue->data, 0);
}

void *cetl_queue_back(const cetl_queue *queue) {
  return cetl_llist_get(queue->data, queue->size - 1);
}

cetl_queue *cetl_queue_push(cetl_queue *queue, const void *data) {

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

size_t cetl_queue_size(const cetl_queue *queue) { return queue->size; }

bool cetl_queue_is_empty(const cetl_queue *queue) { return queue && !queue->size; }

void cetl_queue_swap(cetl_queue **queue1, cetl_queue **queue2){

  if(queue1 == NULL || queue2 == NULL){
    return;
  }

  cetl_queue *tmp = *queue1;
  *queue1 = *queue2;
  *queue2 = tmp;

}

void cetl_queue_free(cetl_queue *queue) {

  if(queue == NULL){
    return;
  }

  cetl_llist_free(queue->data);
  free(queue);
}
