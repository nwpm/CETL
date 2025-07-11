#include "../../src/queue/cetl_queue_internal.h"
#include "../../include/cetl/cetl_queue.h"
#include "../../include/cetl/cetl_llist.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include <stdio.h>
#include <stdlib.h>

void setUp() {}
void tearDown() {}

void test_cetl_queue_create_empty() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_size_t(0, q->size);
  TEST_ASSERT_TRUE(cetl_llist_is_empty(q->data));

  cetl_queue_free(q);
}

void test_cetl_queue_create() {

  int elem = 10;

  cetl_queue *q = cetl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_size_t(1, q->size);
  TEST_ASSERT_NOT_NULL(cetl_queue_front(q));
  TEST_ASSERT_EQUAL_PTR(cetl_queue_front(q), cetl_queue_back(q));

  cetl_queue_free(q);
}

void test_cetl_queue_create_data_struct() {

  typedef struct TestStruct {

    int x;
    short y;
    const char *str;

  } TestStruct;

  TestStruct *elem = malloc(sizeof(TestStruct));

  elem->x = 1;
  elem->y = 5;
  elem->str = "string";

  cetl_queue *q = cetl_queue_create(elem, sizeof(TestStruct));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_size_t(1, q->size);
  TEST_ASSERT_NOT_NULL(cetl_queue_front(q));
  TEST_ASSERT_EQUAL_PTR(cetl_queue_front(q), cetl_queue_back(q));

  cetl_queue_free(q);
  free(elem);
}

void test_cetl_queue_create_elem_size_zero() {

  int elem = 3;

  cetl_queue *q = cetl_queue_create(&elem, 0);

  TEST_ASSERT_NULL(q);
}

void test_cetl_queue_create_data_null() {

  cetl_queue *q = cetl_queue_create(NULL, sizeof(int));

  TEST_ASSERT_NULL(q);
}

void test_cetl_queue_create_copy() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  cetl_queue *c_q = cetl_queue_create_copy(q);

  TEST_ASSERT_NOT_NULL(c_q);

  TEST_ASSERT_EQUAL_size_t(q->size, c_q->size);
  TEST_ASSERT_NOT_EQUAL(q->data, c_q->data);

  cetl_queue_free(q);
  cetl_queue_free(c_q);
}

void test_cetl_queue_create_copy_from_empty() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  cetl_queue *c_q = cetl_queue_create_copy(q);

  TEST_ASSERT_NOT_NULL(c_q);

  TEST_ASSERT_EQUAL_size_t(q->size, c_q->size);
  TEST_ASSERT_NOT_EQUAL(q->data, c_q->data);
  TEST_ASSERT_TRUE(cetl_queue_is_empty(c_q));

  cetl_queue_free(q);
  cetl_queue_free(c_q);
}

void test_cetl_queue_create_copy_from_null() {

  cetl_queue *c_q = cetl_queue_create_copy(NULL);

  TEST_ASSERT_NULL(c_q);
}

void test_cetl_queue_push_not_empty_queue() {

  int elem = 11;

  cetl_queue *q = cetl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  int elem_new = 20;

  TEST_ASSERT_NOT_NULL(cetl_queue_push(q, &elem_new, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, q->size);
  TEST_ASSERT_EQUAL_INT(11, *((int *)cetl_queue_front(q)));
  TEST_ASSERT_EQUAL_INT(20, *((int *)cetl_queue_back(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_push_empty_queue() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  int elem_new = 20;

  TEST_ASSERT_NOT_NULL(cetl_queue_push(q, &elem_new, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(1, q->size);
  TEST_ASSERT_EQUAL_PTR(cetl_queue_front(q), cetl_queue_back(q));
  TEST_ASSERT_EQUAL_INT(20, *((int *)cetl_queue_front(q)));
  TEST_ASSERT_EQUAL_INT(20, *((int *)cetl_queue_back(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_push_data_null() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NULL(cetl_queue_push(q, NULL, sizeof(int)));

  cetl_queue_free(q);
}

void test_cetl_queue_pop_not_empty_queue() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_queue_pop(q));

  TEST_ASSERT_EQUAL_size_t(9, q->size);
  TEST_ASSERT_EQUAL_INT(1, *((int *)cetl_queue_front(q)));
  TEST_ASSERT_EQUAL_INT(9, *((int *)cetl_queue_back(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_pop_size_one() {

  int elem = 4;

  cetl_queue *q = cetl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NOT_NULL(cetl_queue_pop(q));

  TEST_ASSERT_EQUAL_size_t(0, q->size);
  TEST_ASSERT_TRUE(cetl_queue_is_empty(q));

  cetl_queue_free(q);
}

void test_cetl_queue_pop_size_zero() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_PTR(q, cetl_queue_pop(q));

  cetl_queue_free(q);
}

void test_cetl_queue_front() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(0, *((int *)cetl_queue_front(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_front_size_zero() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NULL(cetl_queue_front(q));

  cetl_queue_free(q);
}

void test_cetl_queue_front_after_push() {

  int elem = 100;

  cetl_queue *q = cetl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cetl_queue_front(q)));

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(100, *((int *)cetl_queue_front(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_front_after_pop() {

  int elem = 100;

  cetl_queue *q = cetl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cetl_queue_front(q)));

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_queue_pop(q));

  TEST_ASSERT_EQUAL_INT(0, *((int *)cetl_queue_front(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_back() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(9, *((int *)cetl_queue_back(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_back_size_zero() {

  cetl_queue *q = cetl_queue_create_empty();

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_NULL(cetl_queue_back(q));

  cetl_queue_free(q);
}

void test_cetl_queue_back_after_push() {

  int elem = 100;

  cetl_queue *q = cetl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cetl_queue_back(q)));

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(9, *((int *)cetl_queue_back(q)));

  cetl_queue_free(q);
}

void test_cetl_queue_back_after_pop() {

  int elem = 100;

  cetl_queue *q = cetl_queue_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(q);

  TEST_ASSERT_EQUAL_INT(100, *((int *)cetl_queue_back(q)));

  for (int i = 0; i < 10; ++i) {
    cetl_queue_push(q, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_queue_pop(q));

  TEST_ASSERT_EQUAL_INT(9, *((int *)cetl_queue_back(q)));

  cetl_queue_free(q);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_cetl_queue_create_empty);

  printf("\n");

  RUN_TEST(test_cetl_queue_create);
  RUN_TEST(test_cetl_queue_create_data_struct);
  RUN_TEST(test_cetl_queue_create_elem_size_zero);
  RUN_TEST(test_cetl_queue_create_data_null);

  printf("\n");

  RUN_TEST(test_cetl_queue_create_copy);
  RUN_TEST(test_cetl_queue_create_copy_from_empty);
  RUN_TEST(test_cetl_queue_create_copy_from_null);

  printf("\n");

  RUN_TEST(test_cetl_queue_push_not_empty_queue);
  RUN_TEST(test_cetl_queue_push_empty_queue);
  RUN_TEST(test_cetl_queue_push_data_null);

  printf("\n");

  RUN_TEST(test_cetl_queue_pop_not_empty_queue);
  RUN_TEST(test_cetl_queue_pop_size_one);
  RUN_TEST(test_cetl_queue_pop_size_zero);

  printf("\n");

  RUN_TEST(test_cetl_queue_front);
  RUN_TEST(test_cetl_queue_front_size_zero);
  RUN_TEST(test_cetl_queue_front_after_push);
  RUN_TEST(test_cetl_queue_front_after_pop);

  printf("\n");

  RUN_TEST(test_cetl_queue_back);
  RUN_TEST(test_cetl_queue_back_size_zero);
  RUN_TEST(test_cetl_queue_back_after_push);
  RUN_TEST(test_cetl_queue_back_after_pop);

  return UNITY_END();
}
