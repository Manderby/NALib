
#ifndef NA_UI_IMAGE_INCLUDED
#define NA_UI_IMAGE_INCLUDED

#ifdef __cplusplus
  extern "C"{
#endif



// ///////////////////////////////
// An NAUIImage stores a collection of images ready to be presented to a user.
// Such an image might be based on a single image but can have different
// representations based on the systems skin, the resolution of the screen
// or the interaction the user has with the image.

typedef struct NAUIImage NAUIImage;

// An NAUIImage has reference counting built-in. Use naRetain and naRelease.
// ///////////////////////////////

    

NA_PROTOTYPE(NAColor);
NA_PROTOTYPE(NAImage);
#include "../NAVisual/Core/NAColorDefinitions.h"

// Interaction status a user can have with an image
typedef enum{
  NA_UIIMAGE_INTERACTION_NONE,
  NA_UIIMAGE_INTERACTION_PRESSED,
  NA_UIIMAGE_INTERACTION_HOVER,
  NA_UIIMAGE_INTERACTION_DISABLED,
} NAUIImageInteraction;



// naCreateUIImage creates a new UIImage which stores multiple representations
// of an image depending on resolution, skin, interaction.
//
// You always provide the images in the highest resolution available. If you
// have for example a 512x512 point image representing the double resolution
// image of a 256x256 icon, you provide that 512x512 image with the resolution
// NA_UI_RESOLUTION_2x.
//
// Downsampling will be done automatically by NALib in case you do not provide
// a subimage yourself using naSetUIImageSubImage.
//
// Note that a UIImage is here to solve the issue of different resolutions,
// not the issue of different representations depending on the visual size
// of an image. Meaning: If for example an icon usually is 256x256 points in
// size but you display it as a 16x16 sized icon, you might want to have a
// different representation with less detail. In such a case, you create a
// separate UIImage for that representation, not use the same UIImage.
//
// The tinting defines how the images are preprocessed with the system default
// control color which is dependent on the current color scheme of the system.
// For example, in dark mode, the images get tinted with a white color.
//
// Useful tint modes are
// NA_BLEND_ZERO         leaves the image as it is. All colors are preserved.
// NA_BLEND_OPAQUE       tints opaque parts of the image
// NA_BLEND_BLACK_GREEN  tints black pixels (measured by the green channel)
// NA_BLEND_WHITE_GREEN  tints white pixels (measured by the green channel)
// Other NABlendMode values are applicable but will likely produce less useful
// images.
NA_API NAUIImage* naCreateUIImage(
  const NAImage* baseImage,
  double baseResolution,
  NABlendMode tinting);

// Creates a new uiImage with the primary subimage, resolution and tinting
// of the given one. All other subimages (different resolutions, pressed,
// hover, ...) will be computed anew.
NA_API NAUIImage* naRecreateUIImage(const NAUIImage* uiImage);

// Returns the size of the 1x representation.
NA_API NASizes naGetUIImage1xSize(const NAUIImage* uiImage);

// Sets a custom image for the given parameters.
NA_API void naSetUIImageSubImage(
  NAUIImage* uiImage,
  const NAImage* subImage,
  double resolution,
  NASkin skin,
  NAUIImageInteraction interaction);



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
