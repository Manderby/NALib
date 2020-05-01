
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#if defined NA_INT_256_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_INT_256_II_INCLUDED
#define NA_INT_256_II_INCLUDED


#if defined NA_TYPE_INT256
  NA_IDEF NAInt256 naMakeInt256(int128 hi, uint128 lo){return ((NAInt256)hi << 128) | lo;}
  NA_IDEF NAInt256 naMakeInt256WithLo(NAInt128 lo){return (NAInt256)lo;}
  NA_IDEF NAInt256 naMakeInt256WithDouble(double lo){return (NAInt256)lo;}
#else
  #if NA_SIGNED_INTEGER_ENCODING != NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT

    #error "The NAInt256 emulation is not provided for the current signed integer encoding"

  #else



    NA_IDEF NAInt256 naMakeInt256(NAInt128 hi, NAUInt128 lo){
      NAInt256 retInt;
      retInt.hi = hi;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAInt256 naMakeInt256WithLo(NAInt128 lo){
      NAInt256 retInt;
      retInt.hi = naCastUInt128ToInt128(naGetSignum128(lo));
      retInt.lo = naCastInt128ToUInt128(lo);
      return retInt;
    }
    NA_IDEF NAInt256 naMakeInt256WithDouble(double d){
      return naMakeInt256WithLo(naMakeInt128WithLo(naGetDoubleInteger(d)));
    }



    #undef naIncInt256
    #define naIncInt256(i)\
      (i.hi = naAddInt128(i.hi, naMakeInt128WithLo(naMakeInt64WithLo(naEqualUInt128(i.lo, NA_MAX_128u)))),\
      naIncUInt128(i.lo),\
      i)
    #undef naDecInt256
    #define naDecInt256(i)\
      (i.hi = naSubInt128(i.hi, naMakeInt128WithLo(naMakeInt64WithLo(naEqualUInt128(i.lo, NA_ZERO_128u)))),\
      naDecUInt128(i.lo),\
      i)


    NA_IDEF NAInt256 naNegInt256(NAInt256 i){
      NAInt256 retInt = naNotInt256(i);
      naIncInt256(retInt);
      return retInt;
    }
    NA_IDEF NAInt256 naAddInt256(NAInt256 a, NAInt256 b){
      return naCastUInt256ToInt256(naAddUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b)));
    }
    NA_IDEF NAInt256 naSubInt256(NAInt256 a, NAInt256 b){
      return naCastUInt256ToInt256(naSubUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b)));
    }
    NA_IDEF NAInt256 naMulInt256(NAInt256 a, NAInt256 b){
      NAInt256 retInt;
      NAUInt256 retUInt;
      NAInt256 aSign = naSigni256(a);
      NAInt256 bSign = naSigni256(b);
      if(naSmallerInt256(a, NA_ZERO_256)){a = naNegInt256(a);}
      if(naSmallerInt256(b, NA_ZERO_256)){b = naNegInt256(b);}
      retUInt = naMulUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b));
      retUInt.hi = naAndUInt128(retUInt.hi, naNotUInt128(NA_VALUE128_SIGN_MASK));
      retInt = naCastUInt256ToInt256(retUInt);
      if(!naEqualInt256(aSign, bSign)){retInt = naNegInt256(retInt);}
      // todo: overflow may lead to different result than built-in 256 bit integer
      return retInt;
    }
    NA_IDEF NAInt256 naDivInt256(NAInt256 a, NAInt256 b){
      NAInt256 retInt;
      NAUInt256 retUInt;
      NAInt256 aSign = naSigni256(a);
      NAInt256 bSign = naSigni256(b);
      if(naSmallerInt256(a, NA_ZERO_256)){a = naNegInt256(a);}
      if(naSmallerInt256(b, NA_ZERO_256)){b = naNegInt256(b);}
      retUInt = naDivUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b));
      retUInt.hi = naAndUInt128(retUInt.hi, naNotUInt128(NA_VALUE128_SIGN_MASK));
      retInt = naCastUInt256ToInt256(retUInt);
      if(!naEqualInt256(aSign, bSign)){retInt = naNegInt256(retInt);}
      return retInt;
    }
    NA_IDEF NAInt256 naModInt256(NAInt256 a, NAInt256 b){
      NAInt256 retInt;
      NAUInt256 retUInt;
      NAInt256 aSign = naSigni256(a);
      if(naSmallerInt256(a, NA_ZERO_256)){a = naNegInt256(a);}
      if(naSmallerInt256(b, NA_ZERO_256)){b = naNegInt256(b);}
      retUInt = naModUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b));
      retUInt.hi = naAndUInt128(retUInt.hi, naNotUInt128(NA_VALUE128_SIGN_MASK));
      retInt = naCastUInt256ToInt256(retUInt);
      if(!naEqualInt256(aSign, NA_ONE_256)){retInt = naNegInt256(retInt);}
      return retInt;
    }



    NA_IDEF NAInt256 naNotInt256(NAInt256 i){
      return naCastUInt256ToInt256(naNotUInt256(naCastInt256ToUInt256(i)));
    }
    NA_IDEF NAInt256 naOrInt256(NAInt256 a, NAInt256 b){
      return naCastUInt256ToInt256(naOrUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b)));
    }
    NA_IDEF NAInt256 naAndInt256(NAInt256 a, NAInt256 b){
      return naCastUInt256ToInt256(naAndUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b)));
    }
    NA_IDEF NAInt256 naXorInt256(NAInt256 a, NAInt256 b){
      return naCastUInt256ToInt256(naXorUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b)));
    }
    NA_IDEF NAInt256 naShlInt256(NAInt256 a, int n){
      return naCastUInt256ToInt256(naShlUInt256(naCastInt256ToUInt256(a), n));
    }
    NA_IDEF NAInt256 naShrInt256(NAInt256 a, int n){
      NAInt256 retInt;
      if(n < 0){
        retInt = naShlInt256(a, -n);
      }else{
        // Beware, do not use <= as some processors will result
        // in garbage when the shift is equal to the type size.
        if(n < 128){
          retInt.lo = naShrUInt128(a.lo, n);
          retInt.lo = naOrUInt128(retInt.lo, naShlUInt128(naCastInt128ToUInt128(a.hi), (128 - n)));
          retInt.hi = naShrInt128(a.hi, n);
        }else{
          NAUInt128 signum = naGetSignum128(a.hi); // Sign preservation!
          retInt.lo = naCastInt128ToUInt128(naShrInt128(a.hi, (n - 128)));
          retInt.hi = naCastUInt128ToInt128(signum);
        }
      }
      return retInt;
    }



    NA_IDEF NABool naEqualInt256(NAInt256 a, NAInt256 b){
      return naEqualUInt256(naCastInt256ToUInt256(a), naCastInt256ToUInt256(b));
    }
    NA_IDEF NABool naGreaterInt256(NAInt256 a, NAInt256 b){
      return (naGreaterInt128(a.hi, b.hi)
        || (naEqualInt128(a.hi, b.hi)
        && (naSmallerInt128(a.hi, NA_ZERO_128) ? naSmallerUInt128(a.lo, b.lo) : naGreaterUInt128(a.lo, b.lo))));
    }
    NA_IDEF NABool naGreaterEqualInt256(NAInt256 a, NAInt256 b){
      return (naGreaterInt128(a.hi, b.hi)
        || (naEqualInt128(a.hi, b.hi)
        && (naSmallerInt128(a.hi, NA_ZERO_128) ? naSmallerEqualUInt128(a.lo, b.lo) : naGreaterEqualUInt128(a.lo, b.lo))));
    }
    NA_IDEF NABool naSmallerInt256(NAInt256 a, NAInt256 b){
      return (naSmallerInt128(a.hi, b.hi)
        || (naEqualInt128(a.hi, b.hi)
        && (naSmallerInt128(a.hi, NA_ZERO_128) ? naGreaterUInt128(a.lo, b.lo) : naSmallerUInt128(a.lo, b.lo))));
    }
    NA_IDEF NABool naSmallerEqualInt256(NAInt256 a, NAInt256 b){
      return (naSmallerInt128(a.hi, b.hi)
        || (naEqualInt128(a.hi, b.hi)
        && (naSmallerInt128(a.hi, NA_ZERO_128) ? naGreaterEqualUInt128(a.lo, b.lo) : naSmallerEqualUInt128(a.lo, b.lo))));
    }



    NA_IDEF uint8 naCastInt256ToUInt8(NAInt256 i){
      return naCastUInt256ToUInt8(naCastInt256ToUInt256(i));
    }
    NA_IDEF uint16 naCastInt256ToUInt16(NAInt256 i){
      return naCastUInt256ToUInt16(naCastInt256ToUInt256(i));
    }
    NA_IDEF uint32 naCastInt256ToUInt32(NAInt256 i){
      return naCastUInt256ToUInt32(naCastInt256ToUInt256(i));
    }
    NA_IDEF uint64 naCastInt256ToUInt64(NAInt256 i){
      return naCastUInt256ToUInt64(naCastInt256ToUInt256(i));
    }
    NA_IDEF NAUInt128 naCastInt256ToUInt128(NAInt256 i){
      return naCastUInt256ToUInt128(naCastInt256ToUInt256(i));
    }
    NA_IDEF NAUInt256 naCastInt256ToUInt256(NAInt256 i){
      NAUInt256 retInt;
      retInt.hi = naCastInt128ToUInt128(i.hi);
      retInt.lo = i.lo;
      return retInt;
    }
    NA_IDEF int8 naCastInt256ToInt8(NAInt256 i){
      return naCastUInt256ToInt8(naCastInt256ToUInt256(i));
    }
    NA_IDEF int16 naCastInt256ToInt16(NAInt256 i){
      return naCastUInt256ToInt16(naCastInt256ToUInt256(i));
    }
    NA_IDEF int32 naCastInt256ToInt32(NAInt256 i){
      return naCastUInt256ToInt32(naCastInt256ToUInt256(i));
    }
    NA_IDEF int64 naCastInt256ToInt64(NAInt256 i){
      return naCastUInt256ToInt64(naCastInt256ToUInt256(i));
    }
    NA_IDEF NAInt128 naCastInt256ToInt128(NAInt256 i){
      return naCastUInt256ToInt128(naCastInt256ToUInt256(i));
    }
    NA_IDEF double naCastInt256ToDouble(NAInt256 i){
      return naCastInt128ToDouble(naCastInt256ToInt128(i));
    }



    #undef naMakeUInt256WithLiteralLo
    #if NA_ENDIANNESS_HOST == NA_ENDIANNESS_BIG
      #define naMakeUInt256WithLiteralLo(lo)  {0, lo}
    #else
      #define naMakeUInt256WithLiteralLo(lo)  {lo, 0}
    #endif



    NA_IDEF NAUInt256 naMakeUInt256(NAUInt128 hi, NAUInt128 lo){
      NAUInt256 retInt;
      retInt.hi = hi;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAUInt256 naMakeUInt256WithLo(NAUInt128 lo){
      NAUInt256 retInt;
      retInt.hi = NA_ZERO_128u;
      retInt.lo = lo;
      return retInt;
    }
    NA_IDEF NAUInt256 naMakeUInt256WithDouble(double d){
      return naMakeUInt256WithLo(naMakeUInt128WithDouble(d));
    }



    #undef naIncUInt256
    #define naIncUInt256(i)\
      (i.hi = naAddUInt128(i.hi, naMakeUInt128WithLo(naMakeUInt64WithLo(naEqualUInt128(i.lo, NA_MAX_128u)))),\
      naIncUInt128(i.lo),\
      i)
    #undef naDecUInt256
    #define naDecUInt256(i)\
      (i.hi = naSubUInt128(i.hi, naMakeUInt128WithLo(naMakeUInt64WithLo(naEqualUInt128(i.lo, NA_ZERO_128u)))),\
      naDecUInt128(i.lo),\
      i)



    NA_IDEF NAUInt256 naAddUInt256(NAUInt256 a, NAUInt256 b){
      NAUInt256 retInt;
      retInt.lo = naAddUInt128(a.lo, b.lo);
      retInt.hi = naAddUInt128(a.hi, b.hi);
      retInt.hi = naAddUInt128(retInt.hi, naMakeUInt128WithLo(naMakeUInt64WithLo(naSmallerUInt128(retInt.lo, a.lo)))); // add a carry if there was an overflow.
      return retInt;
    }
    NA_IDEF NAUInt256 naSubUInt256(NAUInt256 a, NAUInt256 b){
      return naAddUInt256(a, naCastInt256ToUInt256(naNegInt256(naCastUInt256ToInt256(b))));
    }
    NA_IDEF NAUInt256 naMulUInt256(NAUInt256 a, NAUInt256 b){
      NAUInt256 retInt = NA_ZERO_256u;

      NAUInt128 a0 = naAndUInt128(a.lo, naMakeUInt128WithLo(NA_MAX_64u));
      NAUInt128 a1 = naShrUInt128(a.lo, 64);
      NAUInt128 a2 = naAndUInt128(a.hi, naMakeUInt128WithLo(NA_MAX_64u));
      NAUInt128 a3 = naShrUInt128(a.hi, 64);
      NAUInt128 b0 = naAndUInt128(b.lo, naMakeUInt128WithLo(NA_MAX_64u));
      NAUInt128 b1 = naShrUInt128(b.lo, 64);
      NAUInt128 b2 = naAndUInt128(b.hi, naMakeUInt128WithLo(NA_MAX_64u));
      NAUInt128 b3 = naShrUInt128(b.hi, 64);
      NAUInt128 a0b1 = naMulUInt128(a0, b1);
      NAUInt128 a1b0 = naMulUInt128(a1, b0);

      // multiply a0 * b and add up
      retInt.lo = naAddUInt128(retInt.lo, naMulUInt128(a0, b0));
      retInt.lo = naAddUInt128(retInt.lo, naShlUInt128(a0b1, 64));
      retInt.hi = naAddUInt128(retInt.hi, naShrUInt128(a0b1, 64));
      retInt.hi = naAddUInt128(retInt.hi, naMulUInt128(a0, b2));
      retInt.hi = naAddUInt128(retInt.hi, naShlUInt128(naMulUInt128(a0, b3), 64));

      // multiply a1 * b and add up
      retInt.lo = naAddUInt128(retInt.lo, naShlUInt128(a1b0, 64));
      retInt.hi = naAddUInt128(retInt.hi, naShrUInt128(a1b0, 64));
      retInt.hi = naAddUInt128(retInt.hi, naMulUInt128(a1, b1));
      retInt.hi = naAddUInt128(retInt.hi, naShlUInt128(naMulUInt128(a1, b2), 64));

      // multiply a2 * b and add up
      retInt.hi = naAddUInt128(retInt.hi, naMulUInt128(a2, b0));
      retInt.hi = naAddUInt128(retInt.hi, naShlUInt128(naMulUInt128(a2, b1), 64));

      // multiply a3 * b and add up
      retInt.hi = naAddUInt128(retInt.hi, naShlUInt128(naMulUInt128(a3, b0), 64));

      return retInt;
    }
    NA_HIDEF void naComputeUInt256Division(NAUInt256 a, NAUInt256 b, NAUInt256* div, NAUInt256* rem){
      NAUInt256 bTmp;
      NAUInt256 aHighestBit;
      NAUInt256 bHighestBit;
      *div = NA_ZERO_256u;
      *rem = a;
      if(naEqualUInt256(b, NA_ZERO_256u)){
        #ifndef NDEBUG
          naCrash("Integer Division by 0");
        #endif
      }else if(naSmallerUInt256(a, b)){
        // b is larger than a and hence the result is zero.
        // Do nothing here and just return with the values set above.
      }else{
        int shiftCount;

        // search for the highest bit of b.
        aHighestBit = naMakeUInt256(NA_VALUE128_SIGN_MASK, NA_ZERO_128u);
        while(!naEqualUInt256(naAndUInt256(a, aHighestBit), aHighestBit)){
          aHighestBit = naShrUInt256(aHighestBit, 1);
        }
        bHighestBit = naMakeUInt256(NA_VALUE128_SIGN_MASK, NA_ZERO_128u);
        while(!naEqualUInt256(naAndUInt256(b, bHighestBit), bHighestBit)){
          bHighestBit = naShrUInt256(bHighestBit, 1);
        }

        bTmp = b;
        shiftCount = 0;
        // Make the dividend big enough
        while(!naEqualUInt256(aHighestBit, bHighestBit)){
          if(naEqualUInt256(bTmp, NA_ZERO_256u)){
            // b is larger than a and hence the result is zero.
            #ifndef NDEBUG
              naError("This should not happen.");
            #endif
            return;
          }
          bTmp = naShlUInt256(bTmp, 1);
          bHighestBit = naShlUInt256(bHighestBit, 1);
          shiftCount++;
        }

        while(shiftCount >= 0){
          *div = naShlUInt256(*div, 1);
          if(naGreaterEqualUInt256(*rem, bTmp)){
            *div = naOrUInt256(*div, NA_ONE_256u);
            *rem = naSubUInt256(*rem, bTmp);
          }
          bTmp = naShrUInt256(bTmp, 1);
          shiftCount--;
        }
      }
    }
    NA_IDEF NAUInt256 naDivUInt256(NAUInt256 a, NAUInt256 b){
      NAUInt256 divInt;
      NAUInt256 remInt;
      naComputeUInt256Division(a, b, &divInt, &remInt);
      return divInt;
    }
    NA_IDEF NAUInt256 naModUInt256(NAUInt256 a, NAUInt256 b){
      NAUInt256 divInt;
      NAUInt256 remInt;
      naComputeUInt256Division(a, b, &divInt, &remInt);
      return remInt;
    }



    NA_IDEF NAUInt256  naNotUInt256(NAUInt256 i){
      NAUInt256 retInt;
      retInt.hi = naNotUInt128(i.hi);
      retInt.lo = naNotUInt128(i.lo);
      return retInt;
    }
    NA_IDEF NAUInt256  naOrUInt256 (NAUInt256 a, NAUInt256 b){
      NAUInt256 retInt;
      retInt.hi = naOrUInt128(a.hi, b.hi);
      retInt.lo = naOrUInt128(a.lo, b.lo);
      return retInt;
    }
    NA_IDEF NAUInt256  naAndUInt256(NAUInt256 a, NAUInt256 b){
      NAUInt256 retInt;
      retInt.hi = naAndUInt128(a.hi, b.hi);
      retInt.lo = naAndUInt128(a.lo, b.lo);
      return retInt;
    }
    NA_IDEF NAUInt256  naXorUInt256(NAUInt256 a, NAUInt256 b){
      NAUInt256 retInt;
      retInt.hi = naXorUInt128(a.hi, b.hi);
      retInt.lo = naXorUInt128(a.lo, b.lo);
      return retInt;
    }
    NA_IDEF NAUInt256 naShlUInt256(NAUInt256 a, int n){
      NAUInt256 retInt;
      if(n < 0){
        retInt = naShrUInt256(a, -n);
      }else{
        // Beware, do not use <= as some processors will result
        // in garbage when the shift is equal to the type size.
        if(n < 128){
          retInt.hi = naShlUInt128(a.hi, n);
          retInt.hi = naOrUInt128(retInt.hi, naShrUInt128(a.lo, (128 - n)));
          retInt.lo = naShlUInt128(a.lo, n);
        }else{
          retInt.hi = naShlUInt128(a.lo, (n - 128));
          retInt.lo = NA_ZERO_128u;
        }
      }
      return retInt;
    }
    NA_IDEF NAUInt256 naShrUInt256(NAUInt256 a, int n){
      NAUInt256 retInt;
      if(n < 0){
        retInt = naShlUInt256(a, -n);
      }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.

        if(n < 128){
          retInt.lo = naShrUInt128(a.lo, n);
          retInt.lo = naOrUInt128(retInt.lo, naShlUInt128(a.hi, (128 - n)));
          retInt.hi = naShrUInt128(a.hi, n);
        }else{
          retInt.lo = naShrUInt128(a.hi, (n - 128));
          retInt.hi = NA_ZERO_128u;
        }
      }
      return retInt;
    }



    NA_IDEF NABool naEqualUInt256(NAUInt256 a, NAUInt256 b){
      return (naEqualUInt128(a.hi, b.hi) && naEqualUInt128(a.lo, b.lo));
    }
    NA_IDEF NABool naGreaterUInt256(NAUInt256 a, NAUInt256 b){
      return (naGreaterUInt128(a.hi, b.hi) || (naEqualUInt128(a.hi, b.hi) && naGreaterUInt128(a.lo, b.lo)));
    }
    NA_IDEF NABool naGreaterEqualUInt256(NAUInt256 a, NAUInt256 b){
      return (naGreaterUInt128(a.hi, b.hi) || (naEqualUInt128(a.hi, b.hi) && naGreaterEqualUInt128(a.lo, b.lo)));
    }
    NA_IDEF NABool naSmallerUInt256(NAUInt256 a, NAUInt256 b){
      return (naSmallerUInt128(a.hi, b.hi) || (naEqualUInt128(a.hi, b.hi) && naSmallerUInt128(a.lo, b.lo)));
    }
    NA_IDEF NABool naSmallerEqualUInt256(NAUInt256 a, NAUInt256 b){
      return (naSmallerUInt128(a.hi, b.hi) || (naEqualUInt128(a.hi, b.hi) && naSmallerEqualUInt128(a.lo, b.lo)));
    }



    NA_IDEF int8 naCastUInt256ToInt8(NAUInt256 i){
      return naCastUInt128ToInt8(i.lo);
    }
    NA_IDEF int16 naCastUInt256ToInt16(NAUInt256 i){
      return naCastUInt128ToInt16(i.lo);
    }
    NA_IDEF int32 naCastUInt256ToInt32(NAUInt256 i){
      return naCastUInt128ToInt32(i.lo);
    }
    NA_IDEF int64 naCastUInt256ToInt64(NAUInt256 i){
      return naCastUInt128ToInt64(i.lo);
    }
    NA_IDEF NAInt128 naCastUInt256ToInt128(NAUInt256 i){
      return naCastUInt128ToInt128(i.lo);
    }
    NA_IDEF NAInt256 naCastUInt256ToInt256(NAUInt256 i){
      NAInt256 retInt;
      retInt.hi = naCastUInt128ToInt128(i.hi);
      retInt.lo = i.lo;
      return retInt;
    }
    NA_IDEF uint8 naCastUInt256ToUInt8(NAUInt256 i){
      return naCastUInt128ToUInt8(i.lo);
    }
    NA_IDEF uint16 naCastUInt256ToUInt16(NAUInt256 i){
      return naCastUInt128ToUInt16(i.lo);
    }
    NA_IDEF uint32 naCastUInt256ToUInt32(NAUInt256 i){
      return naCastUInt128ToUInt32(i.lo);
    }
    NA_IDEF uint64 naCastUInt256ToUInt64(NAUInt256 i){
      return naCastUInt128ToUInt64(i.lo);
    }
    NA_IDEF NAUInt128 naCastUInt256ToUInt128(NAUInt256 i){
      return i.lo;
    }
    NA_IDEF double naCastUInt256ToDouble(NAUInt256 i){
      return naCastUInt128ToDouble(i.hi) * naMakeDoubleWithExponent(128) + naCastUInt128ToDouble(i.lo);
    }


  #endif // NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
#endif // defined NA_TYPE_INT256


#endif // NA_INT_256_II_INCLUDED


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
