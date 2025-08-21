#include "../../include/cetl/cetl_vector.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include "../../src/utils/element/cetl_element.h"
#include "../../src/utils/iterator/iterator.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// NOTE: stopped on creating expected data arrays

#define TEST_STR_SIZE 50
#define TEST_STR_CHAR_FILL ('A')
#define CETL_VEC_START_CAPACITY 8
#define CETL_VEC_GROW_RATE 2

struct cetl_vector {
  cetl_size_t size;
  cetl_size_t capacity;
  cetl_byte_t *data;
  const struct cetl_element *type;
};

typedef cetl_void_t (*vec_fill_with_type)(cetl_vector *vec, cetl_size_t size,
                                          cetl_void_t *fill_data);
typedef cetl_void_t (*vec_check_data)(cetl_vector *vec, cetl_size_t size,
                                      cetl_size_t expected_data);
typedef cetl_void_t (*vec_check_copy_data)(cetl_vector *src, cetl_vector *copy,
                                           cetl_size_t size);

static cetl_element *int_type = NULL;
static cetl_element *str_type = NULL;
static cetl_element *heap_struct_type = NULL;

cetl_element *create_int_type() {

  cetl_element *int_type = malloc(sizeof(cetl_element));
  TEST_ASSERT_NOT_NULL(int_type);

  int_type->size = sizeof(int);
  int_type->ctor = NULL;
  int_type->dtor = NULL;
  int_type->cmp = NULL;

  return int_type;
}

cetl_int_t *make_expected_ints(cetl_size_t size) {

  cetl_int_t *expected = malloc(size * sizeof(cetl_int_t));
  TEST_ASSERT_NOT_NULL(expected);

  for (cetl_size_t i = 0; i < size; ++i) {
    expected[i] = i;
  }

  return expected;
}

cetl_void_t vec_fill_with_int(cetl_vector *vec, cetl_size_t size,
                              cetl_void_t *fill_data) {
  cetl_int_t* fill_arr = (int*)fill_data;
  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_vec_push_back(vec, fill_arr + i);
  }
}

cetl_void_t vec_check_int(cetl_vector *vec, cetl_size_t size,
                          cetl_int_t* check_from_data) {
  for (size_t i = 0; i < size; ++i) {
    TEST_ASSERT_EQUAL_INT(check_from_data[i], *((int *)cetl_vec_get(vec, i)));
  }
}

cetl_void_t vec_check_int_copy(cetl_vector *src, cetl_vector *copy,
                               cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_int_t current_src = *((int *)cetl_vec_get(src, i));
    cetl_int_t current_copy = *((int *)cetl_vec_get(copy, i));
    TEST_ASSERT_EQUAL_INT(current_src, current_copy);
  }
}

typedef struct TestHeapStruct {
  cetl_str_t s;
  cetl_int_t x;
  cetl_float_t y;
} TestHeapStruct;

cetl_ptr_t test_heap_struct_ctor(cetl_ptr_t dest, cetl_cptr_t data) {
  TestHeapStruct *target = (TestHeapStruct *)dest;
  TestHeapStruct *source = (TestHeapStruct *)data;
  target->x = source->x;
  target->y = source->y;
  target->s = strdup(source->s);
  return target;
}

cetl_void_t test_heap_struct_dtor(cetl_ptr_t data) {
  TestHeapStruct *target = (TestHeapStruct *)data;
  free(target->s);
  // free(target);
}

cetl_element *create_test_heap_struct_type() {

  cetl_element *test_heap_struct_type = malloc(sizeof(cetl_element));
  TEST_ASSERT_NOT_NULL(test_heap_struct_type);

  test_heap_struct_type->size = sizeof(TestHeapStruct);
  test_heap_struct_type->ctor = test_heap_struct_ctor;
  test_heap_struct_type->dtor = test_heap_struct_dtor;
  test_heap_struct_type->cmp = NULL;

  return test_heap_struct_type;
}

TestHeapStruct *make_test_heap_struct(cetl_int_t x, cetl_float_t y,
                                      cetl_str_t s) {

  TestHeapStruct *test_object = malloc(sizeof(TestHeapStruct));
  TEST_ASSERT_NOT_NULL(test_object);

  test_object->x = x;
  test_object->y = y;
  test_object->s = s;

  return test_object;
}

cetl_str_t create_string_of_char(cetl_size_t len, cetl_int_t ch) {

  cetl_str_t str = malloc(len + 1);
  TEST_ASSERT_NOT_NULL(str);

  str[len] = '\0';
  memset(str, ch, len);

  return str;
}

cetl_void_t vec_fill_with_test_heap_struct(cetl_vector *vec, cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_str_t c_str = create_string_of_char(TEST_STR_SIZE, TEST_STR_CHAR_FILL);
    TestHeapStruct *test_heap_struct = make_test_heap_struct(i, i, c_str);
    cetl_vec_push_back(vec, test_heap_struct);
    free(test_heap_struct->s);
    free(test_heap_struct);
  }
}

cetl_void_t vec_check_heap_struct_copy(cetl_vector *src, cetl_vector *copy,
                                       cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    TestHeapStruct *current_src = (TestHeapStruct *)cetl_vec_get(src, i);
    TestHeapStruct *current_copy = (TestHeapStruct *)cetl_vec_get(copy, i);
    TEST_ASSERT_EQUAL_INT(current_src->x, current_copy->x);
    TEST_ASSERT_EQUAL_FLOAT(current_src->y, current_copy->y);
    TEST_ASSERT_EQUAL_STRING(current_src->s, current_copy->s);
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

  cetl_element *str_type = malloc(sizeof(cetl_element));
  TEST_ASSERT_NOT_NULL(str_type);

  str_type->size = sizeof(OwnedString);
  str_type->ctor = simple_string_ctor;
  str_type->dtor = simple_string_dtor;
  str_type->cmp = NULL;

  return str_type;
}

OwnedString *make_owned_string(cetl_str_t c_str, cetl_size_t len) {

  OwnedString *owned_string = malloc(sizeof(OwnedString));
  TEST_ASSERT_NOT_NULL(owned_string);

  owned_string->data = c_str;
  owned_string->len = len;

  return owned_string;
}

OwnedString **make_expected_str(cetl_size_t size) {

  OwnedString **expected = malloc(size * sizeof(OwnedString *));
  TEST_ASSERT_NOT_NULL(expected);

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_str_t c_str = create_string_of_char(TEST_STR_SIZE, TEST_STR_CHAR_FILL);
    expected[i] = make_owned_string(c_str, TEST_STR_SIZE);
  }

  return expected;
}

cetl_void_t vec_fill_with_owned_string(cetl_vector *vec, cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_str_t c_str = create_string_of_char(TEST_STR_SIZE, TEST_STR_CHAR_FILL);
    OwnedString *owned_string = make_owned_string(c_str, TEST_STR_SIZE);
    cetl_vec_push_back(vec, owned_string);
    free(c_str);
    free(owned_string);
  }
}

cetl_void_t vec_check_str(cetl_vector *vec, cetl_size_t size,
                          OwnedString *check_from_data) {

  for (cetl_size_t i = 0; i < size; ++i) {

    OwnedString *current_vec = (OwnedString *)cetl_vec_get(src, i);

    TEST_ASSERT_EQUAL_size_t(current_src->len, current_copy->len);
    TEST_ASSERT_EQUAL_STRING(current_src->data, current_copy->data);
  }
}

cetl_void_t vec_check_str_copy(cetl_vector *src, cetl_vector *copy,
                               cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {

    OwnedString *current_src = (OwnedString *)cetl_vec_get(src, i);
    OwnedString *current_copy = (OwnedString *)cetl_vec_get(copy, i);

    TEST_ASSERT_EQUAL_size_t(current_src->len, current_copy->len);
    TEST_ASSERT_EQUAL_STRING(current_src->data, current_copy->data);
  }
}

cetl_void_t setUp() {

  int_type = create_int_type();
  str_type = create_owned_string_type();
  heap_struct_type = create_test_heap_struct_type();
}
cetl_void_t tearDown() {

  free(int_type);
  free(str_type);
  free(heap_struct_type);

  int_type = NULL;
  str_type = NULL;
  heap_struct_type = NULL;
}

// Create empty
//-------------------------------------

cetl_void_t test_create_empty_vec_with_type(cetl_element *type) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_EQUAL_size_t(vec->capacity, CETL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(vec->size, 0);
  TEST_ASSERT_NOT_NULL(vec->data);
  TEST_ASSERT_EQUAL_PTR(vec->type, type);

  cetl_vec_free(vec);
}

cetl_void_t test_create_empty_type_int() {
  test_create_empty_vec_with_type(int_type);
}

cetl_void_t test_create_empty_type_owned_string() {
  test_create_empty_vec_with_type(str_type);
}

cetl_void_t test_create_empty_type_test_heap_str() {
  test_create_empty_vec_with_type(heap_struct_type);
}

//-----------------------------

cetl_void_t test_create_copy(cetl_element *type, cetl_size_t size,
                             vec_fill_with_type filler,
                             vec_check_copy_data check_copy) {

  cetl_vector *src_vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(src_vec);

  filler(src_vec, size);

  cetl_vector *copy_vec = cetl_vec_create_copy(copy_vec);
  TEST_ASSERT_NOT_NULL(copy_vec);

  TEST_ASSERT_EQUAL_size_t(src_vec->size, copy_vec->size);
  TEST_ASSERT_EQUAL_size_t(src_vec->capacity, copy_vec->capacity);
  TEST_ASSERT_EQUAL_PTR(src_vec->type, copy_vec->type);

  check_copy(src_vec, copy_vec, size);

  cetl_vec_free(copy_vec);
  cetl_vec_free(src_vec);
}

cetl_void_t test_create_copy_from_size_0_type_int() {
  test_create_copy(int_type, 0, vec_fill_with_int, vec_check_int_copy);
}

cetl_void_t test_create_copy_from_size_0_type_owned_string() {
  test_create_copy(str_type, 0, vec_fill_with_owned_string, vec_check_str_copy);
}

cetl_void_t test_create_copy_from_size_10_type_int() {
  test_create_copy(int_type, 10, vec_fill_with_int, vec_check_int_copy);
}

cetl_void_t test_create_copy_from_size_1000_type_int() {
  test_create_copy(int_type, 1000, vec_fill_with_int, vec_check_int_copy);
}

cetl_void_t test_create_copy_from_size_10_type_test_heap_str() {
  test_create_copy(heap_struct_type, 10, vec_fill_with_test_heap_struct,
                   vec_check_heap_struct_copy);
}

cetl_void_t test_create_copy_from_size_1000_type_test_heap_str() {
  test_create_copy(heap_struct_type, 1000, vec_fill_with_test_heap_struct,
                   vec_check_heap_struct_copy);
}

// Operations

cetl_void_t test_push_back(cetl_element *type, cetl_size_t size,
                           vec_fill_with_type filler,
                           vec_check_data check_data) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  filler(vec, size);

  TEST_ASSERT_EQUAL_size_t(size, vec->size);
  TEST_ASSERT_EQUAL(type, vec->type);

  check_data(vec, size);

  cetl_vec_free(vec);
}

cetl_void_t test_push_back_to_size_1_type_int() {
  test_push_back(int_type, 1, vec_fill_with_int, vec_check_int);
}

cetl_void_t test_push_back_to_size_10_type_int() {
  test_push_back(int_type, 10, vec_fill_with_int, vec_check_int);
}

cetl_void_t test_push_back_to_size_1000_type_int() {
  test_push_back(int_type, 1000, vec_fill_with_int, vec_check_int);
}

cetl_void_t test_push_back_to_size_1_type_owned_string() {
  test_push_back(str_type, 1, vec_fill_with_owned_string, NULL);
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

  cetl_str_t s_for_check =
      create_string_of_char(char_array_size, char_for_string);

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

  cetl_str_t s_for_check =
      create_string_of_char(char_array_size, char_for_string);

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
  TestHeapStruct *expected_test_heap_str =
      make_test_heap_str(0, 0, expected_string);

  TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                        ((TestHeapStruct *)cetl_vec_get(vec, 0))->x);

  TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                          ((TestHeapStruct *)cetl_vec_get(vec, 0))->y);

  TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                           ((TestHeapStruct *)cetl_vec_get(vec, 0))->s);

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

    cetl_str_t expected_string =
        create_string_of_char(str_len, char_for_string);

    TestHeapStruct *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStruct *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStruct *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStruct *)cetl_vec_get(vec, i))->s);

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

    cetl_str_t expected_string =
        create_string_of_char(str_len, char_for_string);

    TestHeapStruct *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStruct *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStruct *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStruct *)cetl_vec_get(vec, i))->s);

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

  cetl_str_t expected_owned_string =
      create_string_of_char(str_len, char_for_string);

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

cetl_void_t
test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_owned_string() {

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

cetl_void_t
test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_owned_string() {

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

cetl_void_t
test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_test_heap_str() {

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

    TestHeapStruct *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStruct *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStruct *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStruct *)cetl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

cetl_void_t
test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_test_heap_str() {

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

    TestHeapStruct *expected_test_heap_str =
        make_test_heap_str(i, i, expected_string);

    TEST_ASSERT_EQUAL_INT(expected_test_heap_str->x,
                          ((TestHeapStruct *)cetl_vec_get(vec, i))->x);

    TEST_ASSERT_EQUAL_FLOAT(expected_test_heap_str->y,
                            ((TestHeapStruct *)cetl_vec_get(vec, i))->y);

    TEST_ASSERT_EQUAL_STRING(expected_test_heap_str->s,
                             ((TestHeapStruct *)cetl_vec_get(vec, i))->s);

    free(expected_string);
    free(expected_test_heap_str);
  }

  cetl_vec_free(vec);
  free(test_heap_str_type);
}

void test_iter_type_int() {

  cetl_element *int_type = create_int_type();

  TEST_ASSERT_NOT_NULL(int_type);

  cetl_vector *vec = cetl_vec_create_empty(int_type);

  vec_fill_with_int(vec, 3);

  cetl_iterator *it = cetl_vec_iter_begin(vec);
  cetl_iterator *it_end = cetl_vec_iter_end(vec);

  size_t i = 0;

  for (; !it->equal(it, it_end); it->next(it), i++) {
    TEST_ASSERT_EQUAL_INT(i, *((int *)it->get(it)));
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

  RUN_TEST(
      test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_owned_string);
  RUN_TEST(
      test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_owned_string);

  RUN_TEST(
      test_recetl_size_to_capacity_3_from_capacity_8_size_5_type_test_heap_str);
  RUN_TEST(
      test_recetl_size_to_capacity_16_from_capacity_8_size_5_type_test_heap_str);

  printf("\n");

  RUN_TEST(test_iter_type_int);

  return UNITY_END();
}
