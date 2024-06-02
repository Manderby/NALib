
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NAVolume naMakeVolume(double width, double height, double depth) {
  NAVolume newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(width) && naIsLengthValueUseful(height) && naIsLengthValueUseful(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumef naMakeVolumef(float width, float height, float depth) {
  NAVolumef newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidf(width) && naIsLengthValueValidf(height) && naIsLengthValueValidf(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefulf(width) && naIsLengthValueUsefulf(height) && naIsLengthValueUsefulf(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumei32 naMakeVolumei32(int32 width, int32 height, int32 depth) {
  NAVolumei32 newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi32(width) && naIsLengthValueValidi32(height) && naIsLengthValueValidi32(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli32(width) && naIsLengthValueUsefuli32(height) && naIsLengthValueUsefuli32(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumei64 naMakeVolumei64(int64 width, int64 height, int64 depth) {
  NAVolumei64 newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi64(width) && naIsLengthValueValidi64(height) && naIsLengthValueValidi64(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli64(width) && naIsLengthValueUsefuli64(height) && naIsLengthValueUsefuli64(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumes naMakeVolumes(size_t width, size_t height, size_t depth) {
  NAVolumes newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValids(width) && naIsLengthValueValids(height) && naIsLengthValueValids(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuls(width) && naIsLengthValueUsefuls(height) && naIsLengthValueUsefuls(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}



NA_IDEF NAVolume naMakeVolumeE(double width, double height, double depth) {
  NAVolume newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("Invalid values given.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumef naMakeVolumefE(float width, float height, float depth) {
  NAVolumef newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidf(width) && naIsLengthValueValidf(height) && naIsLengthValueValidf(depth)))
      naError("Invalid values given.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumei32 naMakeVolumei32E(int32 width, int32 height, int32 depth) {
  NAVolumei32 newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi32(width) && naIsLengthValueValidi32(height) && naIsLengthValueValidi32(depth)))
      naError("Invalid values given.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumei64 naMakeVolumei64E(int64 width, int64 height, int64 depth) {
  NAVolumei64 newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi64(width) && naIsLengthValueValidi64(height) && naIsLengthValueValidi64(depth)))
      naError("Invalid values given.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}

NA_IDEF NAVolumes naMakeVolumesE(size_t width, size_t height, size_t depth) {
  NAVolumes newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValids(width) && naIsLengthValueValids(height) && naIsLengthValueValids(depth)))
      naError("Invalid values given.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}



NA_IDEF NAVolume naMakeVolumeZero() {
  NAVolume newVolume;
  newVolume.width  = 0.;
  newVolume.height = 0.;
  newVolume.depth  = 0.;
  return newVolume;
}
NA_IDEF NAVolumef naMakeVolumefZero() {
  NAVolumef newVolume;
  newVolume.width  = 0.f;
  newVolume.height = 0.f;
  newVolume.depth  = 0.f;
  return newVolume;
}
NA_IDEF NAVolumei32 naMakeVolumei32Zero() {
  NAVolumei32 newVolume;
  newVolume.width  = 0;
  newVolume.height = 0;
  newVolume.depth  = 0;
  return newVolume;
}
NA_IDEF NAVolumei64 naMakeVolumei64Zero() {
  NAVolumei64 newVolume;
  newVolume.width  = 0;
  newVolume.height = 0;
  newVolume.depth  = 0;
  return newVolume;
}
NA_IDEF NAVolumes naMakeVolumesZero() {
  NAVolumes newVolume;
  newVolume.width  = 0;
  newVolume.height = 0;
  newVolume.depth  = 0;
  return newVolume;
}



NA_IDEF void naFillVolumeWithCopy(NAVolume* volumed, const NAVolume* volumes) {
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naFillVolumefWithCopy(NAVolumef* volumed, const NAVolumef* volumes) {
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naFillVolumei32WithCopy(NAVolumei32* volumed, const NAVolumei32* volumes) {
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naFillVolumei64WithCopy(NAVolumei64* volumed, const NAVolumei64* volumes) {
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naFillVolumesWithCopy(NAVolumes* volumed, const NAVolumes* volumes) {
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
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
