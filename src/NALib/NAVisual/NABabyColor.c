
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


#include "../NABabyColor.h"
//#include "../NAMathOperators.h"



//NA_IDEF float naUnlinearizeColorValue(float value){
//  return value / (.7f * value + .3f);
//}
//NA_IDEF float naLinearizeColorValue(float value){
//  return .3f * value / (1.f - .7f * value);
//}

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

// This is an reference implementation using a simplified gamma of 2.2
// But as it is using the pow function, it is considerably slower than the
// implementation above.
//
//#define NA_BABY_GAMMA 2.2f
//
//NA_HIDEF void naUnlinearizeRGB(float* outcolor, const float* incolor){
//  float invgamma = 1.f / NA_BABY_GAMMA;
//  outcolor[0] = naPowf(incolor[0], invgamma);
//  outcolor[1] = naPowf(incolor[1], invgamma);
//  outcolor[2] = naPowf(incolor[2], invgamma);
//}
//
//NA_HIDEF void naLinearizeRGB(float* outcolor, const float* incolor){
//  outcolor[0] = naPowf(incolor[0], NA_BABY_GAMMA);
//  outcolor[1] = naPowf(incolor[1], NA_BABY_GAMMA);
//  outcolor[2] = naPowf(incolor[2], NA_BABY_GAMMA);
//}



NA_DEF void naFillUInt8WithBabyColor(uint8* outcolor, const float* incolor, NABool premultiplied){
  float tmpcolor[4];
  naUnlinearizeRGB(tmpcolor, incolor);
  tmpcolor[3] = incolor[3];
  if(premultiplied){
    tmpcolor[0] *= incolor[3];
    tmpcolor[1] *= incolor[3];
    tmpcolor[2] *= incolor[3];
  }
  if(tmpcolor[0] < 0.f){outcolor[0] = 0;}else if(tmpcolor[0] > 1.f){outcolor[0] = 255;}else{outcolor[0] = (uint8)(tmpcolor[0] * 255);}
  if(tmpcolor[1] < 0.f){outcolor[1] = 0;}else if(tmpcolor[1] > 1.f){outcolor[1] = 255;}else{outcolor[1] = (uint8)(tmpcolor[1] * 255);}
  if(tmpcolor[2] < 0.f){outcolor[2] = 0;}else if(tmpcolor[2] > 1.f){outcolor[2] = 255;}else{outcolor[2] = (uint8)(tmpcolor[2] * 255);}
  if(tmpcolor[3] < 0.f){outcolor[3] = 0;}else if(tmpcolor[3] > 1.f){outcolor[3] = 255;}else{outcolor[3] = (uint8)(tmpcolor[3] * 255);}
}



NA_DEF void naFillBabyColorWithUInt8(float* outcolor, const uint8* incolor, NABool premultiplied){
  float tmpcolor[4];
  if(!incolor[3]){
    outcolor[0] = 0.f;
    outcolor[1] = 0.f;
    outcolor[2] = 0.f;
    outcolor[3] = 0.f;
  }else{
    float inv = 1.f / 255.f;
    outcolor[3] = (float)incolor[3] * inv;
    if(premultiplied){
      inv /= outcolor[3];
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
