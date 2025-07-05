#include "../../include/cstl/cstl_vector.h"
#include "../../include/external/unity/unity.h"
#include "../../include/external/unity/unity_internals.h"
#include "../../src/vector/cstl_vector_internal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _POSIX_C_SOURCE 200809L

cstl_type *_cstl_create_int_type() {
  cstl_type *t = malloc(sizeof(cstl_type));

  if (t == NULL) {
    return NULL;
  }

  t->size = sizeof(int);
  t->ctor = NULL;
  t->dtor = NULL;
  t->cmp = NULL;

  return t;
}

void _cstl_vec_add_int(cstl_vector *v, size_t size) {
  for (size_t i = 0; i < size; ++i) {
    cstl_vec_push_back(v, &i);
  }
}

cstl_type *_cstl_create_double_type() {
  cstl_type *t = malloc(sizeof(cstl_type));

  if (t == NULL) {
    return NULL;
  }

  t->size = sizeof(double);
  t->ctor = NULL;
  t->dtor = NULL;
  t->cmp = NULL;

  return t;
}

typedef struct UserTypeSimple {
  char *s;
  int x;
  float y;
} UserTypeSimple;

void *user_type_simple_ctor(void *dest, const void *data) {
  UserTypeSimple *target = (UserTypeSimple *)dest;
  UserTypeSimple *source = (UserTypeSimple *)data;
  target->x = source->x;
  target->y = source->y;
  target->s = strdup(source->s);
  return target;
}

void user_type_simple_dtor(void *data) {
  UserTypeSimple *target = (UserTypeSimple *)data;
  free(target->s);
}

cstl_type *_cstl_create_user_type_simple() {
  cstl_type *t = malloc(sizeof(cstl_type));

  if (t == NULL) {
    return NULL;
  }

  t->size = sizeof(UserTypeSimple);
  t->ctor = user_type_simple_ctor;
  t->dtor = user_type_simple_dtor;
  t->cmp = NULL;

  return t;
}

UserTypeSimple *_cstl_create_uss(int x, float y, char *s) {

  UserTypeSimple *uss = malloc(sizeof(UserTypeSimple));

  if (uss == NULL) {
    return NULL;
  }

  uss->x = x;
  uss->y = y;
  uss->s = s;

  return uss;
}

char *_cstl_create_str(size_t len) {

  char *str = malloc(len);

  if (str == NULL) {
    return NULL;
  }

  str[len - 1] = '\0';
  memset(str, 65, len - 1);

  return str;
}

void _cstl_vec_add_uss(cstl_vector *v, size_t size) {

  for (size_t i = 0; i < size; ++i) {

    char *str = _cstl_create_str(10);
    UserTypeSimple *uss = _cstl_create_uss(i, i, str);
    cstl_vec_push_back(v, uss);
    free(uss->s);
    free(uss);
  }
}

typedef struct SimpleString {

  char *data;
  size_t len;

} SimpleString;

void *simple_string_ctor(void *dest, const void *data) {
  SimpleString *target = (SimpleString *)dest;
  SimpleString *source = (SimpleString *)data;
  target->data = strdup(source->data);
  target->len = source->len;
  return target;
}

void simple_string_dtor(void *data) {
  SimpleString *target = (SimpleString *)data;
  free(target->data);
}

cstl_type *_cstl_create_simple_string_type() {
  cstl_type *t = malloc(sizeof(cstl_type));

  if (t == NULL) {
    return NULL;
  }

  t->size = sizeof(SimpleString);
  t->ctor = simple_string_ctor;
  t->dtor = simple_string_dtor;
  t->cmp = NULL;

  return t;
}

cstl_vector *_cstl_create_empty_string_vector() {

  cstl_type *t = _cstl_create_simple_string_type();

  if (t == NULL) {
    return NULL;
  }

  cstl_vector *v = cstl_vec_create_empty(t);

  if (v == NULL) {
    return NULL;
  }

  return v;
}

void setUp() {}
void tearDown() {}

//-------------------------------------

void test_create_empty_type_int() {

  cstl_type *t = _cstl_create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CSTL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cstl_vec_free(v);
  free(t);
}

void test_create_empty_type_sstr() {

  cstl_type *t = _cstl_create_simple_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CSTL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cstl_vec_free(v);
  free(t);
}

void test_create_empty_type_uss() {

  cstl_type *t = _cstl_create_user_type_simple();

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  TEST_ASSERT_EQUAL_size_t(v->capacity, CSTL_VEC_START_CAPACITY);
  TEST_ASSERT_EQUAL_size_t(v->size, 0);
  TEST_ASSERT_NOT_NULL(v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, t);

  cstl_vec_free(v);
  free(t);
}

//-----------------------------

void test_create_copy_from_size_0_type_int() {

  cstl_type *t = _cstl_create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_NOT_NULL(c_v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);

  cstl_vec_free(c_v);
  cstl_vec_free(v);
  free(t);
}

void test_create_copy_from_size_0_type_sstr() {

  cstl_type *t = _cstl_create_simple_string_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_NOT_NULL(c_v->data);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);

  cstl_vec_free(c_v);
  cstl_vec_free(v);
  free(t);
}

void test_create_copy_from_size_10_type_int() {

  cstl_type *t = _cstl_create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  _cstl_vec_add_int(v, 10);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 10; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);
    TEST_ASSERT_EQUAL_INT(*((int *)current_orig), *((int *)current_copy));
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

void test_create_copy_from_size_1000_type_int() {

  cstl_type *t = _cstl_create_int_type();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  _cstl_vec_add_int(v, 1000);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 1000; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);
    TEST_ASSERT_EQUAL_INT(*((int *)current_orig), *((int *)current_copy));
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

void test_create_copy_from_size_10_type_uss() {

  cstl_type *t = _cstl_create_user_type_simple();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  _cstl_vec_add_uss(v, 10);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 10; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);

    TEST_ASSERT_EQUAL_INT(((UserTypeSimple *)current_orig)->x,
                          ((UserTypeSimple *)current_copy)->x);

    TEST_ASSERT_EQUAL_FLOAT(((UserTypeSimple *)current_orig)->y,
                            ((UserTypeSimple *)current_orig)->y);

    TEST_ASSERT_EQUAL_STRING(((UserTypeSimple *)current_orig)->s,
                             ((UserTypeSimple *)current_orig)->s);
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

void test_create_copy_from_size_1000_type_uss() {

  cstl_type *t = _cstl_create_user_type_simple();

  TEST_ASSERT_NOT_NULL(t);

  cstl_vector *v = cstl_vec_create_empty(t);

  TEST_ASSERT_NOT_NULL(v);

  _cstl_vec_add_uss(v, 1000);

  cstl_vector *c_v = cstl_vec_create_copy(v);

  TEST_ASSERT_NOT_NULL(c_v);

  TEST_ASSERT_EQUAL_size_t(v->size, c_v->size);
  TEST_ASSERT_EQUAL_size_t(v->capacity, c_v->capacity);
  TEST_ASSERT_EQUAL_PTR(v->type, c_v->type);
  TEST_ASSERT_NOT_EQUAL(v->data, c_v->data);

  for (size_t i = 0; i < 1000; ++i) {
    void *current_orig = cstl_vec_get(v, i);
    void *current_copy = cstl_vec_get(c_v, i);

    TEST_ASSERT_EQUAL_INT(((UserTypeSimple *)current_orig)->x,
                          ((UserTypeSimple *)current_copy)->x);

    TEST_ASSERT_EQUAL_FLOAT(((UserTypeSimple *)current_orig)->y,
                            ((UserTypeSimple *)current_orig)->y);

    TEST_ASSERT_EQUAL_STRING(((UserTypeSimple *)current_orig)->s,
                             ((UserTypeSimple *)current_orig)->s);
  }

  cstl_vec_free(v);
  cstl_vec_free(c_v);
  free(t);
}

int main() {

  UNITY_BEGIN();

  printf("\n");

  RUN_TEST(test_create_empty_type_int);
  RUN_TEST(test_create_empty_type_sstr);
  RUN_TEST(test_create_empty_type_uss);

  printf("\n");

  RUN_TEST(test_create_copy_from_size_0_type_int);
  RUN_TEST(test_create_copy_from_size_10_type_int);
  RUN_TEST(test_create_copy_from_size_1000_type_int);

  RUN_TEST(test_create_copy_from_size_0_type_sstr);

  RUN_TEST(test_create_copy_from_size_10_type_uss);
  RUN_TEST(test_create_copy_from_size_1000_type_uss);

  return UNITY_END();
}
