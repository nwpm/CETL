#include "../../../include/cetl/cetl_cdlist.h"
#include "../../utils/element/cetl_element.h"
#include "../../utils/iterator/iterator.h"

#include <stdlib.h>
#include <string.h>

// TODO: take base form dlist

typedef struct _cetl_dnode {

  cetl_ptr_t data;
  struct _cetl_dnode *next;
  struct _cetl_dnode *prev;

} _cetl_dnode;

struct cetl_cdlist {

  cetl_size_t size;
  _cetl_dnode *head;
  _cetl_dnode *tail;
  const cetl_element *type;
};

static _cetl_dnode *_cetl_cdlist_create_dnode(const cetl_ptr_t data,
                                              const cetl_element *type) {

  _cetl_dnode *dnode = malloc(sizeof(_cetl_dnode));

  if (!dnode)
    return NULL;

  dnode->data = malloc(type->size);

  if (!dnode->data) {
    free(dnode);
    return NULL;
  }

  if (type->ctor) {
    type->ctor(dnode->data, data);
  } else {
    memcpy(dnode->data, data, type->size);
  }

  dnode->next = NULL;
  dnode->prev = NULL;

  return dnode;
}

static cetl_void_t _cetl_cdlist_free_node_data(cetl_cdlist *cdlist,
                                               cetl_ptr_t data) {
  if (cdlist->type->dtor) {
    cdlist->type->dtor(data);
  } else {
    free(data);
  }
}

cetl_cdlist *cetl_cdlist_create_empty(const struct cetl_element *type) {

  cetl_cdlist *cdlist = malloc(sizeof(cetl_cdlist));

  if (!cdlist)
    return NULL;

  cdlist->head = NULL;
  cdlist->tail = NULL;
  cdlist->type = type;
  cdlist->size = 0;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_create_copy(const cetl_cdlist *src_cdlist) {

  if (!src_cdlist)
    return NULL;

  cetl_cdlist *new_cdlist = cetl_cdlist_create_empty(src_cdlist->type);

  if (!new_cdlist || (src_cdlist->size == 0)) {
    return new_cdlist;
  }

  _cetl_dnode *current = src_cdlist->head;

  for (cetl_size_t i = 0; i < src_cdlist->size; ++i) {
    if (cetl_cdlist_push_back(new_cdlist, current->data) == NULL) {
      cetl_cdlist_free_nodes(new_cdlist);
      free(new_cdlist);
      return NULL;
    }
    current = current->next;
  }

  return new_cdlist;
}

cetl_cdlist *cetl_cdlist_push_back(cetl_cdlist *cdlist, const cetl_ptr_t data) {

  if (!cdlist || !data)
    return NULL;

  _cetl_dnode *dnode = _cetl_cdlist_create_dnode(data, cdlist->type);

  if (!dnode)
    return NULL;

  if (cdlist->size == 0) {

    cdlist->head = dnode;
    cdlist->tail = dnode;

    cdlist->tail->next = cdlist->head;
    cdlist->head->prev = cdlist->tail;

    cdlist->size++;
    return cdlist;
  }

  dnode->prev = cdlist->tail;
  dnode->next = cdlist->head;

  cdlist->tail->next = dnode;
  cdlist->tail = dnode;

  cdlist->size++;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_push_front(cetl_cdlist *cdlist,
                                    const cetl_ptr_t data) {

  if (!cdlist || !data)
    return NULL;

  _cetl_dnode *dnode = _cetl_cdlist_create_dnode(data, cdlist->type);

  if (cdlist->size == 0) {

    cdlist->head = dnode;
    cdlist->tail = dnode;

    cdlist->tail->next = cdlist->head;
    cdlist->head->prev = cdlist->tail;

    cdlist->size++;
    return cdlist;
  }

  dnode->next = cdlist->head;
  dnode->prev = cdlist->tail;

  cdlist->head->prev = dnode;
  cdlist->head = dnode;

  cdlist->size++;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_pop_back(cetl_cdlist *cdlist) {

  if (!cdlist || cdlist->size == 0)
    return cdlist;

  _cetl_dnode *old_tail = cdlist->tail;

  if (cdlist->size == 1) {

    cdlist->head = NULL;
    cdlist->tail = NULL;

  } else {

    _cetl_dnode *new_tail = old_tail->prev;
    new_tail->next = cdlist->head;
    cdlist->tail = new_tail;
  }

  _cetl_cdlist_free_node_data(cdlist, old_tail->data);
  free(old_tail);

  cdlist->size--;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_pop_front(cetl_cdlist *cdlist) {

  if (!cdlist || cdlist->size == 0)
    return cdlist;

  _cetl_dnode *old_head = cdlist->head;

  if (cdlist->size == 1) {

    cdlist->head = NULL;
    cdlist->tail = NULL;

  } else {

    _cetl_dnode *new_head = old_head->next;
    new_head->prev = cdlist->tail;
    cdlist->head = new_head;
  }

  _cetl_cdlist_free_node_data(cdlist, old_head->data);
  free(old_head);

  cdlist->size--;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_insert(cetl_cdlist *cdlist, const cetl_ptr_t data,
                                cetl_size_t pos) {

  if (!cdlist || !data || pos > cdlist->size)
    return NULL;

  if (pos == cdlist->size) {
    return cetl_cdlist_push_back(cdlist, data);
  } else if (pos == 0) {
    return cetl_cdlist_push_front(cdlist, data);
  }

  _cetl_dnode *dnode = _cetl_cdlist_create_dnode(data, cdlist->type);

  if (!dnode)
    return NULL;

  _cetl_dnode *prev = cdlist->head;

  for (cetl_size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cetl_dnode *next = prev->next;

  dnode->next = next;
  dnode->prev = prev;

  prev->next = dnode;
  next->prev = dnode;

  cdlist->size++;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_erase(cetl_cdlist *cdlist, cetl_size_t pos) {

  if (!cdlist || pos >= cdlist->size)
    return NULL;

  if (pos == 0) {
    return cetl_cdlist_pop_front(cdlist);
  } else if (pos == cdlist->size - 1) {
    return cetl_cdlist_pop_back(cdlist);
  }

  _cetl_dnode *prev = cdlist->head;

  for (cetl_size_t i = 0; i < pos - 1; ++i) {
    prev = prev->next;
  }

  _cetl_dnode *next = prev->next->next;

  _cetl_cdlist_free_node_data(cdlist, prev->next->data);
  free(prev->next);

  prev->next = next;
  next->prev = prev;

  cdlist->size--;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_merge_two(const cetl_cdlist *cdlist1,
                                   const cetl_cdlist *cdlist2) {

  if (!cdlist1 || !cdlist2)
    return NULL;

  if (cdlist1->size == 0) {
    return cetl_cdlist_create_copy(cdlist2);
  } else if (cdlist2->size == 0) {
    return cetl_cdlist_create_copy(cdlist1);
  }

  cetl_cdlist *copy_cdlist1 = cetl_cdlist_create_copy(cdlist1);
  cetl_cdlist *copy_cdlist2 = cetl_cdlist_create_copy(cdlist2);

  copy_cdlist1->tail->next = copy_cdlist2->head;
  copy_cdlist2->head->prev = copy_cdlist1->tail;

  copy_cdlist2->tail->next = copy_cdlist1->head;
  copy_cdlist1->head->prev = cdlist2->tail;

  copy_cdlist1->tail = copy_cdlist2->tail;
  copy_cdlist1->size += copy_cdlist2->size;

  cetl_cdlist_free(copy_cdlist2);

  return copy_cdlist1;
}

cetl_cdlist *cetl_cdlist_clear(cetl_cdlist *cdlist) {

  if (!cdlist)
    return NULL;

  cetl_cdlist_free_nodes(cdlist);

  cdlist->head = NULL;
  cdlist->tail = NULL;
  cdlist->size = 0;

  return cdlist;
}

cetl_cdlist *cetl_cdlist_set(cetl_cdlist *cdlist, cetl_size_t pos,
                             const cetl_ptr_t data) {

  if (!cdlist || !data || pos >= cdlist->size)
    return NULL;

  _cetl_dnode *target = cdlist->head;

  for (cetl_size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  _cetl_cdlist_free_node_data(cdlist, target->data);

  if (cdlist->type->ctor) {
    cdlist->type->ctor(target->data, data);
  } else {
    memcpy(target->data, data, cdlist->type->size);
  }

  return cdlist;
}

cetl_ptr_t cetl_cdlist_get(const cetl_cdlist *cdlist, cetl_size_t pos) {

  if (!cdlist || pos >= cdlist->size)
    return NULL;

  _cetl_dnode *target = cdlist->head;

  for (cetl_size_t i = 0; i < pos; ++i) {
    target = target->next;
  }

  return target->data;
}

cetl_size_t cetl_cdlist_size(const cetl_cdlist *cdlist) { return cdlist->size; }

cetl_bool_t cetl_cdlist_is_empty(const cetl_cdlist *cdlist) {
  return cdlist && !cdlist->size;
}

cetl_void_t cetl_cdlist_free_nodes(cetl_cdlist *cdlist) {

  _cetl_dnode *current = cdlist->head;

  for (cetl_size_t i = 0; i < cdlist->size; ++i) {
    _cetl_dnode *tmp = current;
    _cetl_cdlist_free_node_data(cdlist, current->data);
    current = current->next;
    free(tmp);
  }
}

cetl_void_t cetl_cdlist_free(cetl_cdlist *cdlist) {
  cetl_cdlist_free_nodes(cdlist);
  free(cdlist);
}
