
#ifndef NA_MATH_OPERATORS_INCLUDED
#define NA_MATH_OPERATORS_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "../NABase/NABase.h"
#include <stdlib.h>

// /////////////////////////////////
// This file contains many basic functions such as min and max or comparing
// functions as well as standard math operators. All functions are defined
// inline. When debugging, many checks are performed like division by zero
// which may slow the implementation down. You can disable these checks by
// setting NA_DEBUG to 0.
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
NA_IAPI NABool        naIsInfinite (double x);
NA_IAPI NABool        naIsInfinitef(float  x);

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
NA_IAPI double       naMin   (double  a, double  b);
NA_IAPI float        naMinf  (float   a, float   b);
NA_IAPI size_t       naMins  (size_t  a, size_t  b);
NA_IAPI int8         naMini8 (int8    a, int8    b);
NA_IAPI int16        naMini16(int16   a, int16   b);
NA_IAPI int32        naMini32(int32   a, int32   b);
NA_IAPI NAi64        naMini64(NAi64 a, NAi64 b);

NA_IAPI double       naMax   (double  a, double  b);
NA_IAPI float        naMaxf  (float   a, float   b);
NA_IAPI size_t       naMaxs  (size_t  a, size_t  b);
NA_IAPI int8         naMaxi8 (int8    a, int8    b);
NA_IAPI int16        naMaxi16(int16   a, int16   b);
NA_IAPI int32        naMaxi32(int32   a, int32   b);
NA_IAPI NAi64        naMaxi64(NAi64 a, NAi64 b);




// ///////////////////////////////////
// Mathematical operators
// ///////////////////////////////////

// Inverse: Returns 1/x
NA_IAPI double        naInv (double x);
NA_IAPI float         naInvf(float  x);

// Returns the square root
NA_IAPI double        naSqrt (double x);
NA_IAPI float         naSqrtf(float  x);

// Returns the cubic root
NA_IAPI double        naCbrt (double x);
NA_IAPI float         naCbrtf(float  x);

// Returns the absolute value
NA_IAPI double        naAbs   (double x);
NA_IAPI float         naAbsf  (float  x);
// The absolute value functions for integers are naAbsi32 and naAbsi64
// They are defined in NANumerics.h

// Returns the rounded down value
NA_IAPI double        naFloor (double x);
NA_IAPI float         naFloorf(float  x);

// Returns the rounded up value
NA_IAPI double        naCeil (double x);
NA_IAPI float         naCeilf(float  x);

// Returns the rounded value
NA_IAPI double        naRound (double x);
NA_IAPI float         naRoundf(float  x);

// Returns the modular value. Return value is always positive.
NA_IAPI double        naMod   (double x, double mod);
NA_IAPI float         naModf  (float  x, float  mod);

// Returns the sinus
NA_IAPI double        naSin (double x);
NA_IAPI float         naSinf(float  x);

// Returns the cosinus
NA_IAPI double        naCos (double x);
NA_IAPI float         naCosf(float  x);

// Returns the tangens
NA_IAPI double        naTan (double x);
NA_IAPI float         naTanf(float  x);

// Returns the arcus sinus
NA_IAPI double        naAsin (double x);
NA_IAPI float         naAsinf(float  x);

// Returns the arcus cosinus
NA_IAPI double        naAcos (double x);
NA_IAPI float         naAcosf(float  x);

// Returns the arcus tangens in the range [-pi/2, +pi/2]
NA_IAPI double        naAtan (double x);
NA_IAPI float         naAtanf(float  x);

// Returns the arcus tangens of y/x in the range [-pi, +pi]
// The angle function does the same, but assumes x and y stored as
// xy[0] and xy[1] respectively.
NA_IAPI double        naAtan2 (double y, double x);
NA_IAPI float         naAtan2f(float  y, float  x);
NA_IAPI double        naAngle (const double* xy);
NA_IAPI float         naAnglef(const float*  xy);

// Returns the natural logarithm
NA_IAPI double        naLog (double x);
NA_IAPI float         naLogf(float  x);

// Returns the 10-logarithm
NA_IAPI double        naLog10 (double x);
NA_IAPI float         naLog10f(float  x);

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
NA_IAPI double        naLog2   (double  x);
NA_IAPI float         naLog2f  (float   x);
NA_IAPI int32         naLog2i32(int32   x);
NA_IAPI NAi64         naLog2i64(NAi64 x);
NA_IAPI size_t        naLog2s (size_t x);

// Returns the exponential function e^x
NA_IAPI double        naExp (double x);
NA_IAPI float         naExpf(float  x);

// Returns the exponent of 2 function 2^x
NA_IAPI float         naExp2f  (float   x);
NA_IAPI double        naExp2   (double  x);
NA_IAPI int32         naExp2i32(int32   x);
NA_IAPI NAi64         naExp2i64(NAi64 x);

// Returns the exponent of 10 function 10^x
NA_IAPI float         naExp10f  (float   x);
NA_IAPI double        naExp10   (double  x);
NA_IAPI int32         naExp10i32(int32   x);
NA_IAPI NAi64         naExp10i64(NAi64 x);

// Returns the power function b^x
NA_IAPI double        naPow (double b, double x);
NA_IAPI float         naPowf(float  b, float  x);

// Returns the power of 2 function 2^n with n being an integer
NA_IAPI double        naPowerOf2   (int32 n);
NA_IAPI float         naPowerOf2f  (int32 n);
NA_IAPI int32         naPowerOf2i32(int32 n);
NA_IAPI int64         naPowerOf2i64(int32 n);
NA_IAPI size_t        naPowerOf2s  (int32 n);

// Factorizes the given value. !-Operator
NA_IAPI int32         naFactorize(int32 x);

// Returns the binomial coefficient n over k
NA_IAPI int32         naBinom(int32 n, int32 k);

// Returns the the angle converted to radiants
NA_IAPI double        naDegToRad (double deg);
NA_IAPI float         naDegToRadf(float  deg);

// Returns the the angle converted to degrees
NA_IAPI double        naRadToDeg (double rad);
NA_IAPI float         naRadToDegf(float  rad);

// Angle functions. Assumes either x and y or r and theta to be stored in a
// two dimensional vector and converts it into the other.
NA_IAPI void          naPolarToCartesian (double* xy, const double* rtheta);
NA_IAPI void          naPolarToCartesianf(float*  xy, const float*  rtheta);
NA_IAPI void          naCartesianToPolar (double* rtheta, const double* xy);
NA_IAPI void          naCartesianToPolarf(float*  rtheta, const float*  xy);



// The evaluation functions will get the arbitrary object as const void* which
// was given to the naIntegrate function as the first parameter and as the
// second parameter, the coordinate at which to evaluate.
typedef float (*IntegrateFuncf)(const void*, float);
typedef double (*IntegrateFuncd)(const void*, double);

// These are standardized summation methods of values inside an array. While
// the simple sum is just a simple for loop, it might result in numerical
// errors. The naSum method is preferred as a very fast, very accurate method.
NA_API float naSimpleSumf(size_t sampleCount, const float* array);
NA_API float naKahanSumf(size_t sampleCount, const float* array);
NA_API float naKahanBabushkaNeumaierSumf(size_t sampleCount, const float* array);
NA_API float naKahanBabushkaKleinSumf(size_t sampleCount, const float* array);
NA_API float naSumf(size_t sampleCount, const float* array);

NA_API double naSimpleSum(size_t sampleCount, const double* array);
NA_API double naKahanSum(size_t sampleCount, const double* array);
NA_API double naKahanBabushkaNeumaierSum(size_t sampleCount, const double* array);
NA_API double naKahanBabushkaKleinSum(size_t sampleCount, const double* array);
NA_API double naSum(size_t sampleCount, const double* array);
NA_API double naSum2(size_t sampleCount, const double* array);

// Use the following integration methods to automatically call an evaluation
// function with the given user object data in a defined range from min to max
// with a defined number of samples.
// This function will not normalize the result but is a simple sum of all
// evaluated values instead. If you really need a proper integration, multiply
// the result by the according weight.
NA_API float naIntegratef(
  size_t sampleCount,
  IntegrateFuncf eval,
  const void* obj,
  float min,
  float max);

NA_API double naIntegrate(
  size_t sampleCount,
  IntegrateFuncd eval,
  const void* obj,
  double min,
  double max);



// ///////////////////////////////////
// Value ranges
// ///////////////////////////////////

// naInside returns true, if x is inside the span depicted by a and b. The
// value a should be smaller than b. The letters I and E depict whether a or b
// are compared inclusive or exclusive. For example:
// naInsideIE(3, 7, x) corresponds to a range of [3,7).

// inclusive, inclusive
NA_IAPI NABool naInsideII (double a, double b, double x);
NA_IAPI NABool naInsideIIf(float  a, float  b, float  x);
// inclusive, exclusive
NA_IAPI NABool naInsideIE (double a, double b, double x); // use these if you
NA_IAPI NABool naInsideIEf(float  a, float  b, float  x); // are uncertain.
// exclusive, inclusive
NA_IAPI NABool naInsideEI (double a, double b, double x);
NA_IAPI NABool naInsideEIf(float  a, float  b, float  x);
// exclusive, exclusive
NA_IAPI NABool naInsideEE (double a, double b, double x);
NA_IAPI NABool naInsideEEf(float  a, float  b, float  x);

// Integer variants. a denotes Min, b denotes Max. Comparison is always
// inclusive - inclusive.
NA_IAPI NABool naInsidei32(int32  a, int32  b, int32  x);
NA_IAPI NABool naInsidei64(NAi64 a, NAi64 b, NAi64 x);

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
NA_IAPI NABool naInsideNormZIf(float  x);
NA_IAPI NABool naInsideNormZE (double x);
NA_IAPI NABool naInsideNormZEf(float  x);
NA_IAPI NABool naInsideNormII (double x);
NA_IAPI NABool naInsideNormIIf(float  x);
NA_IAPI NABool naInsideNormIE (double x);
NA_IAPI NABool naInsideNormIEf(float  x);
NA_IAPI NABool naInsideNormEE (double x);
NA_IAPI NABool naInsideNormEEf(float  x);





// Inline implementations are in a separate file:
#include "Core/NAMathOperatorsII.h"






#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_MATH_OPERATORS_INCLUDED



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
