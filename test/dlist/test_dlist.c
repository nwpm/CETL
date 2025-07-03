#include "../../src/dlist/cstl_dlist_internal.h"
#include "../../include/cstl/cstl_dlist.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"

#include <stdio.h>
#include <stdlib.h>

void setUp() {}
void tearDown() {}

void test_cstl_dlist_create_node() {

  int num = 1;

  _cstl_dnode *dn = _cstl_dlist_create_dnode(&num, sizeof(int));

  TEST_ASSERT_NOT_NULL(dn);

  TEST_ASSERT_NULL(dn->next);
  TEST_ASSERT_NULL(dn->prev);

  TEST_ASSERT_EQUAL_INT(1, *((int *)dn->data));

  free(dn->data);
  free(dn);
}

void test_cstl_dlist_create_node_null() {

  _cstl_dnode *n = _cstl_dlist_create_dnode(NULL, sizeof(int));
  TEST_ASSERT_NULL(n);
}

void test_cstl_dlist_create_empty() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  TEST_ASSERT_NULL(dl->head);
  TEST_ASSERT_NULL(dl->tail);
  TEST_ASSERT_EQUAL_size_t(0, dl->size);
  TEST_ASSERT_EQUAL_size_t(0, dl->elem_size);

  free(dl);
}

void test_cstl_dlist_create() {

  int elem = 10;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);
  TEST_ASSERT_NOT_NULL(dl->head);

  TEST_ASSERT_NULL(dl->head->next);
  TEST_ASSERT_NULL(dl->head->prev);

  TEST_ASSERT_EQUAL(dl->head, dl->tail);
  TEST_ASSERT_EQUAL_size_t(1, dl->size);

  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);

  cstl_dlist_free(dl);
}

void test_cstl_dlist_create_correct_prev() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);
  
  for(int i = 0; i < 10; ++i){
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  _cstl_dnode *current = dl->head;
  TEST_ASSERT_NULL(current->prev);

  for(size_t i = 0; i < dl->size - 1; ++i){
    TEST_ASSERT_EQUAL_PTR(current->next->prev, current);
    current = current->next;
  }

  TEST_ASSERT_NULL(current->next);

  cstl_dlist_free(dl);
}

void test_cstl_dlist_create_correct_prev_reverse() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);
  
  for(int i = 0; i < 10; ++i){
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  _cstl_dnode *current = dl->tail;
  TEST_ASSERT_NULL(current->next);

  for(size_t i = 0; i < dl->size - 1; ++i){
    TEST_ASSERT_EQUAL_PTR(current->prev->next, current);
    current = current->prev;
  }

  TEST_ASSERT_NULL(current->prev);

  cstl_dlist_free(dl);
}

void test_cstl_dlist_create_data_struct() {

  typedef struct TestStruct {

    int x;
    short y;
    const char *str;

  } TestStruct;

  TestStruct *elem = malloc(sizeof(TestStruct));

  elem->x = 1;
  elem->y = 2;
  elem->str = "str";

  cstl_dlist *dl = cstl_dlist_create(elem, sizeof(TestStruct));

  TEST_ASSERT_NOT_NULL(dl);

  TEST_ASSERT_EQUAL(dl->head, dl->tail);
  TEST_ASSERT_EQUAL_size_t(1, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(TestStruct), dl->elem_size);

  cstl_dlist_free(dl);
  free(elem);
}

void test_cstl_dlist_create_elem_size_zero() {

  int elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&elem, 0);

  TEST_ASSERT_NULL(dl);
}

void test_cstl_dlist_create_data_null() {

  cstl_dlist *dl = cstl_dlist_create(NULL, 10);

  TEST_ASSERT_NULL(dl);
}

void test_cstl_dlist_create_copy() {

  int elem = 1;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl, &elem, sizeof(int));
  }

  cstl_dlist *c_dl = cstl_dlist_create_copy(dl);

  TEST_ASSERT_NOT_NULL(c_dl);

  TEST_ASSERT_EQUAL_size_t(dl->size, c_dl->size);
  TEST_ASSERT_EQUAL_size_t(dl->elem_size, c_dl->elem_size);

  _cstl_dnode *n1 = dl->head;
  _cstl_dnode *n2 = c_dl->head;

  for (size_t i = 0; i < dl->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)n1->data), *((int *)n2->data));
    TEST_ASSERT_NOT_EQUAL(n1->data, n2->data);

    n1 = n1->next;
    n2 = n2->next;
  }

  cstl_dlist_free(dl);
  cstl_dlist_free(c_dl);
}

void test_cstl_dlist_create_copy_reverse_elem_check() {

  int elem = 1;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl, &elem, sizeof(int));
  }

  cstl_dlist *c_dl = cstl_dlist_create_copy(dl);

  TEST_ASSERT_NOT_NULL(c_dl);

  TEST_ASSERT_EQUAL_size_t(dl->size, c_dl->size);
  TEST_ASSERT_EQUAL_size_t(dl->elem_size, c_dl->elem_size);

  _cstl_dnode *n1 = dl->tail;
  _cstl_dnode *n2 = c_dl->tail;

  while(n1 && n2) {
    TEST_ASSERT_EQUAL_INT(*((int *)n1->data), *((int *)n2->data));
    TEST_ASSERT_NOT_EQUAL(n1->data, n2->data);

    n1 = n1->prev;
    n2 = n2->prev;
  }

  cstl_dlist_free(dl);
  cstl_dlist_free(c_dl);
}


void test_cstl_dlist_create_copy_from_empty() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  cstl_dlist *c_dl = cstl_dlist_create_copy(dl);

  TEST_ASSERT_NOT_NULL(c_dl);

  TEST_ASSERT_NULL(c_dl->head);
  TEST_ASSERT_NULL(c_dl->tail);
  TEST_ASSERT_EQUAL_size_t(dl->size, c_dl->size);
  TEST_ASSERT_EQUAL_size_t(dl->elem_size, c_dl->elem_size);

  cstl_dlist_free(dl);
  cstl_dlist_free(c_dl);
}

void test_cstl_dlist_create_copy_from_null() {

  cstl_dlist *c_dl = cstl_dlist_create_copy(NULL);

  TEST_ASSERT_NULL(c_dl);
}

void test_cstl_dlist_create_copy_independent() {

  int elem = 1;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl, &elem, sizeof(int));
  }

  cstl_dlist *c_dl = cstl_dlist_create_copy(dl);

  TEST_ASSERT_NOT_NULL(c_dl);

  int n_elem = 10;
  cstl_dlist_set(c_dl, 0, &n_elem);

  TEST_ASSERT_EQUAL_size_t(dl->size, c_dl->size);
  TEST_ASSERT_EQUAL_size_t(dl->elem_size, c_dl->elem_size);

  TEST_ASSERT_NOT_EQUAL_INT(*((int *)dl->head->data), *((int *)c_dl->head->data));

  _cstl_dnode *n1 = dl->head->next;
  _cstl_dnode *n2 = c_dl->head->next;

  for (size_t i = 0; i < dl->size - 1; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)n1->data), *((int *)n2->data));
    TEST_ASSERT_NOT_EQUAL(n1->data, n2->data);

    n1 = n1->next;
    n2 = n2->next;
  }

  cstl_dlist_free(dl);
  cstl_dlist_free(c_dl);
}

void test_cstl_dlist_push_back_not_empty_dlist() {

  int elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int elem_new = 5;

  TEST_ASSERT_NOT_NULL(cstl_dlist_push_back(dl, &elem_new, sizeof(int)));

  TEST_ASSERT_NULL(dl->tail->next);
  TEST_ASSERT_EQUAL_PTR(dl->tail->prev->next, dl->tail);
  TEST_ASSERT_EQUAL_size_t(2, dl->size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_push_back_empty_dlist() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cstl_dlist_push_back(dl, &elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(dl->head);
  TEST_ASSERT_EQUAL(dl->head, dl->tail);
  TEST_ASSERT_EQUAL_size_t(1, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_push_back_data_null() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);
  TEST_ASSERT_NULL(cstl_dlist_push_back(dl, NULL, sizeof(int)));

  free(dl);
}

void test_cstl_dlist_push_front_not_empty_dlist() {

  int elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int elem_new = 10;

  TEST_ASSERT_NOT_NULL(cstl_dlist_push_front(dl, &elem_new, sizeof(int)));

  TEST_ASSERT_NULL(dl->head->prev);
  TEST_ASSERT_EQUAL_PTR(dl->head->next->prev, dl->head);
  TEST_ASSERT_EQUAL_size_t(2, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(2, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_push_front_empty_dlist() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cstl_dlist_push_front(dl, &elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(dl->head);
  TEST_ASSERT_EQUAL(dl->head, dl->tail);
  TEST_ASSERT_EQUAL_size_t(1, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_push_front_data_null() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);
  TEST_ASSERT_NULL(cstl_dlist_push_front(dl, NULL, sizeof(int)));

  free(dl);
}

void test_cstl_dlist_pop_back_not_empty_dlist() {

  int first_elem = 1;

  cstl_dlist *dl = cstl_dlist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int second_elem = 7;

  TEST_ASSERT_NOT_NULL(cstl_dlist_push_back(dl, &second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cstl_dlist_pop_back(dl));

  TEST_ASSERT_NULL(dl->tail->next);
  TEST_ASSERT_EQUAL(dl->head, dl->tail);
  TEST_ASSERT_EQUAL_size_t(1, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(1, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(1, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_pop_back_size_one() {

  int elem = 1;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  TEST_ASSERT_NOT_NULL(cstl_dlist_pop_back(dl));

  TEST_ASSERT_NULL(dl->head);
  TEST_ASSERT_NULL(dl->tail);
  TEST_ASSERT_EQUAL_size_t(0, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);

  cstl_dlist_free(dl);
}

void test_cstl_dlist_pop_back_size_zero() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);
  TEST_ASSERT_EQUAL(dl, cstl_dlist_pop_back(dl));

  free(dl);
}

void test_cstl_dlist_pop_front_not_empty_dlist() {

  int first_elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int second_elem = 9;

  TEST_ASSERT_NOT_NULL(cstl_dlist_push_back(dl, &second_elem, sizeof(int)));

  TEST_ASSERT_NOT_NULL(cstl_dlist_pop_front(dl));

  TEST_ASSERT_NULL(dl->head->prev);
  TEST_ASSERT_EQUAL(dl->head, dl->tail);
  TEST_ASSERT_EQUAL_size_t(1, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(9, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(9, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_pop_front_size_one() {

  int elem = 1;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  TEST_ASSERT_NOT_NULL(cstl_dlist_pop_front(dl));

  TEST_ASSERT_NULL(dl->head);
  TEST_ASSERT_NULL(dl->tail);
  TEST_ASSERT_EQUAL_size_t(0, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);

  cstl_dlist_free(dl);
}

void test_cstl_dlist_pop_front_size_zero() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);
  TEST_ASSERT_EQUAL(dl, cstl_dlist_pop_back(dl));

  free(dl);
}

void test_cstl_dlist_insert_pos_begin() {

  int first_elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int second_elem = 5;

  TEST_ASSERT_NOT_NULL(cstl_dlist_insert(dl, &second_elem, 0, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(5, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(2, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_insert_size_zero() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  int elem = 10;

  TEST_ASSERT_NOT_NULL(cstl_dlist_insert(dl, &elem, 0, sizeof(int)));

  TEST_ASSERT_EQUAL(dl->head, dl->tail);
  TEST_ASSERT_EQUAL_size_t(1, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(10, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(10, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_insert_pos_end() {

  int first_elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int second_elem = 5;

  TEST_ASSERT_NOT_NULL(
      cstl_dlist_insert(dl, &second_elem, dl->size, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(2, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_insert_pos_out_of_bound() {

  int first_elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int second_elem = 5;

  TEST_ASSERT_NULL(cstl_dlist_insert(dl, &second_elem, 100, sizeof(int)));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_insert_pos_middle() {

  int first_elem = 2;

  cstl_dlist *dl = cstl_dlist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  int second_elem = 5;

  cstl_dlist_push_back(dl, &second_elem, sizeof(int));

  int third_elem = 9;

  TEST_ASSERT_NOT_NULL(
      cstl_dlist_insert(dl, &third_elem, dl->size / 2, sizeof(int)));

  TEST_ASSERT_EQUAL_size_t(3, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(2, *((int *)dl->head->data));
  TEST_ASSERT_EQUAL_INT(5, *((int *)dl->tail->data));
  TEST_ASSERT_EQUAL_INT(9, *((int *)dl->head->next->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_erase_pos_begin() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (int i = 0; i < 5; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_dlist_erase(dl, 0));

  TEST_ASSERT_EQUAL_size_t(4, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(1, *((int *)dl->head->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_erase_size_one() {

  int first_elem = 3;

  cstl_dlist *dl = cstl_dlist_create(&first_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);

  TEST_ASSERT_NOT_NULL(cstl_dlist_erase(dl, 0));

  TEST_ASSERT_NULL(dl->tail);
  TEST_ASSERT_NULL(dl->head);
  TEST_ASSERT_EQUAL_size_t(0, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);

  cstl_dlist_free(dl);
}

void test_cstl_dlist_erase_pos_end() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (int i = 0; i < 5; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_dlist_erase(dl, dl->size - 1));

  TEST_ASSERT_EQUAL_size_t(4, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(3, *((int *)dl->tail->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_erase_pos_middle() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (int i = 0; i < 5; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_dlist_erase(dl, dl->size / 2));

  TEST_ASSERT_EQUAL_size_t(4, dl->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), dl->elem_size);
  TEST_ASSERT_EQUAL_INT(3, *((int *)cstl_dlist_get(dl, 2)));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_erase_pos_out_of_bound() {

  int elem = 3;

  cstl_dlist *dl = cstl_dlist_create(&elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl);
  TEST_ASSERT_NULL(cstl_dlist_erase(dl, 100));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_merge_two_both_not_empty() {

  int dl1_elem = 1;

  cstl_dlist *dl1 = cstl_dlist_create(&dl1_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl1);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl1, &dl1_elem, sizeof(int));
  }

  int dl2_elem = 2;

  cstl_dlist *dl2 = cstl_dlist_create(&dl2_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl2);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl2, &dl2_elem, sizeof(int));
  }

  cstl_dlist *merged = cstl_dlist_merge_two(dl1, dl2);
  TEST_ASSERT_NOT_NULL(merged);

  TEST_ASSERT_EQUAL_size_t(dl1->size + dl2->size, merged->size);

  _cstl_dnode *m_node = merged->head;
  _cstl_dnode *dl1_node = dl1->head;

  for (size_t i = 0; i < dl1->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)m_node->data), *((int *)dl1_node->data));
    TEST_ASSERT_NOT_EQUAL(m_node->data, dl1_node->data);

    m_node = m_node->next;
    dl1_node = dl1_node->next;
  }

  _cstl_dnode *dl2_node = dl2->head;

  for (size_t i = 0; i < dl2->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)m_node->data), *((int *)dl2_node->data));
    TEST_ASSERT_NOT_EQUAL(m_node->data, dl2_node->data);

    m_node = m_node->next;
    dl2_node = dl2_node->next;
  }

  cstl_dlist_free(dl1);
  cstl_dlist_free(dl2);
  cstl_dlist_free(merged);
}

void test_cstl_dlist_merge_two_one_empty() {

  cstl_dlist *dl1 = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl1);

  int dl2_elem = 2;

  cstl_dlist *dl2 = cstl_dlist_create(&dl2_elem, sizeof(int));

  TEST_ASSERT_NOT_NULL(dl2);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl2, &dl2_elem, sizeof(int));
  }

  cstl_dlist *merged = cstl_dlist_merge_two(dl1, dl2);
  TEST_ASSERT_NOT_NULL(merged);

  TEST_ASSERT_EQUAL_size_t(dl1->size + dl2->size, merged->size);

  _cstl_dnode *m_node = merged->head;
  _cstl_dnode *dl2_node = dl2->head;

  for (size_t i = 0; i < dl2->size; ++i) {
    TEST_ASSERT_EQUAL_INT(*((int *)m_node->data), *((int *)dl2_node->data));
    TEST_ASSERT_NOT_EQUAL(m_node->data, dl2_node->data);

    m_node = m_node->next;
    dl2_node = dl2_node->next;
  }

  free(dl1);
  cstl_dlist_free(dl2);
  cstl_dlist_free(merged);
}

void test_cstl_dlist_merge_two_both_empty() {

  cstl_dlist *dl1 = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl1);

  cstl_dlist *dl2 = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl2);

  cstl_dlist *merged = cstl_dlist_merge_two(dl1, dl2);
  TEST_ASSERT_NOT_NULL(merged);

  TEST_ASSERT_NULL(merged->head);
  TEST_ASSERT_NULL(merged->tail);
  TEST_ASSERT_EQUAL_size_t(dl1->size + dl2->size, merged->size);
  TEST_ASSERT_EQUAL_size_t(0, merged->elem_size);

  free(dl1);
  free(dl2);
  free(merged);
}

void test_cstl_dlist_clear() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_dlist_clear(dl));

  TEST_ASSERT_EQUAL_size_t(0, dl->size);
  TEST_ASSERT_NULL(dl->head);
  TEST_ASSERT_NULL(dl->tail);

  free(dl);
}

void test_cstl_dlist_clear_double_clear() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cstl_dlist_clear(dl));
  TEST_ASSERT_NOT_NULL(cstl_dlist_clear(dl));

  TEST_ASSERT_EQUAL_size_t(0, dl->size);
  TEST_ASSERT_NULL(dl->head);
  TEST_ASSERT_NULL(dl->tail);

  free(dl);
}

void test_cstl_dlist_set() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  int s_elem = 1;

  TEST_ASSERT_NOT_NULL(cstl_dlist_set(dl, 0, &s_elem));
  TEST_ASSERT_EQUAL_INT(s_elem, *((int *)dl->head->data));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_set_out_of_bounds() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (size_t i = 0; i < 4; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  int s_elem = 1;
  TEST_ASSERT_NULL(cstl_dlist_set(dl, 100, &s_elem));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_get_pos_begin() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (int i = 0; i < 5; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  void *elem = cstl_dlist_get(dl, 0);

  TEST_ASSERT_EQUAL_INT(0, *((int *)elem));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_get_pos_end() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (int i = 0; i < 5; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  void *elem = cstl_dlist_get(dl, dl->size - 1);

  TEST_ASSERT_EQUAL_INT(4, *((int *)elem));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_get_pos_middle() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (int i = 0; i < 5; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  void *elem = cstl_dlist_get(dl, dl->size / 2);

  TEST_ASSERT_EQUAL_INT(2, *((int *)elem));

  cstl_dlist_free(dl);
}

void test_cstl_dlist_get_pos_out_of_bounds() {

  cstl_dlist *dl = cstl_dlist_create_empty();

  TEST_ASSERT_NOT_NULL(dl);

  for (int i = 0; i < 5; ++i) {
    cstl_dlist_push_back(dl, &i, sizeof(int));
  }

  void *elem = cstl_dlist_get(dl, 100);

  TEST_ASSERT_NULL(elem);

  cstl_dlist_free(dl);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_cstl_dlist_create_node);
  RUN_TEST(test_cstl_dlist_create_node_null);

  printf("\n");

  RUN_TEST(test_cstl_dlist_create_empty);

  printf("\n");

  RUN_TEST(test_cstl_dlist_create);
  RUN_TEST(test_cstl_dlist_create_correct_prev);
  RUN_TEST(test_cstl_dlist_create_correct_prev_reverse);
  RUN_TEST(test_cstl_dlist_create_data_struct);
  RUN_TEST(test_cstl_dlist_create_elem_size_zero);
  RUN_TEST(test_cstl_dlist_create_data_null);

  printf("\n");

  RUN_TEST(test_cstl_dlist_create_copy);
  RUN_TEST(test_cstl_dlist_create_copy_reverse_elem_check);
  RUN_TEST(test_cstl_dlist_create_copy_from_empty);
  RUN_TEST(test_cstl_dlist_create_copy_from_null);
  RUN_TEST(test_cstl_dlist_create_copy_independent);

  printf("\n");

  RUN_TEST(test_cstl_dlist_push_back_empty_dlist);
  RUN_TEST(test_cstl_dlist_push_back_not_empty_dlist);
  RUN_TEST(test_cstl_dlist_push_back_data_null);

  printf("\n");

  RUN_TEST(test_cstl_dlist_push_front_empty_dlist);
  RUN_TEST(test_cstl_dlist_push_front_not_empty_dlist);
  RUN_TEST(test_cstl_dlist_push_front_data_null);

  printf("\n");

  RUN_TEST(test_cstl_dlist_pop_back_not_empty_dlist);
  RUN_TEST(test_cstl_dlist_pop_back_size_one);
  RUN_TEST(test_cstl_dlist_pop_back_size_zero);

  printf("\n");

  RUN_TEST(test_cstl_dlist_pop_front_not_empty_dlist);
  RUN_TEST(test_cstl_dlist_pop_front_size_one);
  RUN_TEST(test_cstl_dlist_pop_front_size_zero);

  printf("\n");

  RUN_TEST(test_cstl_dlist_insert_pos_begin);
  RUN_TEST(test_cstl_dlist_insert_pos_middle);
  RUN_TEST(test_cstl_dlist_insert_pos_end);
  RUN_TEST(test_cstl_dlist_insert_size_zero);
  RUN_TEST(test_cstl_dlist_insert_pos_out_of_bound);

  printf("\n");

  RUN_TEST(test_cstl_dlist_erase_pos_begin);
  RUN_TEST(test_cstl_dlist_erase_pos_middle);
  RUN_TEST(test_cstl_dlist_erase_pos_end);
  RUN_TEST(test_cstl_dlist_erase_size_one);
  RUN_TEST(test_cstl_dlist_erase_pos_out_of_bound);

  printf("\n");

  RUN_TEST(test_cstl_dlist_merge_two_both_not_empty);
  RUN_TEST(test_cstl_dlist_merge_two_one_empty);
  RUN_TEST(test_cstl_dlist_merge_two_both_empty);

  printf("\n");

  RUN_TEST(test_cstl_dlist_clear);
  RUN_TEST(test_cstl_dlist_clear_double_clear);

  printf("\n");

  RUN_TEST(test_cstl_dlist_set);
  RUN_TEST(test_cstl_dlist_set_out_of_bounds);

  printf("\n");

  RUN_TEST(test_cstl_dlist_get_pos_begin);
  RUN_TEST(test_cstl_dlist_get_pos_end);
  RUN_TEST(test_cstl_dlist_get_pos_middle);
  RUN_TEST(test_cstl_dlist_get_pos_out_of_bounds);

  return UNITY_END();
}
