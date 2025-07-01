#ifndef CSTL_cstl_stack_H
#define CSTL_cstl_stack_H

#include "./cstl_llist.h"

#include <stdlib.h>
#include <stdbool.h>

typedef struct cstl_stack {

  size_t size;
  cstl_llist *data;

} cstl_stack;

cstl_stack *cstl_stack_create_empty();
cstl_stack *cstl_stack_create(void *data, size_t elem_size);
cstl_stack *cstl_stack_create_copy(cstl_stack *s);

cstl_stack *cstl_stack_push(cstl_stack *s, void *data, size_t elem_size);
cstl_stack *cstl_stack_pop(cstl_stack *s);

bool cstl_stack_is_empty(cstl_stack *s);

void* cstl_stack_top(cstl_stack *s);

void cstl_stack_clear(cstl_stack *s);
void cstl_stack_free(cstl_stack *s);

#endif // CSTL_cstl_stack_H
