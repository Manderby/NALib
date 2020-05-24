
#if defined NA_FLOATING_POINT_II_INCLUDED || !defined NA_BASE_INCLUDED
  #warning "Do not include this file directly. Use NABase.h"
#endif
#ifndef NA_FLOATING_POINT_II_INCLUDED
#define NA_FLOATING_POINT_II_INCLUDED



// This file contains inline implementations of floating point calculations.



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
// Now, with that out of the way, the IEEE-754 standard ranges of the
// exponents are indeed [-126, 127] and [-1022, 1023]. The standard defines
// a bias, to shift these ranges to a positive range: For floats, a bias 127
// is added resulting in [1, 254] and for double, 1023 is added, resulting
// in [1, 2046]. The min and max values (0 and 255 or 2047 respectively) are
// reserved for special numbers like Infinite, NaN or subnormals.
//
// The bias is by IEEE-754 defined to be (2^(exp_bitcount-1))-1. But why? Why
// is the bias 127 or 1023 respectively? Why not 128 and 1024? Or something
// completely different? The reasons lie in history: In earlier times, many
// chip manufacturors tried to make good floating point processors. Pretty
// much all of them agreed that when working with significands and exponents,
// the exponents should be approximately equally distributed in the positive
// as well as in the negative range. Therefore, there are only two possible
// outcomes. But what decided to have 1 more in the positive range? It was
// the handling of subnormal numbers. There was an ancient fight in how to
// handle numbers which are too low to be expressed as a floating point
// number: The subnormal numbers. One party (DEC, bias 128) said, they should
// be treated like zero, the other party (IEEE, bias 127) said they could be
// treated slightly differently than the rest of the normalized floating
// point numbers, namely as so called gradual underflow. Both sides had their
// advantages and disadvantages back in the day, but long story short, the
// second party (IEEE) found ways to diminish the disadvantages, won over and
// therefore, today, fewer negative exponents were needed as they were
// somewhat sufficiently being represented by these subnormal numbers and
// hence the positive exponents gained a whole power of 2 more. End of line.
//
// Source:
// https://people.eecs.berkeley.edu/~wkahan/ieee754status/754story.html


#include "float.h"
#ifndef NDEBUG
  #include "NAMathConstants.h"
#endif

#ifndef FLT_RADIX
#warning "NALib assumes floating points to have a radix of 2"
#define FLT_RADIX 2
#endif

#if FLT_RADIX != 2
#error "NALib requires floating points to have a radix of 2"
#endif



// Undefine the defines of the .h file.
#undef NA_IEEE754_SINGLE_BITS
#undef NA_IEEE754_SINGLE_SIGNIFICAND_BITS
#undef NA_IEEE754_SINGLE_SIGNIFICAND_NORM
#undef NA_IEEE754_SINGLE_SIGNIFICAND_MASK
#undef NA_IEEE754_SINGLE_EXPONENT_BITS
#undef NA_IEEE754_SINGLE_EXPONENT_BIAS
#undef NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL
#undef NA_IEEE754_SINGLE_EXPONENT_SPECIAL
#undef NA_IEEE754_SINGLE_EXPONENT_MASK
#undef NA_IEEE754_SINGLE_SIGN_MASK
#undef NA_IEEE754_DOUBLE_BITS
#undef NA_IEEE754_DOUBLE_SIGNIFICAND_BITS
#undef NA_IEEE754_DOUBLE_SIGNIFICAND_NORM
#undef NA_IEEE754_DOUBLE_SIGNIFICAND_MASK
#undef NA_IEEE754_DOUBLE_EXPONENT_BITS
#undef NA_IEEE754_DOUBLE_EXPONENT_BIAS
#undef NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL
#undef NA_IEEE754_DOUBLE_EXPONENT_SPECIAL
#undef NA_IEEE754_DOUBLE_EXPONENT_MASK
#undef NA_IEEE754_DOUBLE_SIGN_MASK
// Now, redefine them properly.

// single precision
#define NA_IEEE754_SINGLE_BITS                   NA_TYPE32_BITS
#define NA_IEEE754_SINGLE_SIGNIFICAND_BITS       (FLT_MANT_DIG - 1)
#define NA_IEEE754_SINGLE_SIGNIFICAND_NORM       (1 << NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
#define NA_IEEE754_SINGLE_SIGNIFICAND_MASK       (NA_IEEE754_SINGLE_SIGNIFICAND_NORM - 1)
#define NA_IEEE754_SINGLE_EXPONENT_BITS          (NA_IEEE754_SINGLE_BITS - 1 - NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
#define NA_IEEE754_SINGLE_EXPONENT_BIAS          ((1 << (NA_IEEE754_SINGLE_EXPONENT_BITS - 1)) - 1)
#define NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL     (-NA_IEEE754_SINGLE_EXPONENT_BIAS)
#define NA_IEEE754_SINGLE_EXPONENT_SPECIAL       ((1 << NA_IEEE754_SINGLE_EXPONENT_BITS) - NA_IEEE754_SINGLE_EXPONENT_BIAS - 1)
#define NA_IEEE754_SINGLE_EXPONENT_MASK          (((1u << (NA_IEEE754_SINGLE_BITS - 1)) - 1) & ~NA_IEEE754_SINGLE_SIGNIFICAND_MASK)
#define NA_IEEE754_SINGLE_SIGN_MASK              (1u << (NA_IEEE754_SINGLE_BITS  - 1))

// double precision
#define NA_IEEE754_DOUBLE_BITS                   NA_TYPE64_BITS
#define NA_IEEE754_DOUBLE_SIGNIFICAND_BITS       (DBL_MANT_DIG - 1)
#define NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_HI    (1u << (NA_IEEE754_DOUBLE_SIGNIFICAND_BITS - NA_TYPE32_BITS))
#define NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_LO    0x0u
#define NA_IEEE754_DOUBLE_SIGNIFICAND_NORM       naCastu64Toi64(naMakeu64(NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_HI, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_LO))
#define NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_HI    (NA_IEEE754_DOUBLE_SIGNIFICAND_NORM_HI - 1)
#define NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_LO    NA_MAX_u32
#define NA_IEEE754_DOUBLE_SIGNIFICAND_MASK       naCastu64Toi64(naMakeu64(NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_HI, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_LO))
#define NA_IEEE754_DOUBLE_EXPONENT_BITS          (NA_IEEE754_DOUBLE_BITS - 1 - NA_IEEE754_DOUBLE_SIGNIFICAND_BITS)
#define NA_IEEE754_DOUBLE_EXPONENT_BIAS          ((1 << (NA_IEEE754_DOUBLE_EXPONENT_BITS - 1)) - 1)
#define NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL     (-NA_IEEE754_DOUBLE_EXPONENT_BIAS)
#define NA_IEEE754_DOUBLE_EXPONENT_SPECIAL       ((1 << NA_IEEE754_DOUBLE_EXPONENT_BITS) - NA_IEEE754_DOUBLE_EXPONENT_BIAS - 1)
#define NA_IEEE754_DOUBLE_EXPONENT_MASK_HI       (((1u << (NA_IEEE754_DOUBLE_BITS - NA_TYPE32_BITS - 1)) - 1) & ~NA_IEEE754_DOUBLE_SIGNIFICAND_MASK_HI)
#define NA_IEEE754_DOUBLE_EXPONENT_MASK_LO       0x0u
#define NA_IEEE754_DOUBLE_EXPONENT_MASK          naCastu64Toi64(naMakeu64(NA_IEEE754_DOUBLE_EXPONENT_MASK_HI, NA_IEEE754_DOUBLE_EXPONENT_MASK_LO))
#define NA_IEEE754_DOUBLE_SIGN_MASK_HI           (1u << (NA_IEEE754_DOUBLE_BITS - NA_TYPE32_BITS - 1))
#define NA_IEEE754_DOUBLE_SIGN_MASK_LO           0x0u
#define NA_IEEE754_DOUBLE_SIGN_MASK              naCastu64Toi64(naMakeu64(NA_IEEE754_DOUBLE_SIGN_MASK_HI, NA_IEEE754_DOUBLE_SIGN_MASK_LO))



NA_IDEF float naMakeFloat(int32 signedSignificand, int32 signedExponent){
  #ifndef NDEBUG
    if(signedExponent < NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("exponent too low for single precision");
    if((signedSignificand != 0) && (signedExponent == NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL))
      naError("exponent creates subnormal number");
    if(signedExponent > NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent too high for single precision");
    if(signedExponent == NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
    if(((uint32)naAbsi32(signedSignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  int32 dBits = (int32)(
      ((uint32)signedSignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | (((uint32)signedExponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
    | ((uint32)naAbsi32(signedSignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK));
  return *((float*)(void*)&dBits);
  // Note that the additional (void*) is necessary for static code analizers.
}



NA_IDEF float naMakeFloatWithExponent(int32 signedExponent){
  #ifndef NDEBUG
    if(signedExponent < NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("exponent too low for single precision");
    if(signedExponent == NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("exponent creates subnormal number");
    if(signedExponent > NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent too high for single precision");
    if(signedExponent == NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
  #endif
  int32 dBits = ((signedExponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BITS);
  return *((float*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF float naMakeFloatSubnormal(int32 signedSignificand) {
  #ifndef NDEBUG
  if ((naAbsi32(signedSignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
    naError("significand out of range");
  #endif
  int32 dBits = (int32)(
    ((uint32)signedSignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | ((uint32)naAbsi32(signedSignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK));
  return *((float*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDouble(NAi64 signedSignificand, int32 signedExponent) {
  #ifndef NDEBUG
  if (signedExponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
    naError("exponent too low for double precision");
  if (!naEquali64(signedSignificand, NA_ZERO_i64) && (signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL))
    naError("exponent creates subnormal number");
  if (signedExponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
    naError("exponent too high for double precision");
  if (signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
    naError("exponent equals max exponent which is reserved for special values");
  if (naGreateru64(naCasti64Tou64(naAbsi64(signedSignificand)), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
    naError("significand out of range");
  #endif
  NAi64 dBits =
    naOri64(naOri64(
      naAndi64(signedSignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naShli64(naMakei64WithLo(signedExponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS)),
      naAndi64(naAbsi64(signedSignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDoubleWithExponent(int32 signedExponent){
  #ifndef NDEBUG
    if(signedExponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("exponent too low for double precision");
    if(signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("exponent creates subnormal number");
    if(signedExponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent too high for double precision");
    if(signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
  #endif
  NAi64 dBits = naShli64(naMakei64WithLo(signedExponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  return *((double*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDoubleSubnormal(NAi64 signedSignificand){
  #ifndef NDEBUG
    if(naGreateri64(naAbsi64(signedSignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  NAi64 dBits =
      naOri64(
      naAndi64(signedSignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naAndi64(naAbsi64(signedSignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IAPI int32 naGetDoubleExponent(double d){
  #ifndef NDEBUG
    if(d == 0.)
      naError("Given number is 0. Result will be subnormal exponent");
    if(d == NA_INFINITY || d == -NA_INFINITY)
      naError("Given number is +-Infinity. Result will be max+1 exponent");
  #endif
  NAi64 dBits = *((NAi64*)(void*)&d);
  dBits = naAndi64(dBits, NA_IEEE754_DOUBLE_EXPONENT_MASK);
  dBits = naShri64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  #ifndef NDEBUG
    if(d != 0. && dBits == 0)
      naError("Given number is subnormal. Result will always be subnormal exponent");
  #endif
  return naCasti64Toi32(dBits) - NA_IEEE754_DOUBLE_EXPONENT_BIAS;
}



NA_IAPI NAi64 naGetDoubleInteger(double d){
  #ifndef NDEBUG
    if(d == NA_INFINITY || d == -NA_INFINITY)
      naError("Given number is +-Infinity. Result will be undefined");
    if(fabs(d) > 0x1.fffffffffffffp52)
      naError("Given numbers absolute value is too large. Result will be undefined");
    #endif
  NAi64 dBits = *((NAi64*)(void*)&d);
  if(d != 0.){
    dBits = naAndi64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
    dBits = naOri64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
    int32 exponent = naGetDoubleExponent(d);
    if(exponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL || exponent < 0){
      dBits = NA_ZERO_i64;
    }else{
      dBits = naShri64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_BITS - exponent);
      if(d < 0){dBits = naNegi64(dBits);}
    }
  }
  return dBits;
}



NA_IAPI NAi64 naGetDoubleFraction(double d){
  #ifndef NDEBUG
  if(naGetDoubleInteger(d))
    naError("Less than 15 decimal digits available for accuracy. Result may contain rounding errors. Use E or Slow variant.");
  #endif
  NAi64 result = naGetDoubleFractionE(d);
  #ifndef NDEBUG
  if(result == 1000000000000000)
    naError("Fraction rounded up to 1e16. Take care of this case in your code and then use the E method to inhibit this error.");
  #endif
  return result;
}



NA_IAPI NAi64 naGetDoubleFractionE(double d){
  int32 exponent;
  NAi64 dbits = *((NAi64*)(void*)&d);
  if(d == 0.){
    dbits = NA_ZERO_i64;
  }else{
    exponent = naGetDoubleExponent(d);
    if(exponent < 0){
      dbits = naAndi64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
      dbits = naOri64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
      dbits = naShri64(dbits, -exponent - 1);
      naIncu64(dbits);
      dbits = naShri64(dbits, 1);
    }else{
      dbits = naShli64(dbits, exponent);
      dbits = naAndi64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
    }

    NAi128 hyperTens = naMakei128(NA_ZERO_i64, 0x71afd498d0000000);  // = 1e15 * 2^13
    NAi128 hyperBits = naMakei128(NA_ZERO_i64, naCasti64Tou64(dbits));
    hyperBits = naMuli128(hyperBits, hyperTens);
    dbits = naGeti128Hi(hyperBits);

    if(exponent > 0){
      dbits = naShri64(dbits, exponent);
    }
    naInci64(dbits);
    dbits = naShri64(dbits, 1);
    if(exponent > 0){
      dbits = naShli64(dbits, exponent);
    }
  }
  return dbits;
}



NA_IAPI NAi64 naGetDoubleFractionSlow(double d){
  NAi64 result = naGetDoubleFractionSlowE(d);
  #ifndef NDEBUG
    if(result == 1000000000000000)
      naError("Fraction rounded up to 1e16. Take care of this case in your code and then use the SlowE method to inhibit this error.");
    #endif
  return result;
}



NA_IAPI NAi64 naGetDoubleFractionSlowE(double d){
  int32 exponent;
  NAi64 dbits = *((NAi64*)(void*)&d);
  if(d == 0.){
    dbits = NA_ZERO_i64;
  }else{
    NAi128 hyperTens = naMakei128(NA_ZERO_i64, 0x71afd498d0000000);  // = 1e15 * 2^13
    hyperTens = naMakei128(NA_ZERO_i64, 1000000000000000);  // 1e15
    NAu64 mul = 1;
    exponent = naGetDoubleExponent(d);
    if(exponent < 0){
      dbits = naAndi64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
      dbits = naOri64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
      dbits = naShri64(dbits, -exponent - 1);
      naIncu64(dbits);
      dbits = naShri64(dbits, 1);
    }else{
      dbits = naShli64(dbits, exponent);
      dbits = naAndi64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
      int64 i = naGetDoubleInteger(d);
      while(i){i /= 10; mul *= 10;}
      hyperTens.lo /= mul;
    }

    NAi128 hyperBits = naMakei128(NA_ZERO_i64, naCasti64Tou64(dbits));
    hyperBits = naMuli128(hyperBits, hyperTens);
    hyperBits = naShli128(hyperBits, 13);

    dbits = naGeti128Hi(hyperBits);
    dbits = naInci64(dbits);
    dbits = naShri64(dbits, 1);
    dbits = naMuli64(dbits, mul);
  }
  return dbits;
}



#endif // NA_FLOATING_POINT_II_INCLUDED



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
