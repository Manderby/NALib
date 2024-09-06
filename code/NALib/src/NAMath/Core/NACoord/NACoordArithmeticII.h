
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NAPos naAddPosSize(NAPos pos, NASize size) {
  NAPos newPos;
  #if NA_DEBUG
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
    if(!naIsSizeValid(size))
      naError("size is invalid.");
  #endif
  newPos.x = pos.x + size.width;
  newPos.y = pos.y + size.height;
  return newPos;
}
NA_IDEF NAPosf naAddPosfSize(NAPosf pos, NASizef size) {
  NAPosf newPos;
  #if NA_DEBUG
    if(!naIsPosfValid(pos))
      naError("pos is invalid.");
    if(!naIsSizefValid(size))
      naError("size is invalid.");
  #endif
  newPos.x = pos.x + size.width;
  newPos.y = pos.y + size.height;
  return newPos;
}
NA_IDEF NAPosi32 naAddPosi32Size(NAPosi32 pos, NASizei32 size) {
  NAPosi32 newPos;
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
    if(!naIsSizei32Valid(size))
      naError("size is invalid.");
  #endif
  newPos.x = pos.x + size.width;
  newPos.y = pos.y + size.height;
  return newPos;
}
NA_IDEF NAPosi64 naAddPosi64Size(NAPosi64 pos, NASizei64 size) {
  NAPosi64 newPos;
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
    if(!naIsSizei64Valid(size))
      naError("size is invalid.");
  #endif
  newPos.x = pos.x + size.width;
  newPos.y = pos.y + size.height;
  return newPos;
}
NA_IDEF NAPoss naAddPossSize(NAPoss pos, NASizes size) {
  NAPoss newPos;
  #if NA_DEBUG
    if(!naIsPossValid(pos))
      naError("pos is invalid.");
    if(!naIsSizesValid(size))
      naError("size is invalid.");
  #endif
  newPos.x = pos.x + size.width;
  newPos.y = pos.y + size.height;
  return newPos;
}



NA_IDEF NASize naSubPosPos(NAPos pos1, NAPos pos2) {
  NASize newSize;
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  newSize.width  = pos1.x - pos2.x;
  newSize.height = pos1.y - pos2.y;
  return newSize;
}
NA_IDEF NASizef naSubPosfPos(NAPosf pos1, NAPosf pos2) {
  NASizef newSize;
  #if NA_DEBUG
    if(!naIsPosfValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosfValid(pos2))
      naError("pos2 is invalid.");
  #endif
  newSize.width  = pos1.x - pos2.x;
  newSize.height = pos1.y - pos2.y;
  return newSize;
}
NA_IDEF NASizei32 naSubPosi32Pos(NAPosi32 pos1, NAPosi32 pos2) {
  NASizei32 newSize;
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosi32Valid(pos2))
      naError("pos2 is invalid.");
  #endif
  newSize.width  = pos1.x - pos2.x;
  newSize.height = pos1.y - pos2.y;
  return newSize;
}
NA_IDEF NASizei64 naSubPosi64Pos(NAPosi64 pos1, NAPosi64 pos2) {
  NASizei64 newSize;
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosi64Valid(pos2))
      naError("pos2 is invalid.");
  #endif
  newSize.width  = pos1.x - pos2.x;
  newSize.height = pos1.y - pos2.y;
  return newSize;
}
NA_IDEF NASizes naSubPossPos(NAPoss pos1, NAPoss pos2) {
  NASizes newSize;
  #if NA_DEBUG
    if(!naIsPossValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPossValid(pos2))
      naError("pos2 is invalid.");
  #endif
  newSize.width  = pos1.x - pos2.x;
  newSize.height = pos1.y - pos2.y;
  return newSize;
}



NA_IDEF NAVertex naAddVertexVolume(NAVertex vertex, NAVolume volume) {
  NAVertex newVertex;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumeValid(volume))
      naError("volume is invalid.");
  #endif
  newVertex.x = vertex.x + volume.width;
  newVertex.y = vertex.y + volume.height;
  newVertex.z = vertex.z + volume.depth;
  return newVertex;
}
NA_IDEF NAVertexf naAddVertexfVolume(NAVertexf vertex, NAVolumef volume) {
  NAVertexf newVertex;
  #if NA_DEBUG
    if(!naIsVertexfValid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumefValid(volume))
      naError("volume is invalid.");
  #endif
  newVertex.x = vertex.x + volume.width;
  newVertex.y = vertex.y + volume.height;
  newVertex.z = vertex.z + volume.depth;
  return newVertex;
}
NA_IDEF NAVertexi32 naAddVertexi32Volume(NAVertexi32 vertex, NAVolumei32 volume) {
  NAVertexi32 newVertex;
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumei32Valid(volume))
      naError("volume is invalid.");
  #endif
  newVertex.x = vertex.x + volume.width;
  newVertex.y = vertex.y + volume.height;
  newVertex.z = vertex.z + volume.depth;
  return newVertex;
}
NA_IDEF NAVertexi64 naAddVertexi64Volume(NAVertexi64 vertex, NAVolumei64 volume) {
  NAVertexi64 newVertex;
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumei64Valid(volume))
      naError("volume is invalid.");
  #endif
  newVertex.x = vertex.x + volume.width;
  newVertex.y = vertex.y + volume.height;
  newVertex.z = vertex.z + volume.depth;
  return newVertex;
}
NA_IDEF NAVertexs naAddVertexsVolume(NAVertexs vertex, NAVolumes volume) {
  NAVertexs newVertex;
  #if NA_DEBUG
    if(!naIsVertexsValid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumesValid(volume))
      naError("volume is invalid.");
  #endif
  newVertex.x = vertex.x + volume.width;
  newVertex.y = vertex.y + volume.height;
  newVertex.z = vertex.z + volume.depth;
  return newVertex;
}



NA_IDEF NAVolume naSubVertexPos(NAVertex vertex1, NAVertex vertex2) {
  NAVolume newVolume;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  newVolume.width  = vertex1.x - vertex2.x;
  newVolume.height = vertex1.y - vertex2.y;
  newVolume.depth  = vertex1.z - vertex2.z;
  return newVolume;
}
NA_IDEF NAVolumef naSubVertexfPos(NAVertexf vertex1, NAVertexf vertex2) {
  NAVolumef newVolume;
  #if NA_DEBUG
    if(!naIsVertexfValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexfValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  newVolume.width  = vertex1.x - vertex2.x;
  newVolume.height = vertex1.y - vertex2.y;
  newVolume.depth  = vertex1.z - vertex2.z;
  return newVolume;
}
NA_IDEF NAVolumei32 naSubVertexi32Vertex(NAVertexi32 vertex1, NAVertexi32 vertex2) {
  NAVolumei32 newVolume;
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexi32Valid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  newVolume.width  = vertex1.x - vertex2.x;
  newVolume.height = vertex1.y - vertex2.y;
  newVolume.depth  = vertex1.z - vertex2.y;
  return newVolume;
}
NA_IDEF NAVolumei64 naSubVertexi64Vertex(NAVertexi64 vertex1, NAVertexi64 vertex2) {
  NAVolumei64 newVolume;
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexi64Valid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  newVolume.width  = vertex1.x - vertex2.x;
  newVolume.height = vertex1.y - vertex2.y;
  newVolume.depth  = vertex1.z - vertex2.y;
  return newVolume;
}
NA_IDEF NAVolumes naSubVertexsVertex(NAVertexs vertex1, NAVertexs vertex2) {
  NAVolumes newVolume;
  #if NA_DEBUG
    if(!naIsVertexsValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexsValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  newVolume.width  = vertex1.x - vertex2.x;
  newVolume.height = vertex1.y - vertex2.y;
  newVolume.depth  = vertex1.z - vertex2.y;
  return newVolume;
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
