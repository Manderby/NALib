
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_INTEGER_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INTEGER_INCLUDED
#define NA_INTEGER_INCLUDED



// NABool
// In NALib, the definition of NABool is set to an int.
// Not char or unsigned char. This is unusual but most probably the easiest
// way to use whatever the current processor can use as its fastest integer.
//
// It is in the believe of the author, that in modern computers, speed is more
// important than space as opposed to earlier times where wasting space was a
// complete no-go. When using lots of Boolean values, there are better ways to
// store them than in NABools anyway: Use C-style-masks.
//
// Note that there is no distinction between C and C++. Therefore, NALib
// never uses the bool type or the true and false keywords. Also the macros
// available in C11 are unused.

typedef int NABool;
#define NA_TRUE    1
#define NA_FALSE   0



// Int 8 bits
typedef uint8_t        uint8;
typedef int8_t         int8;

#define NA_MAX_u8      UINT8_MAX
#define NA_MAX_i8      INT8_MAX
#define NA_MIN_i8      INT8_MIN
#define NA_ZERO_u8     ((uint8)0u)
#define NA_ONE_u8      ((uint8)1u)
#define NA_ZERO_i8     ((int8)0)
#define NA_ONE_i8      ((int8)1)



// Int 16 bits
typedef uint16_t       uint16;
typedef int16_t        int16;

#define NA_MAX_u16     UINT16_MAX
#define NA_MAX_i16     INT16_MAX
#define NA_MIN_i16     INT16_MIN
#define NA_ZERO_u16    ((uint16)0u)
#define NA_ONE_u16     ((uint16)1u)
#define NA_ZERO_i16    ((int16)0)
#define NA_ONE_i16     ((int16)1)



// Int 32 bits
typedef uint32_t       uint32;
typedef int32_t        int32;

#define NA_MAX_u32     UINT32_MAX
#define NA_MAX_i32     INT32_MAX
#define NA_MIN_i32     INT32_MIN
#define NA_ZERO_u32    ((uint32)0u)
#define NA_ONE_u32     ((uint32)1u)
#define NA_ZERO_i32    ((int32)0)
#define NA_ONE_i32     ((int32)1)



// Int 64 bits
#if defined NA_TYPE_INT64
  typedef uint64_t       uint64;
  typedef int64_t        int64;
  typedef uint64         NAUInt64;
  typedef int64          NAInt64;
  #if defined UINT64_MAX
    #define NA_MAX_u64   UINT64_MAX
  #else
    #define NA_MAX_u64   (0xffffffffffffffffu)
  #endif
  #if defined INT64_MAX
    #define NA_MAX_i64   INT64_MAX
  #else
    #define NA_MAX_i64   (0x7fffffffffffffff)
  #endif
  #if defined INT64_MIN
    #define NA_MIN_i64   INT64_MIN
  #else
    #define NA_MIN_i64   (0x8000000000000001 - 1)
  #endif
  #define NA_ZERO_u64    ((uint64)0u)
  #define NA_ONE_u64     ((uint64)1u)
  #define NA_ZERO_i64    ((int64)0)
  #define NA_ONE_i64     ((int64)1)
#else
  typedef struct NAUInt64 uint64;
  typedef struct NAInt64  int64;
  typedef struct NAUInt64 NAUInt64;
  typedef struct NAInt64  NAInt64;
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    struct NAInt64 { int32 hi;  uint32 lo; };
    struct NAUInt64{ uint32 hi; uint32 lo; };
  #else
    struct NAInt64 { uint32 lo; int32  hi; };
    struct NAUInt64{ uint32 lo; uint32 hi; };
  #endif

  #define NA_MAX_u64     naMakeUInt64(NA_MAX_u32, NA_MAX_u32)
  #define NA_MAX_i64     naMakeInt64(NA_MAX_i32, NA_MAX_u32)
  #define NA_MIN_i64     naMakeInt64(NA_MIN_i32, NA_ZERO_u32)
  #define NA_ZERO_u64    naMakeUInt64WithLo(NA_ZERO_u32)
  #define NA_ONE_u64     naMakeUInt64WithLo(NA_ONE_u32)
  #define NA_ZERO_i64    naMakeInt64WithLo(NA_ZERO_i32)
  #define NA_ONE_i64     naMakeInt64WithLo(NA_ONE_i32)
#endif



// Int 128 bits
#if defined NA_TYPE_INT128
  typedef unsigned __int128   uint128;
  typedef signed   __int128   int128;
  typedef uint128             NAUInt128
  typedef int128              NAInt128
  #if defined UINT128_MAX
    #define NA_MAX_u128  UINT128_MAX
  #else
    #define NA_MAX_u128  (0xffffffffffffffffffffffffffffffffu)
  #endif
  #if defined INT128_MAX
    #define NA_MAX_i128  INT128_MAX
  #else
    #define NA_MAX_i128  (0x7fffffffffffffffffffffffffffffff)
  #endif
  #if defined INT128_MIN
    #define NA_MIN_i128  INT128_MIN
  #else
    #define NA_MIN_i128  (0x80000000000000000000000000000001 - 1)
  #endif
  #define NA_ZERO_u128   ((uint128)0u)
  #define NA_ONE_u128    ((uint128)1u)
  #define NA_ZERO_i128   ((int128)0)
  #define NA_ONE_i128    ((int128)1)
#else
  typedef struct NAUInt128 uint128;
  typedef struct NAInt128  int128;
  typedef struct NAUInt128 NAUInt128;
  typedef struct NAInt128  NAInt128;
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    struct NAInt128 { NAInt64 hi;  NAUInt64 lo; };
    struct NAUInt128{ NAUInt64 hi; NAUInt64 lo; };
  #else
    struct NAInt128 { NAUInt64 lo; NAInt64  hi; };
    struct NAUInt128{ NAUInt64 lo; NAUInt64 hi; };
  #endif

  #define NA_MAX_u128    naMakeUInt128(NA_MAX_u64, NA_MAX_u64)
  #define NA_MAX_i128    naMakeInt128(NA_MAX_i64, NA_MAX_u64)
  #define NA_MIN_i128    naMakeInt128(NA_MIN_i64, NA_ZERO_u64)
  #define NA_ZERO_u128   naMakeUInt128WithLo(NA_ZERO_u64)
  #define NA_ONE_u128    naMakeUInt128WithLo(NA_ONE_u64)
  #define NA_ZERO_i128   naMakeInt128WithLo(NA_ZERO_i64)
  #define NA_ONE_i128    naMakeInt128WithLo(NA_ONE_i64)
#endif



// Int 256 bits
#if defined NA_TYPE_INT256
  typedef unsigned __int256   uint256;
  typedef signed   __int256   int256;
  typedef uint256            NAUInt256
  typedef int256             NAInt256
  #if defined UINT256_MAX
    #define NA_MAX_u256    UINT256_MAX
  #else
    #define NA_MAX_u256  (0xffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffu)
  #endif
  #if defined INT256_MAX
    #define NA_MAX_i256    INT256_MAX
  #else
    #define NA_MAX_i256   (0x7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff)
  #endif
  #if defined INT256_MIN
    #define NA_MIN_i256    INT256_MIN
  #else
    #define NA_MIN_i256   (0x8000000000000000000000000000000000000000000000000000000000000001 - 1)
  #endif
  #define NA_ZERO_u256   ((uint256)0u)
  #define NA_ONE_u256    ((uint256)1u)
  #define NA_ZERO_i256   ((int256)0)
  #define NA_ONE_i256    ((int256)1)
#else
  typedef struct NAUInt256 uint256;
  typedef struct NAInt256  int256;
  typedef struct NAUInt256 NAUInt256;
  typedef struct NAInt256  NAInt256;
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    struct NAInt256 { NAInt128 hi;  NAUInt128 lo; };
    struct NAUInt256{ NAUInt128 hi; NAUInt128 lo; };
  #else
    struct NAInt256 { NAUInt128 lo; NAInt128  hi; };
    struct NAUInt256{ NAUInt128 lo; NAUInt128 hi; };
  #endif

  #define NA_MAX_u256    naMakeUInt256(NA_MAX_u128, NA_MAX_u128)
  #define NA_MAX_i256    naMakeInt256(NA_MAX_i128, NA_MAX_u128)
  #define NA_MIN_i256    naMakeInt256(NA_MIN_i128, NA_ZERO_u128)
  #define NA_ZERO_u256   naMakeUInt256WithLo(NA_ZERO_u128)
  #define NA_ONE_u256    naMakeUInt256WithLo(NA_ONE_u128)
  #define NA_ZERO_i256   naMakeInt256WithLo(NA_ZERO_i128)
  #define NA_ONE_i256    naMakeInt256WithLo(NA_ONE_i128)
#endif



// NAInt and NAUInt
//
// The NAInt type is the default integer used in NALib. Usually, that type
// corresponds to the bytesize which is needed for storing an address. This
// means that this type was/is dependent on the system NALib is compiled for.
// The preferred size of an NAInt can be configured in the NAConfiguration.h
// file with the NA_PREFERRED_NAINT_BITS macro.
//
// If your NAInt is defined to use less bits than an address but you are
// still in need for an integer which has the same bytesize as an address,
// have a look at NASizeInt.
//
// NAInt will be used as the default integer type in NALib. Many fundamental
// functions will return NAInt or expect it as an argument. Note that the
// NAInt type is signed!
//
// There also exists NAUInt, the unsigned variant. Some functions explicitely
// require or return an unsigned integer. This helps detecting errors as a
// compiler can emit warnings on sign differences.
//
// The signed variant NAInt is also used for enums as well as by memory and
// array functions. This means that in 32-Bit systems, only about 2 billion
// entries can be accessed with arrays when using NALib. The author is aware
// of the fact that most standard library functions use an unsigned integer
// for size_t, and that NALib therefore will have some minor limitations
// regarding memory usage. But NALib is designed for simplicity. Using a
// signed integer type makes programming and debugging so much easier. And
// in 64 Bit systems, the memory limitations will not be a problem for a
// couple of years coming.
//
// In addition to the type, there is the definition of a printf-argument
// macro. Use the macro for example like this:
//
// printf("The point is at X coordinate %" NA_PRIi, pos.x);
// printf("The array has %" NA_PRIu " entries.", naGetArrayCount(array));
//
// One serious drawback of a system-dependent definition is that NALib might
// behave differently depending on the system it is compiled for. But it will
// only behave differently in cases where the behaviour would get difficult
// anyway. So you will most likely run into out-of-memory problems first.
//
// Note that a different solution would be to always go for int64 and convert
// between the different integer types. But compiling NALib on a non-64-Bit
// system then would definitely be a waste of space with lots of implicit
// arithmetic conversions.

// Define the size of NAInt
#if NA_PREFERRED_NAINT_BITS == 0  // Automatic
  #define NA_TYPE_NAINT_BITS NA_ADDRESS_BITS
#elif NA_PREFERRED_NAINT_BITS == 32 // 32
  #define NA_TYPE_NAINT_BITS NA_TYPE32_BITS
#elif NA_PREFERRED_NAINT_BITS == 64 // 64
  #define NA_TYPE_NAINT_BITS NA_TYPE64_BITS
#else
  #error "NA_PREFERRED_NAINT_BITS has an invalid value"
#endif





#if NA_TYPE_NAINT_BITS == NA_TYPE64_BITS
  #if !defined NA_TYPE_INT64
    #warn "Primary integer type NAInt is emulated."
  #endif
  typedef int64 NAInt;
  typedef uint64 NAUInt;
  #define NA_PRIi NA_PRIi64
  #define NA_PRIu NA_PRIu64
  #define NA_PRIx NA_PRIx64
  #define NA_SCNi NA_SCNi64
  #define NA_SCNu NA_SCNu64
  #define NA_MAX_i NA_MAX_i64
  #define NA_MIN_i NA_MIN_i64
  #define NA_MAX_u NA_MAX_u64
  #define NA_ZERO NA_ZERO_i64
  #define NA_ONE  NA_ONE_i64
#elif NA_TYPE_NAINT_BITS == NA_TYPE32_BITS
  typedef int32 NAInt;
  typedef uint32 NAUInt;
  #define NA_PRIi NA_PRIi32
  #define NA_PRIu NA_PRIu32
  #define NA_PRIx NA_PRIx32
  #define NA_SCNi NA_SCNi32
  #define NA_SCNu NA_SCNu32
  #define NA_MAX_i NA_MAX_i32
  #define NA_MIN_i NA_MIN_i32
  #define NA_MAX_u NA_MAX_u32
  #define NA_ZERO NA_ZERO_i32
  #define NA_ONE  NA_ONE_i32
#elif NA_TYPE_NAINT_BITS == NA_TYPE16_BITS
  typedef int16 NAInt;
  typedef uint16 NAUInt;
  #define NA_PRIi NA_PRIi16
  #define NA_PRIu NA_PRIu16
  #define NA_PRIx NA_PRIx16
  #define NA_SCNi NA_SCNi16
  #define NA_SCNu NA_SCNu16
  #define NA_MAX_i NA_MAX_i16
  #define NA_MIN_i NA_MIN_i16
  #define NA_MAX_u NA_MAX_u16
  #define NA_ZERO NA_ZERO_i16
  #define NA_ONE  NA_ONE_i16
#else
  #error "Can not define NAInt. Bit count not supported."
#endif



// An NAByte is a type definition of a Byte.
//
// Defining an NAByte as an uint8 can be very handy. In NALib, the NAByte type
// is often used when a memory block needs to be accessed byte by byte. You
// could also use a void-pointer for that but void-pointers are sometimes just
// a little too cumbersome to work with and do not always have a bytesize
// defined depending on the standard used. Furthermore, a pointer to an uint8
// can be displayed by a debugger while a pointer to void can not.
// Why not using the signed variant? Because there are many implementations
// using enumerations which go beyound 127 and do not want to use negative
// numbers.
typedef uint8 NAByte;



// NASizeInt is the type which can hold size_t values. These are used for
// example if two pointers are subtracted from each other. More precisely
// it is the type returned by the sizeof operator.
//
// NALib assumes that this is the same as the number of bits used for an
// address.
//
// NASizeInt is always a native int type, never emulated. If for any reason,
// there does not exist a native integer type with as many bits as are
// required for an address, an error is emitted as NALib will most likely
// die a horrible death.

#if NA_ADDRESS_BITS == NA_TYPE64_BITS && defined NA_TYPE_INT64
  typedef int64  NASizeInt;
  typedef uint64 NASizeUInt;
#elif NA_ADDRESS_BITS == NA_TYPE32_BITS && defined NA_TYPE_INT32
  typedef int32  NASizeInt;
  typedef uint32 NASizeUInt;
#elif NA_ADDRESS_BITS == NA_TYPE16_BITS && defined NA_TYPE_INT16
  typedef int16  NASizeInt;
  typedef uint16 NASizeUInt;
#else
  typedef NAInt  NASizeInt;
  typedef NAUInt NASizeUInt
  #if NA_TYPE_NAINT_BITS < NA_ADDRESS_BITS
    #error "No native integer type available to store an address"
  #endif
#endif


#endif // NA_INTEGER_INCLUDED

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
