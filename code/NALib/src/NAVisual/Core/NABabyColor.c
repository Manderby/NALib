
#include "../NABabyColor.h"
#include "../../NAMath/NAMathOperators.h"


NA_HIDEF void na_UnlinearizeRGB(float* outColor, const float* inColor){
  outColor[0] = naUnlinearizeColorValue(inColor[0]);
  outColor[1] = naUnlinearizeColorValue(inColor[1]);
  outColor[2] = naUnlinearizeColorValue(inColor[2]);
}

NA_HIDEF void na_LinearizeRGB(float* outColor, const float* inColor){
  outColor[0] = naLinearizeColorValue(inColor[0]);
  outColor[1] = naLinearizeColorValue(inColor[1]);
  outColor[2] = naLinearizeColorValue(inColor[2]);
}



NA_HIDEF void na_LimitColorComponentu8(uint8* outvalue, const float invalue){
  if(invalue < 0.f){
    *outvalue = 0;
  }else if(invalue > 1.f){
    *outvalue = 255;
  }else{
    *outvalue = (uint8)(invalue * 255);
  }
}

NA_DEF void naFillu8WithBabyColor(uint8* outColor, const NABabyColor inColor, NAColorBufferType bufferType){
  float tmpcolor[4];
  na_UnlinearizeRGB(tmpcolor, inColor);

  switch(bufferType){
  case NA_COLOR_BUFFER_RGBA:
    tmpcolor[3] = inColor[3];
    na_LimitColorComponentu8(&outColor[0], tmpcolor[0]);
    na_LimitColorComponentu8(&outColor[1], tmpcolor[1]);
    na_LimitColorComponentu8(&outColor[2], tmpcolor[2]);
    na_LimitColorComponentu8(&outColor[3], tmpcolor[3]);
    break;
  case NA_COLOR_BUFFER_RGBAPre:
    tmpcolor[3] = inColor[3];
    tmpcolor[0] *= inColor[3];
    tmpcolor[1] *= inColor[3];
    tmpcolor[2] *= inColor[3];
    na_LimitColorComponentu8(&outColor[0], tmpcolor[0]);
    na_LimitColorComponentu8(&outColor[1], tmpcolor[1]);
    na_LimitColorComponentu8(&outColor[2], tmpcolor[2]);
    na_LimitColorComponentu8(&outColor[3], tmpcolor[3]);
    break;
  case NA_COLOR_BUFFER_RGB:
    na_LimitColorComponentu8(&outColor[0], tmpcolor[0]);
    na_LimitColorComponentu8(&outColor[1], tmpcolor[1]);
    na_LimitColorComponentu8(&outColor[2], tmpcolor[2]);
    break;
  case NA_COLOR_BUFFER_BGR0:
    na_LimitColorComponentu8(&outColor[0], tmpcolor[2]);
    na_LimitColorComponentu8(&outColor[1], tmpcolor[1]);
    na_LimitColorComponentu8(&outColor[2], tmpcolor[0]);
    na_LimitColorComponentu8(&outColor[3], 0);
  case NA_COLOR_BUFFER_BGRA:
    tmpcolor[3] = inColor[3];
    na_LimitColorComponentu8(&outColor[0], tmpcolor[2]);
    na_LimitColorComponentu8(&outColor[1], tmpcolor[1]);
    na_LimitColorComponentu8(&outColor[2], tmpcolor[0]);
    na_LimitColorComponentu8(&outColor[3], tmpcolor[3]);
    break;
  }
}



NA_DEF void naFillBabyColorWithu8(NABabyColor outColor, const uint8* inColor, NAColorBufferType bufferType){
  float tmpcolor[4];
  float inv = 1.f / 255.f;

  if(!inColor[3] && (bufferType != NA_COLOR_BUFFER_RGB) && bufferType != NA_COLOR_BUFFER_BGR0){
    outColor[0] = 0.f;
    outColor[1] = 0.f;
    outColor[2] = 0.f;
    outColor[3] = 0.f;
  }else{
    switch(bufferType){
    case NA_COLOR_BUFFER_RGBA:
      outColor[3] = (float)inColor[3] * inv;
      break;
    case NA_COLOR_BUFFER_RGBAPre:
      outColor[3] = (float)inColor[3] * inv;
      inv = naInvf((float)inColor[3]);
      break;
    case NA_COLOR_BUFFER_RGB:
      break;
    case NA_COLOR_BUFFER_BGR0:
      outColor[3] = 1.f;
      break;
    case NA_COLOR_BUFFER_BGRA:
      outColor[3] = (float)inColor[3] * inv;
      break;
    }
    tmpcolor[0] = (float)inColor[0] * inv;
    tmpcolor[1] = (float)inColor[1] * inv;
    tmpcolor[2] = (float)inColor[2] * inv;
    na_LinearizeRGB(outColor, tmpcolor);
  }
}



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
