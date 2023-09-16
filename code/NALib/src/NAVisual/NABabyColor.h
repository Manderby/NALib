
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

#include "../NABase.h"
#include "../NAUtility/NAMemory.h"

#define NA_BABY_COLOR_CHANNEL_COUNT 4
#define NA_BABY_COLOR_BYTES_PER_COMPONENT \
  (NA_BABY_COLOR_CHANNEL_COUNT * sizeof(float))
typedef float NABabyColor[NA_BABY_COLOR_CHANNEL_COUNT];

typedef enum{
  NA_COLOR_BUFFER_RGBA,
  NA_COLOR_BUFFER_RGBAPre,
  NA_COLOR_BUFFER_RGB,
  NA_COLOR_BUFFER_BGR0,
  NA_COLOR_BUFFER_BGRA
} NAColorBufferType;

// Linearizes or unlinearizes a single float value using a baby transformation.
NA_IAPI float naLinearizeColorValue(float value);
NA_IAPI float naUnlinearizeColorValue(float value);

// Fills the given BabyColor with the given values
NA_IAPI naFillBabyColor(NABabyColor color, float r, float g, float b, float a);

// Checks whether the given color is secure
NA_API NABool naIsBabyColorSecure(const NABabyColor color);

// Inverts the color
NA_API void naInvertBabyColor(NABabyColor color);

// Converts between BabyColor and an uint8 representation.
// When premultiplied is set to NA_TRUE, the uint8 variant is assumed to be
// premultiplied with the alpha values.
NA_API void naFillu8WithBabyColor(
  uint8*            outColor,
  const NABabyColor inColor,
  NAColorBufferType bufferType);
NA_API void naFillBabyColorWithu8(
  NABabyColor       outColor,
  const uint8*      inColor,
  NAColorBufferType bufferType);


// This factor is nice and easy to remember. There is no greater mathematical
// purpose in this number. It's just nice.
#define NA_BABY_FACTOR 0.75f

NA_IDEF float naUnlinearizeColorValue(float value){
  return value / (NA_BABY_FACTOR * value + (1.f - NA_BABY_FACTOR));
}
NA_IDEF float naLinearizeColorValue(float value){
  return (1.f - NA_BABY_FACTOR) * value / (1.f - NA_BABY_FACTOR * value);
}

NA_IDEF naFillBabyColor(NABabyColor color, float r, float g, float b, float a){
  color[0] = r;
  color[1] = g;
  color[2] = b;
  color[3] = a;
}



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BABY_COLOR_INCLUDED



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
