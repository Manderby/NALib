
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NABox naMakeBox(NAVertex vertex, NAVolume volume) {
  NABox newBox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("Invalid values given.");
    if(!naIsVertexUseful(vertex) || !naIsVolumeUseful(volume))
      naError("Values given are not useful.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxf naMakeBoxf(NAVertexf vertex, NAVolumef volume) {
  NABoxf newBox;
  #if NA_DEBUG
    if(!naIsVertexfValid(vertex) || !naIsVolumefValid(volume))
      naError("Invalid values given.");
    if(!naIsVertexfUseful(vertex) || !naIsVolumefUseful(volume))
      naError("Values given are not useful.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxi32 naMakeBoxi32(NAVertexi32 vertex, NAVolumei32 volume) {
  NABoxi32 newBox;
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex) || !naIsVolumei32Valid(volume))
      naError("Invalid values given.");
    if(!naIsVertexi32Useful(vertex) || !naIsVolumei32Useful(volume))
      naError("Values given are not useful.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64(NAVertexi64 vertex, NAVolumei64 volume) {
  NABoxi64 newBox;
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex) || !naIsVolumei64Valid(volume))
      naError("Invalid values given.");
    if(!naIsVertexi64Useful(vertex) || !naIsVolumei64Useful(volume))
      naError("Values given are not useful.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxs naMakeBoxs(NAVertexs vertex, NAVolumes volume) {
  NABoxs newBox;
  #if NA_DEBUG
    if(!naIsVertexsValid(vertex) || !naIsVolumesValid(volume))
      naError("Invalid values given.");
    if(!naIsVertexsUseful(vertex) || !naIsVolumesUseful(volume))
      naError("Values given are not useful.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}



NA_IDEF NABox naMakeBoxS(double x, double y, double z, double width, double height, double depth) {
  NABox newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertex(x, y, z);
  newBox.volume = naMakeVolume(width, height, depth);
  return newBox;
}

NA_IDEF NABoxf naMakeBoxfS(float x, float y, float z, float width, float height, float depth) {
  NABoxf newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexf(x, y, z);
  newBox.volume = naMakeVolumef(width, height, depth);
  return newBox;
}

NA_IDEF NABoxi32 naMakeBoxi32S(int32 x, int32 y, int32 z, int32 width, int32 height, int32 depth) {
  NABoxi32 newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexi32(x, y, z);
  newBox.volume = naMakeVolumei32(width, height, depth);
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64S(int64 x, int64 y, int64 z, int64 width, int64 height, int64 depth) {
  NABoxi64 newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexi64(x, y, z);
  newBox.volume = naMakeVolumei64(width, height, depth);
  return newBox;
}

NA_IDEF NABoxs naMakeBoxsS(size_t x, size_t y, size_t z, size_t width, size_t height, size_t depth) {
  NABoxs newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexs(x, y, z);
  newBox.volume = naMakeVolumes(width, height, depth);
  return newBox;
}



NA_IDEF NABox naMakeBoxE(NAVertex vertex, NAVolume volume) {
  NABox newBox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("Invalid values given.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxf naMakeBoxfE(NAVertexf vertex, NAVolumef volume) {
  NABoxf newBox;
  #if NA_DEBUG
    if(!naIsVertexfValid(vertex) || !naIsVolumefValid(volume))
      naError("Invalid values given.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxi32 naMakeBoxi32E(NAVertexi32 vertex, NAVolumei32 volume) {
  NABoxi32 newBox;
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex) || !naIsVolumei32Valid(volume))
      naError("Invalid values given.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64E(NAVertexi64 vertex, NAVolumei64 volume) {
  NABoxi64 newBox;
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex) || !naIsVolumei64Valid(volume))
      naError("Invalid values given.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}

NA_IDEF NABoxs naMakeBoxsE(NAVertexs vertex, NAVolumes volume) {
  NABoxs newBox;
  #if NA_DEBUG
    if(!naIsVertexsValid(vertex) || !naIsVolumesValid(volume))
      naError("Invalid values given.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}



NA_IDEF NABox naMakeBoxSE(double x, double y, double z, double width, double height, double depth) {
  NABox newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertex(x, y, z);
  newBox.volume = naMakeVolumeE(width, height, depth);
  return newBox;
}

NA_IDEF NABoxf naMakeBoxfSE(float x, float y, float z, float width, float height, float depth) {
  NABoxf newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexf(x, y, z);
  newBox.volume = naMakeVolumefE(width, height, depth);
  return newBox;
}

NA_IDEF NABoxi32 naMakeBoxi32SE(int32 x, int32 y, int32 z, int32 width, int32 height, int32 depth) {
  NABoxi32 newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexi32(x, y, z);
  newBox.volume = naMakeVolumei32E(width, height, depth);
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64SE(int64 x, int64 y, int64 z, int64 width, int64 height, int64 depth) {
  NABoxi64 newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexi64(x, y, z);
  newBox.volume = naMakeVolumei64E(width, height, depth);
  return newBox;
}

NA_IDEF NABoxs naMakeBoxsSE(size_t x, size_t y, size_t z, size_t width, size_t height, size_t depth) {
  NABoxs newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexs(x, y, z);
  newBox.volume = naMakeVolumesE(width, height, depth);
  return newBox;
}



NA_IDEF NABox naMakeBoxEmpty() {
  NABox newBox;
  newBox.volume.width = 0.;
  return newBox;
}

NA_IDEF NABoxf naMakeBoxfEmpty() {
  NABoxf newBox;
  newBox.volume.width = 0.f;
  return newBox;
}

NA_IDEF NABoxi32 naMakeBoxi32Empty() {
  NABoxi32 newBox;
  newBox.volume.width = 0;
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64Empty() {
  NABoxi64 newBox;
  newBox.volume.width = 0;
  return newBox;
}

NA_IDEF NABoxs naMakeBoxsEmpty() {
  NABoxs newBox;
  newBox.volume.width = 0;
  return newBox;
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
