
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_INT_128_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_128_INCLUDED
#define NA_INT_128_INCLUDED


#if defined NA_TYPE_INT128

  #define naMakeu128WithLiteralLo(lo) (lo ## uLL)

  // We declare these as inline functions to reduce warnings.
  NA_IAPI NAi128 naMakei128(NAi64 hi, NAu64 lo);
  NA_IAPI NAi128 naMakei128WithLo       (NAi64 lo);
  NA_IAPI NAi128 naMakei128WithDouble   (double lo);

  #define naNegi128(i)                 (-(i))
  #define naInci128(i)                 (++(i))
  #define naDeci128(i)                 (--(i))
  #define naAddi128(a, b)              ((a) + (b))
  #define naSubi128(a, b)              ((a) - (b))
  #define naMuli128(a, b)              ((a) * (b))
  #define naDivi128(a, b)              ((a) / (b))
  #define naModi128(a, b)              ((a) % (b))

  #define naNoti128(i)                 (~(i))
  #define naOri128(a, b)               ((a) | (b))
  #define naAndi128(a, b)              ((a) & (b))
  #define naXori128(a, b)              ((a) ^ (b))
  #define naShli128(a, n)              ((a) << (n))
  #define naShri128(a, n)              ((a) >> (n))

  #define naEquali128(a, b)            ((a) == (b))
  #define naGreateri128(a, b)          ((a) > (b))
  #define naGreaterEquali128(a, b)     ((a) >= (b))
  #define naSmalleri128(a, b)          ((a) < (b))
  #define naSmallerEquali128(a, b)     ((a) <= (b))

  #define naCasti128Tou8(i)         ((uint8)(i))
  #define naCasti128Tou16(i)        ((uint16)(i))
  #define naCasti128Tou32(i)        ((uint32)(i))
  #define naCasti128Tou64(i)        ((NAu64)(i))
  #define naCasti128Tou128(i)       ((NAu128)(i))
  #define naCasti128Toi8(i)          ((int8)(i))
  #define naCasti128Toi16(i)         ((int16)(i))
  #define naCasti128Toi32(i)         ((int32)(i))
  #define naCasti128Toi64(i)         ((NAi64)(i))
  #define naCasti128ToDouble(i)        ((double)(i))

  #define naMakeu128(hi, lo)          ((NAu128)(((NAu128)(hi) << 64) | (lo)))
  #define naMakeu128WithLo(lo)        ((NAu128)(lo))
  #define naMakeu128WithDouble(d)     ((NAu128)(d))

  #define naIncu128(i)                ((i)++)
  #define naDecu128(i)                ((i)--)
  #define naAddu128(a, b)             ((a) + (b))
  #define naSubu128(a, b)             ((a) - (b))
  #define naMulu128(a, b)             ((a) * (b))
  #define naDivu128(a, b)             ((a) / (b))
  #define naModu128(a, b)             ((a) % (b))

  #define naNotu128(i)                (~(i))
  #define naOru128(a, b)              ((a) | (b))
  #define naAndu128(a, b)             ((a) & (b))
  #define naXoru128(a, b)             ((a) ^ (b))
  #define naShlu128(a, n)             ((a) << (n))
  #define naShru128(a, n)             ((a) >> (n))

  #define naEqualu128(a, b)           ((a) == (b))
  #define naGreateru128(a, b)         ((a) > (b))
  #define naGreaterEqualu128(a, b)    ((a) >= (b))
  #define naSmalleru128(a, b)         ((a) < (b))
  #define naSmallerEqualu128(a, b)    ((a) <= (b))

  #define naCastu128Toi8(i)         ((int8)(i))
  #define naCastu128Toi16(i)        ((int16)(i))
  #define naCastu128Toi32(i)        ((int32)(i))
  #define naCastu128Toi64(i)        ((NAi64)(i))
  #define naCastu128Toi128(i)       ((NAi128)(i))
  #define naCastu128Tou8(i)        ((uint8)(i))
  #define naCastu128Tou16(i)       ((uint16)(i))
  #define naCastu128Tou32(i)       ((uint32)(i))
  #define naCastu128Tou64(i)       ((NAu64)(i))
  #define naCastu128ToDouble(i)       ((double)(i))

#else
  // if no native int128 type is available, we have to emulate it.

  #if NA_SIGN_ENCODING != NA_SIGN_ENCODING_TWOS_COMPLEMENT

    #error "NAi128 is unavailable for the current signed integer encoding"

  #endif

  NA_IAPI NAi128  naMakei128(NAi64 hi, NAu64 lo);
  NA_IAPI NAi128  naMakei128WithLo(NAi64 lo);
  NA_IAPI NAi128  naMakei128WithDouble(double d);

  NA_IAPI NAi128  naNegi128(NAi128 i);
  #define           naInci128(i)
  #define           naDeci128(i)
  NA_IAPI NAi128  naAddi128(NAi128 a, NAi128 b);
  NA_IAPI NAi128  naSubi128(NAi128 a, NAi128 b);
  NA_IAPI NAi128  naMuli128(NAi128 a, NAi128 b);
  NA_IAPI NAi128  naDivi128(NAi128 a, NAi128 b);
  NA_IAPI NAi128  naModi128(NAi128 a, NAi128 b);

  NA_IAPI NAi128  naNoti128(NAi128 i);
  NA_IAPI NAi128  naOri128 (NAi128 a, NAi128 b);
  NA_IAPI NAi128  naAndi128(NAi128 a, NAi128 b);
  NA_IAPI NAi128  naXori128(NAi128 a, NAi128 b);
  NA_IAPI NAi128  naShli128(NAi128 a, int n);
  NA_IAPI NAi128  naShri128(NAi128 a, int n);

  NA_IAPI NABool naEquali128        (NAi128 a, NAi128 b);
  NA_IAPI NABool naGreateri128      (NAi128 a, NAi128 b);
  NA_IAPI NABool naGreaterEquali128 (NAi128 a, NAi128 b);
  NA_IAPI NABool naSmalleri128      (NAi128 a, NAi128 b);
  NA_IAPI NABool naSmallerEquali128 (NAi128 a, NAi128 b);

  NA_IAPI uint8     naCasti128Tou8  (NAi128 i);
  NA_IAPI uint16    naCasti128Tou16 (NAi128 i);
  NA_IAPI uint32    naCasti128Tou32 (NAi128 i);
  NA_IAPI NAu64  naCasti128Tou64 (NAi128 i);
  NA_IAPI NAu128 naCasti128Tou128(NAi128 i);
  NA_IAPI int8      naCasti128Toi8   (NAi128 i);
  NA_IAPI int16     naCasti128Toi16  (NAi128 i);
  NA_IAPI int32     naCasti128Toi32  (NAi128 i);
  NA_IAPI NAi64   naCasti128Toi64  (NAi128 i);
  NA_IAPI double    naCasti128ToDouble (NAi128 i);

  #define naMakeu128WithLiteralLo(lo)
  NA_IAPI NAu128 naMakeu128(NAu64 hi, NAu64 lo);
  NA_IAPI NAu128 naMakeu128WithLo(NAu64 lo);
  NA_IAPI NAu128 naMakeu128WithDouble(double d);

  #define           naIncu128(i)
  #define           naDecu128(i)
  NA_IAPI NAu128 naAddu128(NAu128 a, NAu128 b);
  NA_IAPI NAu128 naAddu128(NAu128 a, NAu128 b);
  NA_IAPI NAu128 naSubu128(NAu128 a, NAu128 b);
  NA_IAPI NAu128 naMulu128(NAu128 a, NAu128 b);
  NA_IAPI NAu128 naDivu128(NAu128 a, NAu128 b);
  NA_IAPI NAu128 naModu128(NAu128 a, NAu128 b);

  NA_IAPI NAu128 naNotu128(NAu128 i);
  NA_IAPI NAu128 naOru128 (NAu128 a, NAu128 b);
  NA_IAPI NAu128 naAndu128(NAu128 a, NAu128 b);
  NA_IAPI NAu128 naXoru128(NAu128 a, NAu128 b);
  NA_IAPI NAu128 naShlu128(NAu128 a, int n);
  NA_IAPI NAu128 naShru128(NAu128 a, int n);

  NA_IAPI NABool naEqualu128       (NAu128 a, NAu128 b);
  NA_IAPI NABool naGreateru128     (NAu128 a, NAu128 b);
  NA_IAPI NABool naGreaterEqualu128(NAu128 a, NAu128 b);
  NA_IAPI NABool naSmalleru128     (NAu128 a, NAu128 b);
  NA_IAPI NABool naSmallerEqualu128(NAu128 a, NAu128 b);

  NA_IAPI int8     naCastu128Toi8   (NAu128 i);
  NA_IAPI int16    naCastu128Toi16  (NAu128 i);
  NA_IAPI int32    naCastu128Toi32  (NAu128 i);
  NA_IAPI NAi64  naCastu128Toi64  (NAu128 i);
  NA_IAPI NAi128 naCastu128Toi128 (NAu128 i);
  NA_IAPI uint8    naCastu128Tou8  (NAu128 i);
  NA_IAPI uint16   naCastu128Tou16 (NAu128 i);
  NA_IAPI uint32   naCastu128Tou32 (NAu128 i);
  NA_IAPI NAu64 naCastu128Tou64 (NAu128 i);
  NA_IAPI double   naCastu128ToDouble (NAu128 i);

#endif


#if NA_TYPE_NAINT_BITS == NA_TYPE64_BITS
  #define naCastu128ToUInt(i)   naCastu128Tou64(i)
  #define naCasti128ToInt(i)     naCasti128Toi64(i)
  #define naCastIntToi128(i)     naMakei128WithLo(i)
  #define naCastUIntTou128(i)   naMakeu128WithLo(i)
#elif NA_TYPE_NAINT_BITS == NA_TYPE32_BITS
  #define naCastu128ToUInt(i)   naCastu128Tou32(i)
  #define naCasti128ToInt(i)     naCasti128Toi32(i)
  #define naCastIntToi128(i)     naMakei128WithLo(naMakei64WithLo(i))
  #define naCastUIntTou128(i)   naMakeu128WithLo(naMakeu64WithLo(i))
#elif NA_TYPE_NAINT_BITS == NA_TYPE16_BITS
  #define naCastu128ToUInt(i)   naCastu128Tou16(i)
  #define naCasti128ToInt(i)     naCasti128Toi16(i)
  #define naCastIntToi128(i)     naMakei128WithLo(naMakei64WithLo((int32)i))
  #define naCastUIntTou128(i)   naMakeu128WithLo(naMakeu64WithLo((uint32)i))
#endif


#endif // NA_INT_128_INCLUDED

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
