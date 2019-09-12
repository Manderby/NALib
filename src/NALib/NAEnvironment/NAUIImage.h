
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_UI_IMAGE_INCLUDED
#define NA_UI_IMAGE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// ///////////////////////////
// This file contains API which should be hidden to the programmer but which
// is shared among the different implementations of the UI in Mac and Win.
// ///////////////////////////


#include "NABabyImage.h"

typedef enum{
  NA_UIIMAGE_RESOLUTION_1x,
  NA_UIIMAGE_RESOLUTION_2x,
  NA_UIIMAGE_RESOLUTION_COUNT
} NAUIImageResolution;

typedef enum{
  NA_UIIMAGE_KIND_MAIN,
  NA_UIIMAGE_KIND_ALT,
  NA_UIIMAGE_KIND_COUNT
} NAUIImageKind;

typedef enum{
  NA_UIIMAGE_SKIN_PLAIN,
  NA_UIIMAGE_SKIN_LIGHT,
  NA_UIIMAGE_SKIN_DARK,
  NA_UIIMAGE_SKIN_COUNT
} NAUIImageSkin;

typedef struct NAUIImage NAUIImage;



// Useful tint modes are
// NA_BLEND_ZERO         leaves the image as it is
// NA_BLEND_OPAQUE       tints opaque parts of the image
// NA_BLEND_BLACK_GREEN  tints black pixels (measures by the green channel)
// NA_BLEND_WHITE_GREEN  tints white pixels (measures by the green channel)
// The other tint modes are applicable but will likely produce unuseful images.
NA_API NAUIImage* naAllocUIImage(NABabyImage* main, NABabyImage* alt, NAUIImageResolution resolution, NABlendMode tintMode);

// Deallocates the uiimage
NA_API void naDeallocUIImage(NAUIImage* uiimage);

NA_API void* naAllocNativeImageWithUIImage(NAUIImage* uiimage, NAUIImageKind kind, NAUIImageSkin skin);

NA_IAPI NASizei naGetUIImage1xSize(const NAUIImage* image);

void naFillBabyColorWithSkin(NABabyColor color, NAUIImageSkin skin);

#if NA_OS == NA_OS_MAC_OS_X
  NABabyImage* naAllocBabyImageFromImageRef(const void* imageref);
  NABabyImage* naAllocBabyImageFromFilePath(const NAUTF8Char* pathStr);
#endif



// Inline implementations are in a separate file: 
#include "NAUIImageII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_IMAGE_INCLUDED



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


