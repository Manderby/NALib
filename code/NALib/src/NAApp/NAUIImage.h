
#if defined NA_UI_IMAGE_INCLUDED || !defined NA_APP_INCLUDED
  #warning "Do not include this file directly. Use NAApp.h"
#endif
#ifndef NA_UI_IMAGE_INCLUDED
#define NA_UI_IMAGE_INCLUDED

#ifdef __cplusplus
  extern "C"{
#endif



NA_PROTOTYPE(NAColor);
NA_PROTOTYPE(NAImage);

#include "../NAVisual/Core/NAColorDefinitions.h"



typedef enum{
  NA_UIIMAGE_INTERACTION_NONE,
  NA_UIIMAGE_INTERACTION_PRESSED,
  NA_UIIMAGE_INTERACTION_HOVER,
  NA_UIIMAGE_INTERACTION_DISABLED,
} NAUIImageInteraction;

typedef enum{
  NA_UIIMAGE_SKIN_PLAIN,  // leave the image untouched
  NA_UIIMAGE_SKIN_LIGHT,  // choose a representation for a light display
  NA_UIIMAGE_SKIN_DARK,   // choose a representation for a dark display
  NA_UIIMAGE_SKIN_SYSTEM, // choose dependent on the current system settings
} NAUIImageSkin;

typedef struct NAUIImage NAUIImage;



// naCreateUIImage creates a new UIImage which stores multiple representations
// of an image depending on resolution, theme, interaction.
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
// The tintMode defines how the images are preprocessed with the system default
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
//
// The NAUIImage has reference counting built in. Use naRetain and naRelease.
NA_API NAUIImage* naCreateUIImage(
  const NAImage* baseImage,
  double baseResolution,
  NABlendMode tintMode);

// Creates a new uiImage with the primary subimage, resolution and tintMode
// of the given one. All other subimages (different resolutions, pressed,
// hover, ...) will be computed anew.
NA_API NAUIImage* naRecreateUIImage(const NAUIImage* uiImage);

// Sets a custom image for the given parameters.
NA_API void naSetUIImageSubImage(
  NAUIImage* uiImage,
  const NAImage* subImage,
  double resolution,
  NAUIImageSkin skin,
  NAUIImageInteraction interaction);

// Returns the size of the 1x representation.
NA_API NASizes naGetUIImage1xSize(const NAUIImage* uiImage);

// Returns the Skin for the current Appearance. Either returns LIGHT or DARK.
// Never returns PLAIN.
NA_API NAUIImageSkin naGetSkinForCurrentAppearance(void);

// Returns the default foreground color for the given skin. The skin PLAIN is
// not allowed but will return the color for the LIGHT skin instead.
NA_API void naFillDefaultTextColorWithSkin(NAColor* color, NAUIImageSkin skin);
NA_API void naFillDefaultLinkColorWithSkin(NAColor* color, NAUIImageSkin skin);
NA_API void naFillDefaultAccentColorWithSkin(NAColor* color, NAUIImageSkin skin);

// Creates a new NAImage with the given path. Can only be PNG on windows,
// macOS allows for various kind of input files.
NA_API NAImage* naCreateImageWithFilePath(const NAUTF8Char* pathStr);



// Working with system native images
//
// Operating systems have special types to represent an image. The following
// functions allow to convert between NAImage and the native images which
// are of the following type:
//
// macOS:   NSImage*
// Windows: HBITMAP

// Creates a new image out of a given native image.
NA_API NAImage* naCreateImageWithNativeImage(const void* nativeImage);

// Allocates a new native image with the content of the given image.
NA_API void* naAllocNativeImageWithImage(const NAImage* image);

// Deallocates the native image.
NA_API void naDeallocNativeImage(void* nativeImage);



// Inline implementations are in a separate file: 
#include "Core/NAUIImageII.h"



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
