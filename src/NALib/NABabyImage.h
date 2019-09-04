
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

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


#include "NABase.h"
#include "NACoord.h"
#include "NABabyColor.h"


typedef struct NABabyImage NABabyImage;

// Note that BLACK_GREEN and WHITE_GREEN only work for opaque images.
typedef enum{
  NA_BLEND,             // Linear interpolation according blend factor.
  NA_BLEND_OVERLAY,     // Blends where top is opaque.
  NA_BLEND_OPAQUE,      // Blends where base is opaque.
  NA_BLEND_BLACK_GREEN, // Blends where base has dark green pixels
  NA_BLEND_WHITE_GREEN  // Blends where base has light green pixels
} NABlendMode;

// Allocates an image with the specified size and fills it with the given color.
// If color is Null, the image contents will be uninitialized.
NA_API NABabyImage* naAllocBabyImage(NASizei size, const NABabyColor color);

// Allocates a new image with a semi-transparent one-color representation of
// the base image. The mode defines, how the tint color will be applied. The
// blend factor defines how strong the tinting is.
NA_API NABabyImage* naAllocBabyImageWithTint(
  const NABabyImage* base,
  const NABabyColor tint,
  NABlendMode mode,
  float blend);

// Allocates a new image blending the top image upon the base image using the
// given blend factor and blend mode. If base is Null, mode is ignored and
// the top image is just blended on top of a transparent background.
NA_DEF NABabyImage* naAllocBabyImageWithBlend(
  const NABabyImage* base,
  const NABabyImage* top,
  NABlendMode mode,
  float blend);

// Allocates an image half the size. Rescales data bilinearily.
NA_API NABabyImage* naAllocBabyImageWithHalfSize(const NABabyImage* image);

// Deallocates an image.
NA_API void naDeallocBabyImage(NABabyImage* image);

// Returns the number of bytes per horizontal line.
NA_API NAInt naGetBabyImageValuesPerLine(const NABabyImage* image);

// Returns the image dimensions.
NA_API NASizei naGetBabyImageSize(const NABabyImage* image);

// Fills the image with the given data. The data is expected to contain as
// many RGBA values stored as uint8 necessary for the whole image.
NA_API void naFillBabyImageWithUInt8(NABabyImage* image, const void* data, NABool premultiplied);

// Writes the contents of the image into a buffer. The output data will be
// RGBA values stored as uint8. The data buffer must be big enough.
NA_API void naConvertBabyImageToUInt8(const NABabyImage* image, void* data, NABool premultiplied);


#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_BABY_IMAGE_INCLUDED

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
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
