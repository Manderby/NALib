
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NARange naMakeRangeCombination(double point1, double point2) {
  NARange newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(point1) && naIsOffsetValueValid(point2)))
      naError("Invalid values given.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithStartAndEnd(point1, point2);
  return newRange;
}
NA_IDEF NARangef naMakeRangefCombination(float point1, float point2) {
  NARangef newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(point1) && naIsOffsetValueValidf(point2)))
      naError("Invalid values given.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithStartAndEndf(point1, point2);
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32Combination(int32 point1, int32 point2) {
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
NA_IDEF NARangei64 naMakeRangei64Combination(int64 point1, int64 point2) {
  NARangei64 newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi64(point1) && naIsOffsetValueValidi64(point2)))
      naError("Invalid values given.");
    if(naSmalleri64(point2, point1))
      naError("point1 should be smallerequal to point2.");
  #endif
  newRange.origin = point1;
  newRange.length = naMakeLengthWithMinAndMaxi64(point1, point2);
  return newRange;
}
NA_IDEF NARanges naMakeRangesCombination(size_t point1, size_t point2) {
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



NA_IDEF NARange naMakeRangeExtension(NARange range, double point) {
  #if NA_DEBUG
    if(naIsRangeEmpty(range))
      naError("range is empty.");
    if(!naIsRangeUseful(range))
      naError("range is not useful.");
    if(!naIsOffsetValueValid(point))
      naError("Invalid point given.");
  #endif

  NARange newRange;
  if(point < range.origin) {
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
NA_IDEF NARangef naMakeRangefExtension(NARangef range, float point) {
  #if NA_DEBUG
    if(naIsRangefEmpty(range))
      naError("range is empty.");
    if(!naIsRangefUseful(range))
      naError("range is not useful.");
    if(!naIsOffsetValueValidf(point))
      naError("Invalid point given.");
  #endif

  NARangef newRange;
  if(point < range.origin) {
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
NA_IDEF NARangei32 naMakeRangei32Extension(NARangei32 range, int32 point) {
  #if NA_DEBUG
    if(naIsRangei32Empty(range))
      naError("range is empty.");
    if(!naIsRangei32Useful(range))
      naError("range is not useful.");
    if(!naIsOffsetValueValidi32(point))
      naError("Invalid point given.");
  #endif

  NARangei32 newRange;
  if(point < range.origin) {
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
NA_IDEF NARangei64 naMakeRangei64Extension(NARangei64 range, int64 point) {
  #if NA_DEBUG
    if(naIsRangei64Empty(range))
      naError("range is empty.");
    if(!naIsRangei64Useful(range))
      naError("range is not useful.");
    if(!naIsOffsetValueValidi64(point))
      naError("Invalid point given.");
  #endif
  
  NARangei64 newRange;
  if(naSmalleri64(point, range.origin)) {
    newRange.length = naMakeLengthWithStartAndEndi64(point, naGetRangei64End(range));
    newRange.origin = point;
  }else if(naGreateri64(point, naGetRangei64Max(range))) {
    newRange.length = naMakeLengthWithMinAndMaxi64(range.origin, point);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARanges naMakeRangesExtension(NARanges range, size_t point) {
  #if NA_DEBUG
    if(naIsRangesEmpty(range))
      naError("range is empty.");
    if(!naIsRangesUseful(range))
      naError("range is not useful.");
    if(!naIsOffsetValueValids(point))
      naError("Invalid point given.");
  #endif

  NARanges newRange;
  if(point < range.origin) {
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



NA_IDEF NARange naMakeRangeExtensionE(NARange range, double point) {
  #if NA_DEBUG
    if(!naIsRangeEmpty(range) && !naIsRangeValid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValid(point))
      naError("point is invalid.");
  #endif
  
  if(naIsRangeEmpty(range)) {
    NARange newRange;
    newRange.length = 0.;
    newRange.origin = point;
    return newRange;
  }else{
    return naMakeRangeExtension(range, point);
  }
}
NA_IDEF NARangef naMakeRangefExtensionE(NARangef range, float point) {
  #if NA_DEBUG
    if(!naIsRangefEmpty(range) && !naIsRangefValid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidf(point))
      naError("point is invalid.");
  #endif
  
  if(naIsRangefEmpty(range)) {
    NARangef newRange;
    newRange.length = 0.f;
    newRange.origin = point;
    return newRange;
  }else{
    return naMakeRangefExtension(range, point);
  }
}
NA_IDEF NARangei32 naMakeRangei32ExtensionE(NARangei32 range, int32 point) {
  #if NA_DEBUG
    if(!naIsRangei32Empty(range) && !naIsRangei32Valid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi32(point))
      naError("point is invalid.");
  #endif
  
  if(naIsRangei32Empty(range)) {
    NARangei32 newRange;
    newRange.length = 1;
    newRange.origin = point;
    return newRange;
  }else{
    return naMakeRangei32Extension(range, point);
  }
}
NA_IDEF NARangei64 naMakeRangei64ExtensionE(NARangei64 range, int64 point) {
  #if NA_DEBUG
    if(!naIsRangei64Empty(range) && !naIsRangei64Valid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi64(point))
      naError("point is invalid.");
  #endif
  
  if(naIsRangei64Empty(range)) {
    NARangei64 newRange;
    newRange.length = NA_ONE_i64;
    newRange.origin = point;
    return newRange;
  }else{
    return naMakeRangei64Extension(range, point);
  }
}
NA_IDEF NARanges naMakeRangesExtensionE(NARanges range, size_t point) {
  #if NA_DEBUG
    if(!naIsRangesEmpty(range) && !naIsRangesValid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValids(point))
      naError("point is invalid.");
  #endif
  
  if(naIsRangesEmpty(range)) {
    NARanges newRange;
    newRange.length = 1;
    newRange.origin = point;
    return newRange;
  }else{
    return naMakeRangesExtension(range, point);
  }
}



NA_IDEF NARange naMakeRangeUnion(NARange range1, NARange range2) {
  #if NA_DEBUG
    if(!naIsRangeValid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangeValid(range2))
      naError("Invalid range 2 given.");
  #endif

  NARange newRange = naMakeRangeCombination(
    naMin(range1.origin, range2.origin),
    naMax(naGetRangeEnd(range1), naGetRangeEnd(range2)));
  return newRange;
}
NA_IDEF NARangef naMakeRangefUnion(NARangef range1, NARangef range2) {
  #if NA_DEBUG
    if(!naIsRangefValid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangefValid(range2))
      naError("Invalid range 2 given.");
  #endif

  NARangef newRange = naMakeRangefCombination(
    naMinf(range1.origin, range2.origin),
    naMaxf(naGetRangefEnd(range1), naGetRangefEnd(range2)));
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32Union(NARangei32 range1, NARangei32 range2) {
  #if NA_DEBUG
    if(!naIsRangei32Valid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangei32Valid(range2))
      naError("Invalid range 2 given.");
  #endif

  NARangei32 newRange = naMakeRangei32Combination(
    naMini32(range1.origin, range2.origin),
    naMaxi32(naGetRangei32Max(range1), naGetRangei32Max(range2)));
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64Union(NARangei64 range1, NARangei64 range2) {
  #if NA_DEBUG
    if(!naIsRangei64Valid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangei64Valid(range2))
      naError("Invalid range 2 given.");
  #endif

  NARangei64 newRange = naMakeRangei64Combination(
    naMini64(range1.origin, range2.origin),
    naMaxi64(naGetRangei64Max(range1), naGetRangei64Max(range2)));
  return newRange;
}
NA_IDEF NARanges naMakeRangesUnion(NARanges range1, NARanges range2) {
  #if NA_DEBUG
    if(!naIsRangesValid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangesValid(range2))
      naError("Invalid range 2 given.");
  #endif

  NARanges newRange = naMakeRangesCombination(
    naMins(range1.origin, range2.origin),
    naMaxs(naGetRangesMax(range1), naGetRangesMax(range2)));
  return newRange;
}



NA_IDEF NARange naMakeRangeUnionE(NARange range1, NARange range2) {
  #if NA_DEBUG
    if(!naIsRangeEmpty(range1) && !naIsRangeValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangeEmpty(range2) && !naIsRangeValid(range2))
      naError("range2 is invalid.");
  #endif
  
  if(naIsRangeEmpty(range1))
    return range2;
  if(naIsRangeEmpty(range2))
    return range1;
  
  return naMakeRangeUnion(range1, range2);
}
NA_IDEF NARangef naMakeRangefUnionE(NARangef range1, NARangef range2) {
  #if NA_DEBUG
    if(!naIsRangefEmpty(range1) && !naIsRangefValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangefEmpty(range2) && !naIsRangefValid(range2))
      naError("range2 is invalid.");
  #endif
  
  if(naIsRangefEmpty(range1))
    return range2;
  if(naIsRangefEmpty(range2))
    return range1;
  
  return naMakeRangefUnion(range1, range2);
}
NA_IDEF NARangei32 naMakeRangei32UnionE(NARangei32 range1, NARangei32 range2) {
  #if NA_DEBUG
    if(!naIsRangei32Empty(range1) && !naIsRangei32Valid(range1))
      naError("range1 is invalid.");
    if(!naIsRangei32Empty(range2) && !naIsRangei32Valid(range2))
      naError("range2 is invalid.");
  #endif
  
  if(naIsRangei32Empty(range1))
    return range2;
  if(naIsRangei32Empty(range2))
    return range1;
  
  return naMakeRangei32Union(range1, range2);
}
NA_IDEF NARangei64 naMakeRangei64UnionE(NARangei64 range1, NARangei64 range2) {
  #if NA_DEBUG
    if(!naIsRangei64Empty(range1) && !naIsRangei64Valid(range1))
      naError("range1 is invalid.");
    if(!naIsRangei64Empty(range2) && !naIsRangei64Valid(range2))
      naError("range2 is invalid.");
  #endif
  
  if(naIsRangei64Empty(range1))
    return range2;
  if(naIsRangei64Empty(range2))
    return range1;
  
  return naMakeRangei64Union(range1, range2);
}
NA_IDEF NARanges naMakeRangesUnionE(NARanges range1, NARanges range2) {
  #if NA_DEBUG
    if(!naIsRangesEmpty(range1) && !naIsRangesValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangesEmpty(range2) && !naIsRangesValid(range2))
      naError("range2 is invalid.");
  #endif
  
  if(naIsRangesEmpty(range1))
    return range2;
  if(naIsRangesEmpty(range2))
    return range1;
  
  return naMakeRangesUnion(range1, range2);
}



NA_IDEF NARange naMakeRangeIntersection(NARange range1, NARange range2) {
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
NA_IDEF NARangef naMakeRangefIntersection(NARangef range1, NARangef range2) {
  NARangef newRange;
  float end1;
  float end2;
  #if NA_DEBUG
    if(!naIsRangefValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangefValid(range2))
      naError("range2 is invalid.");
  #endif
  newRange.origin = naMaxf(range1.origin, range2.origin);
  end1 = naGetRangefEnd(range1);
  end2 = naGetRangefEnd(range2);
  newRange.length  = naMakeLengthWithStartAndEndf(newRange.origin, naMinf(end1, end2));
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32Intersection(NARangei32 range1, NARangei32 range2) {
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
NA_IDEF NARangei64 naMakeRangei64Intersection(NARangei64 range1, NARangei64 range2) {
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
NA_IDEF NARanges naMakeRangesIntersection(NARanges range1, NARanges range2) {
  NARanges newRange;
  size_t end1;
  size_t end2;
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
