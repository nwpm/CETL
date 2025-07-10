#ifndef CSTL_STRING_H
#define CSTL_STRING_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct cstl_string cstl_string;

cstl_string *cstl_str_create_empty();
cstl_string *cstl_str_create_uninit(size_t len);
cstl_string *cstl_str_create_filled(size_t len, int ch);
cstl_string *cstl_str_create_from_cstr(const char *cstr);
cstl_string *cstl_str_create_copy(const cstl_string *src_string);

cstl_string *cstl_str_resize(cstl_string *string, size_t new_len);
cstl_string *cstl_str_append_cstr(cstl_string *string, const char *cstr);
cstl_string *cstl_str_append_char(cstl_string *string, int ch);
cstl_string *cstl_str_insert_char(cstl_string *string, size_t pos, int ch);
cstl_string *cstl_str_insert_cstr(cstl_string *string, size_t pos,
                                 const char *cstr);
cstl_string *cstl_str_erase_char(cstl_string *string, size_t pos);
cstl_string *cstl_str_erase_span(cstl_string *string, size_t pos, size_t len);
cstl_string *cstl_str_clear(cstl_string *string);
cstl_string *cstl_str_replace(cstl_string *string, size_t pos, size_t len,
                              const char *new_substr);
cstl_string *cstl_str_shrink_to_fit(cstl_string *string);
cstl_string *cstl_str_pop_back(cstl_string *string);

const char *cstl_str_find(const cstl_string *string, const char *substr);
char *cstl_str_data(const cstl_string *string);

int cstl_str_compare(const cstl_string *stirng, const char *substr);

size_t cstl_str_length(const cstl_string *string);
size_t cstl_str_capacity(const cstl_string *string);

bool cstl_str_is_empty(const cstl_string *string);

void cstl_str_swap(cstl_string **string1, cstl_string **string2);
void cstl_str_free(cstl_string *string);

#endif // CSTL_STRING_H
