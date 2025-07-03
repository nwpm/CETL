#include "../../include/cstl/cstl_queue.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include <stdio.h>
#include <stdlib.h>

void setUp() {}
void tearDown() {}

void test_cstl_queue_create_empty() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_size_t(0, q->size);
  TEST_ASSERT_TRUE(cstl_llist_is_empty(q->data));

  cstl_queue_free(q);
}

void test_cstl_queue_create() {

  int elem = 10;

  cstl_queue *q = cstl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_size_t(1, q->size);
  TEST_ASSERT_NOT_NULL(cstl_queue_front(q));
  TEST_ASSERT_EQUAL_PTR(cstl_queue_front(q), cstl_queue_back(q));

  cstl_queue_free(q);
}

void test_cstl_queue_create_data_struct() {

  typedef struct TestStruct {

    int x;
    short y;
    const char *str;

  } TestStruct;

  TestStruct *elem = malloc(sizeof(TestStruct));

  elem->x = 1;
  elem->y = 5;
  elem->str = "string";

  cstl_queue *q = cstl_queue_create(elem, sizeof(TestStruct));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_size_t(1, q->size);
  TEST_ASSERT_NOT_NULL(cstl_queue_front(q));
  TEST_ASSERT_EQUAL_PTR(cstl_queue_front(q), cstl_queue_back(q));

  cstl_queue_free(q);
  free(elem);
}

void test_cstl_queue_create_elem_size_zero() {

  int elem = 3;

  cstl_queue *q = cstl_queue_create(&elem, 0);

  TEST_ASSERT_NULL(q);
}

void test_cstl_queue_create_data_null() {

  cstl_queue *q = cstl_queue_create(NULL, sizeof(int));

  TEST_ASSERT_NULL(q);
}

void test_cstl_queue_create_copy() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  cstl_queue *c_q = cstl_queue_create_copy(q);

  TEST_ASSERT_NOT_NULL(c_q);

  TEST_ASSERT_EQUAL_size_t(q->size, c_q->size);
  TEST_ASSERT_NOT_EQUAL(q->data, c_q->data);

  cstl_queue_free(q);
  cstl_queue_free(c_q);
}

void test_cstl_queue_create_copy_from_empty() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  cstl_queue *c_q = cstl_queue_create_copy(q);

  TEST_ASSERT_NOT_NULL(c_q);

  TEST_ASSERT_EQUAL_size_t(q->size, c_q->size);
  TEST_ASSERT_NOT_EQUAL(q->data, c_q->data);
  TEST_ASSERT_TRUE(cstl_queue_is_empty(c_q));

  cstl_queue_free(q);
  cstl_queue_free(c_q);
}

void test_cstl_queue_create_copy_from_null() {

  cstl_queue *c_q = cstl_queue_create_copy(NULL);

  TEST_ASSERT_NULL(c_q);
}

void test_cstl_queue_push_not_empty_queue() {

  int elem = 11;

  cstl_queue *q = cstl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  int elem_new = 20;

  TEST_ASSERT_NOT_NULL(cstl_queue_push(q, &elem_new, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, q->size);
  TEST_ASSERT_EQUAL_INT(11, *((int *)cstl_queue_front(q)));
  TEST_ASSERT_EQUAL_INT(20, *((int *)cstl_queue_back(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_push_empty_queue() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  int elem_new = 20;

  TEST_ASSERT_NOT_NULL(cstl_queue_push(q, &elem_new, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(1, q->size);
  TEST_ASSERT_EQUAL_PTR(cstl_queue_front(q), cstl_queue_back(q));
  TEST_ASSERT_EQUAL_INT(20, *((int *)cstl_queue_front(q)));
  TEST_ASSERT_EQUAL_INT(20, *((int *)cstl_queue_back(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_push_data_null() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NULL(cstl_queue_push(q, NULL, sizeof(int)));

  cstl_queue_free(q);
}

void test_cstl_queue_pop_not_empty_queue() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_queue_pop(q));

  TEST_ASSERT_EQUAL_size_t(9, q->size);
  TEST_ASSERT_EQUAL_INT(1, *((int *)cstl_queue_front(q)));
  TEST_ASSERT_EQUAL_INT(9, *((int *)cstl_queue_back(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_pop_size_one() {

  int elem = 4;

  cstl_queue *q = cstl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NOT_NULL(cstl_queue_pop(q));

  TEST_ASSERT_EQUAL_size_t(0, q->size);
  TEST_ASSERT_TRUE(cstl_queue_is_empty(q));

  cstl_queue_free(q);
}

void test_cstl_queue_pop_size_zero() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_PTR(q, cstl_queue_pop(q));

  cstl_queue_free(q);
}

void test_cstl_queue_front() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(0, *((int *)cstl_queue_front(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_front_size_zero() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NULL(cstl_queue_front(q));

  cstl_queue_free(q);
}

void test_cstl_queue_front_after_push() {

  int elem = 100;

  cstl_queue *q = cstl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cstl_queue_front(q)));

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(100, *((int *)cstl_queue_front(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_front_after_pop() {

  int elem = 100;

  cstl_queue *q = cstl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cstl_queue_front(q)));

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_queue_pop(q));

  TEST_ASSERT_EQUAL_INT(0, *((int *)cstl_queue_front(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_back() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(9, *((int *)cstl_queue_back(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_back_size_zero() {

  cstl_queue *q = cstl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NULL(cstl_queue_back(q));

  cstl_queue_free(q);
}

void test_cstl_queue_back_after_push() {

  int elem = 100;

  cstl_queue *q = cstl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cstl_queue_back(q)));

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(9, *((int *)cstl_queue_back(q)));

  cstl_queue_free(q);
}

void test_cstl_queue_back_after_pop() {

  int elem = 100;

  cstl_queue *q = cstl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cstl_queue_back(q)));

  for (int i = 0; i < 10; ++i) {
    cstl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_queue_pop(q));

  TEST_ASSERT_EQUAL_INT(9, *((int *)cstl_queue_back(q)));

  cstl_queue_free(q);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_cstl_queue_create_empty);

  printf("\n");

  RUN_TEST(test_cstl_queue_create);
  RUN_TEST(test_cstl_queue_create_data_struct);
  RUN_TEST(test_cstl_queue_create_elem_size_zero);
  RUN_TEST(test_cstl_queue_create_data_null);

  printf("\n");

  RUN_TEST(test_cstl_queue_create_copy);
  RUN_TEST(test_cstl_queue_create_copy_from_empty);
  RUN_TEST(test_cstl_queue_create_copy_from_null);

  printf("\n");

  RUN_TEST(test_cstl_queue_push_not_empty_queue);
  RUN_TEST(test_cstl_queue_push_empty_queue);
  RUN_TEST(test_cstl_queue_push_data_null);

  printf("\n");

  RUN_TEST(test_cstl_queue_pop_not_empty_queue);
  RUN_TEST(test_cstl_queue_pop_size_one);
  RUN_TEST(test_cstl_queue_pop_size_zero);

  printf("\n");

  RUN_TEST(test_cstl_queue_front);
  RUN_TEST(test_cstl_queue_front_size_zero);
  RUN_TEST(test_cstl_queue_front_after_push);
  RUN_TEST(test_cstl_queue_front_after_pop);

  printf("\n");

  RUN_TEST(test_cstl_queue_back);
  RUN_TEST(test_cstl_queue_back_size_zero);
  RUN_TEST(test_cstl_queue_back_after_push);
  RUN_TEST(test_cstl_queue_back_after_pop);

  return UNITY_END();
}
