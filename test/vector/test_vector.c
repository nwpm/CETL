#include "../../include/cstl/cstl_vector.h"
#include "../../include/external/unity/unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp() {}
void tearDown() {}

void test_cstl_vec_create_capacity_zero() {
  cstl_vector *v = cstl_vec_create(0, sizeof(int));
  TEST_ASSERT_NULL(v);
}

void test_cstl_vec_create_capacity_one() {
  cstl_vector *v = cstl_vec_create(1, sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_create_capacity_ten() {
  cstl_vector *v = cstl_vec_create(10, sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_create_capacity_1kk() {
  cstl_vector *v = cstl_vec_create(10, sizeof(int));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_create_elem_size_one() {
  cstl_vector *v = cstl_vec_create(1, 1);
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(1, v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_create_elem_size_double() {
  cstl_vector *v = cstl_vec_create(1, sizeof(double));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(double), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_create_elem_size_struct() {

  typedef struct Test {

    int x;
    int y;
    double z;
    char c;

  } Test;

  cstl_vector *v = cstl_vec_create(1, sizeof(Test));
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(0, v->size);
  TEST_ASSERT_EQUAL(sizeof(Test), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_create_filled_capacity_zero() {

  int init_val = 1;
  int *init_val_ptr = &init_val;

  cstl_vector *v = cstl_vec_create_filled(0, sizeof(int), init_val_ptr);

  TEST_ASSERT_NULL(v);
}

void test_create_filled_capacity_one() {

  int init_val = 1;
  void *init_val_ptr = &init_val;

  cstl_vector *v = cstl_vec_create_filled(1, sizeof(int), init_val_ptr);

  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(1, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(1, v->capacity);
  TEST_ASSERT_EQUAL(1, *((int *)v->data));

  cstl_vec_free(v);
  free(v);
}

void test_create_filled_capacity_ten() {

  int init_val = 42;
  int *init_val_ptr = &init_val;

  cstl_vector *v = cstl_vec_create_filled(10, sizeof(int), init_val_ptr);

  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_NOT_NULL(v->data);

  TEST_ASSERT_EQUAL(10, v->size);
  TEST_ASSERT_EQUAL(sizeof(int), v->elem_size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL(42, *((int *)v->data + i));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_create_copy_from_empty() {

  cstl_vector *v = cstl_vec_create(1, sizeof(int));
  cstl_vector *v_copy = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(v_copy);
  TEST_ASSERT_NOT_NULL(v_copy->data);

  TEST_ASSERT_EQUAL(0, v_copy->size);
  TEST_ASSERT_EQUAL(sizeof(int), v_copy->elem_size);
  TEST_ASSERT_EQUAL(1, v_copy->capacity);

  cstl_vec_free(v);
  cstl_vec_free(v_copy);
  free(v);
  free(v_copy);
}

void test_cstl_vec_create_copy_from_size_one() {

  int val = 5;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(1, sizeof(int), val_ptr);
  cstl_vector *v_copy = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(v_copy);
  TEST_ASSERT_NOT_NULL(v_copy->data);
  TEST_ASSERT_NOT_EQUAL(v->data, v_copy->data);

  TEST_ASSERT_EQUAL(1, v_copy->size);
  TEST_ASSERT_EQUAL(sizeof(int), v_copy->elem_size);
  TEST_ASSERT_EQUAL(1, v_copy->capacity);
  TEST_ASSERT_EQUAL(5, *((int *)v_copy->data));

  cstl_vec_free(v);
  cstl_vec_free(v_copy);

  free(v);
  free(v_copy);
}

void test_cstl_vec_create_copy_from_size_ten() {

  int val = 2;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(10, sizeof(int), val_ptr);
  cstl_vector *v_copy = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(v_copy);
  TEST_ASSERT_NOT_NULL(v_copy->data);
  TEST_ASSERT_NOT_EQUAL(v->data, v_copy->data);

  TEST_ASSERT_EQUAL(10, v_copy->size);
  TEST_ASSERT_EQUAL(sizeof(int), v_copy->elem_size);
  TEST_ASSERT_EQUAL(10, v_copy->capacity);
  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL(2, *((int *)v->data + i));
  }

  cstl_vec_free(v);
  cstl_vec_free(v_copy);

  free(v);
  free(v_copy);
}

void test_cstl_vec_front_size_zero() {

  cstl_vector *v = cstl_vec_create(1, sizeof(int));
  TEST_ASSERT_NULL(cstl_vec_front(v));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_front_size_greater_zero() {

  int val = 3;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(10, sizeof(int), val_ptr);
  TEST_ASSERT_EQUAL(3, *((int *)cstl_vec_front(v)));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_back_size_zero() {

  cstl_vector *v = cstl_vec_create(1, sizeof(int));
  TEST_ASSERT_NULL(cstl_vec_front(v));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_back_size_greater_zero() {

  float val = 7.f;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(float), val_ptr);
  TEST_ASSERT_EQUAL(7.f, *((float *)cstl_vec_back(v)));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_get_pos_zero() {

  short val = 9;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(3, sizeof(short), val_ptr);
  TEST_ASSERT_EQUAL(9, *((short *)cstl_vec_get(v, 0)));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_get_pos_last() {

  int val = 11;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(4, sizeof(int), val_ptr);
  TEST_ASSERT_EQUAL(11, *((short *)cstl_vec_get(v, v->size - 1)));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_get_pos_out_of_bound() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(7, sizeof(short), val_ptr);
  TEST_ASSERT_NULL(cstl_vec_get(v, 10));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_data_correct_ptr() {

  short val = 2;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(4, sizeof(short), val_ptr);
  TEST_ASSERT_NOT_NULL(v->data);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_set_pos_zero() {

  short val = 9;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(10, sizeof(short), val_ptr);

  short val_set = 10;
  void *val_set_ptr = &val_set;

  TEST_ASSERT_EQUAL(0, cstl_vec_set(v, 0, val_set_ptr));
  TEST_ASSERT_EQUAL(10, *((short *)cstl_vec_front(v)));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_set_pos_last() {

  short val = 4;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short val_set = 1;
  void *val_set_ptr = &val_set;

  TEST_ASSERT_EQUAL(0, cstl_vec_set(v, v->size - 1, val_set_ptr));
  TEST_ASSERT_EQUAL(1, *((short *)cstl_vec_back(v)));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_set_pos_out_of_bound() {

  short val = 4;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short val_set = 1;
  void *val_set_ptr = &val_set;

  TEST_ASSERT_EQUAL(-2, cstl_vec_set(v, 100, val_set_ptr));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_set_elem_val_null() {

  short val = 2;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  TEST_ASSERT_EQUAL(-1, cstl_vec_set(v, 0, NULL));

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_push_back_capacity_greater_size() {

  short val = 10;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create(10, sizeof(short));
  cstl_vec_push_back(v, val_ptr);

  TEST_ASSERT_EQUAL(10, *((short *)cstl_vec_front(v)));
  TEST_ASSERT_EQUAL(1, v->size);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_push_back_capacity_equal_size() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short push_val = 2;
  void *push_ptr_val = &push_val;

  cstl_vec_push_back(v, push_ptr_val);
  TEST_ASSERT_EQUAL(2, *((short *)cstl_vec_back(v)));
  TEST_ASSERT_EQUAL(6, v->size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_pop_back_size_zero() {

  cstl_vector *v = cstl_vec_create_empty();

  cstl_vec_pop_back(v);
  TEST_ASSERT_EQUAL(v, cstl_vec_pop_back(v));

  free(v);
}

void test_cstl_vec_pop_back_size_greater_zero() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_pop_back(v);
  TEST_ASSERT_EQUAL(4, v->size);
  TEST_ASSERT_EQUAL(5, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_resize_new_capacity_equal_old() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_resize(v, 5);

  TEST_ASSERT_EQUAL(5, v->size);
  TEST_ASSERT_EQUAL(5, v->capacity);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_resize_new_capacity_greater_old() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_resize(v, 10);

  TEST_ASSERT_EQUAL(5, v->size);
  TEST_ASSERT_EQUAL(10, v->capacity);

  int test_arr[] = {1, 1, 1, 1, 1};

  for (size_t i = 0; i < 5; ++i) {
    TEST_ASSERT_EQUAL(test_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_resize_new_capacity_less_old() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_resize(v, 2);

  TEST_ASSERT_EQUAL(2, v->size);
  TEST_ASSERT_EQUAL(2, v->capacity);

  int test_arr[] = {1, 1};

  for (size_t i = 0; i < 2; ++i) {
    TEST_ASSERT_EQUAL(test_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_shrink_to_fit() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_resize(v, 10);
  cstl_vec_shrink_to_fit(v);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(5, v->size);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_clear() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_clear(v);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(0, v->size);

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_pos_first() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short val_insert = 10;
  void *val_ptr_insert = &val_insert;

  cstl_vec_insert(v, 0, val_ptr_insert);

  TEST_ASSERT_EQUAL(10, v->capacity);
  TEST_ASSERT_EQUAL(6, v->size);

  short res_arr[] = {10, 1, 1, 1, 1, 1};

  for (size_t i = 0; i < 6; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_pos_last() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short val_insert = 10;
  void *val_ptr_insert = &val_insert;

  cstl_vec_insert(v, v->size, val_ptr_insert);

  TEST_ASSERT_EQUAL(10, v->capacity);
  TEST_ASSERT_EQUAL(6, v->size);

  short res_arr[] = {1, 1, 1, 1, 1, 10};

  for (size_t i = 0; i < 6; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_pos_middle() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short val_insert = 10;
  void *val_ptr_insert = &val_insert;

  cstl_vec_insert(v, v->size / 2, val_ptr_insert);

  TEST_ASSERT_EQUAL(10, v->capacity);
  TEST_ASSERT_EQUAL(6, v->size);

  short res_arr[] = {1, 1, 10, 1, 1, 1};

  for (size_t i = 0; i < 6; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_pos_out_of_bound() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short val_insert = 10;
  void *val_ptr_insert = &val_insert;

  TEST_ASSERT_NULL(cstl_vec_insert(v, 100, val_ptr_insert));
  TEST_ASSERT_EQUAL(5, v->size);
  TEST_ASSERT_EQUAL(5, v->capacity);

  short res_arr[] = {1, 1, 1, 1, 1};

  for (size_t i = 0; i < 5; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_pos_first() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_erase(v, 0);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(4, v->size);

  short res_arr[] = {1, 1, 1, 1};

  for (size_t i = 0; i < 4; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_pos_last() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_erase(v, v->size - 1);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(4, v->size);

  short res_arr[] = {1, 1, 1, 1};

  for (size_t i = 0; i < 4; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_pos_middle() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_erase(v, v->size / 2);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(4, v->size);

  short res_arr[] = {1, 1, 1, 1};

  for (size_t i = 0; i < 4; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_pos_out_of_bound() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  TEST_ASSERT_NULL(cstl_vec_erase(v, 100));
  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(5, v->size);

  short res_arr[] = {1, 1, 1, 1, 1};

  for (size_t i = 0; i < 5; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_range_size_zero() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  int range[] = {1, 2, 3, 4, 5};

  TEST_ASSERT_EQUAL(v, cstl_vec_insert_range(v, (void *)range, 0, 0));
  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(5, v->size);

  short res_arr[] = {1, 1, 1, 1, 1};

  for (size_t i = 0; i < 5; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_range_pos_first() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short range[] = {1, 2, 3, 4, 5};
  cstl_vec_insert_range(v, (void *)range, 5, 0);

  TEST_ASSERT_EQUAL(10, v->capacity);
  TEST_ASSERT_EQUAL(10, v->size);

  short res_arr[] = {1, 2, 3, 4, 5, 1, 1, 1, 1, 1};

  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_range_pos_last() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short range[] = {1, 2, 3, 4, 5, 6, 7};
  cstl_vec_insert_range(v, (void *)range, 7, v->size);

  TEST_ASSERT_EQUAL(20, v->capacity);
  TEST_ASSERT_EQUAL(12, v->size);

  short res_arr[] = {1, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6, 7};

  for (size_t i = 0; i < 12; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_range_pos_middle() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short range[] = {1, 2, 3, 4, 5};
  cstl_vec_insert_range(v, (void *)range, 5, v->size / 2);

  TEST_ASSERT_EQUAL(10, v->capacity);
  TEST_ASSERT_EQUAL(10, v->size);

  short res_arr[] = {1, 1, 1, 2, 3, 4, 5, 1, 1, 1};

  for (size_t i = 0; i < 10; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_insert_range_pos_out_of_bound() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  short range[] = {1, 2, 3, 4, 5};
  TEST_ASSERT_NULL(cstl_vec_insert_range(v, (void *)range, 5, 100));

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(5, v->size);

  short res_arr[] = {1, 1, 1, 1, 1};

  for (size_t i = 0; i < 5; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_range_len_zero() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  TEST_ASSERT_EQUAL(v, cstl_vec_erase_range(v, 1, 0));
  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(5, v->size);

  short res_arr[] = {1, 1, 1, 1, 1};

  for (size_t i = 0; i < 5; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_range_pos_plus_len_is_size() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_erase_range(v, 1, 4);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(1, v->size);

  short res_arr[] = {1};

  for (size_t i = 0; i < 1; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_range_pos_plus_len_less_size() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_erase_range(v, 2, 2);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(3, v->size);

  short res_arr[] = {1, 1, 1};

  for (size_t i = 0; i < 3; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_range_from_begin_to_middle() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  cstl_vec_erase_range(v, 0, 2);

  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(3, v->size);

  short res_arr[] = {1, 1, 1};

  for (size_t i = 0; i < 3; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_erase_range_incorrect() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(5, sizeof(short), val_ptr);

  TEST_ASSERT_NULL(cstl_vec_erase_range(v, 1, 20));
  TEST_ASSERT_EQUAL(5, v->capacity);
  TEST_ASSERT_EQUAL(5, v->size);

  short res_arr[] = {1, 1, 1, 1, 1};

  for (size_t i = 0; i < 5; ++i) {
    TEST_ASSERT_EQUAL(res_arr[i], *((short *)cstl_vec_get(v, i)));
  }

  cstl_vec_free(v);
  free(v);
}

void test_cstl_vec_is_empty_true() {

  cstl_vector *v = cstl_vec_create_empty(); 

  TEST_ASSERT_EQUAL(true, cstl_vec_is_empty(v));

  free(v);
}

void test_cstl_vec_is_empty_false() {

  short val = 1;
  void *val_ptr = &val;

  cstl_vector *v = cstl_vec_create_filled(1, sizeof(short), val_ptr);

  TEST_ASSERT_EQUAL(false, cstl_vec_is_empty(v));

  cstl_vec_free(v);
  free(v);
}

int main() {

  UNITY_BEGIN();

  printf("VEC_CREATE\n");

  RUN_TEST(test_cstl_vec_create_capacity_zero);
  RUN_TEST(test_cstl_vec_create_capacity_one);
  RUN_TEST(test_cstl_vec_create_capacity_ten);
  RUN_TEST(test_cstl_vec_create_capacity_1kk);

  printf("\n");

  RUN_TEST(test_cstl_vec_create_elem_size_one);
  RUN_TEST(test_cstl_vec_create_elem_size_double);
  RUN_TEST(test_cstl_vec_create_elem_size_struct);

  printf("VEC_CREATE_FILLED\n");

  RUN_TEST(test_create_filled_capacity_zero);
  RUN_TEST(test_create_filled_capacity_one);
  RUN_TEST(test_create_filled_capacity_ten);

  printf("VEC_CREATE_COPY\n");

  RUN_TEST(test_cstl_vec_create_copy_from_empty);
  RUN_TEST(test_cstl_vec_create_copy_from_size_one);
  RUN_TEST(test_cstl_vec_create_copy_from_size_ten);

  printf("VEC_FRONT\n");

  RUN_TEST(test_cstl_vec_front_size_zero);
  RUN_TEST(test_cstl_vec_front_size_greater_zero);

  printf("VEC_BACK\n");

  RUN_TEST(test_cstl_vec_back_size_zero);
  RUN_TEST(test_cstl_vec_back_size_greater_zero);

  printf("VEC_GET\n");

  RUN_TEST(test_cstl_vec_get_pos_zero);
  RUN_TEST(test_cstl_vec_get_pos_last);
  RUN_TEST(test_cstl_vec_get_pos_out_of_bound);

  printf("VEC_DATA\n");

  RUN_TEST(test_cstl_vec_data_correct_ptr);

  printf("VEC_SET\n");

  RUN_TEST(test_cstl_vec_set_pos_zero);
  RUN_TEST(test_cstl_vec_set_pos_last);
  RUN_TEST(test_cstl_vec_set_pos_out_of_bound);
  RUN_TEST(test_cstl_vec_set_elem_val_null);

  printf("VEC_PUSH_BACK\n");

  RUN_TEST(test_cstl_vec_push_back_capacity_greater_size);
  RUN_TEST(test_cstl_vec_push_back_capacity_equal_size);

  printf("VEC_POP_BACK\n");

  RUN_TEST(test_cstl_vec_pop_back_size_zero);
  RUN_TEST(test_cstl_vec_pop_back_size_greater_zero);

  printf("VEC_RESIZE\n");

  RUN_TEST(test_cstl_vec_resize_new_capacity_equal_old);
  RUN_TEST(test_cstl_vec_resize_new_capacity_greater_old);
  RUN_TEST(test_cstl_vec_resize_new_capacity_less_old);

  printf("VEC_SHRINK_TO_FIT\n");

  RUN_TEST(test_cstl_vec_shrink_to_fit);

  printf("VEC_CLEAR\n");

  RUN_TEST(test_cstl_vec_clear);

  printf("VEC_INSERT\n");

  RUN_TEST(test_cstl_vec_insert_pos_first);
  RUN_TEST(test_cstl_vec_insert_pos_last);
  RUN_TEST(test_cstl_vec_insert_pos_middle);
  RUN_TEST(test_cstl_vec_insert_pos_out_of_bound);

  printf("VEC_ERASE\n");

  RUN_TEST(test_cstl_vec_erase_pos_first);
  RUN_TEST(test_cstl_vec_erase_pos_last);
  RUN_TEST(test_cstl_vec_erase_pos_middle);
  RUN_TEST(test_cstl_vec_erase_pos_out_of_bound);

  printf("VEC_INSERT_RANGE\n");

  RUN_TEST(test_cstl_vec_insert_range_pos_first);
  RUN_TEST(test_cstl_vec_insert_range_pos_last);
  RUN_TEST(test_cstl_vec_insert_range_pos_middle);
  RUN_TEST(test_cstl_vec_insert_range_pos_out_of_bound);

  printf("VEC_ERASE_RANGE\n");

  RUN_TEST(test_cstl_vec_erase_range_len_zero);
  RUN_TEST(test_cstl_vec_erase_range_pos_plus_len_is_size);
  RUN_TEST(test_cstl_vec_erase_range_pos_plus_len_less_size);
  RUN_TEST(test_cstl_vec_erase_range_from_begin_to_middle);
  RUN_TEST(test_cstl_vec_erase_range_incorrect);

  return UNITY_END();
}
