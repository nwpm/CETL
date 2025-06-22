#include "../../include/cstl/vector.h"
#include "../../include/external/unity/unity.h"
#include <stdio.h>
#include <stdlib.h>

void setUp() {}
void tearDown() {}

void test_vec_create_capacity_zero() {
  cstl_vector *v = vec_create(0, sizeof(int));
  TEST_ASSERT_NULL(v);
}

void test_vec_create_capacity_one() {
  cstl_vector *v = vec_create(1, sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  vec_free(v);
  free(v);
}

void test_vec_create_capacity_ten() {
  cstl_vector *v = vec_create(10, sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  vec_free(v);
  free(v);
}

void test_vec_create_capacity_1kk() {
  cstl_vector *v = vec_create(10, sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  vec_free(v);
  free(v);
}

void test_vec_create_elem_size_one() {
  cstl_vector *v = vec_create(1, 1);
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(1, v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  vec_free(v);
  free(v);
}

void test_vec_create_elem_size_double() {
  cstl_vector *v = vec_create(1, sizeof(double));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(double), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  vec_free(v);
  free(v);
}

void test_vec_create_elem_size_struct() {

  typedef struct Test {

    int x;
    int y;
    double z;
    char c;

  } Test;

  cstl_vector *v = vec_create(1, sizeof(Test));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(Test), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  vec_free(v);
  free(v);
}

void test_create_filled_capacity_zero() {

  int init_val = 1;
  int *init_val_ptr = &init_val;

  cstl_vector *v = vec_create_filled(0, sizeof(int), init_val_ptr);

  TEST_ASSERT_NULL(v);
}

void test_create_filled_capacity_one() {

  int init_val = 1;
  void *init_val_ptr = &init_val;

  cstl_vector *v = vec_create_filled(1, sizeof(int), init_val_ptr);

  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(1, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);
  TEST_ASSERT_EQUAL(1, *((int *)v->data));

  vec_free(v);
  free(v);
}

void test_create_filled_capacity_ten() {

  int init_val = 42;
  int *init_val_ptr = &init_val;

  cstl_vector *v = vec_create_filled(10, sizeof(int), init_val_ptr);

  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(10, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL(42, *((int *)v->data + i));
  }

  vec_free(v);
  free(v);
}

void test_vec_create_copy_from_empty() {

  cstl_vector *v = vec_create(1, sizeof(int));
  cstl_vector *v_copy = vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(v_copy);
  TEST_ASSERT_NULL(v_copy->data);

  TEST_ASSERT_EQUAL(0, v_copy->size);
  TEST_ASSERT_EQUAL(sizeof(int), v_copy->elem_size);
  TEST_ASSERT_EQUAL(1, v_copy->capacity);

  free(v);
  free(v_copy);
}

void test_vec_create_copy_from_size_one() {

  int val = 5;
  void *val_ptr = &val;

  cstl_vector *v = vec_create_filled(1, sizeof(int), val_ptr);
  cstl_vector *v_copy = vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(v_copy);
  TEST_ASSERT_NOT_NULL(v_copy->data);
  TEST_ASSERT_NOT_EQUAL(v->data, v_copy->data);

  TEST_ASSERT_EQUAL(1, v_copy->size);
  TEST_ASSERT_EQUAL(sizeof(int), v_copy->elem_size);
  TEST_ASSERT_EQUAL(1, v_copy->capacity);
  TEST_ASSERT_EQUAL(5, *((int *)v_copy->data));

  vec_free(v);
  vec_free(v_copy);

  free(v);
  free(v_copy);
}

void test_vec_create_copy_from_size_ten() {

  int val = 2;
  void *val_ptr = &val;

  cstl_vector *v = vec_create_filled(10, sizeof(int), val_ptr);
  cstl_vector *v_copy = vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(v_copy);
  TEST_ASSERT_NOT_NULL(v_copy->data);
  TEST_ASSERT_NOT_EQUAL(v->data, v_copy->data);

  TEST_ASSERT_EQUAL(10, v_copy->size);
  TEST_ASSERT_EQUAL(sizeof(int), v_copy->elem_size);
  TEST_ASSERT_EQUAL(10, v_copy->capacity);
  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL(2, *((int *)v->data + i));
  }

  vec_free(v);
  vec_free(v_copy);

  free(v);
  free(v_copy);
}

int main() {

  UNITY_BEGIN();

  RUN_TEST(test_vec_create_capacity_zero);
  RUN_TEST(test_vec_create_capacity_one);
  RUN_TEST(test_vec_create_capacity_ten);
  RUN_TEST(test_vec_create_capacity_1kk);

  printf("\n");

  RUN_TEST(test_vec_create_elem_size_one);
  RUN_TEST(test_vec_create_elem_size_double);
  RUN_TEST(test_vec_create_elem_size_struct);

  printf("\n");

  RUN_TEST(test_create_filled_capacity_zero);
  RUN_TEST(test_create_filled_capacity_one);
  RUN_TEST(test_create_filled_capacity_ten);

  printf("\n");

  RUN_TEST(test_vec_create_copy_from_empty);
  RUN_TEST(test_vec_create_copy_from_size_one);
  RUN_TEST(test_vec_create_copy_from_size_ten);

  return UNITY_END();
}
