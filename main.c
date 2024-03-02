#include "ii/ii-types.h"
#include "ii/ii-utils.h"
#include "ii/ii-array.h"
#include "ii/ii-log.h"

#include "stdlib.h"
#include "string.h"

log_lvl_t    log_lvl = log_lvl_dbg;
const char * log_tag = "algo";

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
  array_t * arr = array_new3(sizeof(int));
  {// initialize
    int vals[] = { 5, 2, 4, 7, 1, 3, 2, 6 };
    array_add(arr, vals, ARR_SZ(vals));
    array_log(arr, int_to_str, "origin:");
  }
  {// insertion sort
    array_insertion_sort(arr, int_lt);
    array_log(arr, int_to_str, "i-sort:");
  }
  {// merge sort
    array_merge_sort(arr, int_gt);
    array_log(arr, int_to_str, "m-sort:");
  }
  array_free(arr, false);
  return 0;
}
