
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NABinaryOperators.h
// Do not include this file directly! It will automatically be included when
// including "NABinaryOperators.h"



NA_IDEF uint8 naGetSignum8(int8 i){
  return (uint8)(i >> (NA_VALUE8_BIT_COUNT  - 1));
}
NA_IDEF uint16 naGetSignum16(int16 i){
  return (uint16)(i >> (NA_VALUE16_BIT_COUNT - 1));
}
NA_IDEF uint32 naGetSignum32(int32 i){
  return (uint32)(i >> (NA_VALUE32_BIT_COUNT - 1));
}
NA_IDEF uint64 naGetSignum64(int64 i){
  return naCastInt64ToUInt64(naShrInt64(i, (NA_VALUE64_BIT_COUNT - 1)));
}



NA_IDEF void naSetSignBit8(void* i){
  *((uint8*)i) |= NA_VALUE8_SIGN_MASK;
}
NA_IDEF void naSetSignBit16(void* i){
  *((uint16*)i) |= NA_VALUE16_SIGN_MASK;
}
NA_IDEF void naSetSignBit32(void* i){
  *((uint32*)i) |= NA_VALUE32_SIGN_MASK;
}
NA_IDEF void naSetSignBit64(void* i){
  *((uint64*)i) = naOrUInt64(*((uint64*)i), NA_VALUE64_SIGN_MASK);
}
NA_IDEF void naUnsetSignBit8(void* i){
  *((uint8*)i) &= ~NA_VALUE8_SIGN_MASK;
}
NA_IDEF void naUnsetSignBit16(void* i){
  *((uint16*)i) &= ~NA_VALUE16_SIGN_MASK;
}
NA_IDEF void naUnsetSignBit32(void* i){
  *((uint32*)i) &= ~NA_VALUE32_SIGN_MASK;
}
NA_IDEF void naUnsetSignBit64(void* i){
  *((uint64*)i) = naAndUInt64(*((uint64*)i), naNotUInt64(NA_VALUE64_SIGN_MASK));
}



// Prototypes
NA_IDEF int32 naAbsi32(int32 x);
NA_IDEF int64 naAbsi64(int64 x);

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
    | ((signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BIT_COUNT)
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
      naShlInt64(naMakeInt64WithLo(signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BIT_COUNT)),
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
  dbits = ((signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BIT_COUNT);
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
  dbits = naShlInt64(naMakeInt64WithLo(signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BIT_COUNT);
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
