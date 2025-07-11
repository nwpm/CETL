#ifndef CETL_STRING_H
#define CETL_STRING_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct cetl_string cetl_string;

cetl_string *cetl_str_create_empty();
cetl_string *cetl_str_create_uninit(size_t len);
cetl_string *cetl_str_create_filled(size_t len, int ch);
cetl_string *cetl_str_create_from_cstr(const char *cstr);
cetl_string *cetl_str_create_copy(const cetl_string *src_string);

cetl_string *cetl_str_resize(cetl_string *string, size_t new_len);
cetl_string *cetl_str_append_cstr(cetl_string *string, const char *cstr);
cetl_string *cetl_str_append_char(cetl_string *string, int ch);
cetl_string *cetl_str_insert_char(cetl_string *string, size_t pos, int ch);
cetl_string *cetl_str_insert_cstr(cetl_string *string, size_t pos,
                                 const char *cstr);
cetl_string *cetl_str_erase_char(cetl_string *string, size_t pos);
cetl_string *cetl_str_erase_span(cetl_string *string, size_t pos, size_t len);
cetl_string *cetl_str_clear(cetl_string *string);
cetl_string *cetl_str_replace(cetl_string *string, size_t pos, size_t len,
                              const char *new_substr);
cetl_string *cetl_str_shrink_to_fit(cetl_string *string);
cetl_string *cetl_str_pop_back(cetl_string *string);

const char *cetl_str_find(const cetl_string *string, const char *substr);
char *cetl_str_data(const cetl_string *string);

int cetl_str_compare(const cetl_string *stirng, const char *substr);

size_t cetl_str_length(const cetl_string *string);
size_t cetl_str_capacity(const cetl_string *string);

bool cetl_str_is_empty(const cetl_string *string);

void cetl_str_swap(cetl_string **string1, cetl_string **string2);
void cetl_str_free(cetl_string *string);

#endif // CETL_STRING_H
