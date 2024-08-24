
#include "../NAColor.h"
#include "../../NAMath/NAMathOperators.h"




NA_DEF void naFillColorWithCopy(NAColor* dstColor, const NAColor* srcColor) {
  *dstColor = *srcColor;
}



NA_DEF void naFillColorWithTransparent(NAColor* color) {
  color->r = 0.f;
  color->g = 0.f;
  color->b = 0.f;
  color->alpha = 0.f;
}

NA_DEF void naFillColorWithSRGB(NAColor* color, float r, float g, float b, float alpha) {
  color->r = r;
  color->g = g;
  color->b = b;
  color->alpha = alpha;
}

NA_DEF void naFillColorWithSRGBv(NAColor* color, const float* srcColor) {
  color->r = srcColor[0];
  color->g = srcColor[1];
  color->b = srcColor[2];
  color->alpha = srcColor[3];
}

#define NA_UINT8_INVERSE (1.f / 255.f)

NA_DEF void naFillColorWithSRGBu8v(
  NAColor* outColor,
  const uint8* inColor,
  NAColorBufferType bufferType)
{
  if((bufferType != NA_COLOR_BUFFER_RGB)
    && (bufferType != NA_COLOR_BUFFER_BGR0)
    && (inColor[3] == 0.f))
  {
    naFillColorWithTransparent(outColor);
  }else{
    switch(bufferType) {
    case NA_COLOR_BUFFER_RGBA:
      naFillColorWithSRGB(
        outColor,
        (float)inColor[0] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[2] * NA_UINT8_INVERSE,
        (float)inColor[3] * NA_UINT8_INVERSE);
      break;
    case NA_COLOR_BUFFER_RGBAPre:
      {
      float inAlpha = (float)inColor[3];
      float inv = naInvf(inAlpha);
      naFillColorWithSRGB(
        outColor,
        (float)inColor[0] * inv,
        (float)inColor[1] * inv,
        (float)inColor[2] * inv,
        inAlpha * NA_UINT8_INVERSE);
      break;
      }
    case NA_COLOR_BUFFER_RGB:
      naFillColorWithSRGB(
        outColor,
        (float)inColor[0] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[2] * NA_UINT8_INVERSE,
        1.f);
      break;
    case NA_COLOR_BUFFER_BGR0:
      naFillColorWithSRGB(
        outColor,
        (float)inColor[2] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[0] * NA_UINT8_INVERSE,
        1.f);
      break;
    case NA_COLOR_BUFFER_BGRA:
      naFillColorWithSRGB(
        outColor,
        (float)inColor[2] * NA_UINT8_INVERSE,
        (float)inColor[1] * NA_UINT8_INVERSE,
        (float)inColor[0] * NA_UINT8_INVERSE,
        (float)inColor[3] * NA_UINT8_INVERSE);
      break;
    }
  }
}



NA_DEF void naFillSRGBAWithColor(float rgba[4], const NAColor* color) {
  rgba[0] = color->r;
  rgba[1] = color->g;
  rgba[2] = color->b;
  rgba[3] = color->alpha;
}

NA_HIDEF void na_LimitColorComponentu8(uint8* outvalue, const float inValue) {
  if(inValue < 0.f) {
    *outvalue = 0;
  }else if(inValue > 1.f) {
    *outvalue = 255;
  }else{
    *outvalue = (uint8)(inValue * 255);
  }
}

NA_DEF void naFillSRGBu8WithColor(uint8* outColor, const NAColor* inColor, NAColorBufferType bufferType) {
  switch(bufferType) {
  case NA_COLOR_BUFFER_RGBA:
    na_LimitColorComponentu8(&outColor[0], inColor->r);
    na_LimitColorComponentu8(&outColor[1], inColor->g);
    na_LimitColorComponentu8(&outColor[2], inColor->b);
    na_LimitColorComponentu8(&outColor[3], inColor->alpha);
    break;
  case NA_COLOR_BUFFER_RGBAPre:
    {
    float inAlpha = inColor->alpha;
    na_LimitColorComponentu8(&outColor[0], inColor->r * inAlpha);
    na_LimitColorComponentu8(&outColor[1], inColor->g * inAlpha);
    na_LimitColorComponentu8(&outColor[2], inColor->b * inAlpha);
    na_LimitColorComponentu8(&outColor[3], inAlpha);
    break;
    }
  case NA_COLOR_BUFFER_RGB:
    na_LimitColorComponentu8(&outColor[0], inColor->r);
    na_LimitColorComponentu8(&outColor[1], inColor->g);
    na_LimitColorComponentu8(&outColor[2], inColor->b);
    break;
  case NA_COLOR_BUFFER_BGR0:
    na_LimitColorComponentu8(&outColor[0], inColor->b);
    na_LimitColorComponentu8(&outColor[1], inColor->g);
    na_LimitColorComponentu8(&outColor[2], inColor->r);
    na_LimitColorComponentu8(&outColor[3], 0);
    break;
  case NA_COLOR_BUFFER_BGRA:
    na_LimitColorComponentu8(&outColor[0], inColor->b);
    na_LimitColorComponentu8(&outColor[1], inColor->g);
    na_LimitColorComponentu8(&outColor[2], inColor->r);
    na_LimitColorComponentu8(&outColor[3], inColor->alpha);
    break;
  }
}



NA_DEF float naGetColorAlpha(const NAColor* color) {
  return color->alpha;
}

NA_DEF void naInvertColor(NAColor* color) {
  color->r = 1.f - color->r;
  color->g = 1.f - color->g;
  color->b = 1.f - color->b;
}



// Including the color blend operations
#include "NAColorOperationsII.h"



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
