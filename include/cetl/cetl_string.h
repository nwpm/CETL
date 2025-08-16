#ifndef CETL_STRING_H
#define CETL_STRING_H

#include "../../src/utils/types/cetl_types.h"

cetl_string *cetl_str_create_empty();
cetl_string *cetl_str_create_uninit(cetl_size_t len);
cetl_string *cetl_str_create_filled(cetl_size_t len, cetl_uchar_t ch);
cetl_string *cetl_str_create_from_cstr(cetl_cstr_t cstr);
cetl_string *cetl_str_create_copy(const cetl_string *src_string);

cetl_string *cetl_str_resize(cetl_string *string, cetl_size_t new_len);
cetl_string *cetl_str_append_cstr(cetl_string *string, cetl_cstr_t cstr);
cetl_string *cetl_str_append_char(cetl_string *string, cetl_uchar_t ch);
cetl_string *cetl_str_insert_char(cetl_string *string, cetl_size_t pos, cetl_uchar_t ch);
cetl_string *cetl_str_insert_cstr(cetl_string *string, cetl_size_t pos,
                                 cetl_cstr_t cstr);
cetl_string *cetl_str_erase_char(cetl_string *string, cetl_size_t pos);
cetl_string *cetl_str_erase_span(cetl_string *string, cetl_size_t pos, cetl_size_t len);
cetl_string *cetl_str_clear(cetl_string *string);
cetl_string *cetl_str_replace(cetl_string *string, cetl_size_t pos, cetl_size_t len,
                              cetl_cstr_t new_substr);
cetl_string *cetl_str_shrink_to_fit(cetl_string *string);
cetl_string *cetl_str_pop_back(cetl_string *string);

cetl_str_t cetl_str_find(const cetl_string *string, cetl_cstr_t substr);
cetl_str_t cetl_str_data(const cetl_string *string);

cetl_result_t cetl_str_compare(const cetl_string *stirng, cetl_cstr_t substr);

cetl_size_t cetl_str_length(const cetl_string *string);
cetl_size_t cetl_str_capacity(const cetl_string *string);

cetl_bool_t cetl_str_is_empty(const cetl_string *string);

cetl_void_t cetl_str_swap(cetl_string **string1, cetl_string **string2);
cetl_void_t cetl_str_free(cetl_string *string);

#endif // CETL_STRING_H
