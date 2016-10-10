
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_PNG_INCLUDED
#define NA_PNG_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NACoord.h"


typedef struct NAPNG NAPNG;

typedef enum{
  NA_PNG_COLORTYPE_GREYSCALE        = 0,
  NA_PNG_COLORTYPE_TRUECOLOR        = 2,
  NA_PNG_COLORTYPE_INDEXEDCOLOR     = 3,
  NA_PNG_COLORTYPE_GREYSCALE_ALPHA  = 4,
  NA_PNG_COLORTYPE_TRUECOLOR_ALPHA  = 6,
} NAPNGColorType;



NA_API NAPNG* naNewPNG(NASizei size, NAPNGColorType colortype, NAUInt bitdepth);
NA_API NAPNG* naNewPNGWithFile(const char* filename);

NA_API void* naGetPNGPixelData(NAPNG* png);
NA_API NAUInt naGetPNGPixelDataSize(NAPNG* png);
NA_API NASizei naGetPNGSize(NAPNG* png);
NA_API NAPNGColorType naGetPNGColorType(NAPNG* png);
NA_API NAUInt naGetPNGBitDepth(NAPNG* png);
NA_API NAUInt naGetPNGBytesPerPixel(NAPNGColorType colortype);


NA_API void naWritePNGToFile(NAPNG* png, const char* filename);





#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_PNG_INCLUDED



// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
