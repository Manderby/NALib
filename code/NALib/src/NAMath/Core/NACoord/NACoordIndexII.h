
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF size_t naGetSizei32IndexCount(NASizei32 size) {
  #if NA_DEBUG
    if(naIsSizei32EmptySlow(size))
      naError("size is empty.");
    if(!naIsSizei32Valid(size))
      naError("size is invalid.");
    if(!naIsSizei32Useful(size))
      naError("size is not useful.");
  #endif
  return (size_t)(size.width * size.height);
}

NA_IDEF size_t naGetSizei64IndexCount(NASizei64 size) {
  #if NA_DEBUG
    if(naIsSizei64EmptySlow(size))
      naError("size is empty.");
    if(!naIsSizei64Valid(size))
      naError("size is invalid.");
    if(!naIsSizei64Useful(size))
      naError("size is not useful.");
  #endif
  return naCasti64ToSize(naMuli64(size.width, size.height));
}

NA_IDEF size_t naGetSizesIndexCount(NASizes size) {
  #if NA_DEBUG
    if(naIsSizesEmptySlow(size))
      naError("size is empty.");
    if(!naIsSizesValid(size))
      naError("size is invalid.");
    if(!naIsSizesUseful(size))
      naError("size is not useful.");
  #endif
  return size.width * size.height;
}



NA_IDEF size_t naGetVolumei32IndexCount(NAVolumei32 volume) {
  #if NA_DEBUG
    if(naIsVolumei32EmptySlow(volume))
      naError("volume is empty.");
    if(!naIsVolumei32Valid(volume))
      naError("volume is invalid.");
    if(!naIsVolumei32Useful(volume))
      naError("volume is not useful.");
  #endif
  return (size_t)(volume.width * volume.height * volume.depth);
}

NA_IDEF size_t naGetVolumei64IndexCount(NAVolumei64 volume) {
  #if NA_DEBUG
    if(naIsVolumei64EmptySlow(volume))
      naError("volume is empty.");
    if(!naIsVolumei64Valid(volume))
      naError("volume is invalid.");
    if(!naIsVolumei64Useful(volume))
      naError("volume is not useful.");
  #endif
  return naCasti64ToSize(naMuli64(naMuli64(volume.width, volume.height), volume.depth));
}

NA_IDEF size_t naGetVolumesIndexCount(NAVolumes volume) {
  #if NA_DEBUG
    if(naIsVolumesEmptySlow(volume))
      naError("volume is empty.");
    if(!naIsVolumesValid(volume))
      naError("volume is invalid.");
    if(!naIsVolumesUseful(volume))
      naError("volume is not useful.");
  #endif
  return (size_t)(volume.width * volume.height * volume.depth);
}



NA_IDEF size_t naGetIndexWithOrigini32AndPosRowFirst(NAPosi32 origin, NAPosi32 offset, int32 width) {
  return (size_t)((offset.y - origin.y) * width + (offset.x - origin.x));
}
NA_IDEF size_t naGetIndexWithOrigini64AndPosRowFirst(NAPosi64 origin, NAPosi64 offset, int64 width) {
  return naCasti64ToSize(naAddi64(naMuli64(naSubi64(offset.y, origin.y), width), naSubi64(offset.x, origin.x)));
}
NA_IDEF size_t naGetIndexWithOriginsAndPosRowFirst(NAPoss origin, NAPoss offset, size_t width) {
  return (size_t)((offset.y - origin.y) * width + (offset.x - origin.x));
}



NA_IDEF size_t naGetIndexWithOrigini32AndPosColumnFirst(NAPosi32 origin, NAPosi32 offset, int32 height) {
  return (size_t)((offset.x - origin.x) * height + (offset.y - origin.y));
}
NA_IDEF size_t naGetIndexWithOrigini64AndPosColumnFirst(NAPosi64 origin, NAPosi64 offset, int64 height) {
  return naCasti64ToSize(naAddi64(naMuli64(naSubi64(offset.x, origin.x), height), naSubi64(offset.y, origin.y)));
}
NA_IDEF size_t naGetIndexWithOriginsAndPosColumnFirst(NAPoss origin, NAPoss offset, size_t height) {
  return (size_t)((offset.x - origin.x) * height + (offset.y - origin.y));
}



NA_IDEF size_t naGetIndexWithOrigini32AndVertexRowFirst(NAVertexi32 origin, NAVertexi32 vertex, int32 width, int32 height) {
  return (size_t)(((vertex.z - origin.z) * height + (vertex.y - origin.y)) * width + (vertex.x - origin.x));
}
NA_IDEF size_t naGetIndexWithOrigini64AndVertexRowFirst(NAVertexi64 origin, NAVertexi64 vertex, int64 width, int64 height) {
  return naCasti64ToSize(naAddi64(naMuli64(naAddi64(naMuli64(naSubi64(vertex.z, origin.z), height), naSubi64(vertex.y, origin.y)), width), naSubi64(vertex.x, origin.x)));
}
NA_IDEF size_t naGetIndexWithOriginsAndVertexRowFirst(NAVertexs origin, NAVertexs vertex, size_t width, size_t height) {
  return (size_t)(((vertex.z - origin.z) * height + (vertex.y - origin.y)) * width + (vertex.x - origin.x));
}



NA_IDEF size_t naGetIndexWithOrigini32AndVertexColumnFirst(NAVertexi32 origin, NAVertexi32 vertex, int32 depth, int32 height) {
  return (size_t)(((vertex.x - origin.x) * height + (vertex.y - origin.y)) * depth + (vertex.z - origin.z));
}
NA_IDEF size_t naGetIndexWithOrigini64AndVertexColumnFirst(NAVertexi64 origin, NAVertexi64 vertex, int64 depth, int64 height) {
  return naCasti64ToSize(naAddi64(naMuli64(naAddi64(naMuli64(naSubi64(vertex.x, origin.x), height), naSubi64(vertex.y, origin.y)), depth), naSubi64(vertex.z, origin.z)));
}
NA_IDEF size_t naGetIndexWithOriginsAndVertexColumnFirst(NAVertexs origin, NAVertexs vertex, size_t depth, size_t height) {
  return (size_t)(((vertex.x - origin.x) * height + (vertex.y - origin.y)) * depth + (vertex.z - origin.z));
}



NA_IDEF size_t naGetRecti32IndexOfPosRowFirst(NARecti32 rect, NAPosi32 pos) {
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
    if(!naContainsRecti32Point(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOrigini32AndPosRowFirst(rect.pos, pos, rect.size.width);
}

NA_IDEF size_t naGetRecti64IndexOfPosRowFirst(NARecti64 rect, NAPosi64 pos) {
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
    if(!naContainsRecti64Point(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOrigini64AndPosRowFirst(rect.pos, pos, rect.size.width);
}

NA_IDEF size_t naGetRectsIndexOfPosRowFirst(NARects rect, NAPoss pos) {
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
    if(!naIsPossValid(pos))
      naError("pos is invalid.");
    if(!naContainsRectsPoint(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOriginsAndPosRowFirst(rect.pos, pos, rect.size.width);
}



NA_IDEF size_t naGetRecti32IndexOfPosColumnFirst(NARecti32 rect, NAPosi32 pos) {
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
    if(!naContainsRecti32Point(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOrigini32AndPosColumnFirst(rect.pos, pos, rect.size.height);
}

NA_IDEF size_t naGetRecti64IndexOfPosColumnFirst(NARecti64 rect, NAPosi64 pos) {
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
    if(!naContainsRecti64Point(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOrigini64AndPosColumnFirst(rect.pos, pos, rect.size.height);
}

NA_IDEF size_t naGetRectsIndexOfPosColumnFirst(NARects rect, NAPoss pos) {
  #if NA_DEBUG
    if(naIsRectsEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectsValid(rect))
      naError("rect is invalid.");
    if(!naIsPossValid(pos))
      naError("pos is invalid.");
    if(!naContainsRectsPoint(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOriginsAndPosColumnFirst(rect.pos, pos, rect.size.height);
}



NA_IDEF size_t naGetBoxi32IndexOfVertexRowFirst(NABoxi32 box, NAVertexi32 vertex) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxi32Point(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOrigini32AndVertexRowFirst(box.vertex, vertex, box.volume.width, box.volume.height);
}

NA_IDEF size_t naGetBoxi64IndexOfVertexRowFirst(NABoxi64 box, NAVertexi64 vertex) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxi64Point(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOrigini64AndVertexRowFirst(box.vertex, vertex, box.volume.width, box.volume.height);
}

NA_IDEF size_t naGetBoxsIndexOfVertexRowFirst(NABoxs box, NAVertexs vertex) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
    if(!naIsVertexsValid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxsPoint(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOriginsAndVertexRowFirst(box.vertex, vertex, box.volume.width, box.volume.height);
}



NA_IDEF size_t naGetBoxi32IndexOfVertexColumnFirst(NABoxi32 box, NAVertexi32 vertex) {
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxi32Point(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOrigini32AndVertexColumnFirst(box.vertex, vertex, box.volume.depth, box.volume.height);
}

NA_IDEF size_t naGetBoxi64IndexOfVertexColumnFirst(NABoxi64 box, NAVertexi64 vertex) {
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxi64Point(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOrigini64AndVertexColumnFirst(box.vertex, vertex, box.volume.depth, box.volume.height);
}

NA_IDEF size_t naGetBoxsIndexOfVertexColumnFirst(NABoxs box, NAVertexs vertex) {
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
    if(!naIsVertexsValid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxsPoint(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOriginsAndVertexColumnFirst(box.vertex, vertex, box.volume.depth, box.volume.height);
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
