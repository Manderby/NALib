
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NABool naEqualRange(NARange range1, NARange range2) {
  #if NA_DEBUG
    if(!naIsRangeValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangeValid(range2))
      naError("range2 is invalid.");
  #endif
  return ((range1.origin == range2.origin) && (range1.length == range2.length));
}
NA_IDEF NABool naEqualRangef(NARangef range1, NARangef range2) {
  #if NA_DEBUG
    if(!naIsRangefValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangefValid(range2))
      naError("range2 is invalid.");
  #endif
  return ((range1.origin == range2.origin) && (range1.length == range2.length));
}
NA_IDEF NABool naEqualRangei32(NARangei32 range1, NARangei32 range2) {
  #if NA_DEBUG
    if(!naIsRangei32Valid(range1))
      naError("range1 is invalid.");
    if(!naIsRangei32Valid(range2))
      naError("range2 is invalid.");
  #endif
  return ((range1.origin == range2.origin) && (range1.length == range2.length));
}
NA_IDEF NABool naEqualRangei64(NARangei64 range1, NARangei64 range2) {
  #if NA_DEBUG
    if(!naIsRangei64Valid(range1))
      naError("range1 is invalid.");
    if(!naIsRangei64Valid(range2))
      naError("range2 is invalid.");
  #endif
  return (naEquali64(range1.origin, range2.origin) && naEquali64(range1.length, range2.length));
}
NA_IDEF NABool naEqualRanges(NARanges range1, NARanges range2) {
  #if NA_DEBUG
    if(!naIsRangesValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangesValid(range2))
      naError("range2 is invalid.");
  #endif
  return ((range1.origin == range2.origin) && (range1.length == range2.length));
}



NA_IDEF NABool naEqualPos(NAPos pos1, NAPos pos2) {
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualPosf(NAPosf pos1, NAPosf pos2) {
  #if NA_DEBUG
    if(!naIsPosfValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosfValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualPosi32(NAPosi32 pos1, NAPosi32 pos2) {
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosi32Valid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualPosi64(NAPosi64 pos1, NAPosi64 pos2) {
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosi64Valid(pos2))
      naError("pos2 is invalid.");
  #endif
  return (naEquali64(pos1.x, pos2.x) && naEquali64(pos1.y, pos2.y));
}
NA_IDEF NABool naEqualPoss(NAPoss pos1, NAPoss pos2) {
  #if NA_DEBUG
    if(!naIsPossValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPossValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}



NA_IDEF NABool naEqualSize(NASize size1, NASize size2) {
  #if NA_DEBUG
    if(naIsSizeEmpty(size1) || naIsSizeEmpty(size2))
      naError("Equality test not valid for empty sizes.");
    if(!naIsSizeValid(size1))
      naError("size1 is invalid.");
    if(!naIsSizeValid(size2))
      naError("size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IDEF NABool naEqualSizef(NASizef size1, NASizef size2) {
  #if NA_DEBUG
    if(naIsSizefEmpty(size1) || naIsSizefEmpty(size2))
      naError("Equality test not valid for empty sizes.");
    if(!naIsSizefValid(size1))
      naError("size1 is invalid.");
    if(!naIsSizefValid(size2))
      naError("size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IDEF NABool naEqualSizei32(NASizei32 size1, NASizei32 size2) {
  #if NA_DEBUG
    if(naIsSizei32Empty(size1) || naIsSizei32Empty(size2))
      naError("Equality test not valid for empty sizes.");
    if(!naIsSizei32Valid(size1))
      naError("size1 is invalid.");
    if(!naIsSizei32Valid(size2))
      naError("size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IDEF NABool naEqualSizei64(NASizei64 size1, NASizei64 size2) {
  #if NA_DEBUG
    if(naIsSizei64Empty(size1) || naIsSizei64Empty(size2))
      naError("Equality test not valid for empty sizes.");
    if(!naIsSizei64Valid(size1))
      naError("size1 is invalid.");
    if(!naIsSizei64Valid(size2))
      naError("size2 is invalid.");
  #endif
  return (naEquali64(size1.width, size2.width) && naEquali64(size1.height, size2.height));
}
NA_IDEF NABool naEqualSizes(NASizes size1, NASizes size2) {
  #if NA_DEBUG
    if(naIsSizesEmpty(size1) || naIsSizesEmpty(size2))
      naError("Equality test not valid for empty sizes.");
    if(!naIsSizesValid(size1))
      naError("size1 is invalid.");
    if(!naIsSizesValid(size2))
      naError("size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}



NA_IDEF NABool naEqualRect(NARect rect1, NARect rect2) {
  #if NA_DEBUG
    if(naIsRectEmpty(rect1) || naIsRectEmpty(rect2))
      naError("Equality test not valid for empty rects.");
    if(!naIsRectValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectValid(rect2))
      naError("rect2 is invalid.");
  #endif
  return (naEqualPos (rect1.pos, rect2.pos)
    &&  naEqualSize(rect1.size, rect2.size));
}

NA_IDEF NABool naEqualRectf(NARectf rect1, NARectf rect2) {
  #if NA_DEBUG
    if(naIsRectfEmpty(rect1) || naIsRectfEmpty(rect2))
      naError("Equality test not valid for empty rects.");
    if(!naIsRectfValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectfValid(rect2))
      naError("rect2 is invalid.");
  #endif
  return (naEqualPosf(rect1.pos, rect2.pos)
    &&  naEqualSizef(rect1.size, rect2.size));
}

NA_IDEF NABool naEqualRecti32(NARecti32 rect1, NARecti32 rect2) {
  #if NA_DEBUG
    if(naIsRecti32Empty(rect1) || naIsRecti32Empty(rect2))
      naError("Equality test not valid for empty rects.");
    if(!naIsRecti32Valid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRecti32Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  return (naEqualPosi32(rect1.pos, rect2.pos)
    && naEqualSizei32(rect1.size, rect2.size));
}
NA_IDEF NABool naEqualRecti64(NARecti64 rect1, NARecti64 rect2) {
  #if NA_DEBUG
    if(naIsRecti64Empty(rect1) || naIsRecti64Empty(rect2))
      naError("Equality test not valid for empty rects.");
    if(!naIsRecti64Valid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRecti64Valid(rect2))
      naError("rect2 is invalid.");
  #endif
  return (naEqualPosi64(rect1.pos, rect2.pos)
    && naEqualSizei64(rect1.size, rect2.size));
}
NA_IDEF NABool naEqualRects(NARects rect1, NARects rect2) {
  #if NA_DEBUG
    if(naIsRectsEmpty(rect1) || naIsRectsEmpty(rect2))
      naError("Equality test not valid for empty rects.");
    if(!naIsRectsValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectsValid(rect2))
      naError("rect2 is invalid.");
  #endif
  return (naEqualPoss(rect1.pos, rect2.pos)
    && naEqualSizes(rect1.size, rect2.size));
}



NA_IDEF NABool naEqualVertex(NAVertex vertex1, NAVertex vertex2) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVertexf(NAVertexf vertex1, NAVertexf vertex2) {
  #if NA_DEBUG
    if(!naIsVertexfValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexfValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVertexi32(NAVertexi32 vertex1, NAVertexi32 vertex2) {
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexi32Valid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVertexi64(NAVertexi64 vertex1, NAVertexi64 vertex2) {
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexi64Valid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return (naEquali64(vertex1.x, vertex2.x) && naEquali64(vertex1.y, vertex2.y) && naEquali64(vertex1.z, vertex2.z));
}
NA_IDEF NABool naEqualVertexs(NAVertexs vertex1, NAVertexs vertex2) {
  #if NA_DEBUG
    if(!naIsVertexsValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexsValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}



NA_IDEF NABool naEqualVolume(NAVolume volume1, NAVolume volume2) {
  #if NA_DEBUG
    if(naIsVolumeEmpty(volume1) || naIsVolumeEmpty(volume2))
      naError("Equality test not valid for empty volumes.");
    if(!naIsVolumeValid(volume1))
      naError("volume1 is invalid.");
    if(!naIsVolumeValid(volume2))
      naError("volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}
NA_IDEF NABool naEqualVolumef(NAVolumef volume1, NAVolumef volume2) {
  #if NA_DEBUG
    if(naIsVolumefEmpty(volume1) || naIsVolumefEmpty(volume2))
      naError("Equality test not valid for empty volumes.");
    if(!naIsVolumefValid(volume1))
      naError("volume1 is invalid.");
    if(!naIsVolumefValid(volume2))
      naError("volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}
NA_IDEF NABool naEqualVolumei32(NAVolumei32 volume1, NAVolumei32 volume2) {
  #if NA_DEBUG
    if(naIsVolumei32Empty(volume1) || naIsVolumei32Empty(volume2))
      naError("Equality test not valid for empty volumes.");
    if(!naIsVolumei32Valid(volume1))
      naError("volume1 is invalid.");
    if(!naIsVolumei32Valid(volume2))
      naError("volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}
NA_IDEF NABool naEqualVolumei64(NAVolumei64 volume1, NAVolumei64 volume2) {
  #if NA_DEBUG
    if(naIsVolumei64Empty(volume1) || naIsVolumei64Empty(volume2))
      naError("Equality test not valid for empty volumes.");
    if(!naIsVolumei64Valid(volume1))
      naError("volume1 is invalid.");
    if(!naIsVolumei64Valid(volume2))
      naError("volume2 is invalid.");
  #endif
  return (naEquali64(volume1.width, volume2.width) && naEquali64(volume1.height, volume2.height) && naEquali64(volume1.depth, volume2.depth));
}
NA_IDEF NABool naEqualVolumes(NAVolumes volume1, NAVolumes volume2) {
  #if NA_DEBUG
    if(naIsVolumesEmpty(volume1) || naIsVolumesEmpty(volume2))
      naError("Equality test not valid for empty volumes.");
    if(!naIsVolumesValid(volume1))
      naError("volume1 is invalid.");
    if(!naIsVolumesValid(volume2))
      naError("volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}



NA_IDEF NABool naEqualBox(NABox box1, NABox box2) {
  #if NA_DEBUG
    if(naIsBoxEmpty(box1) || naIsBoxEmpty(box2))
      naError("Equality test not valid for empty boxs.");
    if(!naIsBoxValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("box2 is invalid.");
  #endif
  return     (naEqualVertex (box1.vertex,  box2.vertex)
          &&  naEqualVolume(box1.volume, box2.volume));
}
NA_IDEF NABool naEqualBoxf(NABoxf box1, NABoxf box2) {
  #if NA_DEBUG
    if(naIsBoxfEmpty(box1) || naIsBoxfEmpty(box2))
      naError("Equality test not valid for empty boxs.");
    if(!naIsBoxfValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxfValid(box2))
      naError("box2 is invalid.");
  #endif
  return     (naEqualVertexf (box1.vertex,  box2.vertex)
          &&  naEqualVolumef(box1.volume, box2.volume));
}
NA_IDEF NABool naEqualBoxi32(NABoxi32 box1, NABoxi32 box2) {
  #if NA_DEBUG
    if(naIsBoxi32Empty(box1) || naIsBoxi32Empty(box2))
      naError("Equality test not valid for empty boxs.");
    if(!naIsBoxi32Valid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxi32Valid(box2))
      naError("box2 is invalid.");
  #endif
  return    (naEqualVertexi32 (box1.vertex,  box2.vertex)
          && naEqualVolumei32(box1.volume, box2.volume));
}
NA_IDEF NABool naEqualBoxi64(NABoxi64 box1, NABoxi64 box2) {
  #if NA_DEBUG
    if(naIsBoxi64Empty(box1) || naIsBoxi64Empty(box2))
      naError("Equality test not valid for empty boxs.");
    if(!naIsBoxi64Valid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxi64Valid(box2))
      naError("box2 is invalid.");
  #endif
  return    (naEqualVertexi64 (box1.vertex,  box2.vertex)
          && naEqualVolumei64(box1.volume, box2.volume));
}
NA_IDEF NABool naEqualBoxs(NABoxs box1, NABoxs box2) {
  #if NA_DEBUG
    if(naIsBoxsEmpty(box1) || naIsBoxsEmpty(box2))
      naError("Equality test not valid for empty boxs.");
    if(!naIsBoxsValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxsValid(box2))
      naError("box2 is invalid.");
  #endif
  return    (naEqualVertexs (box1.vertex,  box2.vertex)
          && naEqualVolumes(box1.volume, box2.volume));
}



NA_IDEF NABool naSmallerPos(NAPos pos1, NAPos pos2) {
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x < pos2.x) && (pos1.y < pos2.y));
}
NA_IDEF NABool naSmallerEqualPos(NAPos pos1, NAPos pos2) {
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x <= pos2.x) && (pos1.y <= pos2.y));
}
NA_IDEF NABool naGreaterEqualPos(NAPos pos1, NAPos pos2) {
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x >= pos2.x) && (pos1.y >= pos2.y));
}
NA_IDEF NABool naGreaterPos(NAPos pos1, NAPos pos2) {
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x > pos2.x) && (pos1.y > pos2.y));
}



NA_IDEF NABool naSmallerVertex(NAVertex vertex1, NAVertex vertex2) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x < vertex2.x) && (vertex1.y < vertex2.y) && (vertex1.z < vertex2.z));
}
NA_IDEF NABool naSmallerEqualVertex(NAVertex vertex1, NAVertex vertex2) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x <= vertex2.x) && (vertex1.y <= vertex2.y) && (vertex1.z <= vertex2.z));
}
NA_IDEF NABool naGreaterEqualVertex(NAVertex vertex1, NAVertex vertex2) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x >= vertex2.x) && (vertex1.y >= vertex2.y) && (vertex1.z >= vertex2.z));
}
NA_IDEF NABool naGreaterVertex(NAVertex vertex1, NAVertex vertex2) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x > vertex2.x) && (vertex1.y > vertex2.y) && (vertex1.z > vertex2.z));
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
