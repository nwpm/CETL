#ifndef CSTL_STRING_INTERNAL_H
#define CSTL_STRING_INTERNAL_H

#include <stddef.h>

struct cstl_string {

  char *data;
  size_t length;
  size_t capacity;

};

#endif // CSTL_STRING_INTERNAL_H

