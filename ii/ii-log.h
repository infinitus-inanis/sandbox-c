#ifndef __II_LOGGER_H__
#define __II_LOGGER_H__

typedef enum {
  log_lvl_err = 0,
  log_lvl_wrn = 1,
  log_lvl_inf = 2,
  log_lvl_dbg = 3
} log_lvl_t;

extern log_lvl_t    log_lvl;
extern const char * log_tag;

#define __log_impl(_lvl, _exp) \
  do { if (log_lvl >= _lvl) { _exp; } } while(0)

#define __log_expr(_fun, _fmt, ...) \
  _fun("[%s][%s:%d] " _fmt "\n", log_tag, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define __log_impl_e(_exp) __log_impl(log_lvl_err, _exp)
#define __log_impl_w(_exp) __log_impl(log_lvl_wrn, _exp)
#define __log_impl_i(_exp) __log_impl(log_lvl_inf, _exp)
#define __log_impl_d(_exp) __log_impl(log_lvl_dbg, _exp)

#include "stdio.h"
#define msgerr(_fmt, ...) fprintf(stderr, _fmt, ##__VA_ARGS__)
#define msgout(_fmt, ...) fprintf(stdout, _fmt, ##__VA_ARGS__)

#define loge(_fmt, ...) __log_impl_e(__log_expr(msgerr, _fmt, ##__VA_ARGS__))
#define logw(_fmt, ...) __log_impl_w(__log_expr(msgerr, _fmt, ##__VA_ARGS__))
#define logi(_fmt, ...) __log_impl_i(__log_expr(msgout, _fmt, ##__VA_ARGS__))
#define logd(_fmt, ...) __log_impl_d(__log_expr(msgout, _fmt, ##__VA_ARGS__))

#endif//__II_LOGGER_H__