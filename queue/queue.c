#include "queue.h"
#include <stdint.h>

queue* queue_create(void){

  queue* q = (queue*)malloc(sizeof(queue));
  list* l = (list*)malloc(sizeof(list));

  list_create(l);

  q->data_ptr = l;
  q->size = l->size;

  return q;

}

int32_t queue_front(queue *q){
  
  if(q->size == 0){
    return -1; //???
  }

  return q->data_ptr->head->data;
}

int32_t queue_back(queue *q){

  if(q->size == 0){
    return -1;
  }

  return q->data_ptr->tail->data;
}

queue *queue_push(queue *q, int32_t value){
  list_push_back(q->data_ptr, value);
  return q;
}

queue *queue_pop(queue *q, int32_t value){
  list_pop_back(q->data_ptr);
  return q;
}

void queue_free(queue *q){

  list_free_nodes(q->data_ptr);
  free(q->data_ptr);
  free(q);

}
