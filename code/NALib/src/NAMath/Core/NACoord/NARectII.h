
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NARect naMakeRect(NAPos pos, NASize size) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("Invalid values given.");
    if(!naIsPosUseful(pos) || !naIsSizeUseful(size))
      naError("Values given are not useful.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARectf naMakeRectf(NAPosf pos, NASizef size) {
  NARectf newRect;
  #if NA_DEBUG
    if(!naIsPosfValid(pos) || !naIsSizefValid(size))
      naError("Invalid values given.");
    if(!naIsPosfUseful(pos) || !naIsSizefUseful(size))
      naError("Values given are not useful.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32(NAPosi32 pos, NASizei32 size) {
  NARecti32 newRect;
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos) || !naIsSizei32Valid(size))
      naError("Invalid values given.");
    if(!naIsPosi32Useful(pos) || !naIsSizei32Useful(size))
      naError("Values given are not useful.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARecti64 naMakeRecti64(NAPosi64 pos, NASizei64 size) {
  NARecti64 newRect;
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos) || !naIsSizei64Valid(size))
      naError("Invalid values given.");
    if(!naIsPosi64Useful(pos) || !naIsSizei64Useful(size))
      naError("Values given are not useful.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARects naMakeRectis(NAPoss pos, NASizes size) {
  NARects newRect;
  #if NA_DEBUG
    if(!naIsPossValid(pos) || !naIsSizesValid(size))
      naError("Invalid values given.");
    if(!naIsPossUseful(pos) || !naIsSizesUseful(size))
      naError("Values given are not useful.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}



NA_IDEF NARect naMakeRectE(NAPos pos, NASize size) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("Invalid values given.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARectf naMakeRectfE(NAPosf pos, NASizef size) {
  NARectf newRect;
  #if NA_DEBUG
    if(!naIsPosfValid(pos) || !naIsSizefValid(size))
      naError("Invalid values given.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32E(NAPosi32 pos, NASizei32 size) {
  NARecti32 newRect;
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos) || !naIsSizei32Valid(size))
      naError("Invalid values given.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARecti64 naMakeRecti64E(NAPosi64 pos, NASizei64 size) {
  NARecti64 newRect;
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos) || !naIsSizei64Valid(size))
      naError("Invalid values given.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}

NA_IDEF NARects naMakeRectsE(NAPoss pos, NASizes size) {
  NARects newRect;
  #if NA_DEBUG
    if(!naIsPossValid(pos) || !naIsSizesValid(size))
      naError("Invalid values given.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}



NA_IDEF NARect naMakeRectS(double x, double y, double width, double height) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValid(x) || !naIsOffsetValueValid(y) || !naIsLengthValueValid(width) || !naIsLengthValueValid(height))
      naError("Invalid values given.");
    if(!naIsOffsetValueUseful(x) || !naIsOffsetValueUseful(y) || !naIsLengthValueUseful(width) || !naIsLengthValueUseful(height))
      naError("Values given are not useful.");
  #endif
  newRect.pos = naMakePos(x, y);
  newRect.size = naMakeSize(width, height);
  return newRect;
}

NA_IDEF NARectf naMakeRectfS(float x, float y, float width, float height) {
  NARectf newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(x) || !naIsOffsetValueValidf(y) || !naIsLengthValueValidf(width) || !naIsLengthValueValidf(height))
      naError("Invalid values given.");
    if(!naIsOffsetValueUsefulf(x) || !naIsOffsetValueUsefulf(y) || !naIsLengthValueUsefulf(width) || !naIsLengthValueUsefulf(height))
      naError("Values given are not useful.");
  #endif
  newRect.pos = naMakePosf(x, y);
  newRect.size = naMakeSizef(width, height);
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32S(int32 x, int32 y, int32 width, int32 height) {
  NARecti32 newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(x) || !naIsOffsetValueValidi32(y) || !naIsLengthValueValidi32(width) || !naIsLengthValueValidi32(height))
      naError("Invalid values given.");
    if(!naIsOffsetValueUsefuli32(x) || !naIsOffsetValueUsefuli32(y) || !naIsLengthValueUsefuli32(width) || !naIsLengthValueUsefuli32(height))
      naError("Values given are not useful.");
  #endif
  newRect.pos = naMakePosi32(x, y);
  newRect.size = naMakeSizei32(width, height);
  return newRect;
}

NA_IDEF NARecti64 naMakeRecti64S(int64 x, int64 y, int64 width, int64 height) {
  NARecti64 newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(x) || !naIsOffsetValueValidi64(y) || !naIsLengthValueValidi64(width) || !naIsLengthValueValidi64(height))
      naError("Invalid values given.");
    if(!naIsOffsetValueUsefuli64(x) || !naIsOffsetValueUsefuli64(y) || !naIsLengthValueUsefuli64(width) || !naIsLengthValueUsefuli64(height))
      naError("Values given are not useful.");
  #endif
  newRect.pos = naMakePosi64(x, y);
  newRect.size = naMakeSizei64(width, height);
  return newRect;
}

NA_IDEF NARects naMakeRectsS(size_t x, size_t y, size_t width, size_t height) {
  NARects newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValids(x) || !naIsOffsetValueValids(y) || !naIsLengthValueValids(width) || !naIsLengthValueValids(height))
      naError("Invalid values given.");
    if(!naIsOffsetValueUsefuls(x) || !naIsOffsetValueUsefuls(y) || !naIsLengthValueUsefuls(width) || !naIsLengthValueUsefuls(height))
      naError("Values given are not useful.");
  #endif
  newRect.pos = naMakePoss(x, y);
  newRect.size = naMakeSizes(width, height);
  return newRect;
}



NA_IDEF NARect naMakeRectSE(double x, double y, double width, double height) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValid(x) || !naIsOffsetValueValid(y) || !naIsLengthValueValid(width) || !naIsLengthValueValid(height))
      naError("Invalid values given.");
  #endif
  newRect.pos = naMakePos(x, y);
  newRect.size = naMakeSizeE(width, height);
  return newRect;
}

NA_IDEF NARectf naMakeRectfSE(float x, float y, float width, float height) {
  NARectf newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(x) || !naIsOffsetValueValidf(y) || !naIsLengthValueValidf(width) || !naIsLengthValueValidf(height))
      naError("Invalid values given.");
  #endif
  newRect.pos = naMakePosf(x, y);
  newRect.size = naMakeSizefE(width, height);
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32SE(int32 x, int32 y, int32 width, int32 height) {
  NARecti32 newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(x) || !naIsOffsetValueValidi32(y) || !naIsLengthValueValidi32(width) || !naIsLengthValueValidi32(height))
      naError("Invalid values given.");
  #endif
  newRect.pos = naMakePosi32(x, y);
  newRect.size = naMakeSizei32E(width, height);
  return newRect;
}

NA_IDEF NARecti64 naMakeRecti64SE(int64 x, int64 y, int64 width, int64 height) {
  NARecti64 newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(x) || !naIsOffsetValueValidi64(y) || !naIsLengthValueValidi64(width) || !naIsLengthValueValidi64(height))
      naError("Invalid values given.");
  #endif
  newRect.pos = naMakePosi64(x, y);
  newRect.size = naMakeSizei64E(width, height);
  return newRect;
}

NA_IDEF NARects naMakeRectsSE(size_t x, size_t y, size_t width, size_t height) {
  NARects newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValids(x) || !naIsOffsetValueValids(y) || !naIsLengthValueValids(width) || !naIsLengthValueValids(height))
      naError("Invalid values given.");
  #endif
  newRect.pos = naMakePoss(x, y);
  newRect.size = naMakeSizesE(width, height);
  return newRect;
}



NA_IDEF NARect naMakeRectZero() {
  NARect newRect;
  newRect.pos.x = 0.;
  newRect.pos.y = 0.;
  newRect.size.width = 0.;
  newRect.size.height = 0.;
  return newRect;
}

NA_IDEF NARectf naMakeRectfZero() {
  NARectf newRect;
  newRect.pos.x = 0.f;
  newRect.pos.y = 0.f;
  newRect.size.width = 0.f;
  newRect.size.height = 0.f;
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32Zero() {
  NARecti32 newRect;
  newRect.pos.x = 0;
  newRect.pos.y = 0;
  newRect.size.width = 0;
  newRect.size.height = 0;
  return newRect;
}

NA_IDEF NARecti64 naMakeRecti64Zero() {
  NARecti64 newRect;
  newRect.pos.x = 0;
  newRect.pos.y = 0;
  newRect.size.width = 0;
  newRect.size.height = 0;
  return newRect;
}

NA_IDEF NARects naMakeRectsZero() {
  NARects newRect;
  newRect.pos.x = 0;
  newRect.pos.y = 0;
  newRect.size.width = 0;
  newRect.size.height = 0;
  return newRect;
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
