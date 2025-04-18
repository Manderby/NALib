
#if defined NA_INT_256_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_256_INCLUDED
#define NA_INT_256_INCLUDED



#if NA_NATIVE_INT256_IN_USE

  // We declare these as inline functions to reduce warnings.
  NA_IAPI NAi256 naMakei256          (NAi128 hi, NAu128 lo);
  NA_IAPI NAi256 naMakei256WithLo    (NAi128 lo);
  NA_IAPI NAi256 naMakei256WithDouble(double lo);
  NA_IAPI NAi128 naMakei256WithBinary(
    uint32 b7,
    uint32 b6,
    uint32 b5,
    uint32 b4,
    uint32 b3,
    uint32 b2,
    uint32 b1,
    uint32 b0);

  #define naGeti256Hi(i)
  #define naGeti256Lo(i)
  #define naGetu256Hi(u)
  #define naGetu256Lo(u)

  #define naNegi256(i)
  #define naInci256(i)
  #define naDeci256(i)
  #define naAddi256(a, b)
  #define naSubi256(a, b)
  #define naMuli256(a, b)
  #define naDivi256(a, b)
  #define naModi256(a, b)

  #define naNoti256(i)
  #define naOri256(a, b)
  #define naAndi256(a, b)
  #define naXori256(a, b)
  #define naShli256(a, n)
  #define naShri256(a, n)

  #define naEquali256(a, b)
  #define naGreateri256(a, b)
  #define naGreaterEquali256(a, b)
  #define naSmalleri256(a, b)
  #define naSmallerEquali256(a, b)

  #define naCasti256Tou8(i)
  #define naCasti256Tou16(i)
  #define naCasti256Tou32(i)
  #define naCasti256Tou64(i)
  #define naCasti256Tou128(i)
  #define naCasti256Tou256(i)
  #define naCasti256Toi8(i)
  #define naCasti256Toi16(i)
  #define naCasti256Toi32(i)
  #define naCasti256Toi64(i)
  #define naCasti256Toi128(i)
  #define naCasti256ToDouble(i)

  #define naMakeu256(hi, lo)
  #define naMakeu256WithLo(lo)
  #define naMakeu256WithDouble(d)
  #define naMakeu256WithBinary(b7, b6, b5, b4, b3, b2, b1, b0)

  #define naIncu256(i)
  #define naDecu256(i)
  #define naAddu256(a, b)
  #define naSubu256(a, b)
  #define naMulu256(a, b)
  #define naDivu256(a, b)
  #define naModu256(a, b)

  #define naNotu256(i)
  #define naOru256(a, b)
  #define naAndu256(a, b)
  #define naXoru256(a, b)
  #define naShlu256(a, n)
  #define naShru256(a, n)

  #define naEqualu256(a, b) 
  #define naGreateru256(a, b)
  #define naGreaterEqualu256(a, b)
  #define naSmalleru256(a, b)
  #define naSmallerEqualu256(a, b)

  #define naCastu256Toi8(i)
  #define naCastu256Toi16(i)
  #define naCastu256Toi32(i)
  #define naCastu256Toi64(i)
  #define naCastu256Toi128(i)
  #define naCastu256Toi256(i)
  #define naCastu256Tou8(i)
  #define naCastu256Tou16(i)
  #define naCastu256Tou32(i)
  #define naCastu256Tou64(i)
  #define naCastu256Tou128(i)
  #define naCastu256ToDouble(i)

#else

  // if no native int256 type is available, we have to emulate it.

  NA_IAPI NAi256  naMakei256          (NAi128 hi, NAu128 lo);
  NA_IAPI NAi256  naMakei256WithLo    (NAi128 lo);
  NA_IAPI NAi256  naMakei256WithDouble(double d);
  NA_IAPI NAi256 naMakei256WithBinary(
    uint32 b7,
    uint32 b6,
    uint32 b5,
    uint32 b4,
    uint32 b3,
    uint32 b2,
    uint32 b1,
    uint32 b0);

  #define naGeti256Hi(i) ((i).hi)
  #define naGeti256Lo(i) ((i).lo)
  #define naGetu256Hi(u) ((u).hi)
  #define naGetu256Lo(u) ((u).lo)

  NA_IAPI NAi256  naNegi256(NAi256 i);
  #define         naInci256(i)
  #define         naDeci256(i)
  NA_IAPI NAi256  naAddi256(NAi256 a, NAi256 b);
  NA_IAPI NAi256  naSubi256(NAi256 a, NAi256 b);
  NA_IAPI NAi256  naMuli256(NAi256 a, NAi256 b);
  NA_IAPI NAi256  naDivi256(NAi256 a, NAi256 b);
  NA_IAPI NAi256  naModi256(NAi256 a, NAi256 b);

  NA_IAPI NAi256  naNoti256(NAi256 i);
  NA_IAPI NAi256  naOri256 (NAi256 a, NAi256 b);
  NA_IAPI NAi256  naAndi256(NAi256 a, NAi256 b);
  NA_IAPI NAi256  naXori256(NAi256 a, NAi256 b);
  NA_IAPI NAi256  naShli256(NAi256 a, int n);
  NA_IAPI NAi256  naShri256(NAi256 a, int n);

  NA_IAPI NABool naEquali256       (NAi256 a, NAi256 b);
  NA_IAPI NABool naGreateri256     (NAi256 a, NAi256 b);
  NA_IAPI NABool naGreaterEquali256(NAi256 a, NAi256 b);
  NA_IAPI NABool naSmalleri256     (NAi256 a, NAi256 b);
  NA_IAPI NABool naSmallerEquali256(NAi256 a, NAi256 b);

  NA_IAPI uint8  naCasti256Tou8    (NAi256 i);
  NA_IAPI uint16 naCasti256Tou16   (NAi256 i);
  NA_IAPI uint32 naCasti256Tou32   (NAi256 i);
  NA_IAPI NAu64  naCasti256Tou64   (NAi256 i);
  NA_IAPI NAu128 naCasti256Tou128  (NAi256 i);
  NA_IAPI NAu256 naCasti256Tou256  (NAi256 i);
  NA_IAPI int8   naCasti256Toi8    (NAi256 i);
  NA_IAPI int16  naCasti256Toi16   (NAi256 i);
  NA_IAPI int32  naCasti256Toi32   (NAi256 i);
  NA_IAPI NAi64  naCasti256Toi64   (NAi256 i);
  NA_IAPI NAi128 naCasti256Toi128  (NAi256 i);
  NA_IAPI double naCasti256ToDouble(NAi256 i);

  NA_IAPI NAu256 naMakeu256          (NAu128 hi, NAu128 lo);
  NA_IAPI NAu256 naMakeu256WithLo    (NAu128 lo);
  NA_IAPI NAu256 naMakeu256WithDouble(double d);
  NA_IAPI NAu256 naMakeu256WithBinary(
    uint32 b7,
    uint32 b6,
    uint32 b5,
    uint32 b4,
    uint32 b3,
    uint32 b2,
    uint32 b1,
    uint32 b0);

  #define        naIncu256(i)
  #define        naDecu256(i)
  NA_IAPI NAu256 naAddu256(NAu256 a, NAu256 b);
  NA_IAPI NAu256 naAddu256(NAu256 a, NAu256 b);
  NA_IAPI NAu256 naSubu256(NAu256 a, NAu256 b);
  NA_IAPI NAu256 naMulu256(NAu256 a, NAu256 b);
  NA_IAPI NAu256 naDivu256(NAu256 a, NAu256 b);
  NA_IAPI NAu256 naModu256(NAu256 a, NAu256 b);

  NA_IAPI NAu256 naNotu256(NAu256 i);
  NA_IAPI NAu256 naOru256 (NAu256 a, NAu256 b);
  NA_IAPI NAu256 naAndu256(NAu256 a, NAu256 b);
  NA_IAPI NAu256 naXoru256(NAu256 a, NAu256 b);
  NA_IAPI NAu256 naShlu256(NAu256 a, int n);
  NA_IAPI NAu256 naShru256(NAu256 a, int n);

  NA_IAPI NABool naEqualu256       (NAu256 a, NAu256 b);
  NA_IAPI NABool naGreateru256     (NAu256 a, NAu256 b);
  NA_IAPI NABool naGreaterEqualu256(NAu256 a, NAu256 b);
  NA_IAPI NABool naSmalleru256     (NAu256 a, NAu256 b);
  NA_IAPI NABool naSmallerEqualu256(NAu256 a, NAu256 b);

  NA_IAPI int8   naCastu256Toi8    (NAu256 i);
  NA_IAPI int16  naCastu256Toi16   (NAu256 i);
  NA_IAPI int32  naCastu256Toi32   (NAu256 i);
  NA_IAPI NAi64  naCastu256Toi64   (NAu256 i);
  NA_IAPI NAi128 naCastu256Toi128  (NAu256 i);
  NA_IAPI NAi256 naCastu256Toi256  (NAu256 i);
  NA_IAPI uint8  naCastu256Tou8    (NAu256 i);
  NA_IAPI uint16 naCastu256Tou16   (NAu256 i);
  NA_IAPI uint32 naCastu256Tou32   (NAu256 i);
  NA_IAPI NAu64  naCastu256Tou64   (NAu256 i);
  NA_IAPI NAu128 naCastu256Tou128  (NAu256 i);
  NA_IAPI double naCastu256ToDouble(NAu256 i);

#endif


//#if NA_TYPE_NAINT_BITS == NA_TYPE64_BITS
//  #define naCastu256ToUInt(i) naCastu256Tou64(i)
//  #define naCasti256ToInt(i)  naCasti256Toi64(i)
//  #define naCastIntToi256(i)  naMakei256WithLo(naMakei128WithLo(i))
//  #define naCastUIntTou256(i) naMakeu256WithLo(naMakeu128WithLo(i))
//#elif NA_TYPE_NAINT_BITS == NA_TYPE32_BITS
//  #define naCastu256ToUInt(i) naCastu256Tou32(i)
//  #define naCasti256ToInt(i)  naCasti256Toi32(i)
//  #define naCastIntToi256(i)  naMakei256WithLo(naMakei128WithLo(naMakei64WithLo(i)))
//  #define naCastUIntTou256(i) naMakei256WithLo(naMakeu128WithLo(naMakeu64WithLo(i)))
//#elif NA_TYPE_NAINT_BITS == NA_TYPE16_BITS
//  #define naCastu256ToUInt(i) naCastu256Tou16(i)
//  #define naCasti256ToInt(i)  naCasti256Toi16(i)
//  #define naCastIntToi256(i)  naMakei256WithLo(naMakei128WithLo(naMakei64WithLo((int32)i)))
//  #define naCastUIntTou256(i) naMakei256WithLo(naMakeu128WithLo(naMakeu64WithLo((uint32)i)))
//#endif
//


#endif // NA_INT_256_INCLUDED



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
