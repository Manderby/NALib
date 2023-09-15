
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



NA_HDEF const NA_UISubImage* na_GetUISubImage(
  const NAUIImage* uiImage,
  double resolution,
  NAUIImageSkin skin,
  NAUIImageInteraction interaction,
  NABool secondaryState)
{

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
  const NA_UISubImage* newSubImage = NA_NULL;
  NAUIImage* mutableUIImage = (NAUIImage*)uiImage;

  // If the status is not NONE, we build an image out of it.
  if(interaction != NA_UIIMAGE_INTERACTION_NONE){
    switch(interaction){
    case NA_UIIMAGE_INTERACTION_PRESSED:
      {
        const NA_UISubImage* originalImage = na_GetUISubImage(mutableUIImage, resolution, skin, NA_UIIMAGE_INTERACTION_NONE, secondaryState);
        NABabyColor accentColor;
        naFillDefaultAccentColorWithSkin(accentColor, skin);
        NABabyImage* newImage = naCreateBabyImageWithTint(
          originalImage->image,
          accentColor,
          NA_BLEND_OPAQUE,
          (mutableUIImage->tintMode == NA_BLEND_ZERO) ? .45f : 1.f);
        newSubImage = na_AddUISubImage(
          mutableUIImage,
          newImage,
          resolution,
          skin,
          interaction);
        naReleaseBabyImage(newImage);
      }
      break;
    case NA_UIIMAGE_INTERACTION_HOVER:
      {
        const NA_UISubImage* originalImage = na_GetUISubImage(mutableUIImage, resolution, skin, NA_UIIMAGE_INTERACTION_NONE, secondaryState);
        
        NABabyColor hoverColor;
        naFillDefaultAccentColorWithSkin(hoverColor, skin);
        NABabyImage* newImage = naCreateBabyImageWithTint(
          originalImage->image,
          hoverColor,
          NA_BLEND_OPAQUE,
          (mutableUIImage->tintMode == NA_BLEND_ZERO) ? .15f : .5f);

        newSubImage = na_AddUISubImage(
          mutableUIImage,
          newImage,
          resolution,
          skin,
          interaction);
        naReleaseBabyImage(newImage);
      }
      break;
    case NA_UIIMAGE_INTERACTION_DISABLED:
      {
        const NA_UISubImage* originalImage = na_GetUISubImage(mutableUIImage, resolution, skin, NA_UIIMAGE_INTERACTION_NONE, secondaryState);
        #if NA_OS == NA_OS_MAC_OS_X
          newSubImage = originalImage;
        #else
          NABabyImage* newImage = naCreateBabyImageWithBlend(
            NA_NULL,
            originalImage->image,
            NA_BLEND_OVERLAY,
            .5f,
            naMakePosi(0, 0));
          newSubImage = na_AddUISubImage(
            mutableUIImage,
            newImage,
            resolution,
            skin,
            interaction);
          naReleaseBabyImage(newImage);
        #endif
      }
      break;
    default: break;
    }
  
  // If the skin is not PLAIN, we build an image out of it.
  }else if(skin != NA_UIIMAGE_SKIN_PLAIN){
    NABabyColor tintColor;
    naFillDefaultTextColorWithSkin(tintColor, skin);
    if(secondaryState && naGetSkinForCurrentAppearance() != NA_UIIMAGE_SKIN_DARK){
      naInvertBabyColor(tintColor);
    }
    const NA_UISubImage* originalImage = na_GetUISubImage(mutableUIImage, resolution, NA_UIIMAGE_SKIN_PLAIN, NA_UIIMAGE_INTERACTION_NONE, secondaryState);
    NABabyImage* newImage = naCreateBabyImageWithTint(originalImage->image, tintColor, uiImage->tintMode, 1.f);
    newSubImage = na_AddUISubImage(
      mutableUIImage,
      newImage,
      resolution,
      skin,
      NA_UIIMAGE_INTERACTION_NONE);
    naReleaseBabyImage(newImage);

  // If the resolution does not match, we build a corresponding one.
  }else{
    double baseResolution = na_GetUIImageBaseResolution(mutableUIImage);
    const NA_UISubImage* originalImage = na_GetUISubImage(mutableUIImage, baseResolution, NA_UIIMAGE_SKIN_PLAIN, NA_UIIMAGE_INTERACTION_NONE, secondaryState);
    NASizei size = naGetBabyImageSize(originalImage->image);
    size.width = (NAInt)((double)size.width * resolution / baseResolution);
    size.height = (NAInt)((double)size.height * resolution / baseResolution);
    NABabyImage* newImage = naCreateBabyImageWithResize(originalImage->image, size);
    newSubImage = na_AddUISubImage(
      mutableUIImage,
      newImage,
      resolution,
      NA_UIIMAGE_SKIN_PLAIN,
      NA_UIIMAGE_INTERACTION_NONE);
    naReleaseBabyImage(newImage);
  }
  
  return newSubImage;
}



NA_HDEF const NABabyImage* na_GetUIImageBabyImage(const NAUIImage* uiImage, double resolution, NAUIImageSkin skin, NAUIImageInteraction interaction, NABool secondaryState){
  // Let the following function do the hard work.
  const NA_UISubImage* subImage = na_GetUISubImage(uiImage, resolution, skin, interaction, secondaryState);
  return subImage->image;
}



NA_HDEF void* na_GetUIImageNativeImage(const NAUIImage* uiImage, double resolution, NAUIImageSkin skin, NAUIImageInteraction interaction, NABool secondaryState){
  // Let the following function do the hard work.
  const NA_UISubImage* subImage = na_GetUISubImage(uiImage, resolution, skin, interaction, secondaryState);
  return subImage->nativeImage;
}



NA_DEF NAUIImage* naCreateUIImage(
  const NABabyImage* baseImage,
  double baseResolution,
  NABlendMode tintMode)
{
  NAUIImage* uiImage = naCreate(NAUIImage);
  
  naInitList(&uiImage->subImages);
  uiImage->tintMode = tintMode;
  
  na_AddUISubImage(
    uiImage,
    baseImage,
    baseResolution,
    NA_UIIMAGE_SKIN_PLAIN,
    NA_UIIMAGE_INTERACTION_NONE);
  
  return uiImage;
}



NA_DEF NAUIImage* naRecreateUIImage(const NAUIImage* uiImage){
  NAUIImage* newUIImage = naCreate(NAUIImage);
  
  naInitList(&newUIImage->subImages);
  newUIImage->tintMode = uiImage->tintMode;
  
  const NA_UISubImage* subImage = naGetListFirstConst(&uiImage->subImages);
  
  na_AddUISubImage(
    newUIImage,
    naRetainBabyImage(subImage->image),
    subImage->resolution,
    NA_UIIMAGE_SKIN_PLAIN,
    NA_UIIMAGE_INTERACTION_NONE);
  
  return newUIImage;
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
  size.width = (NAInt)naFloor(size.width / factor);
  size.height = (NAInt)naFloor(size.height / factor);
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
