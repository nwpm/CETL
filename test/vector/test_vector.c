#include "../../include/cetl/cetl_vector.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include "../../src/utils/element/cetl_element.h"
#include "../../src/utils/iterator/iterator.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static cetl_element *int_type = NULL;
static cetl_element *double_type = NULL;
static cetl_element *char_type = NULL;
static cetl_element *struct_type = NULL;
static cetl_element *heap_struct_type = NULL;

typedef struct TestStruct {
  cetl_int_t x;
  cetl_int_t y;
} TestStruct;

typedef struct TestHeapStruct {
  cetl_str_t data;
} TestHeapStruct;

// ********** Base types **********
cetl_element *create_int_type() {

  cetl_element *int_type = malloc(sizeof(cetl_element));
  if (!int_type)
    return NULL;

  int_type->size = sizeof(int);
  int_type->ctor = NULL;
  int_type->dtor = NULL;
  int_type->cmp = NULL;

  return int_type;
}

cetl_element *create_double_type() {

  cetl_element *double_type = malloc(sizeof(cetl_element));
  if (!double_type)
    return NULL;

  double_type->size = sizeof(double);
  double_type->ctor = NULL;
  double_type->dtor = NULL;
  double_type->cmp = NULL;

  return double_type;
}

cetl_element *create_char_type() {

  cetl_element *char_type = malloc(sizeof(cetl_element));
  if (!char_type)
    return NULL;

  char_type->size = sizeof(char);
  char_type->ctor = NULL;
  char_type->dtor = NULL;
  char_type->cmp = NULL;

  return char_type;
}

// ********************

// ********** User types **********

cetl_element *create_struct_type() {

  cetl_element *struct_type = malloc(sizeof(cetl_element));
  if (!struct_type)
    return NULL;

  struct_type->size = sizeof(TestStruct);
  struct_type->ctor = NULL;
  struct_type->dtor = NULL;
  struct_type->cmp = NULL;

  return struct_type;
}

cetl_ptr_t heap_struct_ctor(cetl_ptr_t dest, cetl_cptr_t data) {

  TestHeapStruct *target = (TestHeapStruct *)dest;
  TestHeapStruct *source = (TestHeapStruct *)data;

  target->data = strdup(source->data);

  return target;
}

cetl_void_t heap_struct_dtor(cetl_ptr_t data) {

  TestHeapStruct *target = (TestHeapStruct *)data;
  free(target->data);
}

cetl_element *create_heap_struct_type() {

  cetl_element *heap_struct_type = malloc(sizeof(cetl_element));
  if (!heap_struct_type)
    return NULL;

  heap_struct_type->size = sizeof(TestHeapStruct);
  heap_struct_type->ctor = heap_struct_ctor;
  heap_struct_type->dtor = heap_struct_dtor;
  heap_struct_type->cmp = NULL;

  return heap_struct_type;
}

// ********************

// ********** Fill vector **********

cetl_void_t fill_int_vector(cetl_vector *vec, cetl_size_t size) {

  cetl_int_t init_value = 1;

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_vec_push_back(vec, &init_value);
  }
}

cetl_void_t fill_double_vector(cetl_vector *vec, cetl_size_t size) {

  cetl_double_t init_value = 1.;

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_vec_push_back(vec, &init_value);
  }
}

cetl_void_t fill_char_vector(cetl_vector *vec, cetl_size_t size) {

  cetl_char_t init_value = 1;

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_vec_push_back(vec, &init_value);
  }
}

cetl_void_t fill_struct_vector(cetl_vector *vec, cetl_size_t size) {
  
  TestStruct init_value = {1, 2}; 

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_vec_push_back(vec, &init_value);
  }
}

cetl_void_t fill_heap_struct_vector(cetl_vector *vec, cetl_size_t size) {
  
  TestHeapStruct *init_value = malloc(sizeof(TestHeapStruct)); 
  init_value->data = "string";

  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_vec_push_back(vec, init_value);
  }

  free(init_value);
}

// ********************
void setUp() {

  int_type = create_int_type();
  double_type = create_double_type();
  char_type = create_char_type();
  struct_type = create_struct_type();
  heap_struct_type = create_heap_struct_type();
}

void tearDown() {

  free(int_type);
  free(double_type);
  free(char_type);
  free(struct_type);
  free(heap_struct_type);
}

// ********** Create empty **********

cetl_void_t check_empty(const cetl_element *type) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  TEST_ASSERT_NOT_NULL(cetl_vec_data(vec));
  TEST_ASSERT_EQUAL_size_t(0, cetl_vec_size(vec));
  TEST_ASSERT_EQUAL_size_t(8, cetl_vec_capacity(vec));
  TEST_ASSERT_EQUAL_PTR(type, cetl_vec_get_type(vec));

  cetl_vec_free(vec);
}

cetl_void_t test_create_empty_type_int() { check_empty(int_type); }

cetl_void_t test_create_empty_type_double() { check_empty(double_type); }

cetl_void_t test_create_empty_type_char() { check_empty(char_type); }

cetl_void_t test_create_empty_type_struct() { check_empty(struct_type); }

cetl_void_t test_create_empty_type_heap_struct() {
  check_empty(heap_struct_type);
}

// ********************

// ********** Create copy **********

typedef cetl_void_t (*vec_filler)(cetl_vector *vec, cetl_size_t size);

cetl_void_t check_copy(const cetl_element *type, vec_filler filler, cetl_size_t size) {

  cetl_vector *source = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(source);

  filler(source, size);

  cetl_vector *copy = cetl_vec_create_copy(source);

  TEST_ASSERT_NOT_NULL(cetl_vec_data(copy));
  TEST_ASSERT_EQUAL_size_t(cetl_vec_size(source), cetl_vec_size(copy));
  TEST_ASSERT_EQUAL_size_t(cetl_vec_capacity(source), cetl_vec_capacity(copy));
  TEST_ASSERT_EQUAL_PTR(cetl_vec_get_type(source), cetl_vec_get_type(copy));

  cetl_vec_free(source);
  cetl_vec_free(copy);
}

cetl_void_t test_create_copy_type_int_size_1() {
  check_copy(int_type, fill_int_vector, 1);
}

cetl_void_t test_create_copy_type_double_size_1() {
  check_copy(double_type, fill_double_vector, 1);
}

cetl_void_t test_create_copy_type_char_size_1() {
  check_copy(char_type, fill_char_vector, 1);
}

cetl_void_t test_create_copy_type_struct_size_1() {
  check_copy(struct_type, fill_struct_vector, 1);
}

cetl_void_t test_create_copy_type_heap_struct_size_1() {
  check_copy(heap_struct_type, fill_heap_struct_vector, 1);
}

cetl_void_t test_create_copy_type_int_size_100() {
  check_copy(int_type, fill_int_vector, 100);
}

cetl_void_t test_create_copy_type_double_size_100() {
  check_copy(double_type, fill_double_vector, 100);
}

cetl_void_t test_create_copy_type_char_size_100() {
  check_copy(char_type, fill_char_vector, 100);
}

cetl_void_t test_create_copy_type_struct_size_100() {
  check_copy(struct_type, fill_struct_vector, 100);
}

cetl_void_t test_create_copy_type_heap_struct_size_100() {
  check_copy(heap_struct_type, fill_heap_struct_vector, 100);
}

//
int main() {

  UNITY_BEGIN();

  puts("\nCreate empty\n");

  RUN_TEST(test_create_empty_type_int);
  RUN_TEST(test_create_empty_type_double);
  RUN_TEST(test_create_empty_type_char);
  RUN_TEST(test_create_empty_type_struct);
  RUN_TEST(test_create_empty_type_heap_struct);

  puts("\nCreate copy\n");

  RUN_TEST(test_create_copy_type_int_size_1);
  RUN_TEST(test_create_copy_type_double_size_1);
  RUN_TEST(test_create_copy_type_char_size_1);
  RUN_TEST(test_create_copy_type_struct_size_1);
  RUN_TEST(test_create_copy_type_heap_struct_size_1);

  RUN_TEST(test_create_copy_type_int_size_100);
  RUN_TEST(test_create_copy_type_double_size_100);
  RUN_TEST(test_create_copy_type_char_size_100);
  RUN_TEST(test_create_copy_type_struct_size_100);
  RUN_TEST(test_create_copy_type_heap_struct_size_100);

  return UNITY_END();
}
