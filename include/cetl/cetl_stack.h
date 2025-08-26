#ifndef CETL_STACK_H
#define CETL_STACK_H

#include "../../src/utils/types/cetl_types.h"

// Create
cetl_stack *cetl_stack_create_empty(const struct cetl_element *type);
cetl_stack *cetl_stack_create_copy(const cetl_stack *src_stack);

// Operations
cetl_stack *cetl_stack_push(cetl_stack *stack, const cetl_ptr_t data);
cetl_stack *cetl_stack_pop(cetl_stack *stack);
cetl_ptr_t cetl_stack_top(const cetl_stack *stack);
cetl_void_t cetl_stack_clear(cetl_stack *stack);

// Size
cetl_bool_t cetl_stack_is_empty(const cetl_stack *stack);

// Iterator
cetl_iterator* cetl_stack_iter_begin(const cetl_stack* stack);
cetl_iterator* cetl_stack_iter_end(const cetl_stack* stack);

cetl_const_iterator* cetl_stack_iter_cbegin(const cetl_stack* stack);
cetl_const_iterator* cetl_stack_iter_cend(const cetl_stack* stack);

// Free
cetl_void_t cetl_stack_free(cetl_stack *stack);
cetl_void_t cetl_stack_iter_free(cetl_iterator *it);
cetl_void_t cetl_stack_iter_cfree(cetl_iterator *it);

#endif // CETL_STACK_H
