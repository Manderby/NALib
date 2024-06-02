
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NASize naMakeSize(double width, double height) {
  NASize newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(width) && naIsLengthValueUseful(height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizef naMakeSizef(float width, float height) {
  NASizef newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidf(width) && naIsLengthValueValidf(height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefulf(width) && naIsLengthValueUsefulf(height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizei32 naMakeSizei32(int32 width, int32 height) {
  NASizei32 newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi32(width) && naIsLengthValueValidi32(height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli32(width) && naIsLengthValueUsefuli32(height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizei64 naMakeSizei64(int64 width, int64 height) {
  NASizei64 newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi64(width) && naIsLengthValueValidi64(height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli64(width) && naIsLengthValueUsefuli64(height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizes naMakeSizes(size_t width, size_t height) {
  NASizes newSize;
  #if NA_DEBUG
    if (!(naIsLengthValueValids(width) && naIsLengthValueValids(height)))
      naError("Invalid values given.");
    if (!(naIsLengthValueUsefuls(width) && naIsLengthValueUsefuls(height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}



NA_IDEF NASize naMakeSizeE(double width, double height) {
  NASize newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height)))
      naError("Invalid values given.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizef naMakeSizefE(float width, float height) {
  NASizef newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidf(width) && naIsLengthValueValidf(height)))
      naError("Invalid values given.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizei32 naMakeSizei32E(int32 width, int32 height) {
  NASizei32 newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi32(width) && naIsLengthValueValidi32(height)))
      naError("Invalid values given.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizei64 naMakeSizei64E(int64 width, int64 height) {
  NASizei64 newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi64(width) && naIsLengthValueValidi64(height)))
      naError("Invalid values given.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}

NA_IDEF NASizes naMakeSizesE(size_t width, size_t height) {
  NASizes newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValids(width) && naIsLengthValueValids(height)))
      naError("Invalid values given.");
  #endif
  newSize.width = width;
  newSize.height = height;
  return newSize;
}



//NA_IDEF NASizei naMakeSizeiWithIntegerSize(NASize size) {
//  NASizei newSize = naMakeSizeiE((NAInt)naGetDoubleInteger(size.width), (NAInt)naGetDoubleInteger(size.height));
//  return newSize;
//}



NA_IDEF NASize naMakeSizeZero() {
  NASize newSize;
  newSize.width = 0.;
  newSize.height = 0.;
  return newSize;
}

NA_IDEF NASizef naMakeSizefZero() {
  NASizef newSize;
  newSize.width = 0.f;
  newSize.height = 0.f;
  return newSize;
}

NA_IDEF NASizei32 naMakeSizei32Zero() {
  NASizei32 newSize;
  newSize.width = 0;
  newSize.height = 0;
  return newSize;
}

NA_IDEF NASizei64 naMakeSizei64Zero() {
  NASizei64 newSize;
  newSize.width = 0;
  newSize.height = 0;
  return newSize;
}

NA_IDEF NASizes naMakeSizesZero() {
  NASizes newSize;
  newSize.width = 0;
  newSize.height = 0;
  return newSize;
}



NA_IDEF void naFillSizeWithCopy(NASize* sized, const NASize* sizes) {
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naFillSizefWithCopy(NASizef* sized, const NASizef* sizes) {
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naFillSizei32WithCopy(NASizei32* sized, const NASizei32* sizes) {
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naFillSizei64WithCopy(NASizei64* sized, const NASizei64* sizes) {
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naFillSizesWithCopy(NASizes* sized, const NASizes* sizes) {
  sized->width  = sizes->width;
  sized->height = sizes->height;
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
