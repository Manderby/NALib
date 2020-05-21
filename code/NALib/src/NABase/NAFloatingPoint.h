
#ifndef NA_FLOATING_POINT_INCLUDED
#define NA_FLOATING_POINT_INCLUDED


#include "../NABase.h"

// Following are the macros for the most important constants when working
// with IEEE-754 floating point values. They are properly defined in the
// NAFloatingPointII.h file.
//
//                       | single      | double
// ----------------------+-------------+--------------------
// BITS                  | 32          | 64
// SIGNIFICAND_BITS      | 23          | 52
// SIGNIFICAND_NORM      | 0x00800000  | 0x0010000000000000
// SIGNIFICAND_MASK      | 0x007fffff  | 0x000fffffffffffff
// EXPONENT_BITS         | 8           | 11
// EXPONENT_BIAS         | 127         | 1023
// EXPNENT_SUBNORMAL     | -127        | -1023
// EXPNENT_SPECIAL       | 128         | 1024
// EXPONENT_MASK         | 0x7f800000  | 0x7ff0000000000000
// SIGN_MASK             | 0x80000000  | 0x8000000000000000


// single precision
#define NA_IEEE754_SINGLE_BITS
#define NA_IEEE754_SINGLE_SIGNIFICAND_BITS
#define NA_IEEE754_SINGLE_SIGNIFICAND_NORM
#define NA_IEEE754_SINGLE_SIGNIFICAND_MASK
#define NA_IEEE754_SINGLE_EXPONENT_BITS
#define NA_IEEE754_SINGLE_EXPONENT_BIAS
#define NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL
#define NA_IEEE754_SINGLE_EXPONENT_SPECIAL
#define NA_IEEE754_SINGLE_EXPONENT_MASK
#define NA_IEEE754_SINGLE_SIGN_MASK

// double precision
#define NA_IEEE754_DOUBLE_BITS
#define NA_IEEE754_DOUBLE_SIGNIFICAND_BITS
#define NA_IEEE754_DOUBLE_SIGNIFICAND_NORM
#define NA_IEEE754_DOUBLE_SIGNIFICAND_MASK
#define NA_IEEE754_DOUBLE_EXPONENT_BITS
#define NA_IEEE754_DOUBLE_EXPONENT_BIAS
#define NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL
#define NA_IEEE754_DOUBLE_EXPONENT_SPECIAL
#define NA_IEEE754_DOUBLE_EXPONENT_MASK
#define NA_IEEE754_DOUBLE_SIGN_MASK


// Creates floats and doubles out of signed significands and exponents
NA_IAPI float  naMakeFloat (int32 signedSignificand, int32 signedExponent);
NA_IAPI float  naMakeFloatWithExponent (int32 signedExponent);
NA_IAPI float  naMakeFloatSubnormal (int32 signedSignificand);

NA_IAPI double naMakeDouble(NAi64 signedSignificand, int32 signedExponent);
NA_IAPI double naMakeDoubleWithExponent(int32 signedExponent);
NA_IAPI double naMakeDoubleSubnormal(NAi64 signedSignificand);


NA_IAPI int32   naGetDoubleExponent(double d);
NA_IAPI NAi64 naGetDoubleInteger(double d);
NA_IAPI NAi64 naGetDoubleFraction(double d);



// Inline implementations are in a separate file:
#include "NAFloatingPointII.h"


#endif // NA_FLOATING_POINT_INCLUDED



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
