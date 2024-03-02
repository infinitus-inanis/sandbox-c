#include "ii-array.h"
#include "ii-utils.h"
#include "ii-log.h"

#include "stdlib.h"
#include "memory.h"

#define __array_off(_arr, _idx) ((_arr)->obj_size * (size_t)(_idx))
#define __array_get(_arr, _idx) ((_arr)->buf + __array_off(_arr, _idx))

struct __array_ext {
  void * buf;      /* underlying memory buffer */
  size_t len;      /* effective array length   */
  size_t obj_size; /* size of object in memory */
  free_f obj_free; /* function to free object  */
  size_t cap;      /* current array capacity   */
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
    arr->obj_size * arr->cap
  );
}

array_t *
array_new0(size_t obj_size, free_f obj_free, size_t cap) {
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
array_new1(size_t obj_size, free_f obj_free) {
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
    objslen * arr->obj_size
  );
  arr->len += objslen;
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
  char * ret = NULL;
  size_t retlen = 0;
  char * tmp[arr->len];
  for (size_t i = 0; i < arr->len; ++i) {
    if (i != 0) {
      retlen += strlen(sep);
    }
    char * str = tmp[i] = obj_to_str(__array_get(arr, i));
    retlen += strlen(str);
  }
  char * dst = ret = malloc(retlen + 1);
  for (size_t i = 0; i < arr->len; ++i) {
    if (i != 0) {
      dst = strcat_fast(dst, sep);
    }
    char * str = tmp[i];
    dst = strcat_fast(dst, str);
    free(str);
  }
  return ret;
}

void 
array_insertion_sort(array_t * farr, compare_f cmp) {
  __array_ext_t * arr = (__array_ext_t *)farr;
  if (arr->len < 2) {
    return;
  }

  size_t sz = arr->obj_size;
  byte_t vT[sz];
  byte_t * v0 = __array_get(arr, 0);
  byte_t * vN = __array_get(arr, arr->len);
  byte_t * vi = __array_get(arr, 1);
  while (vi < vN) {
    memcpy(vT, vi, sz);
    byte_t * vj = vi - sz;
    while (vj >= v0) {
      if (cmp(vj, vT) < 0) {
        break;
      }
      memcpy(vj + sz, vj, sz);
      vj -= sz;
    }
    memcpy(vj + sz, vT, sz);
    vi += sz;
  }
}

static
void
___array_merge_impl(
  __array_ext_t * arr,
  compare_f cmp,
  byte_t * ls,
  byte_t * le,
  byte_t * rs,
  byte_t * re
) {
  size_t sz = arr->obj_size;
  byte_t vT[sz];
  while (ls <= le && rs <= re) {
    if (cmp(ls, rs) <= 0) {
      ls += sz;
      continue;
    }
    memcpy(vT, rs, sz);
    byte_t * rT = rs;
    while (rT > ls) {
      memcpy(rT, rT - sz, sz);
      rT -= sz;
    }
    memcpy(ls, vT, sz);
    ls += sz;
    le += sz;
    rs += sz;
  }
}

static
void
___array_merge_sort(
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
  ___array_merge_sort(arr, cmp, ls, le);
  ___array_merge_sort(arr, cmp, rs, re);

  if (cmp(le, rs) <= 0) {
    return;
  }
  ___array_merge_impl(arr, cmp, ls, le, rs, re);
}

void
array_merge_sort(array_t * farr, compare_f cmp) {
  __array_ext_t * arr = (__array_ext_t *)farr;
  if (arr->len < 2) {
    return;
  }
  ___array_merge_sort(arr, cmp,
    __array_get(arr, 0),
    __array_get(arr, arr->len - 1)
  );
}
