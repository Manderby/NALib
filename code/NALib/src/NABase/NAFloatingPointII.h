
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of floating point calculations.
// Do not include this file directly! It will automatically be included when
// including "NABase.h"





NA_IDEF float naMakeFloat(int32 signedsignificand, int32 signedexponent){
  int32 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("exponent too low for single precision");
    if((signedsignificand != 0) && (signedexponent == NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL))
      naError("exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent too high for single precision");
    if(signedexponent == NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
    if((naAbsi32(signedsignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  dbits = (int32)(
      ((uint32)signedsignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | (((uint32)signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BITS)
    | ((uint32)naAbsi32(signedsignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK));
  return *((float*)(void*)&dbits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDouble(NAInt64 signedsignificand, int32 signedexponent){
  NAInt64 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("exponent too low for double precision");
    if(!naEqualInt64(signedsignificand, NA_ZERO_64) && (signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL))
      naError("exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent too high for double precision");
    if(signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
    if(naGreaterInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  dbits =
      naOrInt64(naOrInt64(
      naAndInt64(signedsignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naShlInt64(naMakeInt64WithLo(signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS)),
      naAndInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)(void*)&dbits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF float naMakeFloatWithExponent(int32 signedexponent){
  int32 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("exponent too low for single precision");
    if(signedexponent == NA_IEEE754_SINGLE_EXPONENT_SUBNORMAL)
      naError("exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent too high for single precision");
    if(signedexponent == NA_IEEE754_SINGLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
  #endif
  dbits = ((signedexponent + NA_IEEE754_SINGLE_EXPONENT_BIAS) << NA_IEEE754_SINGLE_SIGNIFICAND_BITS);
  return *((float*)(void*)&dbits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDoubleWithExponent(int32 signedexponent){
  NAInt64 dbits;
  #ifndef NDEBUG
    if(signedexponent < NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naCrash("exponent too low for double precision");
    if(signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL)
      naError("exponent creates subnormal number");
    if(signedexponent > NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent too high for double precision");
    if(signedexponent == NA_IEEE754_DOUBLE_EXPONENT_SPECIAL)
      naError("exponent equals max exponent which is reserved for special values");
  #endif
  dbits = naShlInt64(naMakeInt64WithLo(signedexponent + NA_IEEE754_DOUBLE_EXPONENT_BIAS), NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  return *((double*)(void*)&dbits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF float naMakeFloatSubnormal(int32 signedsignificand){
  int32 dbits;
  #ifndef NDEBUG
    if((naAbsi32(signedsignificand) > NA_IEEE754_SINGLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  dbits = (int32)(
      ((uint32)signedsignificand & NA_IEEE754_SINGLE_SIGN_MASK)
    | ((uint32)naAbsi32(signedsignificand) & NA_IEEE754_SINGLE_SIGNIFICAND_MASK));
  return *((float*)(void*)&dbits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IDEF double naMakeDoubleSubnormal(NAInt64 signedsignificand){
  NAInt64 dbits;
  #ifndef NDEBUG
    if(naGreaterInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK))
      naError("significand out of range");
  #endif
  dbits =
      naOrInt64(
      naAndInt64(signedsignificand, NA_IEEE754_DOUBLE_SIGN_MASK),
      naAndInt64(naAbsi64(signedsignificand), NA_IEEE754_DOUBLE_SIGNIFICAND_MASK));
  return *((double*)(void*)&dbits);
  // Note that the additional void* cast is necessary for static code analizers.
}



NA_IAPI int32 naGetDoubleExponent(double d){
  NAInt64 dbits = *((NAInt64*)(void*)&d);
  dbits = naAndInt64(dbits, NA_IEEE754_DOUBLE_EXPONENT_MASK);
  dbits = naShrInt64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_BITS);
  return naCastInt64ToInt32(dbits) - NA_IEEE754_DOUBLE_EXPONENT_BIAS;
}



NA_IAPI NAInt64 naGetDoubleInteger(double d){
  int32 exponent;
  NAInt64 dbits = *((NAInt64*)(void*)&d);
  dbits = naAndInt64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
  dbits = naOrInt64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
  exponent = naGetDoubleExponent(d);
  if(exponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL){
    dbits = NA_ZERO_64;
  }else{
    dbits = naShrInt64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_BITS - exponent);
    if(d < 0){dbits = naNegInt64(dbits);}
  }
  return dbits;
}


NA_IAPI NAInt64 naGetDoubleFraction(double d){
  int32 exponent;
  NAInt64 dbits = *((NAInt64*)(void*)&d);
  dbits = naAndInt64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
  dbits = naOrInt64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_NORM);
  exponent = naGetDoubleExponent(d);
  if(exponent == NA_IEEE754_DOUBLE_EXPONENT_SUBNORMAL){
    dbits = NA_ZERO_64;
  }else{
    if(exponent < 0){
      dbits = naShrInt64(dbits, -exponent);
    }else{
      dbits = naShlInt64(dbits, exponent);
    }
    dbits = naAndInt64(dbits, NA_IEEE754_DOUBLE_SIGNIFICAND_MASK);
    NAInt128 hyperBits = naMakeInt128(NA_ZERO_64, dbits);
    NAInt128 hyperTens = naMakeInt128(NA_ZERO_64, 0x71afd498d0000000);  // = 1e15 * 2^13
    hyperBits = naMulInt128(hyperBits, hyperTens);
    if(exponent < 0){
      dbits = naShlInt64(hyperBits.hi, -exponent);
    }else{
      dbits = naShrInt64(hyperBits.hi, exponent);
    }
    dbits = naIncInt64(dbits);
    dbits = naShrInt64(dbits, 1);
    if(exponent < 0){
      dbits = naShrInt64(dbits, -exponent);
    }else{
      dbits = naShlInt64(dbits, exponent);
    }
  }
  return dbits;
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
