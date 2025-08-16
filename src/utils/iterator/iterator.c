#include "iterator.h"
#include <stdlib.h>

cetl_iterator *create_iterator(void *container, size_t size, size_t elem_size,
                               const cetl_element *elem_type) {

  cetl_iterator *iter = malloc(sizeof(cetl_iterator));

  iter->category = CETL_FORWARD_ITERATOR;
  iter->data = container;
  iter->elem_type = elem_type;
  iter->size = size;
  iter->index = 0;

  if (!iter)
    return NULL;

  return iter;
}
