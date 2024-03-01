#include "ii-array.h"
#include "ii-types.h"
#include "ii-stdext.h"
#include "ii-log.h"

#include "stdlib.h"
#include "stdint.h"
#include "memory.h"
#include "alloca.h"

struct array_impl {
  void *    buf;      /* underlying buffer         */
  size_t    len;      /* effective length          */
  size_t    obj_sz;   /* size of one element       */
  ii_free_t obj_free; /* free function for element */
  size_t    cap;      /* allocated length          */
};
typedef struct array_impl array_impl_t;

static 
void 
array_maybe_expand(array_impl_t * arr, size_t expansion) {
  size_t required = arr->len + expansion;
  if (required <= arr->cap) {
    return;
  }
  arr->cap = required;
  arr->buf = realloc(
    arr->buf,
    arr->cap * arr->obj_sz
  );
}

ii_array_t *
ii_array_new0(size_t obj_sz, ii_free_t obj_free, size_t cap) {
  array_impl_t * ret = malloc(sizeof(array_impl_t));
  ret->buf = NULL;
  ret->len = 0;
  ret->cap = cap;
  ret->obj_sz = obj_sz;
  ret->obj_free = obj_free;
  if (ret-cap) {
    array_maybe_expand(ret, cap);
  }
  return (ii_array_t *)ret;
}

ii_array_t *
ii_array_new1(size_t obj_sz, ii_free_t obj_free) {
  return ii_array_new0(obj_sz, obj_free, 0);
}

ii_array_t *
ii_array_new2(size_t obj_sz, size_t cap) {
  return ii_array_new0(obj_sz, NULL, cap);
}

ii_array_t *
ii_array_new3(size_t obj_sz) {
  return ii_array_new0(obj_sz, NULL, 0);
}

void *
ii_array_free(ii_array_t * farr, bool extract_buf) {
  array_impl_t * arr = (array_impl_t *)farr;
  void * ret = arr->buf;
  if (!extract_buf) {
    if (arr->obj_free) {
      for (size_t i = 0; i < arr->len; ++i) {
        arr->obj_free(ii_array_get_obj(arr, i));
      }
    }
    free(arr->buf);
    ret = NULL;
  }
  free(arr);
  return ret;
}

void
ii_array_add(ii_array_t * farr, void * objs, size_t objslen) {
  array_impl_t * arr = (array_impl_t *)farr;
  if (!objslen) {
    return;
  }
  array_maybe_expand(arr, objslen);
  memcpy(
    arr->buf + arr->len * arr->obj_sz,
    objs,
    objslen * arr->obj_sz
  );
  farr->len += objslen;
}

char *
ii_array_2str(ii_array_t * farr, ii_2str_t obj_2str, char * sep) {
  array_impl_t * arr = (array_impl_t *)farr;
  if (!arr->len) {
    return NULL;
  }
  if (!sep) {
    sep = ", ";
  }
  char * ret = NULL;
  size_t retlen = 0;
  char** tmp = malloc(arr->len * sizeof(char *));
  for (size_t i = 0; i < arr->len; ++i) {
    if (i != 0) {
      retlen += strlen(sep);
    }
    retlen += strlen(tmp[i] = obj_2str(
      ii_array_get_obj(arr, i)
    ));
  }
  char * dst = ret = malloc(retlen + 1);
  for (size_t i = 0; i < arr->len; ++i) {
    if (i != 0) {
      dst = ii_strcat(dst, sep);
    }
    char * str = tmp[i];
    dst = ii_strcat(dst, str);
    free(str);
  }
  free(tmp);
  return ret;
}

void 
ii_array_insertion_sort(ii_array_t * farr, ii_compare_t cmp) {
  array_impl_t * arr = (array_impl_t *)farr;
  if (arr->len < 2) {
    return;
  }

  size_t sz = arr->obj_sz;
  byte * vT = alloca(sz);
  byte * v0 = arr->buf;
  byte * vN = arr->buf + arr->len * sz;
  byte * vi = v0 + sz;
  for (; vi < vN; vi += sz) {
    memcpy(vT, vi, sz);
    byte * vj = vi - sz; 
    for (; vj >= v0; vj -= sz) {
      if (cmp(vj, vT) < 0) {
        break;
      }
      memcpy(vj + sz, vj, sz);
    }
    memcpy(vj + sz, vT, sz);
  }
}
