
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#if defined NA_NUMERICS_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_NUMERICS_INCLUDED
#define NA_NUMERICS_INCLUDED



#include "../NABase.h"

// Returns either 0 or -1 in two complement form but stored as an uint
// depending on whether the parameter is positive or negative.
NA_IAPI uint8     naGetSignum8  (int8     i);
NA_IAPI uint16    naGetSignum16 (int16    i);
NA_IAPI uint32    naGetSignum32 (int32    i);
NA_IAPI NAu64  naGetSignum64 (NAi64  i);
NA_IAPI NAu128 naGetSignum128(NAi128 i);
NA_IAPI NAu256 naGetSignum256(NAi256 i);

// Returns either 1 or -1. The value 0 returns 1. Never returns 0.
NA_IAPI int8     naSigni8  (int8     i);
NA_IAPI int16    naSigni16 (int16    i);
NA_IAPI int32    naSigni32 (int32    i);
NA_IAPI NAi64  naSigni64 (NAi64  i);
NA_IAPI NAi128 naSigni128(NAi128 i);
NA_IAPI NAi256 naSigni256(NAi256 i);

// Sets or unsets the sign bit. This is pure bit logic, not performing
// any complement.
NA_IAPI void naSetSignBit8    (void* i);
NA_IAPI void naSetSignBit16   (void* i);
NA_IAPI void naSetSignBit32   (void* i);
NA_IAPI void naSetSignBit64   (void* i);
NA_IAPI void naSetSignBit128  (void* i);
NA_IAPI void naSetSignBit256  (void* i);
NA_IAPI void naUnsetSignBit8  (void* i);
NA_IAPI void naUnsetSignBit16 (void* i);
NA_IAPI void naUnsetSignBit32 (void* i);
NA_IAPI void naUnsetSignBit64 (void* i);
NA_IAPI void naUnsetSignBit128(void* i);
NA_IAPI void naUnsetSignBit256(void* i);

// Makes the value positive.
NA_IAPI int8     naAbsi8  (int8     i);
NA_IAPI int16    naAbsi16 (int16    i);
NA_IAPI int32    naAbsi32 (int32    i);
NA_IAPI NAi64  naAbsi64 (NAi64  i);
NA_IAPI NAi128 naAbsi128(NAi128 i);
NA_IAPI NAi256 naAbsi256(NAi256 i);



// The masks for the sign bits of different type sizes.
#define NA_SIGN_MASK_8   ((uint8) (1u << (NA_TYPE8_BITS   - 1u)))
#define NA_SIGN_MASK_16  ((uint16)(1u << (NA_TYPE16_BITS  - 1u)))
#define NA_SIGN_MASK_32  ((uint32)(1u << (NA_TYPE32_BITS  - 1u)))
#define NA_SIGN_MASK_64  naMakeu64(NA_SIGN_MASK_32, NA_ZERO_u32)
#define NA_SIGN_MASK_128 naMakeu128(NA_SIGN_MASK_64, NA_ZERO_u64)
#define NA_SIGN_MASK_256 naMakeu256(NA_SIGN_MASK_128, NA_ZERO_u128)



// 8 bit basic integer constants
#define NA_ZERO_u8      ((uint8)0u)
#define NA_ONE_u8       ((uint8)1u)
#define NA_ZERO_i8      ((int8)0)
#define NA_ONE_i8       ((int8)1)
#if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
  #define NA_MINUS_ONE_i8 ((int8)NA_MAX_u8)
#elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
  #define NA_MINUS_ONE_i8 ((int8)(NA_MAX_u8 - 1))
#elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
  #define NA_MINUS_ONE_i8 ((int8)(NA_SIGN_MASK_8 + 1))
#else
  #error "Invalid sign encoding"
#endif



// 16 bit basic integer constants
#define NA_ZERO_u16    ((uint16)0u)
#define NA_ONE_u16     ((uint16)1u)
#define NA_ZERO_i16    ((int16)0)
#define NA_ONE_i16     ((int16)1)
#if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
  #define NA_MINUS_ONE_i16 ((int16)NA_MAX_u16)
#elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
  #define NA_MINUS_ONE_i16 ((int16)(NA_MAX_u16 - 1))
#elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
  #define NA_MINUS_ONE_i16 ((int16)(NA_SIGN_MASK_16 + 1))
#else
  #error "Invalid sign encoding"
#endif



// 32 bit basic integer constants
#define NA_ZERO_u32    ((uint32)0u)
#define NA_ONE_u32     ((uint32)1u)
#define NA_ZERO_i32    ((int32)0)
#define NA_ONE_i32     ((int32)1)
#if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
  #define NA_MINUS_ONE_i32 ((int32)NA_MAX_u32)
#elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
  #define NA_MINUS_ONE_i32 ((int32)(NA_MAX_u32 - 1))
#elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
  #define NA_MINUS_ONE_i32 ((int32)(NA_SIGN_MASK_32 + 1))
#else
  #error "Invalid sign encoding"
#endif


// 64 bit basic integer constants
#if defined NA_TYPE_INT64
  #define NA_ZERO_u64    ((uint64)0u)
  #define NA_ONE_u64     ((uint64)1u)
  #define NA_ZERO_i64    ((int64)0)
  #define NA_ONE_i64     ((int64)1)

  #if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
    #define NA_MINUS_ONE_i64 ((int64)NA_MAX_u64)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i64 ((int64)(NA_MAX_u64 - 1))
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i64 ((int64)(NA_SIGN_MASK_64 + 1))
  #else
    #error "Invalid sign encoding"
  #endif
#else
  #define NA_ZERO_u64    naMakeu64WithLo(NA_ZERO_u32)
  #define NA_ONE_u64     naMakeu64WithLo(NA_ONE_u32)
  #define NA_ZERO_i64    naMakei64WithLo(NA_ZERO_i32)
  #define NA_ONE_i64     naMakei64WithLo(NA_ONE_i32)

  #if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
    #define NA_MINUS_ONE_i64 naCastu64Toi64(NA_MAX_u64)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i64 naMakei64(NA_MINUS_ONE_i32)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i64 naMakei64(NA_SIGN_MASK_32, NA_ONE_u32)
  #else
    #error "Invalid sign encoding"
  #endif
#endif



// 128 bit basic integer constants
#if defined NA_TYPE_INT128
  #define NA_ZERO_u128   ((uint128)0u)
  #define NA_ONE_u128    ((uint128)1u)
  #define NA_ZERO_i128   ((int128)0)
  #define NA_ONE_i128    ((int128)1)

  #if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
    #define NA_MINUS_ONE_i128 ((int128)NA_MAX_u128)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i128 ((int128)(NA_MAX_u128 - 1))
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i128 ((int128)(NA_SIGN_MASK_128 + 1))
  #else
    #error "Invalid sign encoding"
  #endif
#else
  #define NA_ZERO_u128   naMakeu128WithLo(NA_ZERO_u64)
  #define NA_ONE_u128    naMakeu128WithLo(NA_ONE_u64)
  #define NA_ZERO_i128   naMakei128WithLo(NA_ZERO_i64)
  #define NA_ONE_i128    naMakei128WithLo(NA_ONE_i64)

  #if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
    #define NA_MINUS_ONE_i128 naCastu128Toi128(NA_MAX_u128)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i128 naMakei128(NA_MINUS_ONE_i64)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i128 naMakei128(NA_SIGN_MASK_64, NA_ONE_u64)
  #else
    #error "Invalid sign encoding"
  #endif
#endif



// 256 bit basic integer constants
#if defined NA_TYPE_INT256
  #define NA_ZERO_u256   ((uint256)0u)
  #define NA_ONE_u256    ((uint256)1u)
  #define NA_ZERO_i256   ((int256)0)
  #define NA_ONE_i256    ((int256)1)

  #if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
    #define NA_MINUS_ONE_i256 ((int256)NA_MAX_u256)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i256 ((int256)(NA_MAX_u256 - 1))
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i256 ((int256)(NA_SIGN_MASK_256 + 1))
  #else
    #error "Invalid sign encoding"
  #endif
#else
  #define NA_ZERO_u256   naMakeu256WithLo(NA_ZERO_u128)
  #define NA_ONE_u256    naMakeu256WithLo(NA_ONE_u128)
  #define NA_ZERO_i256   naMakei256WithLo(NA_ZERO_i128)
  #define NA_ONE_i256    naMakei256WithLo(NA_ONE_i128)

  #if NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
    #define NA_MINUS_ONE_i256 naCastu256Toi256(NA_MAX_u256)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i256 naMakei256(NA_MINUS_ONE_i128)
  #elif NA_SIGN_ENCODING == NA_SIGN_ENCODING_ONES_COMPLEMENT
    #define NA_MINUS_ONE_i256 naMakei256(NA_SIGN_MASK_128, NA_ONE_u128)
  #else
    #error "Invalid sign encoding"
  #endif
#endif


// Inline implementations are in a separate file:
#include "NANumericsII.h"



#endif // NA_NUMERICS_INCLUDED

// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
