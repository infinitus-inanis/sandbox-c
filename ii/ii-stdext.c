#include "ii-stdext.h"
#include "ii-types.h"

#include "stdlib.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "assert.h"

static 
char *
ii_strcat_unsafe_valist(char * dst, va_list args) {
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

void *
ii_memdup(void * mem, size_t mem_sz) {
  return mem ? memcpy(malloc(mem_sz), mem, mem_sz) : NULL;
}

void
ii_memswap(void * m0, void * m1, size_t mem_sz) {
  u8 * bm0 = (u8 *)m0;
  u8 * bm1 = (u8 *)m1;
  u8 * bme = bm0 + mem_sz;
  for (; bm0 < bme; bm0++, bm1++) {
    u8 tmp = *bm0;
    *bm0 = *bm1;
    *bm1 =  tmp;
  }
}

size_t
ii_str_sz(char * str) {
  return str ? strlen(str) + 1 : 0;
}

char *
ii_strdup(char * str) {
  return str ? ii_memdup(str, ii_str_sz(str)) : NULL;
}

char *
ii_strcat_unsafe(char * dst, ...) {
  va_list args;
  va_start(args, dst);
  char * ret = ii_strcat_unsafe_valist(dst, args);
  va_end(args);
  return ret;
}

char *
ii_strfmt_valist(char * fmt, va_list args) {
  va_list argscp;
  va_copy(argscp, args);
  size_t ret_sz = (size_t)(vsnprintf(NULL, 0, fmt, argscp) + (/*nullchar*/ 1));
  va_end(argscp);
  char * ret = malloc(ret_sz);
  vsnprintf(ret, ret_sz, fmt, args);
  return ret;
}

char *
ii_strfmt(char * fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char * ret = ii_strfmt_valist(fmt, args);
  va_end(args);
  return ret;
}