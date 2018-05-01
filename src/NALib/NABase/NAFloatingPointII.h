
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABinaryOperators.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryOperators.h"





NA_IDEF float naCreateFloat(int32 signedsignificand, int32 signedexponent){
  int32 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("naCreateFloat", "exponent too low for single precision");
    if((signedsignificand != 0) && (signedexponent == NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL))
      naError("naCreateFloat", "exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("naCreateFloat", "exponent too high for single precision");
    if(signedexponent == NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("naCreateFloat", "exponent equals max exponent which is reserved for special values");
    if((naAbsi32(signedsignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("naCreateFloat", "significand out of range");
  #endif
  dbits =
      (signedsignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | ((signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
    | (naAbsi32(signedsignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK);
  return *((float*)&dbits);
}

NA_IDEF double naCreateDouble(int64 signedsignificand, int32 signedexponent){
  int64 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("naCreateDouble", "exponent too low for double precision");
    if(!naEqualInt64(signedsignificand, NA_ZERO_64) && (signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL))
      naError("naCreateDouble", "exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("naCreateDouble", "exponent too high for double precision");
    if(signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("naCreateDouble", "exponent equals max exponent which is reserved for special values");
    if(naGreaterInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("naCreateDouble", "significand out of range");
  #endif
  dbits =
      naOrInt64(naOrInt64(
      naAndInt64(signedsignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naShlInt64(naMakeInt64WithLo(signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS)),
      naAndInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)&dbits);
}

NA_IDEF float naCreateFloatWithExponent(int32 signedexponent){
  int32 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("naCreateFloat", "exponent too low for single precision");
    if(signedexponent == NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("naCreateDouble", "exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("naCreateFloat", "exponent too high for single precision");
    if(signedexponent == NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("naCreateFloat", "exponent equals max exponent which is reserved for special values");
  #endif
  dbits = ((signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BITS);
  return *((float*)&dbits);
}

NA_IDEF double naCreateDoubleWithExponent(int32 signedexponent){
  int64 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("naCreateDouble", "exponent too low for double precision");
    if(signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("naCreateDouble", "exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("naCreateDouble", "exponent too high for double precision");
    if(signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("naCreateDouble", "exponent equals max exponent which is reserved for special values");
  #endif
  dbits = naShlInt64(naMakeInt64WithLo(signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  return *((double*)&dbits);
}

NA_IDEF float  naCreateFloatSubnormal (int32 signedsignificand){
  int32 dbits;
  #ifndef NDEBUG
    if((naAbsi32(signedsignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("naCreateFloat", "significand out of range");
  #endif
  dbits =
      (signedsignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | (naAbsi32(signedsignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK);
  return *((float*)&dbits);
}
NA_IDEF double naCreateDoubleSubnormal(int64 signedsignificand){
  int64 dbits;
  #ifndef NDEBUG
    if(naGreaterInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("naCreateDouble", "significand out of range");
  #endif
  dbits =
      naOrInt64(
      naAndInt64(signedsignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naAndInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)&dbits);
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
