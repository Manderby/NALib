
#ifndef NA_IMAGE_INCLUDED
#define NA_IMAGE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



#include "../NABase/NABase.h"
#include "../NAMath/NACoord.h"
#include "NAColor.h"


NA_PROTOTYPE(NAImage);

// Creates an image with the specified size and fills it with the given color.
// If color is nullptr, the image contents will be uninitialized.
NA_API NAImage* naCreateImage(NASizes size, const NAColor* color);

// Creates an image which is an exact duplicate of the image given.
NA_API NAImage* naCreateImageCopy(const NAImage* image);

// Creates a new image with blending. The mode defines, how the top will be
// applied upon the base. The blend factor defines how strong the blending is.
//
//          Base:    Top:
// Tint:    Image    Color
// Blend:   Image    Image
// Apply:   Color    Image
//
// For the Blend method, if NA_NULL is given for the base, transparent color
// is used instead.
NA_API NAImage* naCreateImageWithTint(
  const NAImage* base,
  const NAColor* tint,
  NABlendMode mode,
  float factor);
NA_DEF NAImage* naCreateImageWithBlend(
  const NAImage* base,
  const NAImage* top,
  NABlendMode mode,
  float factor,
  NAPosi32 offset);
NA_DEF NAImage* naCreateImageWithApply(
  const NAColor* ground,
  const NAImage* top,
  NABlendMode mode,
  float blend);

// Creates an image half the size. Rescales data bilinearily.
NA_API NAImage* naCreateImageWithHalfSize(const NAImage* image);

// Creates an image which is a resize of the given image.
NA_API NAImage* naCreateImageWithResize(
  const NAImage* image,
  NASizes newSize);

// Returns the image dimensions.
NA_API NASizes naGetImageSize(const NAImage* image);

// Returns the raw image data.
NA_API NAColor* naGetImageData(const NAImage* image);

// Fills the image with the given data. The data is expected to contain as
// many RGBA values stored as uint8 necessary for the whole image with no
// padding. Depending on the topToBottom flag, the data is expected as such.
// Note that an NAImage internally always stores images bottom to top.
NA_API void naFillImageWithu8(
  NAImage* image,
  const void* data,
  NABool topToBottom,
  NAColorBufferType bufferType);

// Writes the contents of the image into a buffer. The output data will be
// RGBA values stored as uint8 with no padding. The data buffer must be big
// enough. Depending on the topToBottom flag, the data is expected as such.
// Note that an NAImage internally always stores images bottom to top.
NA_API void naConvertImageTou8(
  const NAImage* image,
  void* data,
  NABool topToBottom,
  NAColorBufferType bufferType);




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



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_IMAGE_INCLUDED



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
