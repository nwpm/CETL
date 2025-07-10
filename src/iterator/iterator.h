#ifndef CSTL_ITERATOR_H
#define CSTL_ITERATOR_H

struct cstl_iterator{

  void *ctx;
  void *(*next)(struct cstl_iterator *it);
  void (*has_next)(struct cstl_iterator *it);
  void (*destroy)(struct cstl_iterator *it);

};

#endif // CSTL_ITERATOR_H
