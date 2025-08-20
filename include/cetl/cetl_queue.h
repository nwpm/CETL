#ifndef CETL_QUEUE_H
#define CETL_QUEUE_H

#include "../../src/utils/types/cetl_types.h"

// Create
cetl_queue *cetl_queue_create_empty(const struct cetl_element *type);
cetl_queue *cetl_queue_create_copy(const cetl_queue *queue);

// Operatons
cetl_queue *cetl_queue_push(cetl_queue *queue, const cetl_ptr_t data);
cetl_queue *cetl_queue_pop(cetl_queue *queue);
cetl_ptr_t cetl_queue_front(const cetl_queue *queue);
cetl_ptr_t cetl_queue_back(const cetl_queue *queue);
cetl_void_t cetl_queue_swap(cetl_queue **queue1, cetl_queue **queue2);

// Size
cetl_size_t cetl_queue_size(const cetl_queue *queue);
cetl_bool_t cetl_queue_is_empty(const cetl_queue *queue);

// Iterators
cetl_iterator *cetl_queue_iter_begin(const cetl_queue* queue);
cetl_iterator *cetl_queue_iter_end(const cetl_queue* queue);
cetl_void_t cetl_queue_iter_free(cetl_iterator *it);

// Free
cetl_void_t cetl_queue_free(cetl_queue *queue);

#endif // CETL_QUEUE_H
