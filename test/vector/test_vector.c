#include "../../include/cetl/cetl_vector.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include "../../src/utils/element/cetl_element.h"
#include "../../src/utils/iterator/iterator.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TestStruct {
  cetl_int_t x;
  cetl_int_t y;
} TestStruct;

typedef struct TestHeapStruct {
  cetl_str_t data;
} TestHeapStruct;

typedef cetl_void_t (*vec_insert_checker)(const cetl_vector *vec,
                                          cetl_size_t insert_pos,
                                          cetl_size_t insert_size);
typedef cetl_void_t (*vec_checker)(const cetl_vector *vec, cetl_size_t size);
typedef cetl_void_t (*vec_remover)(cetl_vector *vec, cetl_size_t size);

static cetl_element *int_type = NULL;
static cetl_element *double_type = NULL;
static cetl_element *char_type = NULL;
static cetl_element *struct_type = NULL;
static cetl_element *heap_struct_type = NULL;

static int filler_int_val = 1;
static double filler_double_val = 5.;
static char filler_char_val = 4;
static TestStruct filler_struct_val = {1, 2};
static TestHeapStruct *filler_heap_struct_val = NULL;

static int insert_int_val = 10;

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

cetl_void_t fill_vec_with(cetl_vector *vec, cetl_size_t size,
                          cetl_ptr_t fill_value) {
  for (cetl_size_t i = 0; i < size; ++i)
    cetl_vec_push_back(vec, fill_value);
}

// ********************

// ********** Data checker **********

cetl_void_t int_vector_checker(const cetl_vector *vec, cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    TEST_ASSERT_EQUAL_INT(filler_int_val, *((int *)cetl_vec_get(vec, i)));
  }

  TEST_ASSERT_EQUAL_size_t(size, cetl_vec_size(vec));
}

cetl_void_t double_vector_checker(const cetl_vector *vec, cetl_size_t size) {

  // TODO: don't work double test!
  for (cetl_size_t i = 0; i < size; ++i) {
    TEST_ASSERT_EQUAL_INT(filler_double_val, *((double *)cetl_vec_get(vec, i)));
  }

  TEST_ASSERT_EQUAL_size_t(size, cetl_vec_size(vec));
}

cetl_void_t char_vector_checker(const cetl_vector *vec, cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    TEST_ASSERT_EQUAL_CHAR(filler_char_val, *((char *)cetl_vec_get(vec, i)));
  }

  TEST_ASSERT_EQUAL_size_t(size, cetl_vec_size(vec));
}

cetl_void_t struct_vector_checker(const cetl_vector *vec, cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    TEST_ASSERT_EQUAL_INT(filler_struct_val.x,
                          ((TestStruct *)cetl_vec_get(vec, i))->x);
    TEST_ASSERT_EQUAL_INT(filler_struct_val.y,
                          ((TestStruct *)cetl_vec_get(vec, i))->y);
  }

  TEST_ASSERT_EQUAL_size_t(size, cetl_vec_size(vec));
}

cetl_void_t heap_struct_vector_checker(const cetl_vector *vec,
                                       cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    TEST_ASSERT_EQUAL_STRING(filler_heap_struct_val->data,
                             ((TestHeapStruct *)cetl_vec_get(vec, i))->data);
  }

  TEST_ASSERT_EQUAL_size_t(size, cetl_vec_size(vec));
}

cetl_void_t empty_data_checker(const cetl_vector *vec, cetl_size_t size) {

  TEST_ASSERT_EQUAL_size_t(size, cetl_vec_size(vec));
  TEST_ASSERT_NULL(cetl_vec_data(vec));
}

// ********************

// ********** Insert checker **********

cetl_void_t insert_int_checker(const cetl_vector *vec, cetl_size_t insert_pos,
                               cetl_size_t insert_size) {

  cetl_size_t vec_size = cetl_vec_size(vec);

  TEST_ASSERT_TRUE_MESSAGE(
      vec_size >= insert_pos + insert_size,
      "Vector size is smaller than insert position + insert size");

  cetl_size_t end_size = vec_size - (insert_pos + insert_size);

  for (cetl_size_t j = 0; j < insert_pos; ++j) {
    int *val = (int *)cetl_vec_get(vec, j);
    TEST_ASSERT_NOT_NULL(val);
    TEST_ASSERT_EQUAL_INT(filler_int_val, *val);
  }

  for (cetl_size_t j = 0; j < insert_size; ++j) {
    int *val = (int *)cetl_vec_get(vec, insert_pos + j);
    TEST_ASSERT_NOT_NULL(val);
    TEST_ASSERT_EQUAL_INT(insert_int_val, *val);
  }

  for (cetl_size_t j = 0; j < end_size; ++j) {
    int *val = (int *)cetl_vec_get(vec, insert_pos + insert_size + j);
    TEST_ASSERT_NOT_NULL(val);
    TEST_ASSERT_EQUAL_INT(filler_int_val, *val);
  }
}

// ********************

// ********** Remover **********

cetl_void_t vector_remover(cetl_vector *vec, cetl_size_t remove_size) {

  for (cetl_size_t i = 0; i < remove_size; ++i) {
    cetl_vec_pop_back(vec);
  }
}

// *******************

void setUp() {

  int_type = create_int_type();
  double_type = create_double_type();
  char_type = create_char_type();
  struct_type = create_struct_type();
  heap_struct_type = create_heap_struct_type();

  filler_heap_struct_val = malloc(sizeof(TestHeapStruct));
  filler_heap_struct_val->data = "string";
}

void tearDown() {

  free(int_type);
  free(double_type);
  free(char_type);
  free(struct_type);
  free(heap_struct_type);

  free(filler_heap_struct_val);
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

cetl_void_t check_copy(const cetl_element *type, cetl_ptr_t fill_value,
                       cetl_size_t size) {

  cetl_vector *source = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(source);

  fill_vec_with(source, size, fill_value);

  cetl_vector *copy = cetl_vec_create_copy(source);

  TEST_ASSERT_NOT_NULL(cetl_vec_data(copy));
  TEST_ASSERT_EQUAL_size_t(cetl_vec_size(source), cetl_vec_size(copy));
  TEST_ASSERT_EQUAL_size_t(cetl_vec_capacity(source), cetl_vec_capacity(copy));
  TEST_ASSERT_EQUAL_PTR(cetl_vec_get_type(source), cetl_vec_get_type(copy));

  cetl_vec_free(source);
  cetl_vec_free(copy);
}

cetl_void_t test_create_copy_type_int_size_1() {
  check_copy(int_type, &filler_int_val, 1);
}

cetl_void_t test_create_copy_type_double_size_1() {
  check_copy(double_type, &filler_double_val, 1);
}

cetl_void_t test_create_copy_type_char_size_1() {
  check_copy(char_type, &filler_char_val, 1);
}

cetl_void_t test_create_copy_type_struct_size_1() {
  check_copy(struct_type, &filler_struct_val, 1);
}

cetl_void_t test_create_copy_type_heap_struct_size_1() {
  check_copy(heap_struct_type, filler_heap_struct_val, 1);
}

cetl_void_t test_create_copy_type_int_size_100() {
  check_copy(int_type, &filler_int_val, 100);
}

cetl_void_t test_create_copy_type_double_size_100() {
  check_copy(double_type, &filler_double_val, 100);
}

cetl_void_t test_create_copy_type_char_size_100() {
  check_copy(char_type, &filler_char_val, 100);
}

cetl_void_t test_create_copy_type_struct_size_100() {
  check_copy(struct_type, &filler_struct_val, 100);
}

cetl_void_t test_create_copy_type_heap_struct_size_100() {
  check_copy(heap_struct_type, filler_heap_struct_val, 100);
}

//

// ********** Resize **********

cetl_void_t check_resize(const cetl_element *type, cetl_ptr_t fill_value,
                         cetl_size_t start_size, cetl_size_t new_capacity,
                         vec_checker checker, cetl_size_t expected_size) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  fill_vec_with(vec, start_size, fill_value);

  TEST_ASSERT_NOT_NULL(cetl_vec_resize(vec, new_capacity));
  TEST_ASSERT_EQUAL_size_t(new_capacity, cetl_vec_capacity(vec));

  checker(vec, expected_size);

  cetl_vec_free(vec);
}

// Int
cetl_void_t test_resize_type_int_start_size_0_new_capacity_0() {
  check_resize(int_type, &filler_int_val, 0, 0, empty_data_checker, 0);
}

cetl_void_t test_resize_type_int_start_size_10_new_capacity_5() {
  check_resize(int_type, &filler_int_val, 10, 5, int_vector_checker, 5);
}

cetl_void_t test_resize_type_int_start_size_10_new_capacity_10() {
  check_resize(int_type, &filler_int_val, 10, 10, int_vector_checker, 10);
}

cetl_void_t test_resize_type_int_start_size_10_new_capacity_15() {
  check_resize(int_type, &filler_int_val, 10, 15, int_vector_checker, 10);
}

cetl_void_t test_resize_type_int_start_size_1000_new_capacity_250() {
  check_resize(int_type, &filler_int_val, 1000, 250, int_vector_checker, 250);
}

cetl_void_t test_resize_type_int_start_size_1000_new_capacity_2000() {
  check_resize(int_type, &filler_int_val, 1000, 2000, int_vector_checker, 1000);
}

// Double
cetl_void_t test_resize_type_double_start_size_10_new_capacity_5() {
  check_resize(double_type, &filler_double_val, 10, 5, double_vector_checker,
               5);
}

cetl_void_t test_resize_type_double_start_size_10_new_capacity_10() {
  check_resize(double_type, &filler_double_val, 10, 10, double_vector_checker,
               10);
}

cetl_void_t test_resize_type_double_start_size_10_new_capacity_15() {
  check_resize(double_type, &filler_double_val, 10, 15, double_vector_checker,
               10);
}

cetl_void_t test_resize_type_double_start_size_1000_new_capacity_250() {
  check_resize(double_type, &filler_double_val, 1000, 250,
               double_vector_checker, 250);
}

cetl_void_t test_resize_type_double_start_size_1000_new_capacity_2000() {
  check_resize(double_type, &filler_double_val, 1000, 2000,
               double_vector_checker, 1000);
}

// Struct
cetl_void_t test_resize_type_struct_start_size_10_new_capacity_5() {
  check_resize(struct_type, &filler_struct_val, 10, 5, struct_vector_checker,
               5);
}

cetl_void_t test_resize_type_struct_start_size_10_new_capacity_10() {
  check_resize(struct_type, &filler_struct_val, 10, 10, struct_vector_checker,
               10);
}

cetl_void_t test_resize_type_struct_start_size_10_new_capacity_15() {
  check_resize(struct_type, &filler_struct_val, 10, 15, struct_vector_checker,
               10);
}

cetl_void_t test_resize_type_struct_start_size_1000_new_capacity_250() {
  check_resize(struct_type, &filler_struct_val, 1000, 250,
               struct_vector_checker, 250);
}

cetl_void_t test_resize_type_struct_start_size_1000_new_capacity_2000() {
  check_resize(struct_type, &filler_struct_val, 1000, 2000,
               struct_vector_checker, 1000);
}

// Heap Struct
cetl_void_t test_resize_type_heap_struct_start_size_10_new_capacity_5() {
  check_resize(heap_struct_type, filler_heap_struct_val, 10, 5,
               heap_struct_vector_checker, 5);
}

cetl_void_t test_resize_type_heap_struct_start_size_10_new_capacity_10() {
  check_resize(heap_struct_type, filler_heap_struct_val, 10, 10,
               heap_struct_vector_checker, 10);
}

cetl_void_t test_resize_type_heap_struct_start_size_10_new_capacity_15() {
  check_resize(heap_struct_type, filler_heap_struct_val, 10, 15,
               heap_struct_vector_checker, 10);
}

cetl_void_t test_resize_type_heap_struct_start_size_1000_new_capacity_250() {
  check_resize(heap_struct_type, filler_heap_struct_val, 1000, 250,
               heap_struct_vector_checker, 250);
}

cetl_void_t test_resize_type_heap_struct_start_size_1000_new_capacity_2000() {
  check_resize(heap_struct_type, filler_heap_struct_val, 1000, 2000,
               heap_struct_vector_checker, 1000);
}

// ********************

// ********** Push Back **********

cetl_void_t check_push_back(const cetl_element *type, cetl_size_t start_size,
                            cetl_size_t end_size, cetl_ptr_t fill_value,
                            vec_checker checker, cetl_size_t expected_size) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  fill_vec_with(vec, start_size, fill_value);
  fill_vec_with(vec, end_size, fill_value);
  checker(vec, expected_size);

  cetl_vec_free(vec);
}

// Int

cetl_void_t test_push_back_type_int_start_size_0_push_0_elems() {
  check_push_back(int_type, 0, 0, &filler_int_val, int_vector_checker, 0);
}

cetl_void_t test_push_back_type_int_start_size_0_push_1_elems() {
  check_push_back(int_type, 0, 1, &filler_int_val, int_vector_checker, 1);
}

cetl_void_t test_push_back_type_int_start_size_0_push_1000_elems() {
  check_push_back(int_type, 0, 1000, &filler_int_val, int_vector_checker, 1000);
}

cetl_void_t test_push_back_type_int_start_size_10_push_0_elems() {
  check_push_back(int_type, 10, 0, &filler_int_val, int_vector_checker, 10);
}

cetl_void_t test_push_back_type_int_start_size_10_push_100_elems() {
  check_push_back(int_type, 10, 100, &filler_int_val, int_vector_checker, 110);
}

// Double

cetl_void_t test_push_back_type_double_start_size_0_push_0_elems() {
  check_push_back(double_type, 0, 0, &filler_double_val, double_vector_checker,
                  0);
}

cetl_void_t test_push_back_type_double_start_size_0_push_1_elems() {
  check_push_back(double_type, 0, 1, &filler_double_val, double_vector_checker,
                  1);
}

cetl_void_t test_push_back_type_double_start_size_0_push_1000_elems() {
  check_push_back(double_type, 0, 1000, &filler_double_val,
                  double_vector_checker, 1000);
}

cetl_void_t test_push_back_type_double_start_size_10_push_0_elems() {
  check_push_back(double_type, 10, 0, &filler_double_val, double_vector_checker,
                  10);
}

cetl_void_t test_push_back_type_double_start_size_10_push_100_elems() {
  check_push_back(double_type, 10, 100, &filler_double_val,
                  double_vector_checker, 110);
}

// Char

cetl_void_t test_push_back_type_char_start_size_0_push_0_elems() {
  check_push_back(char_type, 0, 0, &filler_char_val, char_vector_checker, 0);
}

cetl_void_t test_push_back_type_char_start_size_0_push_1_elems() {
  check_push_back(char_type, 0, 1, &filler_char_val, char_vector_checker, 1);
}

cetl_void_t test_push_back_type_char_start_size_0_push_1000_elems() {
  check_push_back(char_type, 0, 1000, &filler_char_val, char_vector_checker,
                  1000);
}

cetl_void_t test_push_back_type_char_start_size_10_push_0_elems() {
  check_push_back(char_type, 10, 0, &filler_char_val, char_vector_checker, 10);
}

cetl_void_t test_push_back_type_char_start_size_10_push_100_elems() {
  check_push_back(char_type, 10, 100, &filler_char_val, char_vector_checker,
                  110);
}

// Struct

cetl_void_t test_push_back_type_struct_start_size_0_push_0_elems() {
  check_push_back(struct_type, 0, 0, &filler_struct_val, struct_vector_checker,
                  0);
}

cetl_void_t test_push_back_type_struct_start_size_0_push_1_elems() {
  check_push_back(struct_type, 0, 1, &filler_struct_val, struct_vector_checker,
                  1);
}

cetl_void_t test_push_back_type_struct_start_size_0_push_1000_elems() {
  check_push_back(struct_type, 0, 1000, &filler_struct_val,
                  struct_vector_checker, 1000);
}

cetl_void_t test_push_back_type_struct_start_size_10_push_0_elems() {
  check_push_back(struct_type, 10, 0, &filler_struct_val, struct_vector_checker,
                  10);
}

cetl_void_t test_push_back_type_struct_start_size_10_push_100_elems() {
  check_push_back(struct_type, 10, 100, &filler_struct_val,
                  struct_vector_checker, 110);
}

// Heap Struct

cetl_void_t test_push_back_type_heap_struct_start_size_0_push_0_elems() {
  check_push_back(heap_struct_type, 0, 0, filler_heap_struct_val,
                  heap_struct_vector_checker, 0);
}

cetl_void_t test_push_back_type_heap_struct_start_size_0_push_1_elems() {
  check_push_back(heap_struct_type, 0, 1, filler_heap_struct_val,
                  heap_struct_vector_checker, 1);
}

cetl_void_t test_push_back_type_heap_struct_start_size_0_push_1000_elems() {
  check_push_back(heap_struct_type, 0, 1000, filler_heap_struct_val,
                  heap_struct_vector_checker, 1000);
}

cetl_void_t test_push_back_type_heap_struct_start_size_10_push_0_elems() {
  check_push_back(heap_struct_type, 10, 0, filler_heap_struct_val,
                  heap_struct_vector_checker, 10);
}

cetl_void_t test_push_back_type_heap_struct_start_size_10_push_100_elems() {
  check_push_back(heap_struct_type, 10, 100, filler_heap_struct_val,
                  heap_struct_vector_checker, 110);
}

// ********************

// ********** Pop back **********

cetl_void_t check_pop_back(const cetl_element *type, cetl_size_t start_size,
                           cetl_size_t remove_size, cetl_ptr_t fill_value,
                           vec_checker checker, cetl_size_t expected_size) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  fill_vec_with(vec, start_size, fill_value);
  vector_remover(vec, remove_size);
  checker(vec, expected_size);

  cetl_vec_free(vec);
}

// Int

cetl_void_t test_pop_back_type_int_start_size_0_pop_0_elems() {
  check_pop_back(int_type, 0, 0, &filler_int_val, int_vector_checker, 0);
}

cetl_void_t test_pop_back_type_int_start_size_0_pop_1_elems() {
  check_pop_back(int_type, 0, 1, &filler_int_val, int_vector_checker, 0);
}

cetl_void_t test_pop_back_type_int_start_size_1_pop_1_elems() {
  check_pop_back(int_type, 1, 1, &filler_int_val, int_vector_checker, 0);
}

cetl_void_t test_pop_back_type_int_start_size_10_pop_1_elems() {
  check_pop_back(int_type, 10, 1, &filler_int_val, int_vector_checker, 9);
}

cetl_void_t test_pop_back_type_int_start_size_100_pop_100_elems() {
  check_pop_back(int_type, 100, 100, &filler_int_val, int_vector_checker, 0);
}

cetl_void_t test_pop_back_type_int_start_size_100_pop_1000_elems() {
  check_pop_back(int_type, 100, 1000, &filler_int_val, int_vector_checker, 0);
}

// Double

cetl_void_t test_pop_back_type_double_start_size_0_pop_0_elems() {
  check_pop_back(double_type, 0, 0, &filler_double_val, double_vector_checker,
                 0);
}

cetl_void_t test_pop_back_type_double_start_size_0_pop_1_elems() {
  check_pop_back(double_type, 0, 1, &filler_double_val, double_vector_checker,
                 0);
}

cetl_void_t test_pop_back_type_double_start_size_1_pop_1_elems() {
  check_pop_back(double_type, 1, 1, &filler_double_val, double_vector_checker,
                 0);
}

cetl_void_t test_pop_back_type_double_start_size_10_pop_1_elems() {
  check_pop_back(double_type, 10, 1, &filler_double_val, double_vector_checker,
                 9);
}

cetl_void_t test_pop_back_type_double_start_size_100_pop_100_elems() {
  check_pop_back(double_type, 100, 100, &filler_double_val,
                 double_vector_checker, 0);
}

cetl_void_t test_pop_back_type_double_start_size_100_pop_1000_elems() {
  check_pop_back(double_type, 100, 1000, &filler_double_val,
                 double_vector_checker, 0);
}

// Struct

cetl_void_t test_pop_back_type_struct_start_size_0_pop_0_elems() {
  check_pop_back(struct_type, 0, 0, &filler_struct_val, struct_vector_checker,
                 0);
}

cetl_void_t test_pop_back_type_struct_start_size_0_pop_1_elems() {
  check_pop_back(struct_type, 0, 1, &filler_struct_val, struct_vector_checker,
                 0);
}

cetl_void_t test_pop_back_type_struct_start_size_1_pop_1_elems() {
  check_pop_back(struct_type, 1, 1, &filler_struct_val, struct_vector_checker,
                 0);
}

cetl_void_t test_pop_back_type_struct_start_size_10_pop_1_elems() {
  check_pop_back(struct_type, 10, 1, &filler_struct_val, struct_vector_checker,
                 9);
}

cetl_void_t test_pop_back_type_struct_start_size_100_pop_100_elems() {
  check_pop_back(struct_type, 100, 100, &filler_struct_val,
                 struct_vector_checker, 0);
}

cetl_void_t test_pop_back_type_struct_start_size_100_pop_1000_elems() {
  check_pop_back(struct_type, 100, 1000, &filler_struct_val,
                 struct_vector_checker, 0);
}

// Heap Struct

cetl_void_t test_pop_back_type_heap_struct_start_size_0_pop_0_elems() {
  check_pop_back(heap_struct_type, 0, 0, filler_heap_struct_val,
                 heap_struct_vector_checker, 0);
}

cetl_void_t test_pop_back_type_heap_struct_start_size_0_pop_1_elems() {
  check_pop_back(heap_struct_type, 0, 1, filler_heap_struct_val,
                 heap_struct_vector_checker, 0);
}

cetl_void_t test_pop_back_type_heap_struct_start_size_1_pop_1_elems() {
  check_pop_back(heap_struct_type, 1, 1, filler_heap_struct_val,
                 heap_struct_vector_checker, 0);
}

cetl_void_t test_pop_back_type_heap_struct_start_size_10_pop_1_elems() {
  check_pop_back(heap_struct_type, 10, 1, filler_heap_struct_val,
                 heap_struct_vector_checker, 9);
}

cetl_void_t test_pop_back_type_heap_struct_start_size_100_pop_100_elems() {
  check_pop_back(heap_struct_type, 100, 100, filler_heap_struct_val,
                 heap_struct_vector_checker, 0);
}

cetl_void_t test_pop_back_type_heap_struct_start_size_100_pop_1000_elems() {
  check_pop_back(heap_struct_type, 100, 1000, filler_heap_struct_val,
                 heap_struct_vector_checker, 0);
}

// ********************

// ********** Shrink to fit **********

cetl_void_t check_shrink_to_fit(const cetl_element *type,
                                cetl_size_t start_size, cetl_ptr_t fill_value,
                                cetl_size_t expected_capacity) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  fill_vec_with(vec, start_size, fill_value);

  cetl_vec_shrink_to_fit(vec);

  TEST_ASSERT_EQUAL_size_t(expected_capacity, cetl_vec_capacity(vec));

  cetl_vec_free(vec);
}

// Int

cetl_void_t test_shrink_to_fit_type_int_size_0() {
  check_shrink_to_fit(int_type, 0, &filler_int_val, 0);
}

cetl_void_t test_shrink_to_fit_type_int_size_1() {
  check_shrink_to_fit(int_type, 1, &filler_int_val, 1);
}

cetl_void_t test_shrink_to_fit_type_int_size_100() {
  check_shrink_to_fit(int_type, 100, &filler_int_val, 100);
}

// Double

cetl_void_t test_shrink_to_fit_type_double_size_0() {
  check_shrink_to_fit(double_type, 0, &filler_double_val, 0);
}

cetl_void_t test_shrink_to_fit_type_double_size_1() {
  check_shrink_to_fit(double_type, 1, &filler_double_val, 1);
}

cetl_void_t test_shrink_to_fit_type_double_size_100() {
  check_shrink_to_fit(double_type, 100, &filler_double_val, 100);
}

// Struct

cetl_void_t test_shrink_to_fit_type_struct_size_0() {
  check_shrink_to_fit(struct_type, 0, &filler_struct_val, 0);
}

cetl_void_t test_shrink_to_fit_type_struct_size_1() {
  check_shrink_to_fit(struct_type, 1, &filler_struct_val, 1);
}

cetl_void_t test_shrink_to_fit_type_struct_size_100() {
  check_shrink_to_fit(struct_type, 100, &filler_struct_val, 100);
}

// Heap Struct

cetl_void_t test_shrink_to_fit_type_heap_struct_size_0() {
  check_shrink_to_fit(heap_struct_type, 0, filler_heap_struct_val, 0);
}

cetl_void_t test_shrink_to_fit_type_heap_struct_size_1() {
  check_shrink_to_fit(heap_struct_type, 1, filler_heap_struct_val, 1);
}

cetl_void_t test_shrink_to_fit_type_heap_struct_size_100() {
  check_shrink_to_fit(heap_struct_type, 100, filler_heap_struct_val, 100);
}

// ********************

// ********** Clear **********

cetl_void_t check_clear(const cetl_element *type, cetl_size_t start_size,
                        cetl_ptr_t fill_value) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  fill_vec_with(vec, start_size, fill_value);
  cetl_vec_clear(vec);

  TEST_ASSERT_EQUAL_size_t(0, cetl_vec_size(vec));
  TEST_ASSERT_NULL(cetl_vec_data(vec));

  cetl_vec_free(vec);
}

// Int

cetl_void_t test_clear_type_int_size_0() {
  check_clear(int_type, 0, &filler_int_val);
}

cetl_void_t test_clear_type_int_size_1() {
  check_clear(int_type, 1, &filler_int_val);
}

cetl_void_t test_clear_type_int_size_1000() {
  check_clear(int_type, 1000, &filler_int_val);
}

// Double

cetl_void_t test_clear_type_double_size_0() {
  check_clear(double_type, 0, &filler_double_val);
}

cetl_void_t test_clear_type_double_size_1() {
  check_clear(double_type, 1, &filler_double_val);
}

cetl_void_t test_clear_type_double_size_1000() {
  check_clear(double_type, 1000, &filler_double_val);
}

// Struct

cetl_void_t test_clear_type_struct_size_0() {
  check_clear(struct_type, 0, &filler_struct_val);
}

cetl_void_t test_clear_type_struct_size_1() {
  check_clear(struct_type, 1, &filler_struct_val);
}

cetl_void_t test_clear_type_struct_size_1000() {
  check_clear(struct_type, 1000, &filler_struct_val);
}

// Heap struct

cetl_void_t test_clear_type_heap_struct_size_0() {
  check_clear(heap_struct_type, 0, filler_heap_struct_val);
}

cetl_void_t test_clear_type_heap_struct_size_1() {
  check_clear(heap_struct_type, 1, filler_heap_struct_val);
}

cetl_void_t test_clear_type_heap_struct_size_1000() {
  check_clear(heap_struct_type, 1000, filler_heap_struct_val);
}

// ********************

// ********** Insert **********

cetl_void_t check_insert(const cetl_element *type, cetl_size_t start_size,
                         cetl_ptr_t fill_value, cetl_size_t insert_pos,
                         cetl_ptr_t insert_val, cetl_size_t insert_size,
                         vec_insert_checker checker) {

  cetl_vector *vec = cetl_vec_create_empty(type);
  TEST_ASSERT_NOT_NULL(vec);

  fill_vec_with(vec, start_size, fill_value);

  for (cetl_size_t i = 0; i < insert_size; ++i) {
    cetl_vec_insert(vec, insert_pos, insert_val);
  }

  cetl_size_t end_size = start_size + insert_size;
  TEST_ASSERT_EQUAL_size_t(end_size, cetl_vec_size(vec));

  checker(vec, insert_pos, insert_size);

  cetl_vec_free(vec);
}

cetl_void_t test_insert_type_int_init_size_0_insert_num_0() {
  check_insert(int_type, 0, &filler_int_val, 0, NULL, 0, insert_int_checker);
}

cetl_void_t test_insert_type_int_init_size_0_insert_num_1() {
  check_insert(int_type, 0, &filler_int_val, 0, &insert_int_val, 1,
               insert_int_checker);
}

cetl_void_t test_insert_type_int_init_size_1_insert_num_0() {
  check_insert(int_type, 1, &filler_int_val, 0, NULL, 0, insert_int_checker);
}

cetl_void_t test_insert_type_int_init_size_5_insert_num_1_pos_begin() {
  check_insert(int_type, 5, &filler_int_val, 0, &insert_int_val, 1,
               insert_int_checker);
}

cetl_void_t test_insert_type_int_init_size_5_insert_num_1_pos_end() {
  check_insert(int_type, 5, &filler_int_val, 5, &insert_int_val, 1,
               insert_int_checker);
}

cetl_void_t test_insert_type_int_init_size_5_insert_num_1_pos_middle() {
  check_insert(int_type, 5, &filler_int_val, 2, &insert_int_val, 1,
               insert_int_checker);
}

// ********************

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

  puts("\nResize\n");

  RUN_TEST(test_resize_type_int_start_size_0_new_capacity_0);
  RUN_TEST(test_resize_type_int_start_size_10_new_capacity_5);
  RUN_TEST(test_resize_type_int_start_size_10_new_capacity_10);
  RUN_TEST(test_resize_type_int_start_size_10_new_capacity_15);
  RUN_TEST(test_resize_type_int_start_size_1000_new_capacity_250);
  RUN_TEST(test_resize_type_int_start_size_1000_new_capacity_2000);

  RUN_TEST(test_resize_type_double_start_size_10_new_capacity_5);
  RUN_TEST(test_resize_type_double_start_size_10_new_capacity_10);
  RUN_TEST(test_resize_type_double_start_size_10_new_capacity_15);
  RUN_TEST(test_resize_type_double_start_size_1000_new_capacity_250);
  RUN_TEST(test_resize_type_double_start_size_1000_new_capacity_2000);

  RUN_TEST(test_resize_type_struct_start_size_10_new_capacity_5);
  RUN_TEST(test_resize_type_struct_start_size_10_new_capacity_10);
  RUN_TEST(test_resize_type_struct_start_size_10_new_capacity_15);
  RUN_TEST(test_resize_type_struct_start_size_1000_new_capacity_250);
  RUN_TEST(test_resize_type_struct_start_size_1000_new_capacity_2000);

  RUN_TEST(test_resize_type_heap_struct_start_size_10_new_capacity_5);
  RUN_TEST(test_resize_type_heap_struct_start_size_10_new_capacity_10);
  RUN_TEST(test_resize_type_heap_struct_start_size_10_new_capacity_15);
  RUN_TEST(test_resize_type_heap_struct_start_size_1000_new_capacity_250);
  RUN_TEST(test_resize_type_heap_struct_start_size_1000_new_capacity_2000);

  puts("\nPush back\n");

  RUN_TEST(test_push_back_type_int_start_size_0_push_0_elems);
  RUN_TEST(test_push_back_type_int_start_size_0_push_1_elems);
  RUN_TEST(test_push_back_type_int_start_size_0_push_1000_elems);
  RUN_TEST(test_push_back_type_int_start_size_10_push_0_elems);
  RUN_TEST(test_push_back_type_int_start_size_10_push_100_elems);

  RUN_TEST(test_push_back_type_double_start_size_0_push_0_elems);
  RUN_TEST(test_push_back_type_double_start_size_0_push_1_elems);
  RUN_TEST(test_push_back_type_double_start_size_0_push_1000_elems);
  RUN_TEST(test_push_back_type_double_start_size_10_push_0_elems);
  RUN_TEST(test_push_back_type_double_start_size_10_push_100_elems);

  RUN_TEST(test_push_back_type_char_start_size_0_push_0_elems);
  RUN_TEST(test_push_back_type_char_start_size_0_push_1_elems);
  RUN_TEST(test_push_back_type_char_start_size_0_push_1000_elems);
  RUN_TEST(test_push_back_type_char_start_size_10_push_0_elems);
  RUN_TEST(test_push_back_type_char_start_size_10_push_100_elems);

  RUN_TEST(test_push_back_type_struct_start_size_0_push_0_elems);
  RUN_TEST(test_push_back_type_struct_start_size_0_push_1_elems);
  RUN_TEST(test_push_back_type_struct_start_size_0_push_1000_elems);
  RUN_TEST(test_push_back_type_struct_start_size_10_push_0_elems);
  RUN_TEST(test_push_back_type_struct_start_size_10_push_100_elems);

  RUN_TEST(test_push_back_type_heap_struct_start_size_0_push_0_elems);
  RUN_TEST(test_push_back_type_heap_struct_start_size_0_push_1_elems);
  RUN_TEST(test_push_back_type_heap_struct_start_size_0_push_1000_elems);
  RUN_TEST(test_push_back_type_heap_struct_start_size_10_push_0_elems);
  RUN_TEST(test_push_back_type_heap_struct_start_size_10_push_100_elems);

  puts("\nPop back\n");

  RUN_TEST(test_pop_back_type_int_start_size_0_pop_0_elems);
  RUN_TEST(test_pop_back_type_int_start_size_0_pop_1_elems);
  RUN_TEST(test_pop_back_type_int_start_size_1_pop_1_elems);
  RUN_TEST(test_pop_back_type_int_start_size_10_pop_1_elems);
  RUN_TEST(test_pop_back_type_int_start_size_100_pop_100_elems);
  RUN_TEST(test_pop_back_type_int_start_size_100_pop_1000_elems);

  RUN_TEST(test_pop_back_type_double_start_size_0_pop_0_elems);
  RUN_TEST(test_pop_back_type_double_start_size_0_pop_1_elems);
  RUN_TEST(test_pop_back_type_double_start_size_1_pop_1_elems);
  RUN_TEST(test_pop_back_type_double_start_size_10_pop_1_elems);
  RUN_TEST(test_pop_back_type_double_start_size_100_pop_100_elems);
  RUN_TEST(test_pop_back_type_double_start_size_100_pop_1000_elems);

  RUN_TEST(test_pop_back_type_struct_start_size_0_pop_0_elems);
  RUN_TEST(test_pop_back_type_struct_start_size_0_pop_1_elems);
  RUN_TEST(test_pop_back_type_struct_start_size_1_pop_1_elems);
  RUN_TEST(test_pop_back_type_struct_start_size_10_pop_1_elems);
  RUN_TEST(test_pop_back_type_struct_start_size_100_pop_100_elems);
  RUN_TEST(test_pop_back_type_struct_start_size_100_pop_1000_elems);

  RUN_TEST(test_pop_back_type_heap_struct_start_size_0_pop_0_elems);
  RUN_TEST(test_pop_back_type_heap_struct_start_size_0_pop_1_elems);
  RUN_TEST(test_pop_back_type_heap_struct_start_size_1_pop_1_elems);
  RUN_TEST(test_pop_back_type_heap_struct_start_size_10_pop_1_elems);
  RUN_TEST(test_pop_back_type_heap_struct_start_size_100_pop_100_elems);
  RUN_TEST(test_pop_back_type_heap_struct_start_size_100_pop_1000_elems);

  puts("\nShrink to fit\n");

  RUN_TEST(test_shrink_to_fit_type_int_size_0);
  RUN_TEST(test_shrink_to_fit_type_int_size_1);
  RUN_TEST(test_shrink_to_fit_type_int_size_100);

  RUN_TEST(test_shrink_to_fit_type_double_size_0);
  RUN_TEST(test_shrink_to_fit_type_double_size_1);
  RUN_TEST(test_shrink_to_fit_type_double_size_100);

  RUN_TEST(test_shrink_to_fit_type_struct_size_0);
  RUN_TEST(test_shrink_to_fit_type_struct_size_1);
  RUN_TEST(test_shrink_to_fit_type_struct_size_100);

  RUN_TEST(test_shrink_to_fit_type_heap_struct_size_0);
  RUN_TEST(test_shrink_to_fit_type_heap_struct_size_1);
  RUN_TEST(test_shrink_to_fit_type_heap_struct_size_100);

  puts("\nClear\n");

  RUN_TEST(test_clear_type_int_size_0);
  RUN_TEST(test_clear_type_int_size_1);
  RUN_TEST(test_clear_type_int_size_1000);

  RUN_TEST(test_clear_type_double_size_0);
  RUN_TEST(test_clear_type_double_size_1);
  RUN_TEST(test_clear_type_double_size_1000);

  RUN_TEST(test_clear_type_struct_size_0);
  RUN_TEST(test_clear_type_struct_size_1);
  RUN_TEST(test_clear_type_struct_size_1000);

  RUN_TEST(test_clear_type_heap_struct_size_0);
  RUN_TEST(test_clear_type_heap_struct_size_1);
  RUN_TEST(test_clear_type_heap_struct_size_1000);

  puts("\nInsert\n");

  RUN_TEST(test_insert_type_int_init_size_0_insert_num_0);
  RUN_TEST(test_insert_type_int_init_size_0_insert_num_1);
  RUN_TEST(test_insert_type_int_init_size_1_insert_num_0);
  RUN_TEST(test_insert_type_int_init_size_5_insert_num_1_pos_begin);
  RUN_TEST(test_insert_type_int_init_size_5_insert_num_1_pos_middle);
  RUN_TEST(test_insert_type_int_init_size_5_insert_num_1_pos_end);

  return UNITY_END();
}
