
#if defined NA_INT_128_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif

#ifndef NA_INT_128_II_INCLUDED
#define NA_INT_128_II_INCLUDED



// first, undefine all things which were defined in the .h file.
#undef naMakei128
#undef naMakei128WithLo
#undef naMakei128WithDouble
#undef naMakei128WithBinary
#undef naGeti128Hi
#undef naGeti128Lo
#undef naGetu128Hi
#undef naGetu128Lo
#undef naNegi128
#undef naInci128
#undef naDeci128
#undef naAddi128
#undef naSubi128
#undef naMuli128
#undef naDivi128
#undef naModi128
#undef naNoti128
#undef naOri128
#undef naAndi128
#undef naXori128
#undef naShli128
#undef naShri128
#undef naEquali128
#undef naGreateri128
#undef naGreaterEquali128
#undef naSmalleri128
#undef naSmallerEquali128
#undef naCasti128Tou8
#undef naCasti128Tou16
#undef naCasti128Tou32
#undef naCasti128Tou64
#undef naCasti128Tou128
#undef naCasti128Toi8
#undef naCasti128Toi16
#undef naCasti128Toi32
#undef naCasti128Toi64
#undef naCasti128ToDouble
#undef naMakeu128
#undef naMakeu128WithLo
#undef naMakeu128WithDouble
#undef naMakeu128WithLiteralLo
#undef naMakeu128WithBinary
#undef naIncu128
#undef naDecu128
#undef naAddu128
#undef naSubu128
#undef naMulu128
#undef naDivu128
#undef naModu128
#undef naNotu128
#undef naOru128
#undef naAndu128
#undef naXoru128
#undef naShlu128
#undef naShru128
#undef naEqualu128
#undef naGreateru128
#undef naGreaterEqualu128
#undef naSmalleru128
#undef naSmallerEqualu128
#undef naCastu128Toi8
#undef naCastu128Toi16
#undef naCastu128Toi32
#undef naCastu128Toi64
#undef naCastu128Toi128
#undef naCastu128Tou8
#undef naCastu128Tou16
#undef naCastu128Tou32
#undef naCastu128Tou64
#undef naCastu128ToDouble



#if NA_NATIVE_INT128_IN_USE

  NA_IDEF int128 naMakei128(int64 hi, uint64 lo) {
    return (int128)((uint128)hi << 64) | lo;
  }
  NA_IDEF int128 naMakei128WithLo(int64 lo) {
    return (int128)lo;
  }
  NA_IDEF int128 naMakei128WithDouble(double lo) {
    return (int128)lo;
  }
  NA_IDEF int128 naMakei128WithBinary(uint32 b3, uint32 b2, uint32 b1, uint32 b0) {
    return ((int128)b3 << 96) | ((int128)b2 << 64) | ((int128)b1 << 32) | b0;
  }

  #define naGeti128Hi(i) ((int64)((i) >> NA_TYPE64_BITS))
  #define naGeti128Lo(i) ((uint64)i)
  #define naGetu128Hi(u) ((uint64)((u) >> NA_TYPE64_BITS))
  #define naGetu128Lo(u) ((uint64)u)

  #define naNegi128(i)    (-(i))
  #define naInci128(i)    (++(i))
  #define naDeci128(i)    (--(i))
  #define naAddi128(a, b) ((a) + (b))
  #define naSubi128(a, b) ((a) - (b))
  #define naMuli128(a, b) ((a) * (b))
  #define naDivi128(a, b) ((a) / (b))
  #define naModi128(a, b) ((a) % (b))

  #define naNoti128(i)    (~(i))
  #define naOri128(a, b)  ((a) | (b))
  #define naAndi128(a, b) ((a) & (b))
  #define naXori128(a, b) ((a) ^ (b))
  #define naShli128(a, n) ((int128)(((uint128)(a)) << (n)))
  #define naShri128(a, n) ((a) >> (n))

  #define naEquali128(a, b)        ((a) == (b))
  #define naGreateri128(a, b)      ((a) > (b))
  #define naGreaterEquali128(a, b) ((a) >= (b))
  #define naSmalleri128(a, b)      ((a) < (b))
  #define naSmallerEquali128(a, b) ((a) <= (b))

  #define naCasti128Tou8(i)     ((uint8)(i))
  #define naCasti128Tou16(i)    ((uint16)(i))
  #define naCasti128Tou32(i)    ((uint32)(i))
  #define naCasti128Tou64(i)    ((NAu64)(i))
  #define naCasti128Tou128(i)   ((NAu128)(i))
  #define naCasti128Toi8(i)     ((int8)(i))
  #define naCasti128Toi16(i)    ((int16)(i))
  #define naCasti128Toi32(i)    ((int32)(i))
  #define naCasti128Toi64(i)    ((NAi64)(i))
  #define naCasti128ToDouble(i) ((double)(i))

  #define naMakeu128(hi, lo) \
    ((NAu128)(((NAu128)(hi) << 64) | (lo)))
  #define naMakeu128WithLo(lo) \
    ((NAu128)(lo))
  #define naMakeu128WithDouble(d) \
    ((NAu128)(d))
  #define naMakeu128WithBinary(b3, b2, b1, b0) \
    naMakeu128(naMakeu64WithBinary(b3, b2), \
    naMakeu64WithBinary(b1, b0))

  #define naIncu128(i)    ((i)++)
  #define naDecu128(i)    ((i)--)
  #define naAddu128(a, b) ((a) + (b))
  #define naSubu128(a, b) ((a) - (b))
  #define naMulu128(a, b) ((a) * (b))
  #define naDivu128(a, b) ((a) / (b))
  #define naModu128(a, b) ((a) % (b))

  #define naNotu128(i)    (~(i))
  #define naOru128(a, b)  ((a) | (b))
  #define naAndu128(a, b) ((a) & (b))
  #define naXoru128(a, b) ((a) ^ (b))
  #define naShlu128(a, n) ((a) << (n))
  #define naShru128(a, n) ((a) >> (n))

  #define naEqualu128(a, b)        ((a) == (b))
  #define naGreateru128(a, b)      ((a) > (b))
  #define naGreaterEqualu128(a, b) ((a) >= (b))
  #define naSmalleru128(a, b)      ((a) < (b))
  #define naSmallerEqualu128(a, b) ((a) <= (b))

  #define naCastu128Toi8(i)     ((int8)(i))
  #define naCastu128Toi16(i)    ((int16)(i))
  #define naCastu128Toi32(i)    ((int32)(i))
  #define naCastu128Toi64(i)    ((NAi64)(i))
  #define naCastu128Toi128(i)   ((NAi128)(i))
  #define naCastu128Tou8(i)     ((uint8)(i))
  #define naCastu128Tou16(i)    ((uint16)(i))
  #define naCastu128Tou32(i)    ((uint32)(i))
  #define naCastu128Tou64(i)    ((NAu64)(i))
  #define naCastu128ToDouble(i) ((double)(i))

#else

  NA_IDEF NAi128 naMakei128(NAi64 hi, NAu64 lo) {
    NAi128 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAi128 naMakei128WithLo(NAi64 lo) {
    NAi128 retValuei;
    retValuei.hi = naCastu64Toi64(naGetSignum64(lo));
    retValuei.lo = naCasti64Tou64(lo);
    return retValuei;
  }
  NA_IDEF NAi128 naMakei128WithDouble(double d) {
    return naMakei128WithLo(naGetDoubleInteger(d));
  }
  NA_IDEF NAi128 naMakei128WithBinary(uint32 b3, uint32 b2, uint32 b1, uint32 b0) {
    NAi128 retValuei;
    retValuei.hi = naMakei64WithBinary(b3, b2);
    retValuei.lo = naMakeu64WithBinary(b1, b0);
    return retValuei;
  }

  
  
  #define naGeti128Hi(i) ((i).hi)
  #define naGeti128Lo(i) ((i).lo)
  
  
  
  #define naGetu128Hi(u) ((u).hi)
  #define naGetu128Lo(u) ((u).lo)



  #define naInci128(i)\
    ((i).hi = naAddi64((i).hi, naCastBoolToi64(naEqualu64((i).lo, NA_MAX_u64))),\
    naIncu64((i).lo),\
    (i))
  #define naDeci128(i)\
    ((i).hi = naSubi64((i).hi, naCastBoolToi64(naEqualu64((i).lo, NA_ZERO_u64))),\
    naDecu64((i).lo),\
    (i))

  NA_IDEF NAi128 naNegi128(NAi128 i) {
    NAi128 retValuei = naNoti128(i);
    naInci128(retValuei);
    return retValuei;
  }
  NA_IDEF NAi128 naAddi128(NAi128 a, NAi128 b) {
    return naCastu128Toi128(naAddu128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naSubi128(NAi128 a, NAi128 b) {
    return naCastu128Toi128(naSubu128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naMuli128(NAi128 a, NAi128 b) {
    NAi128 retValuei;
    NAu128 retValueu;
    NAi128 aSign = naSigni128(a);
    NAi128 bSign = naSigni128(b);
    if(naSmalleri128(a, NA_ZERO_i128)) {
      a = naNegi128(a);
    }
    if(naSmalleri128(b, NA_ZERO_i128)) {
      b = naNegi128(b);
    }
    retValueu = naMulu128(naCasti128Tou128(a), naCasti128Tou128(b));
    retValueu.hi = naAndu64(retValueu.hi, naNotu64(naCasti64Tou64(NA_SIGN_MASK_64)));
    retValuei = naCastu128Toi128(retValueu);
    if(!naEquali128(aSign, bSign)) {
      retValuei = naNegi128(retValuei);
    }
    // todo: overflow may lead to different result than built-in 128 bit integer
    return retValuei;
  }
  NA_IDEF NAi128 naDivi128(NAi128 a, NAi128 b) {
    NAi128 retValuei;
    NAu128 retValueu;
    NAi128 aSign = naSigni128(a);
    NAi128 bSign = naSigni128(b);
    if(naSmalleri128(a, NA_ZERO_i128)) {
      a = naNegi128(a);
    }
    if(naSmalleri128(b, NA_ZERO_i128)) {
      b = naNegi128(b);
    }
    retValueu = naDivu128(naCasti128Tou128(a), naCasti128Tou128(b));
    retValueu.hi = naAndu64(retValueu.hi, naNotu64(naCasti64Tou64(NA_SIGN_MASK_64)));
    retValuei = naCastu128Toi128(retValueu);
    if(!naEquali128(aSign, bSign)) {
      retValuei = naNegi128(retValuei);
    }
    return retValuei;
  }
  NA_IDEF NAi128 naModi128(NAi128 a, NAi128 b) {
    NAi128 retValuei;
    NAu128 retValueu;
    NAi128 aSign = naSigni128(a);
    if(naSmalleri128(a, NA_ZERO_i128)) {
      a = naNegi128(a);
    }
    if(naSmalleri128(b, NA_ZERO_i128)) {
      b = naNegi128(b);
    }
    retValueu = naModu128(naCasti128Tou128(a), naCasti128Tou128(b));
    retValueu.hi = naAndu64(retValueu.hi, naNotu64(naCasti64Tou64(NA_SIGN_MASK_64)));
    retValuei = naCastu128Toi128(retValueu);
    if(!naEquali128(aSign, NA_ONE_i128)) {
      retValuei = naNegi128(retValuei);
    }
    return retValuei;
  }



  NA_IDEF NAi128 naNoti128(NAi128 i) {
    return naCastu128Toi128(naNotu128(naCasti128Tou128(i)));
  }
  NA_IDEF NAi128 naOri128(NAi128 a, NAi128 b) {
    return naCastu128Toi128(naOru128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naAndi128(NAi128 a, NAi128 b) {
    return naCastu128Toi128(naAndu128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naXori128(NAi128 a, NAi128 b) {
    return naCastu128Toi128(naXoru128(naCasti128Tou128(a), naCasti128Tou128(b)));
  }
  NA_IDEF NAi128 naShli128(NAi128 a, int n) {
    return naCastu128Toi128(naShlu128(naCasti128Tou128(a), n));
  }
  NA_IDEF NAi128 naShri128(NAi128 a, int n) {
    NAi128 retValuei;
    if(n < 0) {
      retValuei = naShli128(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 64) {
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



  NA_IDEF NABool naEquali128(NAi128 a, NAi128 b) {
    return naEqualu128(naCasti128Tou128(a), naCasti128Tou128(b));
  }
  NA_IDEF NABool naGreateri128(NAi128 a, NAi128 b) {
    return (naGreateri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naSmalleru64(a.lo, b.lo) : naGreateru64(a.lo, b.lo))));
  }
  NA_IDEF NABool naGreaterEquali128(NAi128 a, NAi128 b) {
    return (naGreateri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naSmallerEqualu64(a.lo, b.lo) : naGreaterEqualu64(a.lo, b.lo))));
  }
  NA_IDEF NABool naSmalleri128(NAi128 a, NAi128 b) {
    return (naSmalleri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naGreateru64(a.lo, b.lo) : naSmalleru64(a.lo, b.lo))));
  }
  NA_IDEF NABool naSmallerEquali128(NAi128 a, NAi128 b) {
    return (naSmalleri64(a.hi, b.hi)
      || (naEquali64(a.hi, b.hi)
      && (naSmalleri64(a.hi, NA_ZERO_i64) ? naGreaterEqualu64(a.lo, b.lo) : naSmallerEqualu64(a.lo, b.lo))));
  }



  NA_IDEF uint8 naCasti128Tou8(NAi128 i) {
    return naCastu128Tou8(naCasti128Tou128(i));
  }
  NA_IDEF uint16 naCasti128Tou16(NAi128 i) {
    return naCastu128Tou16(naCasti128Tou128(i));
  }
  NA_IDEF uint32 naCasti128Tou32(NAi128 i) {
    return naCastu128Tou32(naCasti128Tou128(i));
  }
  NA_IDEF NAu64 naCasti128Tou64(NAi128 i) {
    return naCastu128Tou64(naCasti128Tou128(i));
  }
  NA_IDEF NAu128 naCasti128Tou128(NAi128 i) {
    NAu128 retValuei;
    retValuei.hi = naCasti64Tou64(i.hi);
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF int8 naCasti128Toi8(NAi128 i) {
    return naCastu128Toi8(naCasti128Tou128(i));
  }
  NA_IDEF int16 naCasti128Toi16(NAi128 i) {
    return naCastu128Toi16(naCasti128Tou128(i));
  }
  NA_IDEF int32 naCasti128Toi32(NAi128 i) {
    return naCastu128Toi32(naCasti128Tou128(i));
  }
  NA_IDEF NAi64 naCasti128Toi64(NAi128 i) {
    return naCastu128Toi64(naCasti128Tou128(i));
  }
  NA_IDEF double naCasti128ToDouble(NAi128 i) {
    return naCasti64ToDouble(naCasti128Toi64(i));
  }



  NA_IDEF NAu128 naMakeu128(NAu64 hi, NAu64 lo) {
    NAu128 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu128 naMakeu128WithLo(NAu64 lo) {
    NAu128 retValuei;
    retValuei.hi = NA_ZERO_u64;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu128 naMakeu128WithDouble(double d) {
    return naMakeu128WithLo(naMakeu64WithDouble(d));
  }

  NA_IDEF NAu128 naMakeu128WithBinary(uint32 b3, uint32 b2, uint32 b1, uint32 b0) {
    NAu128 retValueu;
    retValueu.hi = naMakeu64WithBinary(b3, b2);
    retValueu.lo = naMakeu64WithBinary(b1, b0);
    return retValueu;
  }



  #define naIncu128(u)\
    ((u).hi = naAddu64((u).hi, naMakeu64WithLo(naEqualu64((u).lo, NA_MAX_u64))),\
    naIncu64((u).lo),\
    (u))
  #define naDecu128(u)\
    ((u).hi = naSubu64((u).hi, naMakeu64WithLo(naEqualu64((u).lo, NA_ZERO_u64))),\
    naDecu64((u).lo),\
    (u))



  NA_IDEF NAu128 naAddu128(NAu128 a, NAu128 b) {
    NAu128 retValuei;
    retValuei.lo = naAddu64(a.lo, b.lo);
    retValuei.hi = naAddu64(a.hi, b.hi);
    retValuei.hi = naAddu64(retValuei.hi, naMakeu64WithLo(naSmalleru64(retValuei.lo, a.lo))); // add a carry if there was an overflow.
    return retValuei;
  }
  NA_IDEF NAu128 naSubu128(NAu128 a, NAu128 b) {
    return naAddu128(a, naCasti128Tou128(naNegi128(naCastu128Toi128(b))));
  }
  NA_IDEF NAu128 naMulu128(NAu128 a, NAu128 b) {
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
    retValuei.hi = naAddu64(retValuei.hi, naMakeu64WithLo(naSmalleru64(retValuei.lo, shla0b1)));
    retValuei.hi = naAddu64(retValuei.hi, naShru64(a0b1, 32));
    retValuei.hi = naAddu64(retValuei.hi, naMulu64(a0, b2));
    retValuei.hi = naAddu64(retValuei.hi, naShlu64(naMulu64(a0, b3), 32));

    // multiply a1 * b and add up
    NAu64 shla1b0 = naShlu64(a1b0, 32);
    retValuei.lo = naAddu64(retValuei.lo, shla1b0);
    retValuei.hi = naAddu64(retValuei.hi, naMakeu64WithLo(naSmalleru64(retValuei.lo, shla1b0)));
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
  NA_HIDEF void na_Computeu128Division(NAu128 a, NAu128 b, NAu128* div, NAu128* rem) {
    NAu128 bTmp;
    NAu128 aHighestBit;
    NAu128 bHighestBit;
    *div = NA_ZERO_u128;
    *rem = a;
    if(naEqualu128(b, NA_ZERO_u128)) {
      #if NA_DEBUG
        naCrash("Integer Division by 0");
      #endif
    }else if(naSmalleru128(a, b)) {
      // b is larger than a and hence the result is zero.
      // Do nothing here and just return with the values set above.
    }else{
      int shiftCount;

      // search for the highest bit of b.
      aHighestBit = naMakeu128(naCasti64Tou64(NA_SIGN_MASK_64), NA_ZERO_u64);
      while(!naEqualu128(naAndu128(a, aHighestBit), aHighestBit)) {
        aHighestBit = naShru128(aHighestBit, 1);
      }
      bHighestBit = naMakeu128(naCasti64Tou64(NA_SIGN_MASK_64), NA_ZERO_u64);
      while(!naEqualu128(naAndu128(b, bHighestBit), bHighestBit)) {
        bHighestBit = naShru128(bHighestBit, 1);
      }

      bTmp = b;
      shiftCount = 0;
      // Make the dividend big enough
      while(!naEqualu128(aHighestBit, bHighestBit)) {
        if(naEqualu128(bTmp, NA_ZERO_u128)) {
          // b is larger than a and hence the result is zero.
          #if NA_DEBUG
            naError("This should not happen.");
          #endif
        }
        bTmp = naShlu128(bTmp, 1);
        bHighestBit = naShlu128(bHighestBit, 1);
        shiftCount++;
      }

      while(shiftCount >= 0) {
        *div = naShlu128(*div, 1);
        if(naGreaterEqualu128(*rem, bTmp)) {
          *div = naOru128(*div, NA_ONE_u128);
          *rem = naSubu128(*rem, bTmp);
        }
        bTmp = naShru128(bTmp, 1);
        shiftCount--;
      }
    }
  }
  NA_IDEF NAu128 naDivu128(NAu128 a, NAu128 b) {
    NAu128 divInt;
    NAu128 remInt;
    na_Computeu128Division(a, b, &divInt, &remInt);
    return divInt;
  }
  NA_IDEF NAu128 naModu128(NAu128 a, NAu128 b) {
    NAu128 divInt;
    NAu128 remInt;
    na_Computeu128Division(a, b, &divInt, &remInt);
    return remInt;
  }



  NA_IDEF NAu128  naNotu128(NAu128 i) {
    NAu128 retValuei;
    retValuei.hi = naNotu64(i.hi);
    retValuei.lo = naNotu64(i.lo);
    return retValuei;
  }
  NA_IDEF NAu128  naOru128 (NAu128 a, NAu128 b) {
    NAu128 retValuei;
    retValuei.hi = naOru64(a.hi, b.hi);
    retValuei.lo = naOru64(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu128  naAndu128(NAu128 a, NAu128 b) {
    NAu128 retValuei;
    retValuei.hi = naAndu64(a.hi, b.hi);
    retValuei.lo = naAndu64(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu128  naXoru128(NAu128 a, NAu128 b) {
    NAu128 retValuei;
    retValuei.hi = naXoru64(a.hi, b.hi);
    retValuei.lo = naXoru64(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu128 naShlu128(NAu128 a, int n) {
    NAu128 retValuei;
    if(n < 0) {
      retValuei = naShru128(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 64) {
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
  NA_IDEF NAu128 naShru128(NAu128 a, int n) {
    NAu128 retValuei;
    if(n < 0) {
      retValuei = naShlu128(a, -n);
    }else{
    // Beware, do not use <= as some processors will result
    // in garbage when the shift is equal to the type size.
      if(n < 64) {
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



  NA_IDEF NABool naEqualu128(NAu128 a, NAu128 b) {
    return (naEqualu64(a.hi, b.hi) && naEqualu64(a.lo, b.lo));
  }
  NA_IDEF NABool naGreateru128(NAu128 a, NAu128 b) {
    return (naGreateru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naGreateru64(a.lo, b.lo)));
  }
  NA_IDEF NABool naGreaterEqualu128(NAu128 a, NAu128 b) {
    return (naGreateru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naGreaterEqualu64(a.lo, b.lo)));
  }
  NA_IDEF NABool naSmalleru128(NAu128 a, NAu128 b) {
    return (naSmalleru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naSmalleru64(a.lo, b.lo)));
  }
  NA_IDEF NABool naSmallerEqualu128(NAu128 a, NAu128 b) {
    return (naSmalleru64(a.hi, b.hi) || (naEqualu64(a.hi, b.hi) && naSmallerEqualu64(a.lo, b.lo)));
  }



  NA_IDEF int8 naCastu128Toi8(NAu128 i) {
    return naCastu64Toi8(i.lo);
  }
  NA_IDEF int16 naCastu128Toi16(NAu128 i) {
    return naCastu64Toi16(i.lo);
  }
  NA_IDEF int32 naCastu128Toi32(NAu128 i) {
    return naCastu64Toi32(i.lo);
  }
  NA_IDEF NAi64 naCastu128Toi64(NAu128 i) {
    return naCastu64Toi64(i.lo);
  }
  NA_IDEF NAi128 naCastu128Toi128(NAu128 i) {
    NAi128 retValuei;
    retValuei.hi = naCastu64Toi64(i.hi);
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF uint8 naCastu128Tou8(NAu128 i) {
    return naCastu64Tou8(i.lo);
  }
  NA_IDEF uint16 naCastu128Tou16(NAu128 i) {
    return naCastu64Tou16(i.lo);
  }
  NA_IDEF uint32 naCastu128Tou32(NAu128 i) {
    return naCastu64Tou32(i.lo);
  }
  NA_IDEF NAu64 naCastu128Tou64(NAu128 i) {
    return i.lo;
  }
  NA_IDEF double naCastu128ToDouble(NAu128 i) {
    return naCastu64ToDouble(i.hi) * naMakeDoubleWithExponent(64) + naCastu64ToDouble(i.lo);
  }

#endif // NA_NATIVE_INT128_IN_USE

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
