
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

/*

// Do not include this file directly! It will automatically be included when
// including "NAInteger.h"


#if NA_SIGNED_INTEGER_ENCODING != NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT

  #error "The NAInt128 emulation is not provided for the current signed integer encoding"

#else

  typedef struct NAInt128  NAInt128;
  typedef struct NAUInt128 NAUInt128;
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    struct NAInt128 { NAInt64 hi;  NAUInt64 lo; };
    struct NAUInt128{ NAUInt64 hi; NAUInt64 lo; };
  #else
    struct NAInt128 { NAUInt64 lo; NAInt64  hi; };
    struct NAUInt128{ NAUInt64 lo; NAUInt64 hi; };
  #endif

  NA_IAPI NAInt128  naMakeInt128WithLo(NAInt64 lo);
  NA_IAPI NAInt128  naMakeInt128WithDouble(double d);

  NA_IAPI NAInt128  naNegInt128(NAInt128 i);
  #define           naIncInt128(i)
  #define           naDecInt128(i)
  NA_IAPI NAInt128  naAddInt128(NAInt128 a, NAInt128 b);
  NA_IAPI NAInt128  naSubInt128(NAInt128 a, NAInt128 b);
  NA_IAPI NAInt128  naMulInt128(NAInt128 a, NAInt128 b);
  NA_IAPI NAInt128  naDivInt128(NAInt128 a, NAInt128 b);
  NA_IAPI NAInt128  naModInt128(NAInt128 a, NAInt128 b);

  NA_IAPI NAInt128  naNotInt128(NAInt128 i);
  NA_IAPI NAInt128  naOrInt128 (NAInt128 a, NAInt128 b);
  NA_IAPI NAInt128  naAndInt128(NAInt128 a, NAInt128 b);
  NA_IAPI NAInt128  naXorInt128(NAInt128 a, NAInt128 b);
  NA_IAPI NAInt128  naShlInt128(NAInt128 a, int n);
  NA_IAPI NAInt128  naShrInt128(NAInt128 a, int n);

  NA_IAPI NABool naEqualInt128        (NAInt128 a, NAInt128 b);
  NA_IAPI NABool naGreaterInt128      (NAInt128 a, NAInt128 b);
  NA_IAPI NABool naGreaterEqualInt128 (NAInt128 a, NAInt128 b);
  NA_IAPI NABool naSmallerInt128      (NAInt128 a, NAInt128 b);
  NA_IAPI NABool naSmallerEqualInt128 (NAInt128 a, NAInt128 b);

  NA_IAPI uint8    naCastInt128ToUInt8 (NAInt128 i);
  NA_IAPI uint16   naCastInt128ToUInt16(NAInt128 i);
  NA_IAPI uint32   naCastInt128ToUInt32(NAInt128 i);
  NA_IAPI NAUInt64 naCastInt128ToUInt64(NAInt128 i);
  NA_IAPI int8     naCastInt128ToInt8  (NAInt128 i);
  NA_IAPI int16    naCastInt128ToInt16 (NAInt128 i);
  NA_IAPI int32    naCastInt128ToInt32 (NAInt128 i);
  NA_IAPI NAInt64  naCastInt128ToInt64 (NAInt128 i);
  NA_IAPI double   naCastInt128ToDouble(NAInt128 i);

  #define naMakeUInt128WithLiteralLo(lo)
  NA_IAPI NAUInt128 naMakeUInt128(NAUInt64 hi, NAUInt64 lo);
  NA_IAPI NAUInt128 naMakeUInt128WithLo(NAUInt64 lo);
  NA_IAPI NAUInt128 naMakeUInt128WithDouble(double d);

  #define           naIncUInt128(i)
  #define           naDecUInt128(i)
  NA_IAPI NAUInt128 naAddUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naAddUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naSubUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naMulUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naDivUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naModUInt128(NAUInt128 a, NAUInt128 b);

  NA_IAPI NAUInt128 naNotUInt128(NAUInt128 i);
  NA_IAPI NAUInt128 naOrUInt128 (NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naAndUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naXorUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NAUInt128 naShlUInt128(NAUInt128 a, int n);
  NA_IAPI NAUInt128 naShrUInt128(NAUInt128 a, int n);

  NA_IAPI NABool naEqualUInt128       (NAUInt128 a, NAUInt128 b);
  NA_IAPI NABool naGreaterUInt128     (NAUInt128 a, NAUInt128 b);
  NA_IAPI NABool naGreaterEqualUInt128(NAUInt128 a, NAUInt128 b);
  NA_IAPI NABool naSmallerUInt128     (NAUInt128 a, NAUInt128 b);
  NA_IAPI NABool naSmallerEqualUInt128(NAUInt128 a, NAUInt128 b);

  NA_IAPI int8     naCastUInt128ToInt8  (NAUInt128 i);
  NA_IAPI int16    naCastUInt128ToInt16 (NAUInt128 i);
  NA_IAPI int32    naCastUInt128ToInt32 (NAUInt128 i);
  NA_IAPI NAInt128 naCastUInt128ToInt128 (NAUInt128 i);
  NA_IAPI uint8    naCastUInt128ToUInt8 (NAUInt128 i);
  NA_IAPI uint16   naCastUInt128ToUInt16(NAUInt128 i);
  NA_IAPI uint32   naCastUInt128ToUInt32(NAUInt128 i);
  NA_IAPI NAUInt64 naCastUInt128ToUInt64(NAUInt128 i);
  NA_IAPI double   naCastUInt128ToDouble(NAUInt128 i);


#endif
*/

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
