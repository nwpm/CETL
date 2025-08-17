#include "../../include/cetl/cetl_vector.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include "../../src/utils/element/cetl_element.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CETL_VEC_START_CAPACITY 8
#define CETL_VEC_GROW_RATE 2

struct cetl_vector {
  cetl_size_t size;
  cetl_size_t capacity;
  cetl_byte_t *data;
  const struct cetl_element *type;
};

cetl_element *create_int_type() {

  cetl_element *int_type = malloc(sizeof(cetl_element));

  if (int_type == NULL) {
    return NULL;
  }

  int_type->size = sizeof(int);
  int_type->ctor = NULL;
  int_type->dtor = NULL;
  int_type->cmp = NULL;

  return int_type;
}

cetl_void_t vec_fill_with_int(cetl_vector *vec, cetl_size_t size) {
  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_vec_push_back(vec, &i);
  }
}

cetl_element *create_double_type() {

  cetl_element *double_type = malloc(sizeof(cetl_element));

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
  cetl_str_t s;
  cetl_int_t x;
  cetl_float_t y;
} TestHeapStr;

cetl_ptr_t test_heap_str_ctor(cetl_ptr_t dest, cetl_cptr_t data) {
  TestHeapStr *target = (TestHeapStr *)dest;
  TestHeapStr *source = (TestHeapStr *)data;
  target->x = source->x;
  target->y = source->y;
  target->s = strdup(source->s);
  return target;
}

cetl_void_t test_heap_str_dtor(cetl_ptr_t data) {
  TestHeapStr *target = (TestHeapStr *)data;
  free(target->s);
}

cetl_element *create_test_heap_str_type() {

  cetl_element *test_heap_str_type = malloc(sizeof(cetl_element));

  if (test_heap_str_type == NULL) {
    return NULL;
  }

  test_heap_str_type->size = sizeof(TestHeapStr);
  test_heap_str_type->ctor = test_heap_str_ctor;
  test_heap_str_type->dtor = test_heap_str_dtor;
  test_heap_str_type->cmp = NULL;

  return test_heap_str_type;
}

TestHeapStr *make_test_heap_str(cetl_int_t x, cetl_float_t y, cetl_str_t s) {

  TestHeapStr *test_object = malloc(sizeof(TestHeapStr));

  if (test_object == NULL) {
    return NULL;
  }

  test_object->x = x;
  test_object->y = y;
  test_object->s = s;

  return test_object;
}

cetl_str_t create_string_of_char(cetl_size_t len, cetl_int_t ch) {

  cetl_str_t str = malloc(len + 1);

  if (str == NULL) {
    return NULL;
  }

  str[len] = '\0';
  memset(str, ch, len);

  return str;
}

cetl_void_t vec_fill_with_test_heap_str(cetl_vector *v, cetl_size_t size, cetl_size_t s_len,
                                 cetl_int_t ch) {

  for (cetl_size_t i = 0; i < size; ++i) {

    cetl_str_t str = create_string_of_char(s_len, ch);
    TestHeapStr *test_heap_str = make_test_heap_str(i, i, str);
    cetl_vec_push_back(v, test_heap_str);
    free(test_heap_str->s);
    free(test_heap_str);
  }
}

typedef struct OwnedString {

  cetl_str_t data;
  cetl_size_t len;

} OwnedString;

cetl_ptr_t simple_string_ctor(cetl_ptr_t dest, cetl_cptr_t data) {
  OwnedString *target = (OwnedString *)dest;
  OwnedString *source = (OwnedString *)data;
  target->data = strdup(source->data);
  target->len = source->len;
  return target;
}

cetl_void_t simple_string_dtor(cetl_ptr_t data) {
  OwnedString *target = (OwnedString *)data;
  free(target->data);
}

cetl_element *create_owned_string_type() {

  cetl_element *t = malloc(sizeof(cetl_element));

  if (t == NULL) {
    return NULL;
  }

  t->size = sizeof(OwnedString);
  t->ctor = simple_string_ctor;
  t->dtor = simple_string_dtor;
  t->cmp = NULL;

  return t;
}

OwnedString *make_owned_string(cetl_str_t s, cetl_size_t len) {

  OwnedString *owned_string = malloc(sizeof(OwnedString));

  if (owned_string == NULL) {
    return NULL;
  }

  owned_string->data = s;
  owned_string->len = len;

  return owned_string;
}

cetl_void_t vec_fill_with_owned_string(cetl_vector *v, cetl_size_t num, cetl_size_t s_len,
                                cetl_int_t ch) {

  for (cetl_size_t i = 0; i < num; ++i) {
    cetl_str_t s = create_string_of_char(s_len, ch);
    OwnedString *owned_string = make_owned_string(s, s_len);
    cetl_vec_push_back(v, owned_string);
    free(s);
    free(owned_string);
  }
}

cetl_void_t setUp() {}
cetl_void_t tearDown() {}

//-------------------------------------

cetl_void_t test_create_empty_type_int() {

  cetl_element *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CETL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cetl_vec_free(v);
  free(t);
}

cetl_void_t test_create_empty_type_owned_string() {

  cetl_element *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CETL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cetl_vec_free(v);
  free(t);
}

cetl_void_t test_create_empty_type_test_heap_str() {

  cetl_element *t = create_test_heap_str_type();

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CETL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cetl_vec_free(v);
  free(t);
}

//-----------------------------

cetl_void_t test_create_copy_from_size_0_type_int() {

  cetl_element *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_vector *c_v = cetl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_NOT_NULL(c_v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);

  cetl_vec_free(c_v);
  cetl_vec_free(v);
  free(t);
}

cetl_void_t test_create_copy_from_size_0_type_owned_string() {

  cetl_element *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_vector *c_v = cetl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_NOT_NULL(c_v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);

  cetl_vec_free(c_v);
  cetl_vec_free(v);
  free(t);
}

cetl_void_t test_create_copy_from_size_10_type_int() {

  cetl_element *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 10);

  cetl_vector *c_v = cetl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (cetl_size_t i = 0; i < 10; ++i) {
    cetl_ptr_t current_orig = cetl_vec_get(v, i);
    cetl_ptr_t current_copy = cetl_vec_get(c_v, i);
    TEST_ASSERT_EQUAL_INT(*((int *)current_orig), *((int *)current_copy));
  }

  cetl_vec_free(v);
  cetl_vec_free(c_v);
  free(t);
}

cetl_void_t test_create_copy_from_size_1000_type_int() {

  cetl_element *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 1000);

  cetl_vector *c_v = cetl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (cetl_size_t i = 0; i < 1000; ++i) {
    cetl_ptr_t current_orig = cetl_vec_get(v, i);
    cetl_ptr_t current_copy = cetl_vec_get(c_v, i);
    TEST_ASSERT_EQUAL_INT(*((int *)current_orig), *((int *)current_copy));
  }

  cetl_vec_free(v);
  cetl_vec_free(c_v);
  free(t);
}

cetl_void_t test_create_copy_from_size_10_type_test_heap_str() {

  cetl_element *t = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_size_t num_test_heap_str_objects = 10;
  cetl_size_t str_len = 15;
  cetl_uchar_t char_for_string = 'S';

  vec_fill_with_test_heap_str(v, num_test_heap_str_objects, str_len,
                              char_for_string);

  cetl_vector *c_v = cetl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (cetl_size_t i = 0; i < 10; ++i) {
    cetl_ptr_t current_orig = cetl_vec_get(v, i);
    cetl_ptr_t current_copy = cetl_vec_get(c_v, i);

    TEST_ASSERT_EQUAL_INT(((TestHeapStr *)current_orig)->x,
                          ((TestHeapStr *)current_copy)->x);

    TEST_ASSERT_EQUAL_FLOAT(((TestHeapStr *)current_orig)->y,
                            ((TestHeapStr *)current_orig)->y);

    TEST_ASSERT_EQUAL_STRING(((TestHeapStr *)current_orig)->s,
                             ((TestHeapStr *)current_orig)->s);
  }

  cetl_vec_free(v);
  cetl_vec_free(c_v);
  free(t);
}

cetl_void_t test_create_copy_from_size_1000_type_test_heap_str() {

  cetl_element *t = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_size_t num_test_heap_str_objects = 1000;
  cetl_size_t str_len = 50;
  cetl_uchar_t char_for_string = 'S';

  vec_fill_with_test_heap_str(v, num_test_heap_str_objects, str_len,
                              char_for_string);

  cetl_vector *c_v = cetl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (cetl_size_t i = 0; i < 1000; ++i) {
    cetl_ptr_t current_orig = cetl_vec_get(v, i);
    cetl_ptr_t current_copy = cetl_vec_get(c_v, i);

    TEST_ASSERT_EQUAL_INT(((TestHeapStr *)current_orig)->x,
                          ((TestHeapStr *)current_copy)->x);

    TEST_ASSERT_EQUAL_FLOAT(((TestHeapStr *)current_orig)->y,
                            ((TestHeapStr *)current_orig)->y);

    TEST_ASSERT_EQUAL_STRING(((TestHeapStr *)current_orig)->s,
                             ((TestHeapStr *)current_orig)->s);
  }

  cetl_vec_free(v);
  cetl_vec_free(c_v);
  free(t);
}

cetl_void_t test_push_back_to_size_1_type_int() {

  cetl_element *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_int_t data = 10;

  TEST_ASSERT_NOT_NULL(cetl_vec_push_back(v, &data));

  TEST_ASSERT_EQUAL_size_t(1, v->size);
  TEST_ASSERT_EQUAL_size_t(CETL_VEC_START_CAPACITY, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  TEST_ASSERT_EQUAL_INT(10, *((int *)cetl_vec_get(v, 0)));

  cetl_vec_free(v);
  free(t);
}

cetl_void_t test_push_back_to_size_10_type_int() {

  cetl_element *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 10);

  TEST_ASSERT_EQUAL_size_t(10, v->size);
  TEST_ASSERT_EQUAL_size_t(CETL_VEC_START_CAPACITY * 2, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  for (cetl_size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cetl_vec_get(v, i)));
  }

  cetl_vec_free(v);
  free(t);
}

cetl_void_t test_push_back_to_size_1000_type_int() {

  cetl_element *t = create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  vec_fill_with_int(v, 1000);

  TEST_ASSERT_EQUAL_size_t(1000, v->size);
  TEST_ASSERT_EQUAL_size_t(1024, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  for (cetl_size_t i = 0; i < 1000; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cetl_vec_get(v, i)));
  }

  cetl_vec_free(v);
  free(t);
}

cetl_void_t test_push_back_to_size_1_type_owned_string() {

  cetl_element *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_uchar_t char_for_string = 'A';
  cetl_size_t char_array_size = 20;

  vec_fill_with_owned_string(v, 1, char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(1, v->size);
  TEST_ASSERT_EQUAL_size_t(CETL_VEC_START_CAPACITY, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  cetl_str_t s_for_check = create_string_of_char(char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(char_array_size,
                           ((OwnedString *)cetl_vec_get(v, 0))->len);
  TEST_ASSERT_EQUAL_STRING(s_for_check,
                           ((OwnedString *)cetl_vec_get(v, 0))->data);

  cetl_vec_free(v);
  free(t);
  free(s_for_check);
}

cetl_void_t test_push_back_to_size_10_type_owned_string() {

  cetl_element *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_uchar_t char_for_string = 'A';
  cetl_size_t char_array_size = 50;
  cetl_size_t num_elements = 10;

  vec_fill_with_owned_string(v, num_elements, char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(num_elements, v->size);
  TEST_ASSERT_EQUAL_size_t(CETL_VEC_START_CAPACITY * 2, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  cetl_str_t s_for_check = create_string_of_char(char_array_size, char_for_string);

  for (cetl_size_t i = 0; i < num_elements; ++i) {
    TEST_ASSERT_EQUAL_size_t(char_array_size,
                             ((OwnedString *)cetl_vec_get(v, 0))->len);
    TEST_ASSERT_EQUAL_STRING(s_for_check,
                             ((OwnedString *)cetl_vec_get(v, 0))->data);
  }

  cetl_vec_free(v);
  free(t);
  free(s_for_check);
}

cetl_void_t test_push_back_to_size_1000_type_owned_string() {

  cetl_element *t = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cetl_vector *v = cetl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cetl_uchar_t char_for_string = 'A';
  cetl_size_t char_array_size = 80;

  vec_fill_with_owned_string(v, 1000, char_array_size, char_for_string);

  TEST_ASSERT_EQUAL_size_t(1000, v->size);
  TEST_ASSERT_EQUAL_size_t(1024, v->capacity);
  TEST_ASSERT_EQUAL(t, v->type);

  cetl_str_t s_for_check = create_string_of_char(char_array_size, char_for_string);

  for (cetl_size_t i = 0; i < 1000; ++i) {
    TEST_ASSERT_EQUAL_size_t(char_array_size,
                             ((OwnedString *)cetl_vec_get(v, 0))->len);
    TEST_ASSERT_EQUAL_STRING(s_for_check,
                             ((OwnedString *)cetl_vec_get(v, 0))->data);
  }

  cetl_vec_free(v);
  free(t);
  free(s_for_check);
}

cetl_void_t test_push_back_to_size_1_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  cetl_size_t num_test_heap_str_objects = 1;
  cetl_size_t str_len = 10;
  cetl_uchar_t char_for_string = 'S';

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len,
                              char_for_string);

  TEST_ASSERT_EQUAL_size_t(1, vec->size);
  TEST_ASSERT_EQUAL_size_t(CETL_VEC_START_CAPACITY, vec->capacity);
  TEST_ASSERT_EQUAL(test_heap_str_type, vec->type);

  cetl_str_t expected_string = create_string_of_char(str_len, char_for_string);
  TestHeapStr *expected_test_heap_str =
      make_test_heap_str(0, 0, expected_string);

  TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                        ((TestHeapStr *)cetl_vec_get(vec, 0))->x);

  TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                          ((TestHeapStr *)cetl_vec_get(vec, 0))->y);

  TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                           ((TestHeapStr *)cetl_vec_get(vec, 0))->s);

  cetl_vec_free(vec);
  free(test_heap_str_type);
  free(expected_string);
  free(expected_test_heap_str);
}

cetl_void_t test_push_back_to_size_10_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  cetl_size_t num_test_heap_str_objects = 10;
  cetl_size_t str_len = 40;
  cetl_uchar_t char_for_string = 'Q';

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len,
                              char_for_string);

  TEST_ASSERT_EQUAL_size_t(num_test_heap_str_objects, vec->size);
  TEST_ASSERT_EQUAL_size_t(16, vec->capacity);
  TEST_ASSERT_EQUAL(test_heap_str_type, vec->type);

  for (cetl_size_t i = 0; i < num_test_heap_str_objects; ++i) {

    cetl_str_t expected_string = create_string_of_char(str_len, char_for_string);

    TestHeapStr *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStr *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStr *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStr *)cetl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

cetl_void_t test_push_back_to_size_1000_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  cetl_size_t num_test_heap_str_objects = 1000;
  cetl_size_t str_len = 40;
  cetl_uchar_t char_for_string = 'F';

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len,
                              char_for_string);

  TEST_ASSERT_EQUAL_size_t(num_test_heap_str_objects, vec->size);
  TEST_ASSERT_EQUAL_size_t(1024, vec->capacity);
  TEST_ASSERT_EQUAL(test_heap_str_type, vec->type);

  for (cetl_size_t i = 0; i < num_test_heap_str_objects; ++i) {

    cetl_str_t expected_string = create_string_of_char(str_len, char_for_string);

    TestHeapStr *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStr *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStr *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStr *)cetl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

cetl_void_t test_pop_back_size_0_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_EQUAL_PTR(vec, cetl_vec_pop_back(vec));
  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_pop_back_size_1_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_int(vec, 1);
  cetl_vec_pop_back(vec);

  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_pop_back_size_10_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_int(vec, 10);
  cetl_vec_pop_back(vec);

  TEST_ASSERT_EQUAL_size_t(9, vec->size);

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cetl_vec_get(vec, i)));
  }

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_pop_back_size_0_type_owned_string() {

  cetl_element *owned_string_type = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(owned_string_type);

  cetl_vector *vec = cetl_vec_create_empty(owned_string_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_EQUAL_PTR(vec, cetl_vec_pop_back(vec));
  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cetl_vec_free(vec);
  free(owned_string_type);
}

cetl_void_t test_pop_back_size_10_type_owned_string() {

  cetl_element *owned_string_type = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(owned_string_type);

  cetl_vector *vec = cetl_vec_create_empty(owned_string_type);

  TEST_ASSERT_NOT_NULL(vec);

  cetl_size_t num_owned_string_objects = 10;
  cetl_size_t str_len = 20;
  cetl_uchar_t char_for_string = 'F';

  vec_fill_with_owned_string(vec, num_owned_string_objects, str_len,
                             char_for_string);

  TEST_ASSERT_EQUAL_PTR(vec, cetl_vec_pop_back(vec));
  TEST_ASSERT_EQUAL_size_t(9, vec->size);

  cetl_str_t expected_owned_string = create_string_of_char(str_len, char_for_string);

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_STRING(expected_owned_string,
                             ((OwnedString *)cetl_vec_get(vec, i))->data);
  }

  cetl_vec_free(vec);
  free(owned_string_type);
  free(expected_owned_string);
}

cetl_void_t test_pop_back_size_0_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_EQUAL_PTR(vec, cetl_vec_pop_back(vec));
  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

cetl_void_t test_pop_back_size_1_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  cetl_size_t num_test_heap_str_objects = 1;
  cetl_size_t str_len = 10;
  cetl_uchar_t char_for_string = 'F';

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len,
                              char_for_string);

  cetl_vec_pop_back(vec);

  TEST_ASSERT_EQUAL_size_t(0, vec->size);

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

cetl_void_t test_pop_back_size_10_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  cetl_size_t num_test_heap_str_objects = 10;
  cetl_size_t str_len = 50;
  cetl_uchar_t char_for_string = 'F';

  vec_fill_with_test_heap_str(vec, num_test_heap_str_objects, str_len,
                              char_for_string);

  cetl_vec_pop_back(vec);

  TEST_ASSERT_EQUAL_size_t(9, vec->size);

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_INT(i, i);
  }

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

cetl_void_t test_recetl_size_to_capacity_8_from_capacity_8_size_0_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_EQUAL_PTR(vec, cetl_vec_resize(vec, 8));

  TEST_ASSERT_EQUAL_size_t(0, vec->size);
  TEST_ASSERT_EQUAL_size_t(8, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(int_type, vec->type);
  TEST_ASSERT_NOT_NULL(vec->data);

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_recetl_size_to_capacity_0_from_capacity_8_size_0_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 0));

  TEST_ASSERT_EQUAL_size_t(0, vec->size);
  TEST_ASSERT_EQUAL_size_t(0, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(int_type, vec->type);
  TEST_ASSERT_NULL(vec->data);

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_recetl_size_to_capacity_16_from_capacity_8_size_0_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 16));

  TEST_ASSERT_EQUAL_size_t(0, vec->size);
  TEST_ASSERT_EQUAL_size_t(16, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(int_type, vec->type);
  TEST_ASSERT_NOT_NULL(vec->data);

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_int(vec, 5);

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 16));

  TEST_ASSERT_EQUAL_size_t(5, vec->size);
  TEST_ASSERT_EQUAL_size_t(16, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(int_type, vec->type);

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cetl_vec_get(vec, i)));
  }

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_int(vec, 5);

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 3));

  TEST_ASSERT_EQUAL_size_t(3, vec->size);
  TEST_ASSERT_EQUAL_size_t(3, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(int_type, vec->type);

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)cetl_vec_get(vec, i)));
  }

  cetl_vec_free(vec);
  free(int_type);
}

cetl_void_t test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_owned_string() {

  cetl_element *owned_string_type = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(owned_string_type);

  cetl_vector *vec = cetl_vec_create_empty(owned_string_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_owned_string(vec, 5, 10, 'A');

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 16));

  TEST_ASSERT_EQUAL_size_t(5, vec->size);
  TEST_ASSERT_EQUAL_size_t(16, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(owned_string_type, vec->type);

  cetl_str_t expected_owned_string = create_string_of_char(10, 'A');

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_STRING(expected_owned_string,
                             ((OwnedString *)cetl_vec_get(vec, i))->data);
  }

  cetl_vec_free(vec);
  free(owned_string_type);
  free(expected_owned_string);
}

cetl_void_t test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_owned_string() {

  cetl_element *owned_string_type = create_owned_string_type();

  TEST_ASSERT_NOT_NULL(owned_string_type);

  cetl_vector *vec = cetl_vec_create_empty(owned_string_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_owned_string(vec, 5, 10, 'A');

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 3));

  TEST_ASSERT_EQUAL_size_t(3, vec->size);
  TEST_ASSERT_EQUAL_size_t(3, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(owned_string_type, vec->type);

  cetl_str_t expected_owned_string = create_string_of_char(10, 'A');

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    TEST_ASSERT_EQUAL_STRING(expected_owned_string,
                             ((OwnedString *)cetl_vec_get(vec, i))->data);
  }

  cetl_vec_free(vec);
  free(owned_string_type);
  free(expected_owned_string);
}

cetl_void_t test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_test_heap_str(vec, 5, 40, 'F');

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 16));

  TEST_ASSERT_EQUAL_size_t(5, vec->size);
  TEST_ASSERT_EQUAL_size_t(16, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(test_heap_str_type, vec->type);

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    cetl_str_t expected_string = create_string_of_char(40, 'F');

    TestHeapStr *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStr *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStr *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStr *)cetl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

cetl_void_t test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_test_heap_str() {

  cetl_element *test_heap_str_type = create_test_heap_str_type();

  TEST_ASSERT_NOT_NULL(test_heap_str_type);

  cetl_vector *vec = cetl_vec_create_empty(test_heap_str_type);

  TEST_ASSERT_NOT_NULL(vec);

  vec_fill_with_test_heap_str(vec, 5, 40, 'F');

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, 3));

  TEST_ASSERT_EQUAL_size_t(3, vec->size);
  TEST_ASSERT_EQUAL_size_t(3, vec->capacity);
  TEST_ASSERT_EQUAL_PTR(test_heap_str_type, vec->type);

  for (cetl_size_t i = 0; i < vec->size; ++i) {
    cetl_str_t expected_string = create_string_of_char(40, 'F');

    TestHeapStr *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStr *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStr *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStr *)cetl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

void test_iter_type_int(){

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  vec_fill_with_int(vec, 3);

  cetl_iterator* it = cetl_vec_iter_begin(vec);
  cetl_iterator* it_end = cetl_vec_iter_end(vec);

  size_t i = 0;

  for(; !it->equal(it, it_end); it->next(it), i++){
    TEST_ASSERT_EQUAL_INT(i, *((int*)it->get(it)));
  }

  cetl_vec_free(vec);
  free(int_type);
  cetl_vec_iter_free(it);
  cetl_vec_iter_free(it_end);
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

  RUN_TEST(test_pop_back_size_0_type_test_heap_str);
  RUN_TEST(test_pop_back_size_10_type_test_heap_str);

  printf("\n");

  RUN_TEST(test_recetl_size_to_capacity_0_from_capacity_8_size_0_type_int);
  RUN_TEST(test_recetl_size_to_capacity_8_from_capacity_8_size_0_type_int);
  RUN_TEST(test_recetl_size_to_capacity_16_from_capacity_8_size_0_type_int);
  RUN_TEST(test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_int);
  RUN_TEST(test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_int);

  RUN_TEST(test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_owned_string);
  RUN_TEST(test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_owned_string);

  RUN_TEST(test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_test_heap_str);
  RUN_TEST(test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_test_heap_str);

  printf("\n");

  RUN_TEST(test_iter_type_int);

  return UNITY_END();
}
