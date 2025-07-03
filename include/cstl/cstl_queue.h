#ifndef CSTL_QUEUE_H
#define CSTL_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

struct cstl_queue;
typedef struct cstl_queue cstl_queue;

cstl_queue *cstl_queue_create_empty();
cstl_queue *cstl_queue_create(void *data, size_t elem_size);
cstl_queue *cstl_queue_create_copy(cstl_queue *q);

cstl_queue *cstl_queue_push(cstl_queue *q, void *data, size_t elem_size);
cstl_queue *cstl_queue_pop(cstl_queue *q);

size_t cstl_queue_size(cstl_queue *q);

bool cstl_queue_is_empty(cstl_queue *q);

void *cstl_queue_front(cstl_queue *q);
void *cstl_queue_back(cstl_queue *q);

// cstl_queue_swap
void cstl_queue_free(cstl_queue *q);

#endif // CSTL_QUEUE_H
