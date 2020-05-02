
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_INT_256_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_256_INCLUDED
#define NA_INT_256_INCLUDED


#if defined NA_TYPE_INT256
  // If there is a native int256 type available, use it.

  #define NAInt256 int256
  #define NAUInt256 uint256

  #define naMakeUInt256WithLiteralLo(lo) (lo ## uLL)

  // We declare these as inline functions to reduce warnings.
  NA_IAPI NAInt256 naMakeInt256(NAInt128 hi, NAUInt128 lo);
  NA_IAPI NAInt256 naMakeInt256WithLo       (NAInt128 lo);
  NA_IAPI NAInt256 naMakeInt256WithDouble   (double lo);

  #define naNegInt256(i)              (-(i))
  #define naIncInt256(i)              (++(i))
  #define naDecInt256(i)              (--(i))
  #define naAddInt256(a, b)           ((a) + (b))
  #define naSubInt256(a, b)           ((a) - (b))
  #define naMulInt256(a, b)           ((a) * (b))
  #define naDivInt256(a, b)           ((a) / (b))
  #define naModInt256(a, b)           ((a) % (b))

  #define naNotInt256(i)              (~(i))
  #define naOrInt256(a, b)            ((a) | (b))
  #define naAndInt256(a, b)           ((a) & (b))
  #define naXorInt256(a, b)           ((a) ^ (b))
  #define naShlInt256(a, n)           ((a) << (n))
  #define naShrInt256(a, n)           ((a) >> (n))

  #define naEqualInt256(a, b)         ((a) == (b))
  #define naGreaterInt256(a, b)       ((a) > (b))
  #define naGreaterEqualInt256(a, b)  ((a) >= (b))
  #define naSmallerInt256(a, b)       ((a) < (b))
  #define naSmallerEqualInt256(a, b)  ((a) <= (b))

  #define naCastInt256ToUInt8(i)      ((uint8)(i))
  #define naCastInt256ToUInt16(i)     ((uint16)(i))
  #define naCastInt256ToUInt32(i)     ((uint32)(i))
  #define naCastInt256ToUInt64(i)     ((NAUInt64)(i))
  #define naCastInt256ToUInt128(i)    ((NAUInt128)(i))
  #define naCastInt256ToUInt256(i)    ((NAUInt256)(i))
  #define naCastInt256ToInt8(i)       ((int8)(i))
  #define naCastInt256ToInt16(i)      ((int16)(i))
  #define naCastInt256ToInt32(i)      ((int32)(i))
  #define naCastInt256ToInt64(i)      ((NAInt64)(i))
  #define naCastInt256ToInt128(i)     ((NAInt128)(i))
  #define naCastInt256ToDouble(i)     ((double)(i))

  #define naMakeUInt256(hi, lo)       ((NAUInt256)(((NAUInt256)(hi) << 128) | (lo)))
  #define naMakeUInt256WithLo(lo)     ((NAUInt256)(lo))
  #define naMakeUInt256WithDouble(d)  ((NAUInt256)(d))

  #define naIncUInt256(i)             ((i)++)
  #define naDecUInt256(i)             ((i)--)
  #define naAddUInt256(a, b)          ((a) + (b))
  #define naSubUInt256(a, b)          ((a) - (b))
  #define naMulUInt256(a, b)          ((a) * (b))
  #define naDivUInt256(a, b)          ((a) / (b))
  #define naModUInt256(a, b)          ((a) % (b))

  #define naNotUInt256(i)             (~(i))
  #define naOrUInt256(a, b)           ((a) | (b))
  #define naAndUInt256(a, b)          ((a) & (b))
  #define naXorUInt256(a, b)          ((a) ^ (b))
  #define naShlUInt256(a, n)          ((a) << (n))
  #define naShrUInt256(a, n)          ((a) >> (n))

  #define naEqualUInt256(a, b)        ((a) == (b))
  #define naGreaterUInt256(a, b)      ((a) > (b))
  #define naGreaterEqualUInt256(a, b) ((a) >= (b))
  #define naSmallerUInt256(a, b)      ((a) < (b))
  #define naSmallerEqualUInt256(a, b) ((a) <= (b))

  #define naCastUInt256ToInt8(i)      ((int8)(i))
  #define naCastUInt256ToInt16(i)     ((int16)(i))
  #define naCastUInt256ToInt32(i)     ((int32)(i))
  #define naCastUInt256ToInt64(i)     ((NAInt64)(i))
  #define naCastUInt256ToInt128(i)    ((NAInt128)(i))
  #define naCastUInt256ToInt256(i)    ((NAInt256)(i))
  #define naCastUInt256ToUInt8(i)     ((uint8)(i))
  #define naCastUInt256ToUInt16(i)    ((uint16)(i))
  #define naCastUInt256ToUInt32(i)    ((uint32)(i))
  #define naCastUInt256ToUInt64(i)    ((NAUInt64)(i))
  #define naCastUInt256ToUInt128(i)   ((NAUInt128)(i))
  #define naCastUInt256ToDouble(i)    ((double)(i))

#else
  // if no native int256 type is available, we have to emulate it.

  #if NA_SIGN_ENCODING != NA_SIGN_ENCODING_TWOS_COMPLEMENT

    #error "NAInt256 is unavailable for the current signed integer encoding"

  #else

    typedef struct NAInt256  NAInt256;
    typedef struct NAUInt256 NAUInt256;
    #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
      struct NAInt256 { NAInt128 hi;  NAUInt128 lo; };
      struct NAUInt256{ NAUInt128 hi; NAUInt128 lo; };
    #else
      struct NAInt256 { NAUInt128 lo; NAInt128  hi; };
      struct NAUInt256{ NAUInt128 lo; NAUInt128 hi; };
    #endif

    NA_IAPI NAInt256  naMakeInt256(NAInt128 hi, NAUInt128 lo);
    NA_IAPI NAInt256  naMakeInt256WithLo(NAInt128 lo);
    NA_IAPI NAInt256  naMakeInt256WithDouble(double d);

    NA_IAPI NAInt256  naNegInt256(NAInt256 i);
    #define           naIncInt256(i)
    #define           naDecInt256(i)
    NA_IAPI NAInt256  naAddInt256(NAInt256 a, NAInt256 b);
    NA_IAPI NAInt256  naSubInt256(NAInt256 a, NAInt256 b);
    NA_IAPI NAInt256  naMulInt256(NAInt256 a, NAInt256 b);
    NA_IAPI NAInt256  naDivInt256(NAInt256 a, NAInt256 b);
    NA_IAPI NAInt256  naModInt256(NAInt256 a, NAInt256 b);

    NA_IAPI NAInt256  naNotInt256(NAInt256 i);
    NA_IAPI NAInt256  naOrInt256 (NAInt256 a, NAInt256 b);
    NA_IAPI NAInt256  naAndInt256(NAInt256 a, NAInt256 b);
    NA_IAPI NAInt256  naXorInt256(NAInt256 a, NAInt256 b);
    NA_IAPI NAInt256  naShlInt256(NAInt256 a, int n);
    NA_IAPI NAInt256  naShrInt256(NAInt256 a, int n);

    NA_IAPI NABool naEqualInt256        (NAInt256 a, NAInt256 b);
    NA_IAPI NABool naGreaterInt256      (NAInt256 a, NAInt256 b);
    NA_IAPI NABool naGreaterEqualInt256 (NAInt256 a, NAInt256 b);
    NA_IAPI NABool naSmallerInt256      (NAInt256 a, NAInt256 b);
    NA_IAPI NABool naSmallerEqualInt256 (NAInt256 a, NAInt256 b);

    NA_IAPI uint8     naCastInt256ToUInt8  (NAInt256 i);
    NA_IAPI uint16    naCastInt256ToUInt16 (NAInt256 i);
    NA_IAPI uint32    naCastInt256ToUInt32 (NAInt256 i);
    NA_IAPI NAUInt64  naCastInt256ToUInt64 (NAInt256 i);
    NA_IAPI NAUInt128 naCastInt256ToUInt128(NAInt256 i);
    NA_IAPI NAUInt256 naCastInt256ToUInt256(NAInt256 i);
    NA_IAPI int8      naCastInt256ToInt8   (NAInt256 i);
    NA_IAPI int16     naCastInt256ToInt16  (NAInt256 i);
    NA_IAPI int32     naCastInt256ToInt32  (NAInt256 i);
    NA_IAPI NAInt64   naCastInt256ToInt64  (NAInt256 i);
    NA_IAPI NAInt128  naCastInt256ToInt128 (NAInt256 i);
    NA_IAPI double    naCastInt256ToDouble (NAInt256 i);

    #define naMakeUInt256WithLiteralLo(lo)
    NA_IAPI NAUInt256 naMakeUInt256(NAUInt128 hi, NAUInt128 lo);
    NA_IAPI NAUInt256 naMakeUInt256WithLo(NAUInt128 lo);
    NA_IAPI NAUInt256 naMakeUInt256WithDouble(double d);

    #define           naIncUInt256(i)
    #define           naDecUInt256(i)
    NA_IAPI NAUInt256 naAddUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naAddUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naSubUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naMulUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naDivUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naModUInt256(NAUInt256 a, NAUInt256 b);

    NA_IAPI NAUInt256 naNotUInt256(NAUInt256 i);
    NA_IAPI NAUInt256 naOrUInt256 (NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naAndUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naXorUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NAUInt256 naShlUInt256(NAUInt256 a, int n);
    NA_IAPI NAUInt256 naShrUInt256(NAUInt256 a, int n);

    NA_IAPI NABool naEqualUInt256       (NAUInt256 a, NAUInt256 b);
    NA_IAPI NABool naGreaterUInt256     (NAUInt256 a, NAUInt256 b);
    NA_IAPI NABool naGreaterEqualUInt256(NAUInt256 a, NAUInt256 b);
    NA_IAPI NABool naSmallerUInt256     (NAUInt256 a, NAUInt256 b);
    NA_IAPI NABool naSmallerEqualUInt256(NAUInt256 a, NAUInt256 b);

    NA_IAPI int8      naCastUInt256ToInt8    (NAUInt256 i);
    NA_IAPI int16     naCastUInt256ToInt16   (NAUInt256 i);
    NA_IAPI int32     naCastUInt256ToInt32   (NAUInt256 i);
    NA_IAPI NAInt64   naCastUInt256ToInt64   (NAUInt256 i);
    NA_IAPI NAInt128  naCastUInt256ToInt128  (NAUInt256 i);
    NA_IAPI NAInt256  naCastUInt256ToInt256  (NAUInt256 i);
    NA_IAPI uint8     naCastUInt256ToUInt8   (NAUInt256 i);
    NA_IAPI uint16    naCastUInt256ToUInt16  (NAUInt256 i);
    NA_IAPI uint32    naCastUInt256ToUInt32  (NAUInt256 i);
    NA_IAPI NAUInt64  naCastUInt256ToUInt64  (NAUInt256 i);
    NA_IAPI NAUInt128 naCastUInt256ToUInt128 (NAUInt256 i);
    NA_IAPI double    naCastUInt256ToDouble  (NAUInt256 i);


  #endif

#endif


#if NA_TYPE_NAINT_BITS == NA_TYPE64_BITS
  #define naCastUInt256ToUInt(i)   naCastUInt256ToUInt64(i)
  #define naCastInt256ToInt(i)     naCastInt256ToInt64(i)
  #define naCastIntToInt256(i)     naMakeInt256WithLo(naMakeInt128WithLo(i))
  #define naCastUIntToUInt256(i)   naMakeUInt256WithLo(naMakeUInt128WithLo(i))
#elif NA_TYPE_NAINT_BITS == NA_TYPE32_BITS
  #define naCastUInt256ToUInt(i)   naCastUInt256ToUInt32(i)
  #define naCastInt256ToInt(i)     naCastInt256ToInt32(i)
  #define naCastIntToInt256(i)     naMakeInt256WithLo(naMakeInt128WithLo(naMakeInt64WithLo(i)))
  #define naCastUIntToUInt256(i)   naMakeInt256WithLo(naMakeUInt128WithLo(naMakeUInt64WithLo(i)))
#elif NA_TYPE_NAINT_BITS == NA_TYPE16_BITS
  #define naCastUInt256ToUInt(i)   naCastUInt256ToUInt16(i)
  #define naCastInt256ToInt(i)     naCastInt256ToInt16(i)
  #define naCastIntToInt256(i)     naMakeInt256WithLo(naMakeInt128WithLo(naMakeInt64WithLo((int32)i)))
  #define naCastUIntToUInt256(i)   naMakeInt256WithLo(naMakeUInt128WithLo(naMakeUInt64WithLo((uint32)i)))
#endif


#endif // NA_INT_256_INCLUDED

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
