#ifndef CETL_STRING_INTERNAL_H
#define CETL_STRING_INTERNAL_H

#include <stddef.h>

#define CETL_STR_GROW_RATE 2

struct cetl_string {

  char *data;
  size_t length;
  size_t capacity;

};

#endif // CETL_STRING_INTERNAL_H

