#ifndef CSTL_STRING_H
#define CSTL_STRING_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct cstl_string {

  char *data;
  size_t length;
  size_t capacity;

} cstl_string;

cstl_string *cstl_str_create_empty();
cstl_string *cstl_str_create(size_t len);
cstl_string *cstl_str_create_filled(size_t len, int c);
cstl_string *cstl_str_create_from_cstr(const char *str);
cstl_string *cstl_str_create_copy(cstl_string *s);

cstl_string *cstl_str_resize(cstl_string *s, size_t new_len);
cstl_string *cstl_str_append(cstl_string *s, const char *c_s);
cstl_string *cstl_str_append_char(cstl_string *s, int c);
cstl_string *cstl_str_insert(cstl_string *s, size_t pos, int c);
cstl_string *cstl_str_insert_range(cstl_string *s, size_t pos, const char *c_s);
cstl_string *cstl_str_erase(cstl_string *s, size_t pos);
cstl_string *cstl_str_erase_range(cstl_string *s, size_t pos, size_t len);
cstl_string *cstl_str_clear(cstl_string *s);
cstl_string *cstl_str_replace(cstl_string *s, const char* substr, size_t pos);
cstl_string *cstl_str_shrink_to_fit(cstl_string *s);
cstl_string *cstl_str_pop_back(cstl_string *s);

const char *cstl_str_find(cstl_string *s, const char *substr);
char *cstl_str_data(cstl_string *s);

int cstl_str_compare(cstl_string *s, const char *substr);

size_t cstl_str_length(cstl_string *s);
size_t cstl_str_capacity(cstl_string *s);

bool cstl_str_is_empty(cstl_string *s);

void cstl_str_swap(cstl_string **s1, cstl_string **s2);
void cstl_str_free(cstl_string *s);

#endif // CSTL_STRING_H
