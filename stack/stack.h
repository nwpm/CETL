#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stdlib.h>

#include "../list/linked_list.h"

typedef struct stack {

  size_t size;
  list *data_ptr;

} stack;

stack *stack_create(void);
stack *stack_create_init(int32_t value);

stack *stack_push(stack *s, int32_t value);
stack *stack_pop(stack *s);
int32_t stack_top(stack *s);

void stack_print_data(stack *s);
void stack_free(stack *s);

#endif // STACK_H
