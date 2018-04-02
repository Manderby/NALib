
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAMathOperators.h
// Do not include this file directly! It will automatically be included when
// including "NAMathOperators.h"


#include <math.h>
#include "NAMathConstants.h"
#include "NABinaryData.h"


// Note that the smaller- or greater-operator often is faster than the
// smallerequal- or greaterequal-operator.
// (a<=b) will always be written as !(a>b)
// (a>=b) will always be written as !(a<b)


NA_IDEF NAInt naSigni(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naSigni32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naSigni64(x);
#endif
}
NA_IDEF int8 naSigni8(int8 x){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    return ((int8)(naGetSignum8(x) << 1) + 1);
  #else
    return (x<0)?-1:1;
  #endif
}
NA_IDEF int16 naSigni16(int16 x){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    return ((int16)(naGetSignum16(x) << 1) + 1);
  #else
    return (x<0)?-1:1;
  #endif
}
NA_IDEF int32 naSigni32(int32 x){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    return ((int32)(naGetSignum32(x) << 1) + 1);
  #else
    return (x<0)?-1:1;
  #endif
}
NA_IDEF int64 naSigni64(int64 x){
  #if NA_SIGNED_INTEGER_ENCODING == NA_SIGNED_INTEGER_ENCODING_TWOS_COMPLEMENT
    return ((int64)(naGetSignum64(x) << 1) + 1LL);
  #else
    return (x<0)?-1LL:1LL;
  #endif
}



NA_IDEF NABool naAlmostZerof(float x){
  return ((x < NA_SINGULARITYf) && (x > -NA_SINGULARITYf));
}
NA_IDEF NABool naAlmostZero(double x){
  return ((x < NA_SINGULARITY) && (x > -NA_SINGULARITY));
}



NA_IDEF NABool naAlmostOnef(float x){
  return ((x < NA_SUP_NORMf) && (x > NA_SUB_NORMf));
}
NA_IDEF NABool naAlmostOne(double x){
  return ((x < NA_SUP_NORM) && (x > NA_SUB_NORM));
}



NA_IDEF NABool naAlmostf(float x, float y){
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
NA_IDEF NABool naAlmost(double x, double y){
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



// Note that checking for NaN must be performed with the built-in isnan()-macro
// or -function. Checking x == NA_NAN would return NA_FALSE always.
// Due to a mis-definition in OS X, Linking between C and C++ does not work
// with the isnan macro. The author is not aware of a portable solution.
NA_IAPI NABool naIsNaNf(float x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _isnan((double)x);
  #else
    #ifndef isnan
      return (x != x);
    #else
      return (NABool)isnan(x);
    #endif
  #endif
}
NA_IAPI NABool naIsNaN(double x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _isnan(x);
  #else
    #ifndef isnan
      return (x != x);
    #else
      return (NABool)isnan(x);
    #endif
  #endif
}



NA_IAPI NABool naIsInfinitef(float x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _finite((double)x);
  #else
    #ifndef isnan
      return !naIsNaNf(x) && naIsNaNf(x - x);
    #else
      return (NABool)isinf(x);
    #endif
  #endif
}
NA_IAPI NABool naIsInfinite(double x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return !_finite(x);
  #else
    #ifndef isnan
      return !naIsNaNf(x) && naIsNaNf(x - x);
    #else
      return (NABool)isinf(x);
    #endif
  #endif
}



NA_IDEF float naMinf(float a, float b){
  return (a<b)?a:b;
}
NA_IDEF double naMin(double a, double b){
  return (a<b)?a:b;
}
NA_IDEF NAInt naMini(NAInt a, NAInt b){
  return (a<b)?a:b;
}
NA_IDEF int8 naMini8(int8 a, int8 b){
  return (a<b)?a:b;
}
NA_IDEF int16 naMini16(int16 a, int16 b){
  return (a<b)?a:b;
}
NA_IDEF int32 naMini32(int32 a, int32 b){
  return (a<b)?a:b;
}
NA_IDEF int64 naMini64(int64 a, int64 b){
  return (a<b)?a:b;
}


NA_IDEF float naMaxf(float a, float b){
  return (a>b)?a:b;
}
NA_IDEF double naMax(double a, double b){
  return (a>b)?a:b;
}
NA_IDEF NAInt naMaxi(NAInt a, NAInt b){
  return (a>b)?a:b;
}
NA_IDEF int8 naMaxi8(int8 a, int8 b){
  return (a>b)?a:b;
}
NA_IDEF int16 naMaxi16(int16 a, int16 b){
  return (a>b)?a:b;
}
NA_IDEF int32 naMaxi32(int32 a, int32 b){
  return (a>b)?a:b;
}
NA_IDEF int64 naMaxi64(int64 a, int64 b){
  return (a>b)?a:b;
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
    if(x < 0.f){naError("naCbrtf", "naCbrtd of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return powf(x, NA_THIRDf);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return cbrtf(x);
  #endif
}

NA_IDEF double naCbrt(double x){
  #ifndef NDEBUG
    if(x < 0.){naError("naCbrtd", "naCbrtd of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return pow(x, NA_THIRD);
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return cbrt(x);
  #endif
}


NA_IDEF float naAbsf(float x){
  return naUnsetSignBit32((value32)x);
}
NA_IDEF double naAbs(double x){
  return naUnsetSignBit64((value64)x);
}
NA_IDEF NAInt naAbsi(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naAbsi32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naAbsi64(x);
#endif
}
NA_IDEF int8 naAbsi8(int8 x){
  return naSigni8(x) * x;
//  return (int8)abs(x);  // The stdlib implementation is not inlineable.
}
NA_IDEF int16 naAbsi16(int16 x){
  return naSigni16(x) * x;
//  return (int16)abs(x);  // The stdlib implementation is not inlineable.
}
NA_IDEF int32 naAbsi32(int32 x){
  return naSigni32(x) * x;
//  return (int32)abs(x);  // The stdlib implementation is not inlineable.
}
NA_IDEF int64 naAbsi64(int64 x){
  return naSigni64(x) * x;
//  return (int64)llabs(x);  // The stdlib implementation is not inlineable.
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



NA_IAPI float naModf(float x, float mod){
  float count = naFloorf(x / mod);
  if(x<0){count += 1.f;}
  return x - count * mod;
}
NA_IAPI double naMod(double x, double mod){
  double count = naFloor(x / mod);
  if(x<0){count += 1.;}
  return x - count * mod;
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
    if(!naInsideNormIIf(x)){naError("naAsinf", "naAsind of invalid value.");}
  #endif
  return asinf(x);
}
NA_IDEF double naAsin(double x){
  #ifndef NDEBUG
    if(!naInsideNormII(x)){naError("naAsind", "naAsind of invalid value.");}
  #endif
  return asin(x);
}


NA_IDEF float naAcosf(float x){
  #ifndef NDEBUG
    if(!naInsideNormIIf(x)){naError("naAcosf", "naAcosd of invalid value.");}
  #endif
  return acosf(x);
}
NA_IDEF double naAcos(double x){
  #ifndef NDEBUG
    if(!naInsideNormII(x)){naError("naAcosd", "naAcosd of invalid value.");}
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
    if(x < 0.){naError("naLogd", "Logarithm of negative number.");}
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
      naError("naLog10d", "Logarithm of negative number.");
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
    if(x < 0.){naError("naLog2d", "Logarithm of negative number.");}
  #endif
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return log(x) * NA_INV_LOGOF2;
  #elif NA_SYSTEM == NA_SYSTEM_MAC_OS_X
    return log2(x);
  #endif
}

NA_IDEF int32 naLog2i32(int32 x){
  int32 retvalue;
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
  int64 retvalue;
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
NA_IDEF NAInt naLog2i(NAInt x){
  #if NA_SYSTEM_ADDRESS_BITS == 32
    return naLog2i32(x);
  #elif NA_SYSTEM_ADDRESS_BITS == 64
    return naLog2i64(x);
  #endif
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


NA_IDEF float naExp10f(float x){
  return powf(10.f, x);
}
NA_IDEF double naExp10(double x){
  return pow(10., x);
}
NA_IDEF NAInt naExp10i(NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naExp10i32(x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naExp10i64(x);
#endif
}
NA_IDEF int32 naExp10i32(int32 x){
  int32 i;
  #ifndef NDEBUG
    if(x > 9)
      naError("naExp10i32", "Exponent of 10 exceeds integer range.");
  #endif
  i = 1;
  while(x){i *= 10; x--;}
  return i;
}
NA_IDEF int64 naExp10i64(int64 x){
  int64 i;
  #ifndef NDEBUG
    if(x > 18)
      naError("naExp10i64", "Exponent of 10 exceeds integer range.");
  #endif
  i = 1;
  while(x){i *= 10; x--;}
  return i;
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



NA_IDEF float naPowerOf2f(NAInt n){
  return naCreateFloatWithExponent((value32)n);
}
NA_IDEF double naPowerOf2(NAInt n){
  return naCreateDoubleWithExponent((value64)n);
}



NA_IDEF NAInt naFactorize(NAInt x){
  NAInt result = 1;
  while(x>1){result *= x--;}
  return result;
}

NA_IDEF NAInt naBinom(NAInt n, NAInt k){
  NAInt nminusk = n - k;
  NAInt kfactor;
  NAInt x;
  NAInt nminuskfactor;
  NAInt nfactor;
  if(k > nminusk){naSwapi(&k, &nminusk);}
  kfactor = 1;
  x = 2;
  while(x <= k){kfactor *= x++;}
  nminuskfactor = kfactor;
  while(x <= nminusk){nminuskfactor *= x++;}
  nfactor = nminuskfactor;
  while(x <= n){nfactor *= x++;}
  return nfactor / (kfactor * nminuskfactor);
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



// inclusive, inclusive
NA_IDEF NABool naInsideII(double a, double b, double x){
  #ifndef NDEBUG
    if(a > b)
      naError("naInsideIId", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_IDEF NABool naInsideIIf(float a, float b, float x){
  #ifndef NDEBUG
    if(a > b)
      naError("naInsideIIf", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}


// inclusive, exclusive
NA_IDEF NABool naInsideIE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideIEd", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}
NA_IDEF NABool naInsideIEf(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideIEf", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}


// exclusive, inclusive
NA_IDEF NABool naInsideEI(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideEId", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}
NA_IDEF NABool naInsideEIf(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideEIf", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}


// exclusive, exclusive
NA_IDEF NABool naInsideEE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideEEd", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}
NA_IDEF NABool naInsideEEf(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideEEf", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}



NA_IDEF NABool naInsidei(NAInt a, NAInt b, NAInt x){
#if NA_SYSTEM_ADDRESS_BITS == 32
  return naInsidei32(a, b, x);
#elif NA_SYSTEM_ADDRESS_BITS == 64
  return naInsidei64(a, b, x);
#endif
}
NA_IDEF NABool naInsidei32(int32 a, int32 b, int32 x){
  #ifndef NDEBUG
    if(a > b)
      naError("naInsidei32", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_IDEF NABool naInsidei64(int64 a, int64 b, int64 x){
  #ifndef NDEBUG
    if(a > b)
      naError("naInsidei64", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}


// Note that for the naAlmostInsided functions, naInsideEEd is used after
// adjusting the range. This cound be anything else but as this is a function
// which only needs to be almost accurate, the one function is chosen which is
// simplest.
NA_IDEF NABool naAlmostInside(double a, double b, double x){
  #ifndef NDEBUG
    if(a>b)
      naError("naAlmostInsided", "a is greater b.");
  #endif
  a *= (a < 0.) ? NA_SUP_NORM : NA_SUB_NORM;
  b *= (b < 0.) ? NA_SUB_NORM : NA_SUP_NORM;
  return naInsideEE(a, b, x);
}
NA_IDEF NABool naAlmostInsidef(float a, float b, float x){
  #ifndef NDEBUG
    if(a>b)
      naError("naAlmostInsidef", "a is greater b.");
  #endif
  a *= (a < 0.f) ? NA_SUP_NORMf : NA_SUB_NORMf;
  b *= (b < 0.f) ? NA_SUB_NORMf : NA_SUP_NORMf;
  return naInsideEEf(a, b, x);
}


// naInsideNormZId: [ 0, 1]
NA_IDEF NABool naInsideNormZI(double x){
  return !((x < 0.) || (x > 1.));
}
NA_IDEF NABool naInsideNormZIf(float x){
  return !((x < 0.f) || (x > 1.f));
}


// naInsideNormZEd: [ 0, 1)
NA_IDEF NABool naInsideNormZE(double x){
  return (!(x < 0.) && (x < 1.));
}
NA_IDEF NABool naInsideNormZEf(float x){
  return (!(x < 0.f) && (x < 1.f));
}


// naInsideNormIId: [-1, 1]
NA_IDEF NABool naInsideNormII(double x){
  return !((x < -1.) || (x > 1.));
}
NA_IDEF NABool naInsideNormIIf(float x){
  return !((x < -1.f) || (x > 1.f));
}


// naInsideNormIEd: [-1, 1)
NA_IDEF NABool naInsideNormIE(double x){
  return (!(x < -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormIEf(float x){
  return (!(x < -1.f) && (x < 1.f));
}


// naInsideNormEEd: (-1, 1)
NA_IDEF NABool naInsideNormEE(double x){
  return ((x > -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormEEf(float x){
  return ((x > -1.f) && (x < 1.f));
}





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
