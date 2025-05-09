
// This file contains inline implementations of the file NAMathOperators.h
// Do not include this file directly! It will automatically be included when
// including "NAMathOperators.h"


#include <math.h>
#include "../NAMathConstants.h"
#include "../../NAUtility/NABinaryData.h"


// Note that the smaller- or greater-operator often is faster than the
// smallerequal- or greaterequal-operator.
// (a <= b) will always be written as !(a > b)
// (a >= b) will always be written as !(a < b)


NA_IDEF NABool naAlmostZero(double x) {
  return ((x < NA_SINGULARITY) && (x > -NA_SINGULARITY));
}
NA_IDEF NABool naAlmostZerof(float x) {
  return ((x < NA_SINGULARITYf) && (x > -NA_SINGULARITYf));
}



NA_IDEF NABool naAlmostOne(double x) {
  return ((x < NA_SUP_NORM) && (x > NA_SUB_NORM));
}
NA_IDEF NABool naAlmostOnef(float x) {
  return ((x < NA_SUP_NORMf) && (x > NA_SUB_NORMf));
}



NA_IDEF NABool naAlmost(double x, double y) {
  if(naAlmostZero(y)) {
    if(naAlmostZero(x)) {
      return NA_TRUE;
    }else{
      return naAlmostZero(x-y);
    }
  }else{
    return naAlmostOne(x/y);
  }
}
NA_IDEF NABool naAlmostf(float x, float y) {
  if(naAlmostZerof(y)) {
    if(naAlmostZerof(x)) {
      return NA_TRUE;
    }else{
      return naAlmostZerof(x-y);
    }
  }else{
    return naAlmostOnef(x/y);
  }
}



// Note that checking for NaN must be performed with the built-in isnan()-macro
// or -function. Checking x == NA_NAN would return NA_FALSE always.
// Due to a mis-definition in OS X, Linking between C and C++ does not work
// with the isnan macro. The author is not aware of a portable solution.
NA_IAPI NABool naIsNaN(double x) {
  #if NA_OS == NA_OS_WINDOWS
    return _isnan(x);
  #else
    #ifndef isnan
      return (x != x);
    #else
      return (NABool)isnan(x);
    #endif
  #endif
}
NA_IAPI NABool naIsNaNf(float x) {
  #if NA_OS == NA_OS_WINDOWS
    return _isnan((double)x);
  #else
    #ifndef isnan
      return (x != x);
    #else
      return (NABool)isnan(x);
    #endif
  #endif
}



NA_IAPI NABool naIsInfinite(double x) {
  #if NA_OS == NA_OS_WINDOWS
    return !_finite(x);
  #else
    #ifndef isnan
      return !naIsNaNf(x) && naIsNaNf(x - x);
    #else
      return (NABool)isinf(x);
    #endif
  #endif
}
NA_IAPI NABool naIsInfinitef(float x) {
  #if NA_OS == NA_OS_WINDOWS
    return !_finite((double)x);
  #else
    #ifndef isnan
      return !naIsNaNf(x) && naIsNaNf(x - x);
    #else
      return (NABool)isinf(x);
    #endif
  #endif
}

NA_IDEF double naMin(double a, double b) {
  return (a < b) ? a : b;
}
NA_IDEF float naMinf(float a, float b) {
  return (a < b) ? a : b;
}
NA_IDEF size_t naMins(size_t a, size_t b) {
  return (a < b) ? a : b;
}
NA_IDEF int8 naMini8(int8 a, int8 b) {
  return (a < b) ? a : b;
}
NA_IDEF int16 naMini16(int16 a, int16 b) {
  return (a < b) ? a : b;
}
NA_IDEF int32 naMini32(int32 a, int32 b) {
  return (a < b) ? a : b;
}
NA_IDEF int64 naMini64(int64 a, int64 b) {
  return naSmalleri64(a, b) ? a : b;
}


NA_IDEF double naMax(double a, double b) {
  return (a > b) ? a : b;
}
NA_IDEF float naMaxf(float a, float b) {
  return (a > b) ? a : b;
}
NA_IDEF size_t naMaxs(size_t a, size_t b) {
  return (a > b) ? a : b;
}
NA_IDEF int8 naMaxi8(int8 a, int8 b) {
  return (a > b) ? a : b;
}
NA_IDEF int16 naMaxi16(int16 a, int16 b) {
  return (a > b) ? a : b;
}
NA_IDEF int32 naMaxi32(int32 a, int32 b) {
  return (a > b) ? a : b;
}
NA_IDEF int64 naMaxi64(int64 a, int64 b) {
  return naGreateri64(a, b) ? a : b;
}



NA_IDEF double naInv(double x) {
  #if NA_DEBUG
    if(x == 0.)
      naError("Division by zero.");
    if(naAlmostZero(x))
      naError("Division by almost zero.");
  #endif
  return 1. / x;
}
NA_IDEF float naInvf(float x) {
  #if NA_DEBUG
    if(x == 0.f)
      naError("Division by zero.");
    if(naAlmostZerof(x))
      naError("Division by almost zero.");
  #endif
  return 1.f / x;
}



NA_IDEF double naSqrt(double x) {
  #if NA_DEBUG
    if(x < 0.)
      naError("naSqrt of negative number.");
  #endif
  return sqrt(x);
}
NA_IDEF float naSqrtf(float x) {
  #if NA_DEBUG
    if(x < 0.f)
      naError("naSqrt of negative number.");
  #endif
  return sqrtf(x);
}



NA_IDEF double naCbrt(double x) {
  #if NA_DEBUG
    if(x < 0.)
      naError("naCbrtd of negative number.");
  #endif
  #if NA_OS == NA_OS_WINDOWS
    return pow(x, NA_THIRD);
  #elif NA_IS_POSIX
    return cbrt(x);
  #endif
}
NA_IDEF float naCbrtf(float x) {
  #if NA_DEBUG
    if(x < 0.f)
      naError("naCbrtd of negative number.");
  #endif
  #if NA_OS == NA_OS_WINDOWS
    return powf(x, NA_THIRDf);
  #elif NA_IS_POSIX
    return cbrtf(x);
  #endif
}



NA_IDEF double naAbs(double x) {
  naUnsetSignBit64(&x);
  return x;
}
NA_IDEF float naAbsf(float x) {
  naUnsetSignBit32(&x);
  return x;
}



NA_IDEF double naFloor(double x) {
  return floor(x);
}
NA_IDEF float naFloorf(float x) {
  return floorf(x);
}



NA_IDEF double naCeil(double x) {
  return ceil(x);
}
NA_IDEF float naCeilf(float x) {
  return ceilf(x);
}



NA_IDEF double naRound(double x) {
  #if NA_OS == NA_OS_WINDOWS
    return floor(x + .5);
  #elif NA_IS_POSIX
    return round(x);
  #endif
}
NA_IDEF float naRoundf(float x) {
  #if NA_OS == NA_OS_WINDOWS
    return floorf(x + .5f);
  #elif NA_IS_POSIX
    return roundf(x);
  #endif
}



NA_IAPI double naMod(double x, double mod) {
  double count = naFloor(x / mod);
  return x - count * mod;
}
NA_IAPI float naModf(float x, float mod) {
  float count = naFloorf(x / mod);
  return x - count * mod;
}
//NA_IAPI int32 naModi32(int32 x, int32 mod) {
//  return ((x % mod) + mod) % mod;
//}
//NA_IAPI int64 naModi64(int64 x, int64 mod) {
//  return ((x % mod) + mod) % mod;
//}



NA_IDEF double naSin(double x) {
  return sin(x);
}
NA_IDEF float naSinf(float x) {
  return sinf(x);
}



NA_IDEF double naCos(double x) {
  return cos(x);
}
NA_IDEF float naCosf(float x) {
  return cosf(x);
}



NA_IDEF double naTan(double x) {
  return tan(x);
}
NA_IDEF float naTanf(float x) {
  return tanf(x);
}



NA_IDEF double naAsin(double x) {
  #if NA_DEBUG
    if(!naInsideNormII(x))
      naError("naAsind of invalid value.");
  #endif
  return asin(x);
}
NA_IDEF float naAsinf(float x) {
  #if NA_DEBUG
    if(!naInsideNormIIf(x))
      naError("naAsind of invalid value.");
  #endif
  return asinf(x);
}



NA_IDEF double naAcos(double x) {
  #if NA_DEBUG
    if(!naInsideNormII(x))
      naError("naAcosd of invalid value.");
  #endif
  return acos(x);
}
NA_IDEF float naAcosf(float x) {
  #if NA_DEBUG
    if(!naInsideNormIIf(x))
      naError("naAcosd of invalid value.");
  #endif
  return acosf(x);
}



NA_IDEF double naAtan(double x) {
  return atan(x);
}
NA_IDEF float naAtanf(float x) {
  return atanf(x);
}



NA_IDEF double naAtan2(double y, double x) {
  return atan2(y, x);
}
NA_IDEF float naAtan2f(float y, float x) {
  return atan2f(y, x);
}



NA_IDEF double naAngle(const double* xy) {
  return naAtan2(xy[1], xy[0]);
}
NA_IDEF float naAnglef(const float* xy) {
  return naAtan2f(xy[1], xy[0]);
}



NA_IDEF double naExp(double x) {
  return exp(x);
}
NA_IDEF float naExpf(float x) {
  return expf(x);
}



NA_IDEF double naLog(double x) {
  #if NA_DEBUG
    if(x < 0.)
      naError("Logarithm of negative number.");
  #endif
  return log(x);
}
NA_IDEF float naLogf(float x) {
  #if NA_DEBUG
    if(x < 0.f)
      naError("Logarithm of negative number.");
  #endif
  return logf(x);
}



NA_IDEF double naLog10(double x) {
  #if NA_DEBUG
    if(x < 0)
      naError("Logarithm of negative number.");
  #endif
  return log10(x);
}
NA_IDEF float naLog10f(float x) {
  #if NA_DEBUG
    if(x < 0)
      naError("Logarithm of negative number.");
  #endif
  return log10f(x);
}



NA_IDEF double naLog2(double x) {
  #if NA_DEBUG
    if(x < 0.)
      naError("Logarithm of negative number.");
  #endif
  #if NA_OS == NA_OS_WINDOWS
    return log(x) * NA_INV_LOGOF2;
  #elif NA_IS_POSIX
    return log2(x);
  #endif
}
NA_IDEF float naLog2f(float x) {
  #if NA_DEBUG
    if(x < 0.f)
      naError("Logarithm of negative number.");
  #endif
  #if NA_OS == NA_OS_WINDOWS
    return logf(x) * NA_INV_LOGOF2f;
  #elif NA_IS_POSIX
    return log2f(x);
  #endif
}
NA_IDEF int32 naLog2i32(int32 x) {
  int32 retValue;
  #if NA_DEBUG
    if(x < 0)
      naError("Logarithm of negative number.");
  #endif
  
  if(x <= 0)
    return 0;
  
  x--;
  retValue = 0;
  while(x) {
    retValue++;
    x>>=1;
  }
  return retValue;
}
NA_IDEF int64 naLog2i64(int64 x) {
  int64 retValue;
  #if NA_DEBUG
    if(naSmalleri64(x, NA_ZERO_i64))
      naError("Logarithm of negative number.");
  #endif
  
  if(naSmallerEquali64(x, NA_ZERO_i64)) {
    return NA_ZERO_i64;
  }
  
  naDeci64(x);
  retValue = NA_ZERO_i64;
  while(!naEquali64(x, NA_ZERO_i64)) {
    naInci64(retValue);
    x = naShri64(x, 1);
  }
  return retValue;
}
NA_IDEF size_t naLog2s(size_t x) {
  size_t retValue;
  if(x <= 0)
    return 0;
  
  x--;
  retValue = 0;
  while(x) {
    retValue++;
    x>>=1;
  }
  return retValue;
}



NA_IDEF double naExp2(double x) {
  #if NA_OS == NA_OS_WINDOWS
    return pow(2., x);
  #elif NA_IS_POSIX
    return exp2(x);
  #endif
}
NA_IDEF float naExp2f(float x) {
  #if NA_OS == NA_OS_WINDOWS
    return powf(2.f, x);
  #elif NA_IS_POSIX
    return exp2f(x);
  #endif
}
NA_IDEF int32 naExp2i32(int32 x) {
  #if NA_DEBUG
    if(x < 0)
      naError("Exponent negative. Will result in 0.");
    if(x >= 32)
      naError("Exponent too big. Will result in 0.");
  #endif
  return NA_ONE_i32 << x;
}
NA_IDEF int64 naExp2i64(int64 x) {
  #if NA_DEBUG
    if(naSmalleri64(x, NA_ZERO_i64))
      naError("Exponent negative. Will result in 0.");
    if(naGreaterEquali64(x, naCastu32Toi64(64)))
      naError("Exponent too big. Will result in 0.");
  #endif
  return naShli64(NA_ONE_i64, naCasti64Toi32(x));
}



NA_IDEF double naExp10(double x) {
  return pow(10., x);
}
NA_IDEF float naExp10f(float x) {
  return powf(10.f, x);
}
NA_IDEF int32 naExp10i32(int32 x) {
  int32 i;
  #if NA_DEBUG
    if(x > 9)
      naError("Exponent of 10 exceeds integer range.");
  #endif
  i = 1;
  while(x) {
    i *= 10;
    x--;
  }
  return i;
}
NA_IDEF int64 naExp10i64(int64 x) {
  int64 i;
  #if NA_DEBUG
    if(naGreateri64(x, naCastu32Toi64(18)))
      naError("Exponent of 10 exceeds integer range.");
  #endif
  i = NA_ONE_i64;
  while(!naEquali64(x, NA_ZERO_i64)) {
    i = naMuli64(i, naCastu32Toi64(10));
    naDeci64(x);
  }
  return i;
}



NA_IDEF double naPow(double b, double x) {
  #if NA_DEBUG
    if(b < 0.)
      naError("Base of power function smaller 0.");
  #endif
  return pow(b, x);
}
NA_IDEF float naPowf(float b, float x) {
  #if NA_DEBUG
    if(b < 0.f)
      naError("Base of power function smaller 0.");
  #endif
  return powf(b, x);
}



NA_IDEF double naPowerOf2(int32 n) {
  return naMakeDoubleWithExponent((int32)n);
}
NA_IDEF float naPowerOf2f(int32 n) {
  return naMakeFloatWithExponent((int32)n);
}
NA_IDEF int32 naPowerOf2i32(int32 n) {
  #if NA_DEBUG
    if(n < 0)
      naError("Can only compute integral numbers");
    if(n > 30)
      naError("Overflow");
  #endif
  return NA_ONE_i32 << n;
}
NA_IDEF int64 naPowerOf2i64(int32 n) {
  #if NA_DEBUG
    if(n < 0)
      naError("Can only compute integral numbers");
    if(n > 62)
      naError("Overflow");
  #endif
  return naShli64(NA_ONE_i64, n);
}
NA_IDEF size_t naPowerOf2s(int32 n) {
  #if NA_DEBUG
    if(n < 0)
      naError("Can only compute integral numbers");
    if(n > 62)
      naError("Overflow");
  #endif
  return NA_ONE_s << n;
}



NA_IDEF int32 naFactorize(int32 x) {
  int32 result = 1;
  while(x > 1) {
    result *= x--;
  }
  return result;
}



NA_IDEF int32 naBinom(int32 n, int32 k) {
  int32 nminusk = n - k;
  int32 kfactor;
  int32 x;
  int32 nminuskfactor;
  int32 nfactor;
  if(k > nminusk) {
    naSwap32(&k, &nminusk);
  }
  kfactor = 1;
  x = 2;
  while(x <= k) {
    kfactor *= x++;
  }
  nminuskfactor = kfactor;
  while(x <= nminusk) {
    nminuskfactor *= x++;
  }
  nfactor = nminuskfactor;
  while(x <= n) {
    nfactor *= x++;
  }
  return nfactor / (kfactor * nminuskfactor);
}



NA_IDEF double naDegToRad(double deg) {
  return deg * NA_DEGREES;
}
NA_IDEF float naDegToRadf(float deg) {
  return deg * NA_DEGREESf;
}



NA_IDEF double naRadToDeg(double rad) {
  return rad * NA_DEG_PER_RAD;
}
NA_IDEF float naRadToDegf(float rad) {
  return rad * NA_DEG_PER_RADf;
}



NA_IDEF void naPolarToCartesian(double* xy, const double* rtheta) {
  double r = rtheta[0];
  xy[0] = r * naCos(rtheta[1]);
  xy[1] = r * naSin(rtheta[1]);
}
NA_IDEF void naPolarToCartesianf(float* xy, const float* rtheta) {
  float r = rtheta[0];
  xy[0] = r * naCosf(rtheta[1]);
  xy[1] = r * naSinf(rtheta[1]);
}



// Note that the sqrt computation could be done using naLengthV2 but this would
// require NAVectorAlgebra.h to be included.
NA_IDEF void naCartesianToPolar(double* rtheta, const double* xy) {
  rtheta[0] = naSqrt(xy[0] * xy[0] + xy[1] * xy[1]);
  rtheta[1] = (rtheta[0] == 0.) ? 0. : naAngle(xy);
}
NA_IDEF void naCartesianToPolarf(float* rtheta, const float* xy) {
  rtheta[0] = naSqrtf(xy[0] * xy[0] + xy[1] * xy[1]);
  rtheta[1] = (rtheta[0] == 0.f) ? 0.f : naAnglef(xy);
}



// inclusive, inclusive
NA_IDEF NABool naInsideII(double a, double b, double x) {
  #if NA_DEBUG
    if(a > b)
      naError("a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_IDEF NABool naInsideIIf(float a, float b, float x) {
  #if NA_DEBUG
    if(a > b)
      naError("a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}


// inclusive, exclusive
NA_IDEF NABool naInsideIE(double a, double b, double x) {
  #if NA_DEBUG
    if(!(a < b))
      naError("a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}
NA_IDEF NABool naInsideIEf(float a, float b, float x) {
  #if NA_DEBUG
    if(!(a < b))
      naError("a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}


// exclusive, inclusive
NA_IDEF NABool naInsideEI(double a, double b, double x) {
  #if NA_DEBUG
    if(!(a < b))
      naError("a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}
NA_IDEF NABool naInsideEIf(float a, float b, float x) {
  #if NA_DEBUG
    if(!(a < b))
      naError("a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}


// exclusive, exclusive
NA_IDEF NABool naInsideEE(double a, double b, double x) {
  #if NA_DEBUG
    if(!(a < b))
      naError("a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}
NA_IDEF NABool naInsideEEf(float a, float b, float x) {
  #if NA_DEBUG
    if(!(a < b))
      naError("a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}



NA_IDEF NABool naInsidei32(int32 a, int32 b, int32 x) {
  #if NA_DEBUG
    if(a > b)
      naError("a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_IDEF NABool naInsidei64(int64 a, int64 b, int64 x) {
  #if NA_DEBUG
    if(naGreateri64(a, b))
      naError("a is greater b. Will always return FALSE");
  #endif
  return (!(naSmalleri64(x, a) || naGreateri64(x, b)));
}
NA_IDEF NABool naInsides(size_t a, size_t b, size_t x) {
  #if NA_DEBUG
    if(a > b)
      naError("a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}



// Note that for the naAlmostInsided functions, naInsideEEd is used after
// adjusting the range. This cound be anything else but as this is a function
// which only needs to be almost accurate, the one function is chosen which is
// simplest.
NA_IDEF NABool naAlmostInside(double a, double b, double x) {
  #if NA_DEBUG
    if(a>b)
      naError("a is greater b.");
  #endif
  a *= (a < 0.) ? NA_SUP_NORM : NA_SUB_NORM;
  b *= (b < 0.) ? NA_SUB_NORM : NA_SUP_NORM;
  return naInsideEE(a, b, x);
}
NA_IDEF NABool naAlmostInsidef(float a, float b, float x) {
  #if NA_DEBUG
    if(a>b)
      naError("a is greater b.");
  #endif
  a *= (a < 0.f) ? NA_SUP_NORMf : NA_SUB_NORMf;
  b *= (b < 0.f) ? NA_SUB_NORMf : NA_SUP_NORMf;
  return naInsideEEf(a, b, x);
}



// naInsideNormZId: [ 0, 1]
NA_IDEF NABool naInsideNormZI(double x) {
  return !((x < 0.) || (x > 1.));
}
NA_IDEF NABool naInsideNormZIf(float x) {
  return !((x < 0.f) || (x > 1.f));
}



// naInsideNormZEd: [ 0, 1)
NA_IDEF NABool naInsideNormZE(double x) {
  return (!(x < 0.) && (x < 1.));
}
NA_IDEF NABool naInsideNormZEf(float x) {
  return (!(x < 0.f) && (x < 1.f));
}



// naInsideNormIId: [-1, 1]
NA_IDEF NABool naInsideNormII(double x) {
  return !((x < -1.) || (x > 1.));
}
NA_IDEF NABool naInsideNormIIf(float x) {
  return !((x < -1.f) || (x > 1.f));
}



// naInsideNormIEd: [-1, 1)
NA_IDEF NABool naInsideNormIE(double x) {
  return (!(x < -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormIEf(float x) {
  return (!(x < -1.f) && (x < 1.f));
}



// naInsideNormEEd: (-1, 1)
NA_IDEF NABool naInsideNormEE(double x) {
  return ((x > -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormEEf(float x) {
  return ((x > -1.f) && (x < 1.f));
}



// This is free and unencumbered software released into the public domain.

// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non-commercial, and by any
// means.

// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain. We make this dedication for the benefit
// of the public at large and to the detriment of our heirs and
// successors. We intend this dedication to be an overt act of
// relinquishment in perpetuity of all present and future rights to this
// software under copyright law.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.

// For more information, please refer to <http://unlicense.org/>
