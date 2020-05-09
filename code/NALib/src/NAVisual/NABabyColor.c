
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "../NABabyColor.h"
#include "../NAMathOperators.h"


NA_HIDEF void naUnlinearizeRGB(float* outcolor, const float* incolor){
  outcolor[0] = naUnlinearizeColorValue(incolor[0]);
  outcolor[1] = naUnlinearizeColorValue(incolor[1]);
  outcolor[2] = naUnlinearizeColorValue(incolor[2]);
}

NA_HIDEF void naLinearizeRGB(float* outcolor, const float* incolor){
  outcolor[0] = naLinearizeColorValue(incolor[0]);
  outcolor[1] = naLinearizeColorValue(incolor[1]);
  outcolor[2] = naLinearizeColorValue(incolor[2]);
}



NA_HIDEF void naLimitColorComponentu8(uint8* outvalue, const float invalue){
  if(invalue < 0.f){
    *outvalue = 0;
  }else if(invalue > 1.f){
    *outvalue = 255;
  }else{
    *outvalue = (uint8)(invalue * 255);
  }
}

NA_DEF void naFillu8WithBabyColor(uint8* outcolor, const NABabyColor incolor, NAColorBufferType bufferType){
  float tmpcolor[4];
  naUnlinearizeRGB(tmpcolor, incolor);

  switch(bufferType){
  case NA_COLOR_BUFFER_RGBA:
    tmpcolor[3] = incolor[3];
    naLimitColorComponentu8(&outcolor[0], tmpcolor[0]);
    naLimitColorComponentu8(&outcolor[1], tmpcolor[1]);
    naLimitColorComponentu8(&outcolor[2], tmpcolor[2]);
    naLimitColorComponentu8(&outcolor[3], tmpcolor[3]);
    break;
  case NA_COLOR_BUFFER_RGBAPre:
    tmpcolor[3] = incolor[3];
    tmpcolor[0] *= incolor[3];
    tmpcolor[1] *= incolor[3];
    tmpcolor[2] *= incolor[3];
    naLimitColorComponentu8(&outcolor[0], tmpcolor[0]);
    naLimitColorComponentu8(&outcolor[1], tmpcolor[1]);
    naLimitColorComponentu8(&outcolor[2], tmpcolor[2]);
    naLimitColorComponentu8(&outcolor[3], tmpcolor[3]);
    break;
  case NA_COLOR_BUFFER_RGB:
    naLimitColorComponentu8(&outcolor[0], tmpcolor[0]);
    naLimitColorComponentu8(&outcolor[1], tmpcolor[1]);
    naLimitColorComponentu8(&outcolor[2], tmpcolor[2]);
    break;
  case NA_COLOR_BUFFER_BGR0:
    naLimitColorComponentu8(&outcolor[0], tmpcolor[2]);
    naLimitColorComponentu8(&outcolor[1], tmpcolor[1]);
    naLimitColorComponentu8(&outcolor[2], tmpcolor[0]);
    naLimitColorComponentu8(&outcolor[3], 0);
  case NA_COLOR_BUFFER_BGRA:
    tmpcolor[3] = incolor[3];
    naLimitColorComponentu8(&outcolor[0], tmpcolor[2]);
    naLimitColorComponentu8(&outcolor[1], tmpcolor[1]);
    naLimitColorComponentu8(&outcolor[2], tmpcolor[0]);
    naLimitColorComponentu8(&outcolor[3], tmpcolor[3]);
    break;
  }
}



NA_DEF void naFillBabyColorWithu8(NABabyColor outcolor, const uint8* incolor, NAColorBufferType bufferType){
  float tmpcolor[4];
  float inv = 1.f / 255.f;

  if(!incolor[3] && (bufferType != NA_COLOR_BUFFER_RGB) && bufferType != NA_COLOR_BUFFER_BGR0){
    outcolor[0] = 0.f;
    outcolor[1] = 0.f;
    outcolor[2] = 0.f;
    outcolor[3] = 0.f;
  }else{
    switch(bufferType){
    case NA_COLOR_BUFFER_RGBA:
      outcolor[3] = (float)incolor[3] * inv;
      break;
    case NA_COLOR_BUFFER_RGBAPre:
      outcolor[3] = (float)incolor[3] * inv;
      inv = naInvf((float)incolor[3]);
      break;
    case NA_COLOR_BUFFER_RGB:
      break;
    case NA_COLOR_BUFFER_BGR0:
      outcolor[3] = 1.f;
      break;
    case NA_COLOR_BUFFER_BGRA:
      outcolor[3] = (float)incolor[3] * inv;
      break;
    }
    tmpcolor[0] = (float)incolor[0] * inv;
    tmpcolor[1] = (float)incolor[1] * inv;
    tmpcolor[2] = (float)incolor[2] * inv;
    naLinearizeRGB(outcolor, tmpcolor);
  }
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
