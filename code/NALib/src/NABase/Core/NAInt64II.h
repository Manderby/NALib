
#if defined NA_INT_64_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_64_II_INCLUDED
#define NA_INT_64_II_INCLUDED



// first, undefine all things which were defined in the .h file.
#undef naMakei64
#undef naMakei64WithLo
#undef naMakei64WithDouble
#undef naMakei64WithBinary
#undef naGeti64Hi
#undef naGeti64Lo
#undef naGetu64Hi
#undef naGetu64Lo
#undef naNegi64
#undef naInci64
#undef naDeci64
#undef naAddi64
#undef naSubi64
#undef naMuli64
#undef naDivi64
#undef naModi64
#undef naNoti64
#undef naOri64
#undef naAndi64
#undef naXori64
#undef naShli64
#undef naShri64
#undef naEquali64
#undef naGreateri64
#undef naGreaterEquali64
#undef naSmalleri64
#undef naSmallerEquali64
#undef naCasti64Tou8
#undef naCasti64Tou16
#undef naCasti64Tou32
#undef naCasti64Tou64
#undef naCasti64Toi8
#undef naCasti64Toi16
#undef naCasti64Toi32
#undef naCasti64ToDouble
#undef naMakeu64
#undef naMakeu64WithLo
#undef naMakeu64WithDouble
#undef naMakeu64WithLiteralLo
#undef naMakeu64WithBinary
#undef naIncu64
#undef naDecu64
#undef naAddu64
#undef naSubu64
#undef naMulu64
#undef naDivu64
#undef naModu64
#undef naNotu64
#undef naOru64
#undef naAndu64
#undef naXoru64
#undef naShlu64
#undef naShru64
#undef naEqualu64
#undef naGreateru64
#undef naGreaterEqualu64
#undef naSmalleru64
#undef naSmallerEqualu64
#undef naCastu64Toi8
#undef naCastu64Toi16
#undef naCastu64Toi32
#undef naCastu64Toi64
#undef naCastu64Tou8
#undef naCastu64Tou16
#undef naCastu64Tou32
#undef naCastu64ToDouble



#if defined NA_TYPE_INT64

  // Native

  NA_IDEF NAi64 na_Makei64(int32 hi, uint32 lo) {
    return ((NAi64)((NAu64)hi << 32)) | lo;
  }
  NA_IDEF NAi64 na_Makei64WithLo(int32 lo) {
    return (NAi64)lo;
  }
  NA_IDEF NAi64 na_Makei64WithDouble(double lo) {
    return (NAi64)lo;
  }
  NA_IDEF NAi64 na_Makei64WithBinary(uint32 b1, uint32 b0) {
    return ((NAi64)((NAu64)b1 << 32)) | b0;
  }

  // We declare these as inline functions to reduce warnings.
  #define naMakei64(hi, lo)           na_Makei64(hi, lo)
  #define naMakei64WithLo(lo)         na_Makei64WithLo(lo)
  #define naMakei64WithDouble(lo)     na_Makei64WithDouble(lo)
  #define naMakei64WithBinary(b1, b0) na_Makei64WithBinary(b1, b0)

  #define naGeti64Hi(i)               ((int32)((i) >> NA_TYPE32_BITS))
  #define naGeti64Lo(i)               ((uint32)i)
  #define naGetu64Hi(u)               ((uint32)((u) >> NA_TYPE32_BITS))
  #define naGetu64Lo(u)               ((uint32)u)

  #define naNegi64(i)                 (-(i))
  #define naInci64(i)                 (++(i))
  #define naDeci64(i)                 (--(i))
  #define naAddi64(a, b)              ((a) + (b))
  #define naSubi64(a, b)              ((a) - (b))
  #define naMuli64(a, b)              ((a) * (b))
  #define naDivi64(a, b)              ((a) / (b))
  #define naModi64(a, b)              ((a) % (b))

  #define naNoti64(i)                 (~(i))
  #define naOri64(a, b)               ((a) | (b))
  #define naAndi64(a, b)              ((a) & (b))
  #define naXori64(a, b)              ((a) ^ (b))
  #define naShli64(a, n)              ((int64)(((uint64)(a)) << (n)))
  #define naShri64(a, n)              ((a) >> (n))

  #define naEquali64(a, b)            ((a) == (b))
  #define naGreateri64(a, b)          ((a) > (b))
  #define naGreaterEquali64(a, b)     ((a) >= (b))
  #define naSmalleri64(a, b)          ((a) < (b))
  #define naSmallerEquali64(a, b)     ((a) <= (b))

  #define naCasti64Tou8(i)            ((uint8)(i))
  #define naCasti64Tou16(i)           ((uint16)(i))
  #define naCasti64Tou32(i)           ((uint32)(i))
  #define naCasti64Tou64(i)           ((NAu64)(i))
  #define naCasti64Toi8(i)            ((int8)(i))
  #define naCasti64Toi16(i)           ((int16)(i))
  #define naCasti64Toi32(i)           ((int32)(i))
  #define naCasti64ToDouble(i)        ((double)(i))

  #define naMakeu64(hi, lo)           ((NAu64)(((NAu64)(hi) << 32) | (lo)))
  #define naMakeu64WithLo(lo)         ((NAu64)(lo))
  #define naMakeu64WithDouble(d)      ((NAu64)(d))
  #define naMakeu64WithLiteralLo(lo)  (lo ## uLL)
  #define naMakeu64WithBinary(b1, b0) ((NAu64)(((NAu64)(b1) << 32) | (b0)))

  #define naIncu64(i)                 ((i)++)
  #define naDecu64(i)                 ((i)--)
  #define naAddu64(a, b)              ((a) + (b))
  #define naSubu64(a, b)              ((a) - (b))
  #define naMulu64(a, b)              ((a) * (b))
  #define naDivu64(a, b)              ((a) / (b))
  #define naModu64(a, b)              ((a) % (b))
                                     
  #define naNotu64(i)                 (~(i))
  #define naOru64(a, b)               ((a) | (b))
  #define naAndu64(a, b)              ((a) & (b))
  #define naXoru64(a, b)              ((a) ^ (b))
  #define naShlu64(a, n)              ((a) << (n))
  #define naShru64(a, n)              ((a) >> (n))
                                     
  #define naEqualu64(a, b)            ((a) == (b))
  #define naGreateru64(a, b)          ((a) > (b))
  #define naGreaterEqualu64(a, b)     ((a) >= (b))
  #define naSmalleru64(a, b)          ((a) < (b))
  #define naSmallerEqualu64(a, b)     ((a) <= (b))
                                     
  #define naCastu64Toi8(i)            ((int8)(i))
  #define naCastu64Toi16(i)           ((int16)(i))
  #define naCastu64Toi32(i)           ((int32)(i))
  #define naCastu64Toi64(i)           ((NAi64)(i))
  #define naCastu64Tou8(i)            ((uint8)(i))
  #define naCastu64Tou16(i)           ((uint16)(i))
  #define naCastu64Tou32(i)           ((uint32)(i))
  #define naCastu64ToDouble(i)        ((double)(i))

#else

  // Emulated

  NA_IDEF NAi64 naMakei64(int32 hi, uint32 lo) {
    NAi64 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAi64 naMakei64WithLo(int32 lo) {
    NAi64 retValuei;
    retValuei.hi = (int32)naGetSignum32(lo);
    retValuei.lo = (uint32)lo;
    return retValuei;
  }
  NA_IDEF NAi64 naMakei64WithDouble(double d) {
    return naGetDoubleInteger(d);
  }
  NA_IDEF NAi64 naMakei64WithBinary(uint32 b1, uint32 b0) {
    NAi64 retValuei;
    retValuei.hi = (int32)b1;
    retValuei.lo = b0;
    return retValuei;
  }



  #undef naInci64
  #define naInci64(i) (i.hi += (i.lo == NA_MAX_u32), i.lo++, i)
  #undef naDeci64
  #define naDeci64(i) (i.hi -= (i.lo == NA_ZERO_u32), i.lo--, i)


  NA_IDEF NAi64 naNegi64(NAi64 i) {
    NAi64 retValuei = naNoti64(i);
    naInci64(retValuei);
    return retValuei;
  }
  NA_IDEF NAi64 naAddi64(NAi64 a, NAi64 b) {
    return naCastu64Toi64(naAddu64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naSubi64(NAi64 a, NAi64 b) {
    return naCastu64Toi64(naSubu64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naMuli64(NAi64 a, NAi64 b) {
    NAi64 retValuei;
    NAu64 retValueu;
    NAi64 aSign = naSigni64(a);
    NAi64 bSign = naSigni64(b);
    if(naSmalleri64(a, NA_ZERO_i64)) {
      a = naNegi64(a);
    }
    if(naSmalleri64(b, NA_ZERO_i64)) {
      b = naNegi64(b);
    }
    retValueu = naMulu64(naCasti64Tou64(a), naCasti64Tou64(b));
    retValueu.hi &= ~NA_SIGN_MASK_32;
    retValuei = naCastu64Toi64(retValueu);
    if(!naEquali64(aSign, bSign)) {
      retValuei = naNegi64(retValuei);
    }
    // todo: overflow may lead to different result than built-in 64 bit integer
    return retValuei;
  }
  NA_IDEF NAi64 naDivi64(NAi64 a, NAi64 b) {
    NAi64 retValuei;
    NAu64 retValueu;
    NAi64 aSign = naSigni64(a);
    NAi64 bSign = naSigni64(b);
    if(naSmalleri64(a, NA_ZERO_i64)) {
      a = naNegi64(a);
    }
    if(naSmalleri64(b, NA_ZERO_i64)) {
      b = naNegi64(b);
    }
    retValueu = naDivu64(naCasti64Tou64(a), naCasti64Tou64(b));
    retValueu.hi &= ~NA_SIGN_MASK_32;
    retValuei = naCastu64Toi64(retValueu);
    if(!naEquali64(aSign, bSign)) {
      retValuei = naNegi64(retValuei);
    }
    return retValuei;
  }
  NA_IDEF NAi64 naModi64(NAi64 a, NAi64 b) {
    NAi64 retValuei;
    NAu64 retValueu;
    NAi64 aSign = naSigni64(a);
    if(naSmalleri64(a, NA_ZERO_i64)) {
      a = naNegi64(a);
    }
    if(naSmalleri64(b, NA_ZERO_i64)) {
      b = naNegi64(b);
    }
    retValueu = naModu64(naCasti64Tou64(a), naCasti64Tou64(b));
    retValueu.hi &= ~NA_SIGN_MASK_32;
    retValuei = naCastu64Toi64(retValueu);
    if(!naEquali64(aSign, NA_ONE_i64)) {
      retValuei = naNegi64(retValuei);
    }
    return retValuei;
  }



  NA_IDEF NAi64 naNoti64(NAi64 i) {
    return naCastu64Toi64(naNotu64(naCasti64Tou64(i)));
  }
  NA_IDEF NAi64 naOri64(NAi64 a, NAi64 b) {
    return naCastu64Toi64(naOru64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naAndi64(NAi64 a, NAi64 b) {
    return naCastu64Toi64(naAndu64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naXori64(NAi64 a, NAi64 b) {
    return naCastu64Toi64(naXoru64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naShli64(NAi64 a, int n) {
    return naCastu64Toi64(naShlu64(naCasti64Tou64(a), n));
  }
  NA_IDEF NAi64 naShri64(NAi64 a, int n) {
    NAi64 retValuei;
    if(n < 0) {
      retValuei = naShli64(a, -n);
    }else{
    // Beware, do not use <= as some processors will result
    // in garbage when the shift is equal to the type size.
      if(n < 32) {
        retValuei.lo = a.lo >> n;
        retValuei.lo |= ((uint32)a.hi << (32 - n));
        retValuei.hi = a.hi >> n;
      }else{
        uint32 signum = naGetSignum32(a.hi); // Sign preservation!
        retValuei.lo = (uint32)((int32)a.hi >> (n - 32));
        retValuei.hi = (int32)signum;
      }
    }
    return retValuei;
  }



  NA_IDEF NABool naEquali64(NAi64 a, NAi64 b) {
    return naEqualu64(naCasti64Tou64(a), naCasti64Tou64(b));
  }
  NA_IDEF NABool naGreateri64(NAi64 a, NAi64 b) {
    return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo < b.lo) : (a.lo > b.lo))));
  }
  NA_IDEF NABool naGreaterEquali64(NAi64 a, NAi64 b) {
    return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo <= b.lo) : (a.lo >= b.lo))));
  }
  NA_IDEF NABool naSmalleri64(NAi64 a, NAi64 b) {
    return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo > b.lo) : (a.lo < b.lo))));
  }
  NA_IDEF NABool naSmallerEquali64(NAi64 a, NAi64 b) {
    return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo >= b.lo) : (a.lo <= b.lo))));
  }



  NA_IDEF uint8 naCasti64Tou8(NAi64 i) {
    return naCastu64Tou8(naCasti64Tou64(i));
  }
  NA_IDEF uint16 naCasti64Tou16(NAi64 i) {
    return naCastu64Tou16(naCasti64Tou64(i));
  }
  NA_IDEF uint32 naCasti64Tou32(NAi64 i) {
    return naCastu64Tou32(naCasti64Tou64(i));
  }
  NA_IDEF NAu64 naCasti64Tou64(NAi64 i) {
    NAu64 retValuei;
    retValuei.hi = (uint32)i.hi;
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF int8 naCasti64Toi8(NAi64 i) {
    return naCastu64Toi8(naCasti64Tou64(i));
  }
  NA_IDEF int16 naCasti64Toi16(NAi64 i) {
    return naCastu64Toi16(naCasti64Tou64(i));
  }
  NA_IDEF int32 naCasti64Toi32(NAi64 i) {
    return naCastu64Toi32(naCasti64Tou64(i));
  }
  NA_IDEF double naCasti64ToDouble(NAi64 i) {
    // warning: this seems to be troublesome in the lower part. Find a
    // better solution in the future by using bit manipulation. todo
    return (double)i.hi * naMakeDoubleWithExponent(32) + ((i.hi < 0) ? -(double)i.lo : (double)i.lo);
  }



  NA_IDEF NAu64 naMakeu64(uint32 hi, uint32 lo) {
    NAu64 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu64 naMakeu64WithLo(uint32 lo) {
    NAu64 retValuei;
    retValuei.hi = NA_ZERO_u32;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu64 naMakeu64WithDouble(double d) {
    NAu64 retValuei;
    // note: this is somewhat cumbersome. Do it with bit manipulation. todo.
    retValuei.hi = (uint32)(d / naMakeDoubleWithExponent(32));
    retValuei.lo = (uint32)(d - ((double)((int32)retValuei.hi) * naMakeDoubleWithExponent(32)));
    return retValuei;
  }

  #undef naMakeu64WithLiteralLo
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    #define naMakeu64WithLiteralLo(lo)  {0, lo}
  #else
    #define naMakeu64WithLiteralLo(lo)  {lo, 0}
  #endif

  NA_IDEF NAu64 naMakeu64WithBinary(uint32 b1, uint32 b0) {
    NAu64 retValuei;
    retValuei.hi = b1;
    retValuei.lo = b0;
    return retValuei;
  }



  #undef naIncu64
  #define naIncu64(i) (i.hi += (i.lo == NA_MAX_u32), i.lo += 1, i)
  #undef naDecu64
  #define naDecu64(i) (i.hi -= (i.lo == NA_ZERO_u32), i.lo -= 1, i)



  NA_IDEF NAu64 naAddu64(NAu64 a, NAu64 b) {
    NAu64 retValuei;
    retValuei.lo = a.lo + b.lo;
    retValuei.hi = a.hi + b.hi;
    retValuei.hi += (retValuei.lo < a.lo); // add a carry if there was an overflow.
    return retValuei;
  }
  NA_IDEF NAu64 naSubu64(NAu64 a, NAu64 b) {
    return naAddu64(a, naCasti64Tou64(naNegi64(naCastu64Toi64(b))));
  }
  NA_IDEF NAu64 naMulu64(NAu64 a, NAu64 b) {
    NAu64 retValuei = NA_ZERO_u64;

    uint32 a0 = a.lo & NA_MAX_u16;
    uint32 a1 = a.lo >> 16;
    uint32 a2 = a.hi & NA_MAX_u16;
    uint32 a3 = a.hi >> 16;
    uint32 b0 = b.lo & NA_MAX_u16;
    uint32 b1 = b.lo >> 16;
    uint32 b2 = b.hi & NA_MAX_u16;
    uint32 b3 = b.hi >> 16;
    uint32 a0b1 = a0 * b1;
    uint32 a1b0 = a1 * b0;

    // multiply a0 * b and add up
    retValuei.lo += a0 * b0;
    uint32 shla0b1 = a0b1 << 16;
    retValuei.lo += shla0b1;
    retValuei.hi += retValuei.lo < shla0b1;
    retValuei.hi += a0b1 >> 16;
    retValuei.hi += a0 * b2;
    retValuei.hi += (a0 * b3) << 16;

    // multiply a1 * b and add up
    uint32 shla1b0 = a1b0 << 16;
    retValuei.lo += shla1b0;
    retValuei.hi += retValuei.lo < shla1b0;
    retValuei.hi += a1b0 >> 16;
    retValuei.hi += a1 * b1;
    retValuei.hi += (a1 * b2) << 16;

    // multiply a2 * b and add up
    retValuei.hi += a2 * b0;
    retValuei.hi += (a2 * b1) << 16;

    // multiply a3 * b and add up
    retValuei.hi += (a3 * b0) << 16;

    return retValuei;
  }
  NA_HIDEF void na_Computeu64Division(NAu64 a, NAu64 b, NAu64* div, NAu64* rem) {
    NAu64 bTmp;
    NAu64 aHighestBit;
    NAu64 bHighestBit;
    *div = NA_ZERO_u64;
    *rem = a;
    if(naEqualu64(b, NA_ZERO_u64)) {
      #if NA_DEBUG
        naCrash("Integer Division by 0");
      #endif
    }else if(naSmalleru64(a, b)) {
      // b is larger than a and hence the result is zero.
      // Do nothing here and just return with the values set above.
    }else{
      int shiftCount;

      // search for the highest bit of b.
      aHighestBit = naMakeu64(NA_SIGN_MASK_32, NA_ZERO_u32);
      while(!naEqualu64(naAndu64(a, aHighestBit), aHighestBit)) {
        aHighestBit = naShru64(aHighestBit, 1);
      }
      bHighestBit = naMakeu64(NA_SIGN_MASK_32, NA_ZERO_u32);
      while(!naEqualu64(naAndu64(b, bHighestBit), bHighestBit)) {
        bHighestBit = naShru64(bHighestBit, 1);
      }

      bTmp = b;
      shiftCount = 0;
      // Make the dividend big enough
      while(!naEqualu64(aHighestBit, bHighestBit)) {
        if(naEqualu64(bTmp, NA_ZERO_u64)) {
          // b is larger than a and hence the result is zero.
          #if NA_DEBUG
            naError("This should not happen.");
          #endif
        }
        bTmp = naShlu64(bTmp, 1);
        bHighestBit = naShlu64(bHighestBit, 1);
        shiftCount++;
      }

      while(shiftCount >= 0) {
        *div = naShlu64(*div, 1);
        if(naGreaterEqualu64(*rem, bTmp)) {
          *div = naOru64(*div, NA_ONE_u64);
          *rem = naSubu64(*rem, bTmp);
        }
        bTmp = naShru64(bTmp, 1);
        shiftCount--;
      }
    }
  }
  NA_IDEF NAu64 naDivu64(NAu64 a, NAu64 b) {
    NAu64 divInt;
    NAu64 remInt;
    na_Computeu64Division(a, b, &divInt, &remInt);
    return divInt;
  }
  NA_IDEF NAu64 naModu64(NAu64 a, NAu64 b) {
    NAu64 divInt;
    NAu64 remInt;
    na_Computeu64Division(a, b, &divInt, &remInt);
    return remInt;
  }



  NA_IDEF NAu64  naNotu64(NAu64 i) {
    NAu64 retValuei;
    retValuei.hi = ~i.hi;
    retValuei.lo = ~i.lo;
    return retValuei;
  }
  NA_IDEF NAu64  naOru64 (NAu64 a, NAu64 b) {
    NAu64 retValuei;
    retValuei.hi = a.hi | b.hi;
    retValuei.lo = a.lo | b.lo;
    return retValuei;
  }
  NA_IDEF NAu64  naAndu64(NAu64 a, NAu64 b) {
    NAu64 retValuei;
    retValuei.hi = a.hi & b.hi;
    retValuei.lo = a.lo & b.lo;
    return retValuei;
  }
  NA_IDEF NAu64  naXoru64(NAu64 a, NAu64 b) {
    NAu64 retValuei;
    retValuei.hi = a.hi ^ b.hi;
    retValuei.lo = a.lo ^ b.lo;
    return retValuei;
  }
  NA_IDEF NAu64 naShlu64(NAu64 a, int n) {
    NAu64 retValuei;
    if(n < 0) {
      retValuei = naShru64(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 32) {
        retValuei.hi = a.hi << n;
        retValuei.hi |= a.lo >> (32 - n);
        retValuei.lo = a.lo << n;
      }else{
        retValuei.hi = a.lo << (n - 32);
        retValuei.lo = NA_ZERO_u32;
      }
    }
    return retValuei;
  }
  NA_IDEF NAu64 naShru64(NAu64 a, int n) {
    NAu64 retValuei;
    if(n < 0) {
      retValuei = naShlu64(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 32) {
        retValuei.lo = a.lo >> n;
        retValuei.lo |= a.hi << (32 - n);
        retValuei.hi = a.hi >> n;
      }else{
        retValuei.lo = a.hi >> (n - 32);
        retValuei.hi = NA_ZERO_u32;
      }
    }
    return retValuei;
  }



  NA_IDEF NABool naEqualu64(NAu64 a, NAu64 b) {
    return ((a.hi == b.hi) && (a.lo == b.lo));
  }
  NA_IDEF NABool naGreateru64(NAu64 a, NAu64 b) {
    return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo > b.lo)));
  }
  NA_IDEF NABool naGreaterEqualu64(NAu64 a, NAu64 b) {
    return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo >= b.lo)));
  }
  NA_IDEF NABool naSmalleru64(NAu64 a, NAu64 b) {
    return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo < b.lo)));
  }
  NA_IDEF NABool naSmallerEqualu64(NAu64 a, NAu64 b) {
    return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo <= b.lo)));
  }



  NA_IDEF int8 naCastu64Toi8(NAu64 i) {
    return (int8)i.lo;
  }
  NA_IDEF int16 naCastu64Toi16(NAu64 i) {
    return (int16)i.lo;
  }
  NA_IDEF int32 naCastu64Toi32(NAu64 i) {
    return (int32)i.lo;
  }
  NA_IDEF NAi64 naCastu64Toi64(NAu64 i) {
    NAi64 retValuei;
    retValuei.hi = (int32)i.hi;
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF uint8 naCastu64Tou8(NAu64 i) {
    return (uint8)i.lo;
  }
  NA_IDEF uint16 naCastu64Tou16(NAu64 i) {
    return (uint16)i.lo;
  }
  NA_IDEF uint32 naCastu64Tou32(NAu64 i) {
    return i.lo;
  }
  NA_IDEF double naCastu64ToDouble(NAu64 i) {
    return (double)i.hi * naMakeDoubleWithExponent(32) + (double)i.lo;
  }

#endif // defined NA_TYPE_INT64



#endif // NA_INT_64_II_INCLUDED



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
