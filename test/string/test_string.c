#include "../../src/string/cetl_string_internal.h"
#include "../../include/cetl/cetl_string.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"

#include <stdio.h>
#include <string.h>

void setUp() {}
void tearDown() {}

void test_cetl_str_create_empty() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[0]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create() {

  cetl_string *s = cetl_str_create(10);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(10, s->length);
  TEST_ASSERT_EQUAL_size_t(11, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[10]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_len_zero() {

  cetl_string *s = cetl_str_create(0);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[0]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_len_very_large() {

  cetl_string *s = cetl_str_create(INT32_MAX - 1);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(INT32_MAX - 1, s->length);
  TEST_ASSERT_EQUAL_size_t(INT32_MAX, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[INT32_MAX - 1]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_filled() {

  cetl_string *s = cetl_str_create_filled(50, 65);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(50, s->length);
  TEST_ASSERT_EQUAL_size_t(51, s->capacity);

  for (size_t i = 0; i < 50; ++i) {
    TEST_ASSERT_EQUAL_UINT8(65, s->data[i]);
  }

  TEST_ASSERT_EQUAL('\0', s->data[50]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_filled_len_zero() {

  cetl_string *s = cetl_str_create_filled(0, 65);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL('\0', s->data[0]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_filled_ASCII_greater_127() {

  cetl_string *s = cetl_str_create_filled(10, 230);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(10, s->length);
  TEST_ASSERT_EQUAL_size_t(11, s->capacity);

  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL_UINT8(230, s->data[i]);
  }

  TEST_ASSERT_EQUAL('\0', s->data[10]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_from_str() {

  const char *cstr = "string";
  size_t cstr_len = strlen(cstr);

  cetl_string *s = cetl_str_create_from_cstr(cstr);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(cstr_len, s->length);
  TEST_ASSERT_EQUAL_size_t(cstr_len + 1, s->capacity);
  TEST_ASSERT_EQUAL_STRING(cstr, s->data);
  TEST_ASSERT_EQUAL('\0', s->data[cstr_len]);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_from_empty_str() {

  cetl_string *s = cetl_str_create_from_cstr("");

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_create_from_null_str() {

  cetl_string *s = cetl_str_create_from_cstr(NULL);

  TEST_ASSERT_NULL(s);
}

void test_cetl_str_create_copy() {

  cetl_string *s_orig = cetl_str_create_from_cstr("string");
  cetl_string *s_copy = cetl_str_create_copy(s_orig);

  TEST_ASSERT_NOT_NULL(s_copy);

  size_t s_len = strlen("string");

  TEST_ASSERT_EQUAL_size_t(s_len, s_copy->length);
  TEST_ASSERT_EQUAL_size_t(s_len + 1, s_copy->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s_copy->data);
  TEST_ASSERT_TRUE(s_orig->data != s_copy->data);

  cetl_str_free(s_orig);
  free(s_orig);

  cetl_str_free(s_copy);
  free(s_copy);
}

void test_cetl_str_create_copy_empty_cetl_string() {

  cetl_string *s_orig = cetl_str_create_from_cstr("");
  cetl_string *s_copy = cetl_str_create_copy(s_orig);

  TEST_ASSERT_NOT_NULL(s_copy);

  size_t s_len = strlen("");

  TEST_ASSERT_EQUAL_size_t(s_len, s_copy->length);
  TEST_ASSERT_EQUAL_size_t(s_len + 1, s_copy->capacity);
  TEST_ASSERT_EQUAL_STRING("", s_copy->data);
  TEST_ASSERT_TRUE(s_orig->data != s_copy->data);

  cetl_str_free(s_orig);
  free(s_orig);

  cetl_str_free(s_copy);
  free(s_copy);
}

void test_cetl_str_create_copy_cetl_string_null() {

  cetl_string *s = cetl_str_create_copy(NULL);

  TEST_ASSERT_NULL(s);
}

void test_cetl_str_resize_positive() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_resize(s, 15);

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(16, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_resize_negative() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_resize(s, 3);

  TEST_ASSERT_EQUAL_size_t(3, s->length);
  TEST_ASSERT_EQUAL_size_t(4, s->capacity);
  TEST_ASSERT_EQUAL_STRING("str", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_resize_new_len_zero() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_resize(s, 0);

  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_resize_cetl_string_null() {

  cetl_string *s = cetl_str_resize(NULL, 3);

  TEST_ASSERT_NULL(s);
}

void test_cetl_str_append() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_append(s, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string12345", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_append_empty_string() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_append(s, "");

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_append_to_empty_string() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_append(s, "string");

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(8, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_append_null() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_append(s, NULL));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_append_char() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_append_char(s, 'A');

  TEST_ASSERT_EQUAL_size_t(7, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("stringA", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_append_char_in_empty_str() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_append_char(s, 'A');

  TEST_ASSERT_EQUAL_size_t(1, s->length);
  TEST_ASSERT_EQUAL_size_t(2, s->capacity);
  TEST_ASSERT_EQUAL_STRING("A", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_append_char_several_tms_in_row() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_append_char(s, 'A');
  cetl_str_append_char(s, 'B');
  cetl_str_append_char(s, 'C');

  TEST_ASSERT_EQUAL_size_t(3, s->length);
  TEST_ASSERT_EQUAL_size_t(4, s->capacity);
  TEST_ASSERT_EQUAL_STRING("ABC", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_range_pos_begin() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_insert_range(s, 0, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("12345string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_range_pos_end() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_insert_range(s, 6, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string12345", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_range_pos_middle() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_insert_range(s, 2, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("st12345ring", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_range_empty() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_insert_range(s, 6, "");

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_range_pos_greater_length() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_insert_range(s, 20, "12345"));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_range_null() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_insert_range(s, 0, NULL));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_range_begin() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_erase_range(s, 0, 3));

  TEST_ASSERT_EQUAL_size_t(3, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("ing", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_range_end() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_erase_range(s, 3, 3));

  TEST_ASSERT_EQUAL_size_t(3, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("str", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_range_middle() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_erase_range(s, 1, 4));

  TEST_ASSERT_EQUAL_size_t(2, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("sg", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_range_len_zero() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_erase_range(s, 3, 0));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_range_out_of_bound() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_erase_range(s, 10, 3));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_begin() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_insert(s, 0, 'A'));

  TEST_ASSERT_EQUAL_size_t(7, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("Astring", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_end() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_insert(s, s->length, 'A'));

  TEST_ASSERT_EQUAL_size_t(7, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("stringA", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_middle() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_insert(s, s->length / 2, 'A'));

  TEST_ASSERT_EQUAL_size_t(7, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("strAing", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_insert_out_of_bound() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_insert(s, 100, 'A'));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_begin() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_erase(s, 0));

  TEST_ASSERT_EQUAL_size_t(5, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("tring", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_end() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_erase(s, s->length - 1));

  TEST_ASSERT_EQUAL_size_t(5, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("strin", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_middle() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_erase(s, s->length / 2));

  TEST_ASSERT_EQUAL_size_t(5, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("strng", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_erase_out_of_bound() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_erase(s, 10));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_clear() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_clear(s));

  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_clear_double_clean() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_clear(s));
  TEST_ASSERT_NOT_NULL(cetl_str_clear(s));

  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_replace() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_replace(s, "test", 1));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("stestg", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_replace_pos_equal_length() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_replace(s, "test", s->length));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_replace_pos_out_of_bound() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_replace(s, "test", 150));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_replace_substr_null() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_str_replace(s, NULL, 2));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_shrink_to_fit() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_resize(s, 20);

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(21, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  TEST_ASSERT_NOT_NULL(cetl_str_shrink_to_fit(s));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_shrink_to_fit_double_fit() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_resize(s, 20);

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(21, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  TEST_ASSERT_NOT_NULL(cetl_str_shrink_to_fit(s));
  TEST_ASSERT_NOT_NULL(cetl_str_shrink_to_fit(s));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_pop_back() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_pop_back(s));

  TEST_ASSERT_EQUAL_size_t(5, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("strin", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_pop_back_len_one() {

  cetl_string *s = cetl_str_create_from_cstr("s");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_pop_back(s));

  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(2, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_pop_back_len_zero() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NOT_NULL(cetl_str_pop_back(s));

  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_find() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  const char *ptr = cetl_str_find(s, "tri");

  TEST_ASSERT_NOT_NULL(ptr);
  TEST_ASSERT_EQUAL(s->data + 1, ptr);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_find_not_exist_substr() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  const char *ptr = cetl_str_find(s, "test");

  TEST_ASSERT_NULL(ptr);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_length() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_EQUAL_size_t(6, cetl_str_length(s));

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_length_empty() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_EQUAL_size_t(0, cetl_str_length(s));

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_capacity() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_EQUAL_size_t(7, cetl_str_capacity(s));

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_capacity_empty() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_EQUAL_size_t(1, cetl_str_capacity(s));

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_find_substr_empty() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  const char *ptr = cetl_str_find(s, "");

  TEST_ASSERT_NOT_NULL(ptr);
  TEST_ASSERT_EQUAL_STRING("string", ptr);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_find_substr_null() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  const char *ptr = cetl_str_find(s, NULL);

  TEST_ASSERT_NULL(ptr);

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_is_empty() {

  cetl_string *s = cetl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_EQUAL_size_t(0, cetl_str_is_empty(s));

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_is_empty_len_zero() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_EQUAL_size_t(1, cetl_str_is_empty(s));

  cetl_str_free(s);
  free(s);
}

void test_cetl_str_swap() {

  cetl_string *s1 = cetl_str_create_from_cstr("string");
  cetl_string *s2 = cetl_str_create_from_cstr("test");

  TEST_ASSERT_NOT_NULL(s1);
  TEST_ASSERT_NOT_NULL(s2);

  cetl_str_swap(&s1, &s2);

  TEST_ASSERT_EQUAL_size_t(4, s1->length);
  TEST_ASSERT_EQUAL_size_t(5, s1->capacity);
  TEST_ASSERT_EQUAL_STRING("test", s1->data);

  TEST_ASSERT_EQUAL_size_t(6, s2->length);
  TEST_ASSERT_EQUAL_size_t(7, s2->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s2->data);

  cetl_str_free(s1);
  free(s1);

  cetl_str_free(s2);
  free(s2);
}

void test_cetl_str_swap_with_empty() {

  cetl_string *s1 = cetl_str_create_from_cstr("string");
  cetl_string *s2 = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s1);
  TEST_ASSERT_NOT_NULL(s2);

  cetl_str_swap(&s1, &s2);

  TEST_ASSERT_EQUAL_size_t(0, s1->length);
  TEST_ASSERT_EQUAL_size_t(1, s1->capacity);
  TEST_ASSERT_EQUAL_STRING("", s1->data);

  TEST_ASSERT_EQUAL_size_t(6, s2->length);
  TEST_ASSERT_EQUAL_size_t(7, s2->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s2->data);

  cetl_str_free(s1);
  free(s1);

  cetl_str_free(s2);
  free(s2);
}

void test_cetl_str_swap_one_null() {

  cetl_string *s = cetl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cetl_str_swap(NULL, &s);

  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cetl_str_free(s);
  free(s);

}

int main() {

  UNITY_BEGIN();

  printf("STR_CREATE\n");

  RUN_TEST(test_cetl_str_create_empty);
  RUN_TEST(test_cetl_str_create);
  RUN_TEST(test_cetl_str_create_len_zero);
  RUN_TEST(test_cetl_str_create_len_very_large);

  printf("\n");

  RUN_TEST(test_cetl_str_create_filled);
  RUN_TEST(test_cetl_str_create_filled_len_zero);
  RUN_TEST(test_cetl_str_create_filled_ASCII_greater_127);

  printf("\n");

  RUN_TEST(test_cetl_str_create_from_str);
  RUN_TEST(test_cetl_str_create_from_empty_str);
  RUN_TEST(test_cetl_str_create_from_null_str);

  printf("\n");

  RUN_TEST(test_cetl_str_create_copy);
  RUN_TEST(test_cetl_str_create_copy_empty_cetl_string);
  RUN_TEST(test_cetl_str_create_copy_cetl_string_null);

  printf("\n");

  RUN_TEST(test_cetl_str_resize_positive);
  RUN_TEST(test_cetl_str_resize_negative);
  RUN_TEST(test_cetl_str_resize_new_len_zero);
  RUN_TEST(test_cetl_str_resize_cetl_string_null);

  printf("\n");

  RUN_TEST(test_cetl_str_append);
  RUN_TEST(test_cetl_str_append_empty_string);
  RUN_TEST(test_cetl_str_append_to_empty_string);
  RUN_TEST(test_cetl_str_append_null);

  printf("\n");

  RUN_TEST(test_cetl_str_append_char);
  RUN_TEST(test_cetl_str_append_char_in_empty_str);
  RUN_TEST(test_cetl_str_append_char_several_tms_in_row);

  printf("\n");

  RUN_TEST(test_cetl_str_insert_range_pos_begin);
  RUN_TEST(test_cetl_str_insert_range_pos_end);
  RUN_TEST(test_cetl_str_insert_range_pos_middle);
  RUN_TEST(test_cetl_str_insert_range_empty);
  RUN_TEST(test_cetl_str_insert_range_pos_greater_length);
  RUN_TEST(test_cetl_str_insert_range_null);

  printf("\n");

  RUN_TEST(test_cetl_str_erase_range_begin);
  RUN_TEST(test_cetl_str_erase_range_end);
  RUN_TEST(test_cetl_str_erase_range_middle);
  RUN_TEST(test_cetl_str_erase_range_len_zero);
  RUN_TEST(test_cetl_str_erase_range_out_of_bound);

  printf("\n");

  RUN_TEST(test_cetl_str_insert_begin);
  RUN_TEST(test_cetl_str_insert_end);
  RUN_TEST(test_cetl_str_insert_middle);
  RUN_TEST(test_cetl_str_insert_out_of_bound);

  printf("\n");

  RUN_TEST(test_cetl_str_erase_begin);
  RUN_TEST(test_cetl_str_erase_end);
  RUN_TEST(test_cetl_str_erase_middle);
  RUN_TEST(test_cetl_str_erase_out_of_bound);

  printf("\n");

  RUN_TEST(test_cetl_str_clear);
  RUN_TEST(test_cetl_str_clear_double_clean);

  printf("\n");

  RUN_TEST(test_cetl_str_replace);
  RUN_TEST(test_cetl_str_replace_pos_equal_length);
  RUN_TEST(test_cetl_str_replace_pos_out_of_bound);
  RUN_TEST(test_cetl_str_replace_substr_null);

  printf("\n");

  RUN_TEST(test_cetl_str_shrink_to_fit);
  RUN_TEST(test_cetl_str_shrink_to_fit_double_fit);

  printf("\n");

  RUN_TEST(test_cetl_str_pop_back);
  RUN_TEST(test_cetl_str_pop_back_len_one);
  RUN_TEST(test_cetl_str_pop_back_len_zero);

  printf("\n");

  RUN_TEST(test_cetl_str_find);
  RUN_TEST(test_cetl_str_find_not_exist_substr);
  RUN_TEST(test_cetl_str_find_substr_empty);
  RUN_TEST(test_cetl_str_find_substr_null);

  printf("\n");

  RUN_TEST(test_cetl_str_length);
  RUN_TEST(test_cetl_str_length_empty);

  printf("\n");

  RUN_TEST(test_cetl_str_capacity);
  RUN_TEST(test_cetl_str_capacity_empty);

  printf("\n");

  RUN_TEST(test_cetl_str_is_empty);
  RUN_TEST(test_cetl_str_is_empty_len_zero);

  printf("\n");

  RUN_TEST(test_cetl_str_swap);
  RUN_TEST(test_cetl_str_swap_with_empty);
  RUN_TEST(test_cetl_str_swap_one_null);

  return UNITY_END();
}
