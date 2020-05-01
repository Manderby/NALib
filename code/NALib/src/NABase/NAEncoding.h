
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#if defined NA_ENCODING_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_ENCODING_INCLUDED
#define NA_ENCODING_INCLUDED



// We gather basic information about integer types from the standardized
// limits.h library
#include <limits.h>
#include <stdint.h>


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



// The various signed integer encodings:
#define NA_SIGNED_INTEGER_ENCODING_UNKNOWN          0
#define NA_SIGNED_INTEGER_ENCODING_SIGN_MAGNITUDE   1
#define NA_SIGNED_INTEGER_ENCODING_ONES_COMPLEMENT  2
#define NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT  3



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



// We try to find out, which bit lengths we have for the (old) integer types
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
#elif UINT_MAX == 0xffffffffffffffffuLL
  #define NA_TYPE_NATIVE_INT_BITS NA_TYPE64_BITS
#endif
#ifndef NA_TYPE_NATIVE_INT_BITS
  #warning "NALib will make assumptions about the native size of an int."
  #define NA_TYPE_NATIVE_INT_BITS NA_TYPE32_BITS
#endif

// long
#if ULONG_MAX == 0xffffffffu
  #define NA_TYPE_NATIVE_LONG_INT_BITS NA_TYPE32_BITS
#elif ULONG_MAX == 0xffffffffffffffffuLL
  #define NA_TYPE_NATIVE_LONG_INT_BITS NA_TYPE64_BITS
#endif
#ifndef NA_TYPE_NATIVE_LONG_INT_BITS
  #warning "NALib will make assumptions about the native size of a long."
  #define NA_TYPE_NATIVE_LONG_INT_BITS NA_TYPE32_BITS
#endif

// long long
#if ULLONG_MAX == 0xffffffffffffffffuLL
  #define NA_TYPE_NATIVE_LONG_LONG_INT_BITS NA_TYPE64_BITS
#endif
#ifndef NA_TYPE_NATIVE_LONG_LONG_INT_BITS
  #warning "NALib will make assumptions about the native size of a long long."
  #define NA_TYPE_NATIVE_LONG_LONG_INT_BITS NA_TYPE64_BITS
#endif



// Make a mapping towards which bit width corresponds to which native type.
// Doing so allows to detect, which type has a native representation and
// which ones need to be emulated.
//
// The author is perfectly aware that this is a little over the top, but it
// allows for a preference and helps for later PRI and SCN macros.

#define NA_TYPE_NATIVE_NONE           0
#define NA_TYPE_NATIVE_CHAR           1
#define NA_TYPE_NATIVE_SHORT_INT      2
#define NA_TYPE_NATIVE_INT            3
#define NA_TYPE_NATIVE_LONG_INT       4
#define NA_TYPE_NATIVE_LONG_LONG_INT  5
#define NA_TYPE_NATIVE_INT_128        6
#define NA_TYPE_NATIVE_INT_256        7

// 8 Bits: Should be char. Otherwise, we might have a "slight" problem.
#if NA_TYPE8_BITS == NA_TYPE_NATIVE_CHAR_BITS
  #define NA_TYPE_INT8_REPRESENTATION NA_TYPE_NATIVE_CHAR
#else
  #define NA_TYPE_INT8_REPRESENTATION NA_TYPE_NATIVE_NONE
#endif

// 16 Bits: Preference is first short, then int
#if NA_TYPE16_BITS == NA_TYPE_NATIVE_SHORT_INT_BITS
  #define NA_TYPE_INT16_REPRESENTATION NA_TYPE_NATIVE_SHORT_INT
#elif NA_TYPE16_BITS == NA_TYPE_NATIVE_INT_BITS
  #define NA_TYPE_INT16_REPRESENTATION NA_TYPE_NATIVE_INT
#else
  #define NA_TYPE_INT16_REPRESENTATION NA_TYPE_NATIVE_NONE
#endif

// 32 Bits: Preference is first int, then long, then short
#if NA_TYPE32_BITS == NA_TYPE_NATIVE_INT_BITS
  #define NA_TYPE_INT32_REPRESENTATION NA_TYPE_NATIVE_INT
#elif NA_TYPE32_BITS == NA_TYPE_NATIVE_LONG_INT_BITS
  #define NA_TYPE_INT32_REPRESENTATION NA_TYPE_NATIVE_LONG_INT
#elif NA_TYPE32_BITS == NA_TYPE_NATIVE_SHORT_INT_BITS
  #define NA_TYPE_INT32_REPRESENTATION NA_TYPE_NATIVE_SHORT_INT
#else
  #define NA_TYPE_INT32_REPRESENTATION NA_TYPE_NATIVE_NONE
#endif

// 64 Bits: Preference is first long long, then long, then int
// Special case for MSVC compiler 6.0: Use __int64
#if NA_TYPE64_BITS == NA_TYPE_NATIVE_LONG_LONG_INT_BITS
  #define NA_TYPE_INT64_REPRESENTATION NA_TYPE_NATIVE_LONG_LONG_INT
#elif NA_TYPE64_BITS == NA_TYPE_NATIVE_LONG_INT_BITS
  #define NA_TYPE_INT64_REPRESENTATION NA_TYPE_NATIVE_LONG_INT
#elif NA_TYPE64_BITS == NA_TYPE_NATIVE_INT_BITS
  #define NA_TYPE_INT64_REPRESENTATION NA_TYPE_NATIVE_INT
#else
  #define NA_TYPE_INT64_REPRESENTATION NA_TYPE_NATIVE_NONE
#endif

// 128 Bits: Dependent on the system/compiler/extension.
#ifdef __SIZEOF_INT128__
  #define NA_TYPE_INT128_REPRESENTATION NA_TYPE_NATIVE_INT_128
#else
  #define NA_TYPE_INT128_REPRESENTATION NA_TYPE_NATIVE_NONE
#endif

// 256 Bits: Dependent on the system/compiler/extension.
#ifdef __SIZEOF_INT256__
  #define NA_TYPE_INT256_REPRESENTATION NA_TYPE_NATIVE_INT_256
#else
  #define NA_TYPE_INT256_REPRESENTATION NA_TYPE_NATIVE_NONE
#endif



// Depending on the representations gathered, find out, which types have a
// native representation and define integer type macros for them.
// If these macros are not defined, no native type is available.
#if NA_TYPE_INT8_REPRESENTATION != NA_TYPE_NATIVE_NONE
  #define NA_TYPE_INT8
#endif
#if NA_TYPE_INT16_REPRESENTATION != NA_TYPE_NATIVE_NONE
  #define NA_TYPE_INT16
#endif
#if NA_TYPE_INT32_REPRESENTATION != NA_TYPE_NATIVE_NONE
  #define NA_TYPE_INT32
#endif
#if NA_TYPE_INT64_REPRESENTATION != NA_TYPE_NATIVE_NONE
  #define NA_TYPE_INT64
#endif
#if NA_TYPE_INT128_REPRESENTATION != NA_TYPE_NATIVE_NONE
  #define NA_TYPE_INT128
#endif
#if NA_TYPE_INT256_REPRESENTATION != NA_TYPE_NATIVE_NONE
  #define NA_TYPE_INT256
#endif



#endif // NA_ENCODING_INCLUDED

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
