#ifndef QUEUE_H
#define QUEUE_H

#include "../list/linked_list.h"
#include <stdint.h>

typedef struct queue {

  size_t size;
  list *data_ptr;

} queue;

queue *queue_create(void);

int32_t queue_front(queue *q);
int32_t queue_back(queue *q);

queue *queue_push(queue *q, int32_t value);
queue *queue_pop(queue *q, int32_t value);

void queue_free(queue *q);

#endif // QUEUE_H
