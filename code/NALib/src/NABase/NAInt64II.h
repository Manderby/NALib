
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_INT_64_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_64_II_INCLUDED
#define NA_INT_64_II_INCLUDED


#if defined NA_TYPE_INT64
  NA_IDEF NAi64 naMakei64(int32 hi, uint32 lo){return ((NAi64)hi << 32) | lo;}
  NA_IDEF NAi64 naMakei64WithLo(int32 lo){return (NAi64)lo;}
  NA_IDEF NAi64 naMakei64WithDouble(double lo){return (NAi64)lo;}
#else

  NA_IDEF NAi64 naMakei64(int32 hi, uint32 lo){
    NAi64 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAi64 naMakei64WithLo(int32 lo){
    NAi64 retValuei;
    retValuei.hi = (int32)naGetSignum32(lo);
    retValuei.lo = (uint32)lo;
    return retValuei;
  }
  NA_IDEF NAi64 naMakei64WithDouble(double d){
    return naGetDoubleInteger(d);
  }



  #undef naInci64
  #define naInci64(i) (i.hi += (i.lo == NA_MAX_u32), i.lo++, i)
  #undef naDeci64
  #define naDeci64(i) (i.hi -= (i.lo == NA_ZERO_u32), i.lo--, i)


  NA_IDEF NAi64 naNegi64(NAi64 i){
    NAi64 retValuei = naNoti64(i);
    naInci64(retValuei);
    return retValuei;
  }
  NA_IDEF NAi64 naAddi64(NAi64 a, NAi64 b){
    return naCastu64Toi64(naAddu64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naSubi64(NAi64 a, NAi64 b){
    return naCastu64Toi64(naSubu64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naMuli64(NAi64 a, NAi64 b){
    NAi64 retValuei;
    NAu64 retValueu;
    NAi64 aSign = naSigni64(a);
    NAi64 bSign = naSigni64(b);
    if(naSmalleri64(a, NA_ZERO_i64)){a = naNegi64(a);}
    if(naSmalleri64(b, NA_ZERO_i64)){b = naNegi64(b);}
    retValueu = naMulu64(naCasti64Tou64(a), naCasti64Tou64(b));
    retValueu.hi &= ~NA_SIGN_MASK_32;
    retValuei = naCastu64Toi64(retValueu);
    if(!naEquali64(aSign, bSign)){retValuei = naNegi64(retValuei);}
    // todo: overflow may lead to different result than built-in 64 bit integer
    return retValuei;
  }
  NA_IDEF NAi64 naDivi64(NAi64 a, NAi64 b){
    NAi64 retValuei;
    NAu64 retValueu;
    NAi64 aSign = naSigni64(a);
    NAi64 bSign = naSigni64(b);
    if(naSmalleri64(a, NA_ZERO_i64)){a = naNegi64(a);}
    if(naSmalleri64(b, NA_ZERO_i64)){b = naNegi64(b);}
    retValueu = naDivu64(naCasti64Tou64(a), naCasti64Tou64(b));
    retValueu.hi &= ~NA_SIGN_MASK_32;
    retValuei = naCastu64Toi64(retValueu);
    if(!naEquali64(aSign, bSign)){retValuei = naNegi64(retValuei);}
    return retValuei;
  }
  NA_IDEF NAi64 naModi64(NAi64 a, NAi64 b){
    NAi64 retValuei;
    NAu64 retValueu;
    NAi64 aSign = naSigni64(a);
    if(naSmalleri64(a, NA_ZERO_i64)){a = naNegi64(a);}
    if(naSmalleri64(b, NA_ZERO_i64)){b = naNegi64(b);}
    retValueu = naModu64(naCasti64Tou64(a), naCasti64Tou64(b));
    retValueu.hi &= ~NA_SIGN_MASK_32;
    retValuei = naCastu64Toi64(retValueu);
    if(!naEquali64(aSign, NA_ONE_i64)){retValuei = naNegi64(retValuei);}
    return retValuei;
  }



  NA_IDEF NAi64 naNoti64(NAi64 i){
    return naCastu64Toi64(naNotu64(naCasti64Tou64(i)));
  }
  NA_IDEF NAi64 naOri64(NAi64 a, NAi64 b){
    return naCastu64Toi64(naOru64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naAndi64(NAi64 a, NAi64 b){
    return naCastu64Toi64(naAndu64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naXori64(NAi64 a, NAi64 b){
    return naCastu64Toi64(naXoru64(naCasti64Tou64(a), naCasti64Tou64(b)));
  }
  NA_IDEF NAi64 naShli64(NAi64 a, int n){
    return naCastu64Toi64(naShlu64(naCasti64Tou64(a), n));
  }
  NA_IDEF NAi64 naShri64(NAi64 a, int n){
    NAi64 retValuei;
    if(n < 0){
      retValuei = naShli64(a, -n);
    }else{
    // Beware, do not use <= as some processors will result
    // in garbage when the shift is equal to the type size.
      if(n < 32){
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



  NA_IDEF NABool naEquali64(NAi64 a, NAi64 b){
    return naEqualu64(naCasti64Tou64(a), naCasti64Tou64(b));
  }
  NA_IDEF NABool naGreateri64(NAi64 a, NAi64 b){
    return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo < b.lo) : (a.lo > b.lo))));
  }
  NA_IDEF NABool naGreaterEquali64(NAi64 a, NAi64 b){
    return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo <= b.lo) : (a.lo >= b.lo))));
  }
  NA_IDEF NABool naSmalleri64(NAi64 a, NAi64 b){
    return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo > b.lo) : (a.lo < b.lo))));
  }
  NA_IDEF NABool naSmallerEquali64(NAi64 a, NAi64 b){
    return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_i32) ? (a.lo >= b.lo) : (a.lo <= b.lo))));
  }



  NA_IDEF uint8 naCasti64Tou8(NAi64 i){
    return naCastu64Tou8(naCasti64Tou64(i));
  }
  NA_IDEF uint16 naCasti64Tou16(NAi64 i){
    return naCastu64Tou16(naCasti64Tou64(i));
  }
  NA_IDEF uint32 naCasti64Tou32(NAi64 i){
    return naCastu64Tou32(naCasti64Tou64(i));
  }
  NA_IDEF NAu64 naCasti64Tou64(NAi64 i){
    NAu64 retValuei;
    retValuei.hi = (uint32)i.hi;
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF int8 naCasti64Toi8(NAi64 i){
    return naCastu64Toi8(naCasti64Tou64(i));
  }
  NA_IDEF int16 naCasti64Toi16(NAi64 i){
    return naCastu64Toi16(naCasti64Tou64(i));
  }
  NA_IDEF int32 naCasti64Toi32(NAi64 i){
    return naCastu64Toi32(naCasti64Tou64(i));
  }
  NA_IDEF double naCasti64ToDouble(NAi64 i){
    // warning: this seems to be troublesome in the lower part. Find a
    // better solution in the future by using bit manipulation. todo
    return (double)i.hi * naMakeDoubleWithExponent(32) + ((i.hi < 0) ? -(double)i.lo : (double)i.lo);
  }



  #undef naMakeu64WithLiteralLo
  #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
    #define naMakeu64WithLiteralLo(lo)  {0, lo}
  #else
    #define naMakeu64WithLiteralLo(lo)  {lo, 0}
  #endif



  NA_IDEF NAu64 naMakeu64(uint32 hi, uint32 lo){
    NAu64 retValuei;
    retValuei.hi = hi;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu64 naMakeu64WithLo(uint32 lo){
    NAu64 retValuei;
    retValuei.hi = NA_ZERO_u32;
    retValuei.lo = lo;
    return retValuei;
  }
  NA_IDEF NAu64 naMakeu64WithDouble(double d){
    NAu64 retValuei;
    // note: this is somewhat cumbersome. Do it with bit manipulation. todo.
    retValuei.hi = (uint32)(d / naMakeDoubleWithExponent(32));
    retValuei.lo = (uint32)(d - ((double)((int32)retValuei.hi) * naMakeDoubleWithExponent(32)));
    return retValuei;
  }



  #undef naIncu64
  #define naIncu64(i) (i.hi += (i.lo == NA_MAX_u32), i.lo += 1, i)
  #undef naDecu64
  #define naDecu64(i) (i.hi -= (i.lo == NA_ZERO_u32), i.lo -= 1, i)



  NA_IDEF NAu64 naAddu64(NAu64 a, NAu64 b){
    NAu64 retValuei;
    retValuei.lo = a.lo + b.lo;
    retValuei.hi = a.hi + b.hi;
    retValuei.hi += (retValuei.lo < a.lo); // add a carry if there was an overflow.
    return retValuei;
  }
  NA_IDEF NAu64 naSubu64(NAu64 a, NAu64 b){
    return naAddu64(a, naCasti64Tou64(naNegi64(naCastu64Toi64(b))));
  }
  NA_IDEF NAu64 naMulu64(NAu64 a, NAu64 b){
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
    retValuei.lo += a0b1 << 16;
    retValuei.hi += a0b1 >> 16;
    retValuei.hi += a0 * b2;
    retValuei.hi += (a0 * b3) << 16;

    // multiply a1 * b and add up
    retValuei.lo += a1b0 << 16;
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
  NA_HIDEF void naComputeu64Division(NAu64 a, NAu64 b, NAu64* div, NAu64* rem){
    NAu64 bTmp;
    NAu64 aHighestBit;
    NAu64 bHighestBit;
    *div = NA_ZERO_u64;
    *rem = a;
    if(naEqualu64(b, NA_ZERO_u64)){
      #ifndef NDEBUG
        naCrash("Integer Division by 0");
      #endif
    }else if(naSmalleru64(a, b)){
      // b is larger than a and hence the result is zero.
      // Do nothing here and just return with the values set above.
    }else{
      int shiftCount;

      // search for the highest bit of b.
      aHighestBit = naMakeu64(NA_SIGN_MASK_32, NA_ZERO_u32);
      while(!naEqualu64(naAndu64(a, aHighestBit), aHighestBit)){
        aHighestBit = naShru64(aHighestBit, 1);
      }
      bHighestBit = naMakeu64(NA_SIGN_MASK_32, NA_ZERO_u32);
      while(!naEqualu64(naAndu64(b, bHighestBit), bHighestBit)){
        bHighestBit = naShru64(bHighestBit, 1);
      }

      bTmp = b;
      shiftCount = 0;
      // Make the dividend big enough
      while(!naEqualu64(aHighestBit, bHighestBit)){
        if(naEqualu64(bTmp, NA_ZERO_u64)){
          // b is larger than a and hence the result is zero.
          #ifndef NDEBUG
            naError("This should not happen.");
          #endif
        }
        bTmp = naShlu64(bTmp, 1);
        bHighestBit = naShlu64(bHighestBit, 1);
        shiftCount++;
      }

      while(shiftCount >= 0){
        *div = naShlu64(*div, 1);
        if(naGreaterEqualu64(*rem, bTmp)){
          *div = naOru64(*div, NA_ONE_u64);
          *rem = naSubu64(*rem, bTmp);
        }
        bTmp = naShru64(bTmp, 1);
        shiftCount--;
      }
    }
  }
  NA_IDEF NAu64 naDivu64(NAu64 a, NAu64 b){
    NAu64 divInt;
    NAu64 remInt;
    naComputeu64Division(a, b, &divInt, &remInt);
    return divInt;
  }
  NA_IDEF NAu64 naModu64(NAu64 a, NAu64 b){
    NAu64 divInt;
    NAu64 remInt;
    naComputeu64Division(a, b, &divInt, &remInt);
    return remInt;
  }



  NA_IDEF NAu64  naNotu64(NAu64 i){
    NAu64 retValuei;
    retValuei.hi = ~i.hi;
    retValuei.lo = ~i.lo;
    return retValuei;
  }
  NA_IDEF NAu64  naOru64 (NAu64 a, NAu64 b){
    NAu64 retValuei;
    retValuei.hi = a.hi | b.hi;
    retValuei.lo = a.lo | b.lo;
    return retValuei;
  }
  NA_IDEF NAu64  naAndu64(NAu64 a, NAu64 b){
    NAu64 retValuei;
    retValuei.hi = a.hi & b.hi;
    retValuei.lo = a.lo & b.lo;
    return retValuei;
  }
  NA_IDEF NAu64  naXoru64(NAu64 a, NAu64 b){
    NAu64 retValuei;
    retValuei.hi = a.hi ^ b.hi;
    retValuei.lo = a.lo ^ b.lo;
    return retValuei;
  }
  NA_IDEF NAu64 naShlu64(NAu64 a, int n){
    NAu64 retValuei;
    if(n < 0){
      retValuei = naShru64(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 32){
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
  NA_IDEF NAu64 naShru64(NAu64 a, int n){
    NAu64 retValuei;
    if(n < 0){
      retValuei = naShlu64(a, -n);
    }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 32){
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



  NA_IDEF NABool naEqualu64(NAu64 a, NAu64 b){
    return ((a.hi == b.hi) && (a.lo == b.lo));
  }
  NA_IDEF NABool naGreateru64(NAu64 a, NAu64 b){
    return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo > b.lo)));
  }
  NA_IDEF NABool naGreaterEqualu64(NAu64 a, NAu64 b){
    return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo >= b.lo)));
  }
  NA_IDEF NABool naSmalleru64(NAu64 a, NAu64 b){
    return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo < b.lo)));
  }
  NA_IDEF NABool naSmallerEqualu64(NAu64 a, NAu64 b){
    return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo <= b.lo)));
  }



  NA_IDEF int8 naCastu64Toi8(NAu64 i){
    return (int8)i.lo;
  }
  NA_IDEF int16 naCastu64Toi16(NAu64 i){
    return (int16)i.lo;
  }
  NA_IDEF int32 naCastu64Toi32(NAu64 i){
    return (int32)i.lo;
  }
  NA_IDEF NAi64 naCastu64Toi64(NAu64 i){
    NAi64 retValuei;
    retValuei.hi = (int32)i.hi;
    retValuei.lo = i.lo;
    return retValuei;
  }
  NA_IDEF uint8 naCastu64Tou8(NAu64 i){
    return (uint8)i.lo;
  }
  NA_IDEF uint16 naCastu64Tou16(NAu64 i){
    return (uint16)i.lo;
  }
  NA_IDEF uint32 naCastu64Tou32(NAu64 i){
    return i.lo;
  }
  NA_IDEF double naCastu64ToDouble(NAu64 i){
    return (double)i.hi * naMakeDoubleWithExponent(32) + (double)i.lo;
  }

#endif // defined NA_TYPE_INT64


#endif // NA_INT_64_II_INCLUDED

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
