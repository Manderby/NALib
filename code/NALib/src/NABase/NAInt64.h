
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NAInteger.h"


#if defined NA_TYPE_INT64
  // If there is a native int64 type available, use it.

  #define NAInt64 int64
  #define NAUInt64 uint64

  #define naMakeUInt64WithLiteralLo(lo) (lo ## uLL)

  // We declare these as inline functions to reduce warnings.
  NA_IAPI NAInt64 naMakeInt64             (int32 hi, uint32 lo);
  NA_IAPI NAInt64 naMakeInt64WithLo       (int32 lo);
  NA_IAPI NAInt64 naMakeInt64WithDouble   (double lo);

  #define naNegInt64(i)                 (-(i))
  #define naIncInt64(i)                 (++(i))
  #define naDecInt64(i)                 (--(i))
  #define naAddInt64(a, b)              ((a) + (b))
  #define naSubInt64(a, b)              ((a) - (b))
  #define naMulInt64(a, b)              ((a) * (b))
  #define naDivInt64(a, b)              ((a) / (b))
  #define naModInt64(a, b)              ((a) % (b))

  #define naNotInt64(i)                 (~(i))
  #define naOrInt64(a, b)               ((a) | (b))
  #define naAndInt64(a, b)              ((a) & (b))
  #define naXorInt64(a, b)              ((a) ^ (b))
  #define naShlInt64(a, n)              ((a) << (n))
  #define naShrInt64(a, n)              ((a) >> (n))

  #define naEqualInt64(a, b)            ((a) == (b))
  #define naGreaterInt64(a, b)          ((a) > (b))
  #define naGreaterEqualInt64(a, b)     ((a) >= (b))
  #define naSmallerInt64(a, b)          ((a) < (b))
  #define naSmallerEqualInt64(a, b)     ((a) <= (b))

  #define naCastInt64ToUInt8(i)         ((uint8)(i))
  #define naCastInt64ToUInt16(i)        ((uint16)(i))
  #define naCastInt64ToUInt32(i)        ((uint32)(i))
  #define naCastInt64ToUInt64(i)        ((NAUInt64)(i))
  #define naCastInt64ToInt8(i)          ((int8)(i))
  #define naCastInt64ToInt16(i)         ((int16)(i))
  #define naCastInt64ToInt32(i)         ((int32)(i))
  #define naCastInt64ToDouble(i)        ((double)(i))

  #define naMakeUInt64(hi, lo)          ((NAUInt64)(((NAUInt64)(hi) << 32) | (lo)))
  #define naMakeUInt64WithLo(lo)        ((NAUInt64)(lo))
  #define naMakeUInt64WithDouble(d)     ((NAUInt64)(d))

  #define naIncUInt64(i)                ((i)++)
  #define naDecUInt64(i)                ((i)--)
  #define naAddUInt64(a, b)             ((a) + (b))
  #define naSubUInt64(a, b)             ((a) - (b))
  #define naMulUInt64(a, b)             ((a) * (b))
  #define naDivUInt64(a, b)             ((a) / (b))
  #define naModUInt64(a, b)             ((a) % (b))

  #define naNotUInt64(i)                (~(i))
  #define naOrUInt64(a, b)              ((a) | (b))
  #define naAndUInt64(a, b)             ((a) & (b))
  #define naXorUInt64(a, b)             ((a) ^ (b))
  #define naShlUInt64(a, n)             ((a) << (n))
  #define naShrUInt64(a, n)             ((a) >> (n))

  #define naEqualUInt64(a, b)           ((a) == (b))
  #define naGreaterUInt64(a, b)         ((a) > (b))
  #define naGreaterEqualUInt64(a, b)    ((a) >= (b))
  #define naSmallerUInt64(a, b)         ((a) < (b))
  #define naSmallerEqualUInt64(a, b)    ((a) <= (b))

  #define naCastUInt64ToInt8(i)         ((int8)(i))
  #define naCastUInt64ToInt16(i)        ((int16)(i))
  #define naCastUInt64ToInt32(i)        ((int32)(i))
  #define naCastUInt64ToInt64(i)        ((NAInt64)(i))
  #define naCastUInt64ToUInt8(i)        ((uint8)(i))
  #define naCastUInt64ToUInt16(i)       ((uint16)(i))
  #define naCastUInt64ToUInt32(i)       ((uint32)(i))
  #define naCastUInt64ToDouble(i)       ((double)(i))

#else
  // if no native int64 type is available, we have to emulate it.

  #if NA_SIGNED_INTEGER_ENCODING != NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT

    #error "The NAInt64 emulation is not provided for the current signed integer encoding"

  #else

    typedef struct NAInt64  NAInt64;
    typedef struct NAUInt64 NAUInt64;
    #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
      struct NAInt64 { int32 hi;  uint32 lo; };
      struct NAUInt64{ uint32 hi; uint32 lo; };
    #else
      struct NAInt64 { uint32 lo; int32  hi; };
      struct NAUInt64{ uint32 lo; uint32 hi; };
    #endif

    NA_IAPI NAInt64  naMakeInt64(int32 hi, uint32 lo);
    NA_IAPI NAInt64  naMakeInt64WithLo(int32 lo);
    NA_IAPI NAInt64  naMakeInt64WithDouble(double d);

    NA_IAPI NAInt64  naNegInt64(NAInt64 i);
    #define          naIncInt64(i)
    #define          naDecInt64(i)
    NA_IAPI NAInt64  naAddInt64(NAInt64 a, NAInt64 b);
    NA_IAPI NAInt64  naSubInt64(NAInt64 a, NAInt64 b);
    NA_IAPI NAInt64  naMulInt64(NAInt64 a, NAInt64 b);
    NA_IAPI NAInt64  naDivInt64(NAInt64 a, NAInt64 b);
    NA_IAPI NAInt64  naModInt64(NAInt64 a, NAInt64 b);

    NA_IAPI NAInt64  naNotInt64(NAInt64 i);
    NA_IAPI NAInt64  naOrInt64 (NAInt64 a, NAInt64 b);
    NA_IAPI NAInt64  naAndInt64(NAInt64 a, NAInt64 b);
    NA_IAPI NAInt64  naXorInt64(NAInt64 a, NAInt64 b);
    NA_IAPI NAInt64  naShlInt64(NAInt64 a, int n);
    NA_IAPI NAInt64  naShrInt64(NAInt64 a, int n);

    NA_IAPI NABool naEqualInt64        (NAInt64 a, NAInt64 b);
    NA_IAPI NABool naGreaterInt64      (NAInt64 a, NAInt64 b);
    NA_IAPI NABool naGreaterEqualInt64 (NAInt64 a, NAInt64 b);
    NA_IAPI NABool naSmallerInt64      (NAInt64 a, NAInt64 b);
    NA_IAPI NABool naSmallerEqualInt64 (NAInt64 a, NAInt64 b);

    NA_IAPI uint8    naCastInt64ToUInt8 (NAInt64 i);
    NA_IAPI uint16   naCastInt64ToUInt16(NAInt64 i);
    NA_IAPI uint32   naCastInt64ToUInt32(NAInt64 i);
    NA_IAPI NAUInt64 naCastInt64ToUInt64(NAInt64 i);
    NA_IAPI int8     naCastInt64ToInt8  (NAInt64 i);
    NA_IAPI int16    naCastInt64ToInt16 (NAInt64 i);
    NA_IAPI int32    naCastInt64ToInt32 (NAInt64 i);
    NA_IAPI double   naCastInt64ToDouble(NAInt64 i);

    #define naMakeUInt64WithLiteralLo(lo)
    NA_IAPI NAUInt64 naMakeUInt64(uint32 hi, uint32 lo);
    NA_IAPI NAUInt64 naMakeUInt64WithLo(uint32 lo);
    NA_IAPI NAUInt64 naMakeUInt64WithDouble(double d);

    #define          naIncUInt64(i)
    #define          naDecUInt64(i)
    NA_IAPI NAUInt64 naAddUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naAddUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naSubUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naMulUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naDivUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naModUInt64(NAUInt64 a, NAUInt64 b);

    NA_IAPI NAUInt64 naNotUInt64(NAUInt64 i);
    NA_IAPI NAUInt64 naOrUInt64 (NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naAndUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naXorUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NAUInt64 naShlUInt64(NAUInt64 a, int n);
    NA_IAPI NAUInt64 naShrUInt64(NAUInt64 a, int n);

    NA_IAPI NABool naEqualUInt64       (NAUInt64 a, NAUInt64 b);
    NA_IAPI NABool naGreaterUInt64     (NAUInt64 a, NAUInt64 b);
    NA_IAPI NABool naGreaterEqualUInt64(NAUInt64 a, NAUInt64 b);
    NA_IAPI NABool naSmallerUInt64     (NAUInt64 a, NAUInt64 b);
    NA_IAPI NABool naSmallerEqualUInt64(NAUInt64 a, NAUInt64 b);

    NA_IAPI int8    naCastUInt64ToInt8  (NAUInt64 i);
    NA_IAPI int16   naCastUInt64ToInt16 (NAUInt64 i);
    NA_IAPI int32   naCastUInt64ToInt32 (NAUInt64 i);
    NA_IAPI NAInt64 naCastUInt64ToInt64 (NAUInt64 i);
    NA_IAPI uint8   naCastUInt64ToUInt8 (NAUInt64 i);
    NA_IAPI uint16  naCastUInt64ToUInt16(NAUInt64 i);
    NA_IAPI uint32  naCastUInt64ToUInt32(NAUInt64 i);
    NA_IAPI double  naCastUInt64ToDouble(NAUInt64 i);


  #endif

#endif


#if NA_TYPE_NAINT_BITS == NA_TYPE64_BITS
  #define naCastUInt64ToUInt(i)   ((NAUInt64)i)
  #define naCastInt64ToInt(i)     ((NAInt64)i)
  #define naCastIntToInt64(i)     ((NAInt64)i)
  #define naCastUIntToUInt64(i)   ((NAUInt64)i)
#elif NA_TYPE_NAINT_BITS == NA_TYPE32_BITS
  #define naCastUInt64ToUInt(i)   naCastUInt64ToUInt32(i)
  #define naCastInt64ToInt(i)     naCastInt64ToInt32(i)
  #define naCastIntToInt64(i)     naMakeInt64WithLo(i)
  #define naCastUIntToUInt64(i)   naMakeUInt64WithLo(i)
#elif NA_TYPE_NAINT_BITS == NA_TYPE16_BITS
  #define naCastUInt64ToUInt(i)   naCastUInt64ToUInt16(i)
  #define naCastInt64ToInt(i)     naCastInt64ToInt16(i)
  #define naCastIntToInt64(i)     naMakeInt64WithLo(int32(i))
  #define naCastUIntToUInt64(i)   naMakeUInt64WithLo(int32(i))
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
