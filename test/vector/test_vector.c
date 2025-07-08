#include "../../include/cstl/cstl_vector.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include "../../src/vector/cstl_vector_internal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

cstl_type *create_int_type() {

  cstl_type *int_type = malloc(sizeof(cstl_type));

  if (int_type == NULL) {
    return NULL;
  }

  int_type->size = sizeof(int);
  int_type->ctor = NULL;
  int_type->dtor = NULL;
  int_type->cmp = NULL;

  return int_type;
}

void vec_fill_with_int(cstl_vector *vec, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    cstl_vec_push_back(vec, &i);
  }
}

cstl_type *create_double_type() {

  cstl_type *double_type = malloc(sizeof(cstl_type));

  if (double_type == NULL) {
    return NULL;
  }

  double_type->size = sizeof(double);
  double_type->ctor = NULL;
  double_type->dtor = NULL;
  double_type->cmp = NULL;

  return double_type;
}

typedef struct TestHeapStr {
  char *s;
  int x;
  float y;
} TestHeapStr;

void *test_heap_str_ctor(void *dest, const void *data) {
  TestHeapStr *target = (TestHeapStr *)dest;
  TestHeapStr *source = (TestHeapStr *)data;
  target->x = source->x;
  target->y = source->y;
  target->s = strdup(source->s);
  return target;
}

void test_heap_str_dtor(void *data) {
  TestHeapStr *target = (TestHeapStr *)data;
  free(target->s);
}

cstl_type *create_test_heap_str_type() {

  cstl_type *test_heap_str_type = malloc(sizeof(cstl_type));

  if (test_heap_str_type == NULL) {
    return NULL;
  }

  test_heap_str_type->size = sizeof(TestHeapStr);
  test_heap_str_type->ctor = test_heap_str_ctor;
  test_heap_str_type->dtor = test_heap_str_dtor;
  test_heap_str_type->cmp = NULL;

  return test_heap_str_type;
}

TestHeapStr *make_test_heap_str(int x, float y, char *s) {

  TestHeapStr *test_object = malloc(sizeof(TestHeapStr));

  if (test_object == NULL) {
    return NULL;
  }

  test_object->x = x;
  test_object->y = y;
  test_object->s = s;

  return test_object;
}

char *create_string_of_char(size_t len, int ch) {

  char *str = malloc(len + 1);

  if (str == NULL) {
    return NULL;
  }

  str[len] = '\0';
  memset(str, ch, len);

  return str;
}

void vec_fill_with_test_heap_str(cstl_vector *v, size_t size, size_t s_len) {

  for (size_t i = 0; i < size; ++i) {

    char *str = create_string_of_char(s_len, 'A');
    TestHeapStr *test_heap_str = make_test_heap_str(i, i, str);
    cstl_vec_push_back(v, test_heap_str);
    free(test_heap_str->s);
    free(test_heap_str);
  }
}

typedef struct OwnedString {

  char *data;
  size_t len;

} OwnedString;

void *simple_string_ctor(void *dest, const void *data) {
  OwnedString *target = (OwnedString *)dest;
  OwnedString *source = (OwnedString *)data;
  target->data = strdup(source->data);
  target->len = source->len;
  return target;
}

void simple_string_dtor(void *data) {
  OwnedString *target = (OwnedString *)data;
  free(target->data);
}

cstl_type *create_owned_string_type() {

  cstl_type *t = malloc(sizeof(cstl_type));

  if (t == NULL) {
    return NULL;
  }

  t->size = sizeof(OwnedString);
  t->ctor = simple_string_ctor;
  t->dtor = simple_string_dtor;
  t->cmp = NULL;

  return t;
}

OwnedString *make_owned_string(char *s, size_t len) {

  OwnedString *owned_string = malloc(len + 1);

  if (s == NULL) {
    return NULL;
  }

  owned_string->data = s;
  owned_string->len = len;

  return owned_string;
}

void vec_fill_with_owned_string(cstl_vector *v, size_t num, size_t s_len, int ch) {

  for (size_t i = 0; i < num; ++i) {
    char *s = create_string_of_char(s_len, ch);
    OwnedString *owned_string = make_owned_string(s, s_len);
    cstl_vec_push_back(v, owned_string);
    free(s);
    free(owned_string);
  }
}

void setUp() {}
void tearDown() {}

//-------------------------------------

void test_create_empty_type_int() {

  cstl_type *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CSTL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cstl_vec_free(v);
  free(t);
}

void test_create_empty_type_owned_string() {

  cstl_type *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CSTL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cstl_vec_free(v);
  free(t);
}

void test_create_empty_type_test_heap_str() {

  cstl_type *t = create_test_heap_str_type();

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CSTL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cstl_vec_free(v);
  free(t);
}

//-----------------------------

void test_create_copy_from_size_0_type_int() {

  cstl_type *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_NOT_NULL(c_v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);

  cstl_vec_free(c_v);
  cstl_vec_free(v);
  free(t);
}

void test_create_copy_from_size_0_type_owned_string() {

  cstl_type *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_NOT_NULL(c_v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);

  cstl_vec_free(c_v);
  cstl_vec_free(v);
  free(t);
}

void test_create_copy_from_size_10_type_int() {

  cstl_type *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 10);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 10; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);
    TEST_ASSERT_EQUAL_INT(*((int *)current_orig), *((int *)current_copy));
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

void test_create_copy_from_size_1000_type_int() {

  cstl_type *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 1000);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 1000; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);
    TEST_ASSERT_EQUAL_INT(*((int *)current_orig), *((int *)current_copy));
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

void test_create_copy_from_size_10_type_test_heap_str() {

  cstl_type *t = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_test_heap_str(v, 10, 15);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 10; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);

    TEST_ASSERT_EQUAL_INT(((TestHeapStr *)current_orig)->x,
                          ((TestHeapStr *)current_copy)->x);

    TEST_ASSERT_EQUAL_FLOAT(((TestHeapStr *)current_orig)->y,
                            ((TestHeapStr *)current_orig)->y);

    TEST_ASSERT_EQUAL_STRING(((TestHeapStr *)current_orig)->s,
                             ((TestHeapStr *)current_orig)->s);
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

void test_create_copy_from_size_1000_type_test_heap_str() {

  cstl_type *t = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_test_heap_str(v, 1000, 50);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 1000; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);

    TEST_ASSERT_EQUAL_INT(((TestHeapStr *)current_orig)->x,
                          ((TestHeapStr *)current_copy)->x);

    TEST_ASSERT_EQUAL_FLOAT(((TestHeapStr *)current_orig)->y,
                            ((TestHeapStr *)current_orig)->y);

    TEST_ASSERT_EQUAL_STRING(((TestHeapStr *)current_orig)->s,
                             ((TestHeapStr *)current_orig)->s);
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

void test_push_back_to_size_1_type_int() {

  cstl_type *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  int data = 10;

  TEST_ASSERT_NOT_NULL(cstl_vec_push_back(v, &data));

  TEST_ASSERT_EQUAL_size_t(1, v->size);
  TEST_ASSERT_EQUAL_size_t(CSTL_VEC_START_CAPACITY, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  TEST_ASSERT_EQUAL_INT(10, *((int *)cstl_vec_get(v, 0)));

  cstl_vec_free(v);
  free(t);
}

void test_push_back_to_size_10_type_int() {

  cstl_type *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 10);

  TEST_ASSERT_EQUAL_size_t(10, v->size);
  TEST_ASSERT_EQUAL_size_t(CSTL_VEC_START_CAPACITY * 2, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(t);
}

void test_push_back_to_size_1000_type_int() {

  cstl_type *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 1000);

  TEST_ASSERT_EQUAL_size_t(1000, v->size);
  TEST_ASSERT_EQUAL_size_t(1024, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  for (size_t i = 0; i < 1000; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(t);
}

void test_push_back_to_size_1_type_owned_string() {

  cstl_type *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  char char_for_string = 'A';
  size_t char_array_size = 20;

  vec_fill_with_owned_string(v, 1, char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(1, v->size);
  TEST_ASSERT_EQUAL_size_t(CSTL_VEC_START_CAPACITY, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  char *s_for_check = create_string_of_char(char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(char_array_size,
                           ((OwnedString *)cstl_vec_get(v, 0))->len);
  TEST_ASSERT_EQUAL_STRING(s_for_check,
                           ((OwnedString *)cstl_vec_get(v, 0))->data);

  cstl_vec_free(v);
  free(t);
  free(s_for_check);
}

void test_push_back_to_size_10_type_owned_string() {

  cstl_type *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  char char_for_string = 'A';
  size_t char_array_size = 50;
  size_t num_elements = 10;

  vec_fill_with_owned_string(v, num_elements, char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(num_elements, v->size);
  TEST_ASSERT_EQUAL_size_t(CSTL_VEC_START_CAPACITY * 2, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  char *s_for_check = create_string_of_char(char_array_size, char_for_string);

  for (size_t i = 0; i < num_elements; ++i) {
    TEST_ASSERT_EQUAL_size_t(char_array_size,
                             ((OwnedString *)cstl_vec_get(v, 0))->len);
    TEST_ASSERT_EQUAL_STRING(s_for_check,
                             ((OwnedString *)cstl_vec_get(v, 0))->data);
  }

  cstl_vec_free(v);
  free(t);
  free(s_for_check);
}

void test_push_back_to_size_1000_type_owned_string() {

  cstl_type *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  char char_for_string = 'A';
  size_t char_array_size = 80;

  vec_fill_with_owned_string(v, 1000, char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(1000, v->size);
  TEST_ASSERT_EQUAL_size_t(1024, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  char *s_for_check = create_string_of_char(char_array_size, char_for_string);

  for (size_t i = 0; i < 1000; ++i) {
    TEST_ASSERT_EQUAL_size_t(char_array_size,
                             ((OwnedString *)cstl_vec_get(v, 0))->len);
    TEST_ASSERT_EQUAL_STRING(s_for_check,
                             ((OwnedString *)cstl_vec_get(v, 0))->data);
  }

  cstl_vec_free(v);
  free(t);
  free(s_for_check);
}

void test_push_back_to_size_1_type_test_heap_str() {

  cstl_type *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cstl_vector *vec = cstl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  size_t num_test_heap_str_objects = 1;
  size_t str_len = 10;

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len);

  TEST_ASSERT_EQUAL_size_t(1, vec->size);
  TEST_ASSERT_EQUAL_size_t(CSTL_VEC_START_CAPACITY, vec->capacity);
  TEST_ASSERT_EQUAL(test_heap_str_type, vec->type);

  char *expected_string = create_string_of_char(str_len, 'A');
  TestHeapStr *expected_test_heap_str =
      make_test_heap_str(0, 0, expected_string);

  TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                        ((TestHeapStr *)cstl_vec_get(vec, 0))->x);

  TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                          ((TestHeapStr *)cstl_vec_get(vec, 0))->y);

  TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                           ((TestHeapStr *)cstl_vec_get(vec, 0))->s);

  cstl_vec_free(vec);
  free(test_heap_str_type);
  free(expected_string);
  free(expected_test_heap_str);
}

void test_push_back_to_size_10_type_test_heap_str() {

  cstl_type *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cstl_vector *vec = cstl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  size_t num_test_heap_str_objects = 10;
  size_t str_len = 40;

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len);

  TEST_ASSERT_EQUAL_size_t(num_test_heap_str_objects, vec->size);
  TEST_ASSERT_EQUAL_size_t(16, vec->capacity);
  TEST_ASSERT_EQUAL(test_heap_str_type, vec->type);

  for (size_t i = 0; i < num_test_heap_str_objects; ++i) {

    char *expected_string = create_string_of_char(str_len, 'A');

    TestHeapStr *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStr *)cstl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStr *)cstl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStr *)cstl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cstl_vec_free(vec);
  free(test_heap_str_type);
}

void test_push_back_to_size_1000_type_test_heap_str() {

  cstl_type *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cstl_vector *vec = cstl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  size_t num_test_heap_str_objects = 1000;
  size_t str_len = 40;

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len);

  TEST_ASSERT_EQUAL_size_t(num_test_heap_str_objects, vec->size);
  TEST_ASSERT_EQUAL_size_t(1024, vec->capacity);
  TEST_ASSERT_EQUAL(test_heap_str_type, vec->type);

  for (size_t i = 0; i < num_test_heap_str_objects; ++i) {

    char *expected_string = create_string_of_char(str_len, 'A');

    TestHeapStr *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStr *)cstl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStr *)cstl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStr *)cstl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cstl_vec_free(vec);
  free(test_heap_str_type);
}

void test_pop_back_size_0_type_int() {

  cstl_type *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cstl_vector *vec = cstl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_EQUAL_PTR(vec, cstl_vec_pop_back(vec));
  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cstl_vec_free(vec);
  free(int_type);
}

void test_pop_back_size_1_type_int() {

  cstl_type *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cstl_vector *vec = cstl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_int(vec, 1);
  cstl_vec_pop_back(vec);

  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cstl_vec_free(vec);
  free(int_type);
}

void test_pop_back_size_10_type_int() {

  cstl_type *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cstl_vector *vec = cstl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_int(vec, 10);
  cstl_vec_pop_back(vec);

  TEST_ASSERT_EQUAL_size_t(9, vec->size);

  for (size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cstl_vec_get(vec, i)));
  }

  cstl_vec_free(vec);
  free(int_type);
}

void test_pop_back_size_0_type_owned_string() {

  cstl_type *owned_string_type = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(owned_string_type);

  cstl_vector *vec = cstl_vec_create_empty(owned_string_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_EQUAL_PTR(vec, cstl_vec_pop_back(vec));
  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cstl_vec_free(vec);
  free(owned_string_type);
}

void test_pop_back_size_10_type_owned_string() {

  cstl_type *owned_string_type = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(owned_string_type);

  cstl_vector *vec = cstl_vec_create_empty(owned_string_type);

  TEST_ASSERT_NOT_NULL(vec);

  char char_for_string = 'A';
  vec_fill_with_owned_string(vec, 10, 20, char_for_string);

  TEST_ASSERT_EQUAL_PTR(vec, cstl_vec_pop_back(vec));
  TEST_ASSERT_EQUAL_size_t(9, vec->size);

  char *expected_owned_string = create_string_of_char(20, char_for_string);

  for(size_t i = 0; i < vec->size; ++i){
    TEST_ASSERT_EQUAL_STRING(expected_owned_string, ((OwnedString*)cstl_vec_get(vec, i))->data);
  }

  cstl_vec_free(vec);
  free(owned_string_type);
  free(expected_owned_string);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_create_empty_type_int);
  RUN_TEST(test_create_empty_type_owned_string);
  RUN_TEST(test_create_empty_type_test_heap_str);

  printf("\n");

  RUN_TEST(test_create_copy_from_size_0_type_int);
  RUN_TEST(test_create_copy_from_size_10_type_int);
  RUN_TEST(test_create_copy_from_size_1000_type_int);

  RUN_TEST(test_create_copy_from_size_0_type_owned_string);

  RUN_TEST(test_create_copy_from_size_10_type_test_heap_str);
  RUN_TEST(test_create_copy_from_size_1000_type_test_heap_str);

  printf("\n");

  RUN_TEST(test_push_back_to_size_1_type_int);
  RUN_TEST(test_push_back_to_size_10_type_int);
  RUN_TEST(test_push_back_to_size_1000_type_int);

  RUN_TEST(test_push_back_to_size_1_type_owned_string);
  RUN_TEST(test_push_back_to_size_10_type_owned_string);
  RUN_TEST(test_push_back_to_size_1000_type_owned_string);

  RUN_TEST(test_push_back_to_size_1_type_test_heap_str);
  RUN_TEST(test_push_back_to_size_10_type_test_heap_str);
  RUN_TEST(test_push_back_to_size_1000_type_test_heap_str);

  printf("\n");

  RUN_TEST(test_pop_back_size_0_type_int);
  RUN_TEST(test_pop_back_size_1_type_int);
  RUN_TEST(test_pop_back_size_10_type_int);

  RUN_TEST(test_pop_back_size_0_type_owned_string);
  RUN_TEST(test_pop_back_size_10_type_owned_string);

  return UNITY_END();
}
