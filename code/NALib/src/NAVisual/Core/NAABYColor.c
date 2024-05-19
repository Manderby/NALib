
#include "../NAABYColor.h"
#include "../../NAMath/NAMathOperators.h"




NA_HIDEF NABool na_IsABYColorClearlyRadiometric(const NAABYColor* color){
  NABool allNegative = (color->a <= 0.f && color->b <= 0.f && color->y <= 0.f);
  NABool allZero = (color->a == 0.f && color->b == 0.f && color->y == 0.f);
  return allNegative && !allZero;
}



NA_DEF void naCopyABYColor(NAABYColor* dstColor, const NAABYColor* srcColor){
  *dstColor = *srcColor;
}



NA_DEF void naFillABYColorWithTransparent(NAABYColor* color){
  color->a = 0.f;
  color->b = 0.f;
  color->y = 0.f;
  color->alpha = 0.f;
}

NA_DEF void naFillABYColorWithSRGB(NAABYColor* color, float r, float g, float b, float alpha){
  color->a = r;
  color->b = b;
  color->y = g;
  color->alpha = alpha;
}

#define NA_UINT8_INVERSE (1.f / 255.f)

NA_DEF void naFillABYColorWithSRGBu8(
  NAABYColor* outColor,
  const uint8* inColor,
  NAColorBufferType bufferType)
{
  if((bufferType != NA_COLOR_BUFFER_RGB)
    && (bufferType != NA_COLOR_BUFFER_BGR0)
    && (inColor[3] == 0.f))
  {
    naFillABYColorWithTransparent(outColor);
  }else{
    switch(bufferType){
    case NA_COLOR_BUFFER_RGBA:
      naFillABYColorWithSRGB(
        outColor,
        (float)inColor[0] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[2] * NA_UINT8_INVERSE,
        (float)inColor[3] * NA_UINT8_INVERSE);
      break;
    case NA_COLOR_BUFFER_RGBAPre: {
      float inAlpha = (float)inColor[3];
      float inv = naInvf(inAlpha);
      naFillABYColorWithSRGB(
        outColor,
        (float)inColor[0] * inv,
        (float)inColor[1] * inv,
        (float)inColor[2] * inv,
        inAlpha * NA_UINT8_INVERSE);
      break; }
    case NA_COLOR_BUFFER_RGB:
      naFillABYColorWithSRGB(
        outColor,
        (float)inColor[0] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[2] * NA_UINT8_INVERSE,
        1.f);
      break;
    case NA_COLOR_BUFFER_BGR0:
      naFillABYColorWithSRGB(
        outColor,
        (float)inColor[2] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[0] * NA_UINT8_INVERSE,
        1.f);
      break;
    case NA_COLOR_BUFFER_BGRA:
      naFillABYColorWithSRGB(
        outColor,
        (float)inColor[2] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[0] * NA_UINT8_INVERSE,
        (float)inColor[3] * NA_UINT8_INVERSE);
      break;
    }
  }
}



NA_DEF void naFillSRGBAWithABYColor(float rgba[4], const NAABYColor* color){
  rgba[0] = color->a;
  rgba[1] = color->y;
  rgba[2] = color->b;
  rgba[3] = color->alpha;
}

NA_HIDEF void na_LimitColorComponentu8(uint8* outvalue, const float inValue){
  if(inValue < 0.f){
    *outvalue = 0;
  }else if(inValue > 1.f){
    *outvalue = 255;
  }else{
    *outvalue = (uint8)(inValue * 255);
  }
}

NA_DEF void naFillSRGBu8WithABYColor(uint8* outColor, const NAABYColor* inColor, NAColorBufferType bufferType){
//  #if NA_DEBUG
//    if(!naIsABYColorUseful(inColor))
//      naError("Using an ABYColor which is not useful");
//    if(na_IsABYColorClearlyRadiometric(inColor))
//      naError("Using a radiometric ABYColor");
//  #endif

  switch(bufferType){
  case NA_COLOR_BUFFER_RGBA:
    na_LimitColorComponentu8(&outColor[0], inColor->a);
    na_LimitColorComponentu8(&outColor[1], inColor->y);
    na_LimitColorComponentu8(&outColor[2], inColor->b);
    na_LimitColorComponentu8(&outColor[3], inColor->alpha);
    break;
  case NA_COLOR_BUFFER_RGBAPre: {
    float inAlpha = inColor->alpha;
    na_LimitColorComponentu8(&outColor[0], inColor->a * inAlpha);
    na_LimitColorComponentu8(&outColor[1], inColor->y * inAlpha);
    na_LimitColorComponentu8(&outColor[2], inColor->b * inAlpha);
    na_LimitColorComponentu8(&outColor[3], inAlpha);
    break; }
  case NA_COLOR_BUFFER_RGB:
    na_LimitColorComponentu8(&outColor[0], inColor->a);
    na_LimitColorComponentu8(&outColor[1], inColor->y);
    na_LimitColorComponentu8(&outColor[2], inColor->b);
    break;
  case NA_COLOR_BUFFER_BGR0:
    na_LimitColorComponentu8(&outColor[0], inColor->b);
    na_LimitColorComponentu8(&outColor[1], inColor->y);
    na_LimitColorComponentu8(&outColor[2], inColor->a);
    na_LimitColorComponentu8(&outColor[3], 0);
    break;
  case NA_COLOR_BUFFER_BGRA:
    na_LimitColorComponentu8(&outColor[0], inColor->b);
    na_LimitColorComponentu8(&outColor[1], inColor->y);
    na_LimitColorComponentu8(&outColor[2], inColor->a);
    na_LimitColorComponentu8(&outColor[3], inColor->alpha);
    break;
  }
}



NA_DEF float naGetABYColorAlpha(const NAABYColor* color){
  return color->alpha;
}

NA_DEF NABool naIsABYColorUseful(const NAABYColor* color){
  NABool allPositive = (color->a >= 0.f && color->b >= 0.f && color->y >= 0.f);
  NABool allNegative = (color->a <= 0.f && color->b <= 0.f && color->y <= 0.f);
  return allPositive || allNegative;
}

NA_DEF void naInvertABYColor(NAABYColor* color){
  #if NA_DEBUG
    if(!naIsABYColorUseful(color))
      naError("Inverting an ABYColor which is not useful");
    if(na_IsABYColorClearlyRadiometric(color))
      naError("Inverting a radiometric ABYColor");
  #endif
  color->a = 1.f - color->a;
  color->b = 1.f - color->b;
  color->y = 1.f - color->y;
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
