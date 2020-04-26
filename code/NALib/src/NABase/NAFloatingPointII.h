
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of floating point calculations.
// Do not include this file directly! It will automatically be included when
// including "NABase.h"





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
    if((naAbsi32(signedSignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  int32 dBits = (int32)(
      ((uint32)signedSignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | (((uint32)signedExponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
    | ((uint32)naAbsi32(signedSignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK));
  return *((float*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDouble(NAInt64 signedSignificand, int32 signedExponent){
  #ifndef NDEBUG
    if(signedExponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("exponent too low for double precision");
    if(!naEqualInt64(signedSignificand, NA_ZERO_64) && (signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL))
      naError("exponent creates subnormal number");
    if(signedExponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent too high for double precision");
    if(signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
    if(naGreaterInt64(naAbsi64(signedSignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
    NAInt64 dBits =
      naOrInt64(naOrInt64(
      naAndInt64(signedSignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naShlInt64(naMakeInt64WithLo(signedExponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS)),
      naAndInt64(naAbsi64(signedSignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
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



NA_IDEF double naMakeDoubleWithExponent(int32 signedExponent){
  #ifndef NDEBUG
    if(signedExponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naCrash("exponent too low for double precision");
    if(signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("exponent creates subnormal number");
    if(signedExponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent too high for double precision");
    if(signedExponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
  #endif
  NAInt64 dBits = naShlInt64(naMakeInt64WithLo(signedExponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  return *((double*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF float naMakeFloatSubnormal(int32 signedSignificand){
  #ifndef NDEBUG
    if((naAbsi32(signedSignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  int32 dBits = (int32)(
      ((uint32)signedSignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | ((uint32)naAbsi32(signedSignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK));
  return *((float*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDoubleSubnormal(NAInt64 signedSignificand){
  #ifndef NDEBUG
    if(naGreaterInt64(naAbsi64(signedSignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  NAInt64 dBits =
      naOrInt64(
      naAndInt64(signedSignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naAndInt64(naAbsi64(signedSignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)(void*)&dBits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IAPI int32 naGetDoubleExponent(double d){
  NAInt64 dBits = *((NAInt64*)(void*)&d);
  dBits = naAndInt64(dBits, NA_IEEE754_DOUBLE_EXPONENT_MASK);
  dBits = naShrInt64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  return naCastInt64ToInt32(dBits) - NA_IEEE754_DOUBLE_EXPONENT_BIAS;
}



NA_IAPI NAInt64 naGetDoubleInteger(double d){
  NAInt64 dBits = *((NAInt64*)(void*)&d);
  dBits = naAndInt64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
  dBits = naOrInt64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
  int32 exponent = naGetDoubleExponent(d);
  if(exponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL){
    dBits = NA_ZERO_64;
  }else{
    dBits = naShrInt64(dBits, NA_IEEE754_DOUBLE_SIGNIFICAND_BITS - exponent);
    if(d < 0){dBits = naNegInt64(dBits);}
  }
  return dBits;
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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
