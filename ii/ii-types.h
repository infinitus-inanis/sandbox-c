#ifndef __II_TYPES_H__
#define __II_TYPES_H__

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

union __byte {
struct {
  uint8_t _0 : 1;
  uint8_t _1 : 1;
  uint8_t _2 : 1;
  uint8_t _3 : 1;
  uint8_t _4 : 1;
  uint8_t _5 : 1;
  uint8_t _6 : 1;
  uint8_t _7 : 1;
};
  uint8_t _v;
};
typedef union __byte byte_t;

#endif//__II_TYPES_H__