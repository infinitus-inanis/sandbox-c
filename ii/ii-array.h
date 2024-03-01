#ifndef __II_ARRAY_H__
#define __II_ARRAY_H__

#include "ii-funcs.h"
#include "stddef.h"
#include "stdbool.h"

struct ii_array {
  void * buf;      /* underlying buffer   */
  size_t len;      /* effective length    */
  size_t obj_sz;   /* size of one element */
};
typedef struct ii_array ii_array_t;

#define ii_array_get_len(_arr, _len) ((size_t)(_arr)->obj_sz * (_len))
#define ii_array_get_obj(_arr, _idx) ((void *)(_arr)->buf + ii_array_get_len(_arr, _idx))

ii_array_t * ii_array_new0(size_t obj_sz, ii_free_t obj_free, size_t cap);
ii_array_t * ii_array_new1(size_t obj_sz, ii_free_t obj_free);
ii_array_t * ii_array_new2(size_t obj_sz, size_t cap);
ii_array_t * ii_array_new3(size_t obj_sz);
void *       ii_array_free(ii_array_t * arr, bool extract_buf);

void ii_array_add(ii_array_t * arr, void * objs, size_t objslen);

char * ii_array_2str(ii_array_t * arr, ii_2str_t obj_2str, char * sep);

void ii_array_insertion_sort(ii_array_t * arr, ii_compare_t cmp);

#endif//__II_ARRAY_H__