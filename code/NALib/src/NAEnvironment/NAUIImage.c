
#include "NAUIImage.h"

#if (NA_COMPILE_GUI == 1)



NA_API void na_DeallocUIImage(NAUIImage* uiImage);
NA_RUNTIME_TYPE(NAUIImage, na_DeallocUIImage, NA_TRUE);


// Will retain the babyImage.
NA_HIAPI void na_SetUIImageBabyImage(NAUIImage* uiImage, const NABabyImage* babyImage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin);



void naFillBabyColorWithSkin(NABabyColor color, NAUIImageSkin skin){
  uint8 skinColor[4];
  switch(skin){
  case NA_UIIMAGE_SKIN_LIGHT:
    skinColor[0] = 16;
    skinColor[1] = 16;
    skinColor[2] = 16;
    skinColor[3] = 255;
    break;
  case NA_UIIMAGE_SKIN_DARK:
    skinColor[0] = 240;
    skinColor[1] = 240;
    skinColor[2] = 240;
    skinColor[3] = 255;
    break;
  default:
    #if NA_DEBUG
      naError("Cannot provide color for plain skin");
    #endif
    break;
  }
  naFillBabyColorWithu8(color, skinColor, NA_COLOR_BUFFER_RGBA);
}



NA_HIDEF NAInt na_GetUIImageSubImageIndex(NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  return ((NAInt)resolution * (NAInt)NA_UIIMAGE_KIND_COUNT + (NAInt)kind) * (NAInt)NA_UIIMAGE_SKIN_COUNT + (NAInt)skin;
}

NA_HDEF const NABabyImage* na_GetUIImageBabyImage(const NAUIImage* uiImage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  NAInt subIndex = na_GetUIImageSubImageIndex(resolution, kind, skin);
  const NABabyImage* retimg = uiImage->babyImages[subIndex];
  if(!retimg && skin != NA_UIIMAGE_SKIN_PLAIN){
    NAInt plainIndex = na_GetUIImageSubImageIndex(resolution, kind, NA_UIIMAGE_SKIN_PLAIN);
    const NABabyImage* plainimg = uiImage->babyImages[plainIndex];
    if(plainimg){
      NABabyColor skinColor;
      NABabyImage* skinnedImage;
      naFillBabyColorWithSkin(skinColor, skin);
      skinnedImage = naCreateBabyImageWithTint(plainimg, skinColor, uiImage->tintMode, 1.f);
      // todo: not so beautiful const cast to NAUIImage*.
      na_SetUIImageBabyImage((NAUIImage*)uiImage, skinnedImage, resolution, kind, skin);
      naReleaseBabyImage(skinnedImage);
      retimg = uiImage->babyImages[subIndex];
    }
  }
  return retimg;
}


NA_HDEF void* na_GetUIImageNativeImage(const NAUIImage* uiImage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  NAInt subIndex;
  // Let the following function do the hard work.
  na_GetUIImageBabyImage(uiImage, resolution, kind, skin);
  // Now, we are sure that, if ever possible, nativeImages will contain the desired image.
  subIndex = na_GetUIImageSubImageIndex(resolution, kind, skin);
  return uiImage->nativeImages[subIndex];
}



NA_HIDEF void na_SetUIImageBabyImage(NAUIImage* uiImage, const NABabyImage* babyImage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
  NAInt subIndex = na_GetUIImageSubImageIndex(resolution, kind, skin);
  uiImage->babyImages[subIndex] = naRetainBabyImage(babyImage);
  uiImage->nativeImages[subIndex] = naAllocNativeImageWithBabyImage(babyImage);
}



NA_DEF NAUIImage* naNewUIImage(const NABabyImage* main, NABabyImage* alt, NAUIImageResolution resolution, NABlendMode tintMode){
  NAUIImage* uiImage;
  NABabyImage* main1x;
  
  #if NA_DEBUG
    #if NA_OS == NA_OS_WINDOWS
      if(sizeof(WORD) > 4)
        naError("Bitamps require WORD alignment. But WORD is kind-a big on this system. Expect bad images.");
    #endif
    if(!main)
      naError("There must be a main image");
    if(alt && !naEqualSizei(naGetBabyImageSize(main), naGetBabyImageSize(alt)))
      naError("Both images must have the same size.");
  #endif
  uiImage = naNew(NAUIImage);
  
  uiImage->size1x = naGetBabyImageSize(main);
  uiImage->tintMode = tintMode;
  naZeron(uiImage->babyImages, NA_UIIMAGE_SUBIMAGES_COUNT * sizeof(NABabyImage*));
  #if NA_OS == NA_OS_WINDOWS
    naZeron(uiImage->nativeImages, NA_UIIMAGE_SUBIMAGES_COUNT * sizeof(HBITMAP));
  #else
    naZeron(uiImage->nativeImages, NA_UIIMAGE_SUBIMAGES_COUNT * sizeof(void*));
  #endif
  
  switch(resolution){
  case NA_UIIMAGE_RESOLUTION_1x:
    na_SetUIImageBabyImage(uiImage, main, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    if(alt){
      na_SetUIImageBabyImage(uiImage, alt, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
    }
    break;
  case NA_UIIMAGE_RESOLUTION_2x:
    #if NA_DEBUG
      if(uiImage->size1x.width % 2 || uiImage->size1x.height % 2)
        naError("Image size is not divisable by 2");
    #endif
    uiImage->size1x.width /= 2;
    uiImage->size1x.height /= 2;
    na_SetUIImageBabyImage(uiImage, main, NA_UIIMAGE_RESOLUTION_2x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    main1x = naCreateBabyImageWithHalfSize(main);
    na_SetUIImageBabyImage(uiImage, main1x, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
    naReleaseBabyImage(main1x);
    if(alt){
      NABabyImage* alt1x;
      na_SetUIImageBabyImage(uiImage, alt, NA_UIIMAGE_RESOLUTION_2x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
      alt1x = naCreateBabyImageWithHalfSize(alt);
      na_SetUIImageBabyImage(uiImage, alt1x, NA_UIIMAGE_RESOLUTION_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
      naReleaseBabyImage(alt1x);
    }
    break;
  default:
    #if NA_DEBUG
      naError("Unknown resolution");
    #endif
    break;
  }

  return uiImage;
}



NA_API void na_DeallocUIImage(NAUIImage* uiImage){
  NAInt i;
  for(i = 0; i < NA_UIIMAGE_SUBIMAGES_COUNT; i++){
    if(uiImage->nativeImages[i]){naDeallocNativeImage(uiImage->nativeImages[i]);}
    if(uiImage->babyImages[i]){naReleaseBabyImage(uiImage->babyImages[i]);}
  }
}


#endif // (NA_COMPILE_GUI == 1)



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
