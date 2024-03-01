#include "stdlib.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

#include "ii/ii-log.h"
#include "ii/ii-stdext.h"
#include "ii/ii-array.h"

ii_log_lvl_t ii_log_lvl = ii_log_lvl_dbg;
const char * ii_log_tag = "algo";

s32 __int_cmp(void * l, void * r) {
  s32 lv = *(s32 *)l;
  s32 rv = *(s32 *)r;
  return  lv < rv ? -1 :
          lv > rv ?  1 : 0;
}

char * __int_2str(void * t) {
  int tv = *(int *)t;
  return ii_strfmt("%d", tv);
}

void __array_log(ii_array_t * arr, ii_2str_t obj_2str, char * tag) {
  char * str = ii_array_2str(arr, obj_2str, ", ");
  logi("%s %s", tag, str ? str : "<empty>");
  free(str);
}

s32 main(void) {
  s32 vals[] = { 5, 2, 4, 6, 1, 3 };
  ii_array_t * arr = ii_array_new3(sizeof(int));
  ii_array_add(arr, vals, ii_arr_sz(vals));
  __array_log(arr, __int_2str, "prev:");
  ii_array_insertion_sort(arr, __int_cmp);
  __array_log(arr, __int_2str, "curr:");
  ii_array_free(arr, false);
  return 0;
}
