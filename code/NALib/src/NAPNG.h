
#ifndef NA_PNG_INCLUDED
#define NA_PNG_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "NACoord.h"
#include "NABabyImage.h"


typedef struct NAPNG NAPNG;

typedef enum{
  NA_PNG_COLORTYPE_GREYSCALE        = 0,
  NA_PNG_COLORTYPE_TRUECOLOR        = 2,
  NA_PNG_COLORTYPE_INDEXEDCOLOR     = 3,
  NA_PNG_COLORTYPE_GREYSCALE_ALPHA  = 4,
  NA_PNG_COLORTYPE_TRUECOLOR_ALPHA  = 6
} NAPNGColorType;



NA_API NAPNG* naNewPNG(NASizei size, NAPNGColorType colorType, int8 bitDepth);
NA_API NAPNG* naNewPNGWithPath(const char* filePath);
NA_API NAPNG* naNewPNGWithBabyImage(NABabyImage* babyImage);

NA_API void* naGetPNGPixelData(NAPNG* png);
NA_API size_t naGetPNGPixelDataByteSize(NAPNG* png);
NA_API NABabyImage* naCreateBabyImageFromPNG(NAPNG* png);
NA_API NASizei naGetPNGSize(NAPNG* png);
NA_API NAPNGColorType naGetPNGColorType(NAPNG* png);
NA_API NAInt naGetPNGBitDepth(NAPNG* png);
NA_API size_t naGetPNGBytesPerPixel(NAPNGColorType colorType);


NA_API void naWritePNGToPath(NAPNG* png, const char* filePath);





#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_PNG_INCLUDED



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
