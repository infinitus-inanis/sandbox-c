#ifndef __II_TYPES_H__
#define __II_TYPES_H__

typedef signed   char s8;
typedef unsigned char u8;

typedef signed   short s16;
typedef unsigned short u16;

typedef signed   int s32;
typedef unsigned int u32;

typedef signed   long long s64;
typedef unsigned long long u64;

union __byte {
  struct {
    u8 b0 : 1;
    u8 b1 : 1;
    u8 b2 : 1;
    u8 b3 : 1;
    u8 b4 : 1;
    u8 b5 : 1;
    u8 b6 : 1;
    u8 b7 : 1;
  };
  u8 v;
};
typedef u8 byte;


#endif//__II_TYPES_H__