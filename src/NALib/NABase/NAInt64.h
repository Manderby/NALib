
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// Do not include this file directly! It will automatically be included when
// including "NAInteger.h"


#if defined NA_TYPE_INT64

  #define naMakeInt64WithLiteralLo(lo)  (lo ## LL)
  #define naMakeUInt64WithLiteralLo(lo) (lo ## uLL)

  #define naMakeInt64WithLo(lo)         ((int64)(lo))
  #define naMakeInt64WithDouble(d)      ((int64)(d))
  
  #define naNegInt64(i)                 (-(i))
  #define naIncInt64(i)                 ((i)++)
  #define naDecInt64(i)                 ((i)--)
  #define naAddInt64(a, b)              ((a) + (b))
  #define naSubInt64(a, b)              ((a) - (b))
  #define naMulInt64(a, b)              ((a) * (b))
  #define naDivInt64(a, b)              ((a) / (b))
  #define naModInt64(a, b)              ((a) % (b))
  
  #define naNotInt64(i)                 (~(i))
  #define naOrInt64(a, b)               ((a) | (b))
  #define naAndInt64(a, b)              ((a) & (b))
  #define naXorInt64(a, b)              ((a) ^ (b))
  #define naShlInt64(a, n)              ((a) << (n))
  #define naShrInt64(a, n)              ((a) >> (n))
  
  #define naEqualInt64(a, b)            ((a) == (b))
  #define naGreaterInt64(a, b)          ((a) > (b))
  #define naGreaterEqualInt64(a, b)     ((a) >= (b))
  #define naSmallerInt64(a, b)          ((a) < (b))
  #define naSmallerEqualInt64(a, b)     ((a) <= (b))

  #define naCastInt64ToUInt8(i)         ((uint8)(i))
  #define naCastInt64ToUInt16(i)        ((uint16)(i))
  #define naCastInt64ToUInt32(i)        ((uint32)(i))
  #define naCastInt64ToUInt64(i)        ((uint64)(i))
  #define naCastInt64ToInt8(i)          ((int8)(i))
  #define naCastInt64ToInt16(i)         ((int16)(i))
  #define naCastInt64ToInt32(i)         ((int32)(i))
  #define naCastInt64ToDouble(i)        ((double)(i))

  #define naMakeUInt64(hi, lo)          ((uint64)(((uint64)(hi) << 32) | (lo)))
  #define naMakeUInt64WithLo(lo)        ((uint64)(lo))
  #define naMakeUInt64WithDouble(d)     ((uint64)(d))

  #define naIncUInt64(i)                ((i)++)
  #define naDecUInt64(i)                ((i)--)
  #define naAddUInt64(a, b)             ((a) + (b))
  #define naSubUInt64(a, b)             ((a) - (b))
  #define naMulUInt64(a, b)             ((a) * (b))
  #define naDivUInt64(a, b)             ((a) / (b))
  #define naModUInt64(a, b)             ((a) % (b))
  
  #define naNotUInt64(i)                (~(i))
  #define naOrUInt64(a, b)              ((a) | (b))
  #define naAndUInt64(a, b)             ((a) & (b))
  #define naXorUInt64(a, b)             ((a) ^ (b))
  #define naShlUInt64(a, n)             ((a) << (n))
  #define naShrUInt64(a, n)             ((a) >> (n))
  
  #define naEqualUInt64(a, b)           ((a) == (b))
  #define naGreaterUInt64(a, b)         ((a) > (b))
  #define naGreaterEqualUInt64(a, b)    ((a) >= (b))
  #define naSmallerUInt64(a, b)         ((a) < (b))
  #define naSmallerEqualUInt64(a, b)    ((a) <= (b))
  
  #define naCastUInt64ToInt8(i)         ((int8)(i))
  #define naCastUInt64ToInt16(i)        ((int16)(i))
  #define naCastUInt64ToInt32(i)        ((int32)(i))
  #define naCastUInt64ToInt64(i)        ((int64)(i))
  #define naCastUInt64ToUInt8(i)        ((uint8)(i))
  #define naCastUInt64ToUInt16(i)       ((uint16)(i))
  #define naCastUInt64ToUInt32(i)       ((uint32)(i))
  #define naCastUInt64ToDouble(i)       ((double)(i))

#else

  #if NA_SIGNED_INTEGER_ENCODING != NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
  
    #error "The int64 emulation is not provided for the current signed integer encoding"
  
  #else
  
    #define NA_UINT64_MAX  naMakeUInt64(NA_UINT32_MAX, NA_UINT32_MAX)
    #define NA_INT64_MAX   naCastUInt64ToInt64(naMakeUInt64((uint32)NA_INT32_MAX, NA_UINT32_MAX))
    #define NA_INT64_MIN   naCastUInt64ToInt64(naMakeUInt64((uint32)NA_INT32_MIN, 0x0))
    typedef struct NAInt64  int64;
    typedef struct NAUInt64 uint64;
    #if NA_SYSTEM_ENDIANNESS == NA_ENDIANNESS_BIG
      struct NAInt64{
        int32  hi;
        uint32 lo;
      };
      struct NAUInt64{
        uint32 hi;
        uint32 lo;
      };
      #define naMakeInt64WithLiteralLo(lo)   {0,lo}
      #define naMakeUInt64WithLiteralLo(lo)  {0,lo}
    #else
      struct NAInt64{
        uint32 lo;
        int32  hi;
      };
      struct NAUInt64{
        uint32 lo;
        uint32 hi;
      };
      #define naMakeInt64WithLiteralLo(lo)   {lo,0}
      #define naMakeUInt64WithLiteralLo(lo)  {lo,0}
  #endif

    NA_IAPI int64  naMakeInt64WithLo(int32 lo);
    NA_IAPI int64  naMakeInt64WithDouble(double d);
    
    NA_IAPI int64  naNegInt64(int64 i);
    NA_IAPI int64  naIncInt64(int64 i);
    NA_IAPI int64  naDecInt64(int64 i);
    NA_IAPI int64  naAddInt64(int64 a, int64 b);
    NA_IAPI int64  naSubInt64(int64 a, int64 b);
    NA_IAPI int64  naMulInt64(int64 a, int64 b);
    NA_IAPI int64  naDivInt64(int64 a, int64 b);
    NA_IAPI int64  naModInt64(int64 a, int64 b);
    
    NA_IAPI int64  naNotInt64(int64 i);
    NA_IAPI int64  naOrInt64 (int64 a, int64 b);
    NA_IAPI int64  naAndInt64(int64 a, int64 b);
    NA_IAPI int64  naXorInt64(int64 a, int64 b);
    NA_IAPI int64  naShlInt64(int64 a, int n);
    NA_IAPI int64  naShrInt64(int64 a, int n);
    
    NA_IAPI NABool naEqualInt64        (int64 a, int64 b);
    NA_IAPI NABool naGreaterInt64      (int64 a, int64 b);
    NA_IAPI NABool naGreaterEqualInt64 (int64 a, int64 b);
    NA_IAPI NABool naSmallerInt64      (int64 a, int64 b);
    NA_IAPI NABool naSmallerEqualInt64 (int64 a, int64 b);
    
    NA_IAPI uint8  naCastInt64ToUInt8 (int64 i);
    NA_IAPI uint16 naCastInt64ToUInt16(int64 i);
    NA_IAPI uint32 naCastInt64ToUInt32(int64 i);
    NA_IAPI uint64 naCastInt64ToUInt64(int64 i);
    NA_IAPI int8   naCastInt64ToInt8  (int64 i);
    NA_IAPI int16  naCastInt64ToInt16 (int64 i);
    NA_IAPI int32  naCastInt64ToInt32 (int64 i);
    NA_IAPI double naCastInt64ToDouble(int64 i);

    NA_IAPI uint64 naMakeUInt64(uint32 hi, uint32 lo);
    NA_IAPI uint64 naMakeUInt64WithLo(uint32 lo);
    NA_IAPI uint64 naMakeUInt64WithDouble(double d);
    
    NA_IAPI uint64 naIncUInt64(uint64 i);
    NA_IAPI uint64 naDecUInt64(uint64 i);
    NA_IAPI uint64 naAddUInt64(uint64 a, uint64 b);
    NA_IAPI uint64 naAddUInt64(uint64 a, uint64 b);
    NA_IAPI uint64 naSubUInt64(uint64 a, uint64 b);
    NA_IAPI uint64 naMulUInt64(uint64 a, uint64 b);
    NA_IAPI uint64 naDivUInt64(uint64 a, uint64 b);
    NA_IAPI uint64 naModUInt64(uint64 a, uint64 b);
    
    NA_IAPI uint64 naNotUInt64(uint64 i);
    NA_IAPI uint64 naOrUInt64 (uint64 a, uint64 b);
    NA_IAPI uint64 naAndUInt64(uint64 a, uint64 b);
    NA_IAPI uint64 naXorUInt64(uint64 a, uint64 b);
    NA_IAPI uint64 naShlUInt64(uint64 a, int n);
    NA_IAPI uint64 naShrUInt64(uint64 a, int n);
    
    NA_IAPI NABool naEqualUInt64       (uint64 a, uint64 b);
    NA_IAPI NABool naGreaterUInt64     (uint64 a, uint64 b);
    NA_IAPI NABool naGreaterEqualUInt64(uint64 a, uint64 b);
    NA_IAPI NABool naSmallerUInt64     (uint64 a, uint64 b);
    NA_IAPI NABool naSmallerEqualUInt64(uint64 a, uint64 b);
    
    NA_IAPI int8   naCastUInt64ToInt8  (uint64 i);
    NA_IAPI int16  naCastUInt64ToInt16 (uint64 i);
    NA_IAPI int32  naCastUInt64ToInt32 (uint64 i);
    NA_IAPI int64  naCastUInt64ToInt64 (uint64 i);
    NA_IAPI uint8  naCastUInt64ToUInt8 (uint64 i);
    NA_IAPI uint16 naCastUInt64ToUInt16(uint64 i);
    NA_IAPI uint32 naCastUInt64ToUInt32(uint64 i);
    NA_IAPI double naCastUInt64ToDouble(uint64 i);



    NA_IDEF int64 naMakeInt64WithLo(int32 lo){
      int64 retint;
      retint.hi = 0x0;  // todo
      retint.lo = (uint32)lo;
      return retint;
    }
    NA_IDEF int64 naMakeInt64WithDouble(double d){
      int64 retint;
      retint.hi = 0x0;  // todo
      retint.lo = (uint32)d;
      return retint;
    }
    
    
    
    NA_IDEF int64 naIncInt64(int64 i){
      return naCastUInt64ToInt64(naIncUInt64(naCastInt64ToUInt64(i)));
    }
    NA_IDEF int64 naDecInt64(int64 i){
      return naCastUInt64ToInt64(naDecUInt64(naCastInt64ToUInt64(i)));
    }
    NA_IDEF int64 naNegInt64(int64 i){
      return naAddInt64(naNotInt64(i), naMakeInt64WithLo(1));
    }
    NA_IDEF int64 naAddInt64(int64 a, int64 b){
      return naCastUInt64ToInt64(naAddUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF int64 naSubInt64(int64 a, int64 b){
      return naCastUInt64ToInt64(naSubUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF int64 naMulInt64(int64 a, int64 b){
      int64 retint;
      retint.hi = a.hi * b.hi;  // todo
      retint.lo = a.lo * b.lo;
      return retint;
    }
    NA_IDEF int64 naDivInt64(int64 a, int64 b){
      int64 retint;
      retint.hi = a.hi / b.hi;  // todo
      retint.lo = a.lo / b.lo;
      return retint;
    }
    NA_IDEF int64 naModInt64(int64 a, int64 b){
      int64 retint;
      retint.hi = a.hi % b.hi;  // todo
      retint.lo = a.lo % b.lo;
      return retint;
    }
    
    
    
    NA_IDEF int64 naNotInt64(int64 i){
      return naCastUInt64ToInt64(naNotUInt64(naCastInt64ToUInt64(i)));
    }
    NA_IDEF int64 naOrInt64(int64 a, int64 b){
      return naCastUInt64ToInt64(naOrUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF int64 naAndInt64(int64 a, int64 b){
      return naCastUInt64ToInt64(naAndUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF int64 naXorInt64(int64 a, int64 b){
      return naCastUInt64ToInt64(naXorUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b)));
    }
    NA_IDEF int64 naShlInt64(int64 a, int n){
      return naCastUInt64ToInt64(naShlUInt64(naCastInt64ToUInt64(a), n));
    }
    NA_IDEF int64 naShrInt64(int64 a, int n){
      int64 retint;
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
    

    
    NA_IDEF NABool naEqualInt64(int64 a, int64 b){
      return naEqualUInt64(naCastInt64ToUInt64(a), naCastInt64ToUInt64(b));
    }
    NA_IDEF NABool naGreaterInt64(int64 a, int64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo < b.lo) : (a.lo > b.lo))));
    }
    NA_IDEF NABool naGreaterEqualInt64(int64 a, int64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo <= b.lo) : (a.lo >= b.lo))));
    }
    NA_IDEF NABool naSmallerInt64(int64 a, int64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo > b.lo) : (a.lo < b.lo))));
    }
    NA_IDEF NABool naSmallerEqualInt64(int64 a, int64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && ((a.hi < 0) ? (a.lo >= b.lo) : (a.lo <= b.lo))));
    }
    


    NA_IDEF uint8 naCastInt64ToUInt8(int64 i){
      return naCastUInt64ToUInt8(naCastInt64ToUInt64(i));
    }
    NA_IDEF uint16 naCastInt64ToUInt16(int64 i){
      return naCastUInt64ToUInt16(naCastInt64ToUInt64(i));
    }
    NA_IDEF uint32 naCastInt64ToUInt32(int64 i){
      return naCastUInt64ToUInt32(naCastInt64ToUInt64(i));
    }
    NA_IDEF uint64 naCastInt64ToUInt64(int64 i){
      uint64 retint;
      retint.hi = (uint32)i.hi;
      retint.lo = i.lo;
      return retint;
    }
    NA_IDEF int8 naCastInt64ToInt8(int64 i){
      return naCastUInt64ToInt8(naCastInt64ToUInt64(i));
    }
    NA_IDEF int16 naCastInt64ToInt16(int64 i){
      return naCastUInt64ToInt16(naCastInt64ToUInt64(i));
    }
    NA_IDEF int32 naCastInt64ToInt32(int64 i){
      return naCastUInt64ToInt32(naCastInt64ToUInt64(i));
    }
    NA_IDEF double naCastInt64ToDouble(int64 i){
      return (double)i.lo;  // todo
    }



    NA_IDEF uint64 naMakeUInt64(uint32 hi, uint32 lo){
      uint64 retint;
      retint.hi = hi;
      retint.lo = lo;
      return retint;
    }
    NA_IDEF uint64 naMakeUInt64WithLo(uint32 lo){
      uint64 retint;
      retint.hi = 0x0;
      retint.lo = lo;
      return retint;
    }
    NA_IDEF uint64 naMakeUInt64WithDouble(double d){
      uint64 retint;
      retint.hi = 0x0;  // todo
      retint.lo = (uint32)d;
      return retint;
    }
    
    
    
    NA_IDEF uint64 naIncUInt64(uint64 i){
      return naAddUInt64(i, naMakeUInt64WithLo(1));
    }
    NA_IDEF uint64 naDecUInt64(uint64 i){
      return naSubUInt64(i, naMakeUInt64WithLo(1));
    }
    NA_IDEF uint64 naAddUInt64(uint64 a, uint64 b){
      uint64 retint;
      retint.lo = a.lo + b.lo;
      retint.hi = a.hi + b.hi;
      retint.hi += (retint.lo < a.lo); // add a carry if there was an overflow.
      return retint;
    }
    NA_IDEF uint64 naSubUInt64(uint64 a, uint64 b){
      return naAddUInt64(a, naCastInt64ToUInt64(naNegInt64(naCastUInt64ToInt64(b))));
    }
    NA_IDEF uint64 naMulUInt64(uint64 a, uint64 b){
      uint64 retint;
      retint.hi = a.hi * b.hi;  // todo
      retint.lo = a.lo * b.lo;
      return retint;
    }
    NA_IDEF uint64 naDivUInt64(uint64 a, uint64 b){
      uint64 retint;
      retint.hi = a.hi / b.hi;  // todo
      retint.lo = a.lo / b.lo;
      return retint;
    }
    NA_IDEF uint64 naModUInt64(uint64 a, uint64 b){
      uint64 retint;
      retint.hi = a.hi % b.hi;  // todo
      retint.lo = a.lo % b.lo;
      return retint;
    }
    
    
    
    NA_IDEF uint64  naNotUInt64(uint64 i){
      uint64 retint;
      retint.hi = ~i.hi;
      retint.lo = ~i.lo;
      return retint;
    }
    NA_IDEF uint64  naOrUInt64 (uint64 a, uint64 b){
      uint64 retint;
      retint.hi = a.hi | b.hi;
      retint.lo = a.lo | b.lo;
      return retint;
    }
    NA_IDEF uint64  naAndUInt64(uint64 a, uint64 b){
      uint64 retint;
      retint.hi = a.hi & b.hi;
      retint.lo = a.lo & b.lo;
      return retint;
    }
    NA_IDEF uint64  naXorUInt64(uint64 a, uint64 b){
      uint64 retint;
      retint.hi = a.hi ^ b.hi;
      retint.lo = a.lo ^ b.lo;
      return retint;
    }
    NA_IDEF uint64 naShlUInt64(uint64 a, int n){
      uint64 retint;
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
    NA_IDEF uint64 naShrUInt64(uint64 a, int n){
      uint64 retint;
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
    
    
    
    NA_IDEF NABool naEqualUInt64(uint64 a, uint64 b){
      return ((a.hi == b.hi) && (a.lo == b.lo));
    }
    NA_IDEF NABool naGreaterUInt64(uint64 a, uint64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo > b.lo)));
    }
    NA_IDEF NABool naGreaterEqualUInt64(uint64 a, uint64 b){
      return ((a.hi > b.hi) || ((a.hi == b.hi) && (a.lo >= b.lo)));
    }
    NA_IDEF NABool naSmallerUInt64(uint64 a, uint64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo < b.lo)));
    }
    NA_IDEF NABool naSmallerEqualUInt64(uint64 a, uint64 b){
      return ((a.hi < b.hi) || ((a.hi == b.hi) && (a.lo <= b.lo)));
    }
    
    
    
    NA_IDEF int8 naCastUInt64ToInt8(uint64 i){
      return (int8)i.lo;
    }
    NA_IDEF int16 naCastUInt64ToInt16(uint64 i){
      return (int16)i.lo;
    }
    NA_IDEF int32 naCastUInt64ToInt32(uint64 i){
      return (int32)i.lo;
    }
    NA_IDEF int64 naCastUInt64ToInt64(uint64 i){
      int64 retint;
      retint.hi = (int32)i.hi;
      retint.lo = i.lo;
      return retint;
    }
    NA_IDEF uint8 naCastUInt64ToUInt8(uint64 i){
      return (uint8)i.lo;
    }
    NA_IDEF uint16 naCastUInt64ToUInt16(uint64 i){
      return (uint16)i.lo;
    }
    NA_IDEF uint32 naCastUInt64ToUInt32(uint64 i){
      return (uint32)i.lo;
    }
    NA_IDEF double naCastUInt64ToDouble(uint64 i){
      return (double)i.lo;  // todo
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
