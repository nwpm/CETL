#ifndef CETL_STRING_INTERNAL_H
#define CETL_STRING_INTERNAL_H

#include "../../prefix/cetl_ptypes.h"
#include <stddef.h>

#define CETL_STR_GROW_RATE 2

struct cetl_string {

  cetl_str_t data;
  cetl_size_t length;
  cetl_size_t capacity;

};

#endif // CETL_STRING_INTERNAL_H

