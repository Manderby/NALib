
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NABool naContainsRangePoints(NARange outerRange, double point) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(point))
      naError("point is invalid.");
    if(!naIsRangeUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((point < outerRange.origin)
        || (point > naGetRangeEnd(outerRange)));
}
NA_IDEF NABool naContainsRangefPoint(NARangef outerRange, float point) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(point))
      naError("point is invalid.");
    if(!naIsRangefUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((point < outerRange.origin)
        || (point > naGetRangefEnd(outerRange)));
}
NA_IDEF NABool naContainsRangei32Point(NARangei32 outerRange, int32 point) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(point))
      naError("point is invalid.");
    if(!naIsRangei32Useful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((point < outerRange.origin)
        || (point > naGetRangei32Max(outerRange)));
}
NA_IDEF NABool naContainsRangei64Point(NARangei64 outerRange, int64 point) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(point))
      naError("point is invalid.");
    if(!naIsRangei64Useful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !(naSmalleri64(point, outerRange.origin)
        || naGreateri64(point, naGetRangei64Max(outerRange)));
}
NA_IDEF NABool naContainsRangesPoint(NARanges outerRange, size_t point) {
  #if NA_DEBUG
    if(!naIsOffsetValueValids(point))
      naError("point is invalid.");
    if(!naIsRangesUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((point < outerRange.origin)
        || (point > naGetRangesMax(outerRange)));
}



NA_IDEF NABool naContainsRangeRange(NARange outerRange, NARange range) {
  #if NA_DEBUG
    if(!naIsRangeUseful(range))
      naError("Inside test not valid if range is not useful.");
    if(!naIsRangeUseful(outerRange))
      naError("Inside test not valid if outerRange is not useful.");
  #endif
  return  ((range.origin           >= outerRange.origin)
        && (naGetRangeEnd(range) <=  naGetRangeEnd(outerRange)));
}
NA_IDEF NABool naContainsRangefRange(NARangef outerRange, NARangef range) {
  #if NA_DEBUG
    if(!naIsRangefUseful(range))
      naError("Inside test not valid if range is not useful.");
    if(!naIsRangefUseful(outerRange))
      naError("Inside test not valid if outerRange is not useful.");
  #endif
  return  ((range.origin           >= outerRange.origin)
        && (naGetRangefEnd(range) <=  naGetRangefEnd(outerRange)));
}
NA_IDEF NABool naContainsRangei32Range(NARangei32 outerRange, NARangei32 range) {
  #if NA_DEBUG
    if(!naIsRangei32Useful(range))
      naError("Inside test not valid if range is not useful.");
    if(!naIsRangei32Useful(outerRange))
      naError("Inside test not valid if outerRange is not useful.");
  #endif
  return  ((range.origin           >= outerRange.origin)
        && (naGetRangei32End(range) <=  naGetRangei32End(outerRange)));
}
NA_IDEF NABool naContainsRangei64Range(NARangei64 outerRange, NARangei64 range) {
  #if NA_DEBUG
    if(!naIsRangei64Useful(range))
      naError("Inside test not valid if range is not useful.");
    if(!naIsRangei64Useful(outerRange))
      naError("Inside test not valid if outerRange is not useful.");
  #endif
  return  (naGreaterEquali64(range.origin, outerRange.origin)
        && naSmallerEquali64(naGetRangei64End(range), naGetRangei64End(outerRange)));
}
NA_IDEF NABool naContainsRangesRange(NARanges outerRange, NARanges range) {
  #if NA_DEBUG
    if(!naIsRangesUseful(range))
      naError("Inside test not valid if range is not useful.");
    if(!naIsRangesUseful(outerRange))
      naError("Inside test not valid if outerRange is not useful.");
  #endif
  return  ((range.origin           >= outerRange.origin)
        && (naGetRangesEnd(range) <=  naGetRangesEnd(outerRange)));
}




NA_IDEF NABool naContainsSizePoint(NASize outerSize, NAPos point) {
  #if NA_DEBUG
    if(!naIsPosValid(point))
      naError("point is invalid.");
    if(!naIsSizeUseful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  return !((point.x < 0.)
        || (point.x > outerSize.width)
        || (point.y < 0.)
        || (point.y > outerSize.height));
}
NA_IDEF NABool naContainsSizefPoint(NASizef outerSize, NAPosf point) {
  #if NA_DEBUG
    if(!naIsPosfValid(point))
      naError("point is invalid.");
    if(!naIsSizefUseful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  return !((point.x < 0.f)
        || (point.x > outerSize.width)
        || (point.y < 0.f)
        || (point.y > outerSize.height));
}
NA_IDEF NABool naContainsSizei32Point(NASizei32 outerSize, NAPosi32 point) {
  #if NA_DEBUG
    if(!naIsPosi32Valid(point))
      naError("point is invalid.");
    if(naIsSizei32Useful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  return  ((point.x >= 0)
        && (point.x <  outerSize.width)
        && (point.y >= 0)
        && (point.y <  outerSize.height));
}
NA_IDEF NABool naContainsSizei64Point(NASizei64 outerSize, NAPosi64 point) {
  #if NA_DEBUG
    if(!naIsPosi64Valid(point))
      naError("point is invalid.");
    if(naIsSizei64Useful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  return  (naGreaterEquali64(point.x, NA_ZERO_i64)
        && naSmalleri64(point.x, outerSize.width)
        && naGreaterEquali64(point.y, NA_ZERO_i64)
        && naSmalleri64(point.y, outerSize.height));
}
NA_IDEF NABool naContainsSizesPoint(NASizes outerSize, NAPoss point) {
  #if NA_DEBUG
    if(!naIsPossValid(point))
      naError("point is invalid.");
    if(naIsSizesUseful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  // No need to check for >= 0 as this is unsigned.
  return  ((point.x <  outerSize.width)
        && (point.y <  outerSize.height));
}



NA_IDEF NABool naContainsSizeSize(NASize outerSize, NASize size) {
  #if NA_DEBUG
    if(!naIsSizeUseful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizeUseful(outerSize))
      naError("Inside test not valid if outerSize is not useful.");
  #endif
  return !((size.width  > outerSize.width)
        || (size.height > outerSize.height));
}
NA_IDEF NABool naContainsSizefSize(NASizef outerSize, NASizef size) {
  #if NA_DEBUG
    if(!naIsSizefUseful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizefUseful(outerSize))
      naError("Inside test not valid if outerSize is not useful.");
  #endif
  return !((size.width  > outerSize.width)
        || (size.height > outerSize.height));
}
NA_IDEF NABool naContainsSizei32Size(NASizei32 outerSize, NASizei32 size) {
  #if NA_DEBUG
    if(!naIsSizei32Useful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizei32Useful(outerSize))
      naError("Inside test not valid if outerSize is not useful.");
  #endif
  return !((size.width  > outerSize.width)
        || (size.height > outerSize.height));
}
NA_IDEF NABool naContainsSizei64Size(NASizei64 outerSize, NASizei64 size) {
  #if NA_DEBUG
    if(!naIsSizei64Useful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizei64Useful(outerSize))
      naError("Inside test not valid if outerSize is not useful.");
  #endif
  return !(naGreateri64(size.width, outerSize.width)
        || naGreateri64(size.height, outerSize.height));
}
NA_IDEF NABool naContainsSizesSize(NASizes outerSize, NASizes size) {
  #if NA_DEBUG
    if(!naIsSizesUseful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizesUseful(outerSize))
      naError("Inside test not valid if outerSize is not useful.");
  #endif
  return !((size.width  > outerSize.width)
        || (size.height > outerSize.height));
}



NA_IDEF NABool naContainsRectPoint(NARect outerRect, NAPos point) {
  #if NA_DEBUG
    if(!naIsPosValid(point))
      naError("point is invalid.");
    if(naIsRectEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((point.x < outerRect.pos.x)
        || (point.x > naGetRectEndX(outerRect))
        || (point.y < outerRect.pos.y)
        || (point.y > naGetRectEndY(outerRect)));
}
NA_IDEF NABool naContainsRectfPoint(NARectf outerRect, NAPosf point) {
  #if NA_DEBUG
    if(!naIsPosfValid(point))
      naError("point is invalid.");
    if(naIsRectfEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectfValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((point.x < outerRect.pos.x)
        || (point.x > naGetRectfEndX(outerRect))
        || (point.y < outerRect.pos.y)
        || (point.y > naGetRectfEndY(outerRect)));
}
NA_IDEF NABool naContainsRecti32Point(NARecti32 outerRect, NAPosi32 point) {
  #if NA_DEBUG
    if(!naIsPosi32Valid(point))
      naError("point is invalid.");
    if(naIsRecti32EmptySlow(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti32Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((point.x >= outerRect.pos.x)
        && (point.x <  naGetRecti32EndX(outerRect))
        && (point.y >= outerRect.pos.y)
        && (point.y <  naGetRecti32EndY(outerRect)));
}
NA_IDEF NABool naContainsRecti64Point(NARecti64 outerRect, NAPosi64 point) {
  #if NA_DEBUG
    if(!naIsPosi64Valid(point))
      naError("point is invalid.");
    if(naIsRecti64EmptySlow(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti64Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  (naGreaterEquali64(point.x, outerRect.pos.x)
        && naSmalleri64(point.x, naGetRecti64EndX(outerRect))
        && naGreaterEquali64(point.y, outerRect.pos.y)
        && naSmalleri64(point.y, naGetRecti64EndY(outerRect)));
}
NA_IDEF NABool naContainsRectsPoint(NARects outerRect, NAPoss point) {
  #if NA_DEBUG
    if(!naIsPossValid(point))
      naError("point is invalid.");
    if(naIsRectsEmptySlow(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectsValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((point.x >= outerRect.pos.x)
        && (point.x <  naGetRectsEndX(outerRect))
        && (point.y >= outerRect.pos.y)
        && (point.y <  naGetRectsEndY(outerRect)));
}



NA_IDEF NABool naContainsRectPointE(NARect outerRect, NAPos pos) {
  #if NA_DEBUG
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
    if(naIsRectEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((pos.x < outerRect.pos.x)
        || (pos.x >= naGetRectEndX(outerRect))
        || (pos.y < outerRect.pos.y)
        || (pos.y >= naGetRectEndY(outerRect)));
}
NA_IDEF NABool naContainsRectfPointE(NARectf outerRect, NAPosf pos) {
  #if NA_DEBUG
    if(!naIsPosfValid(pos))
      naError("pos is invalid.");
    if(naIsRectfEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectfValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((pos.x < outerRect.pos.x)
        || (pos.x >= naGetRectfEndX(outerRect))
        || (pos.y < outerRect.pos.y)
        || (pos.y >= naGetRectfEndY(outerRect)));
}
NA_IDEF NABool naContainsRecti32PointE(NARecti32 outerRect, NAPosi32 pos) {
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
    if(naIsRecti32Empty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti32Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((pos.x < outerRect.pos.x)
        || (pos.x >= naGetRecti32EndX(outerRect))
        || (pos.y < outerRect.pos.y)
        || (pos.y >= naGetRecti32EndY(outerRect)));
}
NA_IDEF NABool naContainsRecti64PointE(NARecti64 outerRect, NAPosi64 pos) {
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
    if(naIsRecti64Empty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti64Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !(naSmalleri64(pos.x, outerRect.pos.x)
        || naGreaterEquali64(pos.x, naGetRecti64EndX(outerRect))
        || naSmalleri64(pos.y, outerRect.pos.y)
        || naGreaterEquali64(pos.y, naGetRecti64EndY(outerRect)));
}
NA_IDEF NABool naContainsRectsPointE(NARects outerRect, NAPoss pos) {
  #if NA_DEBUG
    if(!naIsPossValid(pos))
      naError("pos is invalid.");
    if(naIsRectsEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectsValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((pos.x < outerRect.pos.x)
        || (pos.x >= naGetRectsEndX(outerRect))
        || (pos.y < outerRect.pos.y)
        || (pos.y >= naGetRectsEndY(outerRect)));
}



NA_IDEF NABool naContainsRectRect(NARect outerRect, NARect rect) {
  #if NA_DEBUG
    if(naIsRectEmpty(rect) || naIsRectEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectValid(rect))
      naError("rect is invalid.");
    if(!naIsRectValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((rect.pos.x          < outerRect.pos.x)
        || (naGetRectEndX(rect) > naGetRectEndX(outerRect))
        || (rect.pos.y          < outerRect.pos.y)
        || (naGetRectEndY(rect) > naGetRectEndY(outerRect)));
}
NA_IDEF NABool naContainsfRectRect(NARectf outerRect, NARectf rect) {
  #if NA_DEBUG
    if(naIsRectfEmpty(rect) || naIsRectfEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectfValid(rect))
      naError("rect is invalid.");
    if(!naIsRectfValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((rect.pos.x          < outerRect.pos.x)
        || (naGetRectfEndX(rect) > naGetRectfEndX(outerRect))
        || (rect.pos.y          < outerRect.pos.y)
        || (naGetRectfEndY(rect) > naGetRectfEndY(outerRect)));
}
NA_IDEF NABool naContainsRecti32Rect(NARecti32 outerRect, NARecti32 rect) {
  #if NA_DEBUG
    if(naIsRecti32Empty(rect) || naIsRecti32Empty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
    if(!naIsRecti32Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((rect.pos.x           >= outerRect.pos.x)
        && (naGetRecti32EndX(rect) <=  naGetRecti32EndX(outerRect))
        && (rect.pos.y           >= outerRect.pos.y)
        && (naGetRecti32EndY(rect) <=  naGetRecti32EndY(outerRect)));
}
NA_IDEF NABool naContainsRecti64Rect(NARecti64 outerRect, NARecti64 rect) {
  #if NA_DEBUG
    if(naIsRecti64Empty(rect) || naIsRecti64Empty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
    if(!naIsRecti64Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  (naGreaterEquali64(rect.pos.x, outerRect.pos.x)
        && naSmallerEquali64(naGetRecti64EndX(rect),  naGetRecti64EndX(outerRect))
        && naGreaterEquali64(rect.pos.y, outerRect.pos.y)
        && naSmallerEquali64(naGetRecti64EndY(rect),naGetRecti64EndY(outerRect)));
}
NA_IDEF NABool naContainsRectsRect(NARects outerRect, NARects rect) {
  #if NA_DEBUG
    if(naIsRectsEmpty(rect) || naIsRectsEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
    if(!naIsRectsValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((rect.pos.x           >= outerRect.pos.x)
        && (naGetRectsEndX(rect) <=  naGetRectsEndX(outerRect))
        && (rect.pos.y           >= outerRect.pos.y)
        && (naGetRectsEndY(rect) <=  naGetRectsEndY(outerRect)));
}



NA_IDEF NABool naContainsVolumePoint(NAVolume outerVolume, NAVertex point) {
  #if NA_DEBUG
    if(!naIsVertexValid(point))
      naError("point is invalid.");
    if(!naIsVolumeUseful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  return !((point.x < 0)
        || (point.x > outerVolume.width)
        || (point.y < 0)
        || (point.y > outerVolume.height)
        || (point.z < 0)
        || (point.z > outerVolume.depth));
}
NA_IDEF NABool naContainsVolumefPoint(NAVolumef outerVolume, NAVertexf point) {
  #if NA_DEBUG
    if(!naIsVertexfValid(point))
      naError("point is invalid.");
    if(!naIsVolumefUseful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  return !((point.x < 0)
        || (point.x > outerVolume.width)
        || (point.y < 0)
        || (point.y > outerVolume.height)
        || (point.z < 0)
        || (point.z > outerVolume.depth));
}
NA_IDEF NABool naContainsVolumei32Point(NAVolumei32 outerVolume, NAVertexi32 point) {
  #if NA_DEBUG
    if(!naIsVertexi32Valid(point))
      naError("point is invalid.");
    if(naIsVolumei32Useful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  return  ((point.x >= 0)
        && (point.x <  outerVolume.width)
        && (point.y >= 0)
        && (point.y <  outerVolume.height)
        && (point.z >= 0)
        && (point.z <  outerVolume.depth));
}
NA_IDEF NABool naContainsVolumei64Point(NAVolumei64 outerVolume, NAVertexi64 point) {
  #if NA_DEBUG
    if(!naIsVertexi64Valid(point))
      naError("point is invalid.");
    if(naIsVolumei64Useful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  return  (naGreaterEquali64(point.x, NA_ZERO_i64)
        && naSmalleri64(point.x,outerVolume.width)
        && naGreaterEquali64(point.y, NA_ZERO_i64)
        && naSmalleri64(point.y, outerVolume.height)
        && naGreaterEquali64(point.z, NA_ZERO_i64)
        && naSmalleri64(point.z, outerVolume.depth));
}
NA_IDEF NABool naContainsVolumesPoint(NAVolumes outerVolume, NAVertexs point) {
  #if NA_DEBUG
    if(!naIsVertexsValid(point))
      naError("point is invalid.");
    if(naIsVolumesUseful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  // No need to check for >= 0 as this is unsigned.
  return  ((point.x <  outerVolume.width)
        && (point.y <  outerVolume.height)
        && (point.z <  outerVolume.depth));
}



NA_IDEF NABool naContainsVolumeVolume(NAVolume outerVolume, NAVolume volume) {
  #if NA_DEBUG
    if(!naIsVolumeUseful(volume))
      naError("Inside test not valid if volume is not useful.");
    if(!naIsVolumeUseful(outerVolume))
      naError("Inside test not valid if outerVolume is not useful.");
  #endif
  return !((volume.width  > outerVolume.width)
        || (volume.height > outerVolume.height)
        || (volume.depth  > outerVolume.depth));
}
NA_IDEF NABool naContainsVolumefVolume(NAVolumef outerVolume, NAVolumef volume) {
  #if NA_DEBUG
    if(!naIsVolumefUseful(volume))
      naError("Inside test not valid if volume is not useful.");
    if(!naIsVolumefUseful(outerVolume))
      naError("Inside test not valid if outerVolume is not useful.");
  #endif
  return !((volume.width  > outerVolume.width)
        || (volume.height > outerVolume.height)
        || (volume.depth  > outerVolume.depth));
}
NA_IDEF NABool naContainsVolumei32Volume(NAVolumei32 outerVolume, NAVolumei32 volume) {
  #if NA_DEBUG
    if(!naIsVolumei32Useful(volume))
      naError("Inside test not valid if volume is not useful.");
    if(!naIsVolumei32Useful(outerVolume))
      naError("Inside test not valid if outerVolume is not useful.");
  #endif
  return !((volume.width  > outerVolume.width)
        || (volume.height > outerVolume.height)
        || (volume.depth  > outerVolume.depth));
}
NA_IDEF NABool naContainsVolumei64Volume(NAVolumei64 outerVolume, NAVolumei64 volume) {
  #if NA_DEBUG
    if(!naIsVolumei64Useful(volume))
      naError("Inside test not valid if volume is not useful.");
    if(!naIsVolumei64Useful(outerVolume))
      naError("Inside test not valid if outerVolume is not useful.");
  #endif
  return !(naGreateri64(volume.width, outerVolume.width)
        || naGreateri64(volume.height, outerVolume.height)
        || naGreateri64(volume.depth, outerVolume.depth));
}
NA_IDEF NABool naContainsVolumesVolume(NAVolumes outerVolume, NAVolumes volume) {
  #if NA_DEBUG
    if(!naIsVolumesUseful(volume))
      naError("Inside test not valid if volume is not useful.");
    if(!naIsVolumesUseful(outerVolume))
      naError("Inside test not valid if outerVolume is not useful.");
  #endif
  return !((volume.width  > outerVolume.width)
        || (volume.height > outerVolume.height)
        || (volume.depth  > outerVolume.depth));
}



NA_IDEF NABool naContainsBoxPoint(NABox outerBox, NAVertex point) {
  #if NA_DEBUG
    if(!naIsVertexValid(point))
      naError("point is invalid.");
    if(naIsBoxEmpty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((point.x < outerBox.vertex.x)
        || (point.x > naGetBoxEndX(outerBox))
        || (point.y < outerBox.vertex.y)
        || (point.y > naGetBoxEndY(outerBox))
        || (point.z < outerBox.vertex.z)
        || (point.z > naGetBoxEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxfPoint(NABoxf outerBox, NAVertexf point) {
  #if NA_DEBUG
    if(!naIsVertexfValid(point))
      naError("point is invalid.");
    if(naIsBoxfEmpty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxfValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((point.x < outerBox.vertex.x)
        || (point.x > naGetBoxfEndX(outerBox))
        || (point.y < outerBox.vertex.y)
        || (point.y > naGetBoxfEndY(outerBox))
        || (point.z < outerBox.vertex.z)
        || (point.z > naGetBoxfEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi32Point(NABoxi32 outerBox, NAVertexi32 point) {
  #if NA_DEBUG
    if(!naIsVertexi32Valid(point))
      naError("point is invalid.");
    if(naIsBoxi32EmptySlow(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxi32Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((point.x >= outerBox.vertex.x)
        && (point.x <  naGetBoxi32EndX(outerBox))
        && (point.y >= outerBox.vertex.y)
        && (point.y <  naGetBoxi32EndY(outerBox))
        && (point.z >= outerBox.vertex.z)
        && (point.z <  naGetBoxi32EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi64Point(NABoxi64 outerBox, NAVertexi64 point) {
  #if NA_DEBUG
    if(!naIsVertexi64Valid(point))
      naError("point is invalid.");
    if(naIsBoxi64EmptySlow(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxi64Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  (naGreaterEquali64(point.x, outerBox.vertex.x)
        && naSmalleri64(point.x, naGetBoxi64EndX(outerBox))
        && naGreaterEquali64(point.y, outerBox.vertex.y)
        && naSmalleri64(point.y, naGetBoxi64EndY(outerBox))
        && naGreaterEquali64(point.z, outerBox.vertex.z)
        && naSmalleri64(point.z, naGetBoxi64EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxsPoint(NABoxs outerBox, NAVertexs point) {
  #if NA_DEBUG
    if(!naIsVertexsValid(point))
      naError("point is invalid.");
    if(naIsBoxsEmptySlow(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxsValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((point.x >= outerBox.vertex.x)
        && (point.x <  naGetBoxsEndX(outerBox))
        && (point.y >= outerBox.vertex.y)
        && (point.y <  naGetBoxsEndY(outerBox))
        && (point.z >= outerBox.vertex.z)
        && (point.z <  naGetBoxsEndZ(outerBox)));
}



NA_IDEF NABool naContainsBoxPointE(NABox outerBox, NAVertex point) {
  #if NA_DEBUG
    if(!naIsVertexValid(point))
      naError("point is invalid.");
    if(naIsBoxEmpty(outerBox))
      naError("Inside test not valid for empty rects.");
    if(!naIsBoxValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((point.x < outerBox.vertex.x)
        || (point.x >= naGetBoxEndX(outerBox))
        || (point.y < outerBox.vertex.y)
        || (point.y >= naGetBoxEndY(outerBox))
        || (point.z < outerBox.vertex.z)
        || (point.z >= naGetBoxEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxfPointE(NABoxf outerBox, NAVertexf point) {
  #if NA_DEBUG
    if(!naIsVertexfValid(point))
      naError("point is invalid.");
    if(naIsBoxfEmpty(outerBox))
      naError("Inside test not valid for empty rects.");
    if(!naIsBoxfValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((point.x < outerBox.vertex.x)
        || (point.x >= naGetBoxfEndX(outerBox))
        || (point.y < outerBox.vertex.y)
        || (point.y >= naGetBoxfEndY(outerBox))
        || (point.z < outerBox.vertex.z)
        || (point.z >= naGetBoxfEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi32PointE(NABoxi32 outerBox, NAVertexi32 point) {
  #if NA_DEBUG
    if(!naIsVertexi32Valid(point))
      naError("point is invalid.");
    if(naIsBoxi32Empty(outerBox))
      naError("Inside test not valid for empty rects.");
    if(!naIsBoxi32Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((point.x < outerBox.vertex.x)
        || (point.x >= naGetBoxi32EndX(outerBox))
        || (point.y < outerBox.vertex.y)
        || (point.y >= naGetBoxi32EndY(outerBox))
        || (point.z < outerBox.vertex.z)
        || (point.z >= naGetBoxi32EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi64PointE(NABoxi64 outerBox, NAVertexi64 point) {
  #if NA_DEBUG
    if(!naIsVertexi64Valid(point))
      naError("point is invalid.");
    if(naIsBoxi64Empty(outerBox))
      naError("Inside test not valid for empty rects.");
    if(!naIsBoxi64Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !(naSmalleri64(point.x, outerBox.vertex.x)
        || naGreaterEquali64(point.x, naGetBoxi64EndX(outerBox))
        || naSmalleri64(point.y, outerBox.vertex.y)
        || naGreaterEquali64(point.y, naGetBoxi64EndY(outerBox))
        || naSmalleri64(point.z, outerBox.vertex.z)
        || naGreaterEquali64(point.z, naGetBoxi64EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxsPointE(NABoxs outerBox, NAVertexs point) {
  #if NA_DEBUG
    if(!naIsVertexsValid(point))
      naError("point is invalid.");
    if(naIsBoxsEmpty(outerBox))
      naError("Inside test not valid for empty rects.");
    if(!naIsBoxsValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((point.x < outerBox.vertex.x)
        || (point.x >= naGetBoxsEndX(outerBox))
        || (point.y < outerBox.vertex.y)
        || (point.y >= naGetBoxsEndY(outerBox))
        || (point.z < outerBox.vertex.z)
        || (point.z >= naGetBoxsEndZ(outerBox)));
}



NA_IDEF NABool naContainsBoxBox(NABox outerBox, NABox box) {
  #if NA_DEBUG
    if(naIsBoxEmpty(box) || naIsBoxEmpty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxValid(box))
      naError("box is invalid.");
    if(!naIsBoxValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((box.vertex.x          < outerBox.vertex.x)
        || (naGetBoxEndX(box) > naGetBoxEndX(outerBox))
        || (box.vertex.y          < outerBox.vertex.y)
        || (naGetBoxEndY(box) > naGetBoxEndY(outerBox))
        || (box.vertex.z          < outerBox.vertex.z)
        || (naGetBoxEndZ(box) > naGetBoxEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxfBox(NABoxf outerBox, NABoxf box) {
  #if NA_DEBUG
    if(naIsBoxfEmpty(box) || naIsBoxfEmpty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxfValid(box))
      naError("box is invalid.");
    if(!naIsBoxfValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((box.vertex.x          < outerBox.vertex.x)
        || (naGetBoxfEndX(box) > naGetBoxfEndX(outerBox))
        || (box.vertex.y          < outerBox.vertex.y)
        || (naGetBoxfEndY(box) > naGetBoxfEndY(outerBox))
        || (box.vertex.z          < outerBox.vertex.z)
        || (naGetBoxfEndZ(box) > naGetBoxfEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi32Box(NABoxi32 outerBox, NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32Empty(box) || naIsBoxi32Empty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
    if(!naIsBoxi32Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((box.vertex.x           >= outerBox.vertex.x)
        && (naGetBoxi32EndX(box) <=  naGetBoxi32EndX(outerBox))
        && (box.vertex.y           >= outerBox.vertex.y)
        && (naGetBoxi32EndY(box) <=  naGetBoxi32EndY(outerBox))
        && (box.vertex.z           >= outerBox.vertex.z)
        && (naGetBoxi32EndZ(box) <=  naGetBoxi32EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi64Box(NABoxi64 outerBox, NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64Empty(box) || naIsBoxi64Empty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
    if(!naIsBoxi64Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  (naGreaterEquali64(box.vertex.x, outerBox.vertex.x)
        && naSmallerEquali64(naGetBoxi64EndX(box), naGetBoxi64EndX(outerBox))
        && naGreaterEquali64(box.vertex.y, outerBox.vertex.y)
        && naSmallerEquali64(naGetBoxi64EndY(box), naGetBoxi64EndY(outerBox))
        && naGreaterEquali64(box.vertex.z, outerBox.vertex.z)
        && naSmallerEquali64(naGetBoxi64EndZ(box), naGetBoxi64EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxsBox(NABoxs outerBox, NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmpty(box) || naIsBoxsEmpty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
    if(!naIsBoxsValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((box.vertex.x           >= outerBox.vertex.x)
        && (naGetBoxsEndX(box) <=  naGetBoxsEndX(outerBox))
        && (box.vertex.y           >= outerBox.vertex.y)
        && (naGetBoxsEndY(box) <=  naGetBoxsEndY(outerBox))
        && (box.vertex.z           >= outerBox.vertex.z)
        && (naGetBoxsEndZ(box) <=  naGetBoxsEndZ(outerBox)));
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
