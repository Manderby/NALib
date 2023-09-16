
#ifndef NA_BABY_IMAGE_INCLUDED
#define NA_BABY_IMAGE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// What is a NABabyImage?
// The name refers to the kind of color being stored in this very simple
// image container. An NABabyImage contains only the very basic values of
// an image storage, namely with, height and the data values.
//
// The data values are always of type NABabyColor and are arranged in a
// simple float array from left to right and then from bottom to top.
// Note that this is different from most image formats which stor the pixels
// from top to bottom.
//
// When serializing, A header is written with the ASCII characters "BABY1000".
// The 1 denotes the version number, the three 0 are reserved for the future.
// After that, binary data is stored, all in big endian format. First, the
// width and height are stored as int32 values. Both with and height must be
// greater than 0 and must not exceed 2^31-1. After that, the data follows
// immediately without padding. This means regarding to the beginning of the
// file, data starts at byte number 16. The color values are stored plainly
// as IEEE-754 single precision floating point values. Any float values with
// highest or lowest exponent are forbidden. There are never any padding bytes
// at the end of a horizontal line, meaning, all bytes are packed. Also at the
// end of the data, there are no padding bytes. For each color value, if the
// alpha channel is zero, all other channels must be zero as well.


#include "../NABase.h"
#include "../NAMath/NACoord.h"
#include "NABabyColor.h"


typedef struct NABabyImage NABabyImage;

// Note that BLACK_GREEN and WHITE_GREEN only work for opaque images.
typedef enum{
  NA_BLEND_ZERO,        // Does not blend at all. The base remains as it is.
  NA_BLEND,             // Linear interpolation according blend factor.
  NA_BLEND_OVERLAY,     // Blends where top is opaque.
  NA_BLEND_OPAQUE,      // Blends where base is opaque.
  NA_BLEND_BLACK_GREEN, // Blends where base has opaque dark pixels measured on green channel.
  NA_BLEND_WHITE_GREEN, // Blends where base has opaque light pixels measured on green channel.
  NA_BLEND_MULTIPLY,    // Blends where base is opaque by multiplying the color.
  NA_BLEND_ERASE_HUE    // Decolorizes the base and makes it transparent based on top hue.
                        // Useful for green-screening or blue-screening.
} NABlendMode;

// Creates an image with the specified size and fills it with the given color.
// If color is Null, the image contents will be uninitialized.
NA_API NABabyImage* naCreateBabyImage(NASizei size, const NABabyColor color);

// Creates an image which is an exact duplicate of the image given.
NA_API NABabyImage* naCreateBabyImageCopy(const NABabyImage* image);

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
NA_API NABabyImage* naCreateBabyImageWithTint(
  const NABabyImage* base,
  const NABabyColor  tint,
  NABlendMode        mode,
  float              blend);
NA_DEF NABabyImage* naCreateBabyImageWithBlend(
  const NABabyImage* base,
  const NABabyImage* top,
  NABlendMode        mode,
  float              blend,
  NAPosi             offset);
NA_DEF NABabyImage* naCreateBabyImageWithApply(
  const NABabyColor  ground,
  const NABabyImage* top,
  NABlendMode        mode,
  float              blend);

// Creates an image half the size. Rescales data bilinearily.
NA_API NABabyImage* naCreateBabyImageWithHalfSize(const NABabyImage* image);

// Creates an image which is a resize of the given image.
NA_API NABabyImage* naCreateBabyImageWithResize(
  const NABabyImage* image,
  NASizei newSize);

// Retains and Releases an image.
NA_API NABabyImage* naRetainBabyImage(const NABabyImage* image);
NA_API void naReleaseBabyImage(const NABabyImage* image);

// Returns the number of float values per horizontal line.
NA_API NAInt naGetBabyImageValuesPerLine(const NABabyImage* image);

// Returns the image dimensions.
NA_API NASizei naGetBabyImageSize(const NABabyImage* image);

// Returns the raw image data.
NA_API float* naGetBabyImageData(const NABabyImage* image);

// Fills the image with the given data. The data is expected to contain as
// many RGBA values stored as uint8 necessary for the whole image with no
// padding. Depending on the topToBottom flag, the data is expected as such.
// Note that a BabyImage internally always stores images bottom to top.
NA_API void naFillBabyImageWithu8(
  NABabyImage*      image,
  const void*       data,
  NABool            topToBottom,
  NAColorBufferType bufferType);

// Writes the contents of the image into a buffer. The output data will be
// RGBA values stored as uint8 with no padding. The data buffer must be big
// enough. Depending on the topToBottom flag, the data is expected as such.
// Note that a BabyImage internally always stores images bottom to top.
NA_API void naConvertBabyImageTou8(
  const NABabyImage* image,
  void*              data,
  NABool             topToBottom,
  NAColorBufferType  bufferType);


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BABY_IMAGE_INCLUDED



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
