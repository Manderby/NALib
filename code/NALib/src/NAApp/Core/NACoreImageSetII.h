
// This file is an inline implmenentation (II) file which is included in the
// NAAppCore.c file. This is a bit special as it is marked as a .h file but
// actually contains non-inlinenable code. See NAAppCore.c for more information.
// Do not include this file anywhere else!


#include "NAAppCore.h"
#include "../../NAStruct/NAList.h"


struct NAImageSet {
  NAList subImages;
  NASizes size1x;
  NABlendMode tinting;
};

NA_API void na_DestructImageSet(NAImageSet* imageSet);
NA_RUNTIME_TYPE(NAImageSet, na_DestructImageSet, NA_TRUE);



NA_PROTOTYPE(NA_UISubImage);
struct NA_UISubImage{
  const NAImage* image;
  void* nativeImage;
  NASkin skin;
  NAImageSetInteraction interaction;
};



// Will retain the image.
NA_HAPI NA_UISubImage* na_AddImageSetSubImage(
  NAImageSet* imageSet,
  const NAImage* image,
  NASkin skin,
  NAImageSetInteraction interaction);
  
NA_HAPI void na_DeallocUISubImage(NA_UISubImage* subImage);



NA_HDEF const NA_UISubImage* na_GetUISubImageRaw(
  const NAImageSet* imageSet,
  NASizes size,
  NASkin skin,
  NAImageSetInteraction interaction)
{
  const NA_UISubImage* subImage = NA_NULL;
  NABool found = NA_FALSE;
  
  NAListIterator listIter = naMakeListAccessor(&imageSet->subImages);
  while(naIterateList(&listIter)) {
    subImage = naGetListCurConst(&listIter);
    if(naEqualSizes(naGetImageSize(subImage->image), size)
      && subImage->skin == skin
      && subImage->interaction == interaction)
    {
      found = NA_TRUE;
      break;
    }
  }
  naClearListIterator(&listIter);
  
  return found ? subImage : NA_NULL;
}



NA_HDEF const NA_UISubImage* na_GetUISubImage(
  const NAImageSet* imageSet,
  NASizes size,
  NASkin skin,
  NAImageSetInteraction interaction,
  NABool secondaryState)
{
  const NA_UISubImage* subImage = na_GetUISubImageRaw(
    imageSet,
    size,
    skin,
    interaction);
  
  if(subImage) {
    return subImage;
  }
  
  // Reaching here, we have not found the desired image.
  const NA_UISubImage* newSubImage = NA_NULL;
  NAImageSet* mutableImageSet = (NAImageSet*)imageSet; // evil cast!

  // If the sizes does not match, we build a corresponding one.
  NASizes baseSize = naGetImageSize(((NA_UISubImage*)naGetListFirstConst(&imageSet->subImages))->image);
  if(!naEqualSizes(size, baseSize)) {
    const NA_UISubImage* originalImage = na_GetUISubImage(mutableImageSet, baseSize, skin, interaction, secondaryState);
    NAImage* newImage = naCreateImageWithResize(originalImage->image, size);
    newSubImage = na_AddImageSetSubImage(
      mutableImageSet,
      newImage,
      NA_SKIN_PLAIN,
      NA_IMAGE_SET_INTERACTION_NONE);
    naRelease(newImage);

  // If the status is not NONE, we build an image out of it.
  }else if(interaction != NA_IMAGE_SET_INTERACTION_NONE) {
    switch(interaction) {
    case NA_IMAGE_SET_INTERACTION_PRESSED:
      {
        const NA_UISubImage* originalImage = na_GetUISubImage(mutableImageSet, size, skin, NA_IMAGE_SET_INTERACTION_NONE, secondaryState);
        NAColor accentColor;
        naFillColorWithSkinAccentColor(&accentColor, skin);
        NAImage* newImage = naCreateImageWithTint(
          originalImage->image,
          &accentColor,
          NA_BLEND_OPAQUE,
          (mutableImageSet->tinting == NA_BLEND_ZERO) ? .45f : 1.f);
        newSubImage = na_AddImageSetSubImage(
          mutableImageSet,
          newImage,
          skin,
          interaction);
        naRelease(newImage);
      }
      break;
    case NA_IMAGE_SET_INTERACTION_HOVER:
      {
        const NA_UISubImage* originalImage = na_GetUISubImage(mutableImageSet, size, skin, NA_IMAGE_SET_INTERACTION_NONE, secondaryState);
        
        NAColor hoverColor;
        naFillColorWithSkinAccentColor(&hoverColor, skin);
        NAImage* newImage = naCreateImageWithTint(
          originalImage->image,
          &hoverColor,
          NA_BLEND_OPAQUE,
          (mutableImageSet->tinting == NA_BLEND_ZERO) ? .15f : .5f);

        newSubImage = na_AddImageSetSubImage(
          mutableImageSet,
          newImage,
          skin,
          interaction);
        naRelease(newImage);
      }
      break;
    case NA_IMAGE_SET_INTERACTION_DISABLED:
      {
        const NA_UISubImage* originalImage = na_GetUISubImage(mutableImageSet, size, skin, NA_IMAGE_SET_INTERACTION_NONE, secondaryState);
        #if NA_OS == NA_OS_MAC_OS_X
          newSubImage = originalImage;
        #else
          NAColor transparent = {0., 0., 0., 0.};
          NAImage* newImage = naCreateImageWithApply(
            &transparent,
            originalImage->image,
            NA_BLEND_OVERLAY,
            .5f);
          newSubImage = na_AddImageSetSubImage(
            mutableImageSet,
            newImage,
            skin,
            interaction);
          naRelease(newImage);
        #endif
      }
      break;
    case NA_IMAGE_SET_INTERACTION_NONE:
      // Fallthrough. This is the default case.
      NA_FALLTHROUGH;
    default: break;
    }
  
  // If the skin is not PLAIN, we build an image out of it.
  }else{
    NAColor tintColor;
    naFillColorWithSkinTextColor(&tintColor, skin);
    if(secondaryState && naGetCurrentSkin() != NA_SKIN_DARK) {
      naInvertColor(&tintColor);
    }
    const NA_UISubImage* originalImage = na_GetUISubImage(mutableImageSet, size, NA_SKIN_PLAIN, NA_IMAGE_SET_INTERACTION_NONE, secondaryState);
    NAImage* newImage = naCreateImageWithTint(originalImage->image, &tintColor, imageSet->tinting, 1.f);
    newSubImage = na_AddImageSetSubImage(
      mutableImageSet,
      newImage,
      skin,
      NA_IMAGE_SET_INTERACTION_NONE);
    naRelease(newImage);
  }
  
  return newSubImage;
}



NA_DEF void naSetImageSetSubImage(
  NAImageSet* imageSet,
  const NAImage* subImage,
  NASkin skin,
  NAImageSetInteraction interaction)
{
  NAListIterator listIter = naMakeListModifier(&imageSet->subImages);
  while(naIterateList(&listIter)) {
    const NA_UISubImage* curSubImage = naGetListCurConst(&listIter);
    if(naEqualSizes(naGetImageSize(curSubImage->image), naGetImageSize(subImage)) && curSubImage->skin == skin && curSubImage->interaction == interaction) {
      NA_UISubImage* oldImage = naRemoveListCurMutable(&listIter, NA_FALSE);
      na_DeallocUISubImage(oldImage);
      break;
    }
  }
  naClearListIterator(&listIter);

  na_AddImageSetSubImage(
    imageSet,
    subImage,
    skin,
    interaction);
}



NA_HDEF const NAImage* naGetImageSetSubImage(const NAImageSet* imageSet, NASizes size, NASkin skin, NAImageSetInteraction interaction, NABool secondaryState) {
  // Let the following function do the hard work.
  const NA_UISubImage* subImage = na_GetUISubImage(imageSet, size, skin, interaction, secondaryState);
  return subImage->image;
}



#include "../../NAVisual/NAPNG.h"
#include "../../NAUtility/NAFile.h"

NA_HDEF void* naGetImageSetNativeSubImage(const NAImageSet* imageSet, NASizes size, NASkin skin, NAImageSetInteraction interaction, NABool secondaryState) {
  // Let the following function do the hard work.
  const NA_UISubImage* subImage = na_GetUISubImage(imageSet, size, skin, interaction, secondaryState);
  return subImage->nativeImage;
}



NA_DEF NAImageSet* naCreateImageSet(
  const NAImage* baseImage,
  NASizes size1x,
  NABlendMode tinting)
{
  NAImageSet* imageSet = naCreate(NAImageSet);
  
  naInitList(&imageSet->subImages);
  imageSet->size1x = size1x;
  imageSet->tinting = tinting;
  
  na_AddImageSetSubImage(
    imageSet,
    baseImage,
    NA_SKIN_PLAIN,
    NA_IMAGE_SET_INTERACTION_NONE);
  
  return imageSet;
}



NA_DEF NAImageSet* naRecreateImageSet(const NAImageSet* imageSet) {
  NAImageSet* newImageSet = naCreate(NAImageSet);
  
  naInitList(&newImageSet->subImages);
  newImageSet->tinting = imageSet->tinting;
  
  const NA_UISubImage* subImage = naGetListFirstConst(&imageSet->subImages);
  
  na_AddImageSetSubImage(
    newImageSet,
    subImage->image,
    NA_SKIN_PLAIN,
    NA_IMAGE_SET_INTERACTION_NONE);
  
  return newImageSet;
}



NA_DEF void na_DestructImageSet(NAImageSet* imageSet) {
  naClearList(&imageSet->subImages, (NAMutator)na_DeallocUISubImage);
}



NA_HDEF void naFillColorWithSkinTextColor(NAColor* color, NASkin skin) {
  uint8 skinColor[4];
  
  switch(skin) {
  case NA_SKIN_PLAIN:
    #if NA_DEBUG
      naError("Cannot provide color for plain skin");
    #endif

    NA_FALLTHROUGH; // Fallthrough to light.

    case NA_SKIN_LIGHT:
    skinColor[0] = 16;
    skinColor[1] = 16;
    skinColor[2] = 16;
    skinColor[3] = 255;
//    skinColor[0] = 0x0d;
//    skinColor[1] = 0x0d;
//    skinColor[2] = 0x0d;
//    skinColor[3] = 0xc0;
    break;
  case NA_SKIN_DARK:
    skinColor[0] = 240;
    skinColor[1] = 240;
    skinColor[2] = 240;
    skinColor[3] = 255;
//    skinColor[0] = 0xf4;
//    skinColor[1] = 0xf4;
//    skinColor[2] = 0xf4;
//    skinColor[3] = 0xf0;
    break;
  case NA_SKIN_SYSTEM:
    naFillColorWithDefaultTextColor(color);
    return;
  }
  
  naFillColorWithSRGBu8v(color, skinColor, NA_COLOR_BUFFER_RGBA);
}



NA_DEF void naFillColorWithSkinLinkColor(NAColor* color, NASkin skin) {
  uint8 skinColor[4];
  
  switch(skin) {
  case NA_SKIN_PLAIN:
    #if NA_DEBUG
      naError("Cannot provide color for plain skin");
    #endif

    NA_FALLTHROUGH; // Fallthrough to light.
    
  case NA_SKIN_LIGHT:
    skinColor[0] = 16;
    skinColor[1] = 128;
    skinColor[2] = 240;
    skinColor[3] = 255;
    break;
  case NA_SKIN_DARK:
    skinColor[0] = 64;
    skinColor[1] = 196;
    skinColor[2] = 240;
    skinColor[3] = 255;
    break;
  case NA_SKIN_SYSTEM:
    naFillColorWithDefaultLinkColor(color);
    return;
  }
  
  naFillColorWithSRGBu8v(color, skinColor, NA_COLOR_BUFFER_RGBA);
}



NA_DEF void naFillColorWithSkinAccentColor(NAColor* color, NASkin skin) {
  uint8 skinColor[4];
  
  switch(skin) {
  case NA_SKIN_PLAIN:
    #if NA_DEBUG
      naError("Cannot provide color for plain skin");
    #endif
    
    NA_FALLTHROUGH; // Fallthrough to light.
    
  case NA_SKIN_LIGHT:
    skinColor[0] = 255;
    skinColor[1] = 128;
    skinColor[2] = 16;
    skinColor[3] = 255;
    break;
  case NA_SKIN_DARK:
    skinColor[0] = 255;
    skinColor[1] = 128;
    skinColor[2] = 16;
    skinColor[3] = 255;
    break;
  case NA_SKIN_SYSTEM:
    naFillColorWithDefaultAccentColor(color);
    return;
  }
  
  naFillColorWithSRGBu8v(color, skinColor, NA_COLOR_BUFFER_RGBA);
}



NA_API NASizes naGetImageSetSize1x(const NAImageSet* imageSet) {
  return imageSet->size1x;
}



NA_API NABlendMode naGetImageSetTinting(const NAImageSet* imageSet) {
  return imageSet->tinting;
}



NA_HDEF NA_UISubImage* na_AddImageSetSubImage(
  NAImageSet* imageSet,
  const NAImage* image,
  NASkin skin,
  NAImageSetInteraction interaction)
{
//  const NA_UISubImage* subImage = na_GetUISubImageRaw(
//    imageSet,
//    skin,
//    interaction,
//    secondaryState);
//
  NA_UISubImage* subImage = naAlloc(NA_UISubImage);
  subImage->image = naRetainConst(image);
  subImage->nativeImage = naAllocNativeImageWithImage(image);
  subImage->skin = skin;
  subImage->interaction = interaction;
  
  naAddListLastMutable(&imageSet->subImages, subImage);
  
  return subImage;
}

NA_HDEF void na_DeallocUISubImage(NA_UISubImage* subImage) {
  naReleaseConst(subImage->image);
  naDeallocNativeImage(subImage->nativeImage);
  naFree(subImage);
}



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
