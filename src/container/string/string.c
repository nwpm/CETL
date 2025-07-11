#include "../../../include/cetl/cetl_string.h"
#include "cetl_string_internal.h"
#include <stddef.h>
#include <string.h>

static cetl_string *_cetl_str_ensure_capacity(cetl_string *string) {

  if (string->length + 1 >= string->capacity) {
    size_t new_capacity = string->capacity * CETL_STR_GROW_RATE;
    if (cetl_str_resize(string, new_capacity - 1) == NULL) {
      return NULL;
    }
  }
  return string;
}

static cetl_string *_cetl_str_reserve_until(cetl_string *string,
                                            size_t new_len) {

  if (new_len >= string->capacity) {

    size_t new_capacity = string->capacity;

    while (new_capacity - 1 < new_len) {

      new_capacity = string->capacity * CETL_STR_GROW_RATE;
      if (cetl_str_resize(string, new_capacity - 1) == NULL) {
        return NULL;
      }
    }
  }

  return string;
}

cetl_string *cetl_str_create_uninit(size_t len) {

  cetl_string *string = malloc(sizeof(cetl_string));

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

cetl_string *cetl_str_create_empty() { return cetl_str_create_uninit(0); }

cetl_string *cetl_str_create_filled(size_t len, int ch) {

  cetl_string *string = cetl_str_create_uninit(len);

  if (string == NULL) {
    return NULL;
  }

  memset(string->data, ch, len);

  return string;
}

cetl_string *cetl_str_create_from_cstr(const char *cstr) {

  if (cstr == NULL) {
    return NULL;
  }

  size_t len_cstr = strlen(cstr);

  cetl_string *string = cetl_str_create_uninit(len_cstr);

  if (string == NULL) {
    return NULL;
  }

  memcpy(string->data, cstr, len_cstr);

  return string;
}

cetl_string *cetl_str_create_copy(const cetl_string *src_string) {

  if (src_string == NULL) {
    return NULL;
  }

  cetl_string *new_string = cetl_str_create_uninit(src_string->length);

  if (new_string == NULL) {
    return NULL;
  }

  memcpy(new_string->data, src_string->data, src_string->length);

  return new_string;
}

size_t cetl_str_length(const cetl_string *string) { return string->length; }

size_t cetl_str_capacity(const cetl_string *string) { return string->capacity; }

char *cetl_str_data(const cetl_string *string) {

  if (string == NULL) {
    return NULL;
  }

  return string->data;
}

bool cetl_str_is_empty(const cetl_string *string) { return !string->length; }

cetl_string *cetl_str_resize(cetl_string *string, size_t new_len) {

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

cetl_string *cetl_str_append_cstr(cetl_string *string, const char *cstr) {

  if (string == NULL || cstr == NULL) {
    return NULL;
  }

  size_t cstr_len = strlen(cstr);

  if (cstr_len == 0) {
    return string;
  }

  size_t new_len = string->length + cstr_len;

  if (_cetl_str_reserve_until(string, new_len) == NULL) {
    return NULL;
  }

  memcpy(string->data + string->length, cstr, cstr_len);

  string->length = new_len;
  string->data[string->length] = '\0';

  return string;
}

cetl_string *cetl_str_append_char(cetl_string *string, int ch) {

  if (string == NULL || _cetl_str_ensure_capacity(string) == NULL) {
    return NULL;
  }

  string->data[string->length + 1] = '\0';
  string->data[string->length] = ch;

  string->length++;

  return string;
}

cetl_string *cetl_str_insert_char(cetl_string *string, size_t pos, int ch) {

  if (string == NULL || pos > string->length ||
      _cetl_str_ensure_capacity(string)) {
    return NULL;
  }

  if (pos == string->length) {
    cetl_str_append_char(string, ch);
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

cetl_string *cetl_str_insert_cstr(cetl_string *string, size_t pos,
                                  const char *cstr) {

  if (string == NULL || cstr == NULL || pos > string->length) {
    return NULL;
  }

  size_t cstr_len = strlen(cstr);

  if (cstr_len == 0) {
    return string;
  }

  size_t new_len = string->length + cstr_len;

  if (_cetl_str_reserve_until(string, new_len) == NULL) {
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

cetl_string *cetl_str_erase_char(cetl_string *string, size_t pos) {

  if (string == NULL || string->length == 0 || pos >= string->length) {
    return NULL;
  }

  if (pos == string->length - 1) {
    return cetl_str_pop_back(string);
  }

  char *dest = string->data + pos;
  char *src = string->data + pos + 1;
  size_t move_size = string->length - pos - 1;
  memmove(dest, src, move_size);

  string->length--;
  string->data[string->length] = '\0';

  return string;
}

cetl_string *cetl_str_erase_span(cetl_string *string, size_t pos, size_t len) {

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

cetl_string *cetl_str_clear(cetl_string *string) {

  if (string == NULL) {
    return NULL;
  }

  string->length = 0;
  string->data[0] = '\0';

  return string;
}

cetl_string *cetl_str_shrink_to_fit(cetl_string *string) {

  if (string == NULL) {
    return NULL;
  }

  return cetl_str_resize(string, string->length);
}

cetl_string *cetl_str_pop_back(cetl_string *string) {

  if (string == NULL || string->length == 0) {
    return string;
  }

  string->length--;
  string->data[string->length] = '\0';

  return string;
}

cetl_string *cetl_str_replace(cetl_string *string, size_t pos, size_t len,
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

const char *cetl_str_find(const cetl_string *string, const char *substr) {

  if (string == NULL || substr == NULL) {
    return NULL;
  }

  return strstr(string->data, substr);
}

int cetl_str_compare(const cetl_string *string, const char *substr) {
  return strcmp(string->data, substr);
}

void cetl_str_swap(cetl_string **string1, cetl_string **string2) {

  if (string1 == NULL || string2 == NULL) {
    return;
  }

  cetl_string *tmp = *string1;
  *string1 = *string2;
  *string2 = tmp;
}

void cetl_str_free(cetl_string *string) {
  free(string->data);
  free(string);
}
