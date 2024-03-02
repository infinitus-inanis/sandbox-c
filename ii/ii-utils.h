#ifndef __II_UTILS_H__
#define __II_UTILS_H__

#include "ii-types.h"
#include <stdarg.h>

void *
memdup(void * mem, size_t mem_sz);

void
memswap(void * mem0, void * mem1, size_t mem_sz);

size_t
str_sz(char * str);

char *
strcat_fast_unsafe(char * dst, ...);

char *
strfmt_valist(char * fmt, va_list args);

char *
strfmt(char * fmt, ...);

#define strcat_fast(_dst, ...) strcat_fast_unsafe(_dst, __VA_ARGS__, NULL)
#define arr_sz(_x) (sizeof(_x) / sizeof(0[_x]))
#define arrlen(_x) (arr_sz(_x) - 1)

#endif//__II_UTILS_H__
