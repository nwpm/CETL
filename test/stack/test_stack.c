#include "../../src/stack/cetl_stack_internal.h"
#include "../../include/cetl/cetl_stack.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include <stdio.h>

void setUp() {}
void tearDown() {}

void test_cetl_stack_create_empty() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->size);
  TEST_ASSERT_NOT_NULL(s->data);

  cetl_stack_free(s);
}

void test_cetl_stack_create() {

  int elem = 5;

  cetl_stack *s = cetl_stack_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(1, s->size);
  TEST_ASSERT_EQUAL_INT(5, *((int *)cetl_stack_top(s)));

  cetl_stack_free(s);
}

void test_cetl_stack_create_elem_size_zero() {

  int elem = 5;

  cetl_stack *s = cetl_stack_create(&elem, 0);

  TEST_ASSERT_NULL(s);
}

void test_cetl_stack_create_data_null() {

  cetl_stack *s = cetl_stack_create(NULL, sizeof(int));

  TEST_ASSERT_NULL(s);
}

void test_cetl_stack_create_copy() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (size_t i = 0; i < 5; ++i) {
    cetl_stack_push(s, &i, sizeof(size_t));
  }

  cetl_stack *c_s = cetl_stack_create_copy(s);

  TEST_ASSERT_NOT_NULL(c_s);
  TEST_ASSERT_EQUAL_size_t(s->size, c_s->size);

  _cetl_node *n1 = s->data->head;
  _cetl_node *n2 = c_s->data->head;

  for (size_t i = 0; i < s->size; ++i) {
    TEST_ASSERT_EQUAL_size_t(*((size_t *)n1->data), *((size_t *)n2->data));
    n1 = n1->next;
    n2 = n2->next;
  }

  cetl_stack_free(s);
  cetl_stack_free(c_s);
}

void test_cetl_stack_create_copy_empty() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cetl_stack *c_s = cetl_stack_create_copy(s);

  TEST_ASSERT_NOT_NULL(c_s);
  TEST_ASSERT_EQUAL_size_t(s->size, c_s->size);

  cetl_stack_free(s);
  cetl_stack_free(c_s);
}

void test_cetl_stack_create_copy_null() {

  cetl_stack *c_s = cetl_stack_create_copy(NULL);

  TEST_ASSERT_NULL(c_s);
}

void test_cetl_stack_push() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cetl_stack_push(s, &i, sizeof(int));
  }

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cetl_stack_push(s, &elem, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(6, s->size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)cetl_stack_top(s)));

  cetl_stack_free(s);
}

void test_cetl_stack_push_in_empty() {

  cetl_stack *s = cetl_stack_create_empty();

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cetl_stack_push(s, &elem, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(1, s->size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)cetl_stack_top(s)));

  cetl_stack_free(s);
}

void test_cetl_stack_pop() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cetl_stack_push(s, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_stack_pop(s));

  TEST_ASSERT_EQUAL_size_t(4, s->size);
  TEST_ASSERT_EQUAL_INT(3, *((int *)cetl_stack_top(s)));

  cetl_stack_free(s);
}

void test_cetl_stack_pop_size_one() {

  int elem = 1;

  cetl_stack *s = cetl_stack_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(cetl_stack_pop(s));

  TEST_ASSERT_EQUAL_size_t(0, s->size);
  TEST_ASSERT_EQUAL_INT(NULL, cetl_stack_top(s));

  cetl_stack_free(s);
}

void test_cetl_stack_pop_size_zero() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NULL(cetl_stack_pop(s));

  cetl_stack_free(s);
}

void test_cetl_stack_is_empty_true() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_TRUE(cetl_stack_is_empty(s));

  cetl_stack_free(s);
}

void test_cetl_stack_is_empty_false() {

  int elem = 2;

  cetl_stack *s = cetl_stack_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_FALSE(cetl_stack_is_empty(s));

  cetl_stack_free(s);
}

void test_cetl_stack_top() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cetl_stack_push(s, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(4, *((int *)cetl_stack_top(s)));

  cetl_stack_free(s);
}

void test_cetl_stack_top_size_zero() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cetl_stack_top(s));

  cetl_stack_free(s);
}

void test_cetl_stack_clear() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cetl_stack_push(s, &i, sizeof(int));
  }

  cetl_stack_clear(s);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->size);

  cetl_stack_free(s);
}

void test_cetl_stack_clear_double_clear() {

  cetl_stack *s = cetl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cetl_stack_push(s, &i, sizeof(int));
  }

  cetl_stack_clear(s);
  cetl_stack_clear(s);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->size);

  cetl_stack_free(s);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_cetl_stack_create_empty);

  printf("\n");

  RUN_TEST(test_cetl_stack_create);
  RUN_TEST(test_cetl_stack_create_elem_size_zero);
  RUN_TEST(test_cetl_stack_create_data_null);

  printf("\n");

  RUN_TEST(test_cetl_stack_create_copy);
  RUN_TEST(test_cetl_stack_create_copy_empty);
  RUN_TEST(test_cetl_stack_create_copy_null);

  printf("\n");

  RUN_TEST(test_cetl_stack_push);
  RUN_TEST(test_cetl_stack_push_in_empty);

  printf("\n");

  RUN_TEST(test_cetl_stack_pop);
  RUN_TEST(test_cetl_stack_pop_size_one);
  RUN_TEST(test_cetl_stack_pop_size_zero);

  printf("\n");

  RUN_TEST(test_cetl_stack_is_empty_true);
  RUN_TEST(test_cetl_stack_is_empty_false);

  printf("\n");

  RUN_TEST(test_cetl_stack_top);
  RUN_TEST(test_cetl_stack_top_size_zero);

  printf("\n");

  RUN_TEST(test_cetl_stack_clear);
  RUN_TEST(test_cetl_stack_clear_double_clear);

  return UNITY_END();
}
