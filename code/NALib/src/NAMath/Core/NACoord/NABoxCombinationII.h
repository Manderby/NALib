
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NABox naMakeBoxCombination(NAVertex point1, NAVertex point2) {
  NABox newBox;
  #if NA_DEBUG
    if(!naIsVertexValid(point1))
      naError("point1 is invalid.");
    if(!naIsVertexValid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newBox.vertex.x = point1.x;
    newBox.volume.width = naMakeLengthWithStartAndEnd(point1.x, point2.x);
  }else{
    newBox.vertex.x = point2.x;
    newBox.volume.width = naMakeLengthWithStartAndEnd(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newBox.vertex.y = point1.y;
    newBox.volume.height = naMakeLengthWithStartAndEnd(point1.y, point2.y);
  }else{
    newBox.vertex.y = point2.y;
    newBox.volume.height = naMakeLengthWithStartAndEnd(point2.y, point1.y);
  }
  if(point2.z > point1.z) {
    newBox.vertex.z = point1.z;
    newBox.volume.depth = naMakeLengthWithStartAndEnd(point1.z, point2.z);
  }else{
    newBox.vertex.z = point2.z;
    newBox.volume.depth = naMakeLengthWithStartAndEnd(point2.z, point1.z);
  }
  return newBox;
}
NA_IDEF NABoxf naMakeBoxfCombination(NAVertexf point1, NAVertexf point2) {
  NABoxf newBox;
  #if NA_DEBUG
    if(!naIsVertexfValid(point1))
      naError("point1 is invalid.");
    if(!naIsVertexfValid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newBox.vertex.x = point1.x;
    newBox.volume.width = naMakeLengthWithStartAndEndf(point1.x, point2.x);
  }else{
    newBox.vertex.x = point2.x;
    newBox.volume.width = naMakeLengthWithStartAndEndf(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newBox.vertex.y = point1.y;
    newBox.volume.height = naMakeLengthWithStartAndEndf(point1.y, point2.y);
  }else{
    newBox.vertex.y = point2.y;
    newBox.volume.height = naMakeLengthWithStartAndEndf(point2.y, point1.y);
  }
  if(point2.z > point1.z) {
    newBox.vertex.z = point1.z;
    newBox.volume.depth = naMakeLengthWithStartAndEndf(point1.z, point2.z);
  }else{
    newBox.vertex.z = point2.z;
    newBox.volume.depth = naMakeLengthWithStartAndEndf(point2.z, point1.z);
  }
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32Combination(NAVertexi32 point1, NAVertexi32 point2) {
  NABoxi32 newBox;
  #if NA_DEBUG
    if(!naIsVertexi32Valid(point1))
      naError("point1 is invalid.");
    if(!naIsVertexi32Valid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newBox.vertex.x = point1.x;
    newBox.volume.width = naMakeLengthWithStartAndEndi32(point1.x, point2.x);
  }else{
    newBox.vertex.x = point2.x;
    newBox.volume.width = naMakeLengthWithStartAndEndi32(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newBox.vertex.y = point1.y;
    newBox.volume.height = naMakeLengthWithStartAndEndi32(point1.y, point2.y);
  }else{
    newBox.vertex.y = point2.y;
    newBox.volume.height = naMakeLengthWithStartAndEndi32(point2.y, point1.y);
  }
  if(point2.z > point1.z) {
    newBox.vertex.z = point1.z;
    newBox.volume.depth = naMakeLengthWithStartAndEndi32(point1.z, point2.z);
  }else{
    newBox.vertex.z = point2.z;
    newBox.volume.depth = naMakeLengthWithStartAndEndi32(point2.z, point1.z);
  }
  return newBox;
}
NA_IDEF NABoxi64 naMakeBoxi64Combination(NAVertexi64 point1, NAVertexi64 point2) {
  NABoxi64 newBox;
  #if NA_DEBUG
    if(!naIsVertexi64Valid(point1))
      naError("point1 is invalid.");
    if(!naIsVertexi64Valid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newBox.vertex.x = point1.x;
    newBox.volume.width = naMakeLengthWithStartAndEndi64(point1.x, point2.x);
  }else{
    newBox.vertex.x = point2.x;
    newBox.volume.width = naMakeLengthWithStartAndEndi64(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newBox.vertex.y = point1.y;
    newBox.volume.height = naMakeLengthWithStartAndEndi64(point1.y, point2.y);
  }else{
    newBox.vertex.y = point2.y;
    newBox.volume.height = naMakeLengthWithStartAndEndi64(point2.y, point1.y);
  }
  if(point2.z > point1.z) {
    newBox.vertex.z = point1.z;
    newBox.volume.depth = naMakeLengthWithStartAndEndi64(point1.z, point2.z);
  }else{
    newBox.vertex.z = point2.z;
    newBox.volume.depth = naMakeLengthWithStartAndEndi64(point2.z, point1.z);
  }
  return newBox;
}
NA_IDEF NABoxs naMakeBoxsCombination(NAVertexs point1, NAVertexs point2) {
  NABoxs newBox;
  #if NA_DEBUG
    if(!naIsVertexsValid(point1))
      naError("point1 is invalid.");
    if(!naIsVertexsValid(point2))
      naError("point2 is invalid.");
  #endif
  if(point2.x > point1.x) {
    newBox.vertex.x = point1.x;
    newBox.volume.width = naMakeLengthWithStartAndEnds(point1.x, point2.x);
  }else{
    newBox.vertex.x = point2.x;
    newBox.volume.width = naMakeLengthWithStartAndEnds(point2.x, point1.x);
  }
  if(point2.y > point1.y) {
    newBox.vertex.y = point1.y;
    newBox.volume.height = naMakeLengthWithStartAndEnds(point1.y, point2.y);
  }else{
    newBox.vertex.y = point2.y;
    newBox.volume.height = naMakeLengthWithStartAndEnds(point2.y, point1.y);
  }
  if(point2.z > point1.z) {
    newBox.vertex.z = point1.z;
    newBox.volume.depth = naMakeLengthWithStartAndEnds(point1.z, point2.z);
  }else{
    newBox.vertex.z = point2.z;
    newBox.volume.depth = naMakeLengthWithStartAndEnds(point2.z, point1.z);
  }
  return newBox;
}



NA_IDEF NABox naMakeBoxExtension(NABox box, NAVertex vertex) {
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
NA_IDEF NABoxf naMakeBoxfExtension(NABoxf box, NAVertexf vertex) {
  NABoxf newBox;
  float end;
  #if NA_DEBUG
    if(naIsBoxfEmpty(box))
      naError("box is empty.");
    if(!naIsBoxfUseful(box))
      naError("box is not useful.");
    if(!naIsVertexfValid(vertex))
      naError("vertex is invalid.");
  #endif
  newBox.vertex.x = naMinf(box.vertex.x, vertex.x);
  newBox.vertex.y = naMinf(box.vertex.y, vertex.y);
  newBox.vertex.z = naMinf(box.vertex.z, vertex.z);
  end = naGetBoxfEndX(box);
  newBox.volume.width  = naMakeLengthWithStartAndEndf(newBox.vertex.x, naMaxf(end, vertex.x));
  end = naGetBoxfEndY(box);
  newBox.volume.height = naMakeLengthWithStartAndEndf(newBox.vertex.y, naMaxf(end, vertex.y));
  end = naGetBoxfEndZ(box);
  newBox.volume.depth = naMakeLengthWithStartAndEndf(newBox.vertex.z, naMaxf(end, vertex.z));
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32Extension(NABoxi32 box, NAVertexi32 vertex) {
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
NA_IDEF NABoxi64 naMakeBoxi64Extension(NABoxi64 box, NAVertexi64 vertex) {
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
NA_IDEF NABoxs naMakeBoxsExtension(NABoxs box, NAVertexs vertex) {
  NABoxs newBox;
  size_t max;
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxsUseful(box))
      naError("box is not useful.");
    if(!naIsVertexsValid(vertex))
      naError("vertex is invalid.");
  #endif
  newBox.vertex.x = naMins(box.vertex.x, vertex.x);
  newBox.vertex.y = naMins(box.vertex.y, vertex.y);
  newBox.vertex.z = naMins(box.vertex.z, vertex.z);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetBoxsMaxX(box);
  newBox.volume.width  = naMakeLengthWithMinAndMaxs(newBox.vertex.x, naMaxs(max, vertex.x));
  max = naGetBoxsMaxY(box);
  newBox.volume.height = naMakeLengthWithMinAndMaxs(newBox.vertex.y, naMaxs(max, vertex.y));
  max = naGetBoxsMaxZ(box);
  newBox.volume.depth = naMakeLengthWithMinAndMaxs(newBox.vertex.z, naMaxs(max, vertex.z));
  return newBox;
}



NA_IDEF NABox naMakeBoxExtensionE(NABox box, NAVertex vertex) {
  #if NA_DEBUG
    if(!naIsBoxValid(box))
      naError("box is invalid.");
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
  #endif
  
  if(naIsBoxEmpty(box)) {
    NABox newBox;
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumeZero();
    return newBox;
  }else{
    return naMakeBoxExtension(box, vertex);
  }
}
NA_IDEF NABoxf naMakeBoxfExtensionE(NABoxf box, NAVertexf vertex) {
  #if NA_DEBUG
    if(!naIsBoxfValid(box))
      naError("box is invalid.");
    if(!naIsVertexfValid(vertex))
      naError("vertex is invalid.");
  #endif
  
  if(naIsBoxfEmpty(box)) {
    NABoxf newBox;
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumefZero();
    return newBox;
  }else{
    return naMakeBoxfExtension(box, vertex);
  }
}
NA_IDEF NABoxi32 naMakeBoxi32ExtensionE(NABoxi32 box, NAVertexi32 vertex) {
  #if NA_DEBUG
    if(!naIsBoxi32Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi32Valid(vertex))
      naError("vertex is invalid.");
  #endif
  
  if(naIsBoxi32Empty(box)) {
    NABoxi32 newBox;
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumei32(1, 1, 1);
    return newBox;
  }else{
    return naMakeBoxi32Extension(box, vertex);
  }
}
NA_IDEF NABoxi64 naMakeBoxi64ExtensionE(NABoxi64 box, NAVertexi64 vertex) {
  #if NA_DEBUG
    if(!naIsBoxi64Valid(box))
      naError("box is invalid.");
    if(!naIsVertexi64Valid(vertex))
      naError("vertex is invalid.");
  #endif

  if(naIsBoxi64Empty(box)) {
    NABoxi64 newBox;
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumei64(1, 1, 1);
    return newBox;
  }else{
    return naMakeBoxi64Extension(box, vertex);
  }
}
NA_IDEF NABoxs naMakeBoxsExtensionE(NABoxs box, NAVertexs vertex) {
  #if NA_DEBUG
    if(!naIsBoxsValid(box))
      naError("box is invalid.");
    if(!naIsVertexsValid(vertex))
      naError("vertex is invalid.");
  #endif

  if(naIsBoxsEmpty(box)) {
    NABoxs newBox;
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumes(1, 1, 1);
    return newBox;
  }else{
    return naMakeBoxsExtension(box, vertex);
  }
}



NA_IDEF NABox naMakeBoxUnion(NABox box1, NABox box2) {
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
  newBox.volume.height = naMakeLengthWithStartAndEnd(newBox.vertex.y, naMax(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEnd(newBox.vertex.z, naMax(end1, end2));
  return newBox;
}
NA_IDEF NABoxf naMakeBoxfUnion(NABoxf box1, NABoxf box2) {
  NABoxf newBox;
  float end1;
  float end2;
  #if NA_DEBUG
    if(naIsBoxfEmpty(box1))
      naError("box1 is empty.");
    if(!naIsBoxfValid(box1))
      naError("box1 is invalid.");
    if(naIsBoxfEmpty(box2))
      naError("box2 is empty.");
    if(!naIsBoxfValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMinf(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMinf(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMinf(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxfEndX(box1);
  end2 = naGetBoxfEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndf(newBox.vertex.x, naMaxf(end1, end2));
  end1 = naGetBoxfEndY(box1);
  end2 = naGetBoxfEndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndf(newBox.vertex.y, naMaxf(end1, end2));
  end1 = naGetBoxfEndZ(box1);
  end2 = naGetBoxfEndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEndf(newBox.vertex.z, naMaxf(end1, end2));
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32Union(NABoxi32 box1, NABoxi32 box2) {
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
NA_IDEF NABoxi64 naMakeBoxi64Union(NABoxi64 box1, NABoxi64 box2) {
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
NA_IDEF NABoxs naMakeBoxsUnion(NABoxs box1, NABoxs box2) {
  NABoxs newBox;
  size_t end1;
  size_t end2;
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(box1))
      naError("box1 is empty.");
    if(!naIsBoxsValid(box1))
      naError("box1 is invalid.");
    if(naIsBoxsEmptySlow(box2))
      naError("box2 is empty.");
    if(!naIsBoxsValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMins(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMins(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMins(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxsEndX(box1);
  end2 = naGetBoxsEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEnds(newBox.vertex.x, naMaxs(end1, end2));
  end1 = naGetBoxsEndY(box1);
  end2 = naGetBoxsEndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEnds(newBox.vertex.y, naMaxs(end1, end2));
  end1 = naGetBoxsEndZ(box1);
  end2 = naGetBoxsEndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEnds(newBox.vertex.z, naMaxs(end1, end2));
  return newBox;
}



NA_IDEF NABox naMakeBoxUnionE(NABox box1, NABox box2) {
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
  
  return naMakeBoxUnion(box1, box2);
}
NA_IDEF NABoxf naMakeBoxfUnionE(NABoxf box1, NABoxf box2) {
  #if NA_DEBUG
    if(!naIsBoxfValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxfValid(box2))
      naError("box2 is invalid.");
  #endif
  
  if(naIsBoxfEmpty(box1))
    return box2;
  if(naIsBoxfEmpty(box2))
    return box1;
  
  return naMakeBoxfUnion(box1, box2);
}
NA_IDEF NABoxi32 naMakeBoxi32UnionE(NABoxi32 box1, NABoxi32 box2) {
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
  
  return naMakeBoxi32Union(box1, box2);
}
NA_IDEF NABoxi64 naMakeBoxi64UnionE(NABoxi64 box1, NABoxi64 box2) {
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
  
  return naMakeBoxi64Union(box1, box2);
}
NA_IDEF NABoxs naMakeBoxsUnionE(NABoxs box1, NABoxs box2) {
  #if NA_DEBUG
    if(!naIsBoxsValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxsValid(box2))
      naError("box2 is invalid.");
  #endif
  
  if(naIsBoxsEmpty(box1))
    return box2;
  if(naIsBoxsEmpty(box2))
    return box1;
  
  return naMakeBoxsUnion(box1, box2);
}



NA_IDEF NABox naMakeBoxIntersection(NABox box1, NABox box2) {
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
NA_IDEF NABoxf naMakeBoxfIntersection(NABoxf box1, NABoxf box2) {
  NABoxf newBox;
  float end1;
  float end2;
  #if NA_DEBUG
    if(!naIsBoxfValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxfValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMaxf(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMaxf(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMaxf(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxfEndX(box1);
  end2 = naGetBoxfEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndf(newBox.vertex.x, naMinf(end1, end2));
  end1 = naGetBoxfEndY(box1);
  end2 = naGetBoxfEndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndf(newBox.vertex.y, naMinf(end1, end2));
  end1 = naGetBoxfEndZ(box1);
  end2 = naGetBoxfEndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEndf(newBox.vertex.z, naMinf(end1, end2));
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32Intersection(NABoxi32 box1, NABoxi32 box2) {
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
NA_IDEF NABoxi64 naMakeBoxi64Intersection(NABoxi64 box1, NABoxi64 box2) {
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
NA_IDEF NABoxs naMakeBoxsIntersection(NABoxs box1, NABoxs box2) {
  NABoxs newBox;
  size_t end1;
  size_t end2;
  #if NA_DEBUG
    if(!naIsBoxsValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxsValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMaxs(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMaxs(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMaxs(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxsEndX(box1);
  end2 = naGetBoxsEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEnds(newBox.vertex.x, naMins(end1, end2));
  end1 = naGetBoxsEndY(box1);
  end2 = naGetBoxsEndY(box2);
  newBox.volume.height  = naMakeLengthWithStartAndEnds(newBox.vertex.y, naMins(end1, end2));
  end1 = naGetBoxsEndZ(box1);
  end2 = naGetBoxsEndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEnds(newBox.vertex.z, naMins(end1, end2));
  return newBox;
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
