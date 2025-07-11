#include "../../src/llist/cetl_llist_internal.h"
#include "../../include/cetl/cetl_llist.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"

#include <stdio.h>
#include <stdlib.h>

void setUp() {}
void tearDown() {}

void test_cetl_llist_create_node() {

  int num = 15;

  _cetl_node *n = _cetl_llist_create_node(&num, sizeof(int));

  TEST_ASSERT_NOT_NULL(n);

  TEST_ASSERT_NULL(n->next);
  TEST_ASSERT_EQUAL_INT(15, *((int *)n->data));

  free(n->data);
  free(n);
}

void test_cetl_llist_create_node_null() {

  _cetl_node *n = _cetl_llist_create_node(NULL, sizeof(int));
  TEST_ASSERT_NULL(n);
}

void test_cetl_llist_create_empty() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(0, l->elem_size);

  free(l);
}

void test_cetl_llist_create() {

  int elem = 10;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail->next);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cetl_llist_free(l);
}

void test_cetl_llist_create_data_struct() {

  typedef struct TestStruct {

    int x;
    short y;
    const char *str;

  } TestStruct;

  TestStruct *elem = malloc(sizeof(TestStruct));

  elem->x = 1;
  elem->y = 2;
  elem->str = "str";

  cetl_llist *l = cetl_llist_create(elem, sizeof(TestStruct));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(TestStruct), l->elem_size);

  cetl_llist_free(l);
  free(elem);
}

void test_cetl_llist_create_elem_size_zero() {

  int elem = 2;

  cetl_llist *l = cetl_llist_create(&elem, 0);

  TEST_ASSERT_NULL(l);
}

void test_cetl_llist_create_data_null() {

  cetl_llist *l = cetl_llist_create(NULL, 10);

  TEST_ASSERT_NULL(l);
}

void test_cetl_llist_create_copy() {

  int elem = 1;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &elem, sizeof(int));
  }

  cetl_llist *c_l = cetl_llist_create_copy(l);

  TEST_ASSERT_NOT_NULL(c_l);

  TEST_ASSERT_EQUAL_size_t(l->size, c_l->size);
  TEST_ASSERT_EQUAL_size_t(l->elem_size, c_l->elem_size);

  _cetl_node *n1 = l->head;
  _cetl_node *n2 = c_l->head;

  for (size_t i = 0; i < l->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)n1->data), *((int *)n2->data));
    TEST_ASSERT_NOT_EQUAL(n1->data, n2->data);

    n1 = n1->next;
    n2 = n2->next;
  }

  cetl_llist_free(l);
  cetl_llist_free(c_l);
}

void test_cetl_llist_create_copy_from_empty() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  cetl_llist *c_l = cetl_llist_create_copy(l);

  TEST_ASSERT_NOT_NULL(c_l);

  TEST_ASSERT_NULL(c_l->head);
  TEST_ASSERT_NULL(c_l->tail);
  TEST_ASSERT_EQUAL_size_t(l->size, c_l->size);
  TEST_ASSERT_EQUAL_size_t(l->elem_size, c_l->elem_size);

  cetl_llist_free(l);
  cetl_llist_free(c_l);
}

void test_cetl_llist_create_copy_from_null() {

  cetl_llist *c_l = cetl_llist_create_copy(NULL);

  TEST_ASSERT_NULL(c_l);
}

void test_cetl_llist_create_copy_independent() {

  int elem = 1;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &elem, sizeof(int));
  }

  cetl_llist *c_l = cetl_llist_create_copy(l);

  TEST_ASSERT_NOT_NULL(c_l);

  int n_elem = 10;
  cetl_llist_set(c_l, 0, &n_elem);

  TEST_ASSERT_EQUAL_size_t(l->size, c_l->size);
  TEST_ASSERT_EQUAL_size_t(l->elem_size, c_l->elem_size);

  TEST_ASSERT_NOT_EQUAL_INT(*((int *)l->head->data), *((int *)c_l->head->data));

  _cetl_node *n1 = l->head->next;
  _cetl_node *n2 = c_l->head->next;

  for (size_t i = 0; i < l->size - 1; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)n1->data), *((int *)n2->data));
    TEST_ASSERT_NOT_EQUAL(n1->data, n2->data);

    n1 = n1->next;
    n2 = n2->next;
  }

  cetl_llist_free(l);
  cetl_llist_free(c_l);
}

void test_cetl_llist_push_back_not_empty_llist() {

  int elem = 2;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int elem_new = 5;

  TEST_ASSERT_NOT_NULL(cetl_llist_push_back(l, &elem_new, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_push_back_empty_llist() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cetl_llist_push_back(l, &elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(l->head);
  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_push_back_data_null() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);
  TEST_ASSERT_NULL(cetl_llist_push_back(l, NULL, sizeof(int)));

  free(l);
}

void test_cetl_llist_push_front_not_empty_llist() {

  int elem = 2;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int elem_new = 10;

  TEST_ASSERT_NOT_NULL(cetl_llist_push_front(l, &elem_new, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_push_front_empty_llist() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cetl_llist_push_front(l, &elem, sizeof(int)));

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_push_front_data_null() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);
  TEST_ASSERT_NULL(cetl_llist_push_front(l, NULL, sizeof(int)));

  free(l);
}

void test_cetl_llist_pop_back_not_empty_llist() {

  int first_elem = 1;

  cetl_llist *l = cetl_llist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 7;

  TEST_ASSERT_NOT_NULL(cetl_llist_push_back(l, &second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cetl_llist_pop_back(l));

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(1, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(1, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_pop_back_size_one() {

  int elem = 1;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(cetl_llist_pop_back(l));

  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cetl_llist_free(l);
}

void test_cetl_llist_pop_back_size_zero() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);
  TEST_ASSERT_EQUAL(l, cetl_llist_pop_back(l));

  free(l);
}

void test_cetl_llist_pop_front_not_empty_llist() {

  int first_elem = 2;

  cetl_llist *l = cetl_llist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 9;

  TEST_ASSERT_NOT_NULL(cetl_llist_push_back(l, &second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cetl_llist_pop_front(l));

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(9, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(9, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_pop_front_size_one() {

  int elem = 1;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(cetl_llist_pop_front(l));

  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cetl_llist_free(l);
}

void test_cetl_llist_pop_front_size_zero() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);
  TEST_ASSERT_EQUAL(l, cetl_llist_pop_back(l));

  free(l);
}

void test_cetl_llist_insert_pos_begin() {

  int first_elem = 2;

  cetl_llist *l = cetl_llist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;

  TEST_ASSERT_NOT_NULL(cetl_llist_insert(l, &second_elem, 0, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_insert_size_zero() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cetl_llist_insert(l, &elem, 0, sizeof(int)));

  TEST_ASSERT_EQUAL(l->head, l->tail);
  TEST_ASSERT_EQUAL_size_t(1, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_insert_pos_end() {

  int first_elem = 2;

  cetl_llist *l = cetl_llist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;

  TEST_ASSERT_NOT_NULL(
      cetl_llist_insert(l, &second_elem, l->size, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_insert_pos_out_of_bound() {

  int first_elem = 2;

  cetl_llist *l = cetl_llist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;

  TEST_ASSERT_NULL(cetl_llist_insert(l, &second_elem, 100, sizeof(int)));

  cetl_llist_free(l);
}

void test_cetl_llist_insert_pos_middle() {

  int first_elem = 2;

  cetl_llist *l = cetl_llist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  int second_elem = 5;

  cetl_llist_push_back(l, &second_elem, sizeof(int));

  int third_elem = 9;

  TEST_ASSERT_NOT_NULL(
      cetl_llist_insert(l, &third_elem, l->size / 2, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(3, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)l->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)l->tail->data));
  TEST_ASSERT_EQUAL_INT(9, *((int *)l->head->next->data));

  cetl_llist_free(l);
}

void test_cetl_llist_erase_pos_begin() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for(int i = 0; i < 5; ++i){
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_llist_erase(l, 0));

  TEST_ASSERT_EQUAL_size_t(4, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(1, *((int *)l->head->data));

  cetl_llist_free(l);
}

void test_cetl_llist_erase_size_one() {

  int first_elem = 3;

  cetl_llist *l = cetl_llist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);

  TEST_ASSERT_NOT_NULL(cetl_llist_erase(l, 0));

  TEST_ASSERT_NULL(l->tail);
  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);

  cetl_llist_free(l);
}

void test_cetl_llist_erase_pos_end() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for(int i = 0; i < 5; ++i){
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_llist_erase(l, l->size - 1));

  TEST_ASSERT_EQUAL_size_t(4, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(3, *((int *)l->tail->data));

  cetl_llist_free(l);
}

void test_cetl_llist_erase_pos_middle() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for(int i = 0; i < 5; ++i){
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_llist_erase(l, l->size / 2));

  TEST_ASSERT_EQUAL_size_t(4, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(3, *((int*)cetl_llist_get(l, 2)));

  cetl_llist_free(l);
}

void test_cetl_llist_erase_pos_out_of_bound() {

  int elem = 3;

  cetl_llist *l = cetl_llist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l);
  TEST_ASSERT_NULL(cetl_llist_erase(l, 100));

  cetl_llist_free(l);
}

void test_cetl_llist_merge_two_both_not_empty() {

  int l1_elem = 1;

  cetl_llist *l1 = cetl_llist_create(&l1_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l1);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l1, &l1_elem, sizeof(int));
  }

  int l2_elem = 2;

  cetl_llist *l2 = cetl_llist_create(&l2_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l2);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l2, &l2_elem, sizeof(int));
  }

  cetl_llist *merged = cetl_llist_merge_two(l1, l2);
  TEST_ASSERT_NOT_NULL(merged);

  TEST_ASSERT_EQUAL_size_t(l1->size + l2->size, merged->size);

  _cetl_node *m_node = merged->head;
  _cetl_node *l1_node = l1->head;

  for (size_t i = 0; i < l1->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)m_node->data), *((int *)l1_node->data));
    TEST_ASSERT_NOT_EQUAL(m_node->data, l1_node->data);

    m_node = m_node->next;
    l1_node = l1_node->next;
  }

  _cetl_node *l2_node = l2->head;

  for (size_t i = 0; i < l2->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)m_node->data), *((int *)l2_node->data));
    TEST_ASSERT_NOT_EQUAL(m_node->data, l2_node->data);

    m_node = m_node->next;
    l2_node = l2_node->next;
  }

  cetl_llist_free(l1);
  cetl_llist_free(l2);
  cetl_llist_free(merged);
}

void test_cetl_llist_merge_two_one_empty() {

  cetl_llist *l1 = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l1);

  int l2_elem = 2;

  cetl_llist *l2 = cetl_llist_create(&l2_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(l2);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l2, &l2_elem, sizeof(int));
  }

  cetl_llist *merged = cetl_llist_merge_two(l1, l2);
  TEST_ASSERT_NOT_NULL(merged);

  TEST_ASSERT_EQUAL_size_t(l1->size + l2->size, merged->size);

  _cetl_node *m_node = merged->head;
  _cetl_node *l2_node = l2->head;

  for (size_t i = 0; i < l2->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)m_node->data), *((int *)l2_node->data));
    TEST_ASSERT_NOT_EQUAL(m_node->data, l2_node->data);

    m_node = m_node->next;
    l2_node = l2_node->next;
  }

  free(l1);
  cetl_llist_free(l2);
  cetl_llist_free(merged);
}

void test_cetl_llist_merge_two_both_empty() {

  cetl_llist *l1 = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l1);

  cetl_llist *l2 = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l2);

  cetl_llist *merged = cetl_llist_merge_two(l1, l2);
  TEST_ASSERT_NOT_NULL(merged);

  TEST_ASSERT_NULL(merged->head);
  TEST_ASSERT_NULL(merged->tail);
  TEST_ASSERT_EQUAL_size_t(l1->size + l2->size, merged->size);
  TEST_ASSERT_EQUAL_size_t(0, merged->elem_size);

  free(l1);
  free(l2);
  free(merged);
}

void test_cetl_llist_clear() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_llist_clear(l));

  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);

  free(l);
}

void test_cetl_llist_clear_double_clear() {


  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_llist_clear(l));
  TEST_ASSERT_NOT_NULL(cetl_llist_clear(l));

  TEST_ASSERT_EQUAL_size_t(0, l->size);
  TEST_ASSERT_NULL(l->head);
  TEST_ASSERT_NULL(l->tail);

  free(l);
}

void test_cetl_llist_set(){

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  int s_elem = 1;

  TEST_ASSERT_NOT_NULL(cetl_llist_set(l, 0, &s_elem));
  TEST_ASSERT_EQUAL_INT(s_elem, *((int*)l->head->data));

  cetl_llist_free(l);
}

void test_cetl_llist_set_out_of_bounds(){

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  int s_elem = 1;
  TEST_ASSERT_NULL(cetl_llist_set(l, 100, &s_elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_begin(){

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for(int i = 0; i < 5; ++i){
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, 0);

  TEST_ASSERT_EQUAL_INT(0, *((int*)elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_end(){

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for(int i = 0; i < 5; ++i){
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, l->size - 1);

  TEST_ASSERT_EQUAL_INT(4, *((int*)elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_middle(){

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for(int i = 0; i < 5; ++i){
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, l->size / 2);

  TEST_ASSERT_EQUAL_INT(2, *((int*)elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_out_of_bounds(){

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for(int i = 0; i < 5; ++i){
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, 100);

  TEST_ASSERT_NULL(elem);

  cetl_llist_free(l);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_cetl_llist_create_node);
  RUN_TEST(test_cetl_llist_create_node_null);

  printf("\n");

  RUN_TEST(test_cetl_llist_create_empty);

  printf("\n");

  RUN_TEST(test_cetl_llist_create);
  RUN_TEST(test_cetl_llist_create_data_struct);
  RUN_TEST(test_cetl_llist_create_elem_size_zero);
  RUN_TEST(test_cetl_llist_create_data_null);

  printf("\n");

  RUN_TEST(test_cetl_llist_create_copy);
  RUN_TEST(test_cetl_llist_create_copy_from_empty);
  RUN_TEST(test_cetl_llist_create_copy_from_null);
  RUN_TEST(test_cetl_llist_create_copy_independent);

  printf("\n");

  RUN_TEST(test_cetl_llist_push_back_empty_llist);
  RUN_TEST(test_cetl_llist_push_back_not_empty_llist);
  RUN_TEST(test_cetl_llist_push_back_data_null);

  printf("\n");

  RUN_TEST(test_cetl_llist_push_front_empty_llist);
  RUN_TEST(test_cetl_llist_push_front_not_empty_llist);
  RUN_TEST(test_cetl_llist_push_front_data_null);

  printf("\n");

  RUN_TEST(test_cetl_llist_pop_back_not_empty_llist);
  RUN_TEST(test_cetl_llist_pop_back_size_one);
  RUN_TEST(test_cetl_llist_pop_back_size_zero);

  printf("\n");

  RUN_TEST(test_cetl_llist_pop_front_not_empty_llist);
  RUN_TEST(test_cetl_llist_pop_front_size_one);
  RUN_TEST(test_cetl_llist_pop_front_size_zero);

  printf("\n");

  RUN_TEST(test_cetl_llist_insert_pos_begin);
  RUN_TEST(test_cetl_llist_insert_pos_middle);
  RUN_TEST(test_cetl_llist_insert_pos_end);
  RUN_TEST(test_cetl_llist_insert_size_zero);
  RUN_TEST(test_cetl_llist_insert_pos_out_of_bound);

  printf("\n");

  RUN_TEST(test_cetl_llist_erase_pos_begin);
  RUN_TEST(test_cetl_llist_erase_pos_middle);
  RUN_TEST(test_cetl_llist_erase_pos_end);
  RUN_TEST(test_cetl_llist_erase_size_one);
  RUN_TEST(test_cetl_llist_erase_pos_out_of_bound);

  printf("\n");

  RUN_TEST(test_cetl_llist_merge_two_both_not_empty);
  RUN_TEST(test_cetl_llist_merge_two_one_empty);
  RUN_TEST(test_cetl_llist_merge_two_both_empty);

  printf("\n");

  RUN_TEST(test_cetl_llist_clear);
  RUN_TEST(test_cetl_llist_clear_double_clear);

  printf("\n");

  RUN_TEST(test_cetl_llist_set);
  RUN_TEST(test_cetl_llist_set_out_of_bounds);

  printf("\n");

  RUN_TEST(test_cetl_llist_get_pos_begin);
  RUN_TEST(test_cetl_llist_get_pos_end);
  RUN_TEST(test_cetl_llist_get_pos_middle);
  RUN_TEST(test_cetl_llist_get_pos_out_of_bounds);

  return UNITY_END();
}
