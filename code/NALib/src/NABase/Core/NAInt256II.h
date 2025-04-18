
#if defined NA_INT_256_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_256_II_INCLUDED
#define NA_INT_256_II_INCLUDED



// first, undefine all things which were defined in the .h file.
#undef naMakei256
#undef naMakei256WithLo
#undef naMakei256WithDouble
#undef naMakei256WithBinary
#undef naGeti256Hi
#undef naGeti256Lo
#undef naGetu256Hi
#undef naGetu256Lo
#undef naNegi256
#undef naInci256
#undef naDeci256
#undef naAddi256
#undef naSubi256
#undef naMuli256
#undef naDivi256
#undef naModi256
#undef naNoti256
#undef naOri256
#undef naAndi256
#undef naXori256
#undef naShli256
#undef naShri256
#undef naEquali256
#undef naGreateri256
#undef naGreaterEquali256
#undef naSmalleri256
#undef naSmallerEquali256
#undef naCasti256Tou8
#undef naCasti256Tou16
#undef naCasti256Tou32
#undef naCasti256Tou64
#undef naCasti256Tou128
#undef naCasti256Tou256
#undef naCasti256Toi8
#undef naCasti256Toi16
#undef naCasti256Toi32
#undef naCasti256Toi64
#undef naCasti256Toi128
#undef naCasti256ToDouble
#undef naMakeu256
#undef naMakeu256WithLo
#undef naMakeu256WithDouble
#undef naMakeu256WithLiteralLo
#undef naMakeu256WithBinary
#undef naIncu256
#undef naDecu256
#undef naAddu256
#undef naSubu256
#undef naMulu256
#undef naDivu256
#undef naModu256
#undef naNotu256
#undef naOru256
#undef naAndu256
#undef naXoru256
#undef naShlu256
#undef naShru256
#undef naEqualu256
#undef naGreateru256
#undef naGreaterEqualu256
#undef naSmalleru256
#undef naSmallerEqualu256
#undef naCastu256Toi8
#undef naCastu256Toi16
#undef naCastu256Toi32
#undef naCastu256Toi64
#undef naCastu256Toi128
#undef naCastu256Toi256
#undef naCastu256Tou8
#undef naCastu256Tou16
#undef naCastu256Tou32
#undef naCastu256Tou64
#undef naCastu256Tou128
#undef naCastu256ToDouble


#if NA_NATIVE_INT256_IN_USE

  NA_IDEF int256 naMakei256(int128 hi, uint128 lo) {
    return (int256)((uint256)hi << 128) | lo;
  }
  NA_IDEF int256 naMakei256WithLo(int128 lo) {
    return (int256)lo;
  }
  NA_IDEF int256 naMakei256WithDouble(double lo) {
    return (int256)lo;
  }
  NA_IDEF int256 naMakei256WithBinary(uint32 b7, uint32 b6, uint32 b5, uint32 b4, uint32 b3, uint32 b2, uint32 b1, uint32 b0) {
    return ((int256)b3 << 224) | (int256)b3 << 192) | (int256)b3 << 160) | (int256)b3 << 128) | (int256)b3 << 96) | ((int256)b2 << 64) | ((int256)b1 << 32) | b0;
  }
  
  #define naGeti256Hi(i) ((int128)((i) >> NA_TYPE128_BITS))
  #define naGeti256Lo(i) ((uint128)i)
  #define naGetu256Hi(u) ((uint128)((u) >> NA_TYPE128_BITS))
  #define naGetu256Lo(u) ((uint128)u)

  #define naNegi256(i)    (-(i))
  #define naInci256(i)    (++(i))
  #define naDeci256(i)    (--(i))
  #define naAddi256(a, b) ((a) + (b))
  #define naSubi256(a, b) ((a) - (b))
  #define naMuli256(a, b) ((a) * (b))
  #define naDivi256(a, b) ((a) / (b))
  #define naModi256(a, b) ((a) % (b))

  #define naNoti256(i)    (~(i))
  #define naOri256(a, b)  ((a) | (b))
  #define naAndi256(a, b) ((a) & (b))
  #define naXori256(a, b) ((a) ^ (b))
  #define naShli256(a, n) ((int256)(((uint256)(a)) << (n)))
  #define naShri256(a, n) ((a) >> (n))

  #define naEquali256(a, b)        ((a) == (b))
  #define naGreateri256(a, b)      ((a) > (b))
  #define naGreaterEquali256(a, b) ((a) >= (b))
  #define naSmalleri256(a, b)      ((a) < (b))
  #define naSmallerEquali256(a, b) ((a) <= (b))

  #define naCasti256Tou8(i)     ((uint8)(i))
  #define naCasti256Tou16(i)    ((uint16)(i))
  #define naCasti256Tou32(i)    ((uint32)(i))
  #define naCasti256Tou64(i)    ((NAu64)(i))
  #define naCasti256Tou128(i)   ((NAu128)(i))
  #define naCasti256Tou256(i)   ((NAu256)(i))
  #define naCasti256Toi8(i)     ((int8)(i))
  #define naCasti256Toi16(i)    ((int16)(i))
  #define naCasti256Toi32(i)    ((int32)(i))
  #define naCasti256Toi64(i)    ((NAi64)(i))
  #define naCasti256Toi128(i)   ((NAi128)(i))
  #define naCasti256ToDouble(i) ((double)(i))

  #define naMakeu256(hi, lo) \
    ((NAu256)(((NAu256)(hi) << 128) | (lo)))
  #define naMakeu256WithLo(lo) \
    ((NAu256)(lo))
  #define naMakeu256WithDouble(d) \
    ((NAu256)(d))
  #define naMakeu256WithBinary(b7, b6, b5, b4, b3, b2, b1, b0) \
    naMakeu256(naMakeu128WithBinary(b7, b6, b5, b4), \
    naMakeu128WithBinary(b3, b2, b1, b0))

  #define naIncu256(i)    ((i)++)
  #define naDecu256(i)    ((i)--)
  #define naAddu256(a, b) ((a) + (b))
  #define naSubu256(a, b) ((a) - (b))
  #define naMulu256(a, b) ((a) * (b))
  #define naDivu256(a, b) ((a) / (b))
  #define naModu256(a, b) ((a) % (b))

  #define naNotu256(i)    (~(i))
  #define naOru256(a, b)  ((a) | (b))
  #define naAndu256(a, b) ((a) & (b))
  #define naXoru256(a, b) ((a) ^ (b))
  #define naShlu256(a, n) ((a) << (n))
  #define naShru256(a, n) ((a) >> (n))

  #define naEqualu256(a, b)        ((a) == (b))
  #define naGreateru256(a, b)      ((a) > (b))
  #define naGreaterEqualu256(a, b) ((a) >= (b))
  #define naSmalleru256(a, b)      ((a) < (b))
  #define naSmallerEqualu256(a, b) ((a) <= (b))

  #define naCastu256Toi8(i)     ((int8)(i))
  #define naCastu256Toi16(i)    ((int16)(i))
  #define naCastu256Toi32(i)    ((int32)(i))
  #define naCastu256Toi64(i)    ((NAi64)(i))
  #define naCastu256Toi128(i)   ((NAi128)(i))
  #define naCastu256Toi256(i)   ((NAi256)(i))
  #define naCastu256Tou8(i)     ((uint8)(i))
  #define naCastu256Tou16(i)    ((uint16)(i))
  #define naCastu256Tou32(i)    ((uint32)(i))
  #define naCastu256Tou64(i)    ((NAu64)(i))
  #define naCastu256Tou128(i)   ((NAu128)(i))
  #define naCastu256ToDouble(i) ((double)(i))

#else

  NA_IDEF NAi256 naMakei256(NAi128 hi, NAu128 lo) {
    NAi256 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAi256 naMakei256WithLo(NAi128 lo) {
    NAi256 retValuei;
    retValuei.hi = naCastu128Toi128(naGetSignum128(lo));
    retValuei.lo = naCasti128Tou128(lo);
    return retValuei;
  }
  NA_IDEF NAi256 naMakei256WithDouble(double d) {
    return naMakei256WithLo(naMakei128WithLo(naGetDoubleInteger(d)));
  }
  NA_IDEF NAi256 naMakei256WithBinary(uint32 b7, uint32 b6, uint32 b5, uint32 b4, uint32 b3, uint32 b2, uint32 b1, uint32 b0) {
    NAi256 retValuei;
    retValuei.hi = naMakei128WithBinary(b7, b6, b5, b4);
    retValuei.lo = naMakeu128WithBinary(b3, b2, b1, b0);
    return retValuei;
  }



  #undef naInci256
  #define naInci256(i)\
    (i.hi = naAddi128(i.hi, naMakei128WithLo(naMakei64WithLo(naEqualu128(i.lo, NA_MAX_u128)))),\
    naIncu128(i.lo),\
    i)
  #undef naDeci256
  #define naDeci256(i)\
    (i.hi = naSubi128(i.hi, naMakei128WithLo(naMakei64WithLo(naEqualu128(i.lo, NA_ZERO_u128)))),\
    naDecu128(i.lo),\
    i)



  NA_IDEF NAi256 naNegi256(NAi256 i) {
    NAi256 retValuei = naNoti256(i);
    naInci256(retValuei);
    return retValuei;
  }
  NA_IDEF NAi256 naAddi256(NAi256 a, NAi256 b) {
    return naCastu256Toi256(naAddu256(naCasti256Tou256(a), naCasti256Tou256(b)));
  }
  NA_IDEF NAi256 naSubi256(NAi256 a, NAi256 b) {
    return naCastu256Toi256(naSubu256(naCasti256Tou256(a), naCasti256Tou256(b)));
  }
  NA_IDEF NAi256 naMuli256(NAi256 a, NAi256 b) {
    NAi256 retValuei;
    NAu256 retValueu;
    NAi256 aSign = naSigni256(a);
    NAi256 bSign = naSigni256(b);
    if(naSmalleri256(a, NA_ZERO_i256)) {
      a = naNegi256(a);
    }
    if(naSmalleri256(b, NA_ZERO_i256)) {
      b = naNegi256(b);
    }
    retValueu = naMulu256(naCasti256Tou256(a), naCasti256Tou256(b));
    retValueu.hi = naAndu128(retValueu.hi, naNotu128(naCasti128Tou128(NA_SIGN_MASK_128)));
    retValuei = naCastu256Toi256(retValueu);
    if(!naEquali256(aSign, bSign)) {
      retValuei = naNegi256(retValuei);
    }
    // todo: overflow may lead to different result than built-in 256 bit integer
    return retValuei;
  }
  NA_IDEF NAi256 naDivi256(NAi256 a, NAi256 b) {
    NAi256 retValuei;
    NAu256 retValueu;
    NAi256 aSign = naSigni256(a);
    NAi256 bSign = naSigni256(b);
    if(naSmalleri256(a, NA_ZERO_i256)) {
      a = naNegi256(a);
    }
    if(naSmalleri256(b, NA_ZERO_i256)) {
      b = naNegi256(b);
    }
    retValueu = naDivu256(naCasti256Tou256(a), naCasti256Tou256(b));
    retValueu.hi = naAndu128(retValueu.hi, naNotu128(naCasti128Tou128(NA_SIGN_MASK_128)));
    retValuei = naCastu256Toi256(retValueu);
    if(!naEquali256(aSign, bSign)) {
      retValuei = naNegi256(retValuei);
    }
    return retValuei;
  }
  NA_IDEF NAi256 naModi256(NAi256 a, NAi256 b) {
    NAi256 retValuei;
    NAu256 retValueu;
    NAi256 aSign = naSigni256(a);
    if(naSmalleri256(a, NA_ZERO_i256)) {
      a = naNegi256(a);
    }
    if(naSmalleri256(b, NA_ZERO_i256)) {
      b = naNegi256(b);
    }
    retValueu = naModu256(naCasti256Tou256(a), naCasti256Tou256(b));
    retValueu.hi = naAndu128(retValueu.hi, naNotu128(naCasti128Tou128(NA_SIGN_MASK_128)));
    retValuei = naCastu256Toi256(retValueu);
    if(!naEquali256(aSign, NA_ONE_i256)) {
      retValuei = naNegi256(retValuei);
    }
    return retValuei;
  }



  NA_IDEF NAi256 naNoti256(NAi256 i) {
    return naCastu256Toi256(naNotu256(naCasti256Tou256(i)));
  }
  NA_IDEF NAi256 naOri256(NAi256 a, NAi256 b) {
    return naCastu256Toi256(naOru256(naCasti256Tou256(a), naCasti256Tou256(b)));
  }
  NA_IDEF NAi256 naAndi256(NAi256 a, NAi256 b) {
    return naCastu256Toi256(naAndu256(naCasti256Tou256(a), naCasti256Tou256(b)));
  }
  NA_IDEF NAi256 naXori256(NAi256 a, NAi256 b) {
    return naCastu256Toi256(naXoru256(naCasti256Tou256(a), naCasti256Tou256(b)));
  }
  NA_IDEF NAi256 naShli256(NAi256 a, int n) {
    return naCastu256Toi256(naShlu256(naCasti256Tou256(a), n));
  }
  NA_IDEF NAi256 naShri256(NAi256 a, int n) {
    NAi256 retValuei;
    if(n < 0) {
      retValuei = naShli256(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 128) {
        retValuei.lo = naShru128(a.lo, n);
        retValuei.lo = naOru128(retValuei.lo, naShlu128(naCasti128Tou128(a.hi), (128 - n)));
        retValuei.hi = naShri128(a.hi, n);
      }else{
        NAu128 signum = naGetSignum128(a.hi); // Sign preservation!
        retValuei.lo = naCasti128Tou128(naShri128(a.hi, (n - 128)));
        retValuei.hi = naCastu128Toi128(signum);
      }
    }
    return retValuei;
  }



  NA_IDEF NABool naEquali256(NAi256 a, NAi256 b) {
    return naEqualu256(naCasti256Tou256(a), naCasti256Tou256(b));
  }
  NA_IDEF NABool naGreateri256(NAi256 a, NAi256 b) {
    return (naGreateri128(a.hi, b.hi)
      || (naEquali128(a.hi, b.hi)
      && (naSmalleri128(a.hi, NA_ZERO_i128) ? naSmalleru128(a.lo, b.lo) : naGreateru128(a.lo, b.lo))));
  }
  NA_IDEF NABool naGreaterEquali256(NAi256 a, NAi256 b) {
    return (naGreateri128(a.hi, b.hi)
      || (naEquali128(a.hi, b.hi)
      && (naSmalleri128(a.hi, NA_ZERO_i128) ? naSmallerEqualu128(a.lo, b.lo) : naGreaterEqualu128(a.lo, b.lo))));
  }
  NA_IDEF NABool naSmalleri256(NAi256 a, NAi256 b) {
    return (naSmalleri128(a.hi, b.hi)
      || (naEquali128(a.hi, b.hi)
      && (naSmalleri128(a.hi, NA_ZERO_i128) ? naGreateru128(a.lo, b.lo) : naSmalleru128(a.lo, b.lo))));
  }
  NA_IDEF NABool naSmallerEquali256(NAi256 a, NAi256 b) {
    return (naSmalleri128(a.hi, b.hi)
      || (naEquali128(a.hi, b.hi)
      && (naSmalleri128(a.hi, NA_ZERO_i128) ? naGreaterEqualu128(a.lo, b.lo) : naSmallerEqualu128(a.lo, b.lo))));
  }



  NA_IDEF uint8 naCasti256Tou8(NAi256 i) {
    return naCastu256Tou8(naCasti256Tou256(i));
  }
  NA_IDEF uint16 naCasti256Tou16(NAi256 i) {
    return naCastu256Tou16(naCasti256Tou256(i));
  }
  NA_IDEF uint32 naCasti256Tou32(NAi256 i) {
    return naCastu256Tou32(naCasti256Tou256(i));
  }
  NA_IDEF NAu64 naCasti256Tou64(NAi256 i) {
    return naCastu256Tou64(naCasti256Tou256(i));
  }
  NA_IDEF NAu128 naCasti256Tou128(NAi256 i) {
    return naCastu256Tou128(naCasti256Tou256(i));
  }
  NA_IDEF NAu256 naCasti256Tou256(NAi256 i) {
    NAu256 retValuei;
    retValuei.hi = naCasti128Tou128(i.hi);
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF int8 naCasti256Toi8(NAi256 i) {
    return naCastu256Toi8(naCasti256Tou256(i));
  }
  NA_IDEF int16 naCasti256Toi16(NAi256 i) {
    return naCastu256Toi16(naCasti256Tou256(i));
  }
  NA_IDEF int32 naCasti256Toi32(NAi256 i) {
    return naCastu256Toi32(naCasti256Tou256(i));
  }
  NA_IDEF NAi64 naCasti256Toi64(NAi256 i) {
    return naCastu256Toi64(naCasti256Tou256(i));
  }
  NA_IDEF NAi128 naCasti256Toi128(NAi256 i) {
    return naCastu256Toi128(naCasti256Tou256(i));
  }
  NA_IDEF double naCasti256ToDouble(NAi256 i) {
    return naCasti128ToDouble(naCasti256Toi128(i));
  }



  NA_IDEF NAu256 naMakeu256(NAu128 hi, NAu128 lo) {
    NAu256 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu256 naMakeu256WithLo(NAu128 lo) {
    NAu256 retValuei;
    retValuei.hi = NA_ZERO_u128;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu256 naMakeu256WithDouble(double d) {
    return naMakeu256WithLo(naMakeu128WithDouble(d));
  }

  NA_IDEF NAu256 naMakeu256WithBinary(uint32 b7, uint32 b6, uint32 b5, uint32 b4, uint32 b3, uint32 b2, uint32 b1, uint32 b0) {
    NAu256 retValueu;
    retValueu.hi = naMakeu128WithBinary(b7, b6, b5, b4);
    retValueu.lo = naMakeu128WithBinary(b3, b2, b1, b0);
    return retValueu;
  }


  #define naGeti256Hi(i) (i.hi)
  #define naGeti256Lo(i) (i.lo)
  #define naGetu256Hi(u) (u.hi)
  #define naGetu256Lo(u) (u.lo)

  #define naIncu256(i)\
    (i.hi = naAddu128(i.hi, naMakeu128WithLo(naMakeu64WithLo(naEqualu128(i.lo, NA_MAX_u128)))),\
    naIncu128(i.lo),\
    i)
  #define naDecu256(i)\
    (i.hi = naSubu128(i.hi, naMakeu128WithLo(naMakeu64WithLo(naEqualu128(i.lo, NA_ZERO_u128)))),\
    naDecu128(i.lo),\
    i)



  NA_IDEF NAu256 naAddu256(NAu256 a, NAu256 b) {
    NAu256 retValuei;
    retValuei.lo = naAddu128(a.lo, b.lo);
    retValuei.hi = naAddu128(a.hi, b.hi);
    retValuei.hi = naAddu128(retValuei.hi, naMakeu128WithLo(naMakeu64WithLo(naSmalleru128(retValuei.lo, a.lo)))); // add a carry if there was an overflow.
    return retValuei;
  }
  NA_IDEF NAu256 naSubu256(NAu256 a, NAu256 b) {
    return naAddu256(a, naCasti256Tou256(naNegi256(naCastu256Toi256(b))));
  }
  NA_IDEF NAu256 naMulu256(NAu256 a, NAu256 b) {
    NAu256 retValuei = NA_ZERO_u256;

    NAu128 a0 = naAndu128(a.lo, naMakeu128WithLo(NA_MAX_u64));
    NAu128 a1 = naShru128(a.lo, 64);
    NAu128 a2 = naAndu128(a.hi, naMakeu128WithLo(NA_MAX_u64));
    NAu128 a3 = naShru128(a.hi, 64);
    NAu128 b0 = naAndu128(b.lo, naMakeu128WithLo(NA_MAX_u64));
    NAu128 b1 = naShru128(b.lo, 64);
    NAu128 b2 = naAndu128(b.hi, naMakeu128WithLo(NA_MAX_u64));
    NAu128 b3 = naShru128(b.hi, 64);
    NAu128 a0b1 = naMulu128(a0, b1);
    NAu128 a1b0 = naMulu128(a1, b0);

    // multiply a0 * b and add up
    retValuei.lo = naAddu128(retValuei.lo, naMulu128(a0, b0));
    NAu128 shla0b1 = naShlu128(a0b1, 64);
    retValuei.lo = naAddu128(retValuei.lo, shla0b1);
    retValuei.hi = naAddu128(retValuei.hi, naMakeu128WithLo(naMakeu64WithLo(naSmalleru128(retValuei.lo, shla0b1))));
    retValuei.hi = naAddu128(retValuei.hi, naShru128(a0b1, 64));
    retValuei.hi = naAddu128(retValuei.hi, naMulu128(a0, b2));
    retValuei.hi = naAddu128(retValuei.hi, naShlu128(naMulu128(a0, b3), 64));

    // multiply a1 * b and add up
    NAu128 shla1b0 = naShlu128(a1b0, 64);
    retValuei.lo = naAddu128(retValuei.lo, shla1b0);
    retValuei.hi = naAddu128(retValuei.hi, naMakeu128WithLo(naMakeu64WithLo(naSmalleru128(retValuei.lo, shla1b0))));
    retValuei.hi = naAddu128(retValuei.hi, naShru128(a1b0, 64));
    retValuei.hi = naAddu128(retValuei.hi, naMulu128(a1, b1));
    retValuei.hi = naAddu128(retValuei.hi, naShlu128(naMulu128(a1, b2), 64));

    // multiply a2 * b and add up
    retValuei.hi = naAddu128(retValuei.hi, naMulu128(a2, b0));
    retValuei.hi = naAddu128(retValuei.hi, naShlu128(naMulu128(a2, b1), 64));

    // multiply a3 * b and add up
    retValuei.hi = naAddu128(retValuei.hi, naShlu128(naMulu128(a3, b0), 64));

    return retValuei;
  }
  NA_HIDEF void na_Computeu256Division(NAu256 a, NAu256 b, NAu256* div, NAu256* rem) {
    NAu256 bTmp;
    NAu256 aHighestBit;
    NAu256 bHighestBit;
    *div = NA_ZERO_u256;
    *rem = a;
    if(naEqualu256(b, NA_ZERO_u256)) {
      #if NA_DEBUG
        naCrash("Integer Division by 0");
      #endif
    }else if(naSmalleru256(a, b)) {
      // b is larger than a and hence the result is zero.
      // Do nothing here and just return with the values set above.
    }else{
      int shiftCount;

      // search for the highest bit of b.
      aHighestBit = naMakeu256(naCasti128Tou128(NA_SIGN_MASK_128), NA_ZERO_u128);
      while(!naEqualu256(naAndu256(a, aHighestBit), aHighestBit)) {
        aHighestBit = naShru256(aHighestBit, 1);
      }
      bHighestBit = naMakeu256(naCasti128Tou128(NA_SIGN_MASK_128), NA_ZERO_u128);
      while(!naEqualu256(naAndu256(b, bHighestBit), bHighestBit)) {
        bHighestBit = naShru256(bHighestBit, 1);
      }

      bTmp = b;
      shiftCount = 0;
      // Make the dividend big enough
      while(!naEqualu256(aHighestBit, bHighestBit)) {
        if(naEqualu256(bTmp, NA_ZERO_u256)) {
          // b is larger than a and hence the result is zero.
          #if NA_DEBUG
            naError("This should not happen.");
          #endif
          return;
        }
        bTmp = naShlu256(bTmp, 1);
        bHighestBit = naShlu256(bHighestBit, 1);
        shiftCount++;
      }

      while(shiftCount >= 0) {
        *div = naShlu256(*div, 1);
        if(naGreaterEqualu256(*rem, bTmp)) {
          *div = naOru256(*div, NA_ONE_u256);
          *rem = naSubu256(*rem, bTmp);
        }
        bTmp = naShru256(bTmp, 1);
        shiftCount--;
      }
    }
  }
  NA_IDEF NAu256 naDivu256(NAu256 a, NAu256 b) {
    NAu256 divInt;
    NAu256 remInt;
    na_Computeu256Division(a, b, &divInt, &remInt);
    return divInt;
  }
  NA_IDEF NAu256 naModu256(NAu256 a, NAu256 b) {
    NAu256 divInt;
    NAu256 remInt;
    na_Computeu256Division(a, b, &divInt, &remInt);
    return remInt;
  }



  NA_IDEF NAu256  naNotu256(NAu256 i) {
    NAu256 retValuei;
    retValuei.hi = naNotu128(i.hi);
    retValuei.lo = naNotu128(i.lo);
    return retValuei;
  }
  NA_IDEF NAu256  naOru256 (NAu256 a, NAu256 b) {
    NAu256 retValuei;
    retValuei.hi = naOru128(a.hi, b.hi);
    retValuei.lo = naOru128(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu256  naAndu256(NAu256 a, NAu256 b) {
    NAu256 retValuei;
    retValuei.hi = naAndu128(a.hi, b.hi);
    retValuei.lo = naAndu128(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu256  naXoru256(NAu256 a, NAu256 b) {
    NAu256 retValuei;
    retValuei.hi = naXoru128(a.hi, b.hi);
    retValuei.lo = naXoru128(a.lo, b.lo);
    return retValuei;
  }
  NA_IDEF NAu256 naShlu256(NAu256 a, int n) {
    NAu256 retValuei;
    if(n < 0) {
      retValuei = naShru256(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 128) {
        retValuei.hi = naShlu128(a.hi, n);
        retValuei.hi = naOru128(retValuei.hi, naShru128(a.lo, (128 - n)));
        retValuei.lo = naShlu128(a.lo, n);
      }else{
        retValuei.hi = naShlu128(a.lo, (n - 128));
        retValuei.lo = NA_ZERO_u128;
      }
    }
    return retValuei;
  }
  NA_IDEF NAu256 naShru256(NAu256 a, int n) {
    NAu256 retValuei;
    if(n < 0) {
      retValuei = naShlu256(a, -n);
    }else{
    // Beware, do not use <= as some processors will result
    // in garbage when the shift is equal to the type size.

      if(n < 128) {
        retValuei.lo = naShru128(a.lo, n);
        retValuei.lo = naOru128(retValuei.lo, naShlu128(a.hi, (128 - n)));
        retValuei.hi = naShru128(a.hi, n);
      }else{
        retValuei.lo = naShru128(a.hi, (n - 128));
        retValuei.hi = NA_ZERO_u128;
      }
    }
    return retValuei;
  }



  NA_IDEF NABool naEqualu256(NAu256 a, NAu256 b) {
    return (naEqualu128(a.hi, b.hi) && naEqualu128(a.lo, b.lo));
  }
  NA_IDEF NABool naGreateru256(NAu256 a, NAu256 b) {
    return (naGreateru128(a.hi, b.hi) || (naEqualu128(a.hi, b.hi) && naGreateru128(a.lo, b.lo)));
  }
  NA_IDEF NABool naGreaterEqualu256(NAu256 a, NAu256 b) {
    return (naGreateru128(a.hi, b.hi) || (naEqualu128(a.hi, b.hi) && naGreaterEqualu128(a.lo, b.lo)));
  }
  NA_IDEF NABool naSmalleru256(NAu256 a, NAu256 b) {
    return (naSmalleru128(a.hi, b.hi) || (naEqualu128(a.hi, b.hi) && naSmalleru128(a.lo, b.lo)));
  }
  NA_IDEF NABool naSmallerEqualu256(NAu256 a, NAu256 b) {
    return (naSmalleru128(a.hi, b.hi) || (naEqualu128(a.hi, b.hi) && naSmallerEqualu128(a.lo, b.lo)));
  }



  NA_IDEF int8 naCastu256Toi8(NAu256 i) {
    return naCastu128Toi8(i.lo);
  }
  NA_IDEF int16 naCastu256Toi16(NAu256 i) {
    return naCastu128Toi16(i.lo);
  }
  NA_IDEF int32 naCastu256Toi32(NAu256 i) {
    return naCastu128Toi32(i.lo);
  }
  NA_IDEF int64 naCastu256Toi64(NAu256 i) {
    return naCastu128Toi64(i.lo);
  }
  NA_IDEF NAi128 naCastu256Toi128(NAu256 i) {
    return naCastu128Toi128(i.lo);
  }
  NA_IDEF NAi256 naCastu256Toi256(NAu256 i) {
    NAi256 retValuei;
    retValuei.hi = naCastu128Toi128(i.hi);
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF uint8 naCastu256Tou8(NAu256 i) {
    return naCastu128Tou8(i.lo);
  }
  NA_IDEF uint16 naCastu256Tou16(NAu256 i) {
    return naCastu128Tou16(i.lo);
  }
  NA_IDEF uint32 naCastu256Tou32(NAu256 i) {
    return naCastu128Tou32(i.lo);
  }
  NA_IDEF uint64 naCastu256Tou64(NAu256 i) {
    return naCastu128Tou64(i.lo);
  }
  NA_IDEF NAu128 naCastu256Tou128(NAu256 i) {
    return i.lo;
  }
  NA_IDEF double naCastu256ToDouble(NAu256 i) {
    return naCastu128ToDouble(i.hi) * naMakeDoubleWithExponent(128) + naCastu128ToDouble(i.lo);
  }

#endif // NA_NATIVE_INT256_IN_USE



#endif // NA_INT_256_II_INCLUDED



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
