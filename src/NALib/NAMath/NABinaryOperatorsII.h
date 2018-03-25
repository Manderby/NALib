
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABinaryOperators.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryOperators.h"




NA_IDEF value8 naGetSignum8(value8 i){
  return i >> (NA_VALUE8_BIT_COUNT  - 1);
}
NA_IDEF value16 naGetSignum16(value16 i){
  return i >> (NA_VALUE16_BIT_COUNT  - 1);
}
NA_IDEF value32 naGetSignum32(value32 i){
  return i >> (NA_VALUE32_BIT_COUNT  - 1);
}
NA_IDEF value64 naGetSignum64(value64 i){
  return i >> (NA_VALUE64_BIT_COUNT  - 1);
}



NA_IDEF value8 naSetSignBit8(value8 i){
  return (i | NA_VALUE8_SIGN_MASK);
}
NA_IDEF value16 naSetSignBit16(value16 i){
  return (i | NA_VALUE16_SIGN_MASK);
}
NA_IDEF value32 naSetSignBit32(value32 i){
  return (i | NA_VALUE32_SIGN_MASK);
}
NA_IDEF value64 naSetSignBit64(value64 i){
  return (i | NA_VALUE64_SIGN_MASK);
}
NA_IDEF value8 naUnsetSignBit8(value8 i){
  return (i & ~NA_VALUE8_SIGN_MASK);
}
NA_IDEF value16 naUnsetSignBit16(value16 i){
  return (i & ~NA_VALUE16_SIGN_MASK);
}
NA_IDEF value32 naUnsetSignBit32(value32 i){
  return (i & ~NA_VALUE32_SIGN_MASK);
}
NA_IDEF value64 naUnsetSignBit64(value64 i){
  return (i & ~NA_VALUE64_SIGN_MASK);
}



// Prototypes
NA_IDEF int32 naAbsi32(int32 x);
NA_IDEF int64 naAbsi64(int64 x);

NA_IDEF float naCreateFloat(value32 signedsignificand, value32 signedexponent){
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
  value32 dbits =
      (signedsignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | ((signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BIT_COUNT)
    | (naAbsi32(signedsignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK);
  return *((float*)&dbits);
}

NA_IDEF double naCreateDouble(value64 signedsignificand, value64 signedexponent){
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("naCreateDouble", "exponent too low for double precision");
    if((signedsignificand != 0) && (signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL))
      naError("naCreateDouble", "exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("naCreateDouble", "exponent too high for double precision");
    if(signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("naCreateDouble", "exponent equals max exponent which is reserved for special values");
    if((naAbsi64(signedsignificand) > NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("naCreateDouble", "significand out of range");
  #endif
  value64 dbits =
      (signedsignificand & NA_IEEE754_DOUBLE_SIGN_MASK)
    | ((signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS) << NA_IEEE754_DOUBLE_SIGNIFICAND_BIT_COUNT)
    | (naAbsi64(signedsignificand) & NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
  return *((double*)&dbits);
}

NA_IDEF float naCreateFloatWithExponent(value32 signedexponent){
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
  value32 dbits = ((signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BIT_COUNT);
  return *((float*)&dbits);
}

NA_IDEF double naCreateDoubleWithExponent(value64 signedexponent){
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
  value64 dbits = ((signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS) << NA_IEEE754_DOUBLE_SIGNIFICAND_BIT_COUNT);
  return *((double*)&dbits);
}

NA_IDEF float  naCreateFloatSubnormal (value32 signedsignificand){
  #ifndef NDEBUG
    if((naAbsi32(signedsignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("naCreateFloat", "significand out of range");
  #endif
  value32 dbits =
      (signedsignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | (naAbsi32(signedsignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK);
  return *((float*)&dbits);
}
NA_IDEF double naCreateDoubleSubnormal(value64 signedsignificand){
  #ifndef NDEBUG
    if((naAbsi64(signedsignificand) > NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("naCreateDouble", "significand out of range");
  #endif
  value64 dbits =
      (signedsignificand & NA_IEEE754_DOUBLE_SIGN_MASK)
    | (naAbsi64(signedsignificand) & NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
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
