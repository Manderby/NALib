
#ifndef NA_COLOR_INCLUDED
#define NA_COLOR_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

// What is NAColor?
// After having worked for years in the color reproduction industry, the
// author knows one thing: Colors are not easy. There is no such thing as
// a simple color value. When you try to convert color values or compute
// images, it always gets complicated. And if you want to be accurate,
// performance might drop considerably. Therefore, the author decided to
// use a very specific and simplified color computation.
//
// So here's what NAColor is:
// - RGBA values. A is alpha. Stored in a simple float[4] array (4 * 32 bit).
// - Default RGB range is [0, 1]. Values below 0 are forbidden.
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

#include "../NABase/NABase.h"
#include "../NAUtility/NAMemory.h"



NA_DEF void naConvertHSVToHSL(float out[3], const float in[3]);
NA_DEF void naConvertHSLToHSV(float out[3], const float in[3]);



typedef struct NAColor NAColor;
struct NAColor{
  float r;
  float g;
  float b;
  float alpha;
};

typedef enum{
  NA_COLOR_BUFFER_RGBA,
  NA_COLOR_BUFFER_RGBAPre,
  NA_COLOR_BUFFER_RGB,
  NA_COLOR_BUFFER_BGR0,
  NA_COLOR_BUFFER_BGRA
} NAColorBufferType;

// Note that BLACK_GREEN and WHITE_GREEN only work for opaque images.
typedef enum{
  NA_BLEND_ZERO,          // Does not blend at all. The base remains as it is.
  NA_BLEND_LINEAR,        // Linear interpolation according blend factor.
  NA_BLEND_OVERLAY,       // Blends where top is opaque.
  NA_BLEND_OPAQUE,        // Blends where base is opaque.
  NA_BLEND_MULTIPLY,      // Blends where base is opaque by multiplying the color.
  NA_BLEND_SCREEN,        // Blends where base is opaque by inverse multiplying the color.
  NA_BLEND_ERODE_LIGHT,   // Same as screen but makes light pixels of base transparent.
                          // Useful for icon creation.
  NA_BLEND_ERODE_DARK,    // Same as multiply but makes dark pixels of base transparent.
                          // Useful for icon creation.
  NA_BLEND_ERASE_HUE      // Decolorizes and makes base transparent based on top hue.
                          // Useful for green-screening or blue-screening.
} NABlendMode;

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
