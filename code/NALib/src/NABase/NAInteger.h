
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

#if __cplusplus
  typedef bool NABool;
  #define NA_TRUE    true
  #define NA_FALSE   false
#elif defined NA_C99
  typedef _Bool NABool;
  #define NA_TRUE    1
  #define NA_FALSE   0
#else
  typedef int NABool;
  #define NA_TRUE    1
  #define NA_FALSE   0
#endif



// Basic integer types

// Int 8 bits
typedef uint8_t               uint8;
typedef int8_t                int8;

// Int 16 bits
typedef uint16_t              uint16;
typedef int16_t               int16;

// Int 32 bits
typedef uint32_t              uint32;
typedef int32_t               int32;

// Int 64 bits
#if defined NA_TYPE_INT64
  typedef uint64_t            uint64;
  typedef int64_t             int64;
  typedef uint64              NAu64;
  typedef int64               NAi64;
#else
  typedef struct NAu64        uint64;
  typedef struct NAi64        int64;
  typedef struct NAu64        NAu64;
  typedef struct NAi64        NAi64;
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    struct NAi64 { int32  hi; uint32 lo; };
    struct NAu64 { uint32 hi; uint32 lo; };
  #else
    struct NAi64 { uint32 lo; int32  hi; };
    struct NAu64 { uint32 lo; uint32 hi; };
  #endif
#endif

// Int 128 bits
#if defined NA_TYPE_INT128
  typedef unsigned __int128   uint128;
  typedef signed   __int128   int128;
  typedef uint128             NAu128;
  typedef int128              NAi128;
#else
  typedef struct NAu128       uint128;
  typedef struct NAi128       int128;
  typedef struct NAu128       NAu128;
  typedef struct NAi128       NAi128;
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    struct NAi128 { NAi64 hi; NAu64 lo; };
    struct NAu128 { NAu64 hi; NAu64 lo; };
  #else
    struct NAi128 { NAu64 lo; NAi64 hi; };
    struct NAu128 { NAu64 lo; NAu64 hi; };
  #endif
#endif

// Int 256 bits
#if defined NA_TYPE_INT256
  typedef unsigned __int256   uint256;
  typedef signed   __int256   int256;
  typedef uint256             NAu256;
  typedef int256              NAi256;
#else
  typedef struct NAu256       uint256;
  typedef struct NAi256       int256;
  typedef struct NAu256       NAu256;
  typedef struct NAi256       NAi256;
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    struct NAi256 { NAi128 hi; NAu128 lo; };
    struct NAu256 { NAu128 hi; NAu128 lo; };
  #else
    struct NAi256 { NAu128 lo; NAi128 hi; };
    struct NAu256 { NAu128 lo; NAu128 hi; };
  #endif
#endif



// NAInt
//
// The NAInt type is the default integer used in NALib. Usually, that type
// corresponds to the byteSize which is needed for storing an address. This
// means that this type was/is dependent on the system NALib is compiled for.
// The preferred size of an NAInt can be configured in the NAConfiguration.h
// file with the NA_PREFERRED_NAINT_BITS macro.
//
// If your NAInt is defined to use less bits than an address but you are
// still in need for an integer which has the same byteSize as an address,
// use size_t or ptrdiff_t.
//
// NAInt will be used as the default integer type in NALib. Many fundamental
// functions will return NAInt or expect it as an argument. Note that the
// NAInt type is signed!
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


#define NA_SIZE_T_PRI "%zu"


#if NA_TYPE_NAINT_BITS == NA_TYPE64_BITS
  #if !defined NA_TYPE_INT64
    #warn "Primary integer type NAInt is emulated."
  #endif
  typedef int64 NAInt;
  #define NA_MAX_i NA_MAX_i64
  #define NA_MIN_i NA_MIN_i64
  #define NA_MAX_u NA_MAX_u64
  #define NA_ZERO  NA_ZERO_i64
  #define NA_ONE   NA_ONE_i64
#elif NA_TYPE_NAINT_BITS == NA_TYPE32_BITS
  typedef int32 NAInt;
  #define NA_MAX_i NA_MAX_i32
  #define NA_MIN_i NA_MIN_i32
  #define NA_MAX_u NA_MAX_u32
  #define NA_ZERO  NA_ZERO_i32
  #define NA_ONE   NA_ONE_i32
#elif NA_TYPE_NAINT_BITS == NA_TYPE16_BITS
  typedef int16 NAInt;
  #define NA_MAX_i NA_MAX_i16
  #define NA_MIN_i NA_MIN_i16
  #define NA_MAX_u NA_MAX_u16
  #define NA_ZERO  NA_ZERO_i16
  #define NA_ONE   NA_ONE_i16
#else
  #error "Can not define NAInt. Bit count not supported."
#endif



// An NAByte is a type definition of a Byte.
//
// Defining an NAByte as an uint8 can be very handy. In NALib, the NAByte type
// is often used when a memory block needs to be accessed byte by byte. You
// could also use a void-pointer for that but void-pointers are sometimes just
// a little too cumbersome to work with and do not always have a byteSize
// defined depending on the standard used. Furthermore, a pointer to an uint8
// can be displayed by a debugger while a pointer to void can not.
// Why not using the signed variant? Because there are many implementations
// using enumerations which go beyound 127 and do not want to use negative
// numbers.
typedef uint8 NAByte;



#endif // NA_INTEGER_INCLUDED



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
