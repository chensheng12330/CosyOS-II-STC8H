/**************************************************************************//**
 * @item     CosyOS-II User
 * @file     typedef.h
 * @brief    CosyOS-II typedef for User
 * @author   迟凯峰
 * @version  V1.0.2
 * @date     2024.05.11
 ******************************************************************************/

#ifndef __TYPEDEF_H
#define __TYPEDEF_H

typedef char  s8;
typedef short s16;
typedef long  s32;
typedef unsigned char  uchar,  u8;
typedef unsigned short ushort, u16;
typedef unsigned long  ulong,  u32;
typedef unsigned int   uint;

#if MCUCFG_ISA != __ARM__
typedef char  int8_t;
typedef short int16_t;
typedef long  int32_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;
#else
typedef unsigned long long ullong, u64;
#endif

#endif
