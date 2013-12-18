
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_MATH_OPERATORS_INCLUDED
#define NA_MATH_OPERATORS_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif

#include "NASystem.h"

// /////////////////////////////////
// This file contains many standard math operators. All functions are defined
// inline. Many checks are performed like division by zero which may slow the
// implementation down. You can disable these checks by defining NDEBUG.
//
// Note that there exist implementations for float and double, but not for
// long double. The reason for this is that the author has observed that the
// additional accuracy teases a programmer to believe that his results become
// more accurate just by introducing long doubles. Often times though the
// systematical or even the numerical errors introduced by algorithms are much
// larger than the accuracy of long doubles. Even more so, they are not
// standardized like floats and doubles and therefore might behave differently
// depending on the system. And they usually are quite slow and take a lot of
// space in the code to write.
//
// If you really, really need long doubles, you will most probably already look
// deep inside the dark pit of numerical uncertainties and likely won't use
// this library anymore, because you already have something much more refined.
// /////////////////////////////////


// Returns the sign. x<0 returns -1, x>=0 returns 1. Never returns 0!
// You can use the result of this function safely as a multiplication factor.
// You can also subtract the result of this function from the comma ASCII
// character and get a string representation of the sign:
// NAUTF8Char signASCII = ',' - naSigni(-42);
NA_IAPI NAInt        naSigni  (NAInt x);
NA_IAPI int32        naSigni32(int32 x);
NA_IAPI int64        naSigni64(int64 x);

// Returns true if the given number is NaN
NA_IAPI NABool       naIsNaN(double x);

// Returns 1/x
NA_IAPI float        naInvf(float x);
NA_IAPI double       naInv (double x);

// Returns the square root
NA_IAPI float        naSqrtf(float x);
NA_IAPI double       naSqrt(double x);

// Returns the cubic root
NA_IAPI float        naCbrtf(float x);
NA_IAPI double       naCbrt(double x);

// Returns the absolute value
NA_IAPI float        naAbsf  (float  x);
NA_IAPI double       naAbs   (double x);
NA_IAPI NAInt        naAbsi  (NAInt  x);
NA_IAPI int8         naAbsi8 (int8   x);
NA_IAPI int16        naAbsi16(int16  x);
NA_IAPI int32        naAbsi32(int32  x);
NA_IAPI int64        naAbsi64(int64  x);

// Returns the rounded down value
NA_IAPI float        naFloorf(float x);
NA_IAPI double       naFloor(double x);

// Returns the rounded up value
NA_IAPI float        naCeilf(float x);
NA_IAPI double       naCeil(double x);

// Returns the rounded value
NA_IAPI float        naRoundf(float x);
NA_IAPI double       naRound(double x);

// Returns the sinus
NA_IAPI float        naSinf(float x);
NA_IAPI double       naSin(double x);

// Returns the cosinus
NA_IAPI float        naCosf(float x);
NA_IAPI double       naCos(double x);

// Returns the tangens
NA_IAPI float        naTanf(float x);
NA_IAPI double       naTan(double x);

// Returns the arcus sinus
NA_IAPI float        naAsinf(float x);
NA_IAPI double       naAsin(double x);

// Returns the arcus cosinus
NA_IAPI float        naAcosf(float x);
NA_IAPI double       naAcos(double x);

// Returns the arcus tangens in the range [-pi/2, +pi/2]
NA_IAPI float        naAtanf(float x);
NA_IAPI double       naAtan(double x);

// Returns the arcus tangens of y/x in the range [-pi, +pi]
// The angle function does the same, but assumes x and y stored as
// xy[0] and xy[1] respectively.
NA_IAPI float        naAtan2f(float y, float x);
NA_IAPI double       naAtan2(double y, double x);
NA_IAPI float        naAnglef(const float* xy);
NA_IAPI double       naAngle(const double* xy);

// Returns the natural logarithm
NA_IAPI float        naLogf(float x);
NA_IAPI double       naLog(double x);

// Returns the 10-logarithm
NA_IAPI float        naLog10f(float x);
NA_IAPI double       naLog10(double x);

// Returns the 2-logarithm
// The integer variants of log2, return the rounded down log2. In other words
// the power of 2 which is greaterequal than x.
// - 0 returns 0  -> 2^0 = 1
// - 1 returns 0  -> 2^0 = 1
// - 2 returns 1  -> 2^1 = 2
// - 3 returns 2  -> 2^2 = 4
// - 4 returns 2  -> 2^2 = 4
// - 5 returns 3  -> 2^3 = 8
// - ...
NA_IAPI float        naLog2f(float x);
NA_IAPI double       naLog2(double x);
NA_IAPI int32        naLog2i32(int32 x);
NA_IAPI int64        naLog2i64(int64 x);

// Returns the exponential function e^x
NA_IAPI float        naExpf(float x);
NA_IAPI double       naExp(double x);

// Returns the power of 2 function 2^x
NA_IAPI float        naExp2f  (float x);
NA_IAPI double       naExp2   (double x);
NA_IAPI NAInt        naExp2i  (NAInt x);
NA_IAPI int32        naExp2i32(int32 x);
NA_IAPI int64        naExp2i64(int64 x);

// Returns the power function b^x
NA_IAPI float        naPowf(float b, float x);
NA_IAPI double       naPow(double b, double x);

// Returns the the angle converted to radiants
NA_IAPI float        naDegToRadf(float deg);
NA_IAPI double       naDegToRad(double deg);

// Returns the the angle converted to degrees
NA_IAPI float        naRadToDegf(float rad);
NA_IAPI double       naRadToDeg(double rad);

// Angle functions. Assumes either x and y or r and theta to be stored in a
// two dimensional vector.
NA_IAPI void        naPolarToCartesianf(float*  xy, const float*  rtheta);
NA_IAPI void        naPolarToCartesian (double* xy, const double* rtheta);

// Same thing but the other way around.
NA_IAPI void        naCartesianToPolarf(float*  rtheta, const float*  xy);
NA_IAPI void        naCartesianToPolar (double* rtheta, const double* xy);





















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NAMathConstants.h"
#include "NARange.h"


NA_IDEF NAInt naSigni(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naSigni32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naSigni64(x);
#endif
}
NA_IDEF int32 naSigni32(int32 x){
  #if defined NA_SIGNED_INTEGER_USES_TWOS_COMPLEMENT
    return ((x>>31)<<1)+1;
  #else
    return (x<0)?-1:1;
  #endif
}
NA_IDEF int64 naSigni64(int64 x){
  #if defined NA_SIGNED_INTEGER_USES_TWOS_COMPLEMENT
    return ((x>>63)<<1)+1LL;
  #else
    return (x<0)?-1LL:1LL;
  #endif
}



// Note that checking for NaN must be performed with the built-in isnan()-macro
// or -function. Checking x == NA_NAN would return NA_FALSE always.
NA_IAPI NABool naIsNaN(double x){
  return isnan(x);
}

NA_IDEF float naInvf(float x){
  #ifndef NDEBUG
    if(x == 0.f)
      naError("naInvf", "Division by zero.");
    if(naAlmostZerof(x))
      naError("naInvf", "Division by almost zero.");
  #endif
  return 1.f / x;
}
NA_IDEF double naInv(double x){
  #ifndef NDEBUG
    if(x == 0.)
      naError("naInv", "Division by zero.");
    if(naAlmostZero(x))
      naError("naInv", "Division by almost zero.");
  #endif
  return 1. / x;
}


NA_IDEF float naSqrtf(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naSqrtf", "naSqrt of negative number.");}
  #endif
  return sqrtf(x);
}
NA_IDEF double naSqrt(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naSqrt", "naSqrt of negative number.");}
  #endif
  return sqrt(x);
}



NA_IDEF float naCbrtf(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naCbrtf", "naCbrt of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return powf(x, NA_THIRDf);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return cbrtf(x);
  #endif
}

NA_IDEF double naCbrt(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naCbrt", "naCbrt of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return pow(x, NA_THIRD);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return cbrt(x);
  #endif
}


NA_IDEF float naAbsf(float x){
  return fabsf(x);
}
NA_IDEF double naAbs(double x){
  return fabs(x);
}
NA_IDEF NAInt naAbsi(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naAbsi32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naAbsi64(x);
#endif
}
NA_IDEF int8 naAbsi8(int8 x){
  return (int8)abs(x);
}
NA_IDEF int16 naAbsi16(int16 x){
  return (int16)abs(x);
}
NA_IDEF int32 naAbsi32(int32 x){
  return (int32)abs(x);
}
NA_IDEF int64 naAbsi64(int64 x){
  return (int64)llabs(x);
}


NA_IDEF float naFloorf(float x){
  return floorf(x);
}
NA_IDEF double naFloor(double x){
  return floor(x);
}


NA_IDEF float naCeilf(float x){
  return ceilf(x);
}
NA_IDEF double naCeil(double x){
  return ceil(x);
}


NA_IDEF float naRoundf(float x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return floorf(x + .5f);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return roundf(x);
  #endif
}
NA_IDEF double naRound(double x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return floor(x + .5);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return round(x);
  #endif
}


NA_IDEF float naSinf(float x){
  return sinf(x);
}
NA_IDEF double naSin(double x){
  return sin(x);
}

NA_IDEF float naCosf(float x){
  return cosf(x);
}
NA_IDEF double naCos(double x){
  return cos(x);
}

NA_IDEF float naTanf(float x){
  return tanf(x);
}
NA_IDEF double naTan(double x){
  return tan(x);
}



NA_IDEF float naAsinf(float x){
  #ifndef NDEBUG
    if(!naInNormIIf(x)){naError("naAsinf", "naAsin of invalid value.");}
  #endif
  return asinf(x);
}
NA_IDEF double naAsin(double x){
  #ifndef NDEBUG
    if(!naInNormII(x)){naError("naAsin", "naAsin of invalid value.");}
  #endif
  return asin(x);
}


NA_IDEF float naAcosf(float x){
  #ifndef NDEBUG
    if(!naInNormIIf(x)){naError("naAcosf", "naAcos of invalid value.");}
  #endif
  return acosf(x);
}
NA_IDEF double naAcos(double x){
  #ifndef NDEBUG
    if(!naInNormII(x)){naError("naAcos", "naAcos of invalid value.");}
  #endif
  return acos(x);
}


NA_IDEF float naAtanf(float x){
  return atanf(x);
}
NA_IDEF double naAtan(double x){
  return atan(x);
}


NA_IDEF float naAtan2f(float y, float x){
  return atan2f(y, x);
}
NA_IDEF double naAtan2(double y, double x){
  return atan2(y, x);
}
NA_IDEF float naAnglef(const float* xy){
  return naAtan2f(xy[1], xy[0]);
}
NA_IDEF double naAngle(const double* xy){
  return naAtan2(xy[1], xy[0]);
}


NA_IDEF float naExpf(float x){
  return expf(x);
}
NA_IDEF double naExp(double x){
  return exp(x);
}


NA_IDEF float naLogf(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naLogf", "Logarithm of negative number.");}
  #endif
  return logf(x);
}
NA_IDEF double naLog(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naLog", "Logarithm of negative number.");}
  #endif
  return log(x);
}



NA_IDEF float naLog10f(float x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog10f", "Logarithm of negative number.");
  #endif
  return log10f(x);
}
NA_IDEF double naLog10(double x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog10", "Logarithm of negative number.");
  #endif
  return log10(x);
}




NA_IDEF float naLog2f(float x){
  #ifndef NDEBUG
    if(x < 0.f){naError("naLog2f", "Logarithm of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return logf(x) * NA_INV_LOGOF2f;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return log2f(x);
  #endif
}
NA_IDEF double naLog2(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naLog2", "Logarithm of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return log(x) * NA_INV_LOGOF2;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return log2(x);
  #endif
}

NA_IDEF int32 naLog2i32(int32 x){
  int32 retvalue; // Note: declaration before any other code.
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog2i32", "Logarithm of negative number.");
  #endif
  if(x<=0){return 0;}
  x--;
  retvalue = 0;
  while(x){retvalue++; x>>=1;}
  return retvalue;
}
NA_IDEF int64 naLog2i64(int64 x){
  int64 retvalue; // Note: declaration before any other code.
  #ifndef NDEBUG
    if(x < 0)
      naError("naLog2i64", "Logarithm of negative number.");
  #endif
  if(x<=0){return 0;}
  x--;
  retvalue = 0;
  while(x){retvalue++; x>>=1;}
  return retvalue;
}



NA_IDEF float naExp2f(float x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return powf(2.f, x);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return exp2f(x);
  #endif
}
NA_IDEF double naExp2(double x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return pow(2., x);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return exp2(x);
  #endif
}
NA_IDEF NAInt naExp2i(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naExp2i32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naExp2i64(x);
#endif
}
NA_IDEF int32 naExp2i32(int32 x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naExp2i32", "Exponent negative. Will result in 0.");
    if(x >= 32)
      naError("naExp2i32", "Exponent too big. Will result in 0.");
  #endif
  return 1<<x;
}
NA_IDEF int64 naExp2i64(int64 x){
  #ifndef NDEBUG
    if(x < 0)
      naError("naExp2i64", "Exponent negative. Will result in 0.");
    if(x >= 64)
      naError("naExp2i64", "Exponent too big. Will result in 0.");
  #endif
  return 1LL<<x;
}



NA_IDEF float naPowf(float b, float x){
  #ifndef NDEBUG
    if(b < 0.f)
      naError("naPowf", "Base of power function smaller 0.");
  #endif
  return powf(b, x);
}
NA_IDEF double naPow(double b, double x){
  #ifndef NDEBUG
    if(b < 0.)
      naError("naPow", "Base of power function smaller 0.");
  #endif
  return pow(b, x);
}


NA_IDEF float naDegToRadf(float deg){
  return deg * NA_DEGREESf;
}
NA_IDEF double naDegToRad(double deg){
  return deg * NA_DEGREES;
}

NA_IDEF float naRadToDegf(float rad){
  return rad * NA_DEG_PER_RADf;
}
NA_IDEF double naRadToDeg(double rad){
  return rad * NA_DEG_PER_RAD;
}



NA_IDEF void naPolarToCartesianf(float* xy, const float* rtheta){
  float r = rtheta[0];
  xy[0] = r * naCosf(rtheta[1]);
  xy[1] = r * naSinf(rtheta[1]);
}
NA_IDEF void naPolarToCartesian(double* xy, const double* rtheta){
  double r = rtheta[0];
  xy[0] = r * naCos(rtheta[1]);
  xy[1] = r * naSin(rtheta[1]);
}


// Note that the sqrt computation could be done using naLengthV2 but this would
// require NAVectorAlgebra.h to be included.
NA_IDEF void naCartesianToPolarf(float* rtheta, const float* xy){
  rtheta[0] = naSqrtf(xy[0] * xy[0] + xy[1] * xy[1]);
  rtheta[1] = (rtheta[0] == 0.f) ? 0.f : naAnglef(xy);
}
NA_IDEF void naCartesianToPolar(double* rtheta, const double* xy){
  rtheta[0] = naSqrt(xy[0] * xy[0] + xy[1] * xy[1]);
  rtheta[1] = (rtheta[0] == 0.) ? 0. : naAngle(xy);
}



#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_MATH_OPERATORS_INCLUDED


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
