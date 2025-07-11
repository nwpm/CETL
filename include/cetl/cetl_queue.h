#ifndef CETL_QUEUE_H
#define CETL_QUEUE_H

#include <stdbool.h>
#include <stddef.h>

struct cetl_type;
typedef struct cetl_queue cetl_queue;

cetl_queue *cetl_queue_create_empty(const struct cetl_type *type);
cetl_queue *cetl_queue_create_copy(const cetl_queue *queue);

cetl_queue *cetl_queue_push(cetl_queue *queue, const void *data);
cetl_queue *cetl_queue_pop(cetl_queue *queue);

size_t cetl_queue_size(const cetl_queue *queue);

bool cetl_queue_is_empty(const cetl_queue *queue);

void *cetl_queue_front(const cetl_queue *queue);
void *cetl_queue_back(const cetl_queue *queue);

void cetl_queue_swap(cetl_queue **queue1, cetl_queue **queue2);
void cetl_queue_free(cetl_queue *queue);

#endif // CETL_QUEUE_H
