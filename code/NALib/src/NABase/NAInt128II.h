
#if defined NA_INT_128_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_128_II_INCLUDED
#define NA_INT_128_II_INCLUDED



#if defined NA_TYPE_INT128
  NA_IDEF NAi128 naMakei128(int64 hi, uint64 lo){return ((NAi128)hi << 64) | lo;}
  NA_IDEF NAi128 naMakei128WithLo(NAi64 lo){return (NAi128)lo;}
  NA_IDEF NAi128 naMakei128WithDouble(double lo){return (NAi128)lo;}
  NA_IDEF NAi128 naMakei128WithBinary(uint32 b3, uint32 b2, uint32 b1, uint32 b0){return ((NAi128)b3 << 96) | ((NAi128)b2 << 64) | ((NAi128)b1 << 32) | b0;}
  #else

  NA_IDEF NAi128 naMakei128(NAi64 hi, NAu64 lo){
    NAi128 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAi128 naMakei128WithLo(NAi64 lo){
    NAi128 retValuei;
    retValuei.hi = naCastu64Toi64(naGetSignum64(lo));
    retValuei.lo = naCasti64Tou64(lo);
    return retValuei;
  }
  NA_IDEF NAi128 naMakei128WithDouble(double d){
    return naMakei128WithLo(naGetDoubleInteger(d));
  }
  NA_IDEF NAi128 naMakei128WithBinary(uint32 b3, uint32 b2, uint32 b1, uint32 b0){
    NAi128 retValuei;
    retValuei.hi = naMakei64WithBinary(b3, b2);
    retValuei.lo = naMakeu64WithBinary(b1, b0);
    return retValuei;
  }



  #undef naInci128
  #define naInci128(i)\
    (i.hi = naAddi64(i.hi, naMakei64WithLo(naEqualu64(i.lo, NA_MAX_u64))),\
    naIncu64(i.lo),\
    i)
  #undef naDeci128
  #define naDeci128(i)\
    (i.hi = naSubi64(i.hi, naMakei64WithLo(naEqualu64(i.lo, NA_ZERO_u64))),\
    naDecu64(i.lo),\
    i)


  NA_IDEF NAi128 naNegi128(NAi128 i){
    NAi128 retValuei = naNoti128(i);
    naInci128(retValuei);
    return retValuei;
  }
  NA_IDEF NAi128 naAddi128(NAi128 a, NAi128 b){
    return naCastu128Toi128(naAddu128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naSubi128(NAi128 a, NAi128 b){
    return naCastu128Toi128(naSubu128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naMuli128(NAi128 a, NAi128 b){
    NAi128 retValuei;
    NAu128 retValueu;
    NAi128 aSign = naSigni128(a);
    NAi128 bSign = naSigni128(b);
    if(naSmalleri128(a, NA_ZERO_i128)){a = naNegi128(a);}
    if(naSmalleri128(b, NA_ZERO_i128)){b = naNegi128(b);}
    retValueu = naMulu128(naCasti128Tou128(a), naCasti128Tou128(b));
    retValueu.hi = naAndu64(retValueu.hi, naNotu64(naCasti64Tou64(NA_SIGN_MASK_64)));
    retValuei = naCastu128Toi128(retValueu);
    if(!naEquali128(aSign, bSign)){retValuei = naNegi128(retValuei);}
    // todo: overflow may lead to different result than built-in 128 bit integer
    return retValuei;
  }
  NA_IDEF NAi128 naDivi128(NAi128 a, NAi128 b){
    NAi128 retValuei;
    NAu128 retValueu;
    NAi128 aSign = naSigni128(a);
    NAi128 bSign = naSigni128(b);
    if(naSmalleri128(a, NA_ZERO_i128)){a = naNegi128(a);}
    if(naSmalleri128(b, NA_ZERO_i128)){b = naNegi128(b);}
    retValueu = naDivu128(naCasti128Tou128(a), naCasti128Tou128(b));
    retValueu.hi = naAndu64(retValueu.hi, naNotu64(naCasti64Tou64(NA_SIGN_MASK_64)));
    retValuei = naCastu128Toi128(retValueu);
    if(!naEquali128(aSign, bSign)){retValuei = naNegi128(retValuei);}
    return retValuei;
  }
  NA_IDEF NAi128 naModi128(NAi128 a, NAi128 b){
    NAi128 retValuei;
    NAu128 retValueu;
    NAi128 aSign = naSigni128(a);
    if(naSmalleri128(a, NA_ZERO_i128)){a = naNegi128(a);}
    if(naSmalleri128(b, NA_ZERO_i128)){b = naNegi128(b);}
    retValueu = naModu128(naCasti128Tou128(a), naCasti128Tou128(b));
    retValueu.hi = naAndu64(retValueu.hi, naNotu64(naCasti64Tou64(NA_SIGN_MASK_64)));
    retValuei = naCastu128Toi128(retValueu);
    if(!naEquali128(aSign, NA_ONE_i128)){retValuei = naNegi128(retValuei);}
    return retValuei;
  }



  NA_IDEF NAi128 naNoti128(NAi128 i){
    return naCastu128Toi128(naNotu128(naCasti128Tou128(i)));
  }
  NA_IDEF NAi128 naOri128(NAi128 a, NAi128 b){
    return naCastu128Toi128(naOru128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naAndi128(NAi128 a, NAi128 b){
    return naCastu128Toi128(naAndu128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naXori128(NAi128 a, NAi128 b){
    return naCastu128Toi128(naXoru128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naShli128(NAi128 a, int n){
    return naCastu128Toi128(naShlu128(naCasti128Tou128(a), n));
  }
  NA_IDEF NAi128 naShri128(NAi128 a, int n){
    NAi128 retValuei;
    if(n < 0){
      retValuei = naShli128(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 64){
        retValuei.lo = naShru64(a.lo, n);
        retValuei.lo = naOru64(retValuei.lo, naShlu64(naCasti64Tou64(a.hi), (64 - n)));
        retValuei.hi = naShri64(a.hi, n);
      }else{
        NAu64 signum = naGetSignum64(a.hi); // Sign preservation!
        retValuei.lo = naCasti64Tou64(naShri64(a.hi, (n - 64)));
        retValuei.hi = naCastu64Toi64(signum);
      }
    }
    return retValuei;
  }



  NA_IDEF NABool naEquali128(NAi128 a, NAi128 b){
    return naEqualu128(naCasti128Tou128(a), naCasti128Tou128(b));
  }
  NA_IDEF NABool naGreateri128(NAi128 a, NAi128 b){
    return (naGreateri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naSmalleru64(a.lo, b.lo) : naGreateru64(a.lo, b.lo))));
  }
  NA_IDEF NABool naGreaterEquali128(NAi128 a, NAi128 b){
    return (naGreateri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naSmallerEqualu64(a.lo, b.lo) : naGreaterEqualu64(a.lo, b.lo))));
  }
  NA_IDEF NABool naSmalleri128(NAi128 a, NAi128 b){
    return (naSmalleri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naGreateru64(a.lo, b.lo) : naSmalleru64(a.lo, b.lo))));
  }
  NA_IDEF NABool naSmallerEquali128(NAi128 a, NAi128 b){
    return (naSmalleri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naGreaterEqualu64(a.lo, b.lo) : naSmallerEqualu64(a.lo, b.lo))));
  }



  NA_IDEF uint8 naCasti128Tou8(NAi128 i){
    return naCastu128Tou8(naCasti128Tou128(i));
  }
  NA_IDEF uint16 naCasti128Tou16(NAi128 i){
    return naCastu128Tou16(naCasti128Tou128(i));
  }
  NA_IDEF uint32 naCasti128Tou32(NAi128 i){
    return naCastu128Tou32(naCasti128Tou128(i));
  }
  NA_IDEF NAu64 naCasti128Tou64(NAi128 i){
    return naCastu128Tou64(naCasti128Tou128(i));
  }
  NA_IDEF NAu128 naCasti128Tou128(NAi128 i){
    NAu128 retValuei;
    retValuei.hi = naCasti64Tou64(i.hi);
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF int8 naCasti128Toi8(NAi128 i){
    return naCastu128Toi8(naCasti128Tou128(i));
  }
  NA_IDEF int16 naCasti128Toi16(NAi128 i){
    return naCastu128Toi16(naCasti128Tou128(i));
  }
  NA_IDEF int32 naCasti128Toi32(NAi128 i){
    return naCastu128Toi32(naCasti128Tou128(i));
  }
  NA_IDEF NAi64 naCasti128Toi64(NAi128 i){
    return naCastu128Toi64(naCasti128Tou128(i));
  }
  NA_IDEF double naCasti128ToDouble(NAi128 i){
    return naCasti64ToDouble(naCasti128Toi64(i));
  }



  NA_IDEF NAu128 naMakeu128(NAu64 hi, NAu64 lo){
    NAu128 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu128 naMakeu128WithLo(NAu64 lo){
    NAu128 retValuei;
    retValuei.hi = NA_ZERO_u64;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu128 naMakeu128WithDouble(double d){
    return naMakeu128WithLo(naMakeu64WithDouble(d));
  }
  
  NA_IDEF NAu128 naMakeu128WithBinary(uint32 b3, uint32 b2, uint32 b1, uint32 b0){
    NAu128 retValueu;
    retValueu.hi = naMakeu64WithBinary(b3, b2);
    retValueu.lo = naMakeu64WithBinary(b1, b0);
    return retValueu;
  }



  #undef naIncu128
  #define naIncu128(i)\
    (i.hi = naAddu64(i.hi, naMakeu64WithLo(naEqualu64(i.lo, NA_MAX_u64))),\
    naIncu64(i.lo),\
    i)
  #undef naDecu128
  #define naDecu128(i)\
    (i.hi = naSubu64(i.hi, naMakeu64WithLo(naEqualu64(i.lo, NA_ZERO_u64))),\
    naDecu64(i.lo),\
    i)



  NA_IDEF NAu128 naAddu128(NAu128 a, NAu128 b){
    NAu128 retValuei;
    retValuei.lo = naAddu64(a.lo, b.lo);
    retValuei.hi = naAddu64(a.hi, b.hi);
    retValuei.hi = naAddu64(retValuei.hi, naMakeu64WithLo(naSmalleru64(retValuei.lo, a.lo))); // add a carry if there was an overflow.
    return retValuei;
  }
  NA_IDEF NAu128 naSubu128(NAu128 a, NAu128 b){
    return naAddu128(a, naCasti128Tou128(naNegi128(naCastu128Toi128(b))));
  }
  NA_IDEF NAu128 naMulu128(NAu128 a, NAu128 b){
    NAu128 retValuei = NA_ZERO_u128;

    NAu64 a0 = naAndu64(a.lo, naMakeu64WithLo(NA_MAX_u32));
    NAu64 a1 = naShru64(a.lo, 32);
    NAu64 a2 = naAndu64(a.hi, naMakeu64WithLo(NA_MAX_u32));
    NAu64 a3 = naShru64(a.hi, 32);
    NAu64 b0 = naAndu64(b.lo, naMakeu64WithLo(NA_MAX_u32));
    NAu64 b1 = naShru64(b.lo, 32);
    NAu64 b2 = naAndu64(b.hi, naMakeu64WithLo(NA_MAX_u32));
    NAu64 b3 = naShru64(b.hi, 32);
    NAu64 a0b1 = naMulu64(a0, b1);
    NAu64 a1b0 = naMulu64(a1, b0);

    // multiply a0 * b and add up
    retValuei.lo = naAddu64(retValuei.lo, naMulu64(a0, b0));
    NAu64 shla0b1 = naShlu64(a0b1, 32);
    retValuei.lo = naAddu64(retValuei.lo, shla0b1);
    retValuei.hi = naAddu64(retValuei.hi, naSmalleru64(retValuei.lo, shla0b1));
    retValuei.hi = naAddu64(retValuei.hi, naShru64(a0b1, 32));
    retValuei.hi = naAddu64(retValuei.hi, naMulu64(a0, b2));
    retValuei.hi = naAddu64(retValuei.hi, naShlu64(naMulu64(a0, b3), 32));

    // multiply a1 * b and add up
    NAu64 shla1b0 = naShlu64(a1b0, 32);
    retValuei.lo = naAddu64(retValuei.lo, shla1b0);
    retValuei.hi = naAddu64(retValuei.hi, naSmalleru64(retValuei.lo, shla1b0));
    retValuei.hi = naAddu64(retValuei.hi, naShru64(a1b0, 32));
    retValuei.hi = naAddu64(retValuei.hi, naMulu64(a1, b1));
    retValuei.hi = naAddu64(retValuei.hi, naShlu64(naMulu64(a1, b2), 32));

    // multiply a2 * b and add up
    retValuei.hi = naAddu64(retValuei.hi, naMulu64(a2, b0));
    retValuei.hi = naAddu64(retValuei.hi, naShlu64(naMulu64(a2, b1), 32));

    // multiply a3 * b and add up
    retValuei.hi = naAddu64(retValuei.hi, naShlu64(naMulu64(a3, b0), 32));

    return retValuei;
  }
  NA_HIDEF void na_Computeu128Division(NAu128 a, NAu128 b, NAu128* div, NAu128* rem){
    NAu128 bTmp;
    NAu128 aHighestBit;
    NAu128 bHighestBit;
    *div = NA_ZERO_u128;
    *rem = a;
    if(naEqualu128(b, NA_ZERO_u128)){
      #ifndef NDEBUG
        naCrash("Integer Division by 0");
      #endif
    }else if(naSmalleru128(a, b)){
      // b is larger than a and hence the result is zero.
      // Do nothing here and just return with the values set above.
    }else{
      int shiftCount;

      // search for the highest bit of b.
      aHighestBit = naMakeu128(naCasti64Tou64(NA_SIGN_MASK_64), NA_ZERO_u64);
      while(!naEqualu128(naAndu128(a, aHighestBit), aHighestBit)){
        aHighestBit = naShru128(aHighestBit, 1);
      }
      bHighestBit = naMakeu128(naCasti64Tou64(NA_SIGN_MASK_64), NA_ZERO_u64);
      while(!naEqualu128(naAndu128(b, bHighestBit), bHighestBit)){
        bHighestBit = naShru128(bHighestBit, 1);
      }

      bTmp = b;
      shiftCount = 0;
      // Make the dividend big enough
      while(!naEqualu128(aHighestBit, bHighestBit)){
        if(naEqualu128(bTmp, NA_ZERO_u128)){
          // b is larger than a and hence the result is zero.
          #ifndef NDEBUG
            naError("This should not happen.");
          #endif
        }
        bTmp = naShlu128(bTmp, 1);
        bHighestBit = naShlu128(bHighestBit, 1);
        shiftCount++;
      }

      while(shiftCount >= 0){
        *div = naShlu128(*div, 1);
        if(naGreaterEqualu128(*rem, bTmp)){
          *div = naOru128(*div, NA_ONE_u128);
          *rem = naSubu128(*rem, bTmp);
        }
        bTmp = naShru128(bTmp, 1);
        shiftCount--;
      }
    }
  }
  NA_IDEF NAu128 naDivu128(NAu128 a, NAu128 b){
    NAu128 divInt;
    NAu128 remInt;
    na_Computeu128Division(a, b, &divInt, &remInt);
    return divInt;
  }
  NA_IDEF NAu128 naModu128(NAu128 a, NAu128 b){
    NAu128 divInt;
    NAu128 remInt;
    na_Computeu128Division(a, b, &divInt, &remInt);
    return remInt;
  }



  NA_IDEF NAu128  naNotu128(NAu128 i){
    NAu128 retValuei;
    retValuei.hi = naNotu64(i.hi);
    retValuei.lo = naNotu64(i.lo);
    return retValuei;
  }
  NA_IDEF NAu128  naOru128 (NAu128 a, NAu128 b){
    NAu128 retValuei;
    retValuei.hi = naOru64(a.hi, b.hi);
    retValuei.lo = naOru64(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu128  naAndu128(NAu128 a, NAu128 b){
    NAu128 retValuei;
    retValuei.hi = naAndu64(a.hi, b.hi);
    retValuei.lo = naAndu64(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu128  naXoru128(NAu128 a, NAu128 b){
    NAu128 retValuei;
    retValuei.hi = naXoru64(a.hi, b.hi);
    retValuei.lo = naXoru64(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu128 naShlu128(NAu128 a, int n){
    NAu128 retValuei;
    if(n < 0){
      retValuei = naShru128(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 64){
        retValuei.hi = naShlu64(a.hi, n);
        retValuei.hi = naOru64(retValuei.hi, naShru64(a.lo, (64 - n)));
        retValuei.lo = naShlu64(a.lo, n);
      }else{
        retValuei.hi = naShlu64(a.lo, (n - 64));
        retValuei.lo = NA_ZERO_u64;
      }
    }
    return retValuei;
  }
  NA_IDEF NAu128 naShru128(NAu128 a, int n){
    NAu128 retValuei;
    if(n < 0){
      retValuei = naShlu128(a, -n);
    }else{
    // Beware, do not use <= as some processors will result
    // in garbage when the shift is equal to the type size.
      if(n < 64){
        retValuei.lo = naShru64(a.lo, n);
        retValuei.lo = naOru64(retValuei.lo, naShlu64(a.hi, (64 - n)));
        retValuei.hi = naShru64(a.hi, n);
      }else{
        retValuei.lo = naShru64(a.hi, (n - 64));
        retValuei.hi = NA_ZERO_u64;
      }
    }
    return retValuei;
  }



  NA_IDEF NABool naEqualu128(NAu128 a, NAu128 b){
    return (naEqualu64(a.hi, b.hi) && naEqualu64(a.lo, b.lo));
  }
  NA_IDEF NABool naGreateru128(NAu128 a, NAu128 b){
    return (naGreateru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naGreateru64(a.lo, b.lo)));
  }
  NA_IDEF NABool naGreaterEqualu128(NAu128 a, NAu128 b){
    return (naGreateru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naGreaterEqualu64(a.lo, b.lo)));
  }
  NA_IDEF NABool naSmalleru128(NAu128 a, NAu128 b){
    return (naSmalleru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naSmalleru64(a.lo, b.lo)));
  }
  NA_IDEF NABool naSmallerEqualu128(NAu128 a, NAu128 b){
    return (naSmalleru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naSmallerEqualu64(a.lo, b.lo)));
  }



  NA_IDEF int8 naCastu128Toi8(NAu128 i){
    return naCastu64Toi8(i.lo);
  }
  NA_IDEF int16 naCastu128Toi16(NAu128 i){
    return naCastu64Toi16(i.lo);
  }
  NA_IDEF int32 naCastu128Toi32(NAu128 i){
    return naCastu64Toi32(i.lo);
  }
  NA_IDEF NAi64 naCastu128Toi64(NAu128 i){
    return naCastu64Toi64(i.lo);
  }
  NA_IDEF NAi128 naCastu128Toi128(NAu128 i){
    NAi128 retValuei;
    retValuei.hi = naCastu64Toi64(i.hi);
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF uint8 naCastu128Tou8(NAu128 i){
    return naCastu64Tou8(i.lo);
  }
  NA_IDEF uint16 naCastu128Tou16(NAu128 i){
    return naCastu64Tou16(i.lo);
  }
  NA_IDEF uint32 naCastu128Tou32(NAu128 i){
    return naCastu64Tou32(i.lo);
  }
  NA_IDEF NAu64 naCastu128Tou64(NAu128 i){
    return i.lo;
  }
  NA_IDEF double naCastu128ToDouble(NAu128 i){
    return naCastu64ToDouble(i.hi) * naMakeDoubleWithExponent(64) + naCastu64ToDouble(i.lo);
  }

#endif // defined NA_TYPE_INT128



#endif // NA_INT_128_II_INCLUDED



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
