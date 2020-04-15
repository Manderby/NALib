
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NABase.h"


#if defined NA_TYPE_INT64
  NA_IDEF NAInt64 naMakeInt64WithLo(int32 lo){return (NAInt64)lo;}
  NA_IDEF NAInt64 naMakeInt64WithDouble(double lo){return (NAInt64)lo;}
#else
  #if NA_SIGNED_INTEGER_ENCODING != NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT

    #error "The NAInt64 emulation is not provided for the current signed integer encoding"

  #else



    NA_IDEF NAInt64 naMakeInt64WithLo(int32 lo){
      NAInt64 retint;
      retint.hi = (int32)naGetSignum32(lo);
      retint.lo = (uint32)lo;
      return retint;
    }
    NA_IDEF NAInt64 naMakeInt64WithDouble(double d){
      return naGetDoubleInteger(d);
    }



    #undef naIncInt64
    #define naIncInt64(i) (i.hi += (i.lo == NA_UINT32_MAX), i.lo++, i)
    #undef naDecInt64
    #define naDecInt64(i) (i.hi -= (i.lo == NA_ZERO_32u), i.lo--, i)


    NA_IDEF NAInt64 naNegInt64(NAInt64 i){
      NAInt64 retint = naNotInt64(i);
      naIncInt64(retint);
      return retint;
    }
    NA_IDEF NAInt64 naAddInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naAddUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naSubInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naSubUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naMulInt64(NAInt64 a, NAInt64 b){
      NAInt64 retint;
      NAUInt64 retuint;
      NAInt64 signa = naSigni64(a);
      NAInt64 signb = naSigni64(b);
      if(naSmallerInt64(a, NA_ZERO_64)){a = naNegInt64(a);}
      if(naSmallerInt64(b, NA_ZERO_64)){b = naNegInt64(b);}
      retuint = naMulUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retuint.hi &= ~NA_VALUE32_SIGN_MASK;
      retint = naCastUInt64ToInt64(retuint);
      if(!naEqualInt64(signa, signb)){retint = naNegInt64(retint);}
      // todo: overflow may lead to different result than built-in 64 bit integer
      return retint;
    }
    NA_IDEF NAInt64 naDivInt64(NAInt64 a, NAInt64 b){
      NAInt64 retint;
      NAUInt64 retuint;
      NAInt64 signa = naSigni64(a);
      NAInt64 signb = naSigni64(b);
      if(naSmallerInt64(a, NA_ZERO_64)){a = naNegInt64(a);}
      if(naSmallerInt64(b, NA_ZERO_64)){b = naNegInt64(b);}
      retuint = naDivUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retuint.hi &= ~NA_VALUE32_SIGN_MASK;
      retint = naCastUInt64ToInt64(retuint);
      if(!naEqualInt64(signa, signb)){retint = naNegInt64(retint);}
      return retint;
    }
    NA_IDEF NAInt64 naModInt64(NAInt64 a, NAInt64 b){
      NAInt64 retint;
      NAUInt64 retuint;
      NAInt64 signa = naSigni64(a);
      if(naSmallerInt64(a, NA_ZERO_64)){a = naNegInt64(a);}
      if(naSmallerInt64(b, NA_ZERO_64)){b = naNegInt64(b);}
      retuint = naModUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retuint.hi &= ~NA_VALUE32_SIGN_MASK;
      retint = naCastUInt64ToInt64(retuint);
      if(!naEqualInt64(signa, NA_ONE_64)){retint = naNegInt64(retint);}
      return retint;
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
      NAInt64 retint;
      if(n < 0){
        retint = naShlInt64(a, -n);
      }else{
        if(n <= 32){
          retint.lo = a.lo >> n;
          retint.lo |= ((uint32)a.hi << (32 - n));
          retint.hi = a.hi >> n;
        }else{
          uint32 signum = naGetSignum32(a.hi); // Sign preservation!
          retint.lo = (uint32)((int32)a.hi >> (n - 32));
          retint.hi = signum;
        }
      }
      return retint;
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
      NAUInt64 retint;
      retint.hi = (uint32)i.hi;
      retint.lo = i.lo;
      return retint;
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
      #define naMakeUInt64WithLiteralLo(lo)  {0,(lo)}
    #else
      #define naMakeUInt64WithLiteralLo(lo)  {(lo),0}
    #endif



    NA_IDEF NAUInt64 naMakeUInt64(uint32 hi, uint32 lo){
      NAUInt64 retint;
      retint.hi = hi;
      retint.lo = lo;
      return retint;
    }
    NA_IDEF NAUInt64 naMakeUInt64WithLo(uint32 lo){
      NAUInt64 retint;
      retint.hi = NA_ZERO_32u;
      retint.lo = lo;
      return retint;
    }
    NA_IDEF NAUInt64 naMakeUInt64WithDouble(double d){
      NAUInt64 retint;
      // note: this is somewhat cumbersome. Do it with bit manipulation. todo.
      retint.hi = (uint32)(d / naMakeDoubleWithExponent(32));
      retint.lo = (uint32)(d - ((double)retint.hi * naMakeDoubleWithExponent(32)));
      return retint;
    }



    #undef naIncUInt64
    #define naIncUInt64(i) (i.hi += (i.lo == NA_UINT32_MAX), i.lo += 1, i)
    #undef naDecUInt64
    #define naDecUInt64(i) (i.hi -= (i.lo == NA_ZERO_32u), i.lo -= 1, i)



    NA_IDEF NAUInt64 naAddUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retint;
      retint.lo = a.lo + b.lo;
      retint.hi = a.hi + b.hi;
      retint.hi += (retint.lo < a.lo); // add a carry if there was an overflow.
      return retint;
    }
    NA_IDEF NAUInt64 naSubUInt64(NAUInt64 a, NAUInt64 b){
      return naAddUInt64(a, naCastInt64ToUInt64(naNegInt64(naCastUInt64ToInt64(b))));
    }
    NA_IDEF NAUInt64 naMulUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retint = NA_ZERO_64u;

      uint32 a0 = a.lo & NA_UINT16_MAX;
      uint32 a1 = a.lo >> 16;
      uint32 a2 = a.hi & NA_UINT16_MAX;
      uint32 a3 = a.hi >> 16;
      uint32 b0 = b.lo & NA_UINT16_MAX;
      uint32 b1 = b.lo >> 16;
      uint32 b2 = b.hi & NA_UINT16_MAX;
      uint32 b3 = b.hi >> 16;

      retint.lo += a0 * b0;
      retint.lo += (a0 * b1) << 16;
      retint.hi += (a0 * b1) >> 16;
      retint.hi += a0 * b2;
      retint.hi += (a0 * b3) << 16;

      retint.lo += (a1 * b0) << 16;
      retint.hi += (a1 * b0) >> 16;
      retint.hi += a1 * b1;
      retint.hi += (a1 * b2) << 16;

      retint.hi += a2 * b0;
      retint.hi += (a2 * b1) << 16;

      retint.hi += (a3 * b0) << 16;

      return retint;
    }
    NA_HIDEF void naComputeUInt64Division(NAUInt64 a, NAUInt64 b, NAUInt64* div, NAUInt64* rem){
      NAUInt64 tmpb;
      NAUInt64 highestbita;
      NAUInt64 highestbitb;
      *div = NA_ZERO_64u;
      *rem = a;
      if(naEqualUInt64(b, NA_ZERO_64u)){
        #ifndef NDEBUG
          naCrash("Integer Division by 0");
        #endif
      }else{
        int shiftcount;

        // search for the highest bit of b.
        highestbita = naMakeUInt64(NA_VALUE32_SIGN_MASK, NA_ZERO_32u);
        while(!naEqualUInt64(naAndUInt64(a, highestbita), highestbita)){
          highestbita = naShrUInt64(highestbita, 1);
        }
        highestbitb = naMakeUInt64(NA_VALUE32_SIGN_MASK, NA_ZERO_32u);
        while(!naEqualUInt64(naAndUInt64(b, highestbitb), highestbitb)){
          highestbitb = naShrUInt64(highestbitb, 1);
        }

        tmpb = b;
        shiftcount = 0;
        // Make the dividend big enough
        while(!naEqualUInt64(highestbita, highestbitb)){
          if(naEqualUInt64(tmpb, NA_ZERO_64u)){
            // b is larger than a and hence the result is zero.
            shiftcount = 0;
            break;
          }
          tmpb = naShlUInt64(tmpb, 1);
          highestbitb = naShlUInt64(highestbitb, 1);
          shiftcount++;
        }

        while(shiftcount >= 0){
          *div = naShlUInt64(*div, 1);
          if(naGreaterEqualUInt64(*rem, tmpb)){
            *div = naOrUInt64(*div, NA_ONE_64u);
            *rem = naSubUInt64(*rem, tmpb);
          }
          tmpb = naShrUInt64(tmpb, 1);
          shiftcount--;
        }
      }
    }
    NA_IDEF NAUInt64 naDivUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 divint;
      NAUInt64 remint;
      naComputeUInt64Division(a, b, &divint, &remint);
      return divint;
    }
    NA_IDEF NAUInt64 naModUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 divint;
      NAUInt64 remint;
      naComputeUInt64Division(a, b, &divint, &remint);
      return remint;
    }



    NA_IDEF NAUInt64  naNotUInt64(NAUInt64 i){
      NAUInt64 retint;
      retint.hi = ~i.hi;
      retint.lo = ~i.lo;
      return retint;
    }
    NA_IDEF NAUInt64  naOrUInt64 (NAUInt64 a, NAUInt64 b){
      NAUInt64 retint;
      retint.hi = a.hi | b.hi;
      retint.lo = a.lo | b.lo;
      return retint;
    }
    NA_IDEF NAUInt64  naAndUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retint;
      retint.hi = a.hi & b.hi;
      retint.lo = a.lo & b.lo;
      return retint;
    }
    NA_IDEF NAUInt64  naXorUInt64(NAUInt64 a, NAUInt64 b){
      NAUInt64 retint;
      retint.hi = a.hi ^ b.hi;
      retint.lo = a.lo ^ b.lo;
      return retint;
    }
    NA_IDEF NAUInt64 naShlUInt64(NAUInt64 a, int n){
      NAUInt64 retint;
      if(n < 0){return naShrUInt64(a, -n);}
      if(n <= 32){
        retint.hi = a.hi << n;
        retint.hi |= a.lo >> (32 - n);
        retint.lo = a.lo << n;
      }else{
        retint.hi = a.lo << (n - 32);
        retint.lo = NA_ZERO_32u;
      }
      return retint;
    }
    NA_IDEF NAUInt64 naShrUInt64(NAUInt64 a, int n){
      NAUInt64 retint;
      if(n < 0){
        retint = naShlUInt64(a, -n);
      }else{
        if(n <= 32){
          retint.lo = a.lo >> n;
          retint.lo |= a.hi << (32 - n);
          retint.hi = a.hi >> n;
        }else{
          retint.lo = a.hi >> (n - 32);
          retint.hi = NA_ZERO_32u;
        }
      }
      return retint;
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
      NAInt64 retint;
      retint.hi = (int32)i.hi;
      retint.lo = i.lo;
      return retint;
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


  #endif // NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
#endif // defined NA_TYPE_INT64


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
