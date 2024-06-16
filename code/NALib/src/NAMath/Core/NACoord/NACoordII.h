
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"




// /////////////////////////////////////|
// Coordinate functions are not especially difficult to understand or implement
// but take a lot of code writing. You are free to explore all the inline code
// underneath the API but there really isn't much to see.
// /////////////////////////////////////



#include "../../NAMathOperators.h"
#include "../../../NAUtility/NAValueHelper.h"


#include "NABorderII.h"
#include "NABoxII.h"
#include "NABoxCombinationII.h"
#include "NACoordAlignII.h"
#include "NACoordCalculationII.h"
#include "NACoordClampingII.h"
#include "NACoordComparisonII.h"
#include "NACoordEndMaxII.h"
#include "NACoordIndexII.h"
#include "NACoordMacOSII.h"
#include "NACoordTesterII.h"
#include "NAPosII.h"
#include "NARangeII.h"
#include "NARangeCombinationII.h"
#include "NARectII.h"
#include "NARectCombinationII.h"
#include "NASizeII.h"
#include "NAVertexII.h"
#include "NAVolumeII.h"








// ///////////////////////////////////////
// ADD SUB
// ///////////////////////////////////////

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



NA_IDEF double naDistancePosPos(NAPos pos1, NAPos pos2) {
  double dist[2];
  dist[0] = pos2.x - pos1.x;
  dist[1] = pos2.y - pos1.y;
  return naSqrt(dist[0]*dist[0] + dist[1]*dist[1]);
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



// ///////////////////////////////////////
// CONTAIN
// ///////////////////////////////////////

NA_IDEF NABool naContainsRangeOffset(NARange outerRange, double offset) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(offset))
      naError("offset is invalid.");
    if(!naIsRangeUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerRange.origin)
        || (offset > naGetRangeEnd(outerRange)));
}
NA_IDEF NABool naContainsRangefOffset(NARangef outerRange, float offset) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(offset))
      naError("offset is invalid.");
    if(!naIsRangefUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerRange.origin)
        || (offset > naGetRangefEnd(outerRange)));
}
NA_IDEF NABool naContainsRangei32Offset(NARangei32 outerRange, int32 offset) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(offset))
      naError("offset is invalid.");
    if(!naIsRangei32Useful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerRange.origin)
        || (offset > naGetRangei32Max(outerRange)));
}
NA_IDEF NABool naContainsRangei64Offset(NARangei64 outerRange, int64 offset) {
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(offset))
      naError("offset is invalid.");
    if(!naIsRangei64Useful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerRange.origin)
        || (offset > naGetRangei64Max(outerRange)));
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
  return  ((range.origin           >= outerRange.origin)
        && (naGetRangei64End(range) <=  naGetRangei64End(outerRange)));
}




NA_IDEF NABool naContainsSizePos(NASize outerSize, NAPos pos) {
  #if NA_DEBUG
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
    if(!naIsSizeUseful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  return !((pos.x < 0)
        || (pos.x > outerSize.width)
        || (pos.y < 0)
        || (pos.y > outerSize.height));
}
NA_IDEF NABool naContainsSizei32Pos(NASizei32 outerSize, NAPosi32 pos) {
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
    if(naIsSizei32Useful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  return  ((pos.x >= 0)
        && (pos.x <  outerSize.width)
        && (pos.y >= 0)
        && (pos.y <  outerSize.height));
}
NA_IDEF NABool naContainsSizei64Pos(NASizei64 outerSize, NAPosi64 pos) {
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
    if(naIsSizei64Useful(outerSize))
      naError("Inside test not valid for sizes which are not useful.");
  #endif
  return  ((pos.x >= 0)
        && (pos.x <  outerSize.width)
        && (pos.y >= 0)
        && (pos.y <  outerSize.height));
}



NA_IDEF NABool naContainsRectPos(NARect outerRect, NAPos pos) {
  #if NA_DEBUG
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
    if(naIsRectEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return !((pos.x < outerRect.pos.x)
        || (pos.x > naGetRectEndX(outerRect))
        || (pos.y < outerRect.pos.y)
        || (pos.y > naGetRectEndY(outerRect)));
}
NA_IDEF NABool naContainsRecti32Pos(NARecti32 outerRect, NAPosi32 pos) {
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
    if(naIsRecti32EmptySlow(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti32Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((pos.x >= outerRect.pos.x)
        && (pos.x <  naGetRecti32EndX(outerRect))
        && (pos.y >= outerRect.pos.y)
        && (pos.y <  naGetRecti32EndY(outerRect)));
}
NA_IDEF NABool naContainsRecti64Pos(NARecti64 outerRect, NAPosi64 pos) {
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
    if(naIsRecti64EmptySlow(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRecti64Valid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((pos.x >= outerRect.pos.x)
        && (pos.x <  naGetRecti64EndX(outerRect))
        && (pos.y >= outerRect.pos.y)
        && (pos.y <  naGetRecti64EndY(outerRect)));
}
NA_IDEF NABool naContainsRectsPos(NARects outerRect, NAPoss pos) {
  #if NA_DEBUG
    if(!naIsPossValid(pos))
      naError("pos is invalid.");
    if(naIsRectsEmptySlow(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectsValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((pos.x >= outerRect.pos.x)
        && (pos.x <  naGetRectsEndX(outerRect))
        && (pos.y >= outerRect.pos.y)
        && (pos.y <  naGetRectsEndY(outerRect)));
}



NA_IDEF NABool naContainsRectPosE(NARect outerRect, NAPos pos) {
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
  return !((size.width  > outerSize.width)
        || (size.height > outerSize.height));
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
  return  ((rect.pos.x           >= outerRect.pos.x)
        && (naGetRecti64EndX(rect) <=  naGetRecti64EndX(outerRect))
        && (rect.pos.y           >= outerRect.pos.y)
        && (naGetRecti64EndY(rect) <=  naGetRecti64EndY(outerRect)));
}



NA_IDEF NABool naContainsVolumeVertex(NAVolume outerVolume, NAVertex vertex) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumeUseful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  return !((vertex.x < 0)
        || (vertex.x > outerVolume.width)
        || (vertex.y < 0)
        || (vertex.y > outerVolume.height)
        || (vertex.z < 0)
        || (vertex.z > outerVolume.depth));
}
NA_IDEF NABool naContainsVolumei32Vertex(NAVolumei32 outerVolume, NAVertexi32 vertex) {
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
    if(naIsVolumei32Useful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  return  ((vertex.x >= 0)
        && (vertex.x <  outerVolume.width)
        && (vertex.y >= 0)
        && (vertex.y <  outerVolume.height)
        && (vertex.z >= 0)
        && (vertex.z <  outerVolume.depth));
}
NA_IDEF NABool naContainsVolumei64Vertex(NAVolumei64 outerVolume, NAVertexi64 vertex) {
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
    if(naIsVolumei64Useful(outerVolume))
      naError("Inside test not valid for volumes which are not useful.");
  #endif
  return  ((vertex.x >= 0)
        && (vertex.x <  outerVolume.width)
        && (vertex.y >= 0)
        && (vertex.y <  outerVolume.height)
        && (vertex.z >= 0)
        && (vertex.z <  outerVolume.depth));
}



NA_IDEF NABool naContainsBoxVertex(NABox outerBox, NAVertex vertex) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxEmpty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((vertex.x < outerBox.vertex.x)
        || (vertex.x > naGetBoxEndX(outerBox))
        || (vertex.y < outerBox.vertex.y)
        || (vertex.y > naGetBoxEndY(outerBox))
        || (vertex.z < outerBox.vertex.z)
        || (vertex.z > naGetBoxEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxVertexE(NABox outerBox, NAVertex vertex) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxEmpty(outerBox))
      naError("Inside test not valid for empty rects.");
    if(!naIsBoxValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return !((vertex.x < outerBox.vertex.x)
        || (vertex.x >= naGetBoxEndX(outerBox))
        || (vertex.y < outerBox.vertex.y)
        || (vertex.y >= naGetBoxEndY(outerBox))
        || (vertex.z < outerBox.vertex.z)
        || (vertex.z >= naGetBoxEndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi32Vertex(NABoxi32 outerBox, NAVertexi32 vertex) {
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxi32EmptySlow(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxi32Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((vertex.x >= outerBox.vertex.x)
        && (vertex.x <  naGetBoxi32EndX(outerBox))
        && (vertex.y >= outerBox.vertex.y)
        && (vertex.y <  naGetBoxi32EndY(outerBox))
        && (vertex.z >= outerBox.vertex.z)
        && (vertex.z <  naGetBoxi32EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxi64Vertex(NABoxi64 outerBox, NAVertexi64 vertex) {
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxi64EmptySlow(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxi64Valid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((vertex.x >= outerBox.vertex.x)
        && (vertex.x <  naGetBoxi64EndX(outerBox))
        && (vertex.y >= outerBox.vertex.y)
        && (vertex.y <  naGetBoxi64EndY(outerBox))
        && (vertex.z >= outerBox.vertex.z)
        && (vertex.z <  naGetBoxi64EndZ(outerBox)));
}
NA_IDEF NABool naContainsBoxsVertex(NABoxs outerBox, NAVertexs vertex) {
  #if NA_DEBUG
    if(!naIsVertexsValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxsEmptySlow(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxsValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((vertex.x >= outerBox.vertex.x)
        && (vertex.x <  naGetBoxsEndX(outerBox))
        && (vertex.y >= outerBox.vertex.y)
        && (vertex.y <  naGetBoxsEndY(outerBox))
        && (vertex.z >= outerBox.vertex.z)
        && (vertex.z <  naGetBoxsEndZ(outerBox)));
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
  return !((volume.width  > outerVolume.width)
        || (volume.height > outerVolume.height)
        || (volume.depth  > outerVolume.depth));
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
  return  ((box.vertex.x           >= outerBox.vertex.x)
        && (naGetBoxi64EndX(box) <=  naGetBoxi64EndX(outerBox))
        && (box.vertex.y           >= outerBox.vertex.y)
        && (naGetBoxi64EndY(box) <=  naGetBoxi64EndY(outerBox))
        && (box.vertex.z           >= outerBox.vertex.z)
        && (naGetBoxi64EndZ(box) <=  naGetBoxi64EndZ(outerBox)));
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
