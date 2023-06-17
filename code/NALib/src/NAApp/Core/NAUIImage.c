
#include "../../NAApp.h"

#if NA_COMPILE_GUI == 1



NA_API void na_DestructUIImage(NAUIImage* uiImage);
NA_RUNTIME_TYPE(NAUIImage, na_DestructUIImage, NA_TRUE);


// Will retain the babyImage.
NA_HAPI NA_UISubImage* na_AddUISubImage(
  NAUIImage* uiImage,
  const NABabyImage* image,
  double resolution,
  NAUIImageSkin skin,
  NAUIImageInteraction interaction);
  
NA_HAPI void na_DeallocUISubImage(NA_UISubImage* subImage);

NA_HAPI double na_GetUIImageBaseResolution(const NAUIImage* uiImage);



NA_DEF void naFillDefaultTextColorWithSkin(NABabyColor color, NAUIImageSkin skin){
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



NA_DEF void naFillDefaultLinkColorWithSkin(NABabyColor color, NAUIImageSkin skin){
  uint8 skinColor[4];
  switch(skin){
  case NA_UIIMAGE_SKIN_LIGHT:
    skinColor[0] = 16;
    skinColor[1] = 128;
    skinColor[2] = 240;
    skinColor[3] = 255;
    break;
  case NA_UIIMAGE_SKIN_DARK:
    skinColor[0] = 64;
    skinColor[1] = 196;
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



//NA_HDEF const NABabyImage* na_GetUIImageBabyImage(const NAUIImage* uiImage, NAUIImageResolution resolution, NAUIImageKind kind, NAUIImageSkin skin){
//  NAInt subIndex = na_GetUIImageSubImageIndex(resolution, kind, skin);
//  const NABabyImage* retImg = uiImage->babyImages[subIndex];
//
//  if(!retImg && skin != NA_UIIMAGE_SKIN_PLAIN){
//    // If the skinned image does not exist, create one automatically out of
//    // the plain image of the same kind and resolution.
//    const NABabyImage* plainImg = na_GetUIImageBabyImage(uiImage, resolution, kind, NA_UIIMAGE_SKIN_PLAIN);
//    if(plainImg){
//      NABabyColor tintColor;
//      NABabyImage* tintedImage;
//      naFillDefaultTextColorWithSkin(tintColor, skin);
//      tintedImage = naCreateBabyImageWithTint(plainImg, tintColor, uiImage->tintMode, 1.f);
//      // todo: not so beautiful const cast to NAUIImage*.
//      na_SetUIImageBabyImage((NAUIImage*)uiImage, tintedImage, resolution, kind, skin);
//      naReleaseBabyImage(tintedImage);
//      retImg = uiImage->babyImages[subIndex];
//    }
//  }else if(!retImg && kind != NA_UIIMAGE_KIND_MAIN){
//    // If the kinded image does not exist, create one automatically out of
//    // the main image of the same skin and resolution.
//    const NABabyImage* mainImg = na_GetUIImageBabyImage(uiImage, resolution, NA_UIIMAGE_KIND_MAIN, skin);
//    if(mainImg){
//      NABabyImage* kindedImage = naCreateBabyImageCopy(mainImg);
//      // todo: not so beautiful const cast to NAUIImage*.
//      na_SetUIImageBabyImage((NAUIImage*)uiImage, kindedImage, resolution, kind, skin);
//      naReleaseBabyImage(kindedImage);
//      retImg = uiImage->babyImages[subIndex];
//    }
//    // todo: third else if should automatically create a resized image.
//  }
//  return retImg;
//}



NA_HDEF const NA_UISubImage* na_GetUISubImage(const NAUIImage* uiImage, double resolution, NAUIImageSkin skin, NAUIImageInteraction interaction){
  NAListIterator listIter = naMakeListAccessor(&uiImage->subImages);
  while(naIterateList(&listIter)){
    const NA_UISubImage* subImage = naGetListCurConst(&listIter);
    if(subImage->resolution == resolution && subImage->skin == skin && subImage->interaction == interaction){
      naClearListIterator(&listIter);
      return subImage;
    }
  }
  naClearListIterator(&listIter);
  
  // Reaching here, we have not found the desired image.
  NA_UISubImage* newSubImage = NA_NULL;
  NAUIImage* mutableUIImage = (NAUIImage*)uiImage;
  
  // If the status is not NONE, we build an image out of it.
  if(interaction != NA_UIIMAGE_INTERACTION_NONE){
    return na_GetUISubImage(mutableUIImage, resolution, skin, NA_UIIMAGE_INTERACTION_NONE);
  
  // If the skin is not PLAIN, we build an image out of it.
  }else if(skin != NA_UIIMAGE_SKIN_PLAIN){
    NABabyColor tintColor;
    naFillDefaultTextColorWithSkin(tintColor, skin);
    const NA_UISubImage* originalImage = na_GetUISubImage(mutableUIImage, resolution, NA_UIIMAGE_SKIN_PLAIN, NA_UIIMAGE_INTERACTION_NONE);
    NABabyImage* newImage = naCreateBabyImageWithTint(originalImage->image, tintColor, uiImage->tintMode, 1.f);
    newSubImage = na_AddUISubImage(mutableUIImage, newImage, resolution, skin, NA_UIIMAGE_INTERACTION_NONE);
    naReleaseBabyImage(newImage);

  // If the resolution does not match, we build a corresponding one.
  }else{
    double baseResolution = na_GetUIImageBaseResolution(mutableUIImage);
    const NA_UISubImage* originalImage = na_GetUISubImage(mutableUIImage, baseResolution, NA_UIIMAGE_SKIN_PLAIN, NA_UIIMAGE_INTERACTION_NONE);
    NASizei size = naGetBabyImageSize(originalImage->image);
    size.width = (NAInt)((double)size.width * resolution / baseResolution);
    size.height = (NAInt)((double)size.height * resolution / baseResolution);
    NABabyImage* newImage = naCreateBabyImageWithResize(originalImage->image, size);
    newSubImage = na_AddUISubImage(mutableUIImage, newImage, resolution, NA_UIIMAGE_SKIN_PLAIN, NA_UIIMAGE_INTERACTION_NONE);
    naReleaseBabyImage(newImage);
  }
  
  return newSubImage;
}



NA_HDEF const NABabyImage* na_GetUIImageBabyImage(const NAUIImage* uiImage, double resolution, NAUIImageSkin skin, NAUIImageInteraction interaction){
  // Let the following function do the hard work.
  const NA_UISubImage* subImage = na_GetUISubImage(uiImage, resolution, skin, interaction);
  return subImage->image;
}



NA_HDEF void* na_GetUIImageNativeImage(const NAUIImage* uiImage, double resolution, NAUIImageSkin skin, NAUIImageInteraction interaction){
  // Let the following function do the hard work.
  const NA_UISubImage* subImage = na_GetUISubImage(uiImage, resolution, skin, interaction);
  return subImage->nativeImage;
}



//NA_DEF NAUIImage* naCreateUIImage(const NABabyImage* main, const NABabyImage* alt, NAUIImageResolution resolution, NABlendMode tintMode){
//  NAUIImage* uiImage;
//  NABabyImage* main1x;
//  
//  #if NA_DEBUG
//    #if NA_OS == NA_OS_WINDOWS
//      if(sizeof(WORD) > 4)
//        naError("Bitamps require WORD alignment. But WORD is kind-a big on this system. Expect bad images.");
//    #endif
//    if(!main)
//      naError("There must be a main image");
//    if(alt && !naEqualSizei(naGetBabyImageSize(main), naGetBabyImageSize(alt)))
//      naError("Both images must have the same size.");
//  #endif
//  uiImage = naCreate(NAUIImage);
//  
//  uiImage->size1x = naGetBabyImageSize(main);
//  uiImage->tintMode = tintMode;
//  naZeron(uiImage->babyImages, NA_UIIMAGE_SUBIMAGES_COUNT * sizeof(NABabyImage*));
//  #if NA_OS == NA_OS_WINDOWS
//    naZeron(uiImage->nativeImages, NA_UIIMAGE_SUBIMAGES_COUNT * sizeof(HBITMAP));
//  #else
//    naZeron(uiImage->nativeImages, NA_UIIMAGE_SUBIMAGES_COUNT * sizeof(void*));
//  #endif
//  
//  switch(resolution){
//  case NA_UIIMAGE_RESOLUTION_SCREEN_1x:
//    na_SetUIImageBabyImage(uiImage, main, NA_UIIMAGE_RESOLUTION_SCREEN_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
//    if(alt){
//      na_SetUIImageBabyImage(uiImage, alt, NA_UIIMAGE_RESOLUTION_SCREEN_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
//    }
//    break;
//  case NA_UIIMAGE_RESOLUTION_SCREEN_2x:
//    #if NA_DEBUG
//      if(uiImage->size1x.width % 2 || uiImage->size1x.height % 2)
//        naError("Image size is not divisable by 2");
//    #endif
//    uiImage->size1x.width /= 2;
//    uiImage->size1x.height /= 2;
//    na_SetUIImageBabyImage(uiImage, main, NA_UIIMAGE_RESOLUTION_SCREEN_2x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
//    main1x = naCreateBabyImageWithHalfSize(main);
//    na_SetUIImageBabyImage(uiImage, main1x, NA_UIIMAGE_RESOLUTION_SCREEN_1x, NA_UIIMAGE_KIND_MAIN, NA_UIIMAGE_SKIN_PLAIN);
//    naReleaseBabyImage(main1x);
//    if(alt){
//      NABabyImage* alt1x;
//      na_SetUIImageBabyImage(uiImage, alt, NA_UIIMAGE_RESOLUTION_SCREEN_2x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
//      alt1x = naCreateBabyImageWithHalfSize(alt);
//      na_SetUIImageBabyImage(uiImage, alt1x, NA_UIIMAGE_RESOLUTION_SCREEN_1x, NA_UIIMAGE_KIND_ALT, NA_UIIMAGE_SKIN_PLAIN);
//      naReleaseBabyImage(alt1x);
//    }
//    break;
//  default:
//    #if NA_DEBUG
//      naError("Unknown resolution");
//    #endif
//    break;
//  }
//
//  naInitList(&uiImage->subImages);
//
//  return uiImage;
//}


NA_DEF NAUIImage* naCreateUIImage(
  const NABabyImage* baseImage,
  double baseResolution,
  NABlendMode tintMode)
{
  NAUIImage* uiImage = naCreate(NAUIImage);
  
  naInitList(&uiImage->subImages);
  uiImage->tintMode = tintMode;
  
  na_AddUISubImage(uiImage, baseImage, baseResolution, NA_UIIMAGE_SKIN_PLAIN, NA_UIIMAGE_INTERACTION_NONE);
  
  return uiImage;
}



NA_DEF void na_DestructUIImage(NAUIImage* uiImage){
  naForeachListMutable(&uiImage->subImages, (NAMutator)na_DeallocUISubImage);
  naClearList(&uiImage->subImages);
}



NA_HDEF double na_GetUIImageBaseResolution(const NAUIImage* uiImage){
  const NA_UISubImage* subImage = naGetListFirstConst(&uiImage->subImages);
  return subImage->resolution;
}



NA_API NASizei naGetUIImage1xSize(const NAUIImage* uiImage){
  const NA_UISubImage* subImage = naGetListFirstConst(&uiImage->subImages);
  NASizei size = naGetBabyImageSize(subImage->image);
  double factor = subImage->resolution / NA_UIIMAGE_RESOLUTION_SCREEN_1x;
  size.width = naFloor(size.width / factor);
  size.height = naFloor(size.height / factor);
  return size;
}



NA_HDEF NA_UISubImage* na_AddUISubImage(
  NAUIImage* uiImage,
  const NABabyImage* image,
  double resolution,
  NAUIImageSkin skin,
  NAUIImageInteraction interaction)
{
  NA_UISubImage* subImage = naAlloc(NA_UISubImage);
  subImage->image = naRetainBabyImage(image);
  subImage->nativeImage = naAllocNativeImageWithBabyImage(image);
  subImage->resolution = resolution;
  subImage->skin = skin;
  subImage->interaction = interaction;
  
  naAddListLastMutable(&uiImage->subImages, subImage);
  
  return subImage;
}

NA_HDEF void na_DeallocUISubImage(NA_UISubImage* subImage){
  naReleaseBabyImage(subImage->image);
  naFree(subImage);
}

#endif // NA_COMPILE_GUI == 1


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
