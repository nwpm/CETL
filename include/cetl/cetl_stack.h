#ifndef CETL_STACK_H
#define CETL_STACK_H

#include "../../src/prefix/cetl_ptypes.h"
#include <stddef.h>

struct cetl_type;
typedef struct cetl_stack cetl_stack;

cetl_stack *cetl_stack_create_empty(const struct cetl_type *type);
cetl_stack *cetl_stack_create_copy(const cetl_stack *src_stack);

cetl_stack *cetl_stack_push(cetl_stack *stack, const cetl_ptr_t data);
cetl_stack *cetl_stack_pop(cetl_stack *stack);

cetl_bool_t cetl_stack_is_empty(const cetl_stack *stack);

cetl_ptr_t cetl_stack_top(const cetl_stack *stack);

cetl_void_t cetl_stack_clear(cetl_stack *stack);
cetl_void_t cetl_stack_free(cetl_stack *stack);

#endif // CETL_STACK_H
