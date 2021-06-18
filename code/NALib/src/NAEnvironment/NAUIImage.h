
#ifndef NA_UI_IMAGE_INCLUDED
#define NA_UI_IMAGE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif

// ///////////////////////////
// This file contains API which should be hidden to the programmer but which
// is shared among the different implementations of the UI in Mac and Win.
// ///////////////////////////


#include "../NABabyImage.h"

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

#define NA_UIIMAGE_SUBIMAGES_COUNT (NA_UIIMAGE_RESOLUTION_COUNT * NA_UIIMAGE_KIND_COUNT * NA_UIIMAGE_SKIN_COUNT)

typedef struct NAUIImage NAUIImage;



// Useful tint modes are
// NA_BLEND_ZERO         leaves the image as it is
// NA_BLEND_OPAQUE       tints opaque parts of the image
// NA_BLEND_BLACK_GREEN  tints black pixels (measured by the green channel)
// NA_BLEND_WHITE_GREEN  tints white pixels (measured by the green channel)
// The other tint modes are applicable but will likely produce unuseful images.
// Use naRetain and naRelease.
NA_API NAUIImage* naNewUIImage(const NABabyImage* main, const NABabyImage* alt, NAUIImageResolution resolution, NABlendMode tintMode);

NA_IAPI NASizei naGetUIImage1xSize(const NAUIImage* uiImage);

// Returns the default foreground color for the given skin.
void naFillDefaultTextColorWithSkin(NABabyColor color, NAUIImageSkin skin);
void naFillDefaultLinkColorWithSkin(NABabyColor color, NAUIImageSkin skin);

NABabyImage* naCreateBabyImageFromNativeImage(const void* nativeImage);
NABabyImage* naCreateBabyImageFromFilePath(const NAUTF8Char* pathStr);
NA_API void* naAllocNativeImageWithBabyImage(const NABabyImage* image);
NA_API void naDeallocNativeImage(void* nativeImage);



// Inline implementations are in a separate file: 
#include "NAUIImageII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_UI_IMAGE_INCLUDED



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
