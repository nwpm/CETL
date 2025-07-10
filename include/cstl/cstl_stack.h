#ifndef CSTL_STACK_H
#define CSTL_STACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct cstl_type cstl_type;
typedef struct cstl_stack cstl_stack;

cstl_stack *cstl_stack_create_empty(const cstl_type *type);
cstl_stack *cstl_stack_create_copy(const cstl_stack *src_stack);

cstl_stack *cstl_stack_push(cstl_stack *stack, const void *data);
cstl_stack *cstl_stack_pop(cstl_stack *stack);

bool cstl_stack_is_empty(const cstl_stack *stack);

void* cstl_stack_top(const cstl_stack *stack);

void cstl_stack_clear(cstl_stack *stack);
void cstl_stack_free(cstl_stack *stack);

#endif // CSTL_STACK_H
