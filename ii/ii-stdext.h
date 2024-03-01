#ifndef __II_STDEXT_H__
#define __II_STDEXT_H__

#include "stddef.h"
#include "stdarg.h"

void *
ii_memdup(void * mem, size_t mem_sz);

void
ii_memswap(void * m0, void * m1, size_t mem_sz);

int
ii_memcmp(void * m0, void * m1, size_t mem_sz);

size_t
ii_str_sz(char * str);

char *
ii_strdup(char * str);

char *
ii_strcat_unsafe(char * dst, ...);

char *
ii_strfmt_valist(char * fmt, va_list args);

char *
ii_strfmt(char * fmt, ...);

#define ii_strcat(_dst, ...) ii_strcat_unsafe(_dst, __VA_ARGS__, NULL)
#define ii_arr_sz(_x) (sizeof(_x) / sizeof(0[_x]))
#define ii_arrlen(_x) (ii_arr_sz(_x) - 1)

#endif//__II_STDEXT_H__
