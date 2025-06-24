#include "../../include/cstl/cstl_string.h"
#include <string.h>

// NOTE: create functions

cstl_string *cstl_str_create(size_t len) {

  cstl_string *s = malloc(sizeof(cstl_string));

  if (s == NULL) {
    return NULL;
  }

  char *str = malloc(len + 1);

  if (str == NULL) {
    free(s);
    return NULL;
  }

  str[len] = '\0';

  s->data = str;
  s->capacity = len + 1;
  s->length = len;

  return s;
}

cstl_string *cstl_str_create_empty() { return cstl_str_create(0); }

cstl_string *cstl_str_create_filled(size_t len, int c) {

  cstl_string *s = cstl_str_create(len);

  if (s == NULL) {
    return NULL;
  }

  memset(s->data, c, len);

  return s;
}

cstl_string *cstl_str_create_from_cstr(const char *str) {

  if (str == NULL) {
    return NULL;
  }

  size_t len_str = strlen(str);

  cstl_string *s = cstl_str_create(len_str);

  if (s == NULL) {
    return NULL;
  }

  memcpy(s->data, str, len_str);

  return s;
}

cstl_string *cstl_str_create_copy(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  cstl_string *s_copy = cstl_str_create(s->length);

  if (s_copy == NULL) {
    return NULL;
  }

  memcpy(s_copy->data, s->data, s->length);

  return s_copy;
}

// NOTE: get info functions

size_t cstl_str_length(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  return s->length;
}

size_t cstl_str_capacity(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  return s->capacity;
}

char *cstl_str_data(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  return s->data;
}

bool cstl_str_is_empty(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  return !s->length;
}

// NOTE: change str functions

cstl_string *cstl_str_resize(cstl_string *s, size_t new_len) {

  if (s == NULL) {
    return NULL;
  }

  if (new_len == s->length) {
    return s;
  }

  char *new_str = malloc(new_len + 1);

  if (new_str == NULL) {
    return NULL;
  }

  s->length = (s->length > new_len) ? new_len : s->length;
  s->capacity = new_len + 1;

  new_str[new_len] = '\0';
  memmove(new_str, s->data, s->length);

  free(s->data);
  s->data = new_str;

  return s;
}

cstl_string *cstl_str_append(cstl_string *s, const char *c_s) {

  if (s == NULL || c_s == NULL) {
    return NULL;
  }

  size_t cstr_len = strlen(c_s);
  size_t res_len = s->length + cstr_len;

  if (cstr_len + s->length >= s->capacity) {

    size_t new_capacity = s->capacity;
    while (new_capacity < res_len + 1) {

      new_capacity = (s->capacity > 0) ? s->capacity * 2 : 1;

      if (cstl_str_resize(s, new_capacity - 1) == NULL) {
        return NULL;
      }
    }
  }

  memcpy(s->data + s->length, c_s, cstr_len);

  return s;
}

cstl_string *cstl_str_append_char(cstl_string *s, int c) {

  if (s == NULL) {
    return NULL;
  }

  if (s->length == s->capacity - 1) {
    size_t new_capacity = (s->capacity > 0) ? s->capacity * 2 : 1;
    if (cstl_str_resize(s, new_capacity) == NULL) {
      return NULL;
    }
  }

  s->data[s->length + 1] = '\0';
  s->data[s->length] = c;

  s->length++;

  return s;
}

cstl_string *cstl_str_clear(cstl_string *s){
  
  if(s == NULL){
    return NULL;
  }

  s->length = 0;

  return s;
}

void cstl_str_free(cstl_string *s) { free(s->data); }
