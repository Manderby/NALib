
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
    if(point2 < point1)
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
  if(point < range.origin) {
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
    newRange.length = 1;
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
    naMin(range1.origin, range2.origin),
    naMax(naGetRangefEnd(range1), naGetRangefEnd(range2)));
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
  newRange.origin = naMax(range1.origin, range2.origin);
  end1 = naGetRangefEnd(range1);
  end2 = naGetRangefEnd(range2);
  newRange.length  = naMakeLengthWithStartAndEndf(newRange.origin, naMin(end1, end2));
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



NA_IDEF NARect naMakeRectCombination(NAPos point1, NAPos point2) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(point1))
      naError("point1 is invalid.");
    if(!naIsPosValid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newRect.pos.x = point1.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(point1.x, point2.x);
  }else{
    newRect.pos.x = point2.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newRect.pos.y = point1.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(point1.y, point2.y);
  }else{
    newRect.pos.y = point2.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(point2.y, point1.y);
  }
  return newRect;
}
NA_IDEF NARectf naMakeRectfCombination(NAPosf point1, NAPosf point2) {
  NARectf newRect;
  #if NA_DEBUG
    if(!naIsPosfValid(point1))
      naError("point1 is invalid.");
    if(!naIsPosfValid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newRect.pos.x = point1.x;
    newRect.size.width = naMakeLengthWithStartAndEndf(point1.x, point2.x);
  }else{
    newRect.pos.x = point2.x;
    newRect.size.width = naMakeLengthWithStartAndEndf(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newRect.pos.y = point1.y;
    newRect.size.height = naMakeLengthWithStartAndEndf(point1.y, point2.y);
  }else{
    newRect.pos.y = point2.y;
    newRect.size.height = naMakeLengthWithStartAndEndf(point2.y, point1.y);
  }
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32Combination(NAPosi32 point1, NAPosi32 point2) {
  NARecti32 newRect;
  #if NA_DEBUG
    if(!naIsPosi32Valid(point1))
      naError("point1 is invalid.");
    if(!naIsPosi32Valid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newRect.pos.x = point1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi32(point1.x, point2.x);
  }else{
    newRect.pos.x = point2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi32(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newRect.pos.y = point1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi32(point1.y, point2.y);
  }else{
    newRect.pos.y = point2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi32(point2.y, point1.y);
  }
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64Combination(NAPosi64 point1, NAPosi64 point2) {
  NARecti64 newRect;
  #if NA_DEBUG
    if(!naIsPosi64Valid(point1))
      naError("point1 is invalid.");
    if(!naIsPosi64Valid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newRect.pos.x = point1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi64(point1.x, point2.x);
  }else{
    newRect.pos.x = point2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi64(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newRect.pos.y = point1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi64(point1.y, point2.y);
  }else{
    newRect.pos.y = point2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi64(point2.y, point1.y);
  }
  return newRect;
}
NA_IDEF NARects naMakeRectsCombination(NAPoss point1, NAPoss point2) {
  NARects newRect;
  #if NA_DEBUG
    if(!naIsPossValid(point1))
      naError("point1 is invalid.");
    if(!naIsPossValid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newRect.pos.x = point1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxs(point1.x, point2.x);
  }else{
    newRect.pos.x = point2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxs(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newRect.pos.y = point1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxs(point1.y, point2.y);
  }else{
    newRect.pos.y = point2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxs(point2.y, point1.y);
  }
  return newRect;
}



NA_IDEF NARect naMakeRectExtension(NARect rect, NAPos point) {
  NARect newRect;
  double end;
  #if NA_DEBUG
    if(naIsRectEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectUseful(rect))
      naError("rect is not useful.");
    if(!naIsPosValid(point))
      naError("point is invalid.");
  #endif
  newRect.pos.x = naMin(rect.pos.x, point.x);
  newRect.pos.y = naMin(rect.pos.y, point.y);
  end = naGetRectEndX(rect);
  newRect.size.width  = naMakeLengthWithStartAndEnd(newRect.pos.x, naMax(end, point.x));
  end = naGetRectEndY(rect);
  newRect.size.height = naMakeLengthWithStartAndEnd(newRect.pos.y, naMax(end, point.y));
  return newRect;
}
NA_IDEF NARectf naMakeRectfExtension(NARectf rect, NAPosf point) {
  NARectf newRect;
  double end;
  #if NA_DEBUG
    if(naIsRectfEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectfUseful(rect))
      naError("rect is not useful.");
    if(!naIsPosfValid(point))
      naError("point is invalid.");
  #endif
  newRect.pos.x = naMinf(rect.pos.x, point.x);
  newRect.pos.y = naMinf(rect.pos.y, point.y);
  end = naGetRectfEndX(rect);
  newRect.size.width  = naMakeLengthWithStartAndEndf(newRect.pos.x, naMax(end, point.x));
  end = naGetRectfEndY(rect);
  newRect.size.height = naMakeLengthWithStartAndEndf(newRect.pos.y, naMax(end, point.y));
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32Extension(NARecti32 rect, NAPosi32 point) {
  NARecti32 newRect;
  int32 max;
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Useful(rect))
      naError("rect is not useful.");
    if(!naIsPosi32Valid(point))
      naError("point is invalid.");
  #endif
  newRect.pos.x = naMini32(rect.pos.x, point.x);
  newRect.pos.y = naMini32(rect.pos.y, point.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRecti32MaxX(rect);
  newRect.size.width  = naMakeLengthWithMinAndMaxi32(newRect.pos.x, naMaxi32(max, point.x));
  max = naGetRecti32MaxY(rect);
  newRect.size.height = naMakeLengthWithMinAndMaxi32(newRect.pos.y, naMaxi32(max, point.y));
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64Extension(NARecti64 rect, NAPosi64 point) {
  NARecti64 newRect;
  int64 max;
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Useful(rect))
      naError("rect is not useful.");
    if(!naIsPosi64Valid(point))
      naError("point is invalid.");
  #endif
  newRect.pos.x = naMini64(rect.pos.x, point.x);
  newRect.pos.y = naMini64(rect.pos.y, point.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRecti64MaxX(rect);
  newRect.size.width  = naMakeLengthWithMinAndMaxi64(newRect.pos.x, naMaxi64(max, point.x));
  max = naGetRecti64MaxY(rect);
  newRect.size.height = naMakeLengthWithMinAndMaxi64(newRect.pos.y, naMaxi64(max, point.y));
  return newRect;
}
NA_IDEF NARects naMakeRectsExtension(NARects rect, NAPoss point) {
  NARects newRect;
  size_t max;
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsUseful(rect))
      naError("rect is not useful.");
    if(!naIsPossValid(point))
      naError("point is invalid.");
  #endif
  newRect.pos.x = naMins(rect.pos.x, point.x);
  newRect.pos.y = naMins(rect.pos.y, point.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRectsMaxX(rect);
  newRect.size.width  = naMakeLengthWithMinAndMaxs(newRect.pos.x, naMaxs(max, point.x));
  max = naGetRectsMaxY(rect);
  newRect.size.height = naMakeLengthWithMinAndMaxs(newRect.pos.y, naMaxs(max, point.y));
  return newRect;
}



NA_IDEF NARect naMakeRectExtensionE(NARect rect, NAPos point) {
  #if NA_DEBUG
    if(!naIsRectEmpty(rect) && !naIsRectValid(rect))
      naError("rect is invalid.");
    if(!naIsPosValid(point))
      naError("point is invalid.");
  #endif
  
  if(naIsRectEmpty(rect)) {
    NARect newRect;
    newRect.pos = point;
    newRect.size = naMakeSizeZero();
    return newRect;
  }else{
    return naMakeRectExtension(rect, point);
  }
}
NA_IDEF NARectf naMakeRectfExtensionE(NARectf rect, NAPosf point) {
  #if NA_DEBUG
    if(!naIsRectfEmpty(rect) && !naIsRectfValid(rect))
      naError("rect is invalid.");
    if(!naIsPosfValid(point))
      naError("point is invalid.");
  #endif
  
  if(naIsRectfEmpty(rect)) {
    NARectf newRect;
    newRect.pos = point;
    newRect.size = naMakeSizefZero();
    return newRect;
  }else{
    return naMakeRectfExtension(rect, point);
  }
}
NA_IDEF NARecti32 naMakeRecti32ExtensionE(NARecti32 rect, NAPosi32 point) {
  #if NA_DEBUG
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi32Valid(point))
      naError("point is invalid.");
  #endif

  if(naIsRecti32Empty(rect)) {
    NARecti32 newRect;
    newRect.pos = point;
    newRect.size = naMakeSizei32(1, 1);
    return newRect;
  }else{
    return naMakeRecti32Extension(rect, point);
  }
}
NA_IDEF NARecti64 naMakeRecti64ExtensionE(NARecti64 rect, NAPosi64 point) {
  #if NA_DEBUG
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi64Valid(point))
      naError("point is invalid.");
  #endif

  if(naIsRecti64Empty(rect)) {
    NARecti64 newRect;
    newRect.pos = point;
    newRect.size = naMakeSizei64(1, 1);
    return newRect;
  }else{
    return naMakeRecti64Extension(rect, point);
  }
}
NA_IDEF NARects naMakeRectsExtensionE(NARects rect, NAPoss point) {
  #if NA_DEBUG
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
    if(!naIsPossValid(point))
      naError("point is invalid.");
  #endif

  if(naIsRectsEmpty(rect)) {
    NARects newRect;
    newRect.pos = point;
    newRect.size = naMakeSizes(1, 1);
    return newRect;
  }else{
    return naMakeRectsExtension(rect, point);
  }
}



NA_IDEF NARect naMakeRectUnion(NARect rect1, NARect rect2) {
  NARect newRect;
  double end1;
  double end2;
  #if NA_DEBUG
    if(naIsRectEmpty(rect1))
      naError("rect1 is empty.");
    if(!naIsRectValid(rect1))
      naError("rect1 is invalid.");
    if(naIsRectEmpty(rect2))
      naError("rect2 is empty.");
    if(!naIsRectValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMin(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMin(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEnd(newRect.pos.x, naMax(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEnd(newRect.pos.y, naMax(end1, end2));
  return newRect;
}
NA_IDEF NARectf naMakeRectfUnion(NARectf rect1, NARectf rect2) {
  NARectf newRect;
  float end1;
  float end2;
  #if NA_DEBUG
    if(naIsRectfEmpty(rect1))
      naError("rect1 is empty.");
    if(!naIsRectfValid(rect1))
      naError("rect1 is invalid.");
    if(naIsRectfEmpty(rect2))
      naError("rect2 is empty.");
    if(!naIsRectfValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMinf(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMinf(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectfEndX(rect1);
  end2 = naGetRectfEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndf(newRect.pos.x, naMaxf(end1, end2));
  end1 = naGetRectfEndY(rect1);
  end2 = naGetRectfEndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEndf(newRect.pos.y, naMaxf(end1, end2));
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32Union(NARecti32 rect1, NARecti32 rect2) {
  NARecti32 newRect;
  int32 end1;
  int32 end2;
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect1))
      naError("rect1 is empty.");
    if(!naIsRecti32Valid(rect1))
      naError("rect1 is invalid.");
    if(naIsRecti32EmptySlow(rect2))
      naError("rect2 is empty.");
    if(!naIsRecti32Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMini32(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMini32(rect1.pos.y, rect2.pos.y);
  end1 = naGetRecti32EndX(rect1);
  end2 = naGetRecti32EndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndi32(newRect.pos.x, naMaxi32(end1, end2));
  end1 = naGetRecti32EndY(rect1);
  end2 = naGetRecti32EndY(rect2);
  newRect.size.height = naMakeLengthWithStartAndEndi32(newRect.pos.y, naMaxi32(end1, end2));
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64Union(NARecti64 rect1, NARecti64 rect2) {
  NARecti64 newRect;
  int64 end1;
  int64 end2;
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect1))
      naError("rect1 is empty.");
    if(!naIsRecti64Valid(rect1))
      naError("rect1 is invalid.");
    if(naIsRecti64EmptySlow(rect2))
      naError("rect2 is empty.");
    if(!naIsRecti64Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMini64(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMini64(rect1.pos.y, rect2.pos.y);
  end1 = naGetRecti64EndX(rect1);
  end2 = naGetRecti64EndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndi64(newRect.pos.x, naMaxi64(end1, end2));
  end1 = naGetRecti64EndY(rect1);
  end2 = naGetRecti64EndY(rect2);
  newRect.size.height = naMakeLengthWithStartAndEndi64(newRect.pos.y, naMaxi64(end1, end2));
  return newRect;
}
NA_IDEF NARects naMakeRectsUnion(NARects rect1, NARects rect2) {
  NARects newRect;
  size_t end1;
  size_t end2;
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect1))
      naError("rect1 is empty.");
    if(!naIsRectsValid(rect1))
      naError("rect1 is invalid.");
    if(naIsRectsEmptySlow(rect2))
      naError("rect2 is empty.");
    if(!naIsRectsValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMins(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMins(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectsEndX(rect1);
  end2 = naGetRectsEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEnds(newRect.pos.x, naMaxs(end1, end2));
  end1 = naGetRectsEndY(rect1);
  end2 = naGetRectsEndY(rect2);
  newRect.size.height = naMakeLengthWithStartAndEnds(newRect.pos.y, naMaxs(end1, end2));
  return newRect;
}



NA_IDEF NARect naMakeRectUnionE(NARect rect1, NARect rect2) {
  #if NA_DEBUG
    if(!naIsRectEmpty(rect1) && !naIsRectValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectEmpty(rect2) && !naIsRectValid(rect2))
      naError("rect2 is invalid.");
  #endif
  
  if(naIsRectEmpty(rect1))
    return rect2;
  if(naIsRectEmpty(rect2))
    return rect1;
  
  return naMakeRectUnion(rect1, rect2);
}
NA_IDEF NARectf naMakeRectfUnionE(NARectf rect1, NARectf rect2) {
  #if NA_DEBUG
    if(!naIsRectfEmpty(rect1) && !naIsRectfValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectfEmpty(rect2) && !naIsRectfValid(rect2))
      naError("rect2 is invalid.");
  #endif
  
  if(naIsRectfEmpty(rect1))
    return rect2;
  if(naIsRectfEmpty(rect2))
    return rect1;
  
  return naMakeRectfUnion(rect1, rect2);
}
NA_IDEF NARecti32 naMakeRecti32UnionE(NARecti32 rect1, NARecti32 rect2) {
  #if NA_DEBUG
    if(!naIsRecti32Valid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRecti32Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  
  if(naIsRecti32Empty(rect1))
    return rect2;
  if(naIsRecti32Empty(rect2))
    return rect1;
  
  return naMakeRecti32Union(rect1, rect2);
}
NA_IDEF NARecti64 naMakeRecti64UnionE(NARecti64 rect1, NARecti64 rect2) {
  #if NA_DEBUG
    if(!naIsRecti64Valid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRecti64Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  
  if(naIsRecti64Empty(rect1))
    return rect2;
  if(naIsRecti64Empty(rect2))
    return rect1;
  
  return naMakeRecti64Union(rect1, rect2);
}
NA_IDEF NARects naMakeRectsUnionE(NARects rect1, NARects rect2) {
  #if NA_DEBUG
    if(!naIsRectsValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectsValid(rect2))
      naError("rect2 is invalid.");
  #endif
  
  if(naIsRectsEmpty(rect1))
    return rect2;
  if(naIsRectsEmpty(rect2))
    return rect1;
  
  return naMakeRectsUnion(rect1, rect2);
}



NA_IDEF NARect naMakeRectIntersection(NARect rect1, NARect rect2) {
  NARect newRect;
  double end1;
  double end2;
  #if NA_DEBUG
    if(!naIsRectValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMax(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMax(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndE(newRect.pos.x, naMin(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEndE(newRect.pos.y, naMin(end1, end2));
  return newRect;
}
NA_IDEF NARectf naMakeRectfIntersection(NARectf rect1, NARectf rect2) {
  NARectf newRect;
  float end1;
  float end2;
  #if NA_DEBUG
    if(!naIsRectfValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectfValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMaxf(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMaxf(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectfEndX(rect1);
  end2 = naGetRectfEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndfE(newRect.pos.x, naMinf(end1, end2));
  end1 = naGetRectfEndY(rect1);
  end2 = naGetRectfEndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEndfE(newRect.pos.y, naMinf(end1, end2));
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32Intersection(NARecti32 rect1, NARecti32 rect2) {
  NARecti32 newRect;
  int32 end1;
  int32 end2;
  #if NA_DEBUG
    if(!naIsRecti32Valid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRecti32Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMaxi32(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMaxi32(rect1.pos.y, rect2.pos.y);
  end1 = naGetRecti32EndX(rect1);
  end2 = naGetRecti32EndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndi32E(newRect.pos.x, naMini32(end1, end2));
  end1 = naGetRecti32EndY(rect1);
  end2 = naGetRecti32EndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEndi32E(newRect.pos.y, naMini32(end1, end2));
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64Intersection(NARecti64 rect1, NARecti64 rect2) {
  NARecti64 newRect;
  int64 end1;
  int64 end2;
  #if NA_DEBUG
    if(!naIsRecti64Valid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRecti64Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMaxi64(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMaxi64(rect1.pos.y, rect2.pos.y);
  end1 = naGetRecti64EndX(rect1);
  end2 = naGetRecti64EndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndi64E(newRect.pos.x, naMini64(end1, end2));
  end1 = naGetRecti64EndY(rect1);
  end2 = naGetRecti64EndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEndi64E(newRect.pos.y, naMini64(end1, end2));
  return newRect;
}
NA_IDEF NARects naMakeRectsIntersection(NARects rect1, NARects rect2) {
  NARects newRect;
  size_t end1;
  size_t end2;
  #if NA_DEBUG
    if(!naIsRectsValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectsValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMaxs(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMaxs(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectsEndX(rect1);
  end2 = naGetRectsEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndsE(newRect.pos.x, naMins(end1, end2));
  end1 = naGetRectsEndY(rect1);
  end2 = naGetRectsEndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEndsE(newRect.pos.y, naMins(end1, end2));
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
