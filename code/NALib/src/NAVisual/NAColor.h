
#ifndef NA_COLOR_INCLUDED
#define NA_COLOR_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// An NAColor is a simple 4 float value struct containing the three primary colors
// red, green and blue plus an alpha. All values are considered valid in the
// range [0, 1]. Alpha denotes the opacity of a value, meaning 0 is transparent.
// The rgb values are considered to be from the sRGB colorspace.



#include "../NABase/NABase.h"
#include "../NAUtility/NAMemory.h"

#include "Core/NAColorDefinitions.h"



NA_DEF void naConvertHSVToHSL(float out[3], const float in[3]);
NA_DEF void naConvertHSLToHSV(float out[3], const float in[3]);



typedef struct NAColor NAColor;
struct NAColor{
  float r;
  float g;
  float b;
  float alpha;
};

NA_API void naFillColorWithCopy(NAColor* dstColor, const NAColor* srcColor);

NA_API void naFillColorWithTransparent(NAColor* color);
NA_API void naFillColorWithSRGB(NAColor* color, float r, float g, float b, float alpha);
NA_API void naFillColorWithSRGBv(NAColor* color, const float* srcColor);
NA_API void naFillColorWithSRGBu8v(NAColor* outColor, const uint8* inColor, NAColorBufferType bufferType);

NA_API void naFillSRGBAWithColor(float rgba[4], const NAColor* color);
// Converts between NAColor and an uint8 representation.
// When premultiplied is set to NA_TRUE, the uint8 variant is assumed to be
// premultiplied with the alpha values.
NA_API void naFillSRGBu8WithColor(
  uint8* outColor,
  const NAColor* inColor,
  NAColorBufferType bufferType);

NA_API void naBlendColors(
  NAColor* dstPtr,
  const NAColor* basePtr,
  const NAColor* topPtr,
  float factor,
  NABlendMode mode,
  size_t count,
  NABool baseIsImage,
  NABool topIsImage);

NA_API float naGetColorAlpha(const NAColor* color);

// Inverts the color
NA_API void naInvertColor(NAColor* color);



// Inline implementations are in a separate file:
#include "Core/NAColorII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_COLOR_INCLUDED



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
