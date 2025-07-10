#include "../../include/cstl/cstl_string.h"
#include "cstl_string_internal.h"
#include <stddef.h>
#include <string.h>

static cstl_string *_cstl_str_ensure_capacity(cstl_string *string) {

  if (string->length + 1 >= string->capacity) {
    size_t new_capacity = string->capacity * CSTL_STR_GROW_RATE;
    if (cstl_str_resize(string, new_capacity - 1) == NULL) {
      return NULL;
    }
  }
  return string;
}

static cstl_string *_cstl_str_reserve_until(cstl_string *string,
                                            size_t new_len) {

  if (new_len >= string->capacity) {

    size_t new_capacity = string->capacity;

    while (new_capacity - 1 < new_len) {

      new_capacity = string->capacity * CSTL_STR_GROW_RATE;
      if (cstl_str_resize(string, new_capacity - 1) == NULL) {
        return NULL;
      }
    }
  }

  return string;
}

cstl_string *cstl_str_create_uninit(size_t len) {

  cstl_string *string = malloc(sizeof(cstl_string));

  if (string == NULL) {
    return NULL;
  }

  char *cstr = malloc(len + 1);

  if (cstr == NULL) {
    free(string);
    return NULL;
  }

  cstr[len] = '\0';

  string->data = cstr;
  string->capacity = len + 1;
  string->length = len;

  return string;
}

cstl_string *cstl_str_create_empty() { return cstl_str_create_uninit(0); }

cstl_string *cstl_str_create_filled(size_t len, int ch) {

  cstl_string *string = cstl_str_create_uninit(len);

  if (string == NULL) {
    return NULL;
  }

  memset(string->data, ch, len);

  return string;
}

cstl_string *cstl_str_create_from_cstr(const char *cstr) {

  if (cstr == NULL) {
    return NULL;
  }

  size_t len_cstr = strlen(cstr);

  cstl_string *string = cstl_str_create_uninit(len_cstr);

  if (string == NULL) {
    return NULL;
  }

  memcpy(string->data, cstr, len_cstr);

  return string;
}

cstl_string *cstl_str_create_copy(const cstl_string *src_string) {

  if (src_string == NULL) {
    return NULL;
  }

  cstl_string *new_string = cstl_str_create_uninit(src_string->length);

  if (new_string == NULL) {
    return NULL;
  }

  memcpy(new_string->data, src_string->data, src_string->length);

  return new_string;
}

size_t cstl_str_length(const cstl_string *string) { return string->length; }

size_t cstl_str_capacity(const cstl_string *string) { return string->capacity; }

char *cstl_str_data(const cstl_string *string) {

  if (string == NULL) {
    return NULL;
  }

  return string->data;
}

bool cstl_str_is_empty(const cstl_string *string) { return !string->length; }

cstl_string *cstl_str_resize(cstl_string *string, size_t new_len) {

  if (string == NULL || (new_len + 1) == string->capacity) {
    return string;
  }

  char *new_ptr = malloc(new_len + 1);

  if (new_ptr == NULL) {
    return NULL;
  }

  string->length = (string->length > new_len) ? new_len : string->length;
  string->capacity = new_len + 1;

  new_ptr[string->length] = '\0';
  memmove(new_ptr, string->data, string->length);

  free(string->data);
  string->data = new_ptr;

  return string;
}

cstl_string *cstl_str_append_cstr(cstl_string *string, const char *cstr) {

  if (string == NULL || cstr == NULL) {
    return NULL;
  }

  size_t cstr_len = strlen(cstr);

  if (cstr_len == 0) {
    return string;
  }

  size_t new_len = string->length + cstr_len;

  if (_cstl_str_reserve_until(string, new_len) == NULL) {
    return NULL;
  }

  memcpy(string->data + string->length, cstr, cstr_len);

  string->length = new_len;
  string->data[string->length] = '\0';

  return string;
}

cstl_string *cstl_str_append_char(cstl_string *string, int ch) {

  if (string == NULL || _cstl_str_ensure_capacity(string) == NULL) {
    return NULL;
  }

  string->data[string->length + 1] = '\0';
  string->data[string->length] = ch;

  string->length++;

  return string;
}

cstl_string *cstl_str_insert_char(cstl_string *string, size_t pos, int ch) {

  if (string == NULL || pos > string->length ||
      _cstl_str_ensure_capacity(string)) {
    return NULL;
  }

  if (pos == string->length) {
    cstl_str_append_char(string, ch);
    return string;
  }

  char *dest = string->data + pos + 1;
  char *src = string->data + pos;
  memmove(dest, src, string->length - pos);

  char *dest_insert = string->data + pos;
  memset(dest_insert, ch, sizeof(char));

  string->length++;
  string->data[string->length] = '\0';

  return string;
}

cstl_string *cstl_str_insert_cstr(cstl_string *string, size_t pos,
                                  const char *cstr) {

  if (string == NULL || cstr == NULL || pos > string->length) {
    return NULL;
  }

  size_t cstr_len = strlen(cstr);

  if (cstr_len == 0) {
    return string;
  }

  size_t new_len = string->length + cstr_len;

  if (_cstl_str_reserve_until(string, new_len) == NULL) {
    return NULL;
  }

  char *dest = string->data + pos + cstr_len;
  char *src = string->data + pos;
  memmove(dest, src, string->length - pos);

  char *dest_r = string->data + pos;
  memcpy(dest_r, cstr, cstr_len);

  string->length = new_len;
  string->data[string->length] = '\0';

  return string;
}

cstl_string *cstl_str_erase_char(cstl_string *string, size_t pos) {

  if (string == NULL || string->length == 0 || pos >= string->length) {
    return NULL;
  }

  if (pos == string->length - 1) {
    return cstl_str_pop_back(string);
  }

  char *dest = string->data + pos;
  char *src = string->data + pos + 1;
  size_t move_size = string->length - pos - 1;
  memmove(dest, src, move_size);

  string->length--;
  string->data[string->length] = '\0';

  return string;
}

cstl_string *cstl_str_erase_span(cstl_string *string, size_t pos, size_t len) {

  if (string == NULL || pos >= string->length || pos + len > string->length) {
    return NULL;
  }

  if (len == 0) {
    return string;
  }

  if (pos + len == string->length) {
    string->length -= len;
    string->data[pos] = '\0';
    return string;
  }

  char *dest = string->data + pos;
  char *src = string->data + pos + len;
  size_t move_size = string->length - pos - len;
  memmove(dest, src, move_size);

  string->length -= len;
  string->data[string->length] = '\0';

  return string;
}

cstl_string *cstl_str_clear(cstl_string *string) {

  if (string == NULL) {
    return NULL;
  }

  string->length = 0;
  string->data[0] = '\0';

  return string;
}

cstl_string *cstl_str_shrink_to_fit(cstl_string *string) {

  if (string == NULL) {
    return NULL;
  }

  return cstl_str_resize(string, string->length);
}

cstl_string *cstl_str_pop_back(cstl_string *string) {

  if (string == NULL || string->length == 0) {
    return string;
  }

  string->length--;
  string->data[string->length] = '\0';

  return string;
}

cstl_string *cstl_str_replace(cstl_string *string, size_t pos, size_t len,
                              const char *substr) {

  if (string == NULL || substr == NULL || (len + pos > string->length)) {
    return NULL;
  }

  if (len == 0) {
    return string;
  }

  memcpy(string->data + pos, substr, len);

  return string;
}

const char *cstl_str_find(const cstl_string *string, const char *substr) {

  if (string == NULL || substr == NULL) {
    return NULL;
  }

  return strstr(string->data, substr);
}

int cstl_str_compare(const cstl_string *string, const char *substr) {
  return strcmp(string->data, substr);
}

void cstl_str_swap(cstl_string **string1, cstl_string **string2) {

  if (string1 == NULL || string2 == NULL) {
    return;
  }

  cstl_string *tmp = *string1;
  *string1 = *string2;
  *string2 = tmp;
}

void cstl_str_free(cstl_string *string) {
  free(string->data);
  free(string);
}
