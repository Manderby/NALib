
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



#include "../../NAVectorAlgebra.h"



NA_IDEF NAPos naMakePos(double x, double y) {
  NAPos newPos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(x) && naIsOffsetValueValid(y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(x) && naIsOffsetValueUseful(y)))
      naError("Values given are not useful.");
  #endif
  newPos.x = x;
  newPos.y = y;
  return newPos;
}

NA_IDEF NAPosf naMakePosf(float x, float y) {
  NAPosf newPos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(x) && naIsOffsetValueValidf(y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefulf(x) && naIsOffsetValueUsefulf(y)))
      naError("Values given are not useful.");
  #endif
  newPos.x = x;
  newPos.y = y;
  return newPos;
}

NA_IDEF NAPosi32 naMakePosi32(int32 x, int32 y) {
  NAPosi32 newPos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi32(x) && naIsOffsetValueValidi32(y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuli32(x) && naIsOffsetValueUsefuli32(y)))
      naError("Values given are not useful.");
  #endif
  newPos.x = x;
  newPos.y = y;
  return newPos;
}

NA_IDEF NAPosi64 naMakePosi64(int64 x, int64 y) {
  NAPosi64 newPos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi64(x) && naIsOffsetValueValidi64(y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuli64(x) && naIsOffsetValueUsefuli64(y)))
      naError("Values given are not useful.");
  #endif
  newPos.x = x;
  newPos.y = y;
  return newPos;
}

NA_IDEF NAPoss naMakePoss(size_t x, size_t y) {
  NAPoss newPos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValids(x) && naIsOffsetValueValids(y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuls(x) && naIsOffsetValueUsefuls(y)))
      naError("Values given are not useful.");
  #endif
  newPos.x = x;
  newPos.y = y;
  return newPos;
}



NA_IDEF NAPos naMakePosZero() {
  NAPos newPos;
  newPos.x = 0.;
  newPos.y = 0.;
  return newPos;
}

NA_IDEF NAPosf naMakePosfZero() {
  NAPosf newPos;
  newPos.x = 0.f;
  newPos.y = 0.f;
  return newPos;
}

NA_IDEF NAPosi32 naMakePosi32Zero() {
  NAPosi32 newPos;
  newPos.x = 0;
  newPos.y = 0;
  return newPos;
}

NA_IDEF NAPosi64 naMakePosi64Zero() {
  NAPosi64 newPos;
  newPos.x = NA_ZERO_i64;
  newPos.y = NA_ZERO_i64;
  return newPos;
}

NA_IDEF NAPoss naMakePossZero() {
  NAPoss newPos;
  newPos.x = 0;
  newPos.y = 0;
  return newPos;
}



NA_IDEF void naFillPosWithCopy(NAPos* posd, const NAPos* poss) {
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naFillPosfWithCopy(NAPosf* posd, const NAPosf* poss) {
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naFillPosi32WithCopy(NAPosi32* posd, const NAPosi32* poss) {
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naFillPosi64WithCopy(NAPosi64* posd, const NAPosi64* poss) {
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naFillPossWithCopy(NAPoss* posd, const NAPoss* poss) {
  posd->x = poss->x;
  posd->y = poss->y;
}



NA_IDEF double naGetPosDistance(const NAPos pos1, const NAPos pos2) {
  NAVec2d p1 = {pos1.x, pos1.y};
  NAVec2d p2 = {pos2.x, pos2.y};
  return naDistanceV2d(p1, p2);
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
