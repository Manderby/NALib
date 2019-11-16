
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_BABY_COLOR_INCLUDED
#define NA_BABY_COLOR_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

// What is NABabyColor?
// After having worked for years in the color reproduction industry, the
// author knows one thing: Colors are not easy. There is no such thing as
// a simple color value. When you try to convert color values or compute
// images, it always gets complicated. And if you want to be accurate,
// performance might drop considerably. Therefore, the author decided to
// use a very specific and simplified color computation. And with the term
// "simple" came the name "baby". So simple, a baby could program it.
// Basically, the author just needed a short, recognizable name which is not
// generic.
//
// Could have used NALinSRGBPowSimpleGammaFloatColor instead. Like that better?
//
// So here's what NABabyColor is:
// - RGBA values. A is alpha. Stored in a simple float[4] array (4 * 32 bit).
// - Default RGB range is [0, 1). Values below 0 are forbidden.
// - RGB have primaries of sRGB.
// - RGB are stored without gamma, meaning it's linear, meaning it's in
//   densitometric space.
// - Simplified conversion from and to perceyved space uses the following
//   formulas:   y = .25x / (1 - .75x)   and   y = x / (.75x + .25)
//   Both formulas try to anneal the Lab L* response curve standard, hence are
//   visually pleasing but also have no numerical problems around 0 and 1.
//   And they are very fast to compute.
// - Alpha is 0 = Transparent, 1 = opaque. Linear interpolation factor.
// - Alpha is NOT premultiplied. 
// - Simplified conversion from/to uint8 is done with multiplication of 255.
//   Clamps values below 0 and above 1 to 0 and 255 respectively. 

#include "NABase.h"
#include "NAMemory.h"

#define NA_BABY_COLOR_CHANNEL_COUNT 4
#define NA_BABY_COLOR_BYTES_PER_COMPONENT (NA_BABY_COLOR_CHANNEL_COUNT * naSizeof(float))
typedef float NABabyColorValueType;

typedef NABabyColorValueType NABabyColor[NA_BABY_COLOR_CHANNEL_COUNT];

NA_IAPI float naUnlinearizeColorValue(float value);
NA_IAPI float naLinearizeColorValue(float value);

// When premultiplied is set to NA_TRUE, the uint8 variant is assumed to be
// premultiplied with the alpha values.
NA_API void naFillUInt8WithBabyColor(uint8* outcolor, const float* incolor, NABool premultiplied);
NA_API void naFillBabyColorWithUInt8(float* outcolor, const uint8* incolor, NABool premultiplied);



// This factor is nice and easy to remember.
#define NA_BABY_FACTOR 0.75f

NA_IDEF float naUnlinearizeColorValue(float value){
  return value / (NA_BABY_FACTOR * value + (1.f - NA_BABY_FACTOR));
}
NA_IDEF float naLinearizeColorValue(float value){
  return (1.f - NA_BABY_FACTOR) * value / (1.f - NA_BABY_FACTOR * value);
}



//NA_IDEF float naUnlinearizeColorValue(float value){
//  return value;
//}
//NA_IDEF float naLinearizeColorValue(float value){
//  return value;
//}



#define NA_BABY_GAMMA 2.2f
#include "NAMathOperators.h"

//NA_IDEF float naUnlinearizeColorValue(float value){
//  float invgamma = 1.f / NA_BABY_GAMMA;
//  return naPowf(value, invgamma);
//}
//NA_IDEF float naLinearizeColorValue(float value){
//  return naPowf(value, NA_BABY_GAMMA);
//}


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BABY_COLOR_INCLUDED

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
