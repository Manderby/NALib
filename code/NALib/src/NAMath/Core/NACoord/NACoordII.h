
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


#include "NABezelII.h"
#include "NABoxII.h"
#include "NACoordAlignII.h"
#include "NACoordCombinationII.h"
#include "NACoordEndMaxII.h"
#include "NACoordIndexII.h"
#include "NACoordMacOSII.h"
#include "NACoordTesterII.h"
#include "NAPosII.h"
#include "NARangeII.h"
#include "NARectII.h"
#include "NASizeII.h"
#include "NAVertexII.h"
#include "NAVolumeII.h"



NA_IDEF NABox naMakeBoxWithVertexAndVertex(NAVertex vertex1, NAVertex vertex2) {
  NABox newBox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  if(vertex2.x > vertex1.x) {
    newBox.vertex.x = vertex1.x;
    newBox.volume.width = naMakeLengthWithStartAndEnd(vertex1.x, vertex2.x);
  }else{
    newBox.vertex.x = vertex2.x;
    newBox.volume.width = naMakeLengthWithStartAndEnd(vertex2.x, vertex1.x);
  }
  if(vertex2.y > vertex1.y) {
    newBox.vertex.y = vertex1.y;
    newBox.volume.height = naMakeLengthWithStartAndEnd(vertex1.y, vertex2.y);
  }else{
    newBox.vertex.y = vertex2.y;
    newBox.volume.height = naMakeLengthWithStartAndEnd(vertex2.y, vertex1.y);
  }
  if(vertex2.z > vertex1.z) {
    newBox.vertex.z = vertex1.z;
    newBox.volume.depth = naMakeLengthWithStartAndEnd(vertex1.z, vertex2.z);
  }else{
    newBox.vertex.z = vertex2.z;
    newBox.volume.depth = naMakeLengthWithStartAndEnd(vertex2.z, vertex1.z);
  }
  return newBox;
}
NA_IDEF NABox naMakeBoxWithBoxAndVertex(NABox box, NAVertex vertex) {
  NABox newBox;
  double end;
  #if NA_DEBUG
    if(naIsBoxEmpty(box))
      naError("box is empty.");
    if(!naIsBoxUseful(box))
      naError("box is not useful.");
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
  #endif
  newBox.vertex.x = naMin(box.vertex.x, vertex.x);
  newBox.vertex.y = naMin(box.vertex.y, vertex.y);
  newBox.vertex.z = naMin(box.vertex.z, vertex.z);
  end = naGetBoxEndX(box);
  newBox.volume.width  = naMakeLengthWithStartAndEnd(newBox.vertex.x, naMax(end, vertex.x));
  end = naGetBoxEndY(box);
  newBox.volume.height = naMakeLengthWithStartAndEnd(newBox.vertex.y, naMax(end, vertex.y));
  end = naGetBoxEndZ(box);
  newBox.volume.depth = naMakeLengthWithStartAndEnd(newBox.vertex.z, naMax(end, vertex.z));
  return newBox;
}
NA_IDEF NABox naMakeBoxWithBoxAndBox(NABox box1, NABox box2) {
  NABox newBox;
  double end1;
  double end2;
  #if NA_DEBUG
    if(naIsBoxEmpty(box1))
      naError("box1 is empty.");
    if(!naIsBoxValid(box1))
      naError("box1 is invalid.");
    if(naIsBoxEmpty(box2))
      naError("box2 is empty.");
    if(!naIsBoxValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMin(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMin(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMin(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEnd(newBox.vertex.x, naMax(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newBox.volume.height  = naMakeLengthWithStartAndEnd(newBox.vertex.y, naMax(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEnd(newBox.vertex.z, naMax(end1, end2));
  return newBox;
}
NA_IDEF NABox naMakeBoxWithBoxUnionE(NABox box1, NABox box2) {
  NABox newBox;
  double end1;
  double end2;
  #if NA_DEBUG
    if(!naIsBoxValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("box2 is invalid.");
  #endif
  
  if(naIsBoxEmpty(box1))
    return box2;
  if(naIsBoxEmpty(box2))
    return box1;
  
  newBox.vertex.x = naMin(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMin(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMin(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEnd(newBox.vertex.x, naMax(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newBox.volume.height  = naMakeLengthWithStartAndEnd(newBox.vertex.y, naMax(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEnd(newBox.vertex.z, naMax(end1, end2));
  return newBox;
}



NA_IDEF NABoxi32 naMakeBoxi32WithBoxAndVertex(NABoxi32 box, NAVertexi32 vertex) {
  NABoxi32 newBox;
  int32 max;
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Useful(box))
      naError("box is not useful.");
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
  #endif
  newBox.vertex.x = naMini32(box.vertex.x, vertex.x);
  newBox.vertex.y = naMini32(box.vertex.y, vertex.y);
  newBox.vertex.z = naMini32(box.vertex.z, vertex.z);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetBoxi32MaxX(box);
  newBox.volume.width  = naMakeLengthWithMinAndMaxi32(newBox.vertex.x, naMaxi32(max, vertex.x));
  max = naGetBoxi32MaxY(box);
  newBox.volume.height = naMakeLengthWithMinAndMaxi32(newBox.vertex.y, naMaxi32(max, vertex.y));
  max = naGetBoxi32MaxZ(box);
  newBox.volume.depth = naMakeLengthWithMinAndMaxi32(newBox.vertex.z, naMaxi32(max, vertex.z));
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64WithBoxAndVertex(NABoxi64 box, NAVertexi64 vertex) {
  NABoxi64 newBox;
  int64 max;
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Useful(box))
      naError("box is not useful.");
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
  #endif
  newBox.vertex.x = naMini64(box.vertex.x, vertex.x);
  newBox.vertex.y = naMini64(box.vertex.y, vertex.y);
  newBox.vertex.z = naMini64(box.vertex.z, vertex.z);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetBoxi64MaxX(box);
  newBox.volume.width  = naMakeLengthWithMinAndMaxi64(newBox.vertex.x, naMaxi64(max, vertex.x));
  max = naGetBoxi64MaxY(box);
  newBox.volume.height = naMakeLengthWithMinAndMaxi64(newBox.vertex.y, naMaxi64(max, vertex.y));
  max = naGetBoxi64MaxZ(box);
  newBox.volume.depth = naMakeLengthWithMinAndMaxi64(newBox.vertex.z, naMaxi64(max, vertex.z));
  return newBox;
}



NA_IDEF NABoxi32 naMakeBoxi32WithBoxAndVertexE(NABoxi32 box, NAVertexi32 vertex) {
  NABoxi32 newBox;
  #if NA_DEBUG
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
  #endif
  if(naIsBoxi32Empty(box)) {
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumei32(1, 1, 1);
  }else{
    int32 max;
    newBox.vertex.x = naMini32(box.vertex.x, vertex.x);
    newBox.vertex.y = naMini32(box.vertex.y, vertex.y);
    newBox.vertex.z = naMini32(box.vertex.z, vertex.z);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetBoxi32MaxX(box);
    newBox.volume.width  = naMakeLengthWithMinAndMaxi32(newBox.vertex.x, naMaxi32(max, vertex.x));
    max = naGetBoxi32MaxY(box);
    newBox.volume.height = naMakeLengthWithMinAndMaxi32(newBox.vertex.y, naMaxi32(max, vertex.y));
    max = naGetBoxi32MaxZ(box);
    newBox.volume.depth = naMakeLengthWithMinAndMaxi32(newBox.vertex.z, naMaxi32(max, vertex.z));
  }
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64WithBoxAndVertexE(NABoxi64 box, NAVertexi64 vertex) {
  NABoxi64 newBox;
  #if NA_DEBUG
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
  #endif
  if(naIsBoxi64Empty(box)) {
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumei64(1, 1, 1);
  }else{
    int64 max;
    newBox.vertex.x = naMini64(box.vertex.x, vertex.x);
    newBox.vertex.y = naMini64(box.vertex.y, vertex.y);
    newBox.vertex.z = naMini64(box.vertex.z, vertex.z);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetBoxi64MaxX(box);
    newBox.volume.width  = naMakeLengthWithMinAndMaxi64(newBox.vertex.x, naMaxi64(max, vertex.x));
    max = naGetBoxi64MaxY(box);
    newBox.volume.height = naMakeLengthWithMinAndMaxi64(newBox.vertex.y, naMaxi64(max, vertex.y));
    max = naGetBoxi64MaxZ(box);
    newBox.volume.depth = naMakeLengthWithMinAndMaxi64(newBox.vertex.z, naMaxi64(max, vertex.z));
  }
  return newBox;
}



NA_IDEF NABoxi32 naMakeBoxi32WithBoxUnion(NABoxi32 box1, NABoxi32 box2) {
  NABoxi32 newBox;
  int32 end1;
  int32 end2;
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box1))
      naError("box1 is empty.");
    if(!naIsBoxi32Valid(box1))
      naError("box1 is invalid.");
    if(naIsBoxi32EmptySlow(box2))
      naError("box2 is empty.");
    if(!naIsBoxi32Valid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMini32(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMini32(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMini32(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxi32EndX(box1);
  end2 = naGetBoxi32EndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi32(newBox.vertex.x, naMaxi32(end1, end2));
  end1 = naGetBoxi32EndY(box1);
  end2 = naGetBoxi32EndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndi32(newBox.vertex.y, naMaxi32(end1, end2));
  end1 = naGetBoxi32EndZ(box1);
  end2 = naGetBoxi32EndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEndi32(newBox.vertex.z, naMaxi32(end1, end2));
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64WithBoxUnion(NABoxi64 box1, NABoxi64 box2) {
  NABoxi64 newBox;
  int64 end1;
  int64 end2;
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box1))
      naError("box1 is empty.");
    if(!naIsBoxi64Valid(box1))
      naError("box1 is invalid.");
    if(naIsBoxi64EmptySlow(box2))
      naError("box2 is empty.");
    if(!naIsBoxi64Valid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMini64(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMini64(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMini64(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxi64EndX(box1);
  end2 = naGetBoxi64EndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi64(newBox.vertex.x, naMaxi64(end1, end2));
  end1 = naGetBoxi64EndY(box1);
  end2 = naGetBoxi64EndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndi64(newBox.vertex.y, naMaxi64(end1, end2));
  end1 = naGetBoxi64EndZ(box1);
  end2 = naGetBoxi64EndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEndi64(newBox.vertex.z, naMaxi64(end1, end2));
  return newBox;
}



NA_IDEF NABoxi32 naMakeBoxi32WithBoxUnionE(NABoxi32 box1, NABoxi32 box2) {
  NABoxi32 newBox;
  int32 end1;
  int32 end2;
  #if NA_DEBUG
    if(!naIsBoxi32Valid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxi32Valid(box2))
      naError("box2 is invalid.");
  #endif
  
  if(naIsBoxi32Empty(box1))
    return box2;
  if(naIsBoxi32Empty(box2))
    return box1;
  
  newBox.vertex.x = naMini32(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMini32(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMini32(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxi32EndX(box1);
  end2 = naGetBoxi32EndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi32(newBox.vertex.x, naMaxi32(end1, end2));
  end1 = naGetBoxi32EndY(box1);
  end2 = naGetBoxi32EndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndi32(newBox.vertex.y, naMaxi32(end1, end2));
  end1 = naGetBoxi32EndZ(box1);
  end2 = naGetBoxi32EndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEndi32(newBox.vertex.z, naMaxi32(end1, end2));
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64WithBoxUnionE(NABoxi64 box1, NABoxi64 box2) {
  NABoxi64 newBox;
  int64 end1;
  int64 end2;
  #if NA_DEBUG
    if(!naIsBoxi64Valid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxi64Valid(box2))
      naError("box2 is invalid.");
  #endif
  
  if(naIsBoxi64Empty(box1))
    return box2;
  if(naIsBoxi64Empty(box2))
    return box1;
  
  newBox.vertex.x = naMini64(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMini64(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMini64(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxi64EndX(box1);
  end2 = naGetBoxi64EndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi64(newBox.vertex.x, naMaxi64(end1, end2));
  end1 = naGetBoxi64EndY(box1);
  end2 = naGetBoxi64EndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndi64(newBox.vertex.y, naMaxi64(end1, end2));
  end1 = naGetBoxi64EndZ(box1);
  end2 = naGetBoxi64EndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEndi64(newBox.vertex.z, naMaxi64(end1, end2));
  return newBox;
}






NA_IDEF NABox naMakeBoxWithBoxIntersection(NABox box1, NABox box2) {
  NABox newBox;
  double end1;
  double end2;
  #if NA_DEBUG
    if(!naIsBoxValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMax(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMax(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMax(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEnd(newBox.vertex.x, naMin(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEnd(newBox.vertex.y, naMin(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEnd(newBox.vertex.z, naMin(end1, end2));
  return newBox;
}

NA_IDEF NABoxi32 naMakeBoxi32WithBoxIntersection(NABoxi32 box1, NABoxi32 box2) {
  NABoxi32 newBox;
  int32 end1;
  int32 end2;
  #if NA_DEBUG
    if(!naIsBoxi32Valid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxi32Valid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMaxi32(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMaxi32(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMaxi32(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxi32EndX(box1);
  end2 = naGetBoxi32EndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi32(newBox.vertex.x, naMini32(end1, end2));
  end1 = naGetBoxi32EndY(box1);
  end2 = naGetBoxi32EndY(box2);
  newBox.volume.height  = naMakeLengthWithStartAndEndi32(newBox.vertex.y, naMini32(end1, end2));
  end1 = naGetBoxi32EndZ(box1);
  end2 = naGetBoxi32EndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEndi32(newBox.vertex.z, naMini32(end1, end2));
  return newBox;
}

NA_IDEF NABoxi64 naMakeBoxi64WithBoxIntersection(NABoxi64 box1, NABoxi64 box2) {
  NABoxi64 newBox;
  int64 end1;
  int64 end2;
  #if NA_DEBUG
    if(!naIsBoxi64Valid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxi64Valid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMaxi64(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMaxi64(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMaxi64(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxi64EndX(box1);
  end2 = naGetBoxi64EndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi64(newBox.vertex.x, naMini64(end1, end2));
  end1 = naGetBoxi64EndY(box1);
  end2 = naGetBoxi64EndY(box2);
  newBox.volume.height  = naMakeLengthWithStartAndEndi64(newBox.vertex.y, naMini64(end1, end2));
  end1 = naGetBoxi64EndZ(box1);
  end2 = naGetBoxi64EndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEndi64(newBox.vertex.z, naMini64(end1, end2));
  return newBox;
}



NA_IDEF NARect naMakeRectWithRectAndBorder(NARect rect, double border) {
  NARect newRect;
  newRect.pos.x = rect.pos.x - border;
  newRect.pos.y = rect.pos.y - border;
  newRect.size.width = rect.size.width + 2. * border;
  newRect.size.height = rect.size.height + 2. * border;
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32WithRectAndBorder(NARecti32 rect, int32 border) {
  NARecti32 newRect;
  newRect.pos.x = rect.pos.x - border;
  newRect.pos.y = rect.pos.y - border;
  newRect.size.width = rect.size.width + 2 * border;
  newRect.size.height = rect.size.height + 2 * border;
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64WithRectAndBorder(NARecti64 rect, int64 border) {
  NARecti64 newRect;
  newRect.pos.x = rect.pos.x - border;
  newRect.pos.y = rect.pos.y - border;
  newRect.size.width = rect.size.width + 2 * border;
  newRect.size.height = rect.size.height + 2 * border;
  return newRect;
}



NA_IDEF NABox naMakeBoxWithBoxAndBorder(NABox box, double border) {
  NABox newBox;
  newBox.vertex.x = box.vertex.x - border;
  newBox.vertex.y = box.vertex.y - border;
  newBox.vertex.z = box.vertex.z - border;
  newBox.volume.width = box.volume.width + 2. * border;
  newBox.volume.height = box.volume.height + 2. * border;
  newBox.volume.depth = box.volume.depth + 2. * border;
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32WithBoxAndBorder(NABoxi32 box, int32 border) {
  NABoxi32 newBox;
  newBox.vertex.x = box.vertex.x - border;
  newBox.vertex.y = box.vertex.y - border;
  newBox.vertex.z = box.vertex.z - border;
  newBox.volume.width = box.volume.width + 2 * border;
  newBox.volume.height = box.volume.height + 2 * border;
  newBox.volume.depth = box.volume.depth + 2 * border;
  return newBox;
}
NA_IDEF NABoxi64 naMakeBoxi64WithBoxAndBorder(NABoxi64 box, int64 border) {
  NABoxi64 newBox;
  newBox.vertex.x = box.vertex.x - border;
  newBox.vertex.y = box.vertex.y - border;
  newBox.vertex.z = box.vertex.z - border;
  newBox.volume.width = box.volume.width + 2 * border;
  newBox.volume.height = box.volume.height + 2 * border;
  newBox.volume.depth = box.volume.depth + 2 * border;
  return newBox;
}



NA_IDEF NARange naMakeRangePositive(NARange range) {
  double rangeNegative = (range.length < 0) ? 1. : 0.;
  range.origin += rangeNegative * range.length;
  range.length -= rangeNegative * 2. * range.length;
  return range;
}
NA_IDEF NARangei32 naMakeRangei32Positive(NARangei32 range) {
  int32 rangeNegative = (int32)(range.length < 0);
  range.origin += rangeNegative * range.length;
  range.length -= rangeNegative * 2 * range.length;
  return range;
}
NA_IDEF NARangei64 naMakeRangei64Positive(NARangei64 range) {
  int64 rangeNegative = (int64)(range.length < 0);
  range.origin += rangeNegative * range.length;
  range.length -= rangeNegative * 2 * range.length;
  return range;
}



NA_IDEF NARect naMakeRectPositive(NARect rect) {
  NARange range1 = naMakeRangePositive(naMakeRangeE(rect.pos.x, rect.size.width));
  NARange range2 = naMakeRangePositive(naMakeRangeE(rect.pos.y, rect.size.height));
  return naMakeRectS(range1.origin, range2.origin, range1.length, range2.length);
}
NA_IDEF NARecti32 naMakeRecti32Positive(NARecti32 rect) {
  NARangei32 range1 = naMakeRangei32Positive(naMakeRangei32E(rect.pos.x, rect.size.width));
  NARangei32 range2 = naMakeRangei32Positive(naMakeRangei32E(rect.pos.y, rect.size.height));
  return naMakeRecti32S(range1.origin, range2.origin, range1.length, range2.length);
}
NA_IDEF NARecti64 naMakeRecti64Positive(NARecti64 rect) {
  NARangei64 range1 = naMakeRangei64Positive(naMakeRangei64E(rect.pos.x, rect.size.width));
  NARangei64 range2 = naMakeRangei64Positive(naMakeRangei64E(rect.pos.y, rect.size.height));
  return naMakeRecti64S(range1.origin, range2.origin, range1.length, range2.length);
}



NA_IDEF NABoxi32 naMakeBoxi32Positive(NABoxi32 box) {
  NARangei32 range1 = naMakeRangei32Positive(naMakeRangei32E(box.vertex.x, box.volume.width));
  NARangei32 range2 = naMakeRangei32Positive(naMakeRangei32E(box.vertex.y, box.volume.height));
  NARangei32 range3 = naMakeRangei32Positive(naMakeRangei32E(box.vertex.y, box.volume.height));
  return naMakeBoxi32S(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}
NA_IDEF NABoxi64 naMakeBoxi64Positive(NABoxi64 box) {
  NARangei64 range1 = naMakeRangei64Positive(naMakeRangei64E(box.vertex.x, box.volume.width));
  NARangei64 range2 = naMakeRangei64Positive(naMakeRangei64E(box.vertex.y, box.volume.height));
  NARangei64 range3 = naMakeRangei64Positive(naMakeRangei64E(box.vertex.y, box.volume.height));
  return naMakeBoxi64S(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}




// /////////////////////////////////
// GET
// /////////////////////////////////

NA_IDEF NAPos naGetRectCenter(NARect rect) {
  NAPos newPos;
  #if NA_DEBUG
    if(!naIsRectUseful(rect))
      naError("rect is not useful");
  #endif
  newPos.x = rect.pos.x + 0.5 * rect.size.width;
  newPos.y = rect.pos.y + 0.5 * rect.size.height;
  return newPos;
}
NA_IDEF NAPosi32 naGetRecti32Center(NARecti32 rect) {
  NAPosi32 newPos;
  #if NA_DEBUG
    if(!naIsRecti32Useful(rect))
      naError("rect is not useful");
  #endif
  newPos.x = rect.pos.x + rect.size.width / 2;
  newPos.y = rect.pos.y + rect.size.height / 2;
  return newPos;
}
NA_IDEF NAPosi64 naGetRecti64Center(NARecti64 rect) {
  NAPosi64 newPos;
  #if NA_DEBUG
    if(!naIsRecti64Useful(rect))
      naError("rect is not useful");
  #endif
  newPos.x = rect.pos.x + rect.size.width / 2;
  newPos.y = rect.pos.y + rect.size.height / 2;
  return newPos;
}



NA_IDEF NAVertex naGetBoxCenter(NABox box) {
  NAVertex newVertex;
  #if NA_DEBUG
    if(!naIsBoxUseful(box))
      naError("box is not useful");
  #endif
  newVertex.x = box.vertex.x + 0.5 * box.volume.width;
  newVertex.y = box.vertex.y + 0.5 * box.volume.height;
  newVertex.z = box.vertex.z + 0.5 * box.volume.depth;
  return newVertex;
}
NA_IDEF NAVertexi32 naGetBoxi32Center(NABoxi32 box) {
  NAVertexi32 newVertex;
  #if NA_DEBUG
    if(!naIsBoxi32Useful(box))
      naError("box is not useful");
  #endif
  newVertex.x = box.vertex.x + box.volume.width / 2;
  newVertex.y = box.vertex.y + box.volume.height / 2;
  newVertex.z = box.vertex.z + box.volume.depth / 2;
  return newVertex;
}
NA_IDEF NAVertexi64 naGetBoxi64Center(NABoxi64 box) {
  NAVertexi64 newVertex;
  #if NA_DEBUG
    if(!naIsBoxi64Useful(box))
      naError("box is not useful");
  #endif
  newVertex.x = box.vertex.x + box.volume.width / 2;
  newVertex.y = box.vertex.y + box.volume.height / 2;
  newVertex.z = box.vertex.z + box.volume.depth / 2;
  return newVertex;
}



NA_IDEF NAPos naGetRectCenteredSizeOffset(NARect rect, NASize size) {
  NAPos newPos;
  newPos.x = rect.pos.x + 0.5 * (rect.size.width - size.width);
  newPos.y = rect.pos.y + 0.5 * (rect.size.height - size.height);
  return newPos;
}
NA_IDEF NAVertex naGetBoxCeneteredVolumeOffset(NABox box, NAVolume volume) {
  NAVertex newVertex;
  newVertex.x = box.vertex.x + 0.5 * (box.volume.width - volume.width);
  newVertex.y = box.vertex.y + 0.5 * (box.volume.height - volume.height);
  newVertex.z = box.vertex.z + 0.5 * (box.volume.depth - volume.depth);
  return newVertex;
}



// /////////////////////////////////
// CLAMP
// /////////////////////////////////

NA_IDEF NARangei32 naClampRangei32ToRange(NARangei32 range, NARangei32 clampRange) {
  int32 value;
  NARangei32 newRange;
  #if NA_DEBUG
    if(naIsRangei32Empty(range))
      naError("range is empty.");
    if(!naIsRangei32Valid(range))
      naError("range is invalid.");
    if(naIsRangei32Empty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangei32Valid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0) {
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangei32End(clampRange);
  if(naGetRangei32End(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}
NA_IDEF NARangei64 naClampRangei64ToRange(NARangei64 range, NARangei64 clampRange) {
  int64 value;
  NARangei64 newRange;
  #if NA_DEBUG
    if(naIsRangei64Empty(range))
      naError("range is empty.");
    if(!naIsRangei64Valid(range))
      naError("range is invalid.");
    if(naIsRangei64Empty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangei64Valid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0) {
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangei64End(clampRange);
  if(naGetRangei64End(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}
NA_IDEF NARangei32 naClampRangei32ToMin(NARangei32 range, int32 min) {
  int32 value;
  NARangei32 newRange;
  #if NA_DEBUG
    if(naIsRangei32Empty(range))
      naError("range is empty.");
    if(!naIsRangei32Valid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi32(min))
      naError("min is invalid.");
  #endif
  newRange = range;

  value = min - newRange.origin;
  if(value > 0) {
    newRange.length -= value;
    newRange.origin = min;
  }
  return newRange;
}
NA_IDEF NARangei64 naClampRangei64ToMin(NARangei64 range, int64 min) {
  int64 value;
  NARangei64 newRange;
  #if NA_DEBUG
    if(naIsRangei64Empty(range))
      naError("range is empty.");
    if(!naIsRangei64Valid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi64(min))
      naError("min is invalid.");
  #endif
  newRange = range;

  value = min - newRange.origin;
  if(value > 0) {
    newRange.length -= value;
    newRange.origin = min;
  }
  return newRange;
}
NA_IDEF NARangei32 naClampRangei32ToEnd(NARangei32 range, int32 end) {
  int32 value;
  NARangei32 newRange;
  #if NA_DEBUG
    if(naIsRangei32Empty(range))
      naError("range is empty.");
    if(!naIsRangei32Valid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi32(end))
      naError("min is invalid.");
  #endif
  newRange = range;

  value = end;
  if(naGetRangei32End(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}
NA_IDEF NARangei64 naClampRangei64ToEnd(NARangei64 range, int64 end) {
  int64 value;
  NARangei64 newRange;
  #if NA_DEBUG
    if(naIsRangei64Empty(range))
      naError("range is empty.");
    if(!naIsRangei64Valid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi64(end))
      naError("min is invalid.");
  #endif
  newRange = range;

  value = end;
  if(naGetRangei64End(newRange) > value) {
    newRange.length = value - newRange.origin;
  }

  return newRange;
}



NA_IDEF NAPos naClampPosToRect(NAPos pos, NARect clampRect) {
  double end;
  NAPos newPos;
  #if NA_DEBUG
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
    if(naIsRectEmpty(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPos = pos;
  if(newPos.x < clampRect.pos.x) { newPos.x = clampRect.pos.x; }
  end = naGetRectEndX(clampRect);
  if(newPos.x > end) {
    newPos.x = end;
  }
  if(newPos.y < clampRect.pos.y) { newPos.y = clampRect.pos.y; }
  end = naGetRectEndY(clampRect);
  if(newPos.y > end) {
    newPos.y = end;
  }
  return newPos;
}

NA_IDEF NAPosi32 naClampPosi32ToRect(NAPosi32 pos, NARecti32 clampRect) {
  int32 max;
  NAPosi32 newPos;
  #if NA_DEBUG
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
    if(naIsRecti32EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti32Valid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPos = pos;
  if(newPos.x <  clampRect.pos.x) { newPos.x = clampRect.pos.x; }
  max = naGetRecti32MaxX(clampRect);
  if(newPos.x > max) {
    newPos.x = max;
  }
  if(newPos.y <  clampRect.pos.y) { newPos.y = clampRect.pos.y; }
  max = naGetRecti32MaxY(clampRect);
  if(newPos.y > max) {
    newPos.y = max;
  }
  return newPos;
}

NA_IDEF NAPosi64 naClampPosi64ToRect(NAPosi64 pos, NARecti64 clampRect) {
  int64 max;
  NAPosi64 newPos;
  #if NA_DEBUG
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
    if(naIsRecti64EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti64Valid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPos = pos;
  if(newPos.x <  clampRect.pos.x) { newPos.x = clampRect.pos.x; }
  max = naGetRecti64MaxX(clampRect);
  if(newPos.x > max) {
    newPos.x = max;
  }
  if(newPos.y <  clampRect.pos.y) { newPos.y = clampRect.pos.y; }
  max = naGetRecti64MaxY(clampRect);
  if(newPos.y > max) {
    newPos.y = max;
  }
  return newPos;
}



NA_IDEF NARect naClampRect(NARect rect, NARect clampRect) {
  // Todo: use helper functions in this function.
  double value;
  NARect newRect;
  #if NA_DEBUG
    if(naIsRectEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectValid(rect))
      naError("rect is invalid.");
    if(naIsRectEmpty(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newRect = rect;
  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRectEndX(clampRect);
  if(naGetRectEndXE(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRectEndY(clampRect);
  if(naGetRectEndYE(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
  }
  return newRect;
}
NA_IDEF NARecti32 naClampRecti32(NARecti32 rect, NARecti32 clampRect) {
  // Todo: use helper functions in this function.
  int32 value;
  NARecti32 newRect;
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
    if(naIsRecti32EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti32Valid(clampRect))
      naError("clampRect is invalid.");
  #endif

  newRect = rect;

  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    if(newRect.size.width <= 0) {
      return newRect;
    }
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRecti32EndX(clampRect);
  if(naGetRecti32EndX(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
    if(newRect.size.width <= 0) {
      return newRect;
    }
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    if(newRect.size.height <= 0) {
      return newRect;
    }
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRecti32EndY(clampRect);
  if(naGetRecti32EndY(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
    if(newRect.size.height <= 0) {
      return newRect;
    }
  }
  return newRect;
}
NA_IDEF NARecti64 naClampRecti64(NARecti64 rect, NARecti64 clampRect) {
  // Todo: use helper functions in this function.
  int64 value;
  NARecti64 newRect;
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
    if(naIsRecti64EmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRecti64Valid(clampRect))
      naError("clampRect is invalid.");
  #endif

  newRect = rect;

  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0) {
    newRect.size.width -= value;
    if(newRect.size.width <= 0) {
      return newRect;
    }
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRecti64EndX(clampRect);
  if(naGetRecti64EndX(newRect) > value) {
    newRect.size.width = value - newRect.pos.x;
    if(newRect.size.width <= 0) {
      return newRect;
    }
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0) {
    newRect.size.height -= value;
    if(newRect.size.height <= 0) {
      return newRect;
    }
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRecti64EndY(clampRect);
  if(naGetRecti64EndY(newRect) > value) {
    newRect.size.height = value - newRect.pos.y;
    if(newRect.size.height <= 0) {
      return newRect;
    }
  }
  return newRect;
}



NA_IDEF NAVertex naClampVertexToBox(NAVertex vertex, NABox clampBox) {
  double end;
  NAVertex newVertex;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxEmpty(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newVertex = vertex;
  if(newVertex.x < clampBox.vertex.x) { newVertex.x = clampBox.vertex.x; }
  end = naGetBoxEndX(clampBox);
  if(newVertex.x > end) {
    newVertex.x = end;
  }
  if(newVertex.y < clampBox.vertex.y) { newVertex.y = clampBox.vertex.y; }
  end = naGetBoxEndY(clampBox);
  if(newVertex.y > end) {
    newVertex.y = end;
  }
  if(newVertex.z < clampBox.vertex.z) { newVertex.z = clampBox.vertex.z; }
  end = naGetBoxEndZ(clampBox);
  if(newVertex.z > end) {
    newVertex.z = end;
  }
  return newVertex;
}

NA_IDEF NAVertexi32 naClampVertexi32ToBox(NAVertexi32 vertex, NABoxi32 clampBox) {
  int32 max;
  NAVertexi32 newVertex;
  #if NA_DEBUG
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxi32EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi32Valid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newVertex = vertex;
  if(newVertex.x <  clampBox.vertex.x) { newVertex.x = clampBox.vertex.x; }
  max = naGetBoxi32MaxX(clampBox);
  if(newVertex.x > max) {
    newVertex.x = max;
  }
  if(newVertex.y <  clampBox.vertex.y) { newVertex.y = clampBox.vertex.y; }
  max = naGetBoxi32MaxY(clampBox);
  if(newVertex.y > max) {
    newVertex.y = max;
  }
  if(newVertex.z <  clampBox.vertex.z) { newVertex.z = clampBox.vertex.z; }
  max = naGetBoxi32MaxZ(clampBox);
  if(newVertex.z > max) {
    newVertex.z = max;
  }
  return newVertex;
}

NA_IDEF NAVertexi64 naClampVertexi64ToBox(NAVertexi64 vertex, NABoxi64 clampBox) {
  int64 max;
  NAVertexi64 newVertex;
  #if NA_DEBUG
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxi64EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi64Valid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newVertex = vertex;
  if(newVertex.x <  clampBox.vertex.x) { newVertex.x = clampBox.vertex.x; }
  max = naGetBoxi64MaxX(clampBox);
  if(newVertex.x > max) {
    newVertex.x = max;
  }
  if(newVertex.y <  clampBox.vertex.y) { newVertex.y = clampBox.vertex.y; }
  max = naGetBoxi64MaxY(clampBox);
  if(newVertex.y > max) {
    newVertex.y = max;
  }
  if(newVertex.z <  clampBox.vertex.z) { newVertex.z = clampBox.vertex.z; }
  max = naGetBoxi64MaxZ(clampBox);
  if(newVertex.z > max) {
    newVertex.z = max;
  }
  return newVertex;
}



NA_IDEF NABox naClampBoxToBox(NABox box, NABox clampBox) {
  // Todo: use helper functions in this function.
  double value;
  NABox newBox;
  #if NA_DEBUG
    if(naIsBoxEmpty(box))
      naError("box is empty.");
    if(!naIsBoxValid(box))
      naError("box is invalid.");
    if(naIsBoxEmpty(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newBox = box;
  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxEndX(clampBox);
  if(naGetBoxEndXE(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxEndY(clampBox);
  if(naGetBoxEndYE(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxEndZ(clampBox);
  if(naGetBoxEndZE(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
  }
  return newBox;
}

NA_IDEF NABoxi32 naClampBoxi32ToBox(NABoxi32 box, NABoxi32 clampBox) {
  // Todo: use helper functions in this function.
  int32 value;
  NABoxi32 newBox;
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
    if(naIsBoxi32EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi32Valid(clampBox))
      naError("clampBox is invalid.");
  #endif

  newBox = box;

  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxi32EndX(clampBox);
  if(naGetBoxi32EndX(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxi32EndY(clampBox);
  if(naGetBoxi32EndY(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxi32EndZ(clampBox);
  if(naGetBoxi32EndZ(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
  }
  return newBox;
}

NA_IDEF NABoxi64 naClampBoxi64ToBox(NABoxi64 box, NABoxi64 clampBox) {
  // Todo: use helper functions in this function.
  int64 value;
  NABoxi64 newBox;
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
    if(naIsBoxi64EmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxi64Valid(clampBox))
      naError("clampBox is invalid.");
  #endif

  newBox = box;

  // Adjust in first dimension
  value = clampBox.vertex.x - newBox.vertex.x;
  if(value > 0) {
    newBox.volume.width -= value;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
    newBox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxi64EndX(clampBox);
  if(naGetBoxi64EndX(newBox) > value) {
    newBox.volume.width = value - newBox.vertex.x;
    if(newBox.volume.width <= 0) {
      return newBox;
    }
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newBox.vertex.y;
  if(value > 0) {
    newBox.volume.height -= value;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
    newBox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxi64EndY(clampBox);
  if(naGetBoxi64EndY(newBox) > value) {
    newBox.volume.height = value - newBox.vertex.y;
    if(newBox.volume.height <= 0) {
      return newBox;
    }
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newBox.vertex.z;
  if(value > 0) {
    newBox.volume.depth -= value;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
    newBox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxi64EndZ(clampBox);
  if(naGetBoxi64EndZ(newBox) > value) {
    newBox.volume.depth = value - newBox.vertex.z;
    if(newBox.volume.depth <= 0) {
      return newBox;
    }
  }
  return newBox;
}


// ///////////////////////////////////
// EQUAL
// ///////////////////////////////////

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
  return ((size1.width == size2.width) && (size1.height == size2.height));
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



NA_IDEF NABool naEqualVertex(NAVertex vertex1, NAVertex vertex2) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
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



// ///////////////////////////////////////
// COMPARING
// ///////////////////////////////////////

NA_IDEF NABool naLessPos(NAPos pos1, NAPos pos2) {
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x < pos2.x) && (pos1.y < pos2.y));
}
NA_IDEF NABool naLessEqualPos(NAPos pos1, NAPos pos2) {
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



NA_IDEF NABool naLessVertex(NAVertex vertex1, NAVertex vertex2) {
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x < vertex2.x) && (vertex1.y < vertex2.y) && (vertex1.z < vertex2.z));
}
NA_IDEF NABool naLessEqualVertex(NAVertex vertex1, NAVertex vertex2) {
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
