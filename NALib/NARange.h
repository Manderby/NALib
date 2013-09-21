
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.


// ///////////////////////////
// This file contains many functions helping the programmer to work with
// comparing values and ranges. In NALib, ranges are simply defined by a
// start value and an end value and are considered to be one-dimensional.
// The two-dimensional equivalents would be NAPos, NASize and NARect.
//
// Most of the functions here could be written as macros. But in NALib, they
// are explicitely implemented as inline functions. This helps when debugging
// and reduces unwanted implicit arithmetic conversions.
//
// The range functions have been moved to this separate file because they are
// not especially difficult to understand but take a lot of code writing. You
// are free to explore all the implementation but there really isn't much
// to see. Seriously.
// ///////////////////////////


#ifndef NA_RANGE_INCLUDED
#define NA_RANGE_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NASystem.h"
#include "NAMathConstants.h"


// Min and Max return the minimum or maximum of two values.
NA_INLINE_API int32        naMini32(int32  a,      int32  b);
NA_INLINE_API float        naMinf  (float  a,      float  b);
NA_INLINE_API double       naMin   (double a,      double b);

NA_INLINE_API int32        naMaxi32(int32  a,      int32  b);
NA_INLINE_API float        naMaxf  (float  a,      float  b);
NA_INLINE_API double       naMax   (double a,      double b);



// naAlmostZero returns NA_TRUE, if the ABSOLUTE discance between x and 0 is
// smaller than NA_SINGULARITY.
NA_INLINE_API NABool naAlmostZerof(float  x);
NA_INLINE_API NABool naAlmostZero (double x);

// naAlmostOne returns NA_TRUE, if the ABSOLUTE discance between x and 1 is
// smaller than NA_SINGULARITY.
NA_INLINE_API NABool naAlmostOnef(float  x);
NA_INLINE_API NABool naAlmostOne (double x);

// naAlmost returns NA_TRUE, if the RELATIVE distance between x and y is
// smaller than NA_SINGULARITY. If y is close to 0, the absolute distance is
// checked.
NA_INLINE_API NABool naAlmostf(float  x,       float  y);
NA_INLINE_API NABool naAlmost (double x,       double y);



// naInside returns true, if x is inside the span depicted by a and b. The
// value a should be smaller than b. The letters I and E depict whether a or b
// are compared inclusive or exclusive. For example:
// naInsideIE(3, 7, x) corresponds to a range of [3,7).

// inclusive, inclusive
NA_INLINE_API NABool naInsideIIf(float  a, float  b, float  x);
NA_INLINE_API NABool naInsideII (double a, double b, double x);
NA_INLINE_API NABool naInsidei  (NAInt  a, NAInt  b, NAInt  x);
NA_INLINE_API NABool naInsidei32(int32  a, int32  b, int32  x);
NA_INLINE_API NABool naInsidei64(int64  a, int64  b, int64  x);

// inclusive, exclusive
NA_INLINE_API NABool naInsideIEf(float  a, float  b, float  x);
NA_INLINE_API NABool naInsideIE (double a, double b, double x);

// exclusive, inclusive
NA_INLINE_API NABool naInsideEIf(float  a, float  b, float  x);
NA_INLINE_API NABool naInsideEI (double a, double b, double x);

// exclusive, exclusive
NA_INLINE_API NABool naInsideEEf(float  a, float  b, float  x);
NA_INLINE_API NABool naInsideEE (double a, double b, double x);

// naAlmostInside checks if the given value is almost inside the given values.
// Meaning, a small relative margin of NA_SINGULARITY on both sides is
// considered. Takes into consideration that a or b might be negative.
NA_INLINE_API NABool naAlmostInsidef(float  a, float  b, float  x);
NA_INLINE_API NABool naAlmostInside (double a, double b, double x);


// naInNorm returns true, if x is inside a normed range. The following norms
// are impmemented:
// naInNormZI: [ 0, 1]
NA_INLINE_API NABool naInNormZIf(float x);
NA_INLINE_API NABool naInNormZI (double x);

// naInNormZE: [ 0, 1)
NA_INLINE_API NABool naInNormZEf(float x);
NA_INLINE_API NABool naInNormZE (double x);

// naInNormII: [-1, 1]
NA_INLINE_API NABool naInNormIIf(float x);
NA_INLINE_API NABool naInNormII (double x);

// naInNormIE: [-1, 1)
NA_INLINE_API NABool naInNormIEf(float x);
NA_INLINE_API NABool naInNormIE (double x);

// naInNormEE: (-1, 1)
NA_INLINE_API NABool naInNormEEf(float x);
NA_INLINE_API NABool naInNormEE (double x);























// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


// Note that the smaller- or greater-operator often is faster than the
// smallerequal- or greaterequal-operator.
// (a<=b) will always be written as !(a>b)
// (a>=b) will always be written as !(a<b)



NA_INLINE_API int32 naMini32(int32 a, int32 b){
  return (a<b)?a:b;
}
NA_INLINE_API float naMinf(float a, float b){
  return (a<b)?a:b;
}
NA_INLINE_API double naMin (double a, double b){
  return (a<b)?a:b;
}


NA_INLINE_API int32 naMaxi32(int32 a, int32 b){
  return (a>b)?a:b;
}
NA_INLINE_API float naMaxf(float a, float b){
  return (a>b)?a:b;
}
NA_INLINE_API double naMax (double a, double b){
  return (a>b)?a:b;
}


NA_INLINE_API NABool naAlmostZerof (float x){
  return ((x < NA_SINGULARITYf) && (x > -NA_SINGULARITYf));
}
NA_INLINE_API NABool naAlmostZero  (double x){
  return ((x < NA_SINGULARITY) && (x > -NA_SINGULARITY));
}

NA_INLINE_API NABool naAlmostOnef (float x){
  return ((x < NA_SUP_NORMf) && (x > NA_SUB_NORMf));
}
NA_INLINE_API NABool naAlmostOne  (double x){
  return ((x < NA_SUP_NORM) && (x > NA_SUB_NORM));
}


NA_INLINE_API NABool naAlmostf(float x, float y){
  if(naAlmostZerof(y)){
    if(naAlmostZerof(x)){
      return NA_TRUE;
    }else{
      return naAlmostZerof(x-y);
    }
  }else{
    float relativeerror = x/y;
    if(relativeerror < 0.f){
      return naAlmostOnef(-relativeerror);
    }else{
      return naAlmostOnef(relativeerror);
    }
  }
}
NA_INLINE_API NABool naAlmost(double x, double y){
  if(naAlmostZero(y)){
    if(naAlmostZero(x)){
      return NA_TRUE;
    }else{
      return naAlmostZero(x-y);
    }
  }else{
    double relativeerror = x/y;
    if(relativeerror < 0.){
      return naAlmostOne(-relativeerror);
    }else{
      return naAlmostOne(relativeerror);
    }
  }
}



// inclusive, inclusive
NA_INLINE_API NABool naInsideIIf(float a, float b, float x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsideIIf", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_INLINE_API NABool naInsideII(double a, double b, double x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsideII", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_INLINE_API NABool naInsidei(NAInt a, NAInt b, NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naInsidei32(a, b, x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naInsidei64(a, b, x);
#endif
}
NA_INLINE_API NABool naInsidei32(int32 a, int32 b, int32 x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsidei32", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_INLINE_API NABool naInsidei64(int64 a, int64 b, int64 x){
  #ifndef NDEBUG
    if(a>b)
      naError("naInsidei64", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}



// inclusive, exclusive
NA_INLINE_API NABool naInsideIEf(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideIEf", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}
NA_INLINE_API NABool naInsideIE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideIE", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}



// exclusive, inclusive
NA_INLINE_API NABool naInsideEIf(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideEIf", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}
NA_INLINE_API NABool naInsideEI(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideEI", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}


// exclusive, exclusive
NA_INLINE_API NABool naInsideEEf(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideEEf", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}
NA_INLINE_API NABool naInsideEE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a<b))
      naError("naInsideEE", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}


// Note that for the naAlmostInside functions, naInsideEE is used after
// adjusting the range. This cound be anything else but as this is a function
// which only needs to be almost accurate, the one function is chosen which is
// simplest.
NA_INLINE_API NABool naAlmostInsidef(float x, float a, float b){
  #ifndef NDEBUG
    if(a>b)
      naError("naAlmostInsidef", "a is greater b.");
  #endif
  a *= (a < 0.f) ? NA_SUP_NORMf : NA_SUB_NORMf;
  b *= (b < 0.f) ? NA_SUB_NORMf : NA_SUP_NORMf;
  return naInsideEEf(a, b, x);
}
NA_INLINE_API NABool naAlmostInside(double x, double a, double b){
  #ifndef NDEBUG
    if(a>b)
      naError("naAlmostInside", "a is greater b.");
  #endif
  a *= (a < 0.) ? NA_SUP_NORM : NA_SUB_NORM;
  b *= (b < 0.) ? NA_SUB_NORM : NA_SUP_NORM;
  return naInsideEE(a, b, x);
}


// naInNormZI: [ 0, 1]
NA_INLINE_API NABool naInNormZIf(float x){
  return !((x < 0.f) || (x > 1.f));
}
NA_INLINE_API NABool naInNormZI (double x){
  return !((x < 0.) || (x > 1.));
}


// naInNormZE: [ 0, 1)
NA_INLINE_API NABool naInNormZEf(float x){
  return (!(x < 0.f) && (x < 1.f));
}
NA_INLINE_API NABool naInNormZE (double x){
  return (!(x < 0.) && (x < 1.));
}


// naInNormII: [-1, 1]
NA_INLINE_API NABool naInNormIIf(float x){
  return !((x < -1.f) || (x > 1.f));
}
NA_INLINE_API NABool naInNormII (double x){
  return !((x < -1.) || (x > 1.));
}


// naInNormIE: [-1, 1)
NA_INLINE_API NABool naInNormIEf(float x){
  return (!(x < -1.f) && (x < 1.f));
}
NA_INLINE_API NABool naInNormIE (double x){
  return (!(x < -1.) && (x < 1.));
}


// naInNormEE: (-1, 1)
NA_INLINE_API NABool naInNormEEf(float x){
  return ((x > -1.f) && (x < 1.f));
}
NA_INLINE_API NABool naInNormEE (double x){
  return ((x > -1.) && (x < 1.));
}




#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_RANGE_INCLUDED


// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
