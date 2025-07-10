#ifndef CSTL_QUEUE_H
#define CSTL_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct cstl_type cstl_type;
typedef struct cstl_queue cstl_queue;

cstl_queue *cstl_queue_create_empty(const cstl_type *type);
cstl_queue *cstl_queue_create_copy(const cstl_queue *queue);

cstl_queue *cstl_queue_push(cstl_queue *queue, const void *data);
cstl_queue *cstl_queue_pop(cstl_queue *queue);

size_t cstl_queue_size(const cstl_queue *queue);

bool cstl_queue_is_empty(const cstl_queue *queue);

void *cstl_queue_front(const cstl_queue *queue);
void *cstl_queue_back(const cstl_queue *queue);

void cstl_queue_swap(cstl_queue **queue1, cstl_queue **queue2);
void cstl_queue_free(cstl_queue *queue);

#endif // CSTL_QUEUE_H
