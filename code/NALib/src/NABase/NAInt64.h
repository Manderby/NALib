
#if defined NA_INT_64_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_64_INCLUDED
#define NA_INT_64_INCLUDED



#if NA_NATIVE_INT64_IN_USE

  #define naMakei64(hi, lo)
  #define naMakei64WithBinary(b1, b0)

  #define naMakeu64(hi, lo)
  #define naMakeu64WithLo(lo)
  #define naMakeu64WithDouble(d)
  #define naMakeu64WithLiteralLo(lo)
  #define naMakeu64WithBinary(b1, b0)

  #define naGeti64Hi(i)
  #define naGeti64Lo(i)
  
  #define naGetu64Hi(u)
  #define naGetu64Lo(u)

  #define naInci64(i)
  #define naDeci64(i)
  #define naNegi64(i)
  #define naAddi64(a, b)
  #define naSubi64(a, b)
  #define naMuli64(a, b)
  #define naDivi64(a, b)
  #define naModi64(a, b)

  #define naIncu64(i)
  #define naDecu64(i)
  #define naAddu64(a, b)
  #define naSubu64(a, b)
  #define naMulu64(a, b)
  #define naDivu64(a, b)
  #define naModu64(a, b)

  #define naNoti64(i)
  #define naOri64(a, b)
  #define naAndi64(a, b)
  #define naXori64(a, b)
  #define naShli64(a, n)
  #define naShri64(a, n)

  #define naNotu64(i)
  #define naOru64(a, b)
  #define naAndu64(a, b)
  #define naXoru64(a, b)
  #define naShlu64(a, n)
  #define naShru64(a, n)

  #define naEquali64(a, b)
  #define naGreateri64(a, b)
  #define naGreaterEquali64(a, b)
  #define naSmalleri64(a, b)
  #define naSmallerEquali64(a, b)

  #define naEqualu64(a, b)
  #define naGreateru64(a, b)
  #define naGreaterEqualu64(a, b)
  #define naSmalleru64(a, b)
  #define naSmallerEqualu64(a, b)

  #define naCasti64Tou64(i)
  #define naCastu64Toi64(u)

  #define naCasti64Tou8(i)
  #define naCasti64Tou16(i)
  #define naCasti64Tou32(i)
  #define naCasti64Toi8(i)
  #define naCasti64Toi16(i)
  #define naCasti64Toi32(i)
  #define naCasti64ToSize(i)
  #define naCasti64ToFSize(i)
  #define naCasti64ToDouble(i)

  #define naCastu64Toi8(u)
  #define naCastu64Toi16(u)
  #define naCastu64Toi32(u)
  #define naCastu64Tou8(u)
  #define naCastu64Tou16(u)
  #define naCastu64Tou32(u)
  #define naCastu64ToDouble(u)

  #define naCastu8Toi64(u)
  #define naCastu16Toi64(u)
  #define naCastu32Toi64(u)
  #define naCasti8Toi64(i)
  #define naCasti16Toi64(i)
  #define naCasti32Toi64(i)
  #define naCastSizeToi64(s)
  #define naCastFSizeToi64(f)
  #define naCastDoubleToi64(d)
  #define naCastBoolToi64(b)

#else

  // if no native int64 type is available, we have to emulate it.

  NA_IAPI NAi64   naMakei64          (int32 hi, uint32 lo);
  NA_IAPI NAi64   naMakei64WithBinary(
    uint32 b1, 
    uint32 b0); 
 
  NA_IAPI NAu64   naMakeu64             (uint32 hi, uint32 lo);
  NA_IAPI NAu64   naMakeu64WithLo       (uint32 lo);
  NA_IAPI NAu64   naMakeu64WithDouble   (double d);
  #define         naMakeu64WithLiteralLo(lo)
  NA_IAPI NAu64   naMakeu64WithBinary   (
    uint32 hi,
    uint32 lo);
    
  #define         naGeti64Hi(i) ((i).hi)
  #define         naGeti64Lo(i) ((i).lo)
   
  #define         naGetu64Hi(u) ((u).hi)
  #define         naGetu64Lo(u) ((u).lo)
 
  #define         naInci64(i)
  #define         naDeci64(i)
  NA_IAPI NAi64   naNegi64(NAi64 i);
  NA_IAPI NAi64   naAddi64(NAi64 a, NAi64 b);
  NA_IAPI NAi64   naSubi64(NAi64 a, NAi64 b);
  NA_IAPI NAi64   naMuli64(NAi64 a, NAi64 b);
  NA_IAPI NAi64   naDivi64(NAi64 a, NAi64 b);
  NA_IAPI NAi64   naModi64(NAi64 a, NAi64 b);
 
  #define         naIncu64(i)
  #define         naDecu64(i)
  NA_IAPI NAu64   naAddu64(NAu64 a, NAu64 b);
  NA_IAPI NAu64   naAddu64(NAu64 a, NAu64 b);
  NA_IAPI NAu64   naSubu64(NAu64 a, NAu64 b);
  NA_IAPI NAu64   naMulu64(NAu64 a, NAu64 b);
  NA_IAPI NAu64   naDivu64(NAu64 a, NAu64 b);
  NA_IAPI NAu64   naModu64(NAu64 a, NAu64 b);
   
  NA_IAPI NAi64   naNoti64(NAi64 i);
  NA_IAPI NAi64   naOri64 (NAi64 a, NAi64 b);
  NA_IAPI NAi64   naAndi64(NAi64 a, NAi64 b);
  NA_IAPI NAi64   naXori64(NAi64 a, NAi64 b);
  NA_IAPI NAi64   naShli64(NAi64 a, int n);
  NA_IAPI NAi64   naShri64(NAi64 a, int n);
 
  NA_IAPI NAu64   naNotu64(NAu64 u);
  NA_IAPI NAu64   naOru64 (NAu64 a, NAu64 b);
  NA_IAPI NAu64   naAndu64(NAu64 a, NAu64 b);
  NA_IAPI NAu64   naXoru64(NAu64 a, NAu64 b);
  NA_IAPI NAu64   naShlu64(NAu64 a, int n);
  NA_IAPI NAu64   naShru64(NAu64 a, int n);
   
  NA_IAPI NABool  naEquali64       (NAi64 a, NAi64 b);
  NA_IAPI NABool  naGreateri64     (NAi64 a, NAi64 b);
  NA_IAPI NABool  naGreaterEquali64(NAi64 a, NAi64 b);
  NA_IAPI NABool  naSmalleri64     (NAi64 a, NAi64 b);
  NA_IAPI NABool  naSmallerEquali64(NAi64 a, NAi64 b);
 
  NA_IAPI NABool  naEqualu64       (NAu64 a, NAu64 b);
  NA_IAPI NABool  naGreateru64     (NAu64 a, NAu64 b);
  NA_IAPI NABool  naGreaterEqualu64(NAu64 a, NAu64 b);
  NA_IAPI NABool  naSmalleru64     (NAu64 a, NAu64 b);
  NA_IAPI NABool  naSmallerEqualu64(NAu64 a, NAu64 b);
   
  NA_IAPI NAu64   naCasti64Tou64   (NAi64 i);
  NA_IAPI NAi64   naCastu64Toi64   (NAu64 u);

  NA_IAPI uint8   naCasti64Tou8    (NAi64 i);
  NA_IAPI uint16  naCasti64Tou16   (NAi64 i);
  NA_IAPI uint32  naCasti64Tou32   (NAi64 i);
  NA_IAPI int8    naCasti64Toi8    (NAi64 i);
  NA_IAPI int16   naCasti64Toi16   (NAi64 i);
  NA_IAPI int32   naCasti64Toi32   (NAi64 i);
  NA_IAPI size_t  naCasti64ToSize  (NAi64 i);
  NA_IAPI fsize_t naCasti64ToFSize (NAi64 i);
  NA_IAPI double  naCasti64ToDouble(NAi64 i);

  NA_IAPI int8    naCastu64Toi8    (NAu64 u);
  NA_IAPI int16   naCastu64Toi16   (NAu64 u);
  NA_IAPI int32   naCastu64Toi32   (NAu64 u);
  NA_IAPI uint8   naCastu64Tou8    (NAu64 u);
  NA_IAPI uint16  naCastu64Tou16   (NAu64 u);
  NA_IAPI uint32  naCastu64Tou32   (NAu64 u);
  NA_IAPI double  naCastu64ToDouble(NAu64 u);
 
  NA_IAPI int64   naCastu8Toi64    (uint8 u);
  NA_IAPI int64   naCastu16Toi64   (uint16 u);
  NA_IAPI int64   naCastu32Toi64   (uint32 u);
  NA_IAPI int64   naCasti8Toi64    (int8 i);
  NA_IAPI int64   naCasti16Toi64   (int16 i);
  NA_IAPI int64   naCasti32Toi64   (int32 i);
  NA_IAPI int64   naCastSizeToi64  (size_t s);
  NA_IAPI int64   naCastFSizeToi64 (fsize_t f);
  NA_IAPI NAi64   naCastDoubleToi64(double d);
  NA_IAPI int64   naCastBoolToi64  (NABool s);

#endif



#endif // NA_INT_64_INCLUDED



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
