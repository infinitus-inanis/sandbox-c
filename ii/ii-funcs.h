#ifndef __II_FUNCS_H__
#define __II_FUNCS_H__

#include "ii-types.h"

typedef void   (* ii_mutate_t) (void * o);
typedef void   (* ii_free_t)   (void * o);
typedef char * (* ii_2str_t)   (void * o);

typedef int (* ii_compare_t) (void * l, void * r);

#endif//__II_FUNCS_H__