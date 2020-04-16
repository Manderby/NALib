
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"


#if defined NA_TYPE_INT128
  NA_IDEF NAInt128 naMakeInt128(int64 hi, uint64 lo){return ((NAInt128)hi << 64) | lo;}
  NA_IDEF NAInt128 naMakeInt128WithLo(NAInt64 lo){return (NAInt128)lo;}
  NA_IDEF NAInt128 naMakeInt128WithDouble(double lo){return (NAInt128)lo;}
#else
  #if NA_SIGNED_INTEGER_ENCODING != NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT

    #error "The NAInt128 emulation is not provided for the current signed integer encoding"

  #else



    NA_IDEF NAInt128 naMakeInt128(NAInt64 hi, NAUInt64 lo){
      NAInt128 retint;
      retint.hi = hi;
      retint.lo = lo;
      return retint;
    }
    NA_IDEF NAInt128 naMakeInt128WithLo(NAInt64 lo){
      NAInt128 retint;
      retint.hi = naCastUInt64ToInt64(naGetSignum64(lo));
      retint.lo = naCastInt64ToUInt64(lo);
      return retint;
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
      NAInt128 retint = naNotInt128(i);
      naIncInt128(retint);
      return retint;
    }
    NA_IDEF NAInt128 naAddInt128(NAInt128 a, NAInt128 b){
      return naCastUInt128ToInt128(naAddUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b)));
    }
    NA_IDEF NAInt128 naSubInt128(NAInt128 a, NAInt128 b){
      return naCastUInt128ToInt128(naSubUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b)));
    }
    NA_IDEF NAInt128 naMulInt128(NAInt128 a, NAInt128 b){
      NAInt128 retint;
      NAUInt128 retuint;
      NAInt128 signa = naSigni128(a);
      NAInt128 signb = naSigni128(b);
      if(naSmallerInt128(a, NA_ZERO_128)){a = naNegInt128(a);}
      if(naSmallerInt128(b, NA_ZERO_128)){b = naNegInt128(b);}
      retuint = naMulUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b));
      retuint.hi = naAndUInt64(retuint.hi, naNotUInt64(NA_VALUE64_SIGN_MASK));
      retint = naCastUInt128ToInt128(retuint);
      if(!naEqualInt128(signa, signb)){retint = naNegInt128(retint);}
      // todo: overflow may lead to different result than built-in 128 bit integer
      return retint;
    }
    NA_IDEF NAInt128 naDivInt128(NAInt128 a, NAInt128 b){
      NAInt128 retint;
      NAUInt128 retuint;
      NAInt128 signa = naSigni128(a);
      NAInt128 signb = naSigni128(b);
      if(naSmallerInt128(a, NA_ZERO_128)){a = naNegInt128(a);}
      if(naSmallerInt128(b, NA_ZERO_128)){b = naNegInt128(b);}
      retuint = naDivUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b));
      retuint.hi = naAndUInt64(retuint.hi, naNotUInt64(NA_VALUE64_SIGN_MASK));
      retint = naCastUInt128ToInt128(retuint);
      if(!naEqualInt128(signa, signb)){retint = naNegInt128(retint);}
      return retint;
    }
    NA_IDEF NAInt128 naModInt128(NAInt128 a, NAInt128 b){
      NAInt128 retint;
      NAUInt128 retuint;
      NAInt128 signa = naSigni128(a);
      if(naSmallerInt128(a, NA_ZERO_128)){a = naNegInt128(a);}
      if(naSmallerInt128(b, NA_ZERO_128)){b = naNegInt128(b);}
      retuint = naModUInt128(naCastInt128ToUInt128(a), naCastInt128ToUInt128(b));
      retuint.hi = naAndUInt64(retuint.hi, naNotUInt64(NA_VALUE64_SIGN_MASK));
      retint = naCastUInt128ToInt128(retuint);
      if(!naEqualInt128(signa, NA_ONE_128)){retint = naNegInt128(retint);}
      return retint;
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
      NAInt128 retint;
      if(n < 0){
        retint = naShlInt128(a, -n);
      }else{
        // Beware, do not use <= as some processors will result
        // in garbage when the shift is equal to the type size.
        if(n < 64){
          retint.lo = naShrUInt64(a.lo, n);
          retint.lo = naOrUInt64(retint.lo, naShlUInt64(naCastInt64ToUInt64(a.hi), (64 - n)));
          retint.hi = naShrInt64(a.hi, n);
        }else{
          NAUInt64 signum = naGetSignum64(a.hi); // Sign preservation!
          retint.lo = naCastInt64ToUInt64(naShrInt64(a.hi, (n - 64)));
          retint.hi = naCastUInt64ToInt64(signum);
        }
      }
      return retint;
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
      NAUInt128 retint;
      retint.hi = naCastInt64ToUInt64(i.hi);
      retint.lo = i.lo;
      return retint;
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
      #define naMakeUInt128WithLiteralLo(lo)  {NA_ZERO_64u, lo}
    #else
      #define naMakeUInt128WithLiteralLo(lo)  {lo, NA_ZERO_64u}
    #endif



    NA_IDEF NAUInt128 naMakeUInt128(NAUInt64 hi, NAUInt64 lo){
      NAUInt128 retint;
      retint.hi = hi;
      retint.lo = lo;
      return retint;
    }
    NA_IDEF NAUInt128 naMakeUInt128WithLo(NAUInt64 lo){
      NAUInt128 retint;
      retint.hi = NA_ZERO_64u;
      retint.lo = lo;
      return retint;
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
      NAUInt128 retint;
      retint.lo = naAddUInt64(a.lo, b.lo);
      retint.hi = naAddUInt64(a.hi, b.hi);
      retint.hi = naAddUInt64(retint.hi, naMakeUInt64WithLo(naSmallerUInt64(retint.lo, a.lo))); // add a carry if there was an overflow.
      return retint;
    }
    NA_IDEF NAUInt128 naSubUInt128(NAUInt128 a, NAUInt128 b){
      return naAddUInt128(a, naCastInt128ToUInt128(naNegInt128(naCastUInt128ToInt128(b))));
    }
    NA_IDEF NAUInt128 naMulUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 retint = NA_ZERO_128u;

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
      retint.lo = naAddUInt64(retint.lo, naMulUInt64(a0, b0));
      retint.lo = naAddUInt64(retint.lo, naShlUInt64(a0b1, 32));
      retint.hi = naAddUInt64(retint.hi, naShrUInt64(a0b1, 32));
      retint.hi = naAddUInt64(retint.hi, naMulUInt64(a0, b2));
      retint.hi = naAddUInt64(retint.hi, naShlUInt64(naMulUInt64(a0, b3), 32));

      // multiply a1 * b and add up
      retint.lo = naAddUInt64(retint.lo, naShlUInt64(a1b0, 32));
      retint.hi = naAddUInt64(retint.hi, naShrUInt64(a1b0, 32));
      retint.hi = naAddUInt64(retint.hi, naMulUInt64(a1, b1));
      retint.hi = naAddUInt64(retint.hi, naShlUInt64(naMulUInt64(a1, b2), 32));

      // multiply a2 * b and add up
      retint.hi = naAddUInt64(retint.hi, naMulUInt64(a2, b0));
      retint.hi = naAddUInt64(retint.hi, naShlUInt64(naMulUInt64(a2, b1), 32));

      // multiply a3 * b and add up
      retint.hi = naAddUInt64(retint.hi, naShlUInt64(naMulUInt64(a3, b0), 32));

      return retint;
    }
    NA_HIDEF void naComputeUInt128Division(NAUInt128 a, NAUInt128 b, NAUInt128* div, NAUInt128* rem){
      NAUInt128 tmpb;
      NAUInt128 highestbita;
      NAUInt128 highestbitb;
      *div = NA_ZERO_128u;
      *rem = a;
      if(naEqualUInt128(b, NA_ZERO_128u)){
        #ifndef NDEBUG
          naCrash("Integer Division by 0");
        #endif
      }else{
        int shiftcount;

        // search for the highest bit of b.
        highestbita = naMakeUInt128(NA_VALUE64_SIGN_MASK, NA_ZERO_64u);
        while(!naEqualUInt128(naAndUInt128(a, highestbita), highestbita)){
          highestbita = naShrUInt128(highestbita, 1);
        }
        highestbitb = naMakeUInt128(NA_VALUE64_SIGN_MASK, NA_ZERO_64u);
        while(!naEqualUInt128(naAndUInt128(b, highestbitb), highestbitb)){
          highestbitb = naShrUInt128(highestbitb, 1);
        }

        tmpb = b;
        shiftcount = 0;
        // Make the dividend big enough
        while(!naEqualUInt128(highestbita, highestbitb)){
          if(naEqualUInt128(tmpb, NA_ZERO_128u)){
            // b is larger than a and hence the result is zero.
            shiftcount = 0;
            break;
          }
          tmpb = naShlUInt128(tmpb, 1);
          highestbitb = naShlUInt128(highestbitb, 1);
          shiftcount++;
        }

        while(shiftcount >= 0){
          *div = naShlUInt128(*div, 1);
          if(naGreaterEqualUInt128(*rem, tmpb)){
            *div = naOrUInt128(*div, NA_ONE_128u);
            *rem = naSubUInt128(*rem, tmpb);
          }
          tmpb = naShrUInt128(tmpb, 1);
          shiftcount--;
        }
      }
    }
    NA_IDEF NAUInt128 naDivUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 divint;
      NAUInt128 remint;
      naComputeUInt128Division(a, b, &divint, &remint);
      return divint;
    }
    NA_IDEF NAUInt128 naModUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 divint;
      NAUInt128 remint;
      naComputeUInt128Division(a, b, &divint, &remint);
      return remint;
    }



    NA_IDEF NAUInt128  naNotUInt128(NAUInt128 i){
      NAUInt128 retint;
      retint.hi = naNotUInt64(i.hi);
      retint.lo = naNotUInt64(i.lo);
      return retint;
    }
    NA_IDEF NAUInt128  naOrUInt128 (NAUInt128 a, NAUInt128 b){
      NAUInt128 retint;
      retint.hi = naOrUInt64(a.hi, b.hi);
      retint.lo = naOrUInt64(a.lo, b.lo);
      return retint;
    }
    NA_IDEF NAUInt128  naAndUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 retint;
      retint.hi = naAndUInt64(a.hi, b.hi);
      retint.lo = naAndUInt64(a.lo, b.lo);
      return retint;
    }
    NA_IDEF NAUInt128  naXorUInt128(NAUInt128 a, NAUInt128 b){
      NAUInt128 retint;
      retint.hi = naXorUInt64(a.hi, b.hi);
      retint.lo = naXorUInt64(a.lo, b.lo);
      return retint;
    }
    NA_IDEF NAUInt128 naShlUInt128(NAUInt128 a, int n){
      NAUInt128 retint;
      if(n < 0){return naShrUInt128(a, -n);}
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
      if(n < 64){
        retint.hi = naShlUInt64(a.hi, n);
        retint.hi = naOrUInt64(retint.hi, naShrUInt64(a.lo, (64 - n)));
        retint.lo = naShlUInt64(a.lo, n);
      }else{
        retint.hi = naShlUInt64(a.lo, (n - 64));
        retint.lo = NA_ZERO_64u;
      }
      return retint;
    }
    NA_IDEF NAUInt128 naShrUInt128(NAUInt128 a, int n){
      NAUInt128 retint;
      if(n < 0){
        retint = naShlUInt128(a, -n);
      }else{
      // Beware, do not use <= as some processors will result
      // in garbage when the shift is equal to the type size.
        if(n < 64){
          retint.lo = naShrUInt64(a.lo, n);
          retint.lo = naOrUInt64(retint.lo, naShlUInt64(a.hi, (64 - n)));
          retint.hi = naShrUInt64(a.hi, n);
        }else{
          retint.lo = naShrUInt64(a.hi, (n - 64));
          retint.hi = NA_ZERO_64u;
        }
      }
      return retint;
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
      NAInt128 retint;
      retint.hi = naCastUInt64ToInt64(i.hi);
      retint.lo = i.lo;
      return retint;
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
