#include "ii/ii-types.h"
#include "ii/ii-utils.h"
#include "ii/ii-array.h"
#include "ii/ii-log.h"

#include "stdlib.h"
#include "string.h"

ii_log_lvl_t ii_log_lvl = ii_log_lvl_dbg;
const char * ii_log_tag = "algo";

int int_lt(void * l, void * r) {
  int lv = *(int *)l;
  int rv = *(int *)r;
  return lv < rv ? -1 :
         lv > rv ?  1 : 0;
}

int int_gt(void * l, void * r) {
  int lv = *(int *)l;
  int rv = *(int *)r;
  return lv < rv ?  1 :
         lv > rv ? -1 : 0;
}

char * int_to_str(void * t) {
  int tv = *(int *)t;
  return strfmt("%d", tv);
}

void array_log(array_t * arr, to_str_f obj_to_str, char * tag) {
  char * str = array_to_str(arr, obj_to_str, ", ");
  msgout("%s %s \n", tag, str ? str : "<empty>");
  free(str);
}

int main(void) {
  int vals[] = { 5, 2, 4, 7, 1, 3, 2, 6 };
  array_t * arr = array_new3(sizeof(int));
  array_add(arr, vals, arr_sz(vals));
  array_log(arr, int_to_str, "origin:");
  array_insertion_sort(arr, int_lt);
  array_log(arr, int_to_str, "i-sort:");
  array_merge_sort(arr, int_lt);
  array_log(arr, int_to_str, "m-sort:");
  array_free(arr, false);
  return 0;
}
