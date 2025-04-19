
#if defined NA_NUMERICS_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_NUMERICS_INCLUDED
#define NA_NUMERICS_INCLUDED



// Returns either 0 or -1 in two complement form but stored as an uint
// depending on whether the parameter is positive or negative.
NA_IAPI uint8  naGetSignum8  (int8   i);
NA_IAPI uint16 naGetSignum16 (int16  i);
NA_IAPI uint32 naGetSignum32 (int32  i);
NA_IAPI NAu64  naGetSignum64 (NAi64  i);
NA_IAPI NAu128 naGetSignum128(NAi128 i);
NA_IAPI NAu256 naGetSignum256(NAi256 i);

// Returns either 1 or -1. The value 0 returns 1. Never returns 0.
// 
// You can use the result of this function safely as a multiplication factor.
// You can also subtract the result of this function from the comma ASCII
// character and get a string representation of the sign:
// NAUTF8Char signASCII = ',' - naSigni64(-1234);
NA_IAPI int8   naSigni8  (int8   i);
NA_IAPI int16  naSigni16 (int16  i);
NA_IAPI int32  naSigni32 (int32  i);
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
NA_IAPI int8   naAbsi8  (int8   i);
NA_IAPI int16  naAbsi16 (int16  i);
NA_IAPI int32  naAbsi32 (int32  i);
NA_IAPI NAi64  naAbsi64 (NAi64  i);
NA_IAPI NAi128 naAbsi128(NAi128 i);
NA_IAPI NAi256 naAbsi256(NAi256 i);



// 8 bit basic integer constants
#define NA_SIGN_MASK_8         ((int8)(1 << (NA_TYPE8_BITS - 1)))
#define NA_ZERO_u8             ((uint8)0u)
#define NA_ZERO_i8             ((int8)0)
#define NA_ONE_u8              ((uint8)1u)
#define NA_ONE_i8              ((int8)1)

#define NA_MAX_u8              UINT8_MAX
#define NA_MAX_i8              INT8_MAX
#define NA_MIN_i8              INT8_MIN
#define NA_MINUS_ONE_i8        ((int8)NA_MAX_u8)



// 16 bit basic integer constants
#define NA_SIGN_MASK_16        (int16)((uint8)NA_SIGN_MASK_8 << NA_TYPE8_BITS | NA_ZERO_u8)
#define NA_ZERO_u16            ((uint16)0u)
#define NA_ZERO_i16            ((int16)0)
#define NA_ONE_u16             ((uint16)1u)
#define NA_ONE_i16             ((int16)1)

#define NA_MAX_u16             UINT16_MAX
#define NA_MAX_i16             INT16_MAX
#define NA_MIN_i16             INT16_MIN
#define NA_MINUS_ONE_i16       ((int16)NA_MAX_u16)



// 32 bit basic integer constants
#define NA_SIGN_MASK_32        (int32)((uint16)NA_SIGN_MASK_16 << NA_TYPE16_BITS | NA_ZERO_u16)
#define NA_ZERO_u32            ((uint32)0u)
#define NA_ZERO_i32            ((int32)0)
#define NA_ONE_u32             ((uint32)1u)
#define NA_ONE_i32             ((int32)1)

#define NA_MAX_u32             UINT32_MAX
#define NA_MAX_i32             INT32_MAX
#define NA_MIN_i32             INT32_MIN
#define NA_MINUS_ONE_i32       ((int32)NA_MAX_u32)



// 64 bit basic integer constants
#define NA_SIGN_MASK_64      naMakei64(NA_SIGN_MASK_32, NA_ZERO_u32)
#if NA_NATIVE_INT64_IN_USE
  #define NA_ZERO_u64          ((uint64)0u)
  #define NA_ZERO_i64          ((int64)0)
  #define NA_ONE_u64           ((uint64)1u)
  #define NA_ONE_i64           ((int64)1)

  #if defined UINT64_MAX
    #define NA_MAX_u64         UINT64_MAX
  #else
    #define NA_MAX_u64         (~NA_ZERO_u64)
  #endif
  #if defined INT64_MAX
    #define NA_MAX_i64         INT64_MAX
  #else
    #define NA_MAX_i64         (~NA_SIGN_MASK_64)
  #endif
  #if defined INT64_MIN
    #define NA_MIN_i64         INT64_MIN
  #else
    #define NA_MIN_i64         NA_SIGN_MASK_64
  #endif
  #define NA_MINUS_ONE_i64     ((int64)NA_MAX_u64)
#else
  #define NA_ZERO_u64          naMakeu64WithLo(NA_ZERO_u32)
  #define NA_ZERO_i64          naCastu32Toi64(NA_ZERO_u32)
  #define NA_ONE_u64           naMakeu64WithLo(NA_ONE_u32)
  #define NA_ONE_i64           naCastu32Toi64(NA_ONE_u32)

  #define NA_MAX_u64           naMakeu64(NA_MAX_u32, NA_MAX_u32)
  #define NA_MAX_i64           naMakei64(NA_MAX_i32, NA_MAX_u32)
  #define NA_MIN_i64           naMakei64(NA_MIN_i32, NA_ZERO_u32)
  #define NA_MINUS_ONE_i64     naCastu64Toi64(NA_MAX_u64)
#endif



// 128 bit basic integer constants
#define NA_SIGN_MASK_128     naMakei128(NA_SIGN_MASK_64, NA_ZERO_u64)
#if NA_NATIVE_INT128_IN_USE
  #define NA_ZERO_u128         ((uint128)0u)
  #define NA_ZERO_i128         ((int128)0)
  #define NA_ONE_u128          ((uint128)1u)
  #define NA_ONE_i128          ((int128)1)

  #if defined UINT128_MAX
    #define NA_MAX_u128        UINT128_MAX
  #else
    #define NA_MAX_u128        (~NA_ZERO_u128)
  #endif
  #if defined INT128_MAX
    #define NA_MAX_i128        INT128_MAX
  #else
    #define NA_MAX_i128        (~NA_SIGN_MASK_128)
  #endif
  #if defined INT128_MIN
    #define NA_MIN_i128        INT128_MIN
  #else
    #define NA_MIN_i128        NA_SIGN_MASK_128
  #endif
  #define NA_MINUS_ONE_i128    ((int128)NA_MAX_u128)
#else
  #define NA_ZERO_u128         naMakeu128WithLo(NA_ZERO_u64)
  #define NA_ZERO_i128         naMakei128WithLo(NA_ZERO_i64)
  #define NA_ONE_u128          naMakeu128WithLo(NA_ONE_u64)
  #define NA_ONE_i128          naMakei128WithLo(NA_ONE_i64)

  #define NA_MAX_u128          naMakeu128(NA_MAX_u64, NA_MAX_u64)
  #define NA_MAX_i128          naMakei128(NA_MAX_i64, NA_MAX_u64)
  #define NA_MIN_i128          naMakei128(NA_MIN_i64, NA_ZERO_u64)
  #define NA_MINUS_ONE_i128    naCastu128Toi128(NA_MAX_u128)
#endif



// 256 bit basic integer constants
#define NA_SIGN_MASK_256      naMakei256(NA_SIGN_MASK_128, NA_ZERO_u128)

#if NA_NATIVE_INT256_IN_USE
  #define NA_ZERO_u256          ((uint256)0u)
  #define NA_ZERO_i256          ((int256)0)
  #define NA_ONE_u256           ((uint256)1u)
  #define NA_ONE_i256           ((int256)1)

  #if defined UINT256_MAX
    #define NA_MAX_u256         UINT256_MAX
  #else
    #define NA_MAX_u256         (~NA_ZERO_u256)
  #endif
  #if defined INT256_MAX
    #define NA_MAX_i256         INT256_MAX
  #else
    #define NA_MAX_i256         (~NA_SIGN_MASK_256)
  #endif
  #if defined INT256_MIN
    #define NA_MIN_i256         INT256_MIN
  #else
    #define NA_MIN_i256         NA_SIGN_MASK_256
  #endif
  #define NA_MINUS_ONE_i256     ((int256)NA_MAX_u256)
#else
  #define NA_ZERO_u256          naMakeu256WithLo(NA_ZERO_u128)
  #define NA_ZERO_i256          naMakei256WithLo(NA_ZERO_i128)
  #define NA_ONE_u256           naMakeu256WithLo(NA_ONE_u128)
  #define NA_ONE_i256           naMakei256WithLo(NA_ONE_i128)

  #define NA_MAX_u256           naMakeu256(NA_MAX_u128, NA_MAX_u128)
  #define NA_MAX_i256           naMakei256(NA_MAX_i128, NA_MAX_u128)
  #define NA_MIN_i256           naMakei256(NA_MIN_i128, NA_ZERO_u128)
  #define NA_MINUS_ONE_i256     naCastu256Toi256(NA_MAX_u256)
#endif



// size_t basic integer constants
#define NA_ZERO_s              ((size_t)0)
#define NA_ONE_s               ((size_t)1)
#define NA_MAX_s               ((size_t)SIZE_MAX)



// Inline implementations are in a separate file and will be included later
// on in the NABase.h file.



#endif // NA_NUMERICS_INCLUDED



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
