
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#include "NAUIImage.h"


// Will retain the babyimage.
NA_HIAPI void naSetUIImageBabyImage(NAUIImage* uiimage, NABabyImage* babyimage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin);



void naFillBabyColorWithSkin(NABabyColor color, NAUIImageSkin skin){
  uint8 skincolor[4];
  switch(skin){
  case NA_UIIMAGE_SKIN_LIGHT:
    skincolor[0] = 16;
    skincolor[1] = 16;
    skincolor[2] = 16;
    skincolor[3] = 255;
    break;
  case NA_UIIMAGE_SKIN_DARK:
    skincolor[0] = 240;
    skincolor[1] = 240;
    skincolor[2] = 240;
    skincolor[3] = 255;
    break;
  default:
    #ifndef NDEBUG
      naError("Cannot provide color for plain skin");
    #endif
    break;
  }
  naFillBabyColorWithUInt8(color, skincolor, NA_FALSE);
}



NA_HIDEF NAInt naGetUIImageSubImageIndex(NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  return (resolution * NA_UIIMAGE_KIND_COUNT + kind) * NA_UIIMAGE_SKIN_COUNT + skin;
}

NA_HDEF NABabyImage* naGetUIImageBabyImage(NAUIImage* uiimage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  NAInt subIndex = naGetUIImageSubImageIndex(resolution, kind, skin);
  NABabyImage* retimg = uiimage->babyimages[subIndex];
  if(!retimg && skin != NA_UIIMAGE_SKIN_PLAIN){
    NAInt plainIndex = naGetUIImageSubImageIndex(resolution, kind, NA_UIIMAGE_SKIN_PLAIN);
    NABabyImage* plainimg = uiimage->babyimages[plainIndex];
    if(plainimg){
      NABabyColor skincolor;
      naFillBabyColorWithSkin(skincolor, skin);
      NABabyImage* skinnedImage = naCreateBabyImageWithTint(plainimg, skincolor, uiimage->tintMode, 1.f);
      naSetUIImageBabyImage(uiimage, skinnedImage, resolution, kind, skin);
      naReleaseBabyImage(skinnedImage);
      retimg = uiimage->babyimages[subIndex];
    }
  }
  return retimg;
}


NA_HDEF void* naGetUIImageNativeImage(NAUIImage* uiimage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  // Let the following function do the hard work.
  naGetUIImageBabyImage(uiimage, resolution, kind, skin);
  // Now, we are sure that, if ever possible, nativeimages will contain the desired image.
  NAInt subIndex = naGetUIImageSubImageIndex(resolution, kind, skin);
  return uiimage->nativeimages[subIndex];
}



NA_HIDEF void naSetUIImageBabyImage(NAUIImage* uiimage, NABabyImage* babyimage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  NAInt subIndex = naGetUIImageSubImageIndex(resolution, kind, skin);
  uiimage->babyimages[subIndex] = naRetainBabyImage(babyimage);
  uiimage->nativeimages[subIndex] = naAllocNativeImageWithBabyImage(babyimage);
}



NA_DEF NAUIImage* naAllocUIImage(NABabyImage* main, NABabyImage* alt, NAUIImageResolution resolution, NABlendMode tintMode){
  #ifndef NDEBUG
    #if NA_OS == NA_OS_WINDOWS
      if(sizeof(WORD) > 4)
        naError("Bitamps require WORD alignment. But WORD is kind-a big on this system. Expect bad images.");
    #endif
    if(!main)
      naError("There must be a main image");
    if(alt && !naEqualSizei(naGetBabyImageSize(main), naGetBabyImageSize(alt)))
      naError("Both images must have the same size.");
  #endif
  NAUIImage* uiImage = naAlloc(NAUIImage);
  
  uiImage->size1x = naGetBabyImageSize(main);
  uiImage->tintMode = tintMode;
  naZeron(uiImage->babyimages, NA_UIIMAGE_SUBIMAGES_COUNT * naSizeof(NABabyImage*));
  #if NA_OS == NA_OS_WINDOWS
    naZeron(uiImage->nativeimages, NA_UIIMAGE_SUBIMAGES_COUNT * naSizeof(HBITMAP));
  #else
    naZeron(uiImage->nativeimages, NA_UIIMAGE_SUBIMAGES_COUNT * naSizeof(void*));
  #endif
  
  switch(resolution){
  case NA_UIIMAGE_RESOLUTION_1x:
    naSetUIImageBabyImage(uiImage, main, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    if(alt){
      naSetUIImageBabyImage(uiImage, alt, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
    }
    break;
  case NA_UIIMAGE_RESOLUTION_2x:
    #ifndef NDEBUG
      if(uiImage->size1x.width % 2 || uiImage->size1x.height % 2)
        naError("Image size is not divisable by 2");
    #endif
    uiImage->size1x.width /= 2;
    uiImage->size1x.height /= 2;
    naSetUIImageBabyImage(uiImage, main, NA_UIIMAGE_RESOLUTION_2x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    NABabyImage* main1x = naCreateBabyImageWithHalfSize(main);
    naSetUIImageBabyImage(uiImage, main1x, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    naReleaseBabyImage(main1x);
    if(alt){
      naSetUIImageBabyImage(uiImage, alt, NA_UIIMAGE_RESOLUTION_2x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
      NABabyImage* alt1x = naCreateBabyImageWithHalfSize(alt);
      naSetUIImageBabyImage(uiImage, alt1x, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
      naReleaseBabyImage(alt1x);
    }
    break;
  default:
    #ifndef NDEBUG
      naError("Unknown resolution");
    #endif
    break;
  }

  return uiImage;
}



NA_API void naDeallocUIImage(NAUIImage* uiimage){
  for(NAInt i = 0; i < NA_UIIMAGE_SUBIMAGES_COUNT; i++){
    if(uiimage->nativeimages[i]){naDeallocNativeImage(uiimage->nativeimages[i]);}
    if(uiimage->nativeimages[i]){naReleaseBabyImage(uiimage->babyimages[i]);}
  }
  naFree(uiimage);
}

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
// in all copies or substantial portions of the source-code inherently
// dependent on this software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
