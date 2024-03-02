#ifndef __II_LOGGER_H__
#define __II_LOGGER_H__

typedef enum {
  ii_log_lvl_err = 0,
  ii_log_lvl_wrn = 1,
  ii_log_lvl_inf = 2,
  ii_log_lvl_dbg = 3
} ii_log_lvl_t;

extern ii_log_lvl_t ii_log_lvl;
extern const char * ii_log_tag;

#define __ii_log_impl(_lvl, _exp) \
  do { if (ii_log_lvl >= _lvl) { _exp; } } while(0)

#define __ii_log_expr(_fun, _fmt, ...) \
  _fun("[%s][%s:%d] " _fmt "\n", ii_log_tag, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define __ii_log_impl_e(_exp) __ii_log_impl(ii_log_lvl_err, _exp)
#define __ii_log_impl_w(_exp) __ii_log_impl(ii_log_lvl_wrn, _exp)
#define __ii_log_impl_i(_exp) __ii_log_impl(ii_log_lvl_inf, _exp)
#define __ii_log_impl_d(_exp) __ii_log_impl(ii_log_lvl_dbg, _exp)

#include "stdio.h"
#define msgerr(_fmt, ...) fprintf(stderr, _fmt, ##__VA_ARGS__)
#define msgout(_fmt, ...) fprintf(stdout, _fmt, ##__VA_ARGS__)

#define loge(_fmt, ...) __ii_log_impl_e(__ii_log_expr(msgerr, _fmt, ##__VA_ARGS__))
#define logw(_fmt, ...) __ii_log_impl_w(__ii_log_expr(msgerr, _fmt, ##__VA_ARGS__))
#define logi(_fmt, ...) __ii_log_impl_i(__ii_log_expr(msgout, _fmt, ##__VA_ARGS__))
#define logd(_fmt, ...) __ii_log_impl_d(__ii_log_expr(msgout, _fmt, ##__VA_ARGS__))

#endif//__II_LOGGER_H__