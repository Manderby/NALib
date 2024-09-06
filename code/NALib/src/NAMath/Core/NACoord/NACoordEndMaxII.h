
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF double naGetRangeEnd(NARange range) {
  #if NA_DEBUG
    if(naIsRangeEmpty(range))
      naError("range is empty.");
    if(!naIsRangeValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLength(range.origin, range.length);
}
NA_IDEF float naGetRangefEnd(NARangef range) {
  #if NA_DEBUG
    if(naIsRangefEmpty(range))
      naError("range is empty.");
    if(!naIsRangefValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthf(range.origin, range.length);
}
NA_IDEF int32 naGetRangei32End(NARangei32 range) {
  #if NA_DEBUG
    if(naIsRangei32Empty(range))
      naError("range is empty.");
    if(!naIsRangei32Valid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi32(range.origin, range.length);
}
NA_IDEF int64 naGetRangei64End(NARangei64 range) {
  #if NA_DEBUG
    if(naIsRangei64Empty(range))
      naError("range is empty.");
    if(!naIsRangei64Valid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi64(range.origin, range.length);
}
NA_IDEF size_t naGetRangesEnd(NARanges range) {
  #if NA_DEBUG
    if(naIsRangesEmpty(range))
      naError("range is empty.");
    if(!naIsRangesValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengths(range.origin, range.length);
}



NA_IDEF double naGetRangeEndE(NARange range) {
  #if NA_DEBUG
    if(!naIsRangeValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLength(range.origin, range.length);
}
NA_IDEF float naGetRangefEndE(NARangef range) {
  #if NA_DEBUG
    if(!naIsRangefValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthf(range.origin, range.length);
}
NA_IDEF int32 naGetRangei32EndE(NARangei32 range) {
  #if NA_DEBUG
    if(!naIsRangei32Valid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi32(range.origin, range.length);
}
NA_IDEF int64 naGetRangeEndi64E(NARangei64 range) {
  #if NA_DEBUG
    if(!naIsRangei64Valid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi64(range.origin, range.length);
}
NA_IDEF size_t naGetRangesEndE(NARanges range) {
  #if NA_DEBUG
    if(!naIsRangesValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengths(range.origin, range.length);
}



NA_IDEF int32 naGetRangei32Max(NARangei32 range) {
  #if NA_DEBUG
    if(naIsRangei32Empty(range))
      naError("range is empty.");
    if(!naIsRangei32Valid(range))
      naError("range is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi32(range.origin, range.length);
}
NA_IDEF int64 naGetRangei64Max(NARangei64 range) {
  #if NA_DEBUG
    if(naIsRangei64Empty(range))
      naError("range is empty.");
    if(!naIsRangei64Valid(range))
      naError("range is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi64(range.origin, range.length);
}
NA_IDEF size_t naGetRangesMax(NARanges range) {
  #if NA_DEBUG
    if(naIsRangesEmpty(range))
      naError("range is empty.");
    if(!naIsRangesValid(range))
      naError("range is invalid.");
  #endif
  return naMakeMaxWithMinAndLengths(range.origin, range.length);
}



NA_IDEF NAPos naGetRectEnd(NARect rect) {
  #if NA_DEBUG
    if(naIsRectEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePos(naGetRectEndX(rect), naGetRectEndY(rect));
}
NA_IDEF NAPosf naGetRectfEnd(NARectf rect) {
  #if NA_DEBUG
    if(naIsRectfEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectfValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePosf(naGetRectfEndX(rect), naGetRectfEndY(rect));
}
NA_IDEF NAPosi32 naGetRecti32End(NARecti32 rect) {
  #if NA_DEBUG
    if(naIsRecti32Empty(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePosi32(naGetRecti32EndX(rect), naGetRecti32EndY(rect));
}
NA_IDEF NAPosi64 naGetRecti64End(NARecti64 rect) {
  #if NA_DEBUG
    if(naIsRecti64Empty(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePosi64(naGetRecti64EndX(rect), naGetRecti64EndY(rect));
}
NA_IDEF NAPoss naGetRectsEnd(NARects rect) {
  #if NA_DEBUG
    if(naIsRectsEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePoss(naGetRectsEndX(rect), naGetRectsEndY(rect));
}



NA_IDEF double naGetRectEndX(NARect rect) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(rect.pos.x, rect.size.width)))
      naError("rect is empty in X dimension.");
    if(!naIsOffsetValueValid(rect.pos.x) || !naIsLengthValueValid(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.x, rect.size.width);
}
NA_IDEF float naGetRectfEndX(NARectf rect) {
  #if NA_DEBUG
    if(naIsRangefEmpty(naMakeRangef(rect.pos.x, rect.size.width)))
      naError("rect is empty in X dimension.");
    if(!naIsOffsetValueValidf(rect.pos.x) || !naIsLengthValueValidf(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(rect.pos.x, rect.size.width);
}
NA_IDEF int32 naGetRecti32EndX(NARecti32 rect) {
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi32(rect.pos.x, rect.size.width);
}
NA_IDEF int64 naGetRecti64EndX(NARecti64 rect) {
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi64(rect.pos.x, rect.size.width);
}
NA_IDEF size_t naGetRectsEndX(NARects rect) {
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengths(rect.pos.x, rect.size.width);
}



NA_IDEF double naGetRectEndY(NARect rect) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(rect.pos.y, rect.size.height)))
      naError("rect is empty in Y dimension.");
    if(!naIsOffsetValueValid(rect.pos.y) || !naIsLengthValueValid(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
}
NA_IDEF float naGetRectfEndY(NARectf rect) {
  #if NA_DEBUG
    if(naIsRangefEmpty(naMakeRangef(rect.pos.y, rect.size.height)))
      naError("rect is empty in Y dimension.");
    if(!naIsOffsetValueValidf(rect.pos.y) || !naIsLengthValueValidf(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(rect.pos.y, rect.size.height);
}
NA_IDEF int32 naGetRecti32EndY(NARecti32 rect) {
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi32(rect.pos.y, rect.size.height);
}
NA_IDEF int64 naGetRecti64EndY(NARecti64 rect) {
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi64(rect.pos.y, rect.size.height);
}
NA_IDEF size_t naGetRectsEndY(NARects rect) {
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengths(rect.pos.y, rect.size.height);
}



NA_IDEF double naGetRectEndXE(NARect rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(rect.pos.x) || !naIsLengthValueValid(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.x, rect.size.width);
}
NA_IDEF float naGetRectfEndXE(NARectf rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(rect.pos.x) || !naIsLengthValueValidf(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(rect.pos.x, rect.size.width);
}
NA_IDEF int32 naGetRecti32EndXE(NARecti32 rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(rect.pos.x) || !naIsLengthValueValidi32(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthi32(rect.pos.x, rect.size.width);
}
NA_IDEF int64 naGetRecti64EndXE(NARecti64 rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(rect.pos.x) || !naIsLengthValueValidi64(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthi64(rect.pos.x, rect.size.width);
}
NA_IDEF size_t naGetRectsEndXE(NARects rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValids(rect.pos.x) || !naIsLengthValueValids(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengths(rect.pos.x, rect.size.width);
}



NA_IDEF double naGetRectEndYE(NARect rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(rect.pos.y) || !naIsLengthValueValid(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
}
NA_IDEF float naGetRectfEndYE(NARectf rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(rect.pos.y) || !naIsLengthValueValidf(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(rect.pos.y, rect.size.height);
}
NA_IDEF int32 naGetRecti32EndYE(NARecti32 rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(rect.pos.y) || !naIsLengthValueValidi32(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthi32(rect.pos.y, rect.size.height);
}
NA_IDEF int64 naGetRecti64EndYE(NARecti64 rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(rect.pos.y) || !naIsLengthValueValidi64(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthi64(rect.pos.y, rect.size.height);
}
NA_IDEF size_t naGetRectsEndYE(NARects rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValids(rect.pos.y) || !naIsLengthValueValids(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengths(rect.pos.y, rect.size.height);
}



NA_IDEF NAPosi32 naGetRecti32Max(NARecti32 rect) {
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePosi32(naGetRecti32MaxX(rect), naGetRecti32MaxY(rect));
}
NA_IDEF NAPosi64 naGetRecti64Max(NARecti64 rect) {
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePosi64(naGetRecti64MaxX(rect), naGetRecti64MaxY(rect));
}
NA_IDEF NAPoss naGetRectsMax(NARects rect) {
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePoss(naGetRectsMaxX(rect), naGetRectsMaxY(rect));
}



NA_IDEF int32 naGetRecti32MaxX(NARecti32 rect) {
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi32(rect.pos.x, rect.size.width);
}
NA_IDEF int64 naGetRecti64MaxX(NARecti64 rect) {
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi64(rect.pos.x, rect.size.width);
}
NA_IDEF size_t naGetRectsMaxX(NARects rect) {
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengths(rect.pos.x, rect.size.width);
}



NA_IDEF int32 naGetRecti32MaxY(NARecti32 rect) {
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi32(rect.pos.y, rect.size.height);
}
NA_IDEF int64 naGetRecti64MaxY(NARecti64 rect) {
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi64(rect.pos.y, rect.size.height);
}
NA_IDEF size_t naGetRectsMaxY(NARects rect) {
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengths(rect.pos.y, rect.size.height);
}



NA_IDEF NAVertex naGetBoxEnd(NABox box) {
  #if NA_DEBUG
    if(naIsBoxEmpty(box))
      naError("box is empty.");
    if(!naIsBoxValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertex(naGetBoxEndX(box), naGetBoxEndY(box), naGetBoxEndZ(box));
}
NA_IDEF NAVertexf naGetBoxfEnd(NABoxf box) {
  #if NA_DEBUG
    if(naIsBoxfEmpty(box))
      naError("box is empty.");
    if(!naIsBoxfValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexf(naGetBoxfEndX(box), naGetBoxfEndY(box), naGetBoxfEndZ(box));
}
NA_IDEF NAVertexi32 naGetBoxi32End (NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi32(naGetBoxi32EndX(box), naGetBoxi32EndY(box), naGetBoxi32EndZ(box));
}
NA_IDEF NAVertexi64 naGetBoxi64End (NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi64(naGetBoxi64EndX(box), naGetBoxi64EndY(box), naGetBoxi64EndZ(box));
}
NA_IDEF NAVertexs naGetBoxsEnd (NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexs(naGetBoxsEndX(box), naGetBoxsEndY(box), naGetBoxsEndZ(box));
}



NA_IDEF double naGetBoxEndX(NABox box) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.x, box.volume.width)))
      naError("box is empty in X dimension.");
    if(!naIsOffsetValueValid(box.vertex.x) || !naIsLengthValueValid(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
}
NA_IDEF float naGetBoxfEndX(NABoxf box) {
  #if NA_DEBUG
    if(naIsRangefEmpty(naMakeRangef(box.vertex.x, box.volume.width)))
      naError("box is empty in X dimension.");
    if(!naIsOffsetValueValidf(box.vertex.x) || !naIsLengthValueValidf(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(box.vertex.x, box.volume.width);
}
NA_IDEF int32 naGetBoxi32EndX(NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi32(box.vertex.x, box.volume.width);
}
NA_IDEF int64 naGetBoxi64EndX(NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi64(box.vertex.x, box.volume.width);
}
NA_IDEF size_t naGetBoxsEndX(NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengths(box.vertex.x, box.volume.width);
}



NA_IDEF double naGetBoxEndY(NABox box) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.y, box.volume.height)))
      naError("box is empty in Y dimension.");
    if(!naIsOffsetValueValid(box.vertex.y) || !naIsLengthValueValid(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.y, box.volume.height);
}
NA_IDEF float naGetBoxfEndY(NABoxf box) {
  #if NA_DEBUG
    if(naIsRangefEmpty(naMakeRangef(box.vertex.y, box.volume.height)))
      naError("box is empty in Y dimension.");
    if(!naIsOffsetValueValidf(box.vertex.y) || !naIsLengthValueValidf(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(box.vertex.y, box.volume.height);
}
NA_IDEF int32 naGetBoxi32EndY(NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi32(box.vertex.y, box.volume.height);
}
NA_IDEF int64 naGetBoxi64EndY(NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi64(box.vertex.y, box.volume.height);
}
NA_IDEF size_t naGetBoxsEndY(NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengths(box.vertex.y, box.volume.height);
}



NA_IDEF double naGetBoxEndZ(NABox box) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.z, box.volume.depth)))
      naError("box is empty in Z dimension.");
    if(!naIsOffsetValueValid(box.vertex.z) || !naIsLengthValueValid(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
}
NA_IDEF float naGetBoxfEndZ(NABoxf box) {
  #if NA_DEBUG
    if(naIsRangefEmpty(naMakeRangef(box.vertex.z, box.volume.depth)))
      naError("box is empty in Z dimension.");
    if(!naIsOffsetValueValidf(box.vertex.z) || !naIsLengthValueValidf(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(box.vertex.z, box.volume.depth);
}
NA_IDEF int32 naGetBoxi32EndZ(NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi32(box.vertex.z, box.volume.depth);
}
NA_IDEF int64 naGetBoxi64EndZ(NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi64(box.vertex.z, box.volume.depth);
}
NA_IDEF size_t naGetBoxsEndZ(NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengths(box.vertex.z, box.volume.depth);
}




NA_IDEF double naGetBoxEndXE(NABox box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.x) || !naIsLengthValueValid(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
}
NA_IDEF float naGetBoxfEndXE(NABoxf box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(box.vertex.x) || !naIsLengthValueValidf(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(box.vertex.x, box.volume.width);
}
NA_IDEF int32 naGetBoxi32EndXE(NABoxi32 box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(box.vertex.x) || !naIsLengthValueValidi32(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthi32(box.vertex.x, box.volume.width);
}
NA_IDEF int64 naGetBoxi64EndXE(NABoxi64 box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(box.vertex.x) || !naIsLengthValueValidi64(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengthi64(box.vertex.x, box.volume.width);
}
NA_IDEF size_t naGetBoxsEndXE(NABoxs box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValids(box.vertex.x) || !naIsLengthValueValids(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLengths(box.vertex.x, box.volume.width);
}



NA_IDEF double naGetBoxEndYE(NABox box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.y) || !naIsLengthValueValid(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.y, box.volume.height);
}
NA_IDEF float naGetBoxfEndYE(NABoxf box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(box.vertex.y) || !naIsLengthValueValidf(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(box.vertex.y, box.volume.height);
}
NA_IDEF int32 naGetBoxi32EndYE(NABoxi32 box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(box.vertex.y) || !naIsLengthValueValidi32(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthi32(box.vertex.y, box.volume.height);
}
NA_IDEF int64 naGetBoxi64EndYE(NABoxi64 box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(box.vertex.y) || !naIsLengthValueValidi64(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengthi64(box.vertex.y, box.volume.height);
}
NA_IDEF size_t naGetBoxsEndYE(NABoxs box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValids(box.vertex.y) || !naIsLengthValueValids(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLengths(box.vertex.y, box.volume.height);
}



NA_IDEF double naGetBoxEndZE(NABox box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.z) || !naIsLengthValueValid(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
}
NA_IDEF float naGetBoxfEndZE(NABoxf box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(box.vertex.z) || !naIsLengthValueValidf(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLengthf(box.vertex.z, box.volume.depth);
}
NA_IDEF int32 naGetBoxi32EndZE(NABoxi32 box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(box.vertex.z) || !naIsLengthValueValidi32(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLengthi32(box.vertex.z, box.volume.depth);
}
NA_IDEF int64 naGetBoxi64EndZE(NABoxi64 box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(box.vertex.z) || !naIsLengthValueValidi64(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLengthi64(box.vertex.z, box.volume.depth);
}
NA_IDEF size_t naGetBoxsEndZE(NABoxs box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValids(box.vertex.z) || !naIsLengthValueValids(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLengths(box.vertex.z, box.volume.depth);
}


















NA_IDEF NAVertexi32 naGetBoxi32Max (NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi32(naGetBoxi32MaxX(box), naGetBoxi32MaxY(box), naGetBoxi32MaxZ(box));
}
NA_IDEF NAVertexi64 naGetBoxi64Max (NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi64(naGetBoxi64MaxX(box), naGetBoxi64MaxY(box), naGetBoxi64MaxZ(box));
}
NA_IDEF NAVertexs naGetBoxsMax (NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexs(naGetBoxsMaxX(box), naGetBoxsMaxY(box), naGetBoxsMaxZ(box));
}



NA_IDEF int32 naGetBoxi32MaxX(NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi32(box.vertex.x, box.volume.width);
}
NA_IDEF int64 naGetBoxi64MaxX(NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi64(box.vertex.x, box.volume.width);
}
NA_IDEF size_t naGetBoxsMaxX(NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengths(box.vertex.x, box.volume.width);
}



NA_IDEF int32 naGetBoxi32MaxY(NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi32(box.vertex.y, box.volume.height);
}
NA_IDEF int64 naGetBoxi64MaxY(NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi64(box.vertex.y, box.volume.height);
}
NA_IDEF size_t naGetBoxsMaxY(NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengths(box.vertex.y, box.volume.height);
}



NA_IDEF int32 naGetBoxi32MaxZ(NABoxi32 box) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi32(box.vertex.z, box.volume.depth);
}
NA_IDEF int64 naGetBoxi64MaxZ(NABoxi64 box) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi64(box.vertex.z, box.volume.depth);
}
NA_IDEF size_t naGetBoxsMaxZ(NABoxs box) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengths(box.vertex.z, box.volume.depth);
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
