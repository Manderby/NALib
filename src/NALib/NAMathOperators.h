
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_MATH_OPERATORS_INCLUDED
#define NA_MATH_OPERATORS_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NABase.h"


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

// Returns the sign as a multiplicator. x < 0 returns -1, x >= 0 returns 1.
// Never returns 0!  You can use the result of this function safely as a
// multiplication factor. You can also subtract the result of this function
// from the comma ASCII character and get a string representation of the sign:
// NAUTF8Char signASCII = ',' - naSigni(-42);
NA_IAPI NAInt         naSigni  (NAInt x);

// naAlmostZero   Returns NA_TRUE, if the ABSOLUTE discance between x and 0 is
//                smaller than NA_SINGULARITY.
// naAlmostOne    Returns NA_TRUE, if the ABSOLUTE discance between x and 1 is
//                smaller than NA_SINGULARITY.
// naAlmost       Returns NA_TRUE, if the RELATIVE distance between x and y is
//                smaller than NA_SINGULARITY. If y is close to 0, the absolute
//                distance is checked.
// Warning: These functions are slow. Especially naAlmost and naAlmostf.
NA_IAPI NABool        naAlmostZerof (float  x);
NA_IAPI NABool        naAlmostZero  (double x);
NA_IAPI NABool        naAlmostOne   (double x);
NA_IAPI NABool        naAlmostOnef  (float  x);
NA_IAPI NABool        naAlmostf     (float  x, float  y);
NA_IAPI NABool        naAlmost      (double x, double y);

// Returns true if the given number is NaN, infinite.
NA_IAPI NABool        naIsNaNf(float  x);
NA_IAPI NABool        naIsNaN (double x);
NA_IAPI NABool        naIsInfinitef(float  x);
NA_IAPI NABool        naIsInfinite (double x);

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
NA_IAPI float        naMinf  (float  a, float  b);
NA_IAPI double       naMin   (double a, double b);
NA_IAPI NAInt        naMini  (NAInt  a, NAInt  b);
NA_IAPI int8         naMini8 (int8   a, int8   b);
NA_IAPI int16        naMini16(int16  a, int16  b);
NA_IAPI int32        naMini32(int32  a, int32  b);
NA_IAPI int64        naMini64(int64  a, int64  b);

NA_IAPI float        naMaxf  (float  a, float  b);
NA_IAPI double       naMax   (double a, double b);
NA_IAPI NAInt        naMaxi  (NAInt  a, NAInt  b);
NA_IAPI int8         naMaxi8 (int8   a, int8   b);
NA_IAPI int16        naMaxi16(int16  a, int16  b);
NA_IAPI int32        naMaxi32(int32  a, int32  b);
NA_IAPI int64        naMaxi64(int64  a, int64  b);




// ///////////////////////////////////
// Mathematical operators
// ///////////////////////////////////

// Inverse: Returns 1/x
NA_IAPI float         naInvf(float  x);
NA_IAPI double        naInv (double x);

// Returns the square root
NA_IAPI float         naSqrtf(float  x);
NA_IAPI double        naSqrt (double x);

// Returns the cubic root
NA_IAPI float         naCbrtf(float  x);
NA_IAPI double        naCbrt (double x);

// Returns the absolute value
NA_IAPI float         naAbsf  (float  x);
NA_IAPI double        naAbs   (double x);
NA_IAPI NAInt         naAbsi  (NAInt  x);

// Returns the rounded down value
NA_IAPI float         naFloorf(float  x);
NA_IAPI double        naFloor (double x);

// Returns the rounded up value
NA_IAPI float         naCeilf(float  x);
NA_IAPI double        naCeil (double x);

// Returns the rounded value
NA_IAPI float         naRoundf(float  x);
NA_IAPI double        naRound (double x);

// Returns the modular value. Return value is always positive.
NA_IAPI float         naModf(float  x, float  mod);
NA_IAPI double        naMod (double x, double mod);

// Returns the sinus
NA_IAPI float         naSinf(float  x);
NA_IAPI double        naSin (double x);

// Returns the cosinus
NA_IAPI float         naCosf(float  x);
NA_IAPI double        naCos (double x);

// Returns the tangens
NA_IAPI float         naTanf(float  x);
NA_IAPI double        naTan (double x);

// Returns the arcus sinus
NA_IAPI float         naAsinf(float  x);
NA_IAPI double        naAsin (double x);

// Returns the arcus cosinus
NA_IAPI float         naAcosf(float  x);
NA_IAPI double        naAcos (double x);

// Returns the arcus tangens in the range [-pi/2, +pi/2]
NA_IAPI float         naAtanf(float  x);
NA_IAPI double        naAtan (double x);

// Returns the arcus tangens of y/x in the range [-pi, +pi]
// The angle function does the same, but assumes x and y stored as
// xy[0] and xy[1] respectively.
NA_IAPI float         naAtan2f(float  y, float  x);
NA_IAPI double        naAtan2 (double y, double x);
NA_IAPI float         naAnglef(const float*  xy);
NA_IAPI double        naAngle (const double* xy);

// Returns the natural logarithm
NA_IAPI float         naLogf(float  x);
NA_IAPI double        naLog (double x);

// Returns the 10-logarithm
NA_IAPI float         naLog10f(float  x);
NA_IAPI double        naLog10 (double x);

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
NA_IAPI float         naLog2f(float   x);
NA_IAPI double        naLog2 (double  x);
NA_IAPI NAInt         naLog2i(NAInt   x);
NA_IAPI int32         naLog2i32(int32 x);
NA_IAPI int64         naLog2i64(int64 x);

// Returns the exponential function e^x
NA_IAPI float         naExpf(float  x);
NA_IAPI double        naExp (double x);

// Returns the exponent of 2 function 2^x
NA_IAPI float         naExp2f  (float  x);
NA_IAPI double        naExp2d  (double x);
NA_IAPI NAInt         naExp2i  (NAInt  x);
NA_IAPI int32         naExp2i32(int32  x);
NA_IAPI int64         naExp2i64(int64  x);

// Returns the exponent of 10 function 10^x
NA_IAPI float         naExp10f  (float  x);
NA_IAPI double        naExp10d  (double x);
NA_IAPI NAInt         naExp10i  (NAInt  x);
NA_IAPI int32         naExp10i32(int32  x);
NA_IAPI int64         naExp10i64(int64  x);

// Returns the power function b^x
NA_IAPI float         naPowf(float  b, float  x);
NA_IAPI double        naPow (double b, double x);

// Returns the power of 2 function 2^n with n being an integer
NA_IAPI float         naPowerOf2f(NAInt n);
NA_IAPI double        naPowerOf2 (NAInt n);

// Factorizes the given value. !-Operator
NA_IAPI NAInt         naFactorize(NAInt x);

// Returns the binomial coefficient n over k
NA_IAPI NAInt         naBinom(NAInt n, NAInt k);

// Returns the the angle converted to radiants
NA_IAPI float         naDegToRadf(float  deg);
NA_IAPI double        naDegToRad (double deg);

// Returns the the angle converted to degrees
NA_IAPI float         naRadToDegf(float  rad);
NA_IAPI double        naRadToDeg (double rad);

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
#include "NAMath/NAMathOperatorsII.h"






#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_MATH_OPERATORS_INCLUDED


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
