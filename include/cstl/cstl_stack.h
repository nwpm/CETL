#ifndef CSTL_STACK_H
#define CSTL_STACK_H

#include <stddef.h>
#include <stdbool.h>

typedef struct cstl_type cstl_type;
typedef struct cstl_stack cstl_stack;

cstl_stack *cstl_stack_create_empty(cstl_type *t);
cstl_stack *cstl_stack_create_copy(cstl_stack *s);

cstl_stack *cstl_stack_push(cstl_stack *s, void *data);
cstl_stack *cstl_stack_pop(cstl_stack *s);

bool cstl_stack_is_empty(cstl_stack *s);

void* cstl_stack_top(cstl_stack *s);

void cstl_stack_clear(cstl_stack *s);
void cstl_stack_free(cstl_stack *s);

#endif // CSTL_STACK_H
