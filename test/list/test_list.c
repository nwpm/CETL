#include "../../include/cstl/cstl_list.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"

#include <stdio.h>
#include <stdlib.h>

void setUp() {}
void tearDown() {}

void test_cstl_list_create_node() {

  int num = 15;
  void *p_num = &num;

  cstl_node *n = cstl_list_create_node(p_num);

  TEST_ASSERT_NOT_NULL(n);

  TEST_ASSERT_NULL(n->next);
  TEST_ASSERT_EQUAL_INT(15, *((int *)n->data));

  free(n);
}

void test_cstl_list_create_node_null() {

  cstl_node *n = cstl_list_create_node(NULL);
  TEST_ASSERT_NULL(n);
}

void test_cstl_list_create_empty() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(0, l->elem_size);

  free(l);
}

void test_cstl_list_create() {

  int elem = 10;
  void *p_elem = &elem;

  cstl_list *l = cstl_list_create(p_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_create_data_struct() {

  typedef struct TestStruct {

    int x;
    short y;
    char *str;

  } TestStruct;

  TestStruct *elem = malloc(sizeof(TestStruct));

  void *p_elem = &elem;

  cstl_list *l = cstl_list_create(p_elem, sizeof(TestStruct));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(TestStruct), l->elem_size);

  cstl_list_free_nodes(l);
  free(l);
  free(elem);
}

void test_cstl_list_create_elem_size_zero() {

  int elem = 2;
  void *p_elem = &elem;

  cstl_list *l = cstl_list_create(p_elem, 0);

  TEST_ASSERT_NULL(l);
}

void test_cstl_list_create_data_null() {

  cstl_list *l = cstl_list_create(NULL, 10);

  TEST_ASSERT_NULL(l);
}

void test_cstl_list_push_back_not_empty_list() {

  int elem = 2;
  void *p_elem = &elem;

  cstl_list *l = cstl_list_create(p_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int elem_sec = 5;
  void *p_elem_sec = &elem_sec;

  TEST_ASSERT_NOT_NULL(cstl_list_push_back(l, p_elem_sec, sizeof(int)));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_push_back_empty_list() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  int elem = 10;
  void *p_elem = &elem;

  TEST_ASSERT_NOT_NULL(cstl_list_push_back(l, p_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(l->head);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_push_back_data_null() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);
  TEST_ASSERT_NULL(cstl_list_push_back(l, NULL, sizeof(int)));

  free(l);
}

void test_cstl_list_push_front_not_empty_list() {

  int elem = 2;
  void *p_elem = &elem;

  cstl_list *l = cstl_list_create(p_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int elem_sec = 10;
  void *p_elem_sec = &elem_sec;

  TEST_ASSERT_NOT_NULL(cstl_list_push_front(l, p_elem_sec, sizeof(int)));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_push_front_empty_list() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  int elem = 10;
  void *p_elem = &elem;

  TEST_ASSERT_NOT_NULL(cstl_list_push_front(l, p_elem, sizeof(int)));

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_push_front_data_null() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);
  TEST_ASSERT_NULL(cstl_list_push_front(l, NULL, sizeof(int)));

  free(l);
}

void test_cstl_list_pop_back_not_empty_list() {

  int first_elem = 1;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 7;
  void *p_second_elem = &second_elem;

  TEST_ASSERT_NOT_NULL(cstl_list_push_back(l, p_second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cstl_list_pop_back(l));

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(1, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(1, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_pop_back_size_one() {

  int first_elem = 1;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(cstl_list_pop_back(l));

  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_pop_back_size_zero() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_EQUAL(l, cstl_list_pop_back(l));

  free(l);
}

void test_cstl_list_pop_front_not_empty_list() {

  int first_elem = 2;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 9;
  void *p_second_elem = &second_elem;

  TEST_ASSERT_NOT_NULL(cstl_list_push_back(l, p_second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cstl_list_pop_front(l));

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(9, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(9, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_pop_front_size_one() {

  int first_elem = 1;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(cstl_list_pop_front(l));

  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_pop_front_size_zero() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_EQUAL(l, cstl_list_pop_back(l));

  free(l);
}

void test_cstl_list_insert_pos_begin() {

  int first_elem = 2;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;
  void *p_second_elem = &second_elem;

  TEST_ASSERT_NOT_NULL(cstl_list_insert(l, p_second_elem, 0, sizeof(int)));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_insert_size_zero() {

  cstl_list *l = cstl_list_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  int elem = 10;
  void *p_elem = &elem;

  TEST_ASSERT_NOT_NULL(cstl_list_insert(l, p_elem, 0, sizeof(int)));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_insert_pos_end() {

  int first_elem = 2;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;
  void *p_second_elem = &second_elem;

  TEST_ASSERT_NOT_NULL(
      cstl_list_insert(l, p_second_elem, l->size, sizeof(int)));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_NOT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_insert_pos_out_of_bound() {

  int first_elem = 2;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;
  void *p_second_elem = &second_elem;

  TEST_ASSERT_NULL(cstl_list_insert(l, p_second_elem, 100, sizeof(int)));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_insert_pos_middle() {

  int first_elem = 2;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;
  void *p_second_elem = &second_elem;

  cstl_list_push_back(l, p_second_elem, sizeof(int));

  int third_elem = 9;
  void *p_third_elem = &third_elem;

  TEST_ASSERT_NOT_NULL(
      cstl_list_insert(l, p_third_elem, l->size / 2, sizeof(int)));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_NOT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(3, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));
  TEST_ASSERT_EQUAL_INT(9, *((int *)l->head->next->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_erase_pos_begin() {

  int first_elem = 3;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;
  void *p_second_elem = &second_elem;

  TEST_ASSERT_NOT_NULL(cstl_list_push_back(l, p_second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cstl_list_erase(l, 0));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_erase_size_one() {

  int first_elem = 3;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(cstl_list_erase(l, 0));

  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_erase_pos_end() {

  int first_elem = 3;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;
  void *p_second_elem = &second_elem;

  TEST_ASSERT_NOT_NULL(cstl_list_push_back(l, p_second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cstl_list_erase(l, l->size - 1));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(3, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(3, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_erase_pos_middle() {

  int first_elem = 2;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 4;
  void *p_second_elem = &second_elem;

  cstl_list_push_back(l, p_second_elem, sizeof(int));

  int third_elem = 5;
  void *p_third_elem = &third_elem;

  cstl_list_push_back(l, p_third_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(cstl_list_erase(l, l->size / 2));

  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_NOT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));

  cstl_list_free_nodes(l);
  free(l);
}

void test_cstl_list_erase_pos_out_of_bound() {

  int first_elem = 3;
  void *p_first_elem = &first_elem;

  cstl_list *l = cstl_list_create(p_first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NULL(cstl_list_erase(l, 100));

  cstl_list_free_nodes(l);
  free(l);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_cstl_list_create_node);
  RUN_TEST(test_cstl_list_create_node_null);

  printf("\n");

  RUN_TEST(test_cstl_list_create_empty);

  printf("\n");

  RUN_TEST(test_cstl_list_create);
  RUN_TEST(test_cstl_list_create_data_struct);
  RUN_TEST(test_cstl_list_create_elem_size_zero);
  RUN_TEST(test_cstl_list_create_data_null);

  printf("\n");

  RUN_TEST(test_cstl_list_push_back_empty_list);
  RUN_TEST(test_cstl_list_push_back_not_empty_list);
  RUN_TEST(test_cstl_list_push_back_data_null);

  printf("\n");

  RUN_TEST(test_cstl_list_push_front_empty_list);
  RUN_TEST(test_cstl_list_push_front_not_empty_list);
  RUN_TEST(test_cstl_list_push_front_data_null);

  printf("\n");

  RUN_TEST(test_cstl_list_pop_back_not_empty_list);
  RUN_TEST(test_cstl_list_pop_back_size_one);
  RUN_TEST(test_cstl_list_pop_back_size_zero);

  printf("\n");

  RUN_TEST(test_cstl_list_pop_front_not_empty_list);
  RUN_TEST(test_cstl_list_pop_front_size_one);
  RUN_TEST(test_cstl_list_pop_front_size_zero);

  printf("\n");

  RUN_TEST(test_cstl_list_insert_pos_begin);
  RUN_TEST(test_cstl_list_insert_pos_middle);
  RUN_TEST(test_cstl_list_insert_pos_end);
  RUN_TEST(test_cstl_list_insert_size_zero);
  RUN_TEST(test_cstl_list_insert_pos_out_of_bound);

  printf("\n");

  RUN_TEST(test_cstl_list_erase_pos_begin);
  RUN_TEST(test_cstl_list_erase_pos_middle);
  RUN_TEST(test_cstl_list_erase_pos_end);
  RUN_TEST(test_cstl_list_erase_size_one);
  RUN_TEST(test_cstl_list_erase_pos_out_of_bound);

  return UNITY_END();
}
