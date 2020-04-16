
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"


// ABOUT THIS FILE (why is this so complicated!!!)
// Integers are easy, right? Well, if they are standardized yes, otherwise...
// Unfortunately, there is a plethora of compilers and systems out there and
// the true definition of integer types can vary at all corners. This file
// tries to capture all possibilities and in the end provide definitions for
// very "simple" datatypes like int32 or NABool.
//
// Especially critical is the definition of types which are not native on the
// current system. Like for example, the long long datatype is sometimes not
// even defined. And an int128 is barely heard of. In NALib, such types are
// emulated.


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



// Defines, if the current system compiles with long long int types.
// See NA_TYPE_ASSUME_NATIVE_LONG_LONG in the Configuration.h file for manually
// configuring this. Note that the test (_MSC_VER >= 1310) tests for
// Visual Studio 7.1
#if (defined NA_C99) || (defined NA_CPP11) || _MSC_VER >= 1310 || NA_TYPE_ASSUME_NATIVE_LONG_LONG
  #define NA_COMPILE_WITH_LONG_LONG 0
#else
  #define NA_COMPILE_WITH_LONG_LONG 0
#endif



#if defined __STDC__ || defined _MSC_VER
  // We gather basic information about integer types from the standardized
  // limits.h library (note that limits.h is available in C90 but stdint.h
  // is only available since C99)
  #include <limits.h>
#else
  #warning "This is not a standard C compiler. NALib will make assumptions about typesizes."
  // If we are completely out of the standardized C reign, we simply create
  // the macros which should be in limits.h manually.
  // Note the -1 for the signed min values: Some compilers have problems with
  // the lowest value and may output warnings. Also not that these macros
  // should work for all three negative encodings.
  #ifndef CHAR_BIT
    #define CHAR_BIT      8
  #endif
  #ifndef UCHAR_MAX
    #define UCHAR_MAX     (0xffu)
  #endif
  #ifndef SCHAR_MAX
    #define SCHAR_MAX     (0x7f)
  #endif
  #ifndef SCHAR_MIN
    #define SCHAR_MIN     (0x81 - 1)
  #endif
  #ifndef USHRT_MAX
    #define USHRT_MAX     (0xffffu)
  #endif
  #ifndef SHRT_MAX
    #define SHRT_MAX      (0x7fff)
  #endif
  #ifndef SHRT_MIN
    #define SHRT_MIN      (0x8001 - 1)
  #endif
  #ifndef UINT_MAX
    #define UINT_MAX      (0xffffffffu)
  #endif
  #ifndef INT_MAX
    #define INT_MAX       (0x7fffffff)
  #endif
  #ifndef INT_MIN
    #define INT_MIN       (0x80000001 - 1)
  #endif
  #ifndef ULONG_MAX
    #define ULONG_MAX     (0xffffffffu)
  #endif
  #ifndef LONG_MAX
    #define LONG_MAX      (0x7fffffff)
  #endif
  #ifndef LONG_MIN
    #define LONG_MIN      (0x80000001 - 1)
  #endif
  #if NA_COMPILE_WITH_LONG_LONG
    #ifndef ULLONG_MAX
      #define ULLONG_MAX  (0xffffffffffffffffuLL)
    #endif
    #ifndef LLONG_MAX
      #define LLONG_MAX   (0x7fffffffffffffffLL)
    #endif
    #ifndef LLONG_MIN
      #define LLONG_MIN   (0x8000000000000001LL - 1LL)
    #endif
  #endif
#endif



// Currently, NALib assumes a Byte to consist of precisely 8 bits. This is
// reflected in various enumerations and fundamental types like NAByte and
// NAUTF8Char. With this macro, we make sure, our code compiles just on the
// systems which have 8-Bit Bytes.
#if CHAR_BIT != NA_TYPE8_BITS
  #error "NALib can not work properly with chars unequal 8 bits."
#endif



// We test if the current system has a (positive) integer encoding suitable for
// NALib. Some obscure compilers might handle this differently.
#if (0x0100 >> 8) != 0x01
  #error "Unknown integer number encoding. NALib might not compile or run."
#endif



// We test what encoding is used for negative integers. With this knowledge,
// certain tasks can be speeded up a bit.
#if   (-1 & 3) == 3
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
#elif (-1 & 3) == 2
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_ONES_COMPLEMENT
#elif (-1 & 3) == 1
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_SIGN_MAGNITUDE
#else
  #define NA_SIGNED_INTEGER_ENCODING NA_SIGNED_INTEGER_ENCODING_UNKNOWN
  #error "Invalid signed integer encoding. NALib might not work properly."
#endif



// First, we find out, which bit lengths we have for the (old) integer types
// like int, short and long. Agreed, this is very expressive, but we must be
// very precise in that respect to guarantee universality.

// char
#if UCHAR_MAX == 0xffu
  #define NA_TYPE_NATIVE_CHAR_BITS NA_TYPE8_BITS
#elif UCHAR_MAX == 0xffffu
  #define NA_TYPE_NATIVE_CHAR_BITS NA_TYPE16_BITS
#endif
#ifndef NA_TYPE_NATIVE_CHAR_BITS
  #warning "NALib will make assumptions about the native size of a char."
  #define NA_TYPE_NATIVE_CHAR_BITS NA_TYPE8_BITS
#endif

// short
#if USHRT_MAX == 0xffffu
  #define NA_TYPE_NATIVE_SHORT_INT_BITS NA_TYPE16_BITS
#elif USHRT_MAX == 0xffffffffu
  #define NA_TYPE_NATIVE_SHORT_INT_BITS NA_TYPE32_BITS
#endif
#ifndef NA_TYPE_NATIVE_SHORT_INT_BITS
  #warning "NALib will make assumptions about the native size of a short."
  #define NA_TYPE_NATIVE_SHORT_INT_BITS NA_TYPE16_BITS
#endif

// int
#if UINT_MAX == 0xffffu
  #define NA_TYPE_NATIVE_INT_BITS NA_TYPE16_BITS
#elif UINT_MAX == 0xffffffffu
  #define NA_TYPE_NATIVE_INT_BITS NA_TYPE32_BITS
#endif
#if NA_COMPILE_WITH_LONG_LONG
  #if UINT_MAX == 0xffffffffffffffffuLL
    #define NA_TYPE_NATIVE_INT_BITS NA_TYPE64_BITS
  #endif
#endif
#ifndef NA_TYPE_NATIVE_INT_BITS
  #warning "NALib will make assumptions about the native size of an int."
  #define NA_TYPE_NATIVE_INT_BITS NA_TYPE32_BITS
#endif

// long
#if ULONG_MAX == 0xffffffffu
  #define NA_TYPE_NATIVE_LONG_INT_BITS NA_TYPE32_BITS
#endif
#if NA_COMPILE_WITH_LONG_LONG
  #if ULONG_MAX == 0xffffffffffffffffuLL
    #define NA_TYPE_NATIVE_LONG_INT_BITS NA_TYPE64_BITS
  #endif
#endif
#ifndef NA_TYPE_NATIVE_LONG_INT_BITS
  #warning "NALib will make assumptions about the native size of a long."
  #define NA_TYPE_NATIVE_LONG_INT_BITS NA_TYPE32_BITS
#endif

// long long
#if NA_COMPILE_WITH_LONG_LONG
  #if ULLONG_MAX == 0xffffffffffffffffuLL
    #define NA_TYPE_NATIVE_LONG_LONG_INT_BITS NA_TYPE64_BITS
  #endif
#endif
#ifndef NA_TYPE_NATIVE_LONG_LONG_INT_BITS
  // Here, we do not assume anything. long long types are not accepted
  // by all compilers.
#endif



// Make a reverse mapping towards which bit width corresponds to which.
// native type. The author is perfectly aware that this is a little
// over the top, but it allows for a preference and helps for later
// PRI and SCN macros.

#define NA_TYPE_NATIVE_CHAR           0
#define NA_TYPE_NATIVE_SHORT_INT      1
#define NA_TYPE_NATIVE_INT            2
#define NA_TYPE_NATIVE_LONG_INT       3
#define NA_TYPE_NATIVE_LONG_LONG_INT  4
#define NA_TYPE_NATIVE_INT_128        5

// The following tests will define the macros
// NA_TYPE_INT8
// NA_TYPE_INT16
// NA_TYPE_INT32
// NA_TYPE_INT64
// NA_TYPE_INT128
// which, when defined, denote one of the upper native types.
// If undefined, no native representation is possible.

// 8 Bits: Should be char. Otherwise, we might have a "slight" problem.
#if NA_TYPE_NATIVE_CHAR_BITS == NA_TYPE8_BITS
  #define NA_TYPE_INT8 NA_TYPE_NATIVE_CHAR
#else
  #error "We have a slight problem. Char type is not 8 bits"
#endif

// 16 Bits: Preference is first short, then int
#if NA_TYPE_NATIVE_SHORT_INT_BITS == NA_TYPE16_BITS
  #define NA_TYPE_INT16 NA_TYPE_NATIVE_SHORT_INT
#elif NA_TYPE_NATIVE_INT_BITS == NA_TYPE16_BITS
  #define NA_TYPE_INT16 NA_TYPE_NATIVE_INT
#endif

// 32 Bits: Preference is first int, then long, then short
#if NA_TYPE_NATIVE_INT_BITS == NA_TYPE32_BITS
  #define NA_TYPE_INT32 NA_TYPE_NATIVE_INT
#elif NA_TYPE_NATIVE_LONG_INT_BITS == NA_TYPE32_BITS
  #define NA_TYPE_INT32 NA_TYPE_NATIVE_LONG_INT
#elif NA_TYPE_NATIVE_SHORT_INT_BITS == NA_TYPE32_BITS
  #define NA_TYPE_INT32 NA_TYPE_NATIVE_SHORT_INT
#endif

// 64 Bits: Preference is first long long, then long, then int
// Special case for MSVC compiler 6.0: Use __int64
#if NA_TYPE_NATIVE_LONG_LONG_INT_BITS == NA_TYPE64_BITS
  #define NA_TYPE_INT64 NA_TYPE_NATIVE_LONG_LONG_INT
#elif NA_TYPE_NATIVE_LONG_INT_BITS == NA_TYPE64_BITS
  #define NA_TYPE_INT64 NA_TYPE_NATIVE_LONG_INT
#elif NA_TYPE_NATIVE_INT_BITS == NA_TYPE64_BITS
  #define NA_TYPE_INT64 NA_TYPE_NATIVE_INT
#endif

// 128 Bits: Dependent on the system/compiler/extension.
#ifdef __SIZEOF_INT128__
  #define NA_TYPE_INT128 NA_TYPE_NATIVE_INT_128
#endif

// Note that the following macros are defined using the values provided in
// the <limits.h> header file. This is usually not the preferred way but
// in older standards before C99, there was just no other way to do it.
#if NA_COMPILE_WITH_LONG_LONG
  #define NA_PRINTF_CHAR_PREFIX "hh"
  #define NA_PRINTF_LONG_LONG_PREFIX "ll"
#else
  #define NA_PRINTF_CHAR_PREFIX ""
  #define NA_PRINTF_LONG_LONG_PREFIX ""
#endif

// 8 bits
#if NA_TYPE_INT8 == NA_TYPE_NATIVE_CHAR
  #define NA_UINT8_MAX  UCHAR_MAX
  #define NA_INT8_MAX   SCHAR_MAX
  #define NA_INT8_MIN   SCHAR_MIN
  typedef unsigned char uint8;
  typedef signed char   int8;
  #define NA_ZERO_8     (0)
  #define NA_ONE_8      (1)
  #define NA_ZERO_8u    (0u)
  #define NA_ONE_8u     (1u)
  #define NA_PRIi8      NA_PRINTF_CHAR_PREFIX "d"
  #define NA_PRIu8      NA_PRINTF_CHAR_PREFIX "u"
  #define NA_PRIx8      NA_PRINTF_CHAR_PREFIX "x"
  #define NA_SCNi8      NA_PRINTF_CHAR_PREFIX "d"
  #define NA_SCNu8      NA_PRINTF_CHAR_PREFIX "u"
#endif

// 16 bits
#if NA_TYPE_INT16 == NA_TYPE_NATIVE_CHAR
  #define NA_UINT16_MAX  UCHAR_MAX
  #define NA_INT16_MAX   SCHAR_MAX
  #define NA_INT16_MIN   SCHAR_MIN
  typedef unsigned char  uint16;
  typedef signed char    int16;
  #define NA_ZERO_16     (0)
  #define NA_ONE_16      (1)
  #define NA_ZERO_16u    (0u)
  #define NA_ONE_16u     (1u)
  #define NA_PRIi16      NA_PRINTF_CHAR_PREFIX "d"
  #define NA_PRIu16      NA_PRINTF_CHAR_PREFIX "u"
  #define NA_PRIx16      NA_PRINTF_CHAR_PREFIX "x"
  #define NA_SCNi16      NA_PRINTF_CHAR_PREFIX "d"
  #define NA_SCNu16      NA_PRINTF_CHAR_PREFIX "u"
#elif NA_TYPE_INT16 == NA_TYPE_NATIVE_SHORT_INT
  #define NA_UINT16_MAX  USHRT_MAX
  #define NA_INT16_MAX   SHRT_MAX
  #define NA_INT16_MIN   SHRT_MIN
  typedef unsigned short uint16;
  typedef signed short   int16;
  #define NA_ZERO_16     (0)
  #define NA_ONE_16      (1)
  #define NA_ZERO_16u    (0u)
  #define NA_ONE_16u     (1u)
  #define NA_PRIi16      "hd"
  #define NA_PRIu16      "hu"
  #define NA_PRIx16      "hx"
  #define NA_SCNi16      "hd"
  #define NA_SCNu16      "hu"
#endif

// 32 bits
#if NA_TYPE_INT32 == NA_TYPE_NATIVE_SHORT_INT
  #define NA_UINT32_MAX  USHRT_MAX
  #define NA_INT32_MAX   SHRT_MAX
  #define NA_INT32_MIN   SHRT_MIN
  typedef unsigned short uint32;
  typedef signed short   int32;
  #define NA_ZERO_32     (0)
  #define NA_ONE_32      (1)
  #define NA_ZERO_32u    (0u)
  #define NA_ONE_32u     (1u)
  #define NA_PRIi32      "hd"
  #define NA_PRIu32      "hu"
  #define NA_PRIx32      "hx"
  #define NA_SCNi32      "hd"
  #define NA_SCNu32      "hu"
#elif NA_TYPE_INT32 == NA_TYPE_NATIVE_INT
  #define NA_UINT32_MAX  UINT_MAX
  #define NA_INT32_MAX   INT_MAX
  #define NA_INT32_MIN   INT_MIN
  typedef unsigned int   uint32;
  typedef signed int     int32;
  #define NA_ZERO_32     (0)
  #define NA_ONE_32      (1)
  #define NA_ZERO_32u    (0u)
  #define NA_ONE_32u     (1u)
  #define NA_PRIi32      "d"
  #define NA_PRIu32      "u"
  #define NA_PRIx32      "x"
  #define NA_SCNi32      "d"
  #define NA_SCNu32      "u"
#elif NA_TYPE_INT32 == NA_TYPE_NATIVE_LONG_INT
  #define NA_UINT32_MAX  ULONG_MAX
  #define NA_INT32_MAX   LONG_MAX
  #define NA_INT32_MIN   LONG_MIN
  typedef unsigned long  uint32;
  typedef signed long    int32;
  #define NA_ZERO_32     (0L)
  #define NA_ONE_32      (1L)
  #define NA_ZERO_32u    (0uL)
  #define NA_ONE_32u     (1uL)
  #define NA_PRIi32      "ld"
  #define NA_PRIu32      "lu"
  #define NA_PRIx32      "lx"
  #define NA_SCNi32      "ld"
  #define NA_SCNu32      "lu"
#endif

// 64 bits
#if NA_TYPE_INT64 == NA_TYPE_NATIVE_INT
  #define NA_UINT64_MAX  UINT_MAX
  #define NA_INT64_MAX   INT_MAX
  #define NA_INT64_MIN   INT_MIN
  typedef unsigned int   uint64;
  typedef signed int     int64;
  #define NA_ZERO_64     (0)
  #define NA_ONE_64      (1)
  #define NA_ZERO_64u    (0u)
  #define NA_ONE_64u     (1u)
  #define NA_PRIi64      "d"
  #define NA_PRIu64      "u"
  #define NA_PRIx64      "x"
  #define NA_SCNi64      "d"
  #define NA_SCNu64      "u"
#elif NA_TYPE_INT64 == NA_TYPE_NATIVE_LONG_INT
  #define NA_UINT64_MAX  ULONG_MAX
  #define NA_INT64_MAX   LONG_MAX
  #define NA_INT64_MIN   LONG_MIN
  typedef unsigned long  uint64;
  typedef signed long    int64;
  #define NA_ZERO_64     (0L)
  #define NA_ONE_64      (1L)
  #define NA_ZERO_64u    (0uL)
  #define NA_ONE_64u     (1uL)
  #define NA_PRIi64      "ld"
  #define NA_PRIu64      "lu"
  #define NA_PRIx64      "lx"
  #define NA_SCNi64      "ld"
  #define NA_SCNu64      "lu"
#elif NA_TYPE_INT64 == NA_TYPE_NATIVE_LONG_LONG_INT
	#define NA_UINT64_MAX  ULLONG_MAX
	#define NA_INT64_MAX   LLONG_MAX
	#define NA_INT64_MIN   LLONG_MIN
	typedef unsigned long long   uint64;
	typedef signed long long     int64;
	#define NA_ZERO_64     (0LL)
	#define NA_ONE_64      (1LL)
	#define NA_ZERO_64u    (0uLL)
	#define NA_ONE_64u     (1uLL)
	#define NA_PRIi64      NA_PRINTF_LONG_LONG_PREFIX "d"
	#define NA_PRIu64      NA_PRINTF_LONG_LONG_PREFIX "u"
	#define NA_PRIx64      NA_PRINTF_LONG_LONG_PREFIX "x"
	#define NA_SCNi64      NA_PRINTF_LONG_LONG_PREFIX "d"
	#define NA_SCNu64      NA_PRINTF_LONG_LONG_PREFIX "u"
#endif

// 128 bits
#if NA_TYPE_INT128 == NA_TYPE_NATIVE_INT_128
  // Note: These definitions have not been tested at all.
  // The author did not have time to install gcc and add
  // proper values. This will likely not compile.
  #define NA_UINT128_MAX  UINT128_MAX
  #define NA_INT128_MAX   INT128_MAX
  #define NA_INT128_MIN   INT128_MIN
  typedef unsigned __int128_t   uint128;
  typedef signed __int128_t     int128;
  #define NA_ZERO_128     (0LLLLLLELEVEN)
  #define NA_ONE_128      (1LLLLLLELEVEN)
  #define NA_ZERO_128u    (0uLLLLLLELEVEN)
  #define NA_ONE_128u     (1uLLLLLLELEVEN)
  #define NA_PRIi128      "d CANTSHOWi128 "
  #define NA_PRIu128      "u CANTSHOWi128 "
  #define NA_PRIx128      "x CANTSHOWi128 "
  #define NA_SCNi128      "d CANTSHOWi128 "
  #define NA_SCNu128      "u CANTSHOWi128 "
#endif

// NAInt and NAUInt
//
// The NAInt type is the default integer used in NALib. Up until version 20,
// it always had the bytesize which is needed for storing an address. This
// means that this type was/is dependent on the system NALib is compiled for.
// It can for example be 32 Bits on one and 64 Bits on another architecture.
// After version 20, depending on the system you compile NALib with, the
// desired typesize might not be available and hence, NALib automatically
// adapts NAInt to a suitable bytesize. The desired bytesize of an integer
// can therefore differ from the size of an address and can be configured in
// the NAConfiguration.h file with the NA_PREFERRED_NAINT_BITS macro.
//
// If you are still looking for an integer which has the same bytesize as
// an address, have a look at NASizeInt.
//
// NAInt will be used as the default integer type in NALib. Many fundamental
// functions will return NAInt or expect it as an argument. Note that the NAInt
// type is signed!
//
// Starting with NALib Version 9, there also exists NAUInt, the unsigned
// variant. Many functions have been changed which explicitely require or
// return an unsigned integer. This helps detecting errors as a compiler can
// emit warnings on sign differences.
//
// The signed variant NAInt is also used by memory and array functions. This
// means that in 32-Bit systems, only about 2 billion entries can be accessed
// with arrays when using NALib. The author is perfectly aware of the fact
// that most standard library functions use an unsigned integer for size_t,
// and that NALib therefore will have some minor limitations regarding memory
// usage. But the author has been convinced by the simplicity and elegance of
// just using signed integers during the last couple of years. It makes
// programming and debugging so much easier. And in 64 Bit systems, the memory
// limitations will not be a problem for a couple of years coming.
//
// In addition to the type, there is the definition of a printf-argument macro.
// Use the macro for example like this:
//
// printf("The point is at X coordinate %" NA_PRIi, pos.x);
// printf("The array has %" NA_PRIu " entries.", naGetArrayCount(array));
//
// One serious drawback of a system-dependent definition is that NALib might
// behave differently depending on the system it is compiled for. But it will
// only behave differently in cases where the behaviour would become difficult
// anyway. Meaning: You will most likely run into out-of-memory problems first.
//
// Note that a different solution would be to always go for int64 and convert
// between the different integer types. But compiling NALib on a non-64-Bit
// system then would definitely be a waste of space with lots of implicit
// arithmetic conversions.

// Define the size of NAInt
#if NA_PREFERRED_NAINT_BITS == 0  // Automatic
  #define NA_TYPE_NAINT_BITS NA_SYSTEM_ADDRESS_BITS
  #if NA_TYPE_NAINT_BITS == NA_TYPE64_BITS && !defined NA_TYPE_INT64
    #undef NA_TYPE_NAINT_BITS
    #define NA_TYPE_NAINT_BITS NA_TYPE32_BITS
  #endif
#elif NA_PREFERRED_NAINT_BITS == 1  // int
  #define NA_TYPE_NAINT_BITS NA_SYSTEM_NATIVE_INT_BITS
#elif NA_PREFERRED_NAINT_BITS == 2  // long
  #define NA_TYPE_NAINT_BITS NA_SYSTEM_NATIVE_LONG_INT_BITS
#elif NA_PREFERRED_NAINT_BITS == 3  // address length
  #define NA_TYPE_NAINT_BITS NA_SYSTEM_ADDRESS_BITS
#elif NA_PREFERRED_NAINT_BITS == 32 // 32
  #define NA_TYPE_NAINT_BITS NA_TYPE32_BITS
#elif NA_PREFERRED_NAINT_BITS == 64 // 64
  #define NA_TYPE_NAINT_BITS NA_TYPE64_BITS
#else
  #error "NA_PREFERRED_NAINT_BITS has an invalid value"
#endif

// We include the 64bit and 128bit emulation.
#include "NAInt64.h"
#include "NAInt128.h"

// In case, there was no int64 defined before, we have to emulate it now.
#if !defined NA_TYPE_INT64
  #define NA_UINT64_MAX  naMakeUInt64(NA_UINT32_MAX, NA_UINT32_MAX)
  #define NA_INT64_MAX   naCastUInt64ToInt64(naMakeUInt64((uint32)NA_INT32_MAX, NA_UINT32_MAX))
  #define NA_INT64_MIN   naCastUInt64ToInt64(naMakeUInt64((uint32)NA_INT32_MIN, NA_UINT32_MAX))
  typedef NAInt64        int64;
  typedef NAUInt64       uint64;
  #define NA_ZERO_64     (naMakeInt64WithLo(NA_ZERO_32))
  #define NA_ONE_64      (naMakeInt64WithLo(NA_ONE_32))
  #define NA_ZERO_64u    (naMakeUInt64WithLo(NA_ZERO_32u))
  #define NA_ONE_64u     (naMakeUInt64WithLo(NA_ONE_32u))
  #define NA_PRIi64      "d CANTSHOWi64 "
  #define NA_PRIu64      "u CANTSHOWi64 "
  #define NA_PRIx64      "x CANTSHOWi64 "
  #define NA_SCNi64      "d CANTSHOWi64 "
  #define NA_SCNu64      "u CANTSHOWi64 "
#endif

// In case, there was no int128 defined before, we have to emulate it now.
#if !defined NA_TYPE_INT128
  #define NA_UINT128_MAX  naMakeUInt128(NA_UINT64_MAX, NA_UINT64_MAX)
  #define NA_INT128_MAX   naCastUInt128ToInt128(naMakeUInt128((uint64)NA_INT64_MAX, NA_UINT64_MAX))
  #define NA_INT128_MIN   naCastUInt128ToInt128(naMakeUInt128((uint64)NA_INT64_MIN, NA_UINT64_MAX))
  typedef NAInt128        int128;
  typedef NAUInt128       uint128;
  #define NA_ZERO_128     (naMakeInt128WithLo(NA_ZERO_64))
  #define NA_ONE_128      (naMakeInt128WithLo(NA_ONE_64))
  #define NA_ZERO_128u    (naMakeUInt128WithLo(NA_ZERO_64u))
  #define NA_ONE_128u     (naMakeUInt128WithLo(NA_ONE_64u))
  #define NA_PRIi128      "d CANTSHOWi128 "
  #define NA_PRIu128      "u CANTSHOWi128 "
  #define NA_PRIx128      "x CANTSHOWi128 "
  #define NA_SCNi128      "d CANTSHOWi128 "
  #define NA_SCNu128      "u CANTSHOWi128 "
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
  #define NA_INT_MAX NA_INT64_MAX
  #define NA_INT_MIN NA_INT64_MIN
  #define NA_UINT_MAX NA_UINT64_MAX
  #define NA_ZERO NA_ZERO_64
  #define NA_ONE  NA_ONE_64
#elif NA_TYPE_NAINT_BITS == NA_TYPE32_BITS
  typedef int32 NAInt;
  typedef uint32 NAUInt;
  #define NA_PRIi NA_PRIi32
  #define NA_PRIu NA_PRIu32
  #define NA_PRIx NA_PRIx32
  #define NA_SCNi NA_SCNi32
  #define NA_SCNu NA_SCNu32
  #define NA_INT_MAX NA_INT32_MAX
  #define NA_INT_MIN NA_INT32_MIN
  #define NA_UINT_MAX NA_UINT32_MAX
  #define NA_ZERO NA_ZERO_32
  #define NA_ONE  NA_ONE_32
#elif NA_TYPE_NAINT_BITS == NA_TYPE16_BITS
  typedef int16 NAInt;
  typedef uint16 NAUInt;
  #define NA_PRIi NA_PRIi16
  #define NA_PRIu NA_PRIu16
  #define NA_PRIx NA_PRIx16
  #define NA_SCNi NA_SCNi16
  #define NA_SCNu NA_SCNu16
  #define NA_INT_MAX NA_INT16_MAX
  #define NA_INT_MIN NA_INT16_MIN
  #define NA_UINT_MAX NA_UINT16_MAX
  #define NA_ZERO NA_ZERO_16
  #define NA_ONE  NA_ONE_16
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
// there does not exist a native integer type which holds that much bits, the
// type is defined to be the same as NAInt. If that NAInt holds less than the
// required bits, some parts NALib, especially NARuntime, might not work
// properly. A warning is emitted and a special macro is defined. If you want
// to silence the warning, see NA_TYPE_WARN_IF_NO_NATIVE_ADDRESS_TYPE.

#if NA_SYSTEM_ADDRESS_BITS == NA_TYPE64_BITS && defined NA_TYPE_INT64
  typedef int64  NASizeInt;
  typedef uint64 NASizeUInt;
#elif NA_SYSTEM_ADDRESS_BITS == NA_TYPE32_BITS && defined NA_TYPE_INT32
  typedef int32  NASizeInt;
  typedef uint32 NASizeUInt;
#elif NA_SYSTEM_ADDRESS_BITS == NA_TYPE16_BITS && defined NA_TYPE_INT16
  typedef int16  NASizeInt;
  typedef uint16 NASizeUInt;
#else
  typedef NAInt  NASizeInt;
  typedef NAUInt NASizeUInt;
  #if NA_TYPE_NAINT_BITS < NA_SYSTEM_ADDRESS_BITS
    #if NA_TYPE_WARN_IF_NO_NATIVE_ADDRESS_TYPE == 1
      #warning "No native integer type available to store an address"
    #endif
    #define NA_SYSTEM_SIZEINT_NOT_ADDRESS_SIZE
  #endif
#endif

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
