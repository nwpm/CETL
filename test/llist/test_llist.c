#include "../../include/cetl/cetl_llist.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include "../../src/utils/element/cetl_element.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _cetl_node {

  cetl_ptr_t data;
  struct _cetl_node *next;

} _cetl_node;

struct cetl_llist {

  cetl_size_t size;
  struct _cetl_node *head;
  struct _cetl_node *tail;
  const struct cetl_element *type;
};

void setUp() {}
void tearDown() {}

cetl_element *create_int_type() {

  cetl_element *int_type = malloc(sizeof(cetl_element));

  if (int_type == NULL) {
    return NULL;
  }

  int_type->size = sizeof(int);
  int_type->ctor = NULL;
  int_type->dtor = NULL;
  int_type->cmp = NULL;

  return int_type;
}

void llist_fill_with_int(cetl_llist *llist, cetl_size_t size) {
  for (cetl_size_t i = 0; i < size; ++i) {
    cetl_llist_push_back(llist, &i);
  }
}

cetl_bool_t llist_int_check_is_data_correct(const cetl_llist *src_llist,
                                            const cetl_llist *copy_llist,
                                            cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    int current_orig = *((int *)cetl_llist_get(src_llist, i));
    int current_copy = *((int *)cetl_llist_get(copy_llist, i));
    if (current_orig != current_copy) {
      return CETL_FALSE;
    }
  }

  return CETL_TRUE;
}

typedef struct OwnedString {

  cetl_str_t data;
  cetl_size_t len;

} OwnedString;

cetl_ptr_t simple_string_ctor(cetl_ptr_t dest, cetl_cptr_t data) {
  OwnedString *target = (OwnedString *)dest;
  OwnedString *source = (OwnedString *)data;
  target->data = strdup(source->data);
  target->len = source->len;
  return target;
}

cetl_void_t simple_string_dtor(cetl_ptr_t data) {
  OwnedString *target = (OwnedString *)data;
  free(target->data);
}

cetl_element *create_owned_string_type() {

  cetl_element *t = malloc(sizeof(cetl_element));

  if (t == NULL) {
    return NULL;
  }

  t->size = sizeof(OwnedString);
  t->ctor = simple_string_ctor;
  t->dtor = simple_string_dtor;
  t->cmp = NULL;

  return t;
}

OwnedString *make_owned_string(cetl_str_t s, cetl_size_t len) {

  OwnedString *owned_string = malloc(sizeof(OwnedString));

  if (owned_string == NULL) {
    return NULL;
  }

  owned_string->data = s;
  owned_string->len = len;

  return owned_string;
}

typedef struct TestHeapStr {
  cetl_str_t s;
  cetl_int_t x;
  cetl_float_t y;
} TestHeapStr;

cetl_ptr_t test_heap_str_ctor(cetl_ptr_t dest, cetl_cptr_t data) {
  TestHeapStr *target = (TestHeapStr *)dest;
  TestHeapStr *source = (TestHeapStr *)data;
  target->x = source->x;
  target->y = source->y;
  target->s = strdup(source->s);
  return target;
}

cetl_void_t test_heap_str_dtor(cetl_ptr_t data) {
  TestHeapStr *target = (TestHeapStr *)data;
  free(target->s);
  free(target);
}

cetl_element *create_test_heap_str_type() {

  cetl_element *test_heap_str_type = malloc(sizeof(cetl_element));

  if (test_heap_str_type == NULL) {
    return NULL;
  }

  test_heap_str_type->size = sizeof(TestHeapStr);
  test_heap_str_type->ctor = test_heap_str_ctor;
  test_heap_str_type->dtor = test_heap_str_dtor;
  test_heap_str_type->cmp = NULL;

  return test_heap_str_type;
}

TestHeapStr *make_test_heap_str(cetl_int_t x, cetl_float_t y, cetl_str_t s) {

  TestHeapStr *test_object = malloc(sizeof(TestHeapStr));

  if (test_object == NULL) {
    return NULL;
  }

  test_object->x = x;
  test_object->y = y;
  test_object->s = s;

  return test_object;
}

cetl_str_t create_string_of_char(cetl_size_t len, cetl_int_t ch) {

  cetl_str_t str = malloc(len + 1);

  if (str == NULL) {
    return NULL;
  }

  str[len] = '\0';
  memset(str, ch, len);

  return str;
}

cetl_void_t llist_fill_with_test_heap_str(cetl_llist *llist, cetl_size_t size,
                                          cetl_size_t s_len, cetl_int_t ch) {

  for (cetl_size_t i = 0; i < size; ++i) {

    cetl_str_t str = create_string_of_char(s_len, ch);
    TestHeapStr *test_heap_struct = make_test_heap_str(i, i, str);
    cetl_llist_push_back(llist, test_heap_struct);
    // NOTE: ???
    free(test_heap_struct->s);
    free(test_heap_struct);
  }
}

cetl_bool_t
llist_heap_struct_check_is_data_correct(const cetl_llist *src_llist,
                                        const cetl_llist *copy_llist,
                                        cetl_size_t size) {

  for (cetl_size_t i = 0; i < size; ++i) {
    TestHeapStr *current_orig = (TestHeapStr *)cetl_llist_get(src_llist, i);
    TestHeapStr *current_copy = (TestHeapStr *)cetl_llist_get(copy_llist, i);

    TEST_ASSERT_EQUAL_INT(current_orig->x, current_copy->x);
    TEST_ASSERT_EQUAL_FLOAT(current_orig->y, current_copy->y);
    TEST_ASSERT_EQUAL_STRING(current_orig->s, current_copy->s);
  }

  return CETL_TRUE;
}

// Create empty
//-------------------------------------

void test_create_empty_type_int() {

  cetl_element *int_type = create_int_type();
  TEST_ASSERT_NOT_NULL(int_type);

  cetl_llist *list = cetl_llist_create_empty(int_type);
  TEST_ASSERT_NOT_NULL(list);

  TEST_ASSERT_NULL(list->head);
  TEST_ASSERT_NULL(list->tail);
  TEST_ASSERT_EQUAL_size_t(0, list->size);
  TEST_ASSERT_EQUAL_PTR(int_type, list->type);

  cetl_llist_free(list);
  free(int_type);
}

void test_create_empty_type_owned_string() {

  cetl_element *str_type = create_owned_string_type();
  TEST_ASSERT_NOT_NULL(str_type);

  cetl_llist *list = cetl_llist_create_empty(str_type);
  TEST_ASSERT_NOT_NULL(list);

  TEST_ASSERT_NULL(list->head);
  TEST_ASSERT_NULL(list->tail);
  TEST_ASSERT_EQUAL_size_t(list->size, 0);
  TEST_ASSERT_EQUAL_PTR(list->type, str_type);

  cetl_llist_free(list);
  free(str_type);
}

void test_create_empty_type_test_heap_str() {

  cetl_element *struct_type = create_test_heap_str_type();
  TEST_ASSERT_NOT_NULL(struct_type);

  cetl_llist *list = cetl_llist_create_empty(struct_type);
  TEST_ASSERT_NOT_NULL(list);

  TEST_ASSERT_NULL(list->head);
  TEST_ASSERT_NULL(list->tail);
  TEST_ASSERT_EQUAL_size_t(list->size, 0);
  TEST_ASSERT_EQUAL_PTR(list->type, struct_type);

  cetl_llist_free(list);
  free(struct_type);
}

// Create copy
//-------------------------------------

void test_create_copy_from_size_0_type_int() {

  cetl_element *int_type = create_int_type();
  TEST_ASSERT_NOT_NULL(int_type);

  cetl_llist *src_list = cetl_llist_create_empty(int_type);
  TEST_ASSERT_NOT_NULL(src_list);

  cetl_llist *copy_list = cetl_llist_create_copy(src_list);
  TEST_ASSERT_NOT_NULL(copy_list);

  TEST_ASSERT_NULL(copy_list->head);
  TEST_ASSERT_NULL(copy_list->tail);
  TEST_ASSERT_EQUAL_size_t(copy_list->size, 0);
  TEST_ASSERT_EQUAL_PTR(copy_list->type, int_type);

  cetl_llist_free(src_list);
  cetl_llist_free(copy_list);
  free(int_type);
}

void test_create_copy_from_size_10_type_int() {

  cetl_element *int_type = create_int_type();
  TEST_ASSERT_NOT_NULL(int_type);

  cetl_llist *src_llist = cetl_llist_create_empty(int_type);
  TEST_ASSERT_NOT_NULL(src_llist);

  llist_fill_with_int(src_llist, 10);

  cetl_llist *copy_llist = cetl_llist_create_copy(src_llist);
  TEST_ASSERT_NOT_NULL(copy_llist);

  TEST_ASSERT_NOT_NULL(copy_llist->head);
  TEST_ASSERT_NOT_NULL(copy_llist->tail);
  TEST_ASSERT_EQUAL_size_t(copy_llist->size, 10);
  TEST_ASSERT_EQUAL_PTR(copy_llist->type, int_type);

  TEST_ASSERT_TRUE(llist_int_check_is_data_correct(src_llist, copy_llist, 10));

  cetl_llist_free(src_llist);
  cetl_llist_free(copy_llist);
  free(int_type);
}

void test_create_copy_from_size_1000_type_int() {

  cetl_element *int_type = create_int_type();
  TEST_ASSERT_NOT_NULL(int_type);

  cetl_llist *src_llist = cetl_llist_create_empty(int_type);
  TEST_ASSERT_NOT_NULL(src_llist);

  llist_fill_with_int(src_llist, 1000);

  cetl_llist *copy_llist = cetl_llist_create_copy(src_llist);
  TEST_ASSERT_NOT_NULL(copy_llist);

  TEST_ASSERT_NOT_NULL(copy_llist->head);
  TEST_ASSERT_NOT_NULL(copy_llist->tail);
  TEST_ASSERT_EQUAL_size_t(copy_llist->size, 1000);
  TEST_ASSERT_EQUAL_PTR(copy_llist->type, int_type);

  TEST_ASSERT_TRUE(
      llist_int_check_is_data_correct(src_llist, copy_llist, 1000));

  cetl_llist_free(src_llist);
  cetl_llist_free(copy_llist);
  free(int_type);
}

void test_create_copy_from_size_10_type_test_heap_str() {

  cetl_element *struct_type = create_test_heap_str_type();
  TEST_ASSERT_NOT_NULL(struct_type);

  cetl_llist *src_llist = cetl_llist_create_empty(struct_type);
  TEST_ASSERT_NOT_NULL(src_llist);

  cetl_size_t num_test_heap_str_objects = 10;
  cetl_size_t str_len = 15;
  cetl_uchar_t char_for_string = 'S';

  llist_fill_with_test_heap_str(src_llist, num_test_heap_str_objects, str_len,
                                char_for_string);

  cetl_llist *copy_llist = cetl_llist_create_copy(src_llist);
  TEST_ASSERT_NOT_NULL(copy_llist);

  TEST_ASSERT_NOT_NULL(copy_llist->head);
  TEST_ASSERT_NOT_NULL(copy_llist->tail);
  TEST_ASSERT_EQUAL_size_t(copy_llist->size, 10);
  TEST_ASSERT_EQUAL_PTR(copy_llist->type, struct_type);

  TEST_ASSERT_TRUE(
      llist_heap_struct_check_is_data_correct(src_llist, copy_llist, 10));

  cetl_llist_free(src_llist);
  cetl_llist_free(copy_llist);
  free(struct_type);
}

void test_create_copy_from_size_1000_type_test_heap_str() {

  cetl_element *struct_type = create_test_heap_str_type();
  TEST_ASSERT_NOT_NULL(struct_type);

  cetl_llist *src_llist = cetl_llist_create_empty(struct_type);
  TEST_ASSERT_NOT_NULL(src_llist);

  cetl_size_t num_test_heap_str_objects = 1000;
  cetl_size_t str_len = 15;
  cetl_uchar_t char_for_string = 'S';

  llist_fill_with_test_heap_str(src_llist, num_test_heap_str_objects, str_len,
                                char_for_string);

  cetl_llist *copy_llist = cetl_llist_create_copy(src_llist);
  TEST_ASSERT_NOT_NULL(copy_llist);

  TEST_ASSERT_NOT_NULL(copy_llist->head);
  TEST_ASSERT_NOT_NULL(copy_llist->tail);
  TEST_ASSERT_EQUAL_size_t(copy_llist->size, 1000);
  TEST_ASSERT_EQUAL_PTR(copy_llist->type, struct_type);

  TEST_ASSERT_TRUE(
      llist_heap_struct_check_is_data_correct(src_llist, copy_llist, 1000));

  cetl_llist_free(src_llist);
  cetl_llist_free(copy_llist);
  free(struct_type);
}

/*
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

  for (int i = 0; i < 5; ++i) {
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

  for (int i = 0; i < 5; ++i) {
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

  for (int i = 0; i < 5; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  TEST_ASSERT_NOT_NULL(cetl_llist_erase(l, l->size / 2));

  TEST_ASSERT_EQUAL_size_t(4, l->size);
  TEST_ASSERT_EQUAL_size_t(sizeof(int), l->elem_size);
  TEST_ASSERT_EQUAL_INT(3, *((int *)cetl_llist_get(l, 2)));

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

void test_cetl_llist_set() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  int s_elem = 1;

  TEST_ASSERT_NOT_NULL(cetl_llist_set(l, 0, &s_elem));
  TEST_ASSERT_EQUAL_INT(s_elem, *((int *)l->head->data));

  cetl_llist_free(l);
}

void test_cetl_llist_set_out_of_bounds() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (size_t i = 0; i < 4; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  int s_elem = 1;
  TEST_ASSERT_NULL(cetl_llist_set(l, 100, &s_elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_begin() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (int i = 0; i < 5; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, 0);

  TEST_ASSERT_EQUAL_INT(0, *((int *)elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_end() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (int i = 0; i < 5; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, l->size - 1);

  TEST_ASSERT_EQUAL_INT(4, *((int *)elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_middle() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (int i = 0; i < 5; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, l->size / 2);

  TEST_ASSERT_EQUAL_INT(2, *((int *)elem));

  cetl_llist_free(l);
}

void test_cetl_llist_get_pos_out_of_bounds() {

  cetl_llist *l = cetl_llist_create_empty();

  TEST_ASSERT_NOT_NULL(l);

  for (int i = 0; i < 5; ++i) {
    cetl_llist_push_back(l, &i, sizeof(int));
  }

  void *elem = cetl_llist_get(l, 100);

  TEST_ASSERT_NULL(elem);

  cetl_llist_free(l);
}

*/

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_create_empty_type_int);
  RUN_TEST(test_create_empty_type_owned_string);
  RUN_TEST(test_create_empty_type_test_heap_str);

  printf("\n");

  RUN_TEST(test_create_copy_from_size_0_type_int);
  RUN_TEST(test_create_copy_from_size_10_type_int);
  RUN_TEST(test_create_copy_from_size_1000_type_int);

  RUN_TEST(test_create_copy_from_size_10_type_test_heap_str);
  RUN_TEST(test_create_copy_from_size_1000_type_test_heap_str);

  /*
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
  */

  return UNITY_END();
}
