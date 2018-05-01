
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NASystem.h"


#if !defined NA_TYPE_INT64
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
      NAInt64 retint;
      // note: this is somewhat cumbersome. Do it with bit manipulation. todo.
      retint.hi = (int32)(d / naMakeDoubleWithExponent(32));
      if(d < 0){
        retint.lo = (uint32)(d + ((double)retint.hi * naMakeDoubleWithExponent(32)));
      }else{
        retint.lo = (uint32)(d - ((double)retint.hi * naMakeDoubleWithExponent(32)));
      }
      return retint;
    }
    
    
    
    NA_IDEF NAInt64 naIncInt64(NAInt64 i){
      return naCastUInt64ToInt64(naIncUInt64(naCastInt64ToUInt64(i)));
    }
    NA_IDEF NAInt64 naDecInt64(NAInt64 i){
      return naCastUInt64ToInt64(naDecUInt64(naCastInt64ToUInt64(i)));
    }
    NA_IDEF NAInt64 naNegInt64(NAInt64 i){
      return naAddInt64(naNotInt64(i), naMakeInt64WithLo(1));
    }
    NA_IDEF NAInt64 naAddInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naAddUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naSubInt64(NAInt64 a, NAInt64 b){
      return naCastUInt64ToInt64(naSubUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF NAInt64 naMulInt64(NAInt64 a, NAInt64 b){
      NAInt64 retint;
      NAInt64 signa = naSigni64(a);
      NAInt64 signb = naSigni64(a);
      if(signa){naNegInt64(a);}
      if(signb){naNegInt64(b);}
      NAUInt64 retuint = naMulUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retuint.hi &= ~NA_VALUE32_SIGN_MASK;
      retint = naCastUInt64ToInt64(retuint);
      if(!naEqualInt64(signa, signb)){naNegInt64(retint);}
      return retint;
    }
    NA_IDEF NAInt64 naDivInt64(NAInt64 a, NAInt64 b){
      NAInt64 retint;
      NAInt64 signa = naSigni64(a);
      NAInt64 signb = naSigni64(a);
      if(signa){naNegInt64(a);}
      if(signb){naNegInt64(b);}
      NAUInt64 retuint = naDivUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retuint.hi &= ~NA_VALUE32_SIGN_MASK;
      retint = naCastUInt64ToInt64(retuint);
      if(!naEqualInt64(signa, signb)){naNegInt64(retint);}
      return retint;
    }
    NA_IDEF NAInt64 naModInt64(NAInt64 a, NAInt64 b){
      NAInt64 retint;
      NAInt64 signa = naSigni64(a);
      NAInt64 signb = naSigni64(a);
      if(signa){naNegInt64(a);}
      if(signb){naNegInt64(b);}
      NAUInt64 retuint = naModUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
      retuint.hi &= ~NA_VALUE32_SIGN_MASK;
      retint = naCastUInt64ToInt64(retuint);
      if(signa){naNegInt64(retint);}
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
      if(n < 0){return naShlInt64(a, -n);}
      if(n <= 32){
        retint.lo = a.lo >> n;
        retint.lo |= (uint32)(a.hi << (32 - n));
        retint.hi = a.hi >> n;
      }else{
        retint.lo = (uint32)(a.hi >> (n - 32));
        retint.hi = (int32)(a.hi >> 31 >> 1); // Sign preservation!
        // The splitting in >> 31 and >> 1 actually is required to silence
        // some compiler warnings.
      }
      return retint;
    }
    

    
    NA_IDEF NABool naEqualInt64(NAInt64 a, NAInt64 b){
      return naEqualUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
    }
    NA_IDEF NABool naGreaterInt64(NAInt64 a, NAInt64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo < b.lo) : (a.lo > b.lo))));
    }
    NA_IDEF NABool naGreaterEqualInt64(NAInt64 a, NAInt64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo <= b.lo) : (a.lo >= b.lo))));
    }
    NA_IDEF NABool naSmallerInt64(NAInt64 a, NAInt64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo > b.lo) : (a.lo < b.lo))));
    }
    NA_IDEF NABool naSmallerEqualInt64(NAInt64 a, NAInt64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo >= b.lo) : (a.lo <= b.lo))));
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
      return (double)i.hi * naMakeDoubleWithExponent(32) + ((i.hi<0) ? -(double)i.lo : (double)i.lo);
    }



    NA_IDEF NAUInt64 naMakeUInt64(uint32 hi, uint32 lo){
      NAUInt64 retint;
      retint.hi = hi;
      retint.lo = lo;
      return retint;
    }
    NA_IDEF NAUInt64 naMakeUInt64WithLo(uint32 lo){
      NAUInt64 retint;
      retint.hi = 0x0;
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
    
    
    
    NA_IDEF NAUInt64 naIncUInt64(NAUInt64 i){
      return naAddUInt64(i, naMakeUInt64WithLo(1));
    }
    NA_IDEF NAUInt64 naDecUInt64(NAUInt64 i){
      return naSubUInt64(i, naMakeUInt64WithLo(1));
    }
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
      
      uint32 a0 = a.lo & ((1<<16)-1);
      uint32 a1 = a.lo >> 16;
      uint32 a2 = a.hi & ((1<<16)-1);
      uint32 a3 = a.hi >> 16;
      uint32 b0 = b.lo & ((1<<16)-1);
      uint32 b1 = b.lo >> 16;
      uint32 b2 = b.hi & ((1<<16)-1);
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
      div = NA_NULL_64u;
      rem = a;
      tmpb = b;
      // Make the dividend big enough
      while(naGreaterEqual(rem, tmpb)){naShlUInt64(tmpb, 1);}
      // shift right once.
      naShrUInt64(tmpb, 1);
      while(1){
        if(naSmaller(rem, b)){break;}
        naShlInt64(div);
        if(naGreaterEqual(rem, tmpb)){
          naOrUInt64(div, NA_ONE_64u);
          naSubUInt64(rem, tmpb);
        }
        naShrUInt64(tmpb, 1);
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
        retint.lo = 0;
      }
      return retint;
    }
    NA_IDEF NAUInt64 naShrUInt64(NAUInt64 a, int n){
      NAUInt64 retint;
      if(n < 0){return naShlUInt64(a, -n);}
      if(n <= 32){
        retint.lo = a.lo >> n;
        retint.lo |= a.hi << (32 - n);
        retint.hi = a.hi >> n;
      }else{
        retint.lo = a.hi >> (n - 32);
        retint.hi = 0;
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
      return (uint32)i.lo;
    }
    NA_IDEF double naCastUInt64ToDouble(NAUInt64 i){
      return (double)i.hi * naMakeDoubleWithExponent(32) + (double)i.lo;
    }


  #endif
#endif


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
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
