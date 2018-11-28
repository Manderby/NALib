
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_BINARY_OPERATORS_INCLUDED
#define NA_BINARY_OPERATORS_INCLUDED

#include "NABase.h"
#include "float.h"

// In this file, you find floating point constants defined by IEEE-754. Some
// of these constants are based on definitions found in float.h but the
// IEEE-754 standard has evolved since and NALib tries to do the right thing.
// There are several things to consider:
//
// - The significand corresponds to what was known as the "mantisse" in
//   traditional naming. That wording has changed since the first standard.
// - In order to incorporate future expansion, the term FLOAT is omitted but
//   instead, the term SINGLE is used. In a future version, there might be
//   macros like HALF, QUAD or even OCT.
// - The bit count is nowhere to be found in float.h and therefore had been
//   hard coded here.
// - The FLT_MANT_DIG and DBL_MANT_DIG macros of float.h are 1 higher than
//   the IEEE-754 standard. That's because the actual number of digits for
//   a normalized significand always carries a leading 1.
//
// Further more, the author decided to really dig into these numbers and find
// out why there is no bias available as a macro and why the existing min and
// max exponents do not match the ones of the standard. Here are the findings
// for those which are interested:
//
// The reason for FLT_MIN_EXP being -125 instead of -126, FLT_MAX_EXP being
// 128 instead of 127, DBL_MIN_EXP being -1021 instead of -1022, and
// DBL_MAX_EXP being 1024 instead of 1023 is because these macros are used
// for the computation of the highest and lowest possible normalized numbers
// using this formula:
// max = 2^(exp_max+1) - 2^(exp_max+1 - sig_bitcount)
// In other words, for example for a floating point representation with
// maximum exponent 12 and 4 bits of significand this formula looks like
// this when written in binary integer form:
// max = 1000000000000 - 0000100000000
// By subtracting 1 from each term, we get
// max = 0111111111111 - 0000011111111
// hence, max = 0111100000000 which is exactly what the maximum number with
// 4 digits of significand would look like when expressed as an integer. As
// in that formula, the expression (exp_max+1) is used twice, the guys from
// the C standard thought: Why not store it directly with 1 more? And thats
// basically the whole story.
//
// Now, with that out of the way, the IEEE-754 standard ranges of the exponents
// are indeed [-126, 127] and [-1022, 1023]. The standard defines a bias, to
// shift these ranges to a positive range: For floats, the bias 127 is added
// resulting in [1, 254] and for double, 1023 is added, resulting in [1, 2046].
// The lowest and highest values (0 and 255 or 2047 respectively) are reserved
// for special numbers like Infinite, NaN or subnormals.
//
// The bias is by IEEE-754 defined to be (2^(exp_bitcount-1))-1. But why? Why
// is the bias 127 or 1023 respectively? Why not 128 and 1024? Or something
// completely different? The reasons lie in history: In earlier times, many
// chip manufacturors tried to make good floating point processors. Pretty
// much all of them agreed that when working with significands and exponents,
// the exponents should be approximately equally distributed in the positive
// as well as in the negative range. Therefore, there are only two possible
// outcomes. But what decided to have 1 more in the positive range? It was the
// handling of subnormal numbers. There was an ancient fight in how to handle
// numbers which are too low to be expressed as a floating point number: The
// subnormal numbers. One party said, they should be treated like zero, the
// other party said they could be treated slightly differently than the rest
// of the floating point numbers, namely as so called gradual underflow. Both
// sides have advantages and disadvantages, but long story short, the second
// party won over and therefore, now, fewer negative exponents were needed as
// they were somewhat sufficiently being represented by these subnormal numbers
// and hence the positive exponents gained a whole power of 2 more. The end.


#if FLT_RADIX != 2
  #warning "NALib requires floating points to have a radix of 2"
#endif

// Following are the macros for the most important constants when working with
// IEEE-754 floating point values:
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

#define NA_IEEE754_SINGLE_BITS                   NA_TYPE32_BITS
#define NA_IEEE754_SINGLE_SIGNIFICAND_BITS       (FLT_MANT_DIG - 1)
#define NA_IEEE754_SINGLE_SIGNIFICAND_NORM       (1 << NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
#define NA_IEEE754_SINGLE_SIGNIFICAND_MASK       (NA_IEEE754_SINGLE_SIGNIFICAND_NORM - 1)
#define NA_IEEE754_SINGLE_EXPONENT_BITS          (NA_IEEE754_SINGLE_BITS - 1 - NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
#define NA_IEEE754_SINGLE_EXPONENT_BIAS          ((1 << (NA_IEEE754_SINGLE_EXPONENT_BITS - 1)) - 1)
#define NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL     (-NA_IEEE754_SINGLE_EXPONENT_BIAS)
#define NA_IEEE754_SINGLE_EXPONENT_SPECIAL       ((1 << NA_IEEE754_SINGLE_EXPONENT_BITS) - NA_IEEE754_SINGLE_EXPONENT_BIAS - 1)
#define NA_IEEE754_SINGLE_EXPONENT_MASK          (((1 << (NA_IEEE754_SINGLE_BITS - 1)) - 1) & ~NA_IEEE754_SINGLE_SIGNIFICAND_MASK)
#define NA_IEEE754_SINGLE_SIGN_MASK              (1 << (NA_IEEE754_SINGLE_BITS  - 1))

#define NA_IEEE754_DOUBLE_BITS                   NA_TYPE64_BITS
#define NA_IEEE754_DOUBLE_SIGNIFICAND_BITS       (DBL_MANT_DIG - 1)
#define NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_HI    (1u << (NA_IEEE754_DOUBLE_SIGNIFICAND_BITS - NA_TYPE32_BITS))
#define NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_LO    0x0u
#define NA_IEEE754_DOUBLE_SIGNIFICAND_NORM       naCastUInt64ToInt64(naMakeUInt64(NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_HI, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_LO))
#define NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_HI    (NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_HI - 1)
#define NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_LO    NA_UINT32_MAX
#define NA_IEEE754_DOUBLE_SIGNIFICAND_MASK       naCastUInt64ToInt64(naMakeUInt64(NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_HI, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_LO))
#define NA_IEEE754_DOUBLE_EXPONENT_BITS          (NA_IEEE754_DOUBLE_BITS - 1 - NA_IEEE754_DOUBLE_SIGNIFICAND_BITS)
#define NA_IEEE754_DOUBLE_EXPONENT_BIAS          ((1 << (NA_IEEE754_DOUBLE_EXPONENT_BITS - 1)) - 1)
#define NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL     (-NA_IEEE754_DOUBLE_EXPONENT_BIAS)
#define NA_IEEE754_DOUBLE_EXPONENT_SPECIAL       ((1 << NA_IEEE754_DOUBLE_EXPONENT_BITS) - NA_IEEE754_DOUBLE_EXPONENT_BIAS - 1)
#define NA_IEEE754_DOUBLE_EXPONENT_MASK_HI       (((1u << (NA_IEEE754_DOUBLE_BITS - NA_TYPE32_BITS - 1)) - 1) & ~NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_HI)
#define NA_IEEE754_DOUBLE_EXPONENT_MASK_LO       0x0u
#define NA_IEEE754_DOUBLE_EXPONENT_MASK          naCastUInt64ToInt64(naMakeUInt64(NA_IEEE754_DOUBLE_EXPONENT_MASK_HI, NA_IEEE754_DOUBLE_EXPONENT_MASK_LO))
#define NA_IEEE754_DOUBLE_SIGN_MASK_HI           (1u << (NA_IEEE754_DOUBLE_BITS - NA_TYPE32_BITS - 1))
#define NA_IEEE754_DOUBLE_SIGN_MASK_LO           0x0u
#define NA_IEEE754_DOUBLE_SIGN_MASK              naCastUInt64ToInt64(naMakeUInt64(NA_IEEE754_DOUBLE_SIGN_MASK_HI, NA_IEEE754_DOUBLE_SIGN_MASK_LO))

// Creates floats and doubles out of signed significands and exponents
NA_IAPI float  naMakeFloat (int32 signedsignificand, int32 signedexponent);
NA_IAPI float  naMakeFloatWithExponent (int32 signedexponent);
NA_IAPI float  naMakeFloatSubnormal (int32 signedsignificand);

NA_IAPI double naMakeDouble(int64 signedsignificand, int32 signedexponent);
NA_IAPI double naMakeDoubleWithExponent(int32 signedexponent);
NA_IAPI double naMakeDoubleSubnormal(int64 signedsignificand);

NA_IAPI int32 naGetDoubleExponent(double d);
NA_IAPI int64 naGetDoubleInteger(double d);



#endif // NA_BINARY_OPERATORS_INCLUDED

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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
