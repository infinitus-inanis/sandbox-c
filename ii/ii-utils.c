#include "ii-utils.h"
#include "ii-types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void *
memdup(void * mem, size_t mem_sz) {
  return mem ? memcpy(malloc(mem_sz), mem, mem_sz) : NULL;
}

void
memswp(void * m0, void * m1, size_t mem_sz) {
  byte_t * bm0 = (byte_t *)m0;
  byte_t * bm1 = (byte_t *)m1;
  byte_t * bme = bm0 + mem_sz;
  for (; bm0 < bme; bm0++, bm1++) {
    byte_t tmp = *bm0;
    *bm0 = *bm1;
    *bm1 =  tmp;
  }
}

size_t
str_sz(char * str) {
  return str ? strlen(str) + 1 : 0;
}

static 
char *
__strcat_fast_unsafe_valist(char * dst, va_list args) {
  va_list argscp;
  va_copy(argscp, args);
  char * src = NULL;
  while ((src = va_arg(argscp, char *))) {
    size_t srclen = strlen(src);
    memcpy(dst, src, srclen + 1);
    dst += srclen;
  }
  va_end(argscp);
  return dst;
}

char *
strcat_fast_unsafe(char * dst, ...) {
  va_list args;
  va_start(args, dst);
  char * ret = __strcat_fast_unsafe_valist(dst, args);
  va_end(args);
  return ret;
}

char *
strfmt_valist(char * fmt, va_list args) {
  va_list argscp;
  va_copy(argscp, args);
  size_t ret_sz = (size_t)(
    vsnprintf(NULL, 0, fmt, argscp) + 
    (/*nullchar*/ 1)
  );
  va_end(argscp);
  char * ret = malloc(ret_sz);
  vsnprintf(ret, ret_sz, fmt, args);
  return ret;
}

char *
strfmt(char * fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char * ret = strfmt_valist(fmt, args);
  va_end(args);
  return ret;
}