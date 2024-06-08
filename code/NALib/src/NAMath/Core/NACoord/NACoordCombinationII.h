
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NARange naMakeRangeWithCombination(double point1, double point2) {
  NARange newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(point1) && naIsOffsetValueValid(point2)))
      naError("Invalid values given.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithStartAndEnd(point1, point2);
  return newRange;
}
NA_IDEF NARangef naMakeRangefWithCombination(float point1, float point2) {
  NARangef newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(point1) && naIsOffsetValueValidf(point2)))
      naError("Invalid values given.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithStartAndEndf(point1, point2);
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32WithCombination(int32 point1, int32 point2) {
  NARangei32 newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi32(point1) && naIsOffsetValueValidi32(point2)))
      naError("Invalid values given.");
    if(point2 < point1)
      naError("point1 should be smallerequal to point2.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithMinAndMaxi32(point1, point2);
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64WithCombination(int64 point1, int64 point2) {
  NARangei64 newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi64(point1) && naIsOffsetValueValidi64(point2)))
      naError("Invalid values given.");
    if(point2 < point1)
      naError("point1 should be smallerequal to point2.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithMinAndMaxi64(point1, point2);
  return newRange;
}
NA_IDEF NARanges naMakeRangesWithCombination(size_t point1, size_t point2) {
  NARanges newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValids(point1) && naIsOffsetValueValids(point2)))
      naError("Invalid values given.");
    if(point2 < point1)
      naError("point1 should be smallerequal to point2.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithMinAndMaxs(point1, point2);
  return newRange;
}



NA_IDEF NARange naMakeRangeWithExtension(NARange range, double point) {
  NARange newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValid(point))
      naError("Invalid point given.");
  #endif
  if(naIsRangeEmpty(range)) {
    newRange.length = 0.;
    newRange.origin = point;
  }else if(point < range.origin) {
    newRange.length = naMakeLengthWithStartAndEnd(point, naGetRangeEnd(range));
    newRange.origin = point;
  }else if(point >= naGetRangeEnd(range)) {
    newRange.length = naMakeLengthWithStartAndEnd(range.origin, point);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARangef naMakeRangefWithExtension(NARangef range, float point) {
  NARangef newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(point))
      naError("Invalid point given.");
  #endif
  if(naIsRangefEmpty(range)) {
    newRange.length = 0.f;
    newRange.origin = point;
  }else if(point < range.origin) {
    newRange.length = naMakeLengthWithStartAndEndf(point, naGetRangefEnd(range));
    newRange.origin = point;
  }else if(point >= naGetRangefEnd(range)) {
    newRange.length = naMakeLengthWithStartAndEndf(range.origin, point);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32WithExtension(NARangei32 range, int32 point) {
  NARangei32 newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(point))
      naError("Invalid point given.");
  #endif
  if(naIsRangei32Empty(range)) {
    newRange.length = 1;
    newRange.origin = point;
  }else if(point < range.origin) {
    newRange.length = naMakeLengthWithStartAndEndi32(point, naGetRangei32End(range));
    newRange.origin = point;
  }else if(point > naGetRangei32Max(range)) {
    newRange.length = naMakeLengthWithMinAndMaxi32(range.origin, point);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64WithExtension(NARangei64 range, int64 point) {
  NARangei64 newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(point))
      naError("Invalid point given.");
  #endif
  if(naIsRangei64Empty(range)) {
    newRange.length = 1;
    newRange.origin = point;
  }else if(point < range.origin) {
    newRange.length = naMakeLengthWithStartAndEndi64(point, naGetRangei64End(range));
    newRange.origin = point;
  }else if(point > naGetRangei64Max(range)) {
    newRange.length = naMakeLengthWithMinAndMaxi64(range.origin, point);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARanges naMakeRangesWithExtension(NARanges range, size_t point) {
  NARanges newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValids(point))
      naError("Invalid point given.");
  #endif
  if(naIsRangesEmpty(range)) {
    newRange.length = 1;
    newRange.origin = point;
  }else if(point < range.origin) {
    newRange.length = naMakeLengthWithStartAndEnds(point, naGetRangesEnd(range));
    newRange.origin = point;
  }else if(point > naGetRangesMax(range)) {
    newRange.length = naMakeLengthWithMinAndMaxs(range.origin, point);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}



NA_IDEF NARange naMakeRangeWithUnion(NARange range1, NARange range2) {
  NARange newRange;
  #if NA_DEBUG
    if(!naIsRangeValid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangeValid(range2))
      naError("Invalid range 2 given.");
  #endif
  if(naIsRangeEmpty(range1) && naIsRangeEmpty(range2)) {
    newRange = naMakeRangeZero();
  }else if(naIsRangeEmpty(range1)) {
    newRange = range2;
  }else if(naIsRangeEmpty(range2)) {
    newRange = range1;
  }else{
    newRange = naMakeRangeWithCombination(naMin(range1.origin, range2.origin), naMax(naGetRangeEnd(range1), naGetRangeEnd(range2)));
  }
  return newRange;
}

NA_IDEF NARangef naMakeRangefWithUnion(NARangef range1, NARangef range2) {
  NARangef newRange;
  #if NA_DEBUG
    if(!naIsRangefValid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangefValid(range2))
      naError("Invalid range 2 given.");
  #endif
  if(naIsRangefEmpty(range1) && naIsRangefEmpty(range2)) {
    newRange = naMakeRangefZero();
  }else if(naIsRangefEmpty(range1)) {
    newRange = range2;
  }else if(naIsRangefEmpty(range2)) {
    newRange = range1;
  }else{
    newRange = naMakeRangefWithCombination(naMin(range1.origin, range2.origin), naMax(naGetRangefEnd(range1), naGetRangefEnd(range2)));
  }
  return newRange;
}

NA_IDEF NARangei32 naMakeRangei32WithUnion(NARangei32 range1, NARangei32 range2) {
  NARangei32 newRange;
  #if NA_DEBUG
    if(!naIsRangei32Valid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangei32Valid(range2))
      naError("Invalid range 2 given.");
  #endif
  if(naIsRangei32Empty(range1) && naIsRangei32Empty(range2)) {
    newRange = naMakeRangei32Zero();
  }else if(naIsRangei32Empty(range1)) {
    newRange = range2;
  }else if(naIsRangei32Empty(range2)) {
    newRange = range1;
  }else{
    newRange = naMakeRangei32WithCombination(naMini32(range1.origin, range2.origin), naMaxi32(naGetRangei32Max(range1), naGetRangei32Max(range2)));
  }
  return newRange;
}

NA_IDEF NARangei64 naMakeRangei64WithUnion(NARangei64 range1, NARangei64 range2) {
  NARangei64 newRange;
  #if NA_DEBUG
    if(!naIsRangei64Valid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangei64Valid(range2))
      naError("Invalid range 2 given.");
  #endif
  if(naIsRangei64Empty(range1) && naIsRangei64Empty(range2)) {
    newRange = naMakeRangei64Zero();
  }else if(naIsRangei64Empty(range1)) {
    newRange = range2;
  }else if(naIsRangei64Empty(range2)) {
    newRange = range1;
  }else{
    newRange = naMakeRangei64WithCombination(naMini64(range1.origin, range2.origin), naMaxi64(naGetRangei64Max(range1), naGetRangei64Max(range2)));
  }
  return newRange;
}

NA_IDEF NARanges naMakeRangesWithUnion(NARanges range1, NARanges range2) {
  NARanges newRange;
  #if NA_DEBUG
    if(!naIsRangesValid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangesValid(range2))
      naError("Invalid range 2 given.");
  #endif
  if(naIsRangesEmpty(range1) && naIsRangesEmpty(range2)) {
    newRange = naMakeRangesZero();
  }else if(naIsRangesEmpty(range1)) {
    newRange = range2;
  }else if(naIsRangesEmpty(range2)) {
    newRange = range1;
  }else{
    newRange = naMakeRangesWithCombination(naMins(range1.origin, range2.origin), naMaxs(naGetRangesMax(range1), naGetRangesMax(range2)));
  }
  return newRange;
}



NA_IDEF NARange naMakeRangeWithIntersection(NARange range1, NARange range2) {
  NARange newRange;
  double end1;
  double end2;
  #if NA_DEBUG
    if(!naIsRangeValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangeValid(range2))
      naError("range2 is invalid.");
  #endif
  newRange.origin = naMax(range1.origin, range2.origin);
  end1 = naGetRangeEnd(range1);
  end2 = naGetRangeEnd(range2);
  newRange.length  = naMakeLengthWithStartAndEnd(newRange.origin, naMin(end1, end2));
  return newRange;
}

NA_IDEF NARangef naMakeRangefWithIntersection(NARangef range1, NARangef range2) {
  NARangef newRange;
  float end1;
  float end2;
  #if NA_DEBUG
    if(!naIsRangefValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangefValid(range2))
      naError("range2 is invalid.");
  #endif
  newRange.origin = naMax(range1.origin, range2.origin);
  end1 = naGetRangefEnd(range1);
  end2 = naGetRangefEnd(range2);
  newRange.length  = naMakeLengthWithStartAndEndf(newRange.origin, naMin(end1, end2));
  return newRange;
}

NA_IDEF NARangei32 naMakeRangei32WithIntersection(NARangei32 range1, NARangei32 range2) {
  NARangei32 newRange;
  int32 end1;
  int32 end2;
  #if NA_DEBUG
    if(!naIsRangei32Valid(range1))
      naError("range1 is invalid.");
    if(!naIsRangei32Valid(range2))
      naError("range2 is invalid.");
  #endif
  newRange.origin = naMaxi32(range1.origin, range2.origin);
  end1 = naGetRangei32End(range1);
  end2 = naGetRangei32End(range2);
  newRange.length  = naMakeLengthWithStartAndEndi32(newRange.origin, naMini32(end1, end2));
  return newRange;
}

NA_IDEF NARangei64 naMakeRangei64WithIntersection(NARangei64 range1, NARangei64 range2) {
  NARangei64 newRange;
  int64 end1;
  int64 end2;
  #if NA_DEBUG
    if(!naIsRangei64Valid(range1))
      naError("range1 is invalid.");
    if(!naIsRangei64Valid(range2))
      naError("range2 is invalid.");
  #endif
  newRange.origin = naMaxi64(range1.origin, range2.origin);
  end1 = naGetRangei64End(range1);
  end2 = naGetRangei64End(range2);
  newRange.length  = naMakeLengthWithStartAndEndi64(newRange.origin, naMini64(end1, end2));
  return newRange;
}

NA_IDEF NARanges naMakeRangesWithIntersection(NARanges range1, NARanges range2) {
  NARanges newRange;
  int64 end1;
  int64 end2;
  #if NA_DEBUG
    if(!naIsRangesValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangesValid(range2))
      naError("range2 is invalid.");
  #endif
  newRange.origin = naMaxs(range1.origin, range2.origin);
  end1 = naGetRangesEnd(range1);
  end2 = naGetRangesEnd(range2);
  newRange.length  = naMakeLengthWithStartAndEnds(newRange.origin, naMins(end1, end2));
  return newRange;
}



NA_IDEF NARect naMakeRectWithPosAndPos(NAPos pos1, NAPos pos2) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x) {
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y) {
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(pos2.y, pos1.y);
  }
  return newRect;
}

NA_IDEF NARectf naMakeRectfWithPosAndPos(NAPosf pos1, NAPosf pos2) {
  NARectf newRect;
  #if NA_DEBUG
    if(!naIsPosfValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosfValid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x) {
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithStartAndEndf(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithStartAndEndf(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y) {
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithStartAndEndf(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithStartAndEndf(pos2.y, pos1.y);
  }
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32WithPosAndPos(NAPosi32 pos1, NAPosi32 pos2) {
  NARecti32 newRect;
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosi32Valid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x) {
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi32(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi32(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y) {
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi32(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi32(pos2.y, pos1.y);
  }
  return newRect;
}

NA_IDEF NARecti64 naMakeRecti64WithPosAndPos(NAPosi64 pos1, NAPosi64 pos2) {
  NARecti64 newRect;
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosi64Valid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x) {
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi64(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi64(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y) {
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi64(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi64(pos2.y, pos1.y);
  }
  return newRect;
}

NA_IDEF NARects naMakeRectsWithPosAndPos(NAPoss pos1, NAPoss pos2) {
  NARects newRect;
  #if NA_DEBUG
    if(!naIsPossValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPossValid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x) {
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxs(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxs(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y) {
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxs(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxs(pos2.y, pos1.y);
  }
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
