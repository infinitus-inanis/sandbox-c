#include "ii-array.h"
#include "ii-utils.h"
#include "ii-log.h"

#include "stdlib.h"
#include "memory.h"

#define __array_off(_arr, _idx) ((_arr)->obj_size * (size_t)(_idx))
#define __array_get(_arr, _idx) ((_arr)->buf + __array_off(_arr, _idx))

struct __array_ext {
  void *   buf;      /* underlying memory buffer */
  size_t   len;      /* effective array length   */
  size_t   obj_size; /* size of object in memory */
  mutate_f obj_free; /* function to free object  */
  size_t   cap;      /* current array capacity   */
};
typedef struct __array_ext __array_ext_t;

static
void
__array_maybe_expand(__array_ext_t * arr, size_t exp) {
  size_t req = arr->len + exp;
  if (req <= arr->cap) {
    return;
  }
  arr->cap = req;
  arr->buf = realloc(
    arr->buf,
    __array_off(arr, arr->cap)
  );
}

array_t *
array_new0(size_t obj_size, mutate_f obj_free, size_t cap) {
  __array_ext_t * ret = malloc(sizeof(__array_ext_t));
  ret->buf      = NULL;
  ret->len      = 0;
  ret->obj_size = obj_size;
  ret->obj_free = obj_free;
  ret->cap      = 0;
  if (cap) {
    __array_maybe_expand(ret, cap);
  }
  return (array_t *)ret;
}

array_t *
array_new1(size_t obj_size, mutate_f obj_free) {
  return array_new0(obj_size, obj_free, 0);
}

array_t *
array_new2(size_t obj_size, size_t cap) {
  return array_new0(obj_size, NULL, cap);
}

array_t *
array_new3(size_t obj_size) {
  return array_new0(obj_size, NULL, 0);
}

void *
array_free(array_t * farr, bool extract_buf) {
  __array_ext_t * arr = (__array_ext_t *)farr;
  void * ret = arr->buf;
  if (!extract_buf) {
    if (arr->obj_free) {
      for (size_t i = 0; i < arr->len; ++i) {
        arr->obj_free(__array_get(arr, i));
      }
    }
    free(arr->buf);
    ret = NULL;
  }
  free(arr);
  return ret;
}

void
array_add(array_t * farr, void * objs, size_t objslen) {
  __array_ext_t * arr = (__array_ext_t *)farr;
  if (!objslen) {
    return;
  }
  __array_maybe_expand(arr, objslen);
  memcpy(
    __array_get(arr, arr->len),
    objs,
    __array_off(arr, objslen)
  );
  arr->len += objslen;
}

void * 
array_get(array_t * arr, size_t idx) {
  if (idx < arr->len) {
    return NULL;
  }
  return __array_get(arr, idx);
}

void 
array_insertion_sort(array_t * farr, compare_f cmp) {
  __array_ext_t * arr = (__array_ext_t *)farr;
  if (arr->len < 2) {
    return;
  }

  size_t sz = arr->obj_size;
  byte_t _t[sz];
  byte_t * _0 = __array_get(arr, 0);
  byte_t * _N = __array_get(arr, arr->len);
  byte_t * _i = __array_get(arr, 1);
  while (_i < _N) {
    memcpy(_t, _i, sz);
    byte_t * _j = _i - sz;
    while (_j >= _0) {
      if (cmp(_j, _t) < 0) {
        break;
      }
      memcpy(_j + sz, _j, sz);
      _j -= sz;
    }
    memcpy(_j + sz, _t, sz);
    _i += sz;
  }
}

static
void
__array_merge_impl(
  __array_ext_t * arr,
  compare_f cmp,
  byte_t * ls,
  byte_t * le,
  byte_t * rs,
  byte_t * re
) {
  size_t sz = arr->obj_size;
  byte_t _t[sz];
  while (ls <= le && rs <= re) {
    if (cmp(ls, rs) <= 0) {
      ls += sz;
      continue;
    }
    memcpy(_t, rs, sz);
    byte_t * rT = rs;
    while (rT > ls) {
      memcpy(rT, rT - sz, sz);
      rT -= sz;
    }
    memcpy(ls, _t, sz);
    ls += sz;
    le += sz;
    rs += sz;
  }
}

static
void
__array_merge_sort(
  __array_ext_t * arr, 
  compare_f cmp, 
  byte_t * ls, 
  byte_t * re
) {
  if (ls == re) {
    return;
  }
  
  size_t m = (size_t)(re - ls) / arr->obj_size / 2;
  byte_t * le = ls + __array_off(arr, m);
  byte_t * rs = le + __array_off(arr, 1);
  __array_merge_sort(arr, cmp, ls, le);
  __array_merge_sort(arr, cmp, rs, re);

  if (cmp(le, rs) <= 0) {
    return;
  }
  __array_merge_impl(arr, cmp, ls, le, rs, re);
}

void
array_merge_sort(array_t * farr, compare_f cmp) {
  __array_ext_t * arr = (__array_ext_t *)farr;
  if (arr->len < 2) {
    return;
  }
  __array_merge_sort(arr, cmp,
    __array_get(arr, 0),
    __array_get(arr, arr->len - 1)
  );
}

char *
array_to_str(array_t * farr, to_str_f obj_to_str, char * sep) {
  __array_ext_t * arr = (__array_ext_t *)farr;
  if (!arr->len) {
    return NULL;
  }
  if (!sep) {
    sep = " ";
  }
  char *  ret = NULL;
  size_t  retlen = 0;
  char ** tmp = malloc(arr->len * sizeof(char *));
  for (size_t i = 0; i < arr->len; ++i) {
    if (i != 0) {
      retlen += strlen(sep);
    }
    char * str = tmp[i] = obj_to_str(__array_get(arr, i));
    retlen += strlen(str);
  }
  char * dst = ret = malloc(retlen + (/*null*/ 1));
  for (size_t i = 0; i < arr->len; ++i) {
    if (i != 0) {
      dst = strcat_fast(dst, sep);
    }
    char * str = tmp[i];
    dst = strcat_fast(dst, str);
    free(str);
  }
  free(tmp);
  return ret;
}