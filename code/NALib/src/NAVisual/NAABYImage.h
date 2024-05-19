
#ifndef NA_ABY_IMAGE_INCLUDED
#define NA_ABY_IMAGE_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif



// What is a NAABYImage?
// The name refers to the kind of color being stored in this very simple
// image container. An NAABYImage contains only the very basic values of
// an image storage, namely with, height and the data values.
//
// The data values are always of type NAABYColor and are arranged in a
// simple float array from left to right and then from bottom to top.
// Note that this is different from most image formats which stor the pixels
// from top to bottom.
//
// When serializing, A header is written with the ASCII characters "ABY10000".
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


#include "../NABase/NABase.h"
#include "../NAMath/NACoord.h"
#include "NAABYColor.h"


typedef struct NAABYImage NAABYImage;

// Creates an image with the specified size and fills it with the given color.
// If color is Null, the image contents will be uninitialized.
NA_API NAABYImage* naCreateBabyImage(NASizei size, const NAABYColor* color);

// Creates an image which is an exact duplicate of the image given.
NA_API NAABYImage* naCreateBabyImageCopy(const NAABYImage* image);

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
NA_API NAABYImage* naCreateBabyImageWithTint(
  const NAABYImage* base,
  const NAABYColor*  tint,
  NABlendMode        mode,
  float              blend);
NA_DEF NAABYImage* naCreateBabyImageWithBlend(
  const NAABYImage* base,
  const NAABYImage* top,
  NABlendMode        mode,
  float              blend,
  NAPosi             offset);
NA_DEF NAABYImage* naCreateBabyImageWithApply(
  const NAABYColor*  ground,
  const NAABYImage* top,
  NABlendMode        mode,
  float              blend);

// Creates an image half the size. Rescales data bilinearily.
NA_API NAABYImage* naCreateBabyImageWithHalfSize(const NAABYImage* image);

// Creates an image which is a resize of the given image.
NA_API NAABYImage* naCreateBabyImageWithResize(
  const NAABYImage* image,
  NASizei newSize);

// Retains and Releases an image.
NA_API NAABYImage* naRetainBabyImage(const NAABYImage* image);
NA_API void naReleaseBabyImage(const NAABYImage* image);

// Returns the number of float values per horizontal line.
NA_API NAInt naGetABYImageValuesPerLine(const NAABYImage* image);

// Returns the image dimensions.
NA_API NASizei naGetBabyImageSize(const NAABYImage* image);

// Returns the raw image data.
NA_API NAABYColor* naGetBabyImageData(const NAABYImage* image);

// Fills the image with the given data. The data is expected to contain as
// many RGBA values stored as uint8 necessary for the whole image with no
// padding. Depending on the topToBottom flag, the data is expected as such.
// Note that a BabyImage internally always stores images bottom to top.
NA_API void naFillBabyImageWithu8(
  NAABYImage*      image,
  const void*       data,
  NABool            topToBottom,
  NAColorBufferType bufferType);

// Writes the contents of the image into a buffer. The output data will be
// RGBA values stored as uint8 with no padding. The data buffer must be big
// enough. Depending on the topToBottom flag, the data is expected as such.
// Note that a BabyImage internally always stores images bottom to top.
NA_API void naConvertBabyImageTou8(
  const NAABYImage* image,
  void*              data,
  NABool             topToBottom,
  NAColorBufferType  bufferType);


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_ABY_IMAGE_INCLUDED



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
