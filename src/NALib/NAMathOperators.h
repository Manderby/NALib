
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_MATH_OPERATORS_INCLUDED
#define NA_MATH_OPERATORS_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"


// /////////////////////////////////
// This file contains many basic functions such as min and max or comparing
// functions as well as standard math operators. All functions are defined
// inline. When debugging, many checks are performed like division by zero
// which may slow the implementation down. You can disable these checks by
// defining NDEBUG.
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
//
// Note that many of these functions are not especially difficult to understand
// or implement but take a lot of code writing. You are free to explore all the
// implementation but there isn't much to see. Seriously.
// /////////////////////////////////





// ///////////////////////////////////
// Value attributes
// ///////////////////////////////////

// Returns the sign. x<0 returns -1, x>=0 returns 1. Never returns 0!
// You can use the result of this function safely as a multiplication factor.
// You can also subtract the result of this function from the comma ASCII
// character and get a string representation of the sign:
// NAUTF8Char signASCII = ',' - naSigni(-42);
NA_IAPI NAInt         naSigni  (NAInt x);
NA_IAPI int8          naSigni8 (int8  x);
NA_IAPI int16         naSigni16(int16 x);
NA_IAPI int32         naSigni32(int32 x);
NA_IAPI int64         naSigni64(int64 x);

// naAlmostZero   Returns NA_TRUE, if the ABSOLUTE discance between x and 0 is
//                smaller than NA_SINGULARITY.
// naAlmostOne    Returns NA_TRUE, if the ABSOLUTE discance between x and 1 is
//                smaller than NA_SINGULARITY.
// naAlmost       Returns NA_TRUE, if the RELATIVE distance between x and y is
//                smaller than NA_SINGULARITY. If y is close to 0, the absolute
//                distance is checked.
// Warning: These functions are slow. Especially naAlmost and naAlmostf.
NA_IAPI NABool        naAlmostZero  (double x);
NA_IAPI NABool        naAlmostZerof (float  x);
NA_IAPI NABool        naAlmostOne   (double x);
NA_IAPI NABool        naAlmostOnef  (float  x);
NA_IAPI NABool        naAlmost      (double x, double y);
NA_IAPI NABool        naAlmostf     (float  x, float  y);

// Returns true if the given number is NaN, infinite.
NA_IAPI NABool        naIsNaN (double x);
NA_IAPI NABool        naIsNaNf(float  x);
NA_IAPI NABool        naIsInfinite(double x);
NA_IAPI NABool        naIsInfinitef(float x);

// naMin and naMax return the minimum or maximum of two values.
//
// Note that in many implementations, Min and Max are implemented as the
// following macros:
// #define NA_MAX(a, b) (((a)>(b))?(a):(b))
// #define NA_MIN(a, b) (((a)<(b))?(a):(b))
// Nontheless, these are not used here in NALib as they can not be debugged
// as good as the functions: Quite often, the two parameters are only
// available as an expression and hence are not shown in the compiler. When
// using the functions, the two resulting values can be debugged when jumping
// inside the min or max function.
NA_IAPI double       naMin   (double a, double b);
NA_IAPI float        naMinf  (float  a, float  b);
NA_IAPI NAInt        naMini  (NAInt  a, NAInt  b);
NA_IAPI int8         naMini8 (int8   a, int8   b);
NA_IAPI int16        naMini16(int16  a, int16  b);
NA_IAPI int32        naMini32(int32  a, int32  b);
NA_IAPI int64        naMini64(int64  a, int64  b);

NA_IAPI double       naMax   (double a, double b);
NA_IAPI float        naMaxf  (float  a, float  b);
NA_IAPI NAInt        naMaxi  (NAInt  a, NAInt  b);
NA_IAPI int8         naMaxi8 (int8   a, int8   b);
NA_IAPI int16        naMaxi16(int16  a, int16  b);
NA_IAPI int32        naMaxi32(int32  a, int32  b);
NA_IAPI int64        naMaxi64(int64  a, int64  b);




// ///////////////////////////////////
// Mathematical operators
// ///////////////////////////////////

// Inverse: Returns 1/x
NA_IAPI float         naInvf(float x);
NA_IAPI double        naInv (double x);

// Returns the square root
NA_IAPI float         naSqrtf(float x);
NA_IAPI double        naSqrt(double x);

// Returns the cubic root
NA_IAPI float         naCbrtf(float x);
NA_IAPI double        naCbrt(double x);

// Returns the absolute value
NA_IAPI float         naAbsf  (float  x);
NA_IAPI double        naAbs   (double x);
NA_IAPI NAInt         naAbsi  (NAInt  x);
NA_IAPI int8          naAbsi8 (int8   x);
NA_IAPI int16         naAbsi16(int16  x);
NA_IAPI int32         naAbsi32(int32  x);
NA_IAPI int64         naAbsi64(int64  x);

// Returns the rounded down value
NA_IAPI float         naFloorf(float x);
NA_IAPI double        naFloor(double x);

// Returns the rounded up value
NA_IAPI float         naCeilf(float x);
NA_IAPI double        naCeil(double x);

// Returns the rounded value
NA_IAPI float         naRoundf(float x);
NA_IAPI double        naRound(double x);

// Returns the modular value. Return value is always positive.
NA_IAPI double        naMod(double x, double mod);

// Returns the sinus
NA_IAPI float         naSinf(float x);
NA_IAPI double        naSin(double x);

// Returns the cosinus
NA_IAPI float         naCosf(float x);
NA_IAPI double        naCos(double x);

// Returns the tangens
NA_IAPI float         naTanf(float x);
NA_IAPI double        naTan(double x);

// Returns the arcus sinus
NA_IAPI float         naAsinf(float x);
NA_IAPI double        naAsin(double x);

// Returns the arcus cosinus
NA_IAPI float         naAcosf(float x);
NA_IAPI double        naAcos(double x);

// Returns the arcus tangens in the range [-pi/2, +pi/2]
NA_IAPI float         naAtanf(float x);
NA_IAPI double        naAtan(double x);

// Returns the arcus tangens of y/x in the range [-pi, +pi]
// The angle function does the same, but assumes x and y stored as
// xy[0] and xy[1] respectively.
NA_IAPI float         naAtan2f(float y, float x);
NA_IAPI double        naAtan2(double y, double x);
NA_IAPI float         naAnglef(const float* xy);
NA_IAPI double        naAngle(const double* xy);

// Returns the natural logarithm
NA_IAPI float         naLogf(float x);
NA_IAPI double        naLog(double x);

// Returns the 10-logarithm
NA_IAPI float         naLog10f(float x);
NA_IAPI double        naLog10(double x);

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
NA_IAPI float         naLog2f(float x);
NA_IAPI double        naLog2(double x);
NA_IDEF NAInt         naLog2i(NAInt x);
NA_IAPI int32         naLog2i32(int32 x);
NA_IAPI int64         naLog2i64(int64 x);

// Returns the exponential function e^x
NA_IAPI float         naExpf(float x);
NA_IAPI double        naExp(double x);

// Returns the exponent of 2 function 2^x
NA_IAPI float         naExp2f  (float x);
NA_IAPI double        naExp2   (double x);
NA_IAPI NAInt         naExp2i  (NAInt x);
NA_IAPI int32         naExp2i32(int32 x);
NA_IAPI int64         naExp2i64(int64 x);

// Returns the exponent of 10 function 10^x
NA_IAPI float         naExp10f  (float x);
NA_IAPI double        naExp10   (double x);
NA_IAPI NAInt         naExp10i  (NAInt x);
NA_IAPI int32         naExp10i32(int32 x);
NA_IAPI int64         naExp10i64(int64 x);

// Returns the power function b^x
NA_IAPI float         naPowf(float b, float x);
NA_IAPI double        naPow(double b, double x);

// Factorizes the given value. !-Operator
NA_IAPI NAInt         naFactorize(NAInt x);
// Returns the binomial coefficient n over k
NA_IAPI NAInt         naBinom(NAInt n, NAInt k);

// Returns the the angle converted to radiants
NA_IAPI float         naDegToRadf(float deg);
NA_IAPI double        naDegToRad(double deg);

// Returns the the angle converted to degrees
NA_IAPI float         naRadToDegf(float rad);
NA_IAPI double        naRadToDeg(double rad);

// Angle functions. Assumes either x and y or r and theta to be stored in a
// two dimensional vector and converts it into the other.
NA_IAPI void          naPolarToCartesianf(float*  xy, const float*  rtheta);
NA_IAPI void          naPolarToCartesian (double* xy, const double* rtheta);
NA_IAPI void          naCartesianToPolarf(float*  rtheta, const float*  xy);
NA_IAPI void          naCartesianToPolar (double* rtheta, const double* xy);





// ///////////////////////////////////
// Value ranges
// ///////////////////////////////////

// naInside returns true, if x is inside the span depicted by a and b. The
// value a should be smaller than b. The letters I and E depict whether a or b
// are compared inclusive or exclusive. For example:
// naInsideIE(3, 7, x) corresponds to a range of [3,7).

// inclusive, inclusive
NA_IAPI NABool naInsideII (double a, double b, double x);
NA_IAPI NABool naInsidefII(float  a, float  b, float  x);
// inclusive, exclusive
NA_IAPI NABool naInsideIE (double a, double b, double x); // use these if you
NA_IAPI NABool naInsidefIE(float  a, float  b, float  x); // are uncertain.
// exclusive, inclusive
NA_IAPI NABool naInsideEI (double a, double b, double x);
NA_IAPI NABool naInsidefEI(float  a, float  b, float  x);
// exclusive, exclusive
NA_IAPI NABool naInsideEE (double a, double b, double x);
NA_IAPI NABool naInsidefEE(float  a, float  b, float  x);

// Integer variants. a denotes Min, b denotes Max. Comparison is always
// inclusive - inclusive.
NA_IAPI NABool naInsidei  (NAInt  a, NAInt  b, NAInt  x);
NA_IAPI NABool naInsidei32(int32  a, int32  b, int32  x);
NA_IAPI NABool naInsidei64(int64  a, int64  b, int64  x);

// naAlmostInside checks if the given value is almost or completely inside
// the given values. Meaning, a small relative margin of NA_SINGULARITY on
// both sides is considered. Takes into consideration that a or b might be
// negative.
NA_IAPI NABool naAlmostInside (double a, double b, double x);
NA_IAPI NABool naAlmostInsidef(float  a, float  b, float  x);

// naInsideNorm returns true, if x is inside a normed range. The following norms
// are impmemented:
// naInsideNormZI: [ 0, 1 ]
// naInsideNormZE: [ 0, 1 )
// naInsideNormII: [-1, 1 ]
// naInsideNormIE: [-1, 1 )
// naInsideNormEE: (-1, 1 )
// I means inclusive, E means exclusive, Z means Zero.
NA_IAPI NABool naInsideNormZI (double x);
NA_IAPI NABool naInsideNormZIf(float x);
NA_IAPI NABool naInsideNormZE (double x);
NA_IAPI NABool naInsideNormZEf(float x);
NA_IAPI NABool naInsideNormII (double x);
NA_IAPI NABool naInsideNormIIf(float x);
NA_IAPI NABool naInsideNormIE (double x);
NA_IAPI NABool naInsideNormIEf(float x);
NA_IAPI NABool naInsideNormEE (double x);
NA_IAPI NABool naInsideNormEEf(float x);











// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


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
  #if defined NA_SIGNED_INTEGER_USES_TWOS_COMPLEMENT
    return ((x>>7)<<1)+1;
  #else
    return (x<0)?-1:1;
  #endif
}
NA_IDEF int16 naSigni16(int16 x){
  #if defined NA_SIGNED_INTEGER_USES_TWOS_COMPLEMENT
    return ((x>>15)<<1)+1;
  #else
    return (x<0)?-1:1;
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


NA_IDEF NABool naAlmostZero  (double x){
  return ((x < NA_SINGULARITY) && (x > -NA_SINGULARITY));
}
NA_IDEF NABool naAlmostZerof (float x){
  return ((x < NA_SINGULARITYf) && (x > -NA_SINGULARITYf));
}


NA_IDEF NABool naAlmostOne  (double x){
  return ((x < NA_SUP_NORM) && (x > NA_SUB_NORM));
}
NA_IDEF NABool naAlmostOnef (float x){
  return ((x < NA_SUP_NORMf) && (x > NA_SUB_NORMf));
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


// Note that checking for NaN must be performed with the built-in isnan()-macro
// or -function. Checking x == NA_NAN would return NA_FALSE always.
// Due to a mis-definition in OS X, Linking between C and C++ does not work
// with the isnan macro. The author is not aware of a portable solution.
NA_IAPI NABool naIsNaN(double x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _isnan(x);
  #else
    #ifndef isnan
      return (x != x);
    #else
      return isnan(x);
    #endif
  #endif
}

NA_IAPI NABool naIsNaNf(float x){
  #if NA_SYSTEM == NA_SYSTEM_WINDOWS
    return _isnan((double)x);
  #else
    #ifndef isnan
      return (x != x);
    #else
      return isnan(x);
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
      return isinf(x);
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
      return isinf(x);
    #endif
  #endif
}


NA_IDEF double naMin (double a, double b){
  return (a<b)?a:b;
}
NA_IDEF float naMinf(float a, float b){
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


NA_IDEF double naMax (double a, double b){
  return (a>b)?a:b;
}
NA_IDEF float naMaxf(float a, float b){
  return (a>b)?a:b;
}
NA_IDEF NAInt naMaxi  (NAInt a, NAInt b){
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
    if(!naInsideNormIIf(x)){naError("naAsinf", "naAsin of invalid value.");}
  #endif
  return asinf(x);
}
NA_IDEF double naAsin(double x){
  #ifndef NDEBUG
    if(!naInsideNormII(x)){naError("naAsin", "naAsin of invalid value.");}
  #endif
  return asin(x);
}


NA_IDEF float naAcosf(float x){
  #ifndef NDEBUG
    if(!naInsideNormIIf(x)){naError("naAcosf", "naAcos of invalid value.");}
  #endif
  return acosf(x);
}
NA_IDEF double naAcos(double x){
  #ifndef NDEBUG
    if(!naInsideNormII(x)){naError("naAcos", "naAcos of invalid value.");}
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
  int32 i;   // Declaration before implementation. Needed for C90.
  #ifndef NDEBUG
    if(x > 9)
      naError("naExp10i32", "Exponent of 10 exceeds integer range.");
  #endif
  i = 1;
  while(x){i *= 10; x--;}
  return i;
}
NA_IDEF int64 naExp10i64(int64 x){
  int64 i;   // Declaration before implementation. Needed for C90.
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






NA_IDEF NAInt naFactorize(NAInt x){
  NAInt result = 1;
  while(x>1){result *= x--;}
  return result;
}

NA_IDEF NAInt naBinom(NAInt n, NAInt k){
  // Declaration before implementation. Needed for C90.
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
      naError("naInsideII", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}
NA_IDEF NABool naInsidefII(float a, float b, float x){
  #ifndef NDEBUG
    if(a > b)
      naError("naInsidefII", "a is greater b. Will always return FALSE");
  #endif
  return (!((x < a) || (x > b)));
}


// inclusive, exclusive
NA_IDEF NABool naInsideIE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideIE", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}
NA_IDEF NABool naInsidefIE(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsidefIE", "a is greaterequal b. Will always return FALSE");
  #endif
  return (!(x < a) && (x < b));
}


// exclusive, inclusive
NA_IDEF NABool naInsideEI(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideEI", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}
NA_IDEF NABool naInsidefEI(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsidefEI", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && !(x > b));
}


// exclusive, exclusive
NA_IDEF NABool naInsideEE(double a, double b, double x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsideEE", "a is greaterequal b. Will always return FALSE");
  #endif
  return ((x > a) && (x < b));
}
NA_IDEF NABool naInsidefEE(float a, float b, float x){
  #ifndef NDEBUG
    if(!(a < b))
      naError("naInsidefEE", "a is greaterequal b. Will always return FALSE");
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


// Note that for the naAlmostInside functions, naInsideEE is used after
// adjusting the range. This cound be anything else but as this is a function
// which only needs to be almost accurate, the one function is chosen which is
// simplest.
NA_IDEF NABool naAlmostInside(double a, double b, double x){
  #ifndef NDEBUG
    if(a>b)
      naError("naAlmostInside", "a is greater b.");
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
  return naInsidefEE(a, b, x);
}


// naInsideNormZI: [ 0, 1]
NA_IDEF NABool naInsideNormZI (double x){
  return !((x < 0.) || (x > 1.));
}
NA_IDEF NABool naInsideNormZIf(float x){
  return !((x < 0.f) || (x > 1.f));
}


// naInsideNormZE: [ 0, 1)
NA_IDEF NABool naInsideNormZE (double x){
  return (!(x < 0.) && (x < 1.));
}
NA_IDEF NABool naInsideNormZEf(float x){
  return (!(x < 0.f) && (x < 1.f));
}


// naInsideNormII: [-1, 1]
NA_IDEF NABool naInsideNormII (double x){
  return !((x < -1.) || (x > 1.));
}
NA_IDEF NABool naInsideNormIIf(float x){
  return !((x < -1.f) || (x > 1.f));
}


// naInsideNormIE: [-1, 1)
NA_IDEF NABool naInsideNormIE (double x){
  return (!(x < -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormIEf(float x){
  return (!(x < -1.f) && (x < 1.f));
}


// naInsideNormEE: (-1, 1)
NA_IDEF NABool naInsideNormEE (double x){
  return ((x > -1.) && (x < 1.));
}
NA_IDEF NABool naInsideNormEEf(float x){
  return ((x > -1.f) && (x < 1.f));
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
