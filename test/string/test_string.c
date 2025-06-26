#include "../../include/cstl/cstl_string.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void setUp() {}
void tearDown() {}

void test_cstl_str_create_empty() {

  cstl_string *s = cstl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[0]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create() {

  cstl_string *s = cstl_str_create(10);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(10, s->length);
  TEST_ASSERT_EQUAL_size_t(11, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[10]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_len_zero() {

  cstl_string *s = cstl_str_create(0);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[0]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_len_very_large() {

  cstl_string *s = cstl_str_create(INT32_MAX - 1);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(INT32_MAX - 1, s->length);
  TEST_ASSERT_EQUAL_size_t(INT32_MAX, s->capacity);
  TEST_ASSERT_NOT_NULL(s->data);
  TEST_ASSERT_EQUAL('\0', s->data[INT32_MAX - 1]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_filled() {

  cstl_string *s = cstl_str_create_filled(50, 65);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(50, s->length);
  TEST_ASSERT_EQUAL_size_t(51, s->capacity);

  for (size_t i = 0; i < 50; ++i) {
    TEST_ASSERT_EQUAL_UINT8(65, s->data[i]);
  }

  TEST_ASSERT_EQUAL('\0', s->data[50]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_filled_len_zero() {

  cstl_string *s = cstl_str_create_filled(0, 65);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL('\0', s->data[0]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_filled_ASCII_greater_127() {

  cstl_string *s = cstl_str_create_filled(10, 230);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(10, s->length);
  TEST_ASSERT_EQUAL_size_t(11, s->capacity);

  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL_UINT8(230, s->data[i]);
  }

  TEST_ASSERT_EQUAL('\0', s->data[10]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_from_str() {

  const char *cstr = "string";
  size_t cstr_len = strlen(cstr);

  cstl_string *s = cstl_str_create_from_cstr(cstr);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(cstr_len, s->length);
  TEST_ASSERT_EQUAL_size_t(cstr_len + 1, s->capacity);
  TEST_ASSERT_EQUAL_STRING(cstr, s->data);
  TEST_ASSERT_EQUAL('\0', s->data[cstr_len]);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_from_empty_str() {

  cstl_string *s = cstl_str_create_from_cstr("");

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_create_from_null_str() {

  cstl_string *s = cstl_str_create_from_cstr(NULL);

  TEST_ASSERT_NULL(s);
}

void test_cstl_str_create_copy() {

  cstl_string *s_orig = cstl_str_create_from_cstr("string");
  cstl_string *s_copy = cstl_str_create_copy(s_orig);

  TEST_ASSERT_NOT_NULL(s_copy);

  size_t s_len = strlen("string");

  TEST_ASSERT_EQUAL_size_t(s_len, s_copy->length);
  TEST_ASSERT_EQUAL_size_t(s_len + 1, s_copy->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s_copy->data);
  TEST_ASSERT_TRUE(s_orig->data != s_copy->data);

  cstl_str_free(s_orig);
  free(s_orig);

  cstl_str_free(s_copy);
  free(s_copy);
}

void test_cstl_str_create_copy_empty_cstl_string() {

  cstl_string *s_orig = cstl_str_create_from_cstr("");
  cstl_string *s_copy = cstl_str_create_copy(s_orig);

  TEST_ASSERT_NOT_NULL(s_copy);

  size_t s_len = strlen("");

  TEST_ASSERT_EQUAL_size_t(s_len, s_copy->length);
  TEST_ASSERT_EQUAL_size_t(s_len + 1, s_copy->capacity);
  TEST_ASSERT_EQUAL_STRING("", s_copy->data);
  TEST_ASSERT_TRUE(s_orig->data != s_copy->data);

  cstl_str_free(s_orig);
  free(s_orig);

  cstl_str_free(s_copy);
  free(s_copy);
}

void test_cstl_str_create_copy_cstl_string_null() {

  cstl_string *s = cstl_str_create_copy(NULL);

  TEST_ASSERT_NULL(s);
}

void test_cstl_str_resize_positive() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_resize(s, 15);

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(16, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_resize_negative() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_resize(s, 3);

  TEST_ASSERT_EQUAL_size_t(3, s->length);
  TEST_ASSERT_EQUAL_size_t(4, s->capacity);
  TEST_ASSERT_EQUAL_STRING("str", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_resize_new_len_zero() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_resize(s, 0);

  TEST_ASSERT_EQUAL_size_t(0, s->length);
  TEST_ASSERT_EQUAL_size_t(1, s->capacity);
  TEST_ASSERT_EQUAL_STRING("", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_resize_cstl_string_null() {

  cstl_string *s = cstl_str_resize(NULL, 3);

  TEST_ASSERT_NULL(s);
}

void test_cstl_str_append() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_append(s, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string12345", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_append_empty_string() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_append(s, "");

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_append_to_empty_string() {

  cstl_string *s = cstl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_append(s, "string");

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(8, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_append_null() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cstl_str_append(s, NULL));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_append_char() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_append_char(s, 'A');

  TEST_ASSERT_EQUAL_size_t(7, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("stringA", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_append_char_in_empty_str() {

  cstl_string *s = cstl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_append_char(s, 'A');

  TEST_ASSERT_EQUAL_size_t(1, s->length);
  TEST_ASSERT_EQUAL_size_t(2, s->capacity);
  TEST_ASSERT_EQUAL_STRING("A", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_append_char_several_tms_in_row() {

  cstl_string *s = cstl_str_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_append_char(s, 'A');
  cstl_str_append_char(s, 'B');
  cstl_str_append_char(s, 'C');

  TEST_ASSERT_EQUAL_size_t(3, s->length);
  TEST_ASSERT_EQUAL_size_t(4, s->capacity);
  TEST_ASSERT_EQUAL_STRING("ABC", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_insert_range_pos_begin() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_insert_range(s, 0, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("12345string", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_insert_range_pos_end() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_insert_range(s, 6, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string12345", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_insert_range_pos_middle() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_insert_range(s, 2, "12345");

  TEST_ASSERT_EQUAL_size_t(11, s->length);
  TEST_ASSERT_EQUAL_size_t(14, s->capacity);
  TEST_ASSERT_EQUAL_STRING("st12345ring", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_insert_range_empty() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  cstl_str_insert_range(s, 6, "");

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_insert_range_pos_greater_length() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cstl_str_insert_range(s, 20, "12345"));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cstl_str_free(s);
  free(s);
}

void test_cstl_str_insert_range_null() {

  cstl_string *s = cstl_str_create_from_cstr("string");

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cstl_str_insert_range(s, 0, NULL));

  TEST_ASSERT_EQUAL_size_t(6, s->length);
  TEST_ASSERT_EQUAL_size_t(7, s->capacity);
  TEST_ASSERT_EQUAL_STRING("string", s->data);

  cstl_str_free(s);
  free(s);
}

int main() {

  UNITY_BEGIN();

  printf("STR_CREATE\n");

  RUN_TEST(test_cstl_str_create_empty);
  RUN_TEST(test_cstl_str_create);
  RUN_TEST(test_cstl_str_create_len_zero);
  RUN_TEST(test_cstl_str_create_len_very_large);

  printf("\n");

  RUN_TEST(test_cstl_str_create_filled);
  RUN_TEST(test_cstl_str_create_filled_len_zero);
  RUN_TEST(test_cstl_str_create_filled_ASCII_greater_127);

  printf("\n");

  RUN_TEST(test_cstl_str_create_from_str);
  RUN_TEST(test_cstl_str_create_from_empty_str);
  RUN_TEST(test_cstl_str_create_from_null_str);

  printf("\n");

  RUN_TEST(test_cstl_str_create_copy);
  RUN_TEST(test_cstl_str_create_copy_empty_cstl_string);
  RUN_TEST(test_cstl_str_create_copy_cstl_string_null);

  printf("\n");

  RUN_TEST(test_cstl_str_resize_positive);
  RUN_TEST(test_cstl_str_resize_negative);
  RUN_TEST(test_cstl_str_resize_new_len_zero);
  RUN_TEST(test_cstl_str_resize_cstl_string_null);

  printf("\n");

  RUN_TEST(test_cstl_str_append);
  RUN_TEST(test_cstl_str_append_empty_string);
  RUN_TEST(test_cstl_str_append_to_empty_string);
  RUN_TEST(test_cstl_str_append_null);

  printf("\n");

  RUN_TEST(test_cstl_str_append_char);
  RUN_TEST(test_cstl_str_append_char_in_empty_str);
  RUN_TEST(test_cstl_str_append_char_several_tms_in_row);

  printf("\n");

  RUN_TEST(test_cstl_str_insert_range_pos_begin);
  RUN_TEST(test_cstl_str_insert_range_pos_end);
  RUN_TEST(test_cstl_str_insert_range_pos_middle);
  RUN_TEST(test_cstl_str_insert_range_empty);
  RUN_TEST(test_cstl_str_insert_range_pos_greater_length);
  RUN_TEST(test_cstl_str_insert_range_null);

  return UNITY_END();
}
