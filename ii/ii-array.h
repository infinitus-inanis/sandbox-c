#ifndef __II_ARRAY_H__
#define __II_ARRAY_H__

#include "ii-types.h"
#include "ii-funcs.h"

struct __array {
  void * buf;      /* underlying buffer            */
  size_t len;      /* effective length             */
  size_t obj_size; /* size of one object in memory */
};
typedef struct __array array_t;

array_t * array_new0(size_t obj_size, free_f obj_free, size_t cap);
array_t * array_new1(size_t obj_size, free_f obj_free);
array_t * array_new2(size_t obj_size, size_t cap);
array_t * array_new3(size_t obj_size);
void *    array_free(array_t * arr, bool extract_buf);

void array_add(array_t * arr, void * objs, size_t objslen);

void array_insertion_sort(array_t * arr, compare_f cmp);
void array_merge_sort(array_t * arr, compare_f cmp);

char * array_to_str(array_t * arr, to_str_f to_str, char * separator);

#endif//__II_ARRAY_H__