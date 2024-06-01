
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"




// /////////////////////////////////////|
// Warning: This is one huge file!
//
// Coordinate functions are not especially difficult to understand or implement
// but take a lot of code writing. You are free to explore all the inline code
// underneath the API but there really isn't much to see.
// /////////////////////////////////////



#include "../../NAMathOperators.h"
#include "../../../NAUtility/NAValueHelper.h"


#include "NABoundsII.h"
#include "NABezelII.h"
#include "NAPosII.h"
#include "NARangeII.h"
#include "NARectII.h"
#include "NASizeII.h"








NA_IDEF NAVertex naMakeVertex(double x, double y, double z) {
  NAVertex newVertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(x) && naIsOffsetValueValid(y) && naIsOffsetValueValid(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(x) && naIsOffsetValueUseful(y) && naIsOffsetValueUseful(z)))
      naError("Values given are not useful.");
  #endif
  newVertex.x = x;
  newVertex.y = y;
  newVertex.z = z;
  return newVertex;
}
NA_IDEF NAVertexi naMakeVertexi(NAInt x, NAInt y, NAInt z) {
  NAVertexi newVertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi(x) && naIsOffsetValueValidi(y) && naIsOffsetValueValidi(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuli(x) && naIsOffsetValueUsefuli(y) && naIsOffsetValueUsefuli(z)))
      naError("Values given are not useful.");
  #endif
  newVertex.x = x;
  newVertex.y = y;
  newVertex.z = z;
  return newVertex;
}
NA_IDEF NAVertexi32 naMakeVertexi32(int32 x, int32 y, int32 z) {
  NAVertexi32 newVertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi32(x) && naIsOffsetValueValidi32(y) && naIsOffsetValueValidi32(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuli32(x) && naIsOffsetValueUsefuli32(y) && naIsOffsetValueUsefuli32(z)))
      naError("Values given are not useful.");
  #endif
  newVertex.x = x;
  newVertex.y = y;
  newVertex.z = z;
  return newVertex;
}
NA_IDEF NAVertexi64 naMakeVertexi64(int64 x, int64 y, int64 z) {
  NAVertexi64 newVertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi64(x) && naIsOffsetValueValidi64(y) && naIsOffsetValueValidi64(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuli64(x) && naIsOffsetValueUsefuli64(y) && naIsOffsetValueUsefuli64(z)))
      naError("Values given are not useful.");
  #endif
  newVertex.x = x;
  newVertex.y = y;
  newVertex.z = z;
  return newVertex;
}
NA_IDEF NAVertex naMakeVertexWithVertexi(NAVertexi vertex) {
  NAVertex newVertex = naMakeVertex((double)vertex.x, (double)vertex.y, (double)vertex.z);
  return newVertex;
}
NA_IDEF NAVertexi naMakeVertexiWithVertex(NAVertex vertex) {
  NAVertexi newVertex = naMakeVertexi((NAInt)vertex.x, (NAInt)vertex.y, (NAInt)vertex.z);
  return newVertex;
}
NA_IDEF NAVertex naMakeVertexWithV3(const double* v) {
  NAVertex newVertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(v[0]) && naIsOffsetValueValid(v[1]) && naIsOffsetValueValid(v[2])))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(v[0]) && naIsOffsetValueUseful(v[1]) && naIsOffsetValueUseful(v[2])))
      naError("Values given are not useful.");
  #endif
  newVertex.x = v[0];
  newVertex.y = v[1];
  newVertex.z = v[2];
  return newVertex;
}



NA_IDEF NAVolume naMakeVolume(double width, double height, double depth) {
  NAVolume newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(width) && naIsLengthValueUseful(height) && naIsLengthValueUseful(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}
NA_IDEF NAVolumei naMakeVolumei(NAInt width, NAInt height, NAInt depth) {
  NAVolumei newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height) && naIsLengthValueValidi(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli(width) && naIsLengthValueUsefuli(height) && naIsLengthValueUsefuli(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}
NA_IDEF NAVolumei32 naMakeVolumei32(int32 width, int32 height, int32 depth) {
  NAVolumei32 newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi32(width) && naIsLengthValueValidi32(height) && naIsLengthValueValidi32(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli32(width) && naIsLengthValueUsefuli32(height) && naIsLengthValueUsefuli32(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}
NA_IDEF NAVolumei64 naMakeVolumei64(int64 width, int64 height, int64 depth) {
  NAVolumei64 newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi64(width) && naIsLengthValueValidi64(height) && naIsLengthValueValidi64(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli64(width) && naIsLengthValueUsefuli64(height) && naIsLengthValueUsefuli64(depth)))
      naError("Values given are not useful.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}
NA_IDEF NAVolume naMakeVolumeE(double width, double height, double depth) {
  NAVolume newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("Invalid values given.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}
NA_IDEF NAVolumei naMakeVolumeiE(NAInt width, NAInt height, NAInt depth) {
  NAVolumei newVolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height) && naIsLengthValueValidi(depth)))
      naError("Invalid values given.");
  #endif
  newVolume.width = width;
  newVolume.height = height;
  newVolume.depth = depth;
  return newVolume;
}
NA_IDEF NAVolume naMakeVolumeWithVolumei(NAVolumei volume) {
  NAVolume newVolume = naMakeVolumeE((double)volume.width, (double)volume.height, (double)volume.depth);
  return newVolume;
}
NA_IDEF NAVolumei naMakeVolumeiWithVolume(NAVolume volume) {
  NAVolumei newVolume = naMakeVolumeiE((NAInt)volume.width, (NAInt)volume.height, (NAInt)volume.depth);
  return newVolume;
}
NA_IDEF NAVolume naMakeVolumeEmpty() {
  NAVolume newVolume;
  newVolume.width = 0.;
  return newVolume;
}
NA_IDEF NAVolumei naMakeVolumeiEmpty() {
  NAVolumei newVolume;
  newVolume.width = 0;
  return newVolume;
}



NA_IDEF NABox naMakeBox(NAVertex vertex, NAVolume volume) {
  NABox newBox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("Invalid values given.");
    if(!naIsVertexUseful(vertex) || !naIsVolumeUseful(volume))
      naError("Values given are not useful.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}
NA_IDEF NABoxi naMakeBoxi(NAVertexi vertex, NAVolumei volume) {
  NABoxi newBox;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("Invalid values given.");
    if(!naIsVertexiUseful(vertex) || !naIsVolumeiUseful(volume))
      naError("Values given are not useful.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}
NA_IDEF NABox naMakeBoxS(double x, double y, double z, double width, double height, double depth) {
  NABox newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertex(x, y, z);
  newBox.volume = naMakeVolume(width, height, depth);
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32S(int32 x, int32 y, int32 z, int32 width, int32 height, int32 depth) {
  NABoxi32 newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexi32(x, y, z);
  newBox.volume = naMakeVolumei32(width, height, depth);
  return newBox;
}
NA_IDEF NABoxi64 naMakeBoxi64S(int64 x, int64 y, int64 z, int64 width, int64 height, int64 depth) {
  NABoxi64 newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexi64(x, y, z);
  newBox.volume = naMakeVolumei64(width, height, depth);
  return newBox;
}
NA_IDEF NABox naMakeBoxE(NAVertex vertex, NAVolume volume) {
  NABox newBox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("Invalid values given.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiE(NAVertexi vertex, NAVolumei volume) {
  NABoxi newBox;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("Invalid values given.");
  #endif
  newBox.vertex = vertex;
  newBox.volume = volume;
  return newBox;
}
NA_IDEF NABox naMakeBoxSE(double x, double y, double z, double width, double height, double depth) {
  NABox newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertex(x, y, z);
  newBox.volume = naMakeVolumeE(width, height, depth);
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiSE(NAInt x, NAInt y, NAInt z, NAInt width, NAInt height, NAInt depth) {
  NABoxi newBox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newBox.vertex = naMakeVertexi(x, y, z);
  newBox.volume = naMakeVolumeiE(width, height, depth);
  return newBox;
}
NA_IDEF NABox naMakeBoxWithBoxi(NABoxi box) {
  NABox newBox = naMakeBoxSE((double)box.vertex.x, (double)box.vertex.y, (double)box.vertex.z, (double)box.volume.width, (double)box.volume.height, (double)box.volume.depth);
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiWithBox(NABox box) {
  NABoxi newBox = naMakeBoxiSE((NAInt)box.vertex.x, (NAInt)box.vertex.y, (NAInt)box.vertex.z, (NAInt)box.volume.width, (NAInt)box.volume.height, (NAInt)box.volume.depth);
  return newBox;
}
NA_IDEF NABox naMakeBoxEmpty() {
  NABox newBox;
  newBox.volume.width = 0.;
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiEmpty() {
  NABoxi newBox;
  newBox.volume.width = 0;
  return newBox;
}



#ifdef CGGEOMETRY_H_
NA_IDEF NAPos naMakePosWithCGPoint(CGPoint cgPoint) {
  NAPos newPos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(cgPoint.x) && naIsOffsetValueValid(cgPoint.y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(cgPoint.x) && naIsOffsetValueUseful(cgPoint.y)))
      naError("Values given are not useful.");
  #endif
  newPos.x = cgPoint.x;
  newPos.y = cgPoint.y;
  return newPos;
}
NA_IDEF NASize naMakeSizeWithCGSize(CGSize cgSize) {
  NASize newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(cgSize.width) && naIsLengthValueValid(cgSize.height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(cgSize.width) && naIsLengthValueUseful(cgSize.height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = cgSize.width;
  newSize.height = cgSize.height;
  return newSize;
}
NA_IDEF NARect naMakeRectWithCGRect(CGRect cgRect) {
  NARect newRect;
  newRect.pos = naMakePosWithCGPoint(cgRect.origin);
  newRect.size = naMakeSizeWithCGSize(cgRect.size);
  return newRect;
}
#endif



#if defined __OBJC__ && defined __AVAILABILITYMACROS__
NA_IDEF NAPos naMakePosWithNSPoint(NSPoint nsPoint) {
  NAPos newPos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(nsPoint.x) && naIsOffsetValueValid(nsPoint.y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(nsPoint.x) && naIsOffsetValueUseful(nsPoint.y)))
      naError("Values given are not useful.");
  #endif
  newPos.x = nsPoint.x;
  newPos.y = nsPoint.y;
  return newPos;
}
NA_IDEF NSPoint naMakeNSPointWithPos(NAPos naPos) {
  NSPoint newpoint;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(naPos.x) && naIsOffsetValueValid(naPos.y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(naPos.x) && naIsOffsetValueUseful(naPos.y)))
      naError("Values given are not useful.");
  #endif
  newpoint.x = (CGFloat)naPos.x;
  newpoint.y = (CGFloat)naPos.y;
  return newpoint;
}
NA_IDEF NASize naMakeSizeWithNSSize(NSSize nsSize) {
  NASize newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(nsSize.width) && naIsLengthValueValid(nsSize.height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(nsSize.width) && naIsLengthValueUseful(nsSize.height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = nsSize.width;
  newSize.height = nsSize.height;
  return newSize;
}
NA_IDEF NSSize naMakeNSSizeWithSize(NASize naSize) {
  NSSize newSize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(naSize.width) && naIsLengthValueValid(naSize.height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(naSize.width) && naIsLengthValueUseful(naSize.height)))
      naError("Values given are not useful.");
  #endif
  newSize.width = (CGFloat)naSize.width;
  newSize.height = (CGFloat)naSize.height;
  return newSize;
}
NA_IDEF NARect naMakeRectWithNSRect(NSRect nsRect) {
  NARect newRect;
  newRect.pos = naMakePosWithNSPoint(nsRect.origin);
  newRect.size = naMakeSizeWithNSSize(nsRect.size);
  return newRect;
}
NA_IDEF NSRect naMakeNSRectWithSize(NASize naSize) {
  NSRect newRect;
  newRect.origin = NSMakePoint(0., 0.);
  newRect.size = naMakeNSSizeWithSize(naSize);
  return newRect;
}
NA_IDEF NSRect naMakeNSRectWithRect(NARect naRect) {
  return NSMakeRect(
    (CGFloat)naRect.pos.x,
    (CGFloat)naRect.pos.y,
    (CGFloat)naRect.size.width,
    (CGFloat)naRect.size.height);
}
#endif



NA_IDEF NAPosi naMakePosiWithAlignment(NAPosi origin, NARecti alignRect) {
  NAPosi newOrigin;
  #if NA_DEBUG
    if(naIsRectiEmptySlow(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  newOrigin.x = naAlignValuei(origin.x, alignRect.pos.x, alignRect.size.width);
  newOrigin.y = naAlignValuei(origin.y, alignRect.pos.y, alignRect.size.height);
  return newOrigin;
}
NA_IDEF NAPos naMakePosWithAlignment(NAPos origin, NARect alignRect) {
  NAPos newOrigin;
  #if NA_DEBUG
    if(naIsRectEmpty(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  newOrigin.x = naAlignValued(origin.x, alignRect.pos.x, alignRect.size.width);
  newOrigin.y = naAlignValued(origin.y, alignRect.pos.y, alignRect.size.height);
  return newOrigin;
}
NA_IDEF NAVertexi naMakeVertexiWithAlignment(NAVertexi origin, NABoxi alignBox) {
  NAVertexi newOrigin;
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(alignBox))
      naCrash("alignment box is empty.");
  #endif
  newOrigin.x = naAlignValuei(origin.x, alignBox.vertex.x, alignBox.volume.width);
  newOrigin.y = naAlignValuei(origin.y, alignBox.vertex.y, alignBox.volume.height);
  newOrigin.z = naAlignValuei(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return newOrigin;
}
NA_IDEF NAVertex naMakeVertexWithAlignment(NAVertex origin, NABox alignBox) {
  NAVertex newOrigin;
  #if NA_DEBUG
    if(naIsBoxEmpty(alignBox))
      naCrash("alignment box is empty.");
  #endif
  newOrigin.x = naAlignValued(origin.x, alignBox.vertex.x, alignBox.volume.width);
  newOrigin.y = naAlignValued(origin.y, alignBox.vertex.y, alignBox.volume.height);
  newOrigin.z = naAlignValued(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return newOrigin;
}



NA_IDEF NARange naMakeRangeWithStartAndEnd(double start, double end) {
  NARange newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(start) && naIsOffsetValueValid(end)))
      naError("Invalid values given.");
  #endif
  newRange.origin = start;
  newRange.length = naMakeLengthWithStartAndEnd(start, end);
  return newRange;
}
NA_IDEF NARangef naMakeRangefWithStartAndEnd(float start, float end) {
  NARangef newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(start) && naIsOffsetValueValidf(end)))
      naError("Invalid values given.");
  #endif
  newRange.origin = start;
  newRange.length = naMakeLengthWithStartAndEndf(start, end);
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32WithStartAndEnd(int32 start, int32 end) {
  NARangei32 newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi32(start) && naIsOffsetValueValidi32(end)))
      naError("Invalid values given.");
  #endif
  newRange.origin = start;
  newRange.length = naMakeLengthWithStartAndEndi32(start, end);
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64WithStartAndEnd(int64 start, int64 end) {
  NARangei64 newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi64(start) && naIsOffsetValueValidi64(end)))
      naError("Invalid values given.");
  #endif
  newRange.origin = start;
  newRange.length = naMakeLengthWithStartAndEndi64(start, end);
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32WithMinAndMax(int32 min, int32 max) {
  NARangei32 newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi32(min) && naIsOffsetValueValidi32(max)))
      naError("Invalid values given.");
    if(max < min)
      naError("min should be smallerequal to max.");
  #endif
  newRange.origin = min;
  newRange.length = naMakeLengthWithMinAndMaxi32(min, max);
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64WithMinAndMax(int64 min, int64 max) {
  NARangei64 newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi64(min) && naIsOffsetValueValidi64(max)))
      naError("Invalid values given.");
    if(max < min)
      naError("min should be smallerequal to max.");
  #endif
  newRange.origin = min;
  newRange.length = naMakeLengthWithMinAndMaxi64(min, max);
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32WithRangeAndOffset(NARangei32 range, int32 offset) {
  NARangei32 newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi32(offset))
      naError("Invalid offset given.");
  #endif
  if(naIsRangei32Empty(range)) {
    newRange.length = 1;
    newRange.origin = offset;
  }else if(offset < range.origin) {
    newRange.length = naMakeLengthWithStartAndEndi32(offset, naGetRangei32End(range));
    newRange.origin = offset;
  }else if(offset > naGetRangei32Max(range)) {
    newRange.length = naMakeLengthWithMinAndMaxi32(range.origin, offset);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64WithRangeAndOffset(NARangei64 range, int64 offset) {
  NARangei64 newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi64(offset))
      naError("Invalid offset given.");
  #endif
  if(naIsRangei64Empty(range)) {
    newRange.length = 1;
    newRange.origin = offset;
  }else if(offset < range.origin) {
    newRange.length = naMakeLengthWithStartAndEndi64(offset, naGetRangei64End(range));
    newRange.origin = offset;
  }else if(offset > naGetRangei64Max(range)) {
    newRange.length = naMakeLengthWithMinAndMaxi64(range.origin, offset);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARangei32 naMakeRangeiWithRangeUnion (NARangei32 range1, NARangei32 range2) {
  NARangei32 newRange;
  #if NA_DEBUG
    if(!naIsRangei32Valid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangei32Valid(range2))
      naError("Invalid range 2 given.");
  #endif
  if(naIsRangei32Empty(range1) && naIsRangei32Empty(range2)) {
    newRange = naMakeRangei32E(0, 0);
  }else if(naIsRangei32Empty(range1)) {
    newRange = range2;
  }else if(naIsRangei32Empty(range2)) {
    newRange = range1;
  }else{
    newRange = naMakeRangei32WithStartAndEnd(naMini32(range1.origin, range2.origin), naMaxi32(naGetRangei32End(range1), naGetRangei32End(range2)));
  }
  return newRange;
}



NA_IDEF NARect naMakeRectWithPosAndPos(NAPos pos1, NAPos pos2) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x) {
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y) {
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(pos2.y, pos1.y);
  }
  return newRect;
}
NA_IDEF NARect naMakeRectWithRectAndPos(NARect rect, NAPos pos) {
  NARect newRect;
  double end;
  #if NA_DEBUG
    if(naIsRectEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectUseful(rect))
      naError("rect is not useful.");
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
  #endif
  newRect.pos.x = naMin(rect.pos.x, pos.x);
  newRect.pos.y = naMin(rect.pos.y, pos.y);
  end = naGetRectEndX(rect);
  newRect.size.width  = naMakeLengthWithStartAndEnd(newRect.pos.x, naMax(end, pos.x));
  end = naGetRectEndY(rect);
  newRect.size.height = naMakeLengthWithStartAndEnd(newRect.pos.y, naMax(end, pos.y));
  return newRect;
}
NA_IDEF NARect naMakeRectWithRectAndPosE(NARect rect, NAPos pos) {
  NARect newRect;
  #if NA_DEBUG
    if(!naIsRectEmpty(rect) && !naIsRectValid(rect))
      naError("rect is invalid.");
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
  #endif
  if(naIsRectEmpty(rect)) {
    newRect.pos = pos;
    newRect.size = naMakeSize(1., 1.);
  }else{
    double end;
    newRect.pos.x = naMin(rect.pos.x, pos.x);
    newRect.pos.y = naMin(rect.pos.y, pos.y); 
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    end = naGetRectEndX(rect);
    newRect.size.width  = naMakeLengthWithStartAndEnd(newRect.pos.x, naMax(end, pos.x));
    end = naGetRectEndY(rect);
    newRect.size.height = naMakeLengthWithStartAndEnd(newRect.pos.y, naMax(end, pos.y));
  }
  return newRect;
}
NA_IDEF NARect naMakeRectWithRectUnion(NARect rect1, NARect rect2) {
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
NA_IDEF NARect naMakeRectWithRectUnionE(NARect rect1, NARect rect2) {
  NARect newRect;
  double end1;
  double end2;
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
NA_IDEF NARecti naMakeRectiWithPosAndPos(NAPosi pos1, NAPosi pos2) {
  NARecti newRect;
  #if NA_DEBUG
    if(!naIsPosiValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x) {
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y) {
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi(pos2.y, pos1.y);
  }
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32WithRectAndPos(NARecti32 rect, NAPosi32 pos) {
  NARecti32 newRect;
  int32 max;
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti32Useful(rect))
      naError("rect is not useful.");
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
  #endif
  newRect.pos.x = naMini32(rect.pos.x, pos.x);
  newRect.pos.y = naMini32(rect.pos.y, pos.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRecti32MaxX(rect);
  newRect.size.width  = naMakeLengthWithMinAndMaxi32(newRect.pos.x, naMaxi32(max, pos.x));
  max = naGetRecti32MaxY(rect);
  newRect.size.height = naMakeLengthWithMinAndMaxi32(newRect.pos.y, naMaxi32(max, pos.y));
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32WithRectAndPosE(NARecti32 rect, NAPosi32 pos) {
  NARecti32 newRect;
  #if NA_DEBUG
    if(!naIsRecti32Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi32Valid(pos))
      naError("pos is invalid.");
  #endif
  if(naIsRecti32Empty(rect)) {
    newRect.pos = pos;
    newRect.size = naMakeSizei32(1, 1);
  }else{
    int32 max;
    newRect.pos.x = naMini32(rect.pos.x, pos.x);
    newRect.pos.y = naMini32(rect.pos.y, pos.y);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetRecti32MaxX(rect);
    newRect.size.width  = naMakeLengthWithMinAndMaxi32(newRect.pos.x, naMaxi32(max, pos.x));
    max = naGetRecti32MaxY(rect);
    newRect.size.height = naMakeLengthWithMinAndMaxi32(newRect.pos.y, naMaxi32(max, pos.y));
  }
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64WithRectAndPos(NARecti64 rect, NAPosi64 pos) {
  NARecti64 newRect;
  int64 max;
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRecti64Useful(rect))
      naError("rect is not useful.");
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
  #endif
  newRect.pos.x = naMini64(rect.pos.x, pos.x);
  newRect.pos.y = naMini64(rect.pos.y, pos.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRecti64MaxX(rect);
  newRect.size.width  = naMakeLengthWithMinAndMaxi64(newRect.pos.x, naMaxi64(max, pos.x));
  max = naGetRecti64MaxY(rect);
  newRect.size.height = naMakeLengthWithMinAndMaxi64(newRect.pos.y, naMaxi64(max, pos.y));
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64WithRectAndPosE(NARecti64 rect, NAPosi64 pos) {
  NARecti64 newRect;
  #if NA_DEBUG
    if(!naIsRecti64Valid(rect))
      naError("rect is invalid.");
    if(!naIsPosi64Valid(pos))
      naError("pos is invalid.");
  #endif
  if(naIsRecti64Empty(rect)) {
    newRect.pos = pos;
    newRect.size = naMakeSizei64(1, 1);
  }else{
    int64 max;
    newRect.pos.x = naMini64(rect.pos.x, pos.x);
    newRect.pos.y = naMini64(rect.pos.y, pos.y);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetRecti64MaxX(rect);
    newRect.size.width  = naMakeLengthWithMinAndMaxi64(newRect.pos.x, naMaxi64(max, pos.x));
    max = naGetRecti64MaxY(rect);
    newRect.size.height = naMakeLengthWithMinAndMaxi64(newRect.pos.y, naMaxi64(max, pos.y));
  }
  return newRect;
}

NA_IDEF NARecti32 naMakeRecti32WithRectUnion(NARecti32 rect1, NARecti32 rect2) {
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
NA_IDEF NARecti32 naMakeRecti32WithRectUnionE(NARecti32 rect1, NARecti32 rect2) {
  NARecti32 newRect;
  int32 end1;
  int32 end2;
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
NA_IDEF NARecti64 naMakeRecti64WithRectUnion(NARecti64 rect1, NARecti64 rect2) {
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
NA_IDEF NARecti64 naMakeRecti64WithRectUnionE(NARecti64 rect1, NARecti64 rect2) {
  NARecti64 newRect;
  int64 end1;
  int64 end2;
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
NA_IDEF NABoxi naMakeBoxiWithVertexAndVertex(NAVertexi vertex1, NAVertexi vertex2) {
  NABoxi newBox;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  if(vertex2.x > vertex1.x) {
    newBox.vertex.x = vertex1.x;
    newBox.volume.width = naMakeLengthWithMinAndMaxi(vertex1.x, vertex2.x);
  }else{
    newBox.vertex.x = vertex2.x;
    newBox.volume.width = naMakeLengthWithMinAndMaxi(vertex2.x, vertex1.x);
  }
  if(vertex2.y > vertex1.y) {
    newBox.vertex.y = vertex1.y;
    newBox.volume.height = naMakeLengthWithMinAndMaxi(vertex1.y, vertex2.y);
  }else{
    newBox.vertex.y = vertex2.y;
    newBox.volume.height = naMakeLengthWithMinAndMaxi(vertex2.y, vertex1.y);
  }
  if(vertex2.z > vertex1.z) {
    newBox.vertex.z = vertex1.z;
    newBox.volume.depth = naMakeLengthWithMinAndMaxi(vertex1.z, vertex2.z);
  }else{
    newBox.vertex.z = vertex2.z;
    newBox.volume.depth = naMakeLengthWithMinAndMaxi(vertex2.z, vertex1.z);
  }
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxAndVertex(NABoxi box, NAVertexi vertex) {
  NABoxi newBox;
  NAInt max;
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiUseful(box))
      naError("box is not useful.");
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
  #endif
  newBox.vertex.x = naMini(box.vertex.x, vertex.x);
  newBox.vertex.y = naMini(box.vertex.y, vertex.y);
  newBox.vertex.z = naMini(box.vertex.z, vertex.z);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetBoxiMaxX(box);
  newBox.volume.width  = naMakeLengthWithMinAndMaxi(newBox.vertex.x, naMaxi(max, vertex.x));
  max = naGetBoxiMaxY(box);
  newBox.volume.height = naMakeLengthWithMinAndMaxi(newBox.vertex.y, naMaxi(max, vertex.y));
  max = naGetBoxiMaxZ(box);
  newBox.volume.depth = naMakeLengthWithMinAndMaxi(newBox.vertex.z, naMaxi(max, vertex.z));
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxAndVertexE(NABoxi box, NAVertexi vertex) {
  NABoxi newBox;
  #if NA_DEBUG
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
  #endif
  if(naIsBoxiEmpty(box)) {
    newBox.vertex = vertex;
    newBox.volume = naMakeVolumei(1, 1, 1);
  }else{
    NAInt max;
    newBox.vertex.x = naMini(box.vertex.x, vertex.x);
    newBox.vertex.y = naMini(box.vertex.y, vertex.y);
    newBox.vertex.z = naMini(box.vertex.z, vertex.z);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetBoxiMaxX(box);
    newBox.volume.width  = naMakeLengthWithMinAndMaxi(newBox.vertex.x, naMaxi(max, vertex.x));
    max = naGetBoxiMaxY(box);
    newBox.volume.height = naMakeLengthWithMinAndMaxi(newBox.vertex.y, naMaxi(max, vertex.y));
    max = naGetBoxiMaxZ(box);
    newBox.volume.depth = naMakeLengthWithMinAndMaxi(newBox.vertex.z, naMaxi(max, vertex.z));
  }
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxUnion(NABoxi box1, NABoxi box2) {
  NABoxi newBox;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box1))
      naError("box1 is empty.");
    if(!naIsBoxiValid(box1))
      naError("box1 is invalid.");
    if(naIsBoxiEmptySlow(box2))
      naError("box2 is empty.");
    if(!naIsBoxiValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMini(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMini(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMini(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi(newBox.vertex.x, naMaxi(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndi(newBox.vertex.y, naMaxi(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEndi(newBox.vertex.z, naMaxi(end1, end2));
  return newBox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxUnionE(NABoxi box1, NABoxi box2) {
  NABoxi newBox;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(!naIsBoxiValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("box2 is invalid.");
  #endif
  
  if(naIsBoxiEmpty(box1))
    return box2;
  if(naIsBoxiEmpty(box2))
    return box1;
  
  newBox.vertex.x = naMini(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMini(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMini(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi(newBox.vertex.x, naMaxi(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newBox.volume.height = naMakeLengthWithStartAndEndi(newBox.vertex.y, naMaxi(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newBox.volume.depth = naMakeLengthWithStartAndEndi(newBox.vertex.z, naMaxi(end1, end2));
  return newBox;
}



NA_IDEF NARange naMakeRangeWithRangeIntersection(NARange range1, NARange range2) {
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
NA_IDEF NARangei32 naMakeRangei32WithRangeIntersection(NARangei32 range1, NARangei32 range2) {
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
NA_IDEF NARangei64 naMakeRangei64WithRangeIntersection(NARangei64 range1, NARangei64 range2) {
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
NA_IDEF NARect naMakeRectWithRectIntersection(NARect rect1, NARect rect2) {
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
NA_IDEF NARecti32 naMakeRecti32WithRectIntersection(NARecti32 rect1, NARecti32 rect2) {
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
NA_IDEF NARecti64 naMakeRecti64WithRectIntersection(NARecti64 rect1, NARecti64 rect2) {
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
NA_IDEF NABoxi naMakeBoxiWithBoxIntersection(NABoxi box1, NABoxi box2) {
  NABoxi newBox;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(!naIsBoxiValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("box2 is invalid.");
  #endif
  newBox.vertex.x = naMaxi(box1.vertex.x, box2.vertex.x);
  newBox.vertex.y = naMaxi(box1.vertex.y, box2.vertex.y);
  newBox.vertex.z = naMaxi(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newBox.volume.width  = naMakeLengthWithStartAndEndi(newBox.vertex.x, naMini(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newBox.volume.height  = naMakeLengthWithStartAndEndi(newBox.vertex.y, naMini(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newBox.volume.depth  = naMakeLengthWithStartAndEndi(newBox.vertex.z, naMini(end1, end2));
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
NA_IDEF NARecti naMakeRectiWithRectAndBorder(NARecti rect, NAInt border) {
  NARecti newRect;
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
NA_IDEF NABoxi naMakeBoxiWithBoxAndBorder(NABoxi box, NAInt border) {
  NABoxi newBox;
  newBox.vertex.x = box.vertex.x - border;
  newBox.vertex.y = box.vertex.y - border;
  newBox.vertex.z = box.vertex.z - border;
  newBox.volume.width = box.volume.width + 2 * border;
  newBox.volume.height = box.volume.height + 2 * border;
  newBox.volume.depth = box.volume.depth + 2 * border;
  return newBox;
}



NA_IDEF NARange naMakeRangePositive(NARange range) {
  NAInt rangeNegative = (NAInt)(range.length < 0);
  range.origin += rangeNegative * range.length;
  range.length -= (double)rangeNegative * 2. * range.length;
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



NA_IDEF NABox naMakeBoxPositive(NABox box) {
  NARange range1 = naMakeRangePositive(naMakeRangeE(box.vertex.x, box.volume.width));
  NARange range2 = naMakeRangePositive(naMakeRangeE(box.vertex.y, box.volume.height));
  NARange range3 = naMakeRangePositive(naMakeRangeE(box.vertex.y, box.volume.height));
  return naMakeBoxS(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
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
// COPY
// /////////////////////////////////

NA_IDEF void naFillPosWithCopy(NAPos* posd, const NAPos* poss) {
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naFillPosiWithCopy(NAPosi* posd, const NAPosi* poss) {
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naFillSizeWithCopy(NASize* sized, const NASize* sizes) {
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naFillSizeiWithCopy(NASizei* sized, const NASizei* sizes) {
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naFillRectWithCopy(NARect* rectd, const NARect* rects) {
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}
NA_IDEF void naFillRectiWithCopy(NARecti* rectd, const NARecti* rects) {
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}



NA_IDEF void naFillVertexWithCopy(NAVertex* vertexd, const NAVertex* vertexs) {
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naFillVertexiWithCopy(NAVertexi* vertexd, const NAVertexi* vertexs) {
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naFillVolumeWithCopy(NAVolume* volumed, const NAVolume* volumes) {
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naFillVolumeiWithCopy(NAVolumei* volumed, const NAVolumei* volumes) {
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naFillBoxWithCopy(NABox* boxd, const NABox* boxs) {
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
}
NA_IDEF void naFillBoxiWithCopy(NABoxi* boxd, const NABoxi* boxs) {
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
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
NA_IDEF NAPosi naGetRectiCenter(NARecti rect) {
  NAPosi newPos;
  #if NA_DEBUG
    if(!naIsRectiUseful(rect))
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
NA_IDEF NAVertexi naGetBoxiCenter(NABoxi box) {
  NAVertexi newVertex;
  #if NA_DEBUG
    if(!naIsBoxiUseful(box))
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
NA_IDEF NAPosi naClampPosiToRect(NAPosi pos, NARecti clampRect) {
  NAInt max;
  NAPosi newPos;
  #if NA_DEBUG
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(naIsRectiEmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectiValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newPos = pos;
  if(newPos.x <  clampRect.pos.x) { newPos.x = clampRect.pos.x; }
  max = naGetRectiMaxX(clampRect);
  if(newPos.x > max) {
    newPos.x = max;
  }
  if(newPos.y <  clampRect.pos.y) { newPos.y = clampRect.pos.y; }
  max = naGetRectiMaxY(clampRect);
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
NA_IDEF NAVertexi naClampVertexiToBox(NAVertexi vertex, NABoxi clampBox) {
  NAInt max;
  NAVertexi newVertex;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxiEmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxiValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newVertex = vertex;
  if(newVertex.x <  clampBox.vertex.x) { newVertex.x = clampBox.vertex.x; }
  max = naGetBoxiMaxX(clampBox);
  if(newVertex.x > max) {
    newVertex.x = max;
  }
  if(newVertex.y <  clampBox.vertex.y) { newVertex.y = clampBox.vertex.y; }
  max = naGetBoxiMaxY(clampBox);
  if(newVertex.y > max) {
    newVertex.y = max;
  }
  if(newVertex.z <  clampBox.vertex.z) { newVertex.z = clampBox.vertex.z; }
  max = naGetBoxiMaxZ(clampBox);
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
NA_IDEF NABoxi naClampBoxiToBox(NABoxi box, NABoxi clampBox) {
  // Todo: use helper functions in this function.
  NAInt value;
  NABoxi newBox;
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
    if(naIsBoxiEmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxiValid(clampBox))
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
  value = naGetBoxiEndX(clampBox);
  if(naGetBoxiEndX(newBox) > value) {
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
  value = naGetBoxiEndY(clampBox);
  if(naGetBoxiEndY(newBox) > value) {
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
  value = naGetBoxiEndZ(clampBox);
  if(naGetBoxiEndZ(newBox) > value) {
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
NA_IDEF NABool naEqualPosi(NAPosi pos1, NAPosi pos2) {
  #if NA_DEBUG
    if(!naIsPosiValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosiValid(pos2))
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
NA_IDEF NABool naEqualSizei(NASizei size1, NASizei size2) {
  #if NA_DEBUG
    if(naIsSizeiEmpty(size1) || naIsSizeiEmpty(size2))
      naError("Equality test not valid for empty sizes.");
    if(!naIsSizeiValid(size1))
      naError("size1 is invalid.");
    if(!naIsSizeiValid(size2))
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
  return     (naEqualPos (rect1.pos,  rect2.pos)
          &&  naEqualSize(rect1.size, rect2.size));
}
NA_IDEF NABool naEqualRecti(NARecti rect1, NARecti rect2) {
  #if NA_DEBUG
    if(naIsRectiEmpty(rect1) || naIsRectiEmpty(rect2))
      naError("Equality test not valid for empty rects.");
    if(!naIsRectiValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("rect2 is invalid.");
  #endif
  return    (naEqualPosi (rect1.pos,  rect2.pos)
          && naEqualSizei(rect1.size, rect2.size));
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
NA_IDEF NABool naEqualVertexi(NAVertexi vertex1, NAVertexi vertex2) {
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
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
NA_IDEF NABool naEqualVolumei(NAVolumei volume1, NAVolumei volume2) {
  #if NA_DEBUG
    if(naIsVolumeiEmpty(volume1) || naIsVolumeiEmpty(volume2))
      naError("Equality test not valid for empty volumes.");
    if(!naIsVolumeiValid(volume1))
      naError("volume1 is invalid.");
    if(!naIsVolumeiValid(volume2))
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
NA_IDEF NABool naEqualBoxi(NABoxi box1, NABoxi box2) {
  #if NA_DEBUG
    if(naIsBoxiEmpty(box1) || naIsBoxiEmpty(box2))
      naError("Equality test not valid for empty boxs.");
    if(!naIsBoxiValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("box2 is invalid.");
  #endif
  return    (naEqualVertexi (box1.vertex,  box2.vertex)
          && naEqualVolumei(box1.volume, box2.volume));
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
NA_IDEF NAPosi naAddPosiSize(NAPosi pos, NASizei size) {
  NAPosi newPos;
  #if NA_DEBUG
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(!naIsSizeiValid(size))
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
NA_IDEF NASizei naSubPosiPos(NAPosi pos1, NAPosi pos2) {
  NASizei newSize;
  #if NA_DEBUG
    if(!naIsPosiValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosiValid(pos2))
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
NA_IDEF NAVertexi naAddVertexiVolume(NAVertexi vertex, NAVolumei volume) {
  NAVertexi newVertex;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumeiValid(volume))
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
NA_IDEF NAVolumei naSubVertexiVertex(NAVertexi vertex1, NAVertexi vertex2) {
  NAVolumei newVolume;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
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
NA_IDEF NABool naContainsSizeiPos(NASizei outerSize, NAPosi pos) {
  #if NA_DEBUG
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(naIsSizeiUseful(outerSize))
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
NA_IDEF NABool naContainsSizeiSize(NASizei outerSize, NASizei size) {
  #if NA_DEBUG
    if(!naIsSizeiUseful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizeiUseful(outerSize))
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
NA_IDEF NABool naContainsVolumeiVertex(NAVolumei outerVolume, NAVertexi vertex) {
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(naIsVolumeiUseful(outerVolume))
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
NA_IDEF NABool naContainsBoxiVertex(NABoxi outerBox, NAVertexi vertex) {
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxiEmptySlow(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxiValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((vertex.x >= outerBox.vertex.x)
        && (vertex.x <  naGetBoxiEndX(outerBox))
        && (vertex.y >= outerBox.vertex.y)
        && (vertex.y <  naGetBoxiEndY(outerBox))
        && (vertex.z >= outerBox.vertex.z)
        && (vertex.z <  naGetBoxiEndZ(outerBox)));
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
NA_IDEF NABool naContainsVolumeiVolume(NAVolumei outerVolume, NAVolumei volume) {
  #if NA_DEBUG
    if(!naIsVolumeiUseful(volume))
      naError("Inside test not valid if volume is not useful.");
    if(!naIsVolumeiUseful(outerVolume))
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
NA_IDEF NABool naContainsBoxiBox(NABoxi outerBox, NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmpty(box) || naIsBoxiEmpty(outerBox))
      naError("Inside test not valid for empty boxs.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
    if(!naIsBoxiValid(outerBox))
      naError("outerBox is invalid.");
  #endif
  return  ((box.vertex.x           >= outerBox.vertex.x)
        && (naGetBoxiEndX(box) <=  naGetBoxiEndX(outerBox))
        && (box.vertex.y           >= outerBox.vertex.y)
        && (naGetBoxiEndY(box) <=  naGetBoxiEndY(outerBox))
        && (box.vertex.z           >= outerBox.vertex.z)
        && (naGetBoxiEndZ(box) <=  naGetBoxiEndZ(outerBox)));
}




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



NA_IDEF NAPos naGetRectEnd(NARect rect) {
  #if NA_DEBUG
    if(naIsRectEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePos(naGetRectEndX(rect), naGetRectEndY(rect));
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
NA_IDEF double naGetRectEndY(NARect rect) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(rect.pos.y, rect.size.height)))
      naError("rect is empty in Y dimension.");
    if(!naIsOffsetValueValid(rect.pos.y) || !naIsLengthValueValid(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
}
NA_IDEF double naGetRectEndXE(NARect rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(rect.pos.x) || !naIsLengthValueValid(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.x, rect.size.width);
}
NA_IDEF double naGetRectEndYE(NARect rect) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(rect.pos.y) || !naIsLengthValueValid(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
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
NA_IDEF double naGetBoxEndX(NABox box) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.x, box.volume.width)))
      naError("box is empty in X dimension.");
    if(!naIsOffsetValueValid(box.vertex.x) || !naIsLengthValueValid(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
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
NA_IDEF double naGetBoxEndZ(NABox box) {
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.z, box.volume.depth)))
      naError("box is empty in Z dimension.");
    if(!naIsOffsetValueValid(box.vertex.z) || !naIsLengthValueValid(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
}
NA_IDEF double naGetBoxEndXE(NABox box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.x) || !naIsLengthValueValid(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
}
NA_IDEF double naGetBoxEndYE(NABox box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.y) || !naIsLengthValueValid(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.y, box.volume.height);
}
NA_IDEF double naGetBoxEndZE(NABox box) {
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.z) || !naIsLengthValueValid(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
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
NA_IDEF NAInt naGetRectiMaxX(NARecti rect) {
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(rect.pos.x, rect.size.width);
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
NA_IDEF NAInt naGetRectiMaxY(NARecti rect) {
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(rect.pos.y, rect.size.height);
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



NA_IDEF NAVertexi naGetBoxiEnd (NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi(naGetBoxiEndX(box), naGetBoxiEndY(box), naGetBoxiEndZ(box));
}
NA_IDEF NAInt naGetBoxiEndX(NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiEndY(NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiEndZ(NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.z, box.volume.depth);
}
NA_IDEF NAVertexi naGetBoxiMax (NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi(naGetBoxiMaxX(box), naGetBoxiMaxY(box), naGetBoxiMaxZ(box));
}
NA_IDEF NAInt naGetBoxiMaxX(NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiMaxY(NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiMaxZ(NABoxi box) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.z, box.volume.depth);
}



NA_IDEF size_t naGetIndexWithOriginAndPosRowFirst(NAPosi origin, NAPosi offset, NAInt width) {
  return (size_t)((offset.y - origin.y) * width + (offset.x - origin.x));
}
NA_IDEF size_t naGetIndexWithOriginAndPosColumnFirst(NAPosi origin, NAPosi offset, NAInt height) {
  return (size_t)((offset.x - origin.x) * height + (offset.y - origin.y));
}
NA_IDEF size_t naGetIndexWithOriginAndVertexRowFirst(NAVertexi origin, NAVertexi vertex, NAInt width, NAInt height) {
  return (size_t)(((vertex.z - origin.z) * height + (vertex.y - origin.y)) * width + (vertex.x - origin.x));
}
NA_IDEF size_t naGetIndexWithOriginAndVertexColumnFirst(NAVertexi origin, NAVertexi vertex, NAInt depth, NAInt height) {
  return (size_t)(((vertex.x - origin.x) * height + (vertex.y - origin.y)) * depth + (vertex.z - origin.z));
}



NA_IDEF size_t naGetSizeiIndexCount(NASizei size) {
  #if NA_DEBUG
    if(naIsSizeiEmptySlow(size))
      naError("size is empty.");
    if(!naIsSizeiValid(size))
      naError("size is invalid.");
    if(!naIsSizeiUseful(size))
      naError("size is not useful.");
  #endif
  return (size_t)(size.width * size.height);
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



//NA_IDEF size_t naGetRectiIndexOfPosRowFirst(NARecti rect, NAPosi pos) {
//  #if NA_DEBUG
//    if(naIsRectiEmptySlow(rect))
//      naError("rect is empty.");
//    if(!naIsRectiValid(rect))
//      naError("rect is invalid.");
//    if(!naIsPosiValid(pos))
//      naError("pos is invalid.");
//    if(!naContainsRectiPos(rect, pos))
//      naError("pos is not inside rect.");
//  #endif
//  return naGetIndexWithOriginAndPosRowFirst(rect.pos, pos, rect.size.width);
//}
//NA_IDEF size_t naGetRectiIndexOfPosColumnFirst(NARecti rect, NAPosi pos) {
//  #if NA_DEBUG
//    if(naIsRectiEmptySlow(rect))
//      naError("rect is empty.");
//    if(!naIsRectiValid(rect))
//      naError("rect is invalid.");
//    if(!naIsPosiValid(pos))
//      naError("pos is invalid.");
//    if(!naContainsRectiPos(rect, pos))
//      naError("pos is not inside rect.");
//  #endif
//  return naGetIndexWithOriginAndPosColumnFirst(rect.pos, pos, rect.size.height);
//}



NA_IDEF size_t naGetVolumeiIndexCount(NAVolumei volume) {
  #if NA_DEBUG
    if(naIsVolumeiEmptySlow(volume))
      naError("volume is empty.");
    if(!naIsVolumeiValid(volume))
      naError("volume is invalid.");
    if(!naIsVolumeiUseful(volume))
      naError("volume is not useful.");
  #endif
  return (size_t)(volume.width * volume.height * volume.depth);
}



NA_IDEF size_t naGetBoxiIndexOfVertexRowFirst(NABoxi box, NAVertexi vertex) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxiVertex(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOriginAndVertexRowFirst(box.vertex, vertex, box.volume.width, box.volume.height);
}
NA_IDEF size_t naGetBoxiIndexOfVertexColumnFirst(NABoxi box, NAVertexi vertex) {
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(!naContainsBoxiVertex(box, vertex))
      naError("vertex is not inside box.");
  #endif
  return naGetIndexWithOriginAndVertexColumnFirst(box.vertex, vertex, box.volume.depth, box.volume.height);
}



NA_IDEF NABool naIsRangeValid(NARange range) {
  return naIsOffsetValueValid(range.origin) && naIsLengthValueValid(range.length);
}
NA_IDEF NABool naIsRangefValid(NARangef range) {
  return naIsOffsetValueValidf(range.origin) && naIsLengthValueValidf(range.length);
}
NA_IDEF NABool naIsRangei32Valid(NARangei32 range) {
  return naIsOffsetValueValidi32(range.origin) && naIsLengthValueValidi32(range.length);
}
NA_IDEF NABool naIsRangei64Valid(NARangei64 range) {
  return naIsOffsetValueValidi64(range.origin) && naIsLengthValueValidi64(range.length);
}



NA_IDEF NABool naIsPosValid(NAPos pos) {
  return naIsOffsetValueValid(pos.x) && naIsOffsetValueValid(pos.y);
}
NA_IDEF NABool naIsPosfValid(NAPosf pos) {
  return naIsOffsetValueValidf(pos.x) && naIsOffsetValueValidf(pos.y);
}
NA_IDEF NABool naIsPosiValid(NAPosi pos) {
  return naIsOffsetValueValidi(pos.x) && naIsOffsetValueValidi(pos.y);
}
NA_IDEF NABool naIsPosi32Valid(NAPosi32 pos) {
  return naIsOffsetValueValidi32(pos.x) && naIsOffsetValueValidi32(pos.y);
}
NA_IDEF NABool naIsPosi64Valid(NAPosi64 pos) {
  return naIsOffsetValueValidi64(pos.x) && naIsOffsetValueValidi64(pos.y);
}
NA_IDEF NABool naIsPossValid(NAPoss pos) {
  return naIsOffsetValueValids(pos.x) && naIsOffsetValueValids(pos.y);
}
NA_IDEF NABool naIsSizeValid(NASize size) {
  return naIsLengthValueValid(size.width) && naIsLengthValueValid(size.height);
}
NA_IDEF NABool naIsSizefValid(NASizef size) {
  return naIsLengthValueValidf(size.width) && naIsLengthValueValidf(size.height);
}
NA_IDEF NABool naIsSizeiValid(NASizei size) {
  return naIsLengthValueValidi(size.width) && naIsLengthValueValidi(size.height);
}
NA_IDEF NABool naIsSizei32Valid(NASizei32 size) {
  return naIsLengthValueValidi32(size.width) && naIsLengthValueValidi32(size.height);
}
NA_IDEF NABool naIsSizei64Valid(NASizei64 size) {
  return naIsLengthValueValidi64(size.width) && naIsLengthValueValidi64(size.height);
}
NA_IDEF NABool naIsSizesValid(NASizes size) {
  return naIsLengthValueValids(size.width) && naIsLengthValueValids(size.height);
}
NA_IDEF NABool naIsRectValid(NARect rect) {
  return (naIsPosValid(rect.pos) && naIsSizeValid(rect.size));
}
NA_IDEF NABool naIsRectiValid(NARecti rect) {
  return (naIsPosiValid(rect.pos) && naIsSizeiValid(rect.size));
}
NA_IDEF NABool naIsRecti32Valid(NARecti32 rect) {
  return (naIsPosi32Valid(rect.pos) && naIsSizei32Valid(rect.size));
}
NA_IDEF NABool naIsRecti64Valid(NARecti64 rect) {
  return (naIsPosi64Valid(rect.pos) && naIsSizei64Valid(rect.size));
}



NA_IDEF NABool naIsVertexValid(NAVertex vertex) {
  return naIsOffsetValueValid(vertex.x) && naIsOffsetValueValid(vertex.y) && naIsOffsetValueValid(vertex.z);
}
NA_IDEF NABool naIsVertexiValid(NAVertexi vertex) {
  return naIsOffsetValueValidi(vertex.x) && naIsOffsetValueValidi(vertex.y) && naIsOffsetValueValidi(vertex.z);
}
NA_IDEF NABool naIsVolumeValid(NAVolume volume) {
  return naIsLengthValueValid(volume.width) && naIsLengthValueValid(volume.height) && naIsLengthValueValid(volume.depth);
}
NA_IDEF NABool naIsVolumeiValid(NAVolumei volume) {
  return naIsLengthValueValidi(volume.width) && naIsLengthValueValidi(volume.height) && naIsLengthValueValidi(volume.depth);
}
NA_IDEF NABool naIsBoxValid(NABox box) {
  return (naIsVertexValid(box.vertex) && naIsVolumeValid(box.volume));
}
NA_IDEF NABool naIsBoxiValid(NABoxi box) {
  return (naIsVertexiValid(box.vertex) && naIsVolumeiValid(box.volume));
}



NA_IDEF NABool naIsRangeEmpty(NARange range) {
  return naIsLengthValueEmpty(range.length);
}
NA_IDEF NABool naIsRangefEmpty(NARangef range) {
  return naIsLengthValueEmptyf(range.length);
}
NA_IDEF NABool naIsRangei32Empty(NARangei32 range) {
  return naIsLengthValueEmptyi32(range.length);
}
NA_IDEF NABool naIsRangei64Empty(NARangei64 range) {
  return naIsLengthValueEmptyi64(range.length);
}



NA_IDEF NABool naIsSizeEmpty (NASize size) {
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmpty(size.width) || naIsLengthValueEmpty(size.height);
}
NA_IDEF NABool naIsSizeiEmpty(NASizei size) {
  NAInt totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi(totalSize) && !naIsLengthValueEmptyi(size.width) && !naIsLengthValueEmptyi(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi(totalSize);
}
NA_IDEF NABool naIsSizeiEmptySlow(NASizei size) {
  return (naIsLengthValueEmptyi(size.width) || naIsLengthValueEmptyi(size.height));
}
NA_IDEF NABool naIsSizei32Empty(NASizei32 size) {
  int32 totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi32(totalSize) && !naIsLengthValueEmptyi32(size.width) && !naIsLengthValueEmptyi32(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi32(totalSize);
}
NA_IDEF NABool naIsSizei32EmptySlow(NASizei32 size) {
  return (naIsLengthValueEmptyi32(size.width) || naIsLengthValueEmptyi32(size.height));
}
NA_IDEF NABool naIsSizei64Empty(NASizei64 size) {
  int64 totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi64(totalSize) && !naIsLengthValueEmptyi64(size.width) && !naIsLengthValueEmptyi64(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi64(totalSize);
}
NA_IDEF NABool naIsSizei64EmptySlow(NASizei64 size) {
  return (naIsLengthValueEmptyi64(size.width) || naIsLengthValueEmptyi64(size.height));
}
NA_IDEF NABool naIsSizesEmpty(NASizes size) {
  size_t totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptys(totalSize) && !naIsLengthValueEmptys(size.width) && !naIsLengthValueEmptys(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizesEmptySlow");
  #endif
  return naIsLengthValueEmptys(totalSize);
}
NA_IDEF NABool naIsSizesEmptySlow(NASizes size) {
  return (naIsLengthValueEmptys(size.width) || naIsLengthValueEmptys(size.height));
}
NA_IDEF NABool naIsRectEmpty (NARect rect) {
  return naIsSizeEmpty(rect.size);
}
NA_IDEF NABool naIsRectEmptySlow (NARect rect) {
  return naIsSizeEmpty(rect.size);
}
NA_IDEF NABool naIsRectiEmpty(NARecti rect) {
  return naIsSizeiEmpty(rect.size);
}
NA_IDEF NABool naIsRecti32Empty(NARecti32 rect) {
  return naIsSizei32Empty(rect.size);
}
NA_IDEF NABool naIsRecti64Empty(NARecti64 rect) {
  return naIsSizei64Empty(rect.size);
}
NA_IDEF NABool naIsRectiEmptySlow(NARecti rect) {
  return naIsSizeiEmptySlow(rect.size);
}
NA_IDEF NABool naIsRecti32EmptySlow(NARecti32 rect) {
  return naIsSizei32EmptySlow(rect.size);
}
NA_IDEF NABool naIsRecti64EmptySlow(NARecti64 rect) {
  return naIsSizei64EmptySlow(rect.size);
}



NA_IDEF NABool naIsVolumeEmpty (NAVolume volume) {
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmpty(volume.width) || naIsLengthValueEmpty(volume.height) || naIsLengthValueEmpty(volume.depth);
}
NA_IDEF NABool naIsVolumeiEmpty(NAVolumei volume) {
  NAInt totalvolume = volume.width * volume.height * volume.depth;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi(totalvolume) && !naIsLengthValueEmptyi(volume.width) && !naIsLengthValueEmptyi(volume.height) && !naIsLengthValueEmptyi(volume.depth))
      naError("You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi(totalvolume);
}
NA_IDEF NABool naIsVolumeiEmptySlow(NAVolumei volume) {
  return (naIsLengthValueEmptyi(volume.width) || naIsLengthValueEmptyi(volume.height) || naIsLengthValueEmptyi(volume.depth));
}
NA_IDEF NABool naIsBoxEmpty (NABox box) {
  return naIsVolumeEmpty(box.volume);
}
NA_IDEF NABool naIsBoxiEmpty(NABoxi box) {
  return naIsVolumeiEmpty(box.volume);
}
NA_IDEF NABool naIsBoxiEmptySlow(NABoxi box) {
  return naIsVolumeiEmptySlow(box.volume);
}



NA_IDEF NABool naIsRangeNegative(NARange range) {
  return naIsLengthValueNegative(range.length);
}
NA_IDEF NABool naIsRangefNegative(NARangef range) {
  return naIsLengthValueNegativef(range.length);
}
NA_IDEF NABool naIsRangei32Negative(NARangei32 range) {
  return naIsLengthValueNegativei32(range.length);
}
NA_IDEF NABool naIsRangei64Negative(NARangei64 range) {
  return naIsLengthValueNegativei64(range.length);
}



NA_IDEF NABool naIsSizeNegative(NASize size) {
  return (naIsLengthValueNegative(size.width) || naIsLengthValueNegative(size.height));
}
NA_IDEF NABool naIsSizeiNegative(NASizei size) {
  return (naIsLengthValueNegativei(size.width) || naIsLengthValueNegativei(size.height));
}
NA_IDEF NABool naIsRectNegative(NARect rect) {
  return naIsSizeNegative(rect.size);
}
NA_IDEF NABool naIsRectiNegative(NARecti rect) {
  return naIsSizeiNegative(rect.size);
}



NA_IDEF NABool naIsVolumeNegative(NAVolume volume) {
  return (naIsLengthValueNegative(volume.width) || naIsLengthValueNegative(volume.height) || naIsLengthValueNegative(volume.depth));
}
NA_IDEF NABool naIsVolumeiNegative(NAVolumei volume) {
  return (naIsLengthValueNegativei(volume.width) || naIsLengthValueNegativei(volume.height) || naIsLengthValueNegativei(volume.depth));
}
NA_IDEF NABool naIsBoxNegative(NABox box) {
  return naIsVolumeNegative(box.volume);
}
NA_IDEF NABool naIsBoxiNegative(NABoxi box) {
  return naIsVolumeiNegative(box.volume);
}



NA_IDEF NABool naIsRangeUseful(NARange range) {
  return (naIsOffsetValueUseful(range.origin) && naIsLengthValueUseful(range.length));
}
NA_IDEF NABool naIsRangefUseful(NARangef range) {
  return (naIsOffsetValueUsefulf(range.origin) && naIsLengthValueUsefulf(range.length));
}
NA_IDEF NABool naIsRangei32Useful(NARangei32 range) {
  return (naIsOffsetValueUsefuli32(range.origin) && naIsLengthValueUsefuli32(range.length));
}
NA_IDEF NABool naIsRangei64Useful(NARangei64 range) {
  return (naIsOffsetValueUsefuli64(range.origin) && naIsLengthValueUsefuli64(range.length));
}



NA_IDEF NABool naIsPosUseful(NAPos pos) {
  return naIsOffsetValueUseful(pos.x) && naIsOffsetValueUseful(pos.y);
}
NA_IDEF NABool naIsPosfUseful(NAPosf pos) {
  return naIsOffsetValueUsefulf(pos.x) && naIsOffsetValueUsefulf(pos.y);
}
NA_IDEF NABool naIsPosiUseful(NAPosi pos) {
  return naIsOffsetValueUsefuli(pos.x) && naIsOffsetValueUsefuli(pos.y);
}
NA_IDEF NABool naIsPosi32Useful(NAPosi32 pos) {
  return naIsOffsetValueUsefuli32(pos.x) && naIsOffsetValueUsefuli32(pos.y);
}
NA_IDEF NABool naIsPosi64Useful(NAPosi64 pos) {
  return naIsOffsetValueUsefuli64(pos.x) && naIsOffsetValueUsefuli64(pos.y);
}
NA_IDEF NABool naIsPossUseful(NAPoss pos) {
  return naIsOffsetValueUsefuls(pos.x) && naIsOffsetValueUsefuls(pos.y);
}
NA_IDEF NABool naIsSizeUseful(NASize size) {
  return naIsLengthValueUseful(size.width) && naIsLengthValueUseful(size.height);
}
NA_IDEF NABool naIsSizefUseful(NASizef size) {
  return naIsLengthValueUsefulf(size.width) && naIsLengthValueUsefulf(size.height);
}
NA_IDEF NABool naIsSizeiUseful(NASizei size) {
  return naIsLengthValueUsefuli(size.width) && naIsLengthValueUsefuli(size.height);
}
NA_IDEF NABool naIsSizei32Useful(NASizei32 size) {
  return naIsLengthValueUsefuli32(size.width) && naIsLengthValueUsefuli32(size.height);
}
NA_IDEF NABool naIsSizei64Useful(NASizei64 size) {
  return naIsLengthValueUsefuli64(size.width) && naIsLengthValueUsefuli64(size.height);
}
NA_IDEF NABool naIsSizesUseful(NASizes size) {
  return naIsLengthValueUsefuls(size.width) && naIsLengthValueUsefuls(size.height);
}
NA_IDEF NABool naIsRectUseful(NARect rect) {
  return (naIsPosUseful(rect.pos) && naIsSizeUseful(rect.size));
}
NA_IDEF NABool naIsRectiUseful(NARecti rect) {
  return (naIsPosiUseful(rect.pos) && naIsSizeiUseful(rect.size));
}
NA_IDEF NABool naIsRecti32Useful(NARecti32 rect) {
  return (naIsPosi32Useful(rect.pos) && naIsSizei32Useful(rect.size));
}
NA_IDEF NABool naIsRecti64Useful(NARecti64 rect) {
  return (naIsPosi64Useful(rect.pos) && naIsSizei64Useful(rect.size));
}


NA_IDEF NABool naIsVertexUseful(NAVertex vertex) {
  return naIsOffsetValueUseful(vertex.x) && naIsOffsetValueUseful(vertex.y) && naIsOffsetValueUseful(vertex.x);
}
NA_IDEF NABool naIsVertexiUseful(NAVertexi vertex) {
  return naIsOffsetValueUsefuli(vertex.x) && naIsOffsetValueUsefuli(vertex.y) && naIsOffsetValueUsefuli(vertex.x);
}
NA_IDEF NABool naIsVolumeUseful(NAVolume volume) {
  return naIsLengthValueUseful(volume.width) && naIsLengthValueUseful(volume.height) && naIsLengthValueUseful(volume.depth);
}
NA_IDEF NABool naIsVolumeiUseful(NAVolumei volume) {
  return naIsLengthValueUsefuli(volume.width) && naIsLengthValueUsefuli(volume.height) && naIsLengthValueUsefuli(volume.depth);
}
NA_IDEF NABool naIsBoxUseful(NABox box) {
  return (naIsVertexUseful(box.vertex) && naIsVolumeUseful(box.volume));
}
NA_IDEF NABool naIsBoxiUseful(NABoxi box) {
  return (naIsVertexiUseful(box.vertex) && naIsVolumeiUseful(box.volume));
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
