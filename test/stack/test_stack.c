#include "../../src/stack/cstl_stack_internal.h"
#include "../../include/cstl/cstl_stack.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include <stdio.h>

void setUp() {}
void tearDown() {}

void test_cstl_stack_create_empty() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->size);
  TEST_ASSERT_NOT_NULL(s->data);

  cstl_stack_free(s);
}

void test_cstl_stack_create() {

  int elem = 5;

  cstl_stack *s = cstl_stack_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(1, s->size);
  TEST_ASSERT_EQUAL_INT(5, *((int *)cstl_stack_top(s)));

  cstl_stack_free(s);
}

void test_cstl_stack_create_elem_size_zero() {

  int elem = 5;

  cstl_stack *s = cstl_stack_create(&elem, 0);

  TEST_ASSERT_NULL(s);
}

void test_cstl_stack_create_data_null() {

  cstl_stack *s = cstl_stack_create(NULL, sizeof(int));

  TEST_ASSERT_NULL(s);
}

void test_cstl_stack_create_copy() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (size_t i = 0; i < 5; ++i) {
    cstl_stack_push(s, &i, sizeof(size_t));
  }

  cstl_stack *c_s = cstl_stack_create_copy(s);

  TEST_ASSERT_NOT_NULL(c_s);
  TEST_ASSERT_EQUAL_size_t(s->size, c_s->size);

  _cstl_node *n1 = s->data->head;
  _cstl_node *n2 = c_s->data->head;

  for (size_t i = 0; i < s->size; ++i) {
    TEST_ASSERT_EQUAL_size_t(*((size_t *)n1->data), *((size_t *)n2->data));
    n1 = n1->next;
    n2 = n2->next;
  }

  cstl_stack_free(s);
  cstl_stack_free(c_s);
}

void test_cstl_stack_create_copy_empty() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  cstl_stack *c_s = cstl_stack_create_copy(s);

  TEST_ASSERT_NOT_NULL(c_s);
  TEST_ASSERT_EQUAL_size_t(s->size, c_s->size);

  cstl_stack_free(s);
  cstl_stack_free(c_s);
}

void test_cstl_stack_create_copy_null() {

  cstl_stack *c_s = cstl_stack_create_copy(NULL);

  TEST_ASSERT_NULL(c_s);
}

void test_cstl_stack_push() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cstl_stack_push(s, &i, sizeof(int));
  }

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cstl_stack_push(s, &elem, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(6, s->size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)cstl_stack_top(s)));

  cstl_stack_free(s);
}

void test_cstl_stack_push_in_empty() {

  cstl_stack *s = cstl_stack_create_empty();

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cstl_stack_push(s, &elem, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(1, s->size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)cstl_stack_top(s)));

  cstl_stack_free(s);
}

void test_cstl_stack_pop() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cstl_stack_push(s, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_stack_pop(s));

  TEST_ASSERT_EQUAL_size_t(4, s->size);
  TEST_ASSERT_EQUAL_INT(3, *((int *)cstl_stack_top(s)));

  cstl_stack_free(s);
}

void test_cstl_stack_pop_size_one() {

  int elem = 1;

  cstl_stack *s = cstl_stack_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(cstl_stack_pop(s));

  TEST_ASSERT_EQUAL_size_t(0, s->size);
  TEST_ASSERT_EQUAL_INT(NULL, cstl_stack_top(s));

  cstl_stack_free(s);
}

void test_cstl_stack_pop_size_zero() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NULL(cstl_stack_pop(s));

  cstl_stack_free(s);
}

void test_cstl_stack_is_empty_true() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_TRUE(cstl_stack_is_empty(s));

  cstl_stack_free(s);
}

void test_cstl_stack_is_empty_false() {

  int elem = 2;

  cstl_stack *s = cstl_stack_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_FALSE(cstl_stack_is_empty(s));

  cstl_stack_free(s);
}

void test_cstl_stack_top() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cstl_stack_push(s, &i, sizeof(int));
  }

  TEST_ASSERT_EQUAL_INT(4, *((int *)cstl_stack_top(s)));

  cstl_stack_free(s);
}

void test_cstl_stack_top_size_zero() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  TEST_ASSERT_NULL(cstl_stack_top(s));

  cstl_stack_free(s);
}

void test_cstl_stack_clear() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cstl_stack_push(s, &i, sizeof(int));
  }

  cstl_stack_clear(s);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->size);

  cstl_stack_free(s);
}

void test_cstl_stack_clear_double_clear() {

  cstl_stack *s = cstl_stack_create_empty();

  TEST_ASSERT_NOT_NULL(s);

  for (int i = 0; i < 5; ++i) {
    cstl_stack_push(s, &i, sizeof(int));
  }

  cstl_stack_clear(s);
  cstl_stack_clear(s);

  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_EQUAL_size_t(0, s->size);

  cstl_stack_free(s);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_cstl_stack_create_empty);

  printf("\n");

  RUN_TEST(test_cstl_stack_create);
  RUN_TEST(test_cstl_stack_create_elem_size_zero);
  RUN_TEST(test_cstl_stack_create_data_null);

  printf("\n");

  RUN_TEST(test_cstl_stack_create_copy);
  RUN_TEST(test_cstl_stack_create_copy_empty);
  RUN_TEST(test_cstl_stack_create_copy_null);

  printf("\n");

  RUN_TEST(test_cstl_stack_push);
  RUN_TEST(test_cstl_stack_push_in_empty);

  printf("\n");

  RUN_TEST(test_cstl_stack_pop);
  RUN_TEST(test_cstl_stack_pop_size_one);
  RUN_TEST(test_cstl_stack_pop_size_zero);

  printf("\n");

  RUN_TEST(test_cstl_stack_is_empty_true);
  RUN_TEST(test_cstl_stack_is_empty_false);

  printf("\n");

  RUN_TEST(test_cstl_stack_top);
  RUN_TEST(test_cstl_stack_top_size_zero);

  printf("\n");

  RUN_TEST(test_cstl_stack_clear);
  RUN_TEST(test_cstl_stack_clear_double_clear);

  return UNITY_END();
}
