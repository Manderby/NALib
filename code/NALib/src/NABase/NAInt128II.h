
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_INT_128_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_128_II_INCLUDED
#define NA_INT_128_II_INCLUDED


#if defined NA_TYPE_INT128
  NA_IDEF NAInt128 naMakeInt128(int64 hi, uint64 lo){return ((NAInt128)hi << 64) | lo;}
  NA_IDEF NAInt128 naMakeInt128WithLo(NAInt64 lo){return (NAInt128)lo;}
  NA_IDEF NAInt128 naMakeInt128WithDouble(double lo){return (NAInt128)lo;}
#else
  #if NA_SIGNED_INTEGER_ENCODING != NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT

    #error "The NAInt128 emulation is not provided for the current signed integer encoding"

  #else



    NA_IDEF NAInt128 naMakeInt128(NAInt64 hi, NAUInt64 lo){
      NAInt128 retInt;
      retInt.hi = hi;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAInt128 naMakeInt128WithLo(NAInt64 lo){
      NAInt128 retInt;
      retInt.hi = naCastUInt64ToInt64(naGetSignum64(lo));
      retInt.lo = naCastInt64ToUInt64(lo);
      return retInt;
    }
    NA_IDEF NAInt128 naMakeInt128WithDouble(double d){
      return naMakeInt128WithLo(naGetDoubleInteger(d));
    }



    #undef naIncInt128
    #define naIncInt128(i)\
      (i.hi = naAddInt64(i.hi, naMakeInt64WithLo(naEqualUInt64(i.lo, NA_UINT64_MAX))),\
      naIncUInt64(i.lo),\
      i)
    #undef naDecInt128
    #define naDecInt128(i)\
      (i.hi = naSubInt64(i.hi, naMakeInt64WithLo(naEqualUInt64(i.lo, NA_ZERO_64u))),\
      naDecUInt64(i.lo),\
      i)


    NA_IDEF NAInt128 naNegInt128(NAInt128 i){
      NAInt128 retInt = naNotInt128(i);
      naIncInt128(retInt);
      return retInt;
    }
    NA_IDEF NAInt128 naAddInt128(NAInt128 a, NAInt128 b){
      return naCastUInt128ToInt128(naAddUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b)));
    }
    NA_IDEF NAInt128 naSubInt128(NAInt128 a, NAInt128 b){
      return naCastUInt128ToInt128(naSubUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b)));
    }
    NA_IDEF NAInt128 naMulInt128(NAInt128 a, NAInt128 b){
      NAInt128 retInt;
      NAUInt128 retUInt;
      NAInt128 aSign = naSigni128(a);
      NAInt128 bSign = naSigni128(b);
      if(naSmallerInt128(a, NA_ZERO_128)){a = naNegInt128(a);}
      if(naSmallerInt128(b, NA_ZERO_128)){b = naNegInt128(b);}
      retUInt = naMulUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b));
      retUInt.hi = naAndUInt64(retUInt.hi, naNotUInt64(NA_VALUE64_SIGN_MASK));
      retInt = naCastUInt128ToInt128(retUInt);
      if(!naEqualInt128(aSign, bSign)){retInt = naNegInt128(retInt);}
      // todo: overflow may lead to different result than built-in 128 bit integer
      return retInt;
    }
    NA_IDEF NAInt128 naDivInt128(NAInt128 a, NAInt128 b){
      NAInt128 retInt;
      NAUInt128 retUInt;
      NAInt128 aSign = naSigni128(a);
      NAInt128 bSign = naSigni128(b);
      if(naSmallerInt128(a, NA_ZERO_128)){a = naNegInt128(a);}
      if(naSmallerInt128(b, NA_ZERO_128)){b = naNegInt128(b);}
      retUInt = naDivUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b));
      retUInt.hi = naAndUInt64(retUInt.hi, naNotUInt64(NA_VALUE64_SIGN_MASK));
      retInt = naCastUInt128ToInt128(retUInt);
      if(!naEqualInt128(aSign, bSign)){retInt = naNegInt128(retInt);}
      return retInt;
    }
    NA_IDEF NAInt128 naModInt128(NAInt128 a, NAInt128 b){
      NAInt128 retInt;
      NAUInt128 retUInt;
      NAInt128 aSign = naSigni128(a);
      if(naSmallerInt128(a, NA_ZERO_128)){a = naNegInt128(a);}
      if(naSmallerInt128(b, NA_ZERO_128)){b = naNegInt128(b);}
      retUInt = naModUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b));
      retUInt.hi = naAndUInt64(retUInt.hi, naNotUInt64(NA_VALUE64_SIGN_MASK));
      retInt = naCastUInt128ToInt128(retUInt);
      if(!naEqualInt128(aSign, NA_ONE_128)){retInt = naNegInt128(retInt);}
      return retInt;
    }



    NA_IDEF NAInt128 naNotInt128(NAInt128 i){
      return naCastUInt128ToInt128(naNotUInt128(naCastInt128ToUInt128(i)));
    }
    NA_IDEF NAInt128 naOrInt128(NAInt128 a, NAInt128 b){
      return naCastUInt128ToInt128(naOrUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b)));
    }
    NA_IDEF NAInt128 naAndInt128(NAInt128 a, NAInt128 b){
      return naCastUInt128ToInt128(naAndUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b)));
    }
    NA_IDEF NAInt128 naXorInt128(NAInt128 a, NAInt128 b){
      return naCastUInt128ToInt128(naXorUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b)));
    }
    NA_IDEF NAInt128 naShlInt128(NAInt128 a, int n){
      return naCastUInt128ToInt128(naShlUInt128(naCastInt128ToUInt128(a), n));
    }
    NA_IDEF NAInt128 naShrInt128(NAInt128 a, int n){
      NAInt128 retInt;
      if(n < 0){
        retInt = naShlInt128(a, -n);
      }else{
        // Beware, do not use <= as some processors will result
        // in garbage when the shift is equal to the type size.
        if(n < 64){
          retInt.lo = naShrUInt64(a.lo, n);
          retInt.lo = naOrUInt64(retInt.lo, naShlUInt64(naCastInt64ToUInt64(a.hi), (64 - n)));
          retInt.hi = naShrInt64(a.hi, n);
        }else{
          NAUInt64 signum = naGetSignum64(a.hi); // Sign preservation!
          retInt.lo = naCastInt64ToUInt64(naShrInt64(a.hi, (n - 64)));
          retInt.hi = naCastUInt64ToInt64(signum);
        }
      }
      return retInt;
    }



    NA_IDEF NABool naEqualInt128(NAInt128 a, NAInt128 b){
      return naEqualUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b));
    }
    NA_IDEF NABool naGreaterInt128(NAInt128 a, NAInt128 b){
      return (naGreaterInt64(a.hi, b.hi)
        || (naEqualInt64(a.hi, b.hi)
        && (naSmallerInt64(a.hi, NA_ZERO_64) ? naSmallerUInt64(a.lo, b.lo) : naGreaterUInt64(a.lo, b.lo))));
    }
    NA_IDEF NABool naGreaterEqualInt128(NAInt128 a, NAInt128 b){
      return (naGreaterInt64(a.hi, b.hi)
        || (naEqualInt64(a.hi, b.hi)
        && (naSmallerInt64(a.hi, NA_ZERO_64) ? naSmallerEqualUInt64(a.lo, b.lo) : naGreaterEqualUInt64(a.lo, b.lo))));
    }
    NA_IDEF NABool naSmallerInt128(NAInt128 a, NAInt128 b){
      return (naSmallerInt64(a.hi, b.hi)
        || (naEqualInt64(a.hi, b.hi)
        && (naSmallerInt64(a.hi, NA_ZERO_64) ? naGreaterUInt64(a.lo, b.lo) : naSmallerUInt64(a.lo, b.lo))));
    }
    NA_IDEF NABool naSmallerEqualInt128(NAInt128 a, NAInt128 b){
      return (naSmallerInt64(a.hi, b.hi)
        || (naEqualInt64(a.hi, b.hi)
        && (naSmallerInt64(a.hi, NA_ZERO_64) ? naGreaterEqualUInt64(a.lo, b.lo) : naSmallerEqualUInt64(a.lo, b.lo))));
    }



    NA_IDEF uint8 naCastInt128ToUInt8(NAInt128 i){
      return naCastUInt128ToUInt8(naCastInt128ToUInt128(i));
    }
    NA_IDEF uint16 naCastInt128ToUInt16(NAInt128 i){
      return naCastUInt128ToUInt16(naCastInt128ToUInt128(i));
    }
    NA_IDEF uint32 naCastInt128ToUInt32(NAInt128 i){
      return naCastUInt128ToUInt32(naCastInt128ToUInt128(i));
    }
    NA_IDEF NAUInt64 naCastInt128ToUInt64(NAInt128 i){
      return naCastUInt128ToUInt64(naCastInt128ToUInt128(i));
    }
    NA_IDEF NAUInt128 naCastInt128ToUInt128(NAInt128 i){
      NAUInt128 retInt;
      retInt.hi = naCastInt64ToUInt64(i.hi);
      retInt.lo = i.lo;
      return retInt;
    }
    NA_IDEF int8 naCastInt128ToInt8(NAInt128 i){
      return naCastUInt128ToInt8(naCastInt128ToUInt128(i));
    }
    NA_IDEF int16 naCastInt128ToInt16(NAInt128 i){
      return naCastUInt128ToInt16(naCastInt128ToUInt128(i));
    }
    NA_IDEF int32 naCastInt128ToInt32(NAInt128 i){
      return naCastUInt128ToInt32(naCastInt128ToUInt128(i));
    }
    NA_IDEF NAInt64 naCastInt128ToInt64(NAInt128 i){
      return naCastUInt128ToInt64(naCastInt128ToUInt128(i));
    }
    NA_IDEF double naCastInt128ToDouble(NAInt128 i){
      return naCastInt64ToDouble(naCastInt128ToInt64(i));
    }



    #undef naMakeUInt128WithLiteralLo
    #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
      #define naMakeUInt128WithLiteralLo(lo)  {0, lo}
    #else
      #define naMakeUInt128WithLiteralLo(lo)  {lo, 0}
    #endif



    NA_IDEF NAUInt128 naMakeUInt128(NAUInt64 hi, NAUInt64 lo){
      NAUInt128 retInt;
      retInt.hi = hi;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAUInt128 naMakeUInt128WithLo(NAUInt64 lo){
      NAUInt128 retInt;
      retInt.hi = NA_ZERO_64u;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAUInt128 naMakeUInt128WithDouble(double d){
      return naMakeUInt128WithLo(naMakeUInt64WithDouble(d));
    }



    #undef naIncUInt128
    #define naIncUInt128(i)\
      (i.hi = naAddUInt64(i.hi, naMakeUInt64WithLo(naEqualUInt64(i.lo, NA_UINT64_MAX))),\
      naIncUInt64(i.lo),\
      i)
    #undef naDecUInt128
    #define naDecUInt128(i)\
      (i.hi = naSubUInt64(i.hi, naMakeUInt64WithLo(naEqualUInt64(i.lo, NA_ZERO_64u))),\
      naDecUInt64(i.lo),\
      i)



    NA_IDEF NAUInt128 naAddUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 retInt;
      retInt.lo = naAddUInt64(a.lo, b.lo);
      retInt.hi = naAddUInt64(a.hi, b.hi);
      retInt.hi = naAddUInt64(retInt.hi, naMakeUInt64WithLo(naSmallerUInt64(retInt.lo, a.lo))); // add a carry if there was an overflow.
      return retInt;
    }
    NA_IDEF NAUInt128 naSubUInt128(NAUInt128 a, NAUInt128 b){
      return naAddUInt128(a, naCastInt128ToUInt128(naNegInt128(naCastUInt128ToInt128(b))));
    }
    NA_IDEF NAUInt128 naMulUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 retInt = NA_ZERO_128u;

      NAUInt64 a0 = naAndUInt64(a.lo, naMakeUInt64WithLo(NA_UINT32_MAX));
      NAUInt64 a1 = naShrUInt64(a.lo, 32);
      NAUInt64 a2 = naAndUInt64(a.hi, naMakeUInt64WithLo(NA_UINT32_MAX));
      NAUInt64 a3 = naShrUInt64(a.hi, 32);
      NAUInt64 b0 = naAndUInt64(b.lo, naMakeUInt64WithLo(NA_UINT32_MAX));
      NAUInt64 b1 = naShrUInt64(b.lo, 32);
      NAUInt64 b2 = naAndUInt64(b.hi, naMakeUInt64WithLo(NA_UINT32_MAX));
      NAUInt64 b3 = naShrUInt64(b.hi, 32);
      NAUInt64 a0b1 = naMulUInt64(a0, b1);
      NAUInt64 a1b0 = naMulUInt64(a1, b0);

      // multiply a0 * b and add up
      retInt.lo = naAddUInt64(retInt.lo, naMulUInt64(a0, b0));
      retInt.lo = naAddUInt64(retInt.lo, naShlUInt64(a0b1, 32));
      retInt.hi = naAddUInt64(retInt.hi, naShrUInt64(a0b1, 32));
      retInt.hi = naAddUInt64(retInt.hi, naMulUInt64(a0, b2));
      retInt.hi = naAddUInt64(retInt.hi, naShlUInt64(naMulUInt64(a0, b3), 32));

      // multiply a1 * b and add up
      retInt.lo = naAddUInt64(retInt.lo, naShlUInt64(a1b0, 32));
      retInt.hi = naAddUInt64(retInt.hi, naShrUInt64(a1b0, 32));
      retInt.hi = naAddUInt64(retInt.hi, naMulUInt64(a1, b1));
      retInt.hi = naAddUInt64(retInt.hi, naShlUInt64(naMulUInt64(a1, b2), 32));

      // multiply a2 * b and add up
      retInt.hi = naAddUInt64(retInt.hi, naMulUInt64(a2, b0));
      retInt.hi = naAddUInt64(retInt.hi, naShlUInt64(naMulUInt64(a2, b1), 32));

      // multiply a3 * b and add up
      retInt.hi = naAddUInt64(retInt.hi, naShlUInt64(naMulUInt64(a3, b0), 32));

      return retInt;
    }
    NA_HIDEF void naComputeUInt128Division(NAUInt128 a, NAUInt128 b, NAUInt128* div, NAUInt128* rem){
      NAUInt128 bTmp;
      NAUInt128 aHighestBit;
      NAUInt128 bHighestBit;
      *div = NA_ZERO_128u;
      *rem = a;
      if(naEqualUInt128(b, NA_ZERO_128u)){
        #ifndef NDEBUG
          naCrash("Integer Division by 0");
        #endif
      }else if(naSmallerUInt128(a, b)){
        // b is larger than a and hence the result is zero.
        // Do nothing here and just return with the values set above.
      }else{
        int shiftCount;

        // search for the highest bit of b.
        aHighestBit = naMakeUInt128(NA_VALUE64_SIGN_MASK, NA_ZERO_64u);
        while(!naEqualUInt128(naAndUInt128(a, aHighestBit), aHighestBit)){
          aHighestBit = naShrUInt128(aHighestBit, 1);
        }
        bHighestBit = naMakeUInt128(NA_VALUE64_SIGN_MASK, NA_ZERO_64u);
        while(!naEqualUInt128(naAndUInt128(b, bHighestBit), bHighestBit)){
          bHighestBit = naShrUInt128(bHighestBit, 1);
        }

        bTmp = b;
        shiftCount = 0;
        // Make the dividend big enough
        while(!naEqualUInt128(aHighestBit, bHighestBit)){
          if(naEqualUInt128(bTmp, NA_ZERO_128u)){
            // b is larger than a and hence the result is zero.
            #ifndef NDEBUG
              naError("This should not happen.");
            #endif
          }
          bTmp = naShlUInt128(bTmp, 1);
          bHighestBit = naShlUInt128(bHighestBit, 1);
          shiftCount++;
        }

        while(shiftCount >= 0){
          *div = naShlUInt128(*div, 1);
          if(naGreaterEqualUInt128(*rem, bTmp)){
            *div = naOrUInt128(*div, NA_ONE_128u);
            *rem = naSubUInt128(*rem, bTmp);
          }
          bTmp = naShrUInt128(bTmp, 1);
          shiftCount--;
        }
      }
    }
    NA_IDEF NAUInt128 naDivUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 divInt;
      NAUInt128 remInt;
      naComputeUInt128Division(a, b, &divInt, &remInt);
      return divInt;
    }
    NA_IDEF NAUInt128 naModUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 divInt;
      NAUInt128 remInt;
      naComputeUInt128Division(a, b, &divInt, &remInt);
      return remInt;
    }



    NA_IDEF NAUInt128  naNotUInt128(NAUInt128 i){
      NAUInt128 retInt;
      retInt.hi = naNotUInt64(i.hi);
      retInt.lo = naNotUInt64(i.lo);
      return retInt;
    }
    NA_IDEF NAUInt128  naOrUInt128 (NAUInt128 a, NAUInt128 b){
      NAUInt128 retInt;
      retInt.hi = naOrUInt64(a.hi, b.hi);
      retInt.lo = naOrUInt64(a.lo, b.lo);
      return retInt;
    }
    NA_IDEF NAUInt128  naAndUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 retInt;
      retInt.hi = naAndUInt64(a.hi, b.hi);
      retInt.lo = naAndUInt64(a.lo, b.lo);
      return retInt;
    }
    NA_IDEF NAUInt128  naXorUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 retInt;
      retInt.hi = naXorUInt64(a.hi, b.hi);
      retInt.lo = naXorUInt64(a.lo, b.lo);
      return retInt;
    }
    NA_IDEF NAUInt128 naShlUInt128(NAUInt128 a, int n){
      NAUInt128 retInt;
      if(n < 0){
        retInt = naShrUInt128(a, -n);
      }else{
        // Beware, do not use <= as some processors will result
        // in garbage when the shift is equal to the type size.
        if(n < 64){
          retInt.hi = naShlUInt64(a.hi, n);
          retInt.hi = naOrUInt64(retInt.hi, naShrUInt64(a.lo, (64 - n)));
          retInt.lo = naShlUInt64(a.lo, n);
        }else{
          retInt.hi = naShlUInt64(a.lo, (n - 64));
          retInt.lo = NA_ZERO_64u;
        }
      }
      return retInt;
    }
    NA_IDEF NAUInt128 naShrUInt128(NAUInt128 a, int n){
      NAUInt128 retInt;
      if(n < 0){
        retInt = naShlUInt128(a, -n);
      }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
        if(n < 64){
          retInt.lo = naShrUInt64(a.lo, n);
          retInt.lo = naOrUInt64(retInt.lo, naShlUInt64(a.hi, (64 - n)));
          retInt.hi = naShrUInt64(a.hi, n);
        }else{
          retInt.lo = naShrUInt64(a.hi, (n - 64));
          retInt.hi = NA_ZERO_64u;
        }
      }
      return retInt;
    }



    NA_IDEF NABool naEqualUInt128(NAUInt128 a, NAUInt128 b){
      return (naEqualUInt64(a.hi, b.hi) && naEqualUInt64(a.lo, b.lo));
    }
    NA_IDEF NABool naGreaterUInt128(NAUInt128 a, NAUInt128 b){
      return (naGreaterUInt64(a.hi, b.hi) || (naEqualUInt64(a.hi, b.hi) && naGreaterUInt64(a.lo, b.lo)));
    }
    NA_IDEF NABool naGreaterEqualUInt128(NAUInt128 a, NAUInt128 b){
      return (naGreaterUInt64(a.hi, b.hi) || (naEqualUInt64(a.hi, b.hi) && naGreaterEqualUInt64(a.lo, b.lo)));
    }
    NA_IDEF NABool naSmallerUInt128(NAUInt128 a, NAUInt128 b){
      return (naSmallerUInt64(a.hi, b.hi) || (naEqualUInt64(a.hi, b.hi) && naSmallerUInt64(a.lo, b.lo)));
    }
    NA_IDEF NABool naSmallerEqualUInt128(NAUInt128 a, NAUInt128 b){
      return (naSmallerUInt64(a.hi, b.hi) || (naEqualUInt64(a.hi, b.hi) && naSmallerEqualUInt64(a.lo, b.lo)));
    }



    NA_IDEF int8 naCastUInt128ToInt8(NAUInt128 i){
      return naCastUInt64ToInt8(i.lo);
    }
    NA_IDEF int16 naCastUInt128ToInt16(NAUInt128 i){
      return naCastUInt64ToInt16(i.lo);
    }
    NA_IDEF int32 naCastUInt128ToInt32(NAUInt128 i){
      return naCastUInt64ToInt32(i.lo);
    }
    NA_IDEF NAInt64 naCastUInt128ToInt64(NAUInt128 i){
      return naCastUInt64ToInt64(i.lo);
    }
    NA_IDEF NAInt128 naCastUInt128ToInt128(NAUInt128 i){
      NAInt128 retInt;
      retInt.hi = naCastUInt64ToInt64(i.hi);
      retInt.lo = i.lo;
      return retInt;
    }
    NA_IDEF uint8 naCastUInt128ToUInt8(NAUInt128 i){
      return naCastUInt64ToUInt8(i.lo);
    }
    NA_IDEF uint16 naCastUInt128ToUInt16(NAUInt128 i){
      return naCastUInt64ToUInt16(i.lo);
    }
    NA_IDEF uint32 naCastUInt128ToUInt32(NAUInt128 i){
      return naCastUInt64ToUInt32(i.lo);
    }
    NA_IDEF NAUInt64 naCastUInt128ToUInt64(NAUInt128 i){
      return i.lo;
    }
    NA_IDEF double naCastUInt128ToDouble(NAUInt128 i){
      return naCastUInt64ToDouble(i.hi) * naMakeDoubleWithExponent(64) + naCastUInt64ToDouble(i.lo);
    }


  #endif // NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
#endif // defined NA_TYPE_INT128


#endif // NA_INT_128_II_INCLUDED

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
