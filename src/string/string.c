#include "../../include/cstl/cstl_string.h"
#include <stddef.h>
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

size_t cstl_str_length(cstl_string *s) { return s->length; }

size_t cstl_str_capacity(cstl_string *s) { return s->capacity; }

char *cstl_str_data(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  return s->data;
}

bool cstl_str_is_empty(cstl_string *s) { return !s->length; }

// NOTE: change str functions

cstl_string *cstl_str_resize(cstl_string *s, size_t new_len) {

  if (s == NULL) {
    return NULL;
  }

  if (new_len + 1 == s->capacity) {
    return s;
  }

  char *new_str = malloc(new_len + 1);

  if (new_str == NULL) {
    return NULL;
  }

  s->length = (s->length > new_len) ? new_len : s->length;
  s->capacity = new_len + 1;

  new_str[s->length] = '\0';
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

  if (cstr_len == 0) {
    return s;
  }

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

  s->length = res_len;
  s->data[s->length] = '\0';

  return s;
}

cstl_string *cstl_str_append_char(cstl_string *s, int c) {

  if (s == NULL) {
    return NULL;
  }

  if (s->length + 1 >= s->capacity) {
    size_t new_capacity = (s->capacity > 0) ? s->capacity * 2 : 1;
    if (cstl_str_resize(s, new_capacity - 1) == NULL) {
      return NULL;
    }
  }

  s->data[s->length + 1] = '\0';
  s->data[s->length] = c;

  s->length++;

  return s;
}

cstl_string *cstl_str_insert(cstl_string *s, size_t pos, int c) {

  if (s == NULL || pos > s->length) {
    return NULL;
  }

  if (s->length + 1 >= s->capacity) {
    size_t new_capacity = (s->capacity > 0) ? s->capacity * 2 : 2;
    cstl_str_resize(s, new_capacity - 1);
  }

  if (pos == s->length) {
    cstl_str_append_char(s, c);
    return s;
  }

  char *dest = s->data + pos + 1;
  char *src = s->data + pos;
  memmove(dest, src, s->length - pos);

  char *dest_insert = s->data + pos;
  memset(dest_insert, c, sizeof(char));

  s->length++;
  s->data[s->length] = '\0';

  return s;
}

cstl_string *cstl_str_insert_range(cstl_string *s, size_t pos,
                                   const char *c_s) {

  if (s == NULL || c_s == NULL || pos > s->length) {
    return NULL;
  }

  size_t c_s_len = strlen(c_s);

  if (c_s_len == 0) {
    return s;
  }

  size_t new_len = s->length + c_s_len;

  if (new_len > s->capacity) {

    size_t new_capacity = s->capacity;

    while (new_capacity - 1 < new_len) {

      new_capacity = (s->capacity > 0) ? s->capacity * 2 : 1;
      if (cstl_str_resize(s, new_capacity - 1) == NULL) {
        return NULL;
      }
    }
  }

  char *dest = s->data + pos + c_s_len;
  char *src = s->data + pos;
  memmove(dest, src, s->length - pos);

  char *dest_r = s->data + pos;
  memcpy(dest_r, c_s, c_s_len);

  s->length = new_len;
  s->data[s->length] = '\0';

  return s;
}

cstl_string *cstl_str_erase(cstl_string *s, size_t pos) {

  if (s == NULL || s->length == 0 || pos >= s->length) {
    return NULL;
  }

  if (pos == s->length - 1) {
    return cstl_str_pop_back(s);
  }

  char *dest = s->data + pos;
  char *src = s->data + pos + 1;
  size_t move_size = s->length - pos - 1;
  memmove(dest, src, move_size);

  s->length--;
  s->data[s->length] = '\0';

  return s;
}

cstl_string *cstl_str_erase_range(cstl_string *s, size_t pos, size_t len) {

  if (s == NULL || pos >= s->length || pos + len > s->length) {
    return NULL;
  }

  if (len == 0) {
    return s;
  }

  if (pos + len == s->length) {
    s->length -= len;
    s->data[pos] = '\0';
    return s;
  }

  char *dest = s->data + pos;
  char *src = s->data + pos + len;
  size_t move_size = s->length - pos - len;
  memmove(dest, src, move_size);

  s->length -= len;
  s->data[s->length] = '\0';

  return s;
}

cstl_string *cstl_str_clear(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  s->length = 0;
  s->data[0] = '\0';

  return s;
}

cstl_string *cstl_str_shrink_to_fit(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  return cstl_str_resize(s, s->length);
}

cstl_string *cstl_str_pop_back(cstl_string *s) {

  if (s == NULL) {
    return NULL;
  }

  if (s->length == 0) {
    return s;
  }

  s->length--;
  s->data[s->length] = '\0';

  return s;
}

cstl_string *cstl_str_replace(cstl_string *s, const char *substr, size_t pos) {

  if (s == NULL || substr == NULL || pos >= s->length) {
    return NULL;
  }

  size_t substr_len = strlen(substr);

  if (substr_len + pos > s->length) {
    return NULL;
  }

  if (substr_len == 0) {
    return s;
  }

  memcpy(s->data + pos, substr, substr_len);

  return s;
}

const char *cstl_str_find(cstl_string *s, const char *substr) {

  if (s == NULL || substr == NULL) {
    return NULL;
  }

  return strstr(s->data, substr);
}

int cstl_str_compare(cstl_string *s, const char *substr) {
  return strcmp(s->data, substr);
}

void cstl_str_swap(cstl_string **s1, cstl_string **s2) {

  if (s1 == NULL || s2 == NULL) {
    return;
  }

  cstl_string *tmp = &(**s2);
  *s2 = *s1;
  *s1 = &(*tmp);

  size_t *tmp_l = &((*s1)->length);
  (*s1)->length = (*s2)->length;
  (*s2)->length = *tmp_l;

  size_t *tmp_c = &((*s1)->capacity);
  (*s1)->capacity = (*s2)->capacity;
  (*s2)->capacity = *tmp_c;
}

void cstl_str_free(cstl_string *s) { free(s->data); }
