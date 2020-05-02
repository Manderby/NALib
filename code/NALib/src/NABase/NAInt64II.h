
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_INT_64_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_64_II_INCLUDED
#define NA_INT_64_II_INCLUDED


#if defined NA_TYPE_INT64
  NA_IDEF NAInt64 naMakeInt64(int32 hi, uint32 lo){return ((NAInt64)hi << 32) | lo;}
  NA_IDEF NAInt64 naMakeInt64WithLo(int32 lo){return (NAInt64)lo;}
  NA_IDEF NAInt64 naMakeInt64WithDouble(double lo){return (NAInt64)lo;}
#else
  #if NA_SIGN_ENCODING != NA_SIGN_ENCODING_TWOS_COMPLEMENT

    #error "The NAInt64 emulation is not provided for the current signed integer encoding"

  #else



    NA_IDEF NAInt64 naMakeInt64(int32 hi, uint32 lo){
      NAInt64 retInt;
      retInt.hi = hi;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAInt64 naMakeInt64WithLo(int32 lo){
      NAInt64 retInt;
      retInt.hi = (int32)naGetSignum32(lo);
      retInt.lo = (uint32)lo;
      return retInt;
    }
    NA_IDEF NAInt64 naMakeInt64WithDouble(double d){
      return naGetDoubleInteger(d);
    }



    #undef naIncInt64
    #define naIncInt64(i) (i.hi += (i.lo == NA_MAX_32u), i.lo++, i)
    #undef naDecInt64
    #define naDecInt64(i) (i.hi -= (i.lo == NA_ZERO_32u), i.lo--, i)


    NA_IDEF NAInt64 naNegInt64(NAInt64 i){
      NAInt64 retInt = naNotInt64(i);
      naIncInt64(retInt);
      return retInt;
    }
    NA_IDEF NAInt64 naAddInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naAddUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naSubInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naSubUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naMulInt64(NAInt64 a, NAInt64 b){
      NAInt64 retInt;
      NAUInt64 retUInt;
      NAInt64 aSign = naSigni64(a);
      NAInt64 bSign = naSigni64(b);
      if(naSmallerInt64(a, NA_ZERO_64)){a = naNegInt64(a);}
      if(naSmallerInt64(b, NA_ZERO_64)){b = naNegInt64(b);}
      retUInt = naMulUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retUInt.hi &= ~NA_VALUE32_SIGN_MASK;
      retInt = naCastUInt64ToInt64(retUInt);
      if(!naEqualInt64(aSign, bSign)){retInt = naNegInt64(retInt);}
      // todo: overflow may lead to different result than built-in 64 bit integer
      return retInt;
    }
    NA_IDEF NAInt64 naDivInt64(NAInt64 a, NAInt64 b){
      NAInt64 retInt;
      NAUInt64 retUInt;
      NAInt64 aSign = naSigni64(a);
      NAInt64 bSign = naSigni64(b);
      if(naSmallerInt64(a, NA_ZERO_64)){a = naNegInt64(a);}
      if(naSmallerInt64(b, NA_ZERO_64)){b = naNegInt64(b);}
      retUInt = naDivUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retUInt.hi &= ~NA_VALUE32_SIGN_MASK;
      retInt = naCastUInt64ToInt64(retUInt);
      if(!naEqualInt64(aSign, bSign)){retInt = naNegInt64(retInt);}
      return retInt;
    }
    NA_IDEF NAInt64 naModInt64(NAInt64 a, NAInt64 b){
      NAInt64 retInt;
      NAUInt64 retUInt;
      NAInt64 aSign = naSigni64(a);
      if(naSmallerInt64(a, NA_ZERO_64)){a = naNegInt64(a);}
      if(naSmallerInt64(b, NA_ZERO_64)){b = naNegInt64(b);}
      retUInt = naModUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retUInt.hi &= ~NA_VALUE32_SIGN_MASK;
      retInt = naCastUInt64ToInt64(retUInt);
      if(!naEqualInt64(aSign, NA_ONE_64)){retInt = naNegInt64(retInt);}
      return retInt;
    }



    NA_IDEF NAInt64 naNotInt64(NAInt64 i){
      return naCastUInt64ToInt64(naNotUInt64(naCastInt64ToUInt64(i)));
    }
    NA_IDEF NAInt64 naOrInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naOrUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naAndInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naAndUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naXorInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naXorUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naShlInt64(NAInt64 a, int n){
      return naCastUInt64ToInt64(naShlUInt64(naCastInt64ToUInt64(a), n));
    }
    NA_IDEF NAInt64 naShrInt64(NAInt64 a, int n){
      NAInt64 retInt;
      if(n < 0){
        retInt = naShlInt64(a, -n);
      }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
        if(n < 32){
          retInt.lo = a.lo >> n;
          retInt.lo |= ((uint32)a.hi << (32 - n));
          retInt.hi = a.hi >> n;
        }else{
          uint32 signum = naGetSignum32(a.hi); // Sign preservation!
          retInt.lo = (uint32)((int32)a.hi >> (n - 32));
          retInt.hi = (int32)signum;
        }
      }
      return retInt;
    }



    NA_IDEF NABool naEqualInt64(NAInt64 a, NAInt64 b){
      return naEqualUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
    }
    NA_IDEF NABool naGreaterInt64(NAInt64 a, NAInt64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_32) ? (a.lo < b.lo) : (a.lo > b.lo))));
    }
    NA_IDEF NABool naGreaterEqualInt64(NAInt64 a, NAInt64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_32) ? (a.lo <= b.lo) : (a.lo >= b.lo))));
    }
    NA_IDEF NABool naSmallerInt64(NAInt64 a, NAInt64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_32) ? (a.lo > b.lo) : (a.lo < b.lo))));
    }
    NA_IDEF NABool naSmallerEqualInt64(NAInt64 a, NAInt64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < NA_ZERO_32) ? (a.lo >= b.lo) : (a.lo <= b.lo))));
    }



    NA_IDEF uint8 naCastInt64ToUInt8(NAInt64 i){
      return naCastUInt64ToUInt8(naCastInt64ToUInt64(i));
    }
    NA_IDEF uint16 naCastInt64ToUInt16(NAInt64 i){
      return naCastUInt64ToUInt16(naCastInt64ToUInt64(i));
    }
    NA_IDEF uint32 naCastInt64ToUInt32(NAInt64 i){
      return naCastUInt64ToUInt32(naCastInt64ToUInt64(i));
    }
    NA_IDEF NAUInt64 naCastInt64ToUInt64(NAInt64 i){
      NAUInt64 retInt;
      retInt.hi = (uint32)i.hi;
      retInt.lo = i.lo;
      return retInt;
    }
    NA_IDEF int8 naCastInt64ToInt8(NAInt64 i){
      return naCastUInt64ToInt8(naCastInt64ToUInt64(i));
    }
    NA_IDEF int16 naCastInt64ToInt16(NAInt64 i){
      return naCastUInt64ToInt16(naCastInt64ToUInt64(i));
    }
    NA_IDEF int32 naCastInt64ToInt32(NAInt64 i){
      return naCastUInt64ToInt32(naCastInt64ToUInt64(i));
    }
    NA_IDEF double naCastInt64ToDouble(NAInt64 i){
      // warning: this seems to be troublesome in the lower part. Find a
      // better solution in the future by using bit manipulation. todo
      return (double)i.hi * naMakeDoubleWithExponent(32) + ((i.hi < 0) ? -(double)i.lo : (double)i.lo);
    }



    #undef naMakeUInt64WithLiteralLo
    #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
      #define naMakeUInt64WithLiteralLo(lo)  {0, lo}
    #else
      #define naMakeUInt64WithLiteralLo(lo)  {lo, 0}
    #endif



    NA_IDEF NAUInt64 naMakeUInt64(uint32 hi, uint32 lo){
      NAUInt64 retInt;
      retInt.hi = hi;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAUInt64 naMakeUInt64WithLo(uint32 lo){
      NAUInt64 retInt;
      retInt.hi = NA_ZERO_32u;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAUInt64 naMakeUInt64WithDouble(double d){
      NAUInt64 retInt;
      // note: this is somewhat cumbersome. Do it with bit manipulation. todo.
      retInt.hi = (uint32)(d / naMakeDoubleWithExponent(32));
      retInt.lo = (uint32)(d - ((double)((int32)retInt.hi) * naMakeDoubleWithExponent(32)));
      return retInt;
    }



    #undef naIncUInt64
    #define naIncUInt64(i) (i.hi += (i.lo == NA_MAX_32u), i.lo += 1, i)
    #undef naDecUInt64
    #define naDecUInt64(i) (i.hi -= (i.lo == NA_ZERO_32u), i.lo -= 1, i)



    NA_IDEF NAUInt64 naAddUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retInt;
      retInt.lo = a.lo + b.lo;
      retInt.hi = a.hi + b.hi;
      retInt.hi += (retInt.lo < a.lo); // add a carry if there was an overflow.
      return retInt;
    }
    NA_IDEF NAUInt64 naSubUInt64(NAUInt64 a, NAUInt64 b){
      return naAddUInt64(a, naCastInt64ToUInt64(naNegInt64(naCastUInt64ToInt64(b))));
    }
    NA_IDEF NAUInt64 naMulUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retInt = NA_ZERO_64u;

      uint32 a0 = a.lo & NA_MAX_16u;
      uint32 a1 = a.lo >> 16;
      uint32 a2 = a.hi & NA_MAX_16u;
      uint32 a3 = a.hi >> 16;
      uint32 b0 = b.lo & NA_MAX_16u;
      uint32 b1 = b.lo >> 16;
      uint32 b2 = b.hi & NA_MAX_16u;
      uint32 b3 = b.hi >> 16;
      uint32 a0b1 = a0 * b1;
      uint32 a1b0 = a1 * b0;

      // multiply a0 * b and add up
      retInt.lo += a0 * b0;
      retInt.lo += a0b1 << 16;
      retInt.hi += a0b1 >> 16;
      retInt.hi += a0 * b2;
      retInt.hi += (a0 * b3) << 16;

      // multiply a1 * b and add up
      retInt.lo += a1b0 << 16;
      retInt.hi += a1b0 >> 16;
      retInt.hi += a1 * b1;
      retInt.hi += (a1 * b2) << 16;

      // multiply a2 * b and add up
      retInt.hi += a2 * b0;
      retInt.hi += (a2 * b1) << 16;

      // multiply a3 * b and add up
      retInt.hi += (a3 * b0) << 16;

      return retInt;
    }
    NA_HIDEF void naComputeUInt64Division(NAUInt64 a, NAUInt64 b, NAUInt64* div, NAUInt64* rem){
      NAUInt64 bTmp;
      NAUInt64 aHighestBit;
      NAUInt64 bHighestBit;
      *div = NA_ZERO_64u;
      *rem = a;
      if(naEqualUInt64(b, NA_ZERO_64u)){
        #ifndef NDEBUG
          naCrash("Integer Division by 0");
        #endif
      }else if(naSmallerUInt64(a, b)){
        // b is larger than a and hence the result is zero.
        // Do nothing here and just return with the values set above.
      }else{
        int shiftCount;

        // search for the highest bit of b.
        aHighestBit = naMakeUInt64(NA_VALUE32_SIGN_MASK, NA_ZERO_32u);
        while(!naEqualUInt64(naAndUInt64(a, aHighestBit), aHighestBit)){
          aHighestBit = naShrUInt64(aHighestBit, 1);
        }
        bHighestBit = naMakeUInt64(NA_VALUE32_SIGN_MASK, NA_ZERO_32u);
        while(!naEqualUInt64(naAndUInt64(b, bHighestBit), bHighestBit)){
          bHighestBit = naShrUInt64(bHighestBit, 1);
        }

        bTmp = b;
        shiftCount = 0;
        // Make the dividend big enough
        while(!naEqualUInt64(aHighestBit, bHighestBit)){
          if(naEqualUInt64(bTmp, NA_ZERO_64u)){
            // b is larger than a and hence the result is zero.
            #ifndef NDEBUG
              naError("This should not happen.");
            #endif
          }
          bTmp = naShlUInt64(bTmp, 1);
          bHighestBit = naShlUInt64(bHighestBit, 1);
          shiftCount++;
        }

        while(shiftCount >= 0){
          *div = naShlUInt64(*div, 1);
          if(naGreaterEqualUInt64(*rem, bTmp)){
            *div = naOrUInt64(*div, NA_ONE_64u);
            *rem = naSubUInt64(*rem, bTmp);
          }
          bTmp = naShrUInt64(bTmp, 1);
          shiftCount--;
        }
      }
    }
    NA_IDEF NAUInt64 naDivUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 divInt;
      NAUInt64 remInt;
      naComputeUInt64Division(a, b, &divInt, &remInt);
      return divInt;
    }
    NA_IDEF NAUInt64 naModUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 divInt;
      NAUInt64 remInt;
      naComputeUInt64Division(a, b, &divInt, &remInt);
      return remInt;
    }



    NA_IDEF NAUInt64  naNotUInt64(NAUInt64 i){
      NAUInt64 retInt;
      retInt.hi = ~i.hi;
      retInt.lo = ~i.lo;
      return retInt;
    }
    NA_IDEF NAUInt64  naOrUInt64 (NAUInt64 a, NAUInt64 b){
      NAUInt64 retInt;
      retInt.hi = a.hi | b.hi;
      retInt.lo = a.lo | b.lo;
      return retInt;
    }
    NA_IDEF NAUInt64  naAndUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retInt;
      retInt.hi = a.hi & b.hi;
      retInt.lo = a.lo & b.lo;
      return retInt;
    }
    NA_IDEF NAUInt64  naXorUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retInt;
      retInt.hi = a.hi ^ b.hi;
      retInt.lo = a.lo ^ b.lo;
      return retInt;
    }
    NA_IDEF NAUInt64 naShlUInt64(NAUInt64 a, int n){
      NAUInt64 retInt;
      if(n < 0){
        retInt = naShrUInt64(a, -n);
      }else{
        // Beware, do not use <= as some processors will result
        // in garbage when the shift is equal to the type size.
        if(n < 32){
          retInt.hi = a.hi << n;
          retInt.hi |= a.lo >> (32 - n);
          retInt.lo = a.lo << n;
        }else{
          retInt.hi = a.lo << (n - 32);
          retInt.lo = NA_ZERO_32u;
        }
      }
      return retInt;
    }
    NA_IDEF NAUInt64 naShrUInt64(NAUInt64 a, int n){
      NAUInt64 retInt;
      if(n < 0){
        retInt = naShlUInt64(a, -n);
      }else{
        // Beware, do not use <= as some processors will result
        // in garbage when the shift is equal to the type size.
        if(n < 32){
          retInt.lo = a.lo >> n;
          retInt.lo |= a.hi << (32 - n);
          retInt.hi = a.hi >> n;
        }else{
          retInt.lo = a.hi >> (n - 32);
          retInt.hi = NA_ZERO_32u;
        }
      }
      return retInt;
    }



    NA_IDEF NABool naEqualUInt64(NAUInt64 a, NAUInt64 b){
      return ((a.hi == b.hi) && (a.lo == b.lo));
    }
    NA_IDEF NABool naGreaterUInt64(NAUInt64 a, NAUInt64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo > b.lo)));
    }
    NA_IDEF NABool naGreaterEqualUInt64(NAUInt64 a, NAUInt64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo >= b.lo)));
    }
    NA_IDEF NABool naSmallerUInt64(NAUInt64 a, NAUInt64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo < b.lo)));
    }
    NA_IDEF NABool naSmallerEqualUInt64(NAUInt64 a, NAUInt64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo <= b.lo)));
    }



    NA_IDEF int8 naCastUInt64ToInt8(NAUInt64 i){
      return (int8)i.lo;
    }
    NA_IDEF int16 naCastUInt64ToInt16(NAUInt64 i){
      return (int16)i.lo;
    }
    NA_IDEF int32 naCastUInt64ToInt32(NAUInt64 i){
      return (int32)i.lo;
    }
    NA_IDEF NAInt64 naCastUInt64ToInt64(NAUInt64 i){
      NAInt64 retInt;
      retInt.hi = (int32)i.hi;
      retInt.lo = i.lo;
      return retInt;
    }
    NA_IDEF uint8 naCastUInt64ToUInt8(NAUInt64 i){
      return (uint8)i.lo;
    }
    NA_IDEF uint16 naCastUInt64ToUInt16(NAUInt64 i){
      return (uint16)i.lo;
    }
    NA_IDEF uint32 naCastUInt64ToUInt32(NAUInt64 i){
      return i.lo;
    }
    NA_IDEF double naCastUInt64ToDouble(NAUInt64 i){
      return (double)i.hi * naMakeDoubleWithExponent(32) + (double)i.lo;
    }


  #endif // NA_SIGN_ENCODING == NA_SIGN_ENCODING_TWOS_COMPLEMENT
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
