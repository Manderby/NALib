
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



#include "../NAMathOperators.h"
#include "../NAValueHelper.h"


NA_IDEF NARange naMakeRange(double origin, double length){
  NARange newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(origin) && naIsLengthValueValid(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueUseful(length))
      naError("length is not useful.");
  #endif
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}
NA_IDEF NARangef naMakeRangef(float origin, float length){
  NARangef newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(origin) && naIsLengthValueValidf(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueUsefulf(length))
      naError("length is not useful.");
  #endif
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}
NA_IDEF NARangei naMakeRangei(NAInt origin, NAInt length){
  NARangei newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi(origin) && naIsLengthValueValidi(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueUsefuli(length))
      naError("length is not useful.");
  #endif
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}
NA_IDEF NARange naMakeRangeE (double origin, double length){
  NARange newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(origin) && naIsLengthValueValid(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueValid(length))
      naError("length is not valid.");
  #endif
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}
NA_IDEF NARangei naMakeRangeiE(NAInt origin, NAInt length){
  NARangei newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi(origin) && naIsLengthValueValidi(length)))
      naError("Invalid values given.");
    if(!naIsLengthValueValidi(length))
      naError("length is not valid.");
  #endif
  newRange.origin = origin;
  newRange.length = length;
  return newRange;
}
NA_IDEF NARange naMakeRangeWithRangei(NARangei range){
  NARange newRange = naMakeRangeE((double)range.origin, (double)range.length);
  return newRange;
}
NA_IDEF NARangei naMakeRangeiWithRange(NARange range){
  NARangei newRange = naMakeRangeiE((NAInt)range.origin, (NAInt)range.length);
  return newRange;
}
NA_IDEF NARange naMakeRangeEmpty(){
  NARange newRange;
  newRange.length = 0.;
  return newRange;
}
NA_IDEF NARangef naMakeRangefEmpty(){
  NARangef newRange;
  newRange.length = 0.f;
  return newRange;
}
NA_IDEF NARangei naMakeRangeiEmpty(){
  NARangei newRange;
  newRange.length = 0;
  return newRange;
}



NA_IDEF NAPos naMakePos(double x, double y){
  NAPos newpos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(x) && naIsOffsetValueValid(y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(x) && naIsOffsetValueUseful(y)))
      naError("Values given are not useful.");
  #endif
  newpos.x = x;
  newpos.y = y;
  return newpos;
}
NA_IDEF NAPosi naMakePosi(NAInt x, NAInt y){
  NAPosi newpos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi(x) && naIsOffsetValueValidi(y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuli(x) && naIsOffsetValueUsefuli(y)))
      naError("Values given are not useful.");
  #endif
  newpos.x = x;
  newpos.y = y;
  return newpos;
}
NA_IDEF NAPos naMakePosWithPosi(NAPosi pos){
  NAPos newpos = naMakePos((double)pos.x, (double)pos.y);
  return newpos;
}
NA_IDEF NAPosi naMakePosiWithPos(NAPos pos){
  NAPosi newpos = naMakePosi((NAInt)pos.x, (NAInt)pos.y);
  return newpos;
}
NA_IDEF NAPosi naMakePosiWithIntegerPos(NAPos pos){
  NAPosi newpos = naMakePosi((NAInt)naGetDoubleInteger(pos.x), (NAInt)naGetDoubleInteger(pos.y));
  return newpos;
}
NA_IDEF NAPos naMakePosWithV2(double* v){
  NAPos newpos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(v[0]) && naIsOffsetValueValid(v[1])))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(v[0]) && naIsOffsetValueUseful(v[1])))
      naError("Values given are not useful.");
  #endif
  newpos.x = v[0];
  newpos.y = v[1];
  return newpos;
}



NA_IDEF NASize naMakeSize(double width, double height){
  NASize newsize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(width) && naIsLengthValueUseful(height)))
      naError("Values given are not useful.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASizei naMakeSizei(NAInt width, NAInt height){
  NASizei newsize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli(width) && naIsLengthValueUsefuli(height)))
      naError("Values given are not useful.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASize naMakeSizeE(double width, double height){
  NASize newsize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height)))
      naError("Invalid values given.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASizei naMakeSizeiE(NAInt width, NAInt height){
  NASizei newsize;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height)))
      naError("Invalid values given.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASize naMakeSizeWithSizei(NASizei size){
  NASize newsize = naMakeSizeE((double)size.width, (double)size.height);
  return newsize;
}
NA_IDEF NASizei naMakeSizeiWithSize(NASize size){
  NASizei newsize = naMakeSizeiE((NAInt)size.width, (NAInt)size.height);
  return newsize;
}
NA_IDEF NASizei naMakeSizeiWithIntegerSize(NASize size){
  NASizei newsize = naMakeSizeiE((NAInt)naGetDoubleInteger(size.width), (NAInt)naGetDoubleInteger(size.height));
  return newsize;
}
NA_IDEF NASize naMakeSizeEmpty(){
  NASize newsize;
  newsize.width = 0.;
  return newsize;
}
NA_IDEF NASizei naMakeSizeiEmpty(){
  NASizei newsize;
  newsize.width = 0;
  return newsize;
}



NA_IDEF NARect naMakeRect(NAPos pos, NASize size){
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("Invalid values given.");
    if(!naIsPosUseful(pos) || !naIsSizeUseful(size))
      naError("Values given are not useful.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}
NA_IDEF NARecti naMakeRecti(NAPosi pos, NASizei size){
  NARecti newRect;
  #if NA_DEBUG
    if(!naIsPosiValid(pos) || !naIsSizeiValid(size))
      naError("Invalid values given.");
    if(!naIsPosiUseful(pos) || !naIsSizeiUseful(size))
      naError("Values given are not useful.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}
NA_IDEF NARect naMakeRectE(NAPos pos, NASize size){
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("Invalid values given.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}
NA_IDEF NARecti naMakeRectiE(NAPosi pos, NASizei size){
  NARecti newRect;
  #if NA_DEBUG
    if(!naIsPosiValid(pos) || !naIsSizeiValid(size))
      naError("Invalid values given.");
  #endif
  newRect.pos = pos;
  newRect.size = size;
  return newRect;
}
NA_IDEF NARect naMakeRectS(double x, double y, double width, double height){
  NARect newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValid(x) || !naIsOffsetValueValid(y) || !naIsLengthValueValid(width) || !naIsLengthValueValid(height))
      naError("Invalid values given.");
    if(!naIsOffsetValueUseful(x) || !naIsOffsetValueUseful(y) || !naIsLengthValueUseful(width) || !naIsLengthValueUseful(height))
      naError("Values given are not useful.");
  #endif
  newRect.pos = naMakePos(x, y);
  newRect.size = naMakeSize(width, height);
  return newRect;
}
NA_IDEF NARecti naMakeRectiS(NAInt x, NAInt y, NAInt width, NAInt height){
  NARecti newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi(x) || !naIsOffsetValueValidi(y) || !naIsLengthValueValidi(width) || !naIsLengthValueValidi(height))
      naError("Invalid values given.");
    if(!naIsOffsetValueUsefuli(x) || !naIsOffsetValueUsefuli(y) || !naIsLengthValueUsefuli(width) || !naIsLengthValueUsefuli(height))
      naError("Values given are not useful.");
  #endif
  newRect.pos = naMakePosi(x, y);
  newRect.size = naMakeSizei(width, height);
  return newRect;
}
NA_IDEF NARect naMakeRectSE(double x, double y, double width, double height){
  NARect newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValid(x) || !naIsOffsetValueValid(y) || !naIsLengthValueValid(width) || !naIsLengthValueValid(height))
      naError("Invalid values given.");
  #endif
  newRect.pos = naMakePos(x, y);
  newRect.size = naMakeSizeE(width, height);
  return newRect;
}
NA_IDEF NARecti naMakeRectiSE(NAInt x, NAInt y, NAInt width, NAInt height){
  NARecti newRect;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi(x) || !naIsOffsetValueValidi(y) || !naIsLengthValueValidi(width) || !naIsLengthValueValidi(height))
      naError("Invalid values given.");
  #endif
  newRect.pos = naMakePosi(x, y);
  newRect.size = naMakeSizeiE(width, height);
  return newRect;
}
NA_IDEF NARect naMakeRectWithRecti(NARecti rect){
  NARect newRect = naMakeRectSE((double)rect.pos.x, (double)rect.pos.y, (double)rect.size.width, (double)rect.size.height);
  return newRect;
}
NA_IDEF NARecti naMakeRectiWithRect(NARect rect){
  NARecti newRect = naMakeRectiSE((NAInt)rect.pos.x, (NAInt)rect.pos.y, (NAInt)rect.size.width, (NAInt)rect.size.height);
  return newRect;
}
NA_IDEF NARect naMakeRectEmpty(){
  NARect newRect;
  newRect.size.width = 0.;
  return newRect;
}
NA_IDEF NARecti naMakeRectiEmpty(){
  NARecti newRect;
  newRect.size.width = 0;
  return newRect;
}



NA_IDEF NABounds4 naMakeBounds4(double top, double right, double bottom, double left){
  NABounds4 newbounds;
  #if NA_DEBUG
    if(!naIsLengthValueValid(top) || !naIsLengthValueValid(right) || !naIsLengthValueValid(bottom) || !naIsLengthValueValid(left))
      naError("Invalid values given.");
  #endif
  newbounds.top = top;
  newbounds.right = right;
  newbounds.bottom = bottom;
  newbounds.left = left;
  return newbounds;
}
NA_IDEF NABounds4i naMakeBounds4i(NAInt top, NAInt right, NAInt bottom, NAInt left){
  NABounds4i newbounds;
  #if NA_DEBUG
    if(!naIsLengthValueValidi(top) || !naIsLengthValueValidi(right) || !naIsLengthValueValidi(bottom) || !naIsLengthValueValidi(left))
      naError("Invalid values given.");
  #endif
  newbounds.top = top;
  newbounds.right = right;
  newbounds.bottom = bottom;
  newbounds.left = left;
  return newbounds;
}
NA_IDEF NABounds4 naMakeBounds4WithBounds4i(NABounds4i bounds){
  NABounds4 newbounds = naMakeBounds4((double)bounds.top, (double)bounds.right, (double)bounds.bottom, (double)bounds.left);
  return newbounds;
}
NA_IDEF NABounds4i naMakeBounds4iWithBounds4(NABounds4 bounds){
  NABounds4i newbounds = naMakeBounds4i((NAInt)bounds.top, (NAInt)bounds.right, (NAInt)bounds.bottom, (NAInt)bounds.left);
  return newbounds;
}



NA_IDEF NAVertex naMakeVertex(double x, double y, double z){
  NAVertex newvertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(x) && naIsOffsetValueValid(y) && naIsOffsetValueValid(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(x) && naIsOffsetValueUseful(y) && naIsOffsetValueUseful(z)))
      naError("Values given are not useful.");
  #endif
  newvertex.x = x;
  newvertex.y = y;
  newvertex.z = z;
  return newvertex;
}
NA_IDEF NAVertexi naMakeVertexi(NAInt x, NAInt y, NAInt z){
  NAVertexi newvertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi(x) && naIsOffsetValueValidi(y) && naIsOffsetValueValidi(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuli(x) && naIsOffsetValueUsefuli(y) && naIsOffsetValueUsefuli(z)))
      naError("Values given are not useful.");
  #endif
  newvertex.x = x;
  newvertex.y = y;
  newvertex.z = z;
  return newvertex;
}
NA_IDEF NAVertex naMakeVertexWithVertexi(NAVertexi vertex){
  NAVertex newvertex = naMakeVertex((double)vertex.x, (double)vertex.y, (double)vertex.z);
  return newvertex;
}
NA_IDEF NAVertexi naMakeVertexiWithVertex(NAVertex vertex){
  NAVertexi newvertex = naMakeVertexi((NAInt)vertex.x, (NAInt)vertex.y, (NAInt)vertex.z);
  return newvertex;
}
NA_IDEF NAVertex naMakeVertexWithV3(double* v){
  NAVertex newvertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(v[0]) && naIsOffsetValueValid(v[1]) && naIsOffsetValueValid(v[2])))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(v[0]) && naIsOffsetValueUseful(v[1]) && naIsOffsetValueUseful(v[2])))
      naError("Values given are not useful.");
  #endif
  newvertex.x = v[0];
  newvertex.y = v[1];
  newvertex.z = v[2];
  return newvertex;
}



NA_IDEF NAVolume naMakeVolume(double width, double height, double depth){
  NAVolume newvolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(width) && naIsLengthValueUseful(height) && naIsLengthValueUseful(depth)))
      naError("Values given are not useful.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolumei naMakeVolumei(NAInt width, NAInt height, NAInt depth){
  NAVolumei newvolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height) && naIsLengthValueValidi(depth)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUsefuli(width) && naIsLengthValueUsefuli(height) && naIsLengthValueUsefuli(depth)))
      naError("Values given are not useful.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolume naMakeVolumeE(double width, double height, double depth){
  NAVolume newvolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("Invalid values given.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolumei naMakeVolumeiE(NAInt width, NAInt height, NAInt depth){
  NAVolumei newvolume;
  #if NA_DEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height) && naIsLengthValueValidi(depth)))
      naError("Invalid values given.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolume naMakeVolumeWithVolumei(NAVolumei volume){
  NAVolume newvolume = naMakeVolumeE((double)volume.width, (double)volume.height, (double)volume.depth);
  return newvolume;
}
NA_IDEF NAVolumei naMakeVolumeiWithVolume(NAVolume volume){
  NAVolumei newvolume = naMakeVolumeiE((NAInt)volume.width, (NAInt)volume.height, (NAInt)volume.depth);
  return newvolume;
}
NA_IDEF NAVolume naMakeVolumeEmpty(){
  NAVolume newvolume;
  newvolume.width = 0.;
  return newvolume;
}
NA_IDEF NAVolumei naMakeVolumeiEmpty(){
  NAVolumei newvolume;
  newvolume.width = 0;
  return newvolume;
}



NA_IDEF NABox naMakeBox(NAVertex vertex, NAVolume volume){
  NABox newbox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("Invalid values given.");
    if(!naIsVertexUseful(vertex) || !naIsVolumeUseful(volume))
      naError("Values given are not useful.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxi(NAVertexi vertex, NAVolumei volume){
  NABoxi newbox;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("Invalid values given.");
    if(!naIsVertexiUseful(vertex) || !naIsVolumeiUseful(volume))
      naError("Values given are not useful.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABox naMakeBoxS(double x, double y, double z, double width, double height, double depth){
  NABox newbox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newbox.vertex = naMakeVertex(x, y, z);
  newbox.volume = naMakeVolume(width, height, depth);
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiS(NAInt x, NAInt y, NAInt z, NAInt width, NAInt height, NAInt depth){
  NABoxi newbox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newbox.vertex = naMakeVertexi(x, y, z);
  newbox.volume = naMakeVolumei(width, height, depth);
  return newbox;
}
NA_IDEF NABox naMakeBoxE(NAVertex vertex, NAVolume volume){
  NABox newbox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("Invalid values given.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiE(NAVertexi vertex, NAVolumei volume){
  NABoxi newbox;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("Invalid values given.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABox naMakeBoxSE(double x, double y, double z, double width, double height, double depth){
  NABox newbox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newbox.vertex = naMakeVertex(x, y, z);
  newbox.volume = naMakeVolumeE(width, height, depth);
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiSE(NAInt x, NAInt y, NAInt z, NAInt width, NAInt height, NAInt depth){
  NABoxi newbox;
  // We do not test for bad values at this point as it is already done in
  // the following Make functions.
  newbox.vertex = naMakeVertexi(x, y, z);
  newbox.volume = naMakeVolumeiE(width, height, depth);
  return newbox;
}
NA_IDEF NABox naMakeBoxWithBoxi(NABoxi box){
  NABox newbox = naMakeBoxSE((double)box.vertex.x, (double)box.vertex.y, (double)box.vertex.z, (double)box.volume.width, (double)box.volume.height, (double)box.volume.depth);
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBox(NABox box){
  NABoxi newbox = naMakeBoxiSE((NAInt)box.vertex.x, (NAInt)box.vertex.y, (NAInt)box.vertex.z, (NAInt)box.volume.width, (NAInt)box.volume.height, (NAInt)box.volume.depth);
  return newbox;
}
NA_IDEF NABox naMakeBoxEmpty(){
  NABox newbox;
  newbox.volume.width = 0.;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiEmpty(){
  NABoxi newbox;
  newbox.volume.width = 0;
  return newbox;
}



#ifdef CGGEOMETRY_H_
NA_IDEF NAPos naMakePosWithCGPoint(CGPoint cgPoint){
  NAPos newpos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(cgPoint.x) && naIsOffsetValueValid(cgPoint.y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(cgPoint.x) && naIsOffsetValueUseful(cgPoint.y)))
      naError("Values given are not useful.");
  #endif
  newpos.x = cgPoint.x;
  newpos.y = cgPoint.y;
  return newpos;
}
NA_IDEF NASize naMakeSizeWithCGSize(CGSize cgSize){
  NASize newsize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(cgSize.width) && naIsLengthValueValid(cgSize.height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(cgSize.width) && naIsLengthValueUseful(cgSize.height)))
      naError("Values given are not useful.");
  #endif
  newsize.width = cgSize.width;
  newsize.height = cgSize.height;
  return newsize;
}
NA_IDEF NARect naMakeRectWithCGRect(CGRect cgRect){
  NARect newRect;
  newRect.pos = naMakePosWithCGPoint(cgRect.origin);
  newRect.size = naMakeSizeWithCGSize(cgRect.size);
  return newRect;
}
#endif



#if defined __OBJC__ && defined __AVAILABILITYMACROS__
NA_IDEF NAPos naMakePosWithNSPoint(NSPoint nsPoint){
  NAPos newpos;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(nsPoint.x) && naIsOffsetValueValid(nsPoint.y)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUseful(nsPoint.x) && naIsOffsetValueUseful(nsPoint.y)))
      naError("Values given are not useful.");
  #endif
  newpos.x = nsPoint.x;
  newpos.y = nsPoint.y;
  return newpos;
}
NA_IDEF NSPoint naMakeNSPointWithPos(NAPos naPos){
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
NA_IDEF NASize naMakeSizeWithNSSize(NSSize nsSize){
  NASize newsize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(nsSize.width) && naIsLengthValueValid(nsSize.height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(nsSize.width) && naIsLengthValueUseful(nsSize.height)))
      naError("Values given are not useful.");
  #endif
  newsize.width = nsSize.width;
  newsize.height = nsSize.height;
  return newsize;
}
NA_IDEF NSSize naMakeNSSizeWithSize(NASize naSize){
  NSSize newsize;
  #if NA_DEBUG
    if(!(naIsLengthValueValid(naSize.width) && naIsLengthValueValid(naSize.height)))
      naError("Invalid values given.");
    if(!(naIsLengthValueUseful(naSize.width) && naIsLengthValueUseful(naSize.height)))
      naError("Values given are not useful.");
  #endif
  newsize.width = (CGFloat)naSize.width;
  newsize.height = (CGFloat)naSize.height;
  return newsize;
}
NA_IDEF NARect naMakeRectWithNSRect(NSRect nsRect){
  NARect newRect;
  newRect.pos = naMakePosWithNSPoint(nsRect.origin);
  newRect.size = naMakeSizeWithNSSize(nsRect.size);
  return newRect;
}
NA_IDEF NSRect naMakeNSRectWithSize(NASize naSize){
  NSRect newRect;
  newRect.origin = NSMakePoint(0., 0.);
  newRect.size = naMakeNSSizeWithSize(naSize);
  return newRect;
}
NA_IDEF NSRect naMakeNSRectWithRect(NARect naRect){
  return NSMakeRect(
    (CGFloat)naRect.pos.x,
    (CGFloat)naRect.pos.y,
    (CGFloat)naRect.size.width,
    (CGFloat)naRect.size.height);
}
#endif



NA_IDEF NAPosi naMakePosiWithAlignment(NAPosi origin, NARecti alignRect){
  NAPosi neworigin;
  #if NA_DEBUG
    if(naIsRectiEmptySlow(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  neworigin.x = naAlignValuei(origin.x, alignRect.pos.x, alignRect.size.width);
  neworigin.y = naAlignValuei(origin.y, alignRect.pos.y, alignRect.size.height);
  return neworigin;
}
NA_IDEF NAPos naMakePosWithAlignment(NAPos origin, NARect alignRect){
  NAPos neworigin;
  #if NA_DEBUG
    if(naIsRectEmpty(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  neworigin.x = naAlignValued(origin.x, alignRect.pos.x, alignRect.size.width);
  neworigin.y = naAlignValued(origin.y, alignRect.pos.y, alignRect.size.height);
  return neworigin;
}
NA_IDEF NAVertexi naMakeVertexiWithAlignment(NAVertexi origin, NABoxi alignBox){
  NAVertexi neworigin;
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(alignBox))
      naCrash("alignment box is empty.");
  #endif
  neworigin.x = naAlignValuei(origin.x, alignBox.vertex.x, alignBox.volume.width);
  neworigin.y = naAlignValuei(origin.y, alignBox.vertex.y, alignBox.volume.height);
  neworigin.z = naAlignValuei(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return neworigin;
}
NA_IDEF NAVertex naMakeVertexWithAlignment(NAVertex origin, NABox alignBox){
  NAVertex neworigin;
  #if NA_DEBUG
    if(naIsBoxEmpty(alignBox))
      naCrash("alignment box is empty.");
  #endif
  neworigin.x = naAlignValued(origin.x, alignBox.vertex.x, alignBox.volume.width);
  neworigin.y = naAlignValued(origin.y, alignBox.vertex.y, alignBox.volume.height);
  neworigin.z = naAlignValued(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return neworigin;
}



NA_IDEF NARange naMakeRangeWithStartAndEnd(double start, double end){
  NARange newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValid(start) && naIsOffsetValueValid(end)))
      naError("Invalid values given.");
  #endif
  newRange.origin = start;
  newRange.length = naMakeLengthWithStartAndEnd(start, end);
  return newRange;
}
NA_IDEF NARangef naMakeRangefWithStartAndEnd(float start, float end){
  NARangef newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(start) && naIsOffsetValueValidf(end)))
      naError("Invalid values given.");
  #endif
  newRange.origin = start;
  newRange.length = naMakeLengthWithStartAndEndf(start, end);
  return newRange;
}
NA_IDEF NARangei naMakeRangeiWithStartAndEnd(NAInt start, NAInt end){
  NARangei newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi(start) && naIsOffsetValueValidi(end)))
      naError("Invalid values given.");
  #endif
  newRange.origin = start;
  newRange.length = naMakeLengthWithStartAndEndi(start, end);
  return newRange;
}
NA_IDEF NARangei naMakeRangeiWithMinAndMax(NAInt min, NAInt max){
  NARangei newRange;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidi(min) && naIsOffsetValueValidi(max)))
      naError("Invalid values given.");
    if(max < min)
      naError("min should be smallerequal to max.");
  #endif
  newRange.origin = min;
  newRange.length = naMakeLengthWithMinAndMaxi(min, max);
  return newRange;
}
NA_IDEF NARangei naMakeRangeiWithRangeAndOffset(NARangei range, NAInt offset){
  NARangei newRange;
  #if NA_DEBUG
    if(!naIsOffsetValueValidi(offset))
      naError("Invalid offset given.");
  #endif
  if(naIsRangeiEmpty(range)){
    newRange.length = 1;
    newRange.origin = offset;
  }else if(offset < range.origin){
    newRange.length = naMakeLengthWithStartAndEndi(offset, naGetRangeiEnd(range));
    newRange.origin = offset;
  }else if(offset > naGetRangeiMax(range)){
    newRange.length = naMakeLengthWithMinAndMaxi(range.origin, offset);
    newRange.origin = range.origin;
  }else{
    newRange = range;
  }
  return newRange;
}
NA_IDEF NARangei naMakeRangeiWithRangeAndRange (NARangei range1, NARangei range2){
  NARangei newRange;
  #if NA_DEBUG
    if(!naIsRangeiValid(range1))
      naError("Invalid range 1 given.");
    if(!naIsRangeiValid(range2))
      naError("Invalid range 2 given.");
  #endif
  if(naIsRangeiEmpty(range1) && naIsRangeiEmpty(range2)){
    newRange = naMakeRangeiE(0, 0);
  }else if(naIsRangeiEmpty(range1)){
    newRange = range2;
  }else if(naIsRangeiEmpty(range2)){
    newRange = range1;
  }else{
    newRange = naMakeRangeiWithStartAndEnd(naMini(range1.origin, range2.origin), naMaxi(naGetRangeiEnd(range1), naGetRangeiEnd(range2)));
  }
  return newRange;
}



NA_IDEF NARect naMakeRectWithPosAndPos(NAPos pos1, NAPos pos2){
  NARect newRect;
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x){
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithStartAndEnd(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithStartAndEnd(pos2.y, pos1.y);
  }
  return newRect;
}
NA_IDEF NARect naMakeRectWithRectAndPos(NARect rect, NAPos pos){
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
NA_IDEF NARect naMakeRectWithRectAndPosE(NARect rect, NAPos pos){
  NARect newRect;
  #if NA_DEBUG
    if(!naIsRectEmpty(rect) && !naIsRectValid(rect))
      naError("rect is invalid.");
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
  #endif
  if(naIsRectEmpty(rect)){
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
NA_IDEF NARect naMakeRectWithRectAndRect(NARect rect1, NARect rect2){
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
NA_IDEF NARect naMakeRectWithRectAndRectE(NARect rect1, NARect rect2){
  NARect newRect;
  double end1;
  double end2;
  #if NA_DEBUG
    if(!naIsRectEmpty(rect1) && !naIsRectValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectEmpty(rect2) && !naIsRectValid(rect2))
      naError("rect2 is invalid.");
  #endif
  if(naIsRectEmpty(rect1)){return rect2;}
  if(naIsRectEmpty(rect2)){return rect1;}
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
NA_IDEF NARecti naMakeRectiWithPosAndPos(NAPosi pos1, NAPosi pos2){
  NARecti newRect;
  #if NA_DEBUG
    if(!naIsPosiValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x){
    newRect.pos.x = pos1.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi(pos1.x, pos2.x);
  }else{
    newRect.pos.x = pos2.x;
    newRect.size.width = naMakeLengthWithMinAndMaxi(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newRect.pos.y = pos1.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi(pos1.y, pos2.y);
  }else{
    newRect.pos.y = pos2.y;
    newRect.size.height = naMakeLengthWithMinAndMaxi(pos2.y, pos1.y);
  }
  return newRect;
}
NA_IDEF NARecti naMakeRectiWithRectAndPos(NARecti rect, NAPosi pos){
  NARecti newRect;
  NAInt max;
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiUseful(rect))
      naError("rect is not useful.");
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
  #endif
  newRect.pos.x = naMini(rect.pos.x, pos.x);
  newRect.pos.y = naMini(rect.pos.y, pos.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRectiMaxX(rect);
  newRect.size.width  = naMakeLengthWithMinAndMaxi(newRect.pos.x, naMaxi(max, pos.x));
  max = naGetRectiMaxY(rect);
  newRect.size.height = naMakeLengthWithMinAndMaxi(newRect.pos.y, naMaxi(max, pos.y));
  return newRect;
}
NA_IDEF NARecti naMakeRectiWithRectAndPosE(NARecti rect, NAPosi pos){
  NARecti newRect;
  #if NA_DEBUG
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
  #endif
  if(naIsRectiEmpty(rect)){
    newRect.pos = pos;
    newRect.size = naMakeSizei(1, 1);
  }else{
    NAInt max;
    newRect.pos.x = naMini(rect.pos.x, pos.x);
    newRect.pos.y = naMini(rect.pos.y, pos.y);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetRectiMaxX(rect);
    newRect.size.width  = naMakeLengthWithMinAndMaxi(newRect.pos.x, naMaxi(max, pos.x));
    max = naGetRectiMaxY(rect);
    newRect.size.height = naMakeLengthWithMinAndMaxi(newRect.pos.y, naMaxi(max, pos.y));
  }
  return newRect;
}
NA_IDEF NARecti naMakeRectiWithRectAndRect(NARecti rect1, NARecti rect2){
  NARecti newRect;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect1))
      naError("rect1 is empty.");
    if(!naIsRectiValid(rect1))
      naError("rect1 is invalid.");
    if(naIsRectiEmptySlow(rect2))
      naError("rect2 is empty.");
    if(!naIsRectiValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndi(newRect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newRect.size.height = naMakeLengthWithStartAndEndi(newRect.pos.y, naMaxi(end1, end2));
  return newRect;
}
NA_IDEF NARecti naMakeRectiWithRectAndRectE(NARecti rect1, NARecti rect2){
  NARecti newRect;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(!naIsRectiValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("rect2 is invalid.");
  #endif
  if(naIsRectiEmpty(rect1)){return rect2;}
  if(naIsRectiEmpty(rect2)){return rect1;}
  newRect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndi(newRect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newRect.size.height = naMakeLengthWithStartAndEndi(newRect.pos.y, naMaxi(end1, end2));
  return newRect;
}



NA_IDEF NABox naMakeBoxWithVertexAndVertex(NAVertex vertex1, NAVertex vertex2){
  NABox newbox;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  if(vertex2.x > vertex1.x){
    newbox.vertex.x = vertex1.x;
    newbox.volume.width = naMakeLengthWithStartAndEnd(vertex1.x, vertex2.x);
  }else{
    newbox.vertex.x = vertex2.x;
    newbox.volume.width = naMakeLengthWithStartAndEnd(vertex2.x, vertex1.x);
  }
  if(vertex2.y > vertex1.y){
    newbox.vertex.y = vertex1.y;
    newbox.volume.height = naMakeLengthWithStartAndEnd(vertex1.y, vertex2.y);
  }else{
    newbox.vertex.y = vertex2.y;
    newbox.volume.height = naMakeLengthWithStartAndEnd(vertex2.y, vertex1.y);
  }
  if(vertex2.z > vertex1.z){
    newbox.vertex.z = vertex1.z;
    newbox.volume.depth = naMakeLengthWithStartAndEnd(vertex1.z, vertex2.z);
  }else{
    newbox.vertex.z = vertex2.z;
    newbox.volume.depth = naMakeLengthWithStartAndEnd(vertex2.z, vertex1.z);
  }
  return newbox;
}
NA_IDEF NABox naMakeBoxWithBoxAndVertex(NABox box, NAVertex vertex){
  NABox newbox;
  double end;
  #if NA_DEBUG
    if(naIsBoxEmpty(box))
      naError("box is empty.");
    if(!naIsBoxUseful(box))
      naError("box is not useful.");
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
  #endif
  newbox.vertex.x = naMin(box.vertex.x, vertex.x);
  newbox.vertex.y = naMin(box.vertex.y, vertex.y);
  newbox.vertex.z = naMin(box.vertex.z, vertex.z);
  end = naGetBoxEndX(box);
  newbox.volume.width  = naMakeLengthWithStartAndEnd(newbox.vertex.x, naMax(end, vertex.x));
  end = naGetBoxEndY(box);
  newbox.volume.height = naMakeLengthWithStartAndEnd(newbox.vertex.y, naMax(end, vertex.y));
  end = naGetBoxEndZ(box);
  newbox.volume.depth = naMakeLengthWithStartAndEnd(newbox.vertex.z, naMax(end, vertex.z));
  return newbox;
}
NA_IDEF NABox naMakeBoxWithBoxAndBox(NABox box1, NABox box2){
  NABox newbox;
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
  newbox.vertex.x = naMin(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMin(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMin(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newbox.volume.width  = naMakeLengthWithStartAndEnd(newbox.vertex.x, naMax(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newbox.volume.height  = naMakeLengthWithStartAndEnd(newbox.vertex.y, naMax(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newbox.volume.depth  = naMakeLengthWithStartAndEnd(newbox.vertex.z, naMax(end1, end2));
  return newbox;
}
NA_IDEF NABox naMakeBoxWithBoxAndBoxE(NABox box1, NABox box2){
  NABox newbox;
  double end1;
  double end2;
  #if NA_DEBUG
    if(!naIsBoxValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("box2 is invalid.");
  #endif
  if(naIsBoxEmpty(box1)){return box2;}
  if(naIsBoxEmpty(box2)){return box1;}
  newbox.vertex.x = naMin(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMin(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMin(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newbox.volume.width  = naMakeLengthWithStartAndEnd(newbox.vertex.x, naMax(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newbox.volume.height  = naMakeLengthWithStartAndEnd(newbox.vertex.y, naMax(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newbox.volume.depth  = naMakeLengthWithStartAndEnd(newbox.vertex.z, naMax(end1, end2));
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithVertexAndVertex(NAVertexi vertex1, NAVertexi vertex2){
  NABoxi newbox;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  if(vertex2.x > vertex1.x){
    newbox.vertex.x = vertex1.x;
    newbox.volume.width = naMakeLengthWithMinAndMaxi(vertex1.x, vertex2.x);
  }else{
    newbox.vertex.x = vertex2.x;
    newbox.volume.width = naMakeLengthWithMinAndMaxi(vertex2.x, vertex1.x);
  }
  if(vertex2.y > vertex1.y){
    newbox.vertex.y = vertex1.y;
    newbox.volume.height = naMakeLengthWithMinAndMaxi(vertex1.y, vertex2.y);
  }else{
    newbox.vertex.y = vertex2.y;
    newbox.volume.height = naMakeLengthWithMinAndMaxi(vertex2.y, vertex1.y);
  }
  if(vertex2.z > vertex1.z){
    newbox.vertex.z = vertex1.z;
    newbox.volume.depth = naMakeLengthWithMinAndMaxi(vertex1.z, vertex2.z);
  }else{
    newbox.vertex.z = vertex2.z;
    newbox.volume.depth = naMakeLengthWithMinAndMaxi(vertex2.z, vertex1.z);
  }
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxAndVertex(NABoxi box, NAVertexi vertex){
  NABoxi newbox;
  NAInt max;
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiUseful(box))
      naError("box is not useful.");
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
  #endif
  newbox.vertex.x = naMini(box.vertex.x, vertex.x);
  newbox.vertex.y = naMini(box.vertex.y, vertex.y);
  newbox.vertex.z = naMini(box.vertex.z, vertex.z);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetBoxiMaxX(box);
  newbox.volume.width  = naMakeLengthWithMinAndMaxi(newbox.vertex.x, naMaxi(max, vertex.x));
  max = naGetBoxiMaxY(box);
  newbox.volume.height = naMakeLengthWithMinAndMaxi(newbox.vertex.y, naMaxi(max, vertex.y));
  max = naGetBoxiMaxZ(box);
  newbox.volume.depth = naMakeLengthWithMinAndMaxi(newbox.vertex.z, naMaxi(max, vertex.z));
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxAndVertexE(NABoxi box, NAVertexi vertex){
  NABoxi newbox;
  #if NA_DEBUG
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
  #endif
  if(naIsBoxiEmpty(box)){
    newbox.vertex = vertex;
    newbox.volume = naMakeVolumei(1, 1, 1);
  }else{
    NAInt max;
    newbox.vertex.x = naMini(box.vertex.x, vertex.x);
    newbox.vertex.y = naMini(box.vertex.y, vertex.y);
    newbox.vertex.z = naMini(box.vertex.z, vertex.z);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetBoxiMaxX(box);
    newbox.volume.width  = naMakeLengthWithMinAndMaxi(newbox.vertex.x, naMaxi(max, vertex.x));
    max = naGetBoxiMaxY(box);
    newbox.volume.height = naMakeLengthWithMinAndMaxi(newbox.vertex.y, naMaxi(max, vertex.y));
    max = naGetBoxiMaxZ(box);
    newbox.volume.depth = naMakeLengthWithMinAndMaxi(newbox.vertex.z, naMaxi(max, vertex.z));
  }
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxAndBox(NABoxi box1, NABoxi box2){
  NABoxi newbox;
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
  newbox.vertex.x = naMini(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMini(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMini(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newbox.volume.width  = naMakeLengthWithStartAndEndi(newbox.vertex.x, naMaxi(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newbox.volume.height = naMakeLengthWithStartAndEndi(newbox.vertex.y, naMaxi(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newbox.volume.depth = naMakeLengthWithStartAndEndi(newbox.vertex.z, naMaxi(end1, end2));
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxAndBoxE(NABoxi box1, NABoxi box2){
  NABoxi newbox;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(!naIsBoxiValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("box2 is invalid.");
  #endif
  if(naIsBoxiEmpty(box1)){return box2;}
  if(naIsBoxiEmpty(box2)){return box1;}
  newbox.vertex.x = naMini(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMini(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMini(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newbox.volume.width  = naMakeLengthWithStartAndEndi(newbox.vertex.x, naMaxi(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newbox.volume.height = naMakeLengthWithStartAndEndi(newbox.vertex.y, naMaxi(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newbox.volume.depth = naMakeLengthWithStartAndEndi(newbox.vertex.z, naMaxi(end1, end2));
  return newbox;
}



NA_IDEF NARange naMakeRangeWithRangeIntersection(NARange range1, NARange range2){
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
NA_IDEF NARangei naMakeRangeiWithRangeIntersection(NARangei range1, NARangei range2){
  NARangei newRange;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(!naIsRangeiValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangeiValid(range2))
      naError("range2 is invalid.");
  #endif
  newRange.origin = naMaxi(range1.origin, range2.origin);
  end1 = naGetRangeiEnd(range1);
  end2 = naGetRangeiEnd(range2);
  newRange.length  = naMakeLengthWithStartAndEndi(newRange.origin, naMini(end1, end2));
  return newRange;
}
NA_IDEF NARect naMakeRectWithRectIntersection(NARect rect1, NARect rect2){
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
NA_IDEF NARecti naMakeRectiWithRectIntersection(NARecti rect1, NARecti rect2){
  NARecti newRect;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(!naIsRectiValid(rect1))
      naError("rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("rect2 is invalid.");
  #endif
  newRect.pos.x = naMaxi(rect1.pos.x, rect2.pos.x);
  newRect.pos.y = naMaxi(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newRect.size.width  = naMakeLengthWithStartAndEndiE(newRect.pos.x, naMini(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newRect.size.height  = naMakeLengthWithStartAndEndiE(newRect.pos.y, naMini(end1, end2));
  return newRect;
}
NA_IDEF NABox naMakeBoxWithBoxIntersection(NABox box1, NABox box2){
  NABox newbox;
  double end1;
  double end2;
  #if NA_DEBUG
    if(!naIsBoxValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("box2 is invalid.");
  #endif
  newbox.vertex.x = naMax(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMax(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMax(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxEndX(box1);
  end2 = naGetBoxEndX(box2);
  newbox.volume.width  = naMakeLengthWithStartAndEnd(newbox.vertex.x, naMin(end1, end2));
  end1 = naGetBoxEndY(box1);
  end2 = naGetBoxEndY(box2);
  newbox.volume.height = naMakeLengthWithStartAndEnd(newbox.vertex.y, naMin(end1, end2));
  end1 = naGetBoxEndZ(box1);
  end2 = naGetBoxEndZ(box2);
  newbox.volume.depth  = naMakeLengthWithStartAndEnd(newbox.vertex.z, naMin(end1, end2));
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxIntersection(NABoxi box1, NABoxi box2){
  NABoxi newbox;
  NAInt end1;
  NAInt end2;
  #if NA_DEBUG
    if(!naIsBoxiValid(box1))
      naError("box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("box2 is invalid.");
  #endif
  newbox.vertex.x = naMaxi(box1.vertex.x, box2.vertex.x);
  newbox.vertex.y = naMaxi(box1.vertex.y, box2.vertex.y);
  newbox.vertex.z = naMaxi(box1.vertex.z, box2.vertex.z);
  end1 = naGetBoxiEndX(box1);
  end2 = naGetBoxiEndX(box2);
  newbox.volume.width  = naMakeLengthWithStartAndEndi(newbox.vertex.x, naMini(end1, end2));
  end1 = naGetBoxiEndY(box1);
  end2 = naGetBoxiEndY(box2);
  newbox.volume.height  = naMakeLengthWithStartAndEndi(newbox.vertex.y, naMini(end1, end2));
  end1 = naGetBoxiEndZ(box1);
  end2 = naGetBoxiEndZ(box2);
  newbox.volume.depth  = naMakeLengthWithStartAndEndi(newbox.vertex.z, naMini(end1, end2));
  return newbox;
}



NA_IDEF NARect naMakeRectWithRectAndBorder(NARect rect, double border){
  NARect newRect;
  newRect.pos.x = rect.pos.x - border;
  newRect.pos.y = rect.pos.y - border;
  newRect.size.width = rect.size.width + 2 * border;
  newRect.size.height = rect.size.height + 2 * border;
  return newRect;
}
NA_IDEF NARecti naMakeRectiWithRectAndBorder(NARecti rect, NAInt border){
  NARecti newRect;
  newRect.pos.x = rect.pos.x - border;
  newRect.pos.y = rect.pos.y - border;
  newRect.size.width = rect.size.width + 2 * border;
  newRect.size.height = rect.size.height + 2 * border;
  return newRect;
}



NA_IDEF NABox naMakeBoxWithBoxAndBorder(NABox box, double border){
  NABox newbox;
  newbox.vertex.x = box.vertex.x - border;
  newbox.vertex.y = box.vertex.y - border;
  newbox.vertex.z = box.vertex.z - border;
  newbox.volume.width = box.volume.width + 2 * border;
  newbox.volume.height = box.volume.height + 2 * border;
  newbox.volume.depth = box.volume.depth + 2 * border;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiWithBoxAndBorder(NABoxi box, NAInt border){
  NABoxi newbox;
  newbox.vertex.x = box.vertex.x - border;
  newbox.vertex.y = box.vertex.y - border;
  newbox.vertex.z = box.vertex.z - border;
  newbox.volume.width = box.volume.width + 2 * border;
  newbox.volume.height = box.volume.height + 2 * border;
  newbox.volume.depth = box.volume.depth + 2 * border;
  return newbox;
}



NA_IDEF NARange naMakeRangePositive(NARange range){
  NAInt rangenegative = (NAInt)(range.length < 0);
  range.origin += rangenegative * range.length;
  range.length -= (double)rangenegative * 2 * range.length;
  return range;
}
NA_IDEF NARangei naMakeRangeiPositive(NARangei range){
  NAInt rangenegative = (NAInt)(range.length < 0);
  range.origin += rangenegative * range.length;
  range.length -= rangenegative * 2 * range.length;
  return range;
}



NA_IDEF NARect naMakeRectPositive(NARect rect){
  NARange range1 = naMakeRangePositive(naMakeRangeE(rect.pos.x, rect.size.width));
  NARange range2 = naMakeRangePositive(naMakeRangeE(rect.pos.y, rect.size.height));
  return naMakeRectS(range1.origin, range2.origin, range1.length, range2.length);
}
NA_IDEF NARecti naMakeRectiPositive(NARecti rect){
  NARangei range1 = naMakeRangeiPositive(naMakeRangeiE(rect.pos.x, rect.size.width));
  NARangei range2 = naMakeRangeiPositive(naMakeRangeiE(rect.pos.y, rect.size.height));
  return naMakeRectiS(range1.origin, range2.origin, range1.length, range2.length);
}



NA_IDEF NABox naMakeBoxPositive(NABox box){
  NARange range1 = naMakeRangePositive(naMakeRangeE(box.vertex.x, box.volume.width));
  NARange range2 = naMakeRangePositive(naMakeRangeE(box.vertex.y, box.volume.height));
  NARange range3 = naMakeRangePositive(naMakeRangeE(box.vertex.y, box.volume.height));
  return naMakeBoxS(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}
NA_IDEF NABoxi naMakeBoxiPositive(NABoxi box){
  NARangei range1 = naMakeRangeiPositive(naMakeRangeiE(box.vertex.x, box.volume.width));
  NARangei range2 = naMakeRangeiPositive(naMakeRangeiE(box.vertex.y, box.volume.height));
  NARangei range3 = naMakeRangeiPositive(naMakeRangeiE(box.vertex.y, box.volume.height));
  return naMakeBoxiS(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}



// /////////////////////////////////
// COPY
// /////////////////////////////////

NA_IDEF void naCopyPos(NAPos* posd, const NAPos* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCopyPosi(NAPosi* posd, const NAPosi* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCopySize(NASize* sized, const NASize* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCopySizei(NASizei* sized, const NASizei* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCopyRect(NARect* rectd, const NARect* rects){
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}
NA_IDEF void naCopyRecti(NARecti* rectd, const NARecti* rects){
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}



NA_IDEF void naCopyVertex(NAVertex* vertexd, const NAVertex* vertexs){
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naCopyVertexi(NAVertexi* vertexd, const NAVertexi* vertexs){
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naCopyVolume(NAVolume* volumed, const NAVolume* volumes){
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naCopyVolumei(NAVolumei* volumed, const NAVolumei* volumes){
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naCopyBox(NABox* boxd, const NABox* boxs){
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
}
NA_IDEF void naCopyBoxi(NABoxi* boxd, const NABoxi* boxs){
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
}



// /////////////////////////////////
// GET
// /////////////////////////////////

NA_IDEF NAPos naGetRectCenter(NARect rect){
  NAPos newpos;
  #if NA_DEBUG
    if(!naIsRectUseful(rect))
      naError("rect is not useful");
  #endif
  newpos.x = rect.pos.x + 0.5 * rect.size.width;
  newpos.y = rect.pos.y + 0.5 * rect.size.height;
  return newpos;
}
NA_IDEF NAPosi naGetRectiCenter(NARecti rect){
  NAPosi newpos;
  #if NA_DEBUG
    if(!naIsRectiUseful(rect))
      naError("rect is not useful");
  #endif
  newpos.x = rect.pos.x + rect.size.width / 2;
  newpos.y = rect.pos.y + rect.size.height / 2;
  return newpos;
}
NA_IDEF NAVertex naGetBoxCenter(NABox box){
  NAVertex newvertex;
  #if NA_DEBUG
    if(!naIsBoxUseful(box))
      naError("box is not useful");
  #endif
  newvertex.x = box.vertex.x + 0.5 * box.volume.width;
  newvertex.y = box.vertex.y + 0.5 * box.volume.height;
  newvertex.z = box.vertex.z + 0.5 * box.volume.depth;
  return newvertex;
}
NA_IDEF NAVertexi naGetBoxiCenter(NABoxi box){
  NAVertexi newvertex;
  #if NA_DEBUG
    if(!naIsBoxiUseful(box))
      naError("box is not useful");
  #endif
  newvertex.x = box.vertex.x + box.volume.width / 2;
  newvertex.y = box.vertex.y + box.volume.height / 2;
  newvertex.z = box.vertex.z + box.volume.depth / 2;
  return newvertex;
}



NA_IDEF NAPos naGetRectCenteredSizeOffset(NARect rect, NASize size){
  NAPos newpos;
  newpos.x = rect.pos.x + 0.5 * (rect.size.width - size.width);
  newpos.y = rect.pos.y + 0.5 * (rect.size.height - size.height);
  return newpos;
}
NA_IDEF NAVertex naGetBoxCeneteredVolumeOffset(NABox box, NAVolume volume){
  NAVertex newvertex;
  newvertex.x = box.vertex.x + 0.5 * (box.volume.width - volume.width);
  newvertex.y = box.vertex.y + 0.5 * (box.volume.height - volume.height);
  newvertex.z = box.vertex.z + 0.5 * (box.volume.depth - volume.depth);
  return newvertex;
}



// /////////////////////////////////
// CLAMP
// /////////////////////////////////

NA_IDEF NARangei naClampRangeiToRange(NARangei range, NARangei clampRange){
  NAInt value;
  NARangei newRange;
  #if NA_DEBUG
    if(naIsRangeiEmpty(range))
      naError("range is empty.");
    if(!naIsRangeiValid(range))
      naError("range is invalid.");
    if(naIsRangeiEmpty(clampRange))
      naError("clampRange is empty.");
    if(!naIsRangeiValid(clampRange))
      naError("clampRange is invalid.");
  #endif
  newRange = range;

  value = clampRange.origin - newRange.origin;
  if(value > 0){
    newRange.length -= value;
    newRange.origin = clampRange.origin;
  }
  value = naGetRangeiEnd(clampRange);
  if(naGetRangeiEnd(newRange) > value){
    newRange.length = value - newRange.origin;
  }

  return newRange;
}
NA_IDEF NARangei naClampRangeiToMin(NARangei range, NAInt min){
  NAInt value;
  NARangei newRange;
  #if NA_DEBUG
    if(naIsRangeiEmpty(range))
      naError("range is empty.");
    if(!naIsRangeiValid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi(min))
      naError("min is invalid.");
  #endif
  newRange = range;

  value = min - newRange.origin;
  if(value > 0){
    newRange.length -= value;
    newRange.origin = min;
  }
  return newRange;
}
NA_IDEF NARangei naClampRangeiToEnd(NARangei range, NAInt end){
  NAInt value;
  NARangei newRange;
  #if NA_DEBUG
    if(naIsRangeiEmpty(range))
      naError("range is empty.");
    if(!naIsRangeiValid(range))
      naError("range is invalid.");
    if(!naIsOffsetValueValidi(end))
      naError("min is invalid.");
  #endif
  newRange = range;

  value = end;
  if(naGetRangeiEnd(newRange) > value){
    newRange.length = value - newRange.origin;
  }

  return newRange;
}



NA_IDEF NAPos naClampPosToRect(NAPos pos, NARect clampRect){
  double end;
  NAPos newpos;
  #if NA_DEBUG
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
    if(naIsRectEmpty(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newpos = pos;
  if(newpos.x < clampRect.pos.x) {newpos.x = clampRect.pos.x;}
  end = naGetRectEndX(clampRect);
  if(newpos.x > end){newpos.x = end;}
  if(newpos.y < clampRect.pos.y) {newpos.y = clampRect.pos.y;}
  end = naGetRectEndY(clampRect);
  if(newpos.y > end){newpos.y = end;}
  return newpos;
}
NA_IDEF NAPosi naClampPosiToRect(NAPosi pos, NARecti clampRect){
  NAInt max;
  NAPosi newpos;
  #if NA_DEBUG
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(naIsRectiEmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectiValid(clampRect))
      naError("clampRect is invalid.");
  #endif
  newpos = pos;
  if(newpos.x <  clampRect.pos.x) {newpos.x = clampRect.pos.x;}
  max = naGetRectiMaxX(clampRect);
  if(newpos.x > max){newpos.x = max;}
  if(newpos.y <  clampRect.pos.y) {newpos.y = clampRect.pos.y;}
  max = naGetRectiMaxY(clampRect);
  if(newpos.y > max){newpos.y = max;}
  return newpos;
}
NA_IDEF NARect naClampRectToRect(NARect rect, NARect clampRect){
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
  if(value > 0){
    newRect.size.width -= value;
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRectEndX(clampRect);
  if(naGetRectEndXE(newRect) > value){
    newRect.size.width = value - newRect.pos.x;
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0){
    newRect.size.height -= value;
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRectEndY(clampRect);
  if(naGetRectEndYE(newRect) > value){
    newRect.size.height = value - newRect.pos.y;
  }
  return newRect;
}
NA_IDEF NARecti naClampRectiToRect(NARecti rect, NARecti clampRect){
  // Todo: use helper functions in this function.
  NAInt value;
  NARecti newRect;
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
    if(naIsRectiEmptySlow(clampRect))
      naError("clampRect is empty.");
    if(!naIsRectiValid(clampRect))
      naError("clampRect is invalid.");
  #endif

  newRect = rect;

  // Adjust in first dimension
  value = clampRect.pos.x - newRect.pos.x;
  if(value > 0){
    newRect.size.width -= value;
    if(newRect.size.width <= 0){return newRect;}
    newRect.pos.x = clampRect.pos.x;
  }
  value = naGetRectiEndX(clampRect);
  if(naGetRectiEndX(newRect) > value){
    newRect.size.width = value - newRect.pos.x;
    if(newRect.size.width <= 0){return newRect;}
  }

  // Adjust in second dimension
  value = clampRect.pos.y - newRect.pos.y;
  if(value > 0){
    newRect.size.height -= value;
    if(newRect.size.height <= 0){return newRect;}
    newRect.pos.y = clampRect.pos.y;
  }
  value = naGetRectiEndY(clampRect);
  if(naGetRectiEndY(newRect) > value){
    newRect.size.height = value - newRect.pos.y;
    if(newRect.size.height <= 0){return newRect;}
  }
  return newRect;
}



NA_IDEF NAVertex naClampVertexToBox(NAVertex vertex, NABox clampBox){
  double end;
  NAVertex newvertex;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxEmpty(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newvertex = vertex;
  if(newvertex.x < clampBox.vertex.x) {newvertex.x = clampBox.vertex.x;}
  end = naGetBoxEndX(clampBox);
  if(newvertex.x > end){newvertex.x = end;}
  if(newvertex.y < clampBox.vertex.y) {newvertex.y = clampBox.vertex.y;}
  end = naGetBoxEndY(clampBox);
  if(newvertex.y > end){newvertex.y = end;}
  if(newvertex.z < clampBox.vertex.z) {newvertex.z = clampBox.vertex.z;}
  end = naGetBoxEndZ(clampBox);
  if(newvertex.z > end){newvertex.z = end;}
  return newvertex;
}
NA_IDEF NAVertexi naClampVertexiToBox(NAVertexi vertex, NABoxi clampBox){
  NAInt max;
  NAVertexi newvertex;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(naIsBoxiEmptySlow(clampBox))
      naError("clampBox is empty.");
    if(!naIsBoxiValid(clampBox))
      naError("clampBox is invalid.");
  #endif
  newvertex = vertex;
  if(newvertex.x <  clampBox.vertex.x) {newvertex.x = clampBox.vertex.x;}
  max = naGetBoxiMaxX(clampBox);
  if(newvertex.x > max){newvertex.x = max;}
  if(newvertex.y <  clampBox.vertex.y) {newvertex.y = clampBox.vertex.y;}
  max = naGetBoxiMaxY(clampBox);
  if(newvertex.y > max){newvertex.y = max;}
  if(newvertex.z <  clampBox.vertex.z) {newvertex.z = clampBox.vertex.z;}
  max = naGetBoxiMaxZ(clampBox);
  if(newvertex.z > max){newvertex.z = max;}
  return newvertex;
}
NA_IDEF NABox naClampBoxToBox(NABox box, NABox clampBox){
  // Todo: use helper functions in this function.
  double value;
  NABox newbox;
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
  newbox = box;
  // Adjust in first dimension
  value = clampBox.vertex.x - newbox.vertex.x;
  if(value > 0){
    newbox.volume.width -= value;
    newbox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxEndX(clampBox);
  if(naGetBoxEndXE(newbox) > value){
    newbox.volume.width = value - newbox.vertex.x;
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newbox.vertex.y;
  if(value > 0){
    newbox.volume.height -= value;
    newbox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxEndY(clampBox);
  if(naGetBoxEndYE(newbox) > value){
    newbox.volume.height = value - newbox.vertex.y;
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newbox.vertex.z;
  if(value > 0){
    newbox.volume.depth -= value;
    newbox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxEndZ(clampBox);
  if(naGetBoxEndZE(newbox) > value){
    newbox.volume.depth = value - newbox.vertex.z;
  }
  return newbox;
}
NA_IDEF NABoxi naClampBoxiToBox(NABoxi box, NABoxi clampBox){
  // Todo: use helper functions in this function.
  NAInt value;
  NABoxi newbox;
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

  newbox = box;

  // Adjust in first dimension
  value = clampBox.vertex.x - newbox.vertex.x;
  if(value > 0){
    newbox.volume.width -= value;
    if(newbox.volume.width <= 0){return newbox;}
    newbox.vertex.x = clampBox.vertex.x;
  }
  value = naGetBoxiEndX(clampBox);
  if(naGetBoxiEndX(newbox) > value){
    newbox.volume.width = value - newbox.vertex.x;
    if(newbox.volume.width <= 0){return newbox;}
  }

  // Adjust in second dimension
  value = clampBox.vertex.y - newbox.vertex.y;
  if(value > 0){
    newbox.volume.height -= value;
    if(newbox.volume.height <= 0){return newbox;}
    newbox.vertex.y = clampBox.vertex.y;
  }
  value = naGetBoxiEndY(clampBox);
  if(naGetBoxiEndY(newbox) > value){
    newbox.volume.height = value - newbox.vertex.y;
    if(newbox.volume.height <= 0){return newbox;}
  }

  // Adjust in third dimension
  value = clampBox.vertex.z - newbox.vertex.z;
  if(value > 0){
    newbox.volume.depth -= value;
    if(newbox.volume.depth <= 0){return newbox;}
    newbox.vertex.z = clampBox.vertex.z;
  }
  value = naGetBoxiEndZ(clampBox);
  if(naGetBoxiEndZ(newbox) > value){
    newbox.volume.depth = value - newbox.vertex.z;
    if(newbox.volume.depth <= 0){return newbox;}
  }
  return newbox;
}


// ///////////////////////////////////
// EQUAL
// ///////////////////////////////////

NA_IDEF NABool naEqualRangei(NARangei range1, NARangei range2){
  #if NA_DEBUG
    if(!naIsRangeiValid(range1))
      naError("range1 is invalid.");
    if(!naIsRangeiValid(range2))
      naError("range2 is invalid.");
  #endif
  return ((range1.origin == range2.origin) && (range1.length == range2.length));
}



NA_IDEF NABool naEqualPos(NAPos pos1, NAPos pos2){
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualPosi(NAPosi pos1, NAPosi pos2){
  #if NA_DEBUG
    if(!naIsPosiValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualSize(NASize size1, NASize size2){
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
NA_IDEF NABool naEqualSizei(NASizei size1, NASizei size2){
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
NA_IDEF NABool naEqualRect(NARect rect1, NARect rect2){
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
NA_IDEF NABool naEqualRecti(NARecti rect1, NARecti rect2){
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



NA_IDEF NABool naEqualVertex(NAVertex vertex1, NAVertex vertex2){
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVertexi(NAVertexi vertex1, NAVertexi vertex2){
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVolume(NAVolume volume1, NAVolume volume2){
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
NA_IDEF NABool naEqualVolumei(NAVolumei volume1, NAVolumei volume2){
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
NA_IDEF NABool naEqualBox(NABox box1, NABox box2){
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
NA_IDEF NABool naEqualBoxi(NABoxi box1, NABoxi box2){
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

NA_IDEF NABool naLowerPos(NAPos pos1, NAPos pos2){
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x < pos2.x) && (pos1.y < pos2.y));
}
NA_IDEF NABool naLowerEqualPos(NAPos pos1, NAPos pos2){
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x <= pos2.x) && (pos1.y <= pos2.y));
}
NA_IDEF NABool naGreaterEqualPos(NAPos pos1, NAPos pos2){
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x >= pos2.x) && (pos1.y >= pos2.y));
}
NA_IDEF NABool naGreaterPos(NAPos pos1, NAPos pos2){
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  return ((pos1.x > pos2.x) && (pos1.y > pos2.y));
}



NA_IDEF NABool naLowerVertex(NAVertex vertex1, NAVertex vertex2){
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x < vertex2.x) && (vertex1.y < vertex2.y) && (vertex1.z < vertex2.z));
}
NA_IDEF NABool naLowerEqualVertex(NAVertex vertex1, NAVertex vertex2){
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x <= vertex2.x) && (vertex1.y <= vertex2.y) && (vertex1.z <= vertex2.z));
}
NA_IDEF NABool naGreaterEqualVertex(NAVertex vertex1, NAVertex vertex2){
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  return ((vertex1.x >= vertex2.x) && (vertex1.y >= vertex2.y) && (vertex1.z >= vertex2.z));
}
NA_IDEF NABool naGreaterVertex(NAVertex vertex1, NAVertex vertex2){
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

NA_IDEF NAPos naAddPosSize(NAPos pos, NASize size){
  NAPos newpos;
  #if NA_DEBUG
    if(!naIsPosValid(pos))
      naError("pos is invalid.");
    if(!naIsSizeValid(size))
      naError("size is invalid.");
  #endif
  newpos.x = pos.x + size.width;
  newpos.y = pos.y + size.height;
  return newpos;
}
NA_IDEF NAPosi naAddPosiSize(NAPosi pos, NASizei size){
  NAPosi newpos;
  #if NA_DEBUG
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(!naIsSizeiValid(size))
      naError("size is invalid.");
  #endif
  newpos.x = pos.x + size.width;
  newpos.y = pos.y + size.height;
  return newpos;
}

NA_IDEF NASize naSubPosPos(NAPos pos1, NAPos pos2){
  NASize newsize;
  #if NA_DEBUG
    if(!naIsPosValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("pos2 is invalid.");
  #endif
  newsize.width  = pos1.x - pos2.x;
  newsize.height = pos1.y - pos2.y;
  return newsize;
}
NA_IDEF NASizei naSubPosiPos(NAPosi pos1, NAPosi pos2){
  NASizei newsize;
  #if NA_DEBUG
    if(!naIsPosiValid(pos1))
      naError("pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("pos2 is invalid.");
  #endif
  newsize.width  = pos1.x - pos2.x;
  newsize.height = pos1.y - pos2.y;
  return newsize;
}



NA_IDEF double naDistancePosPos(NAPos pos1, NAPos pos2){
  double dist[2];
  dist[0] = pos2.x - pos1.x;
  dist[1] = pos2.y - pos1.y;
  return naSqrt(dist[0]*dist[0] + dist[1]*dist[1]);
}



NA_IDEF NAVertex naAddVertexVolume(NAVertex vertex, NAVolume volume){
  NAVertex newvertex;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumeValid(volume))
      naError("volume is invalid.");
  #endif
  newvertex.x = vertex.x + volume.width;
  newvertex.y = vertex.y + volume.height;
  newvertex.z = vertex.z + volume.depth;
  return newvertex;
}
NA_IDEF NAVertexi naAddVertexiVolume(NAVertexi vertex, NAVolumei volume){
  NAVertexi newvertex;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex))
      naError("vertex is invalid.");
    if(!naIsVolumeiValid(volume))
      naError("volume is invalid.");
  #endif
  newvertex.x = vertex.x + volume.width;
  newvertex.y = vertex.y + volume.height;
  newvertex.z = vertex.z + volume.depth;
  return newvertex;
}

NA_IDEF NAVolume naSubVertexPos(NAVertex vertex1, NAVertex vertex2){
  NAVolume newvolume;
  #if NA_DEBUG
    if(!naIsVertexValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  newvolume.width  = vertex1.x - vertex2.x;
  newvolume.height = vertex1.y - vertex2.y;
  newvolume.depth  = vertex1.z - vertex2.z;
  return newvolume;
}
NA_IDEF NAVolumei naSubVertexiVertex(NAVertexi vertex1, NAVertexi vertex2){
  NAVolumei newvolume;
  #if NA_DEBUG
    if(!naIsVertexiValid(vertex1))
      naError("vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("vertex2 is invalid.");
  #endif
  newvolume.width  = vertex1.x - vertex2.x;
  newvolume.height = vertex1.y - vertex2.y;
  newvolume.depth  = vertex1.z - vertex2.y;
  return newvolume;
}



// ///////////////////////////////////////
// CONTAIN
// ///////////////////////////////////////

NA_IDEF NABool naContainsRangeOffset(NARange outerRange, double offset){
  #if NA_DEBUG
    if(!naIsOffsetValueValid(offset))
      naError("offset is invalid.");
    if(!naIsRangeUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerRange.origin)
        || (offset > naGetRangeEnd(outerRange)));
}
NA_IDEF NABool naContainsRangefOffset(NARangef outerRange, float offset){
  #if NA_DEBUG
    if(!naIsOffsetValueValidf(offset))
      naError("offset is invalid.");
    if(!naIsRangefUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerRange.origin)
        || (offset > naGetRangefEnd(outerRange)));
}
NA_IDEF NABool naContainsRangeiOffset(NARangei outerRange, NAInt offset){
  #if NA_DEBUG
    if(!naIsOffsetValueValidi(offset))
      naError("offset is invalid.");
    if(!naIsRangeiUseful(outerRange))
      naError("Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerRange.origin)
        || (offset > naGetRangeiMax(outerRange)));
}
NA_IDEF NABool naContainsRangeiRange(NARangei outerRange, NARangei range){
  #if NA_DEBUG
    if(!naIsRangeiUseful(range))
      naError("Inside test not valid if range is not useful.");
    if(!naIsRangeiUseful(outerRange))
      naError("Inside test not valid if outerRange is not useful.");
  #endif
  return  ((range.origin           >= outerRange.origin)
        && (naGetRangeiEnd(range) <=  naGetRangeiEnd(outerRange)));
}




NA_IDEF NABool naContainsSizePos(NASize outerSize, NAPos pos){
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
NA_IDEF NABool naContainsSizeiPos(NASizei outerSize, NAPosi pos){
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
NA_IDEF NABool naContainsRectPos(NARect outerRect, NAPos pos){
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
NA_IDEF NABool naContainsRectPosE(NARect outerRect, NAPos pos){
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
NA_IDEF NABool naContainsRectiPos(NARecti outerRect, NAPosi pos){
  #if NA_DEBUG
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(naIsRectiEmptySlow(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectiValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((pos.x >= outerRect.pos.x)
        && (pos.x <  naGetRectiEndX(outerRect))
        && (pos.y >= outerRect.pos.y)
        && (pos.y <  naGetRectiEndY(outerRect)));
}
NA_IDEF NABool naContainsSizeSize(NASize outerSize, NASize size){
  #if NA_DEBUG
    if(!naIsSizeUseful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizeUseful(outerSize))
      naError("Inside test not valid if outerSize is not useful.");
  #endif
  return !((size.width  > outerSize.width)
        || (size.height > outerSize.height));
}
NA_IDEF NABool naContainsSizeiSize(NASizei outerSize, NASizei size){
  #if NA_DEBUG
    if(!naIsSizeiUseful(size))
      naError("Inside test not valid if size is not useful.");
    if(!naIsSizeiUseful(outerSize))
      naError("Inside test not valid if outerSize is not useful.");
  #endif
  return !((size.width  > outerSize.width)
        || (size.height > outerSize.height));
}
NA_IDEF NABool naContainsRectRect(NARect outerRect, NARect rect){
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
NA_IDEF NABool naContainsRectiRect(NARecti outerRect, NARecti rect){
  #if NA_DEBUG
    if(naIsRectiEmpty(rect) || naIsRectiEmpty(outerRect))
      naError("Inside test not valid for empty rects.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
    if(!naIsRectiValid(outerRect))
      naError("outerRect is invalid.");
  #endif
  return  ((rect.pos.x           >= outerRect.pos.x)
        && (naGetRectiEndX(rect) <=  naGetRectiEndX(outerRect))
        && (rect.pos.y           >= outerRect.pos.y)
        && (naGetRectiEndY(rect) <=  naGetRectiEndY(outerRect)));
}



NA_IDEF NABool naContainsVolumeVertex(NAVolume outerVolume, NAVertex vertex){
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
NA_IDEF NABool naContainsVolumeiVertex(NAVolumei outerVolume, NAVertexi vertex){
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
NA_IDEF NABool naContainsBoxVertex(NABox outerBox, NAVertex vertex){
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
NA_IDEF NABool naContainsBoxVertexE(NABox outerBox, NAVertex vertex){
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
NA_IDEF NABool naContainsBoxiVertex(NABoxi outerBox, NAVertexi vertex){
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
NA_IDEF NABool naContainsVolumeVolume(NAVolume outerVolume, NAVolume volume){
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
NA_IDEF NABool naContainsVolumeiVolume(NAVolumei outerVolume, NAVolumei volume){
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
NA_IDEF NABool naContainsBoxBox(NABox outerBox, NABox box){
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
NA_IDEF NABool naContainsBoxiBox(NABoxi outerBox, NABoxi box){
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




NA_IDEF double naGetRangeEnd(NARange range){
  #if NA_DEBUG
    if(naIsRangeEmpty(range))
      naError("range is empty.");
    if(!naIsRangeValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLength(range.origin, range.length);
}
NA_IDEF float naGetRangefEnd(NARangef range){
  #if NA_DEBUG
    if(naIsRangefEmpty(range))
      naError("range is empty.");
    if(!naIsRangefValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthf(range.origin, range.length);
}
NA_IDEF double naGetRangeEndE(NARange range){
  #if NA_DEBUG
    if(!naIsRangeValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLength(range.origin, range.length);
}
NA_IDEF float naGetRangefEndE(NARangef range){
  #if NA_DEBUG
    if(!naIsRangefValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthf(range.origin, range.length);
}



NA_IDEF NAPos naGetRectEnd(NARect rect){
  #if NA_DEBUG
    if(naIsRectEmpty(rect))
      naError("rect is empty.");
    if(!naIsRectValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePos(naGetRectEndX(rect), naGetRectEndY(rect));
}
NA_IDEF double naGetRectEndX(NARect rect){
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(rect.pos.x, rect.size.width)))
      naError("rect is empty in X dimension.");
    if(!naIsOffsetValueValid(rect.pos.x) || !naIsLengthValueValid(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.x, rect.size.width);
}
NA_IDEF double naGetRectEndY(NARect rect){
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(rect.pos.y, rect.size.height)))
      naError("rect is empty in Y dimension.");
    if(!naIsOffsetValueValid(rect.pos.y) || !naIsLengthValueValid(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
}
NA_IDEF double naGetRectEndXE(NARect rect){
  #if NA_DEBUG
    if(!naIsOffsetValueValid(rect.pos.x) || !naIsLengthValueValid(rect.size.width))
      naError("rect is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.x, rect.size.width);
}
NA_IDEF double naGetRectEndYE(NARect rect){
  #if NA_DEBUG
    if(!naIsOffsetValueValid(rect.pos.y) || !naIsLengthValueValid(rect.size.height))
      naError("rect is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
}



NA_IDEF NAVertex naGetBoxEnd(NABox box){
  #if NA_DEBUG
    if(naIsBoxEmpty(box))
      naError("box is empty.");
    if(!naIsBoxValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertex(naGetBoxEndX(box), naGetBoxEndY(box), naGetBoxEndZ(box));
}
NA_IDEF double naGetBoxEndX(NABox box){
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.x, box.volume.width)))
      naError("box is empty in X dimension.");
    if(!naIsOffsetValueValid(box.vertex.x) || !naIsLengthValueValid(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
}
NA_IDEF double naGetBoxEndY(NABox box){
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.y, box.volume.height)))
      naError("box is empty in Y dimension.");
    if(!naIsOffsetValueValid(box.vertex.y) || !naIsLengthValueValid(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.y, box.volume.height);
}
NA_IDEF double naGetBoxEndZ(NABox box){
  #if NA_DEBUG
    if(naIsRangeEmpty(naMakeRange(box.vertex.z, box.volume.depth)))
      naError("box is empty in Z dimension.");
    if(!naIsOffsetValueValid(box.vertex.z) || !naIsLengthValueValid(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
}
NA_IDEF double naGetBoxEndXE(NABox box){
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.x) || !naIsLengthValueValid(box.volume.width))
      naError("box is has invalid values in X dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
}
NA_IDEF double naGetBoxEndYE(NABox box){
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.y) || !naIsLengthValueValid(box.volume.height))
      naError("box is has invalid values in Y dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.y, box.volume.height);
}
NA_IDEF double naGetBoxEndZE(NABox box){
  #if NA_DEBUG
    if(!naIsOffsetValueValid(box.vertex.z) || !naIsLengthValueValid(box.volume.depth))
      naError("box is has invalid values in Z dimension.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
}



NA_IDEF NAInt naGetRangeiEnd(NARangei range){
  #if NA_DEBUG
    if(naIsRangeiEmpty(range))
      naError("range is empty.");
    if(!naIsRangeiValid(range))
      naError("range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(range.origin, range.length);
}
NA_IDEF NAInt naGetRangeiMax(NARangei range){
  #if NA_DEBUG
    if(naIsRangeiEmpty(range))
      naError("range is empty.");
    if(!naIsRangeiValid(range))
      naError("range is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(range.origin, range.length);
}




NA_IDEF NAPosi naGetRectiEnd (NARecti rect){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePosi(naGetRectiEndX(rect), naGetRectiEndY(rect));
}
NA_IDEF NAInt naGetRectiEndX(NARecti rect){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiEndY(NARecti rect){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(rect.pos.y, rect.size.height);
}
NA_IDEF NAPosi naGetRectiMax(NARecti rect){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakePosi(naGetRectiMaxX(rect), naGetRectiMaxY(rect));
}
NA_IDEF NAInt naGetRectiMaxX(NARecti rect){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiMaxY(NARecti rect){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(rect.pos.y, rect.size.height);
}



NA_IDEF NAVertexi naGetBoxiEnd (NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi(naGetBoxiEndX(box), naGetBoxiEndY(box), naGetBoxiEndZ(box));
}
NA_IDEF NAInt naGetBoxiEndX(NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiEndY(NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiEndZ(NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.z, box.volume.depth);
}
NA_IDEF NAVertexi naGetBoxiMax (NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeVertexi(naGetBoxiMaxX(box), naGetBoxiMaxY(box), naGetBoxiMaxZ(box));
}
NA_IDEF NAInt naGetBoxiMaxX(NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiMaxY(NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiMaxZ(NABoxi box){
  #if NA_DEBUG
    if(naIsBoxiEmptySlow(box))
      naError("box is empty.");
    if(!naIsBoxiValid(box))
      naError("box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.z, box.volume.depth);
}



NA_IDEF size_t naGetIndexWithOriginAndPosRowFirst(NAPosi origin, NAPosi offset, NAInt width){
  return (size_t)((offset.y - origin.y) * width + (offset.x - origin.x));
}
NA_IDEF size_t naGetIndexWithOriginAndPosColumnFirst(NAPosi origin, NAPosi offset, NAInt height){
  return (size_t)((offset.x - origin.x) * height + (offset.y - origin.y));
}
NA_IDEF size_t naGetIndexWithOriginAndVertexRowFirst(NAVertexi origin, NAVertexi vertex, NAInt width, NAInt height){
  return (size_t)(((vertex.z - origin.z) * height + (vertex.y - origin.y)) * width + (vertex.x - origin.x));
}
NA_IDEF size_t naGetIndexWithOriginAndVertexColumnFirst(NAVertexi origin, NAVertexi vertex, NAInt depth, NAInt height){
  return (size_t)(((vertex.x - origin.x) * height + (vertex.y - origin.y)) * depth + (vertex.z - origin.z));
}



NA_IDEF size_t naGetSizeiIndexCount(NASizei size){
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



NA_IDEF size_t naGetRectiIndexOfPosRowFirst(NARecti rect, NAPosi pos){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(!naContainsRectiPos(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosRowFirst(rect.pos, pos, rect.size.width);
}
NA_IDEF size_t naGetRectiIndexOfPosColumnFirst(NARecti rect, NAPosi pos){
  #if NA_DEBUG
    if(naIsRectiEmptySlow(rect))
      naError("rect is empty.");
    if(!naIsRectiValid(rect))
      naError("rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("pos is invalid.");
    if(!naContainsRectiPos(rect, pos))
      naError("pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosColumnFirst(rect.pos, pos, rect.size.height);
}



NA_IDEF size_t naGetVolumeiIndexCount(NAVolumei volume){
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



NA_IDEF size_t naGetBoxiIndexOfVertexRowFirst(NABoxi box, NAVertexi vertex){
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
NA_IDEF size_t naGetBoxiIndexOfVertexColumnFirst(NABoxi box, NAVertexi vertex){
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



NA_IDEF NABool naIsRangeValid(NARange range){
  return naIsOffsetValueValid(range.origin) && naIsLengthValueValid(range.length);
}
NA_IDEF NABool naIsRangefValid(NARangef range){
  return naIsOffsetValueValidf(range.origin) && naIsLengthValueValidf(range.length);
}
NA_IDEF NABool naIsRangeiValid(NARangei range){
  return naIsOffsetValueValidi(range.origin) && naIsLengthValueValidi(range.length);
}



NA_IDEF NABool naIsPosValid(NAPos pos){
  return naIsOffsetValueValid(pos.x) && naIsOffsetValueValid(pos.y);
}
NA_IDEF NABool naIsPosiValid(NAPosi pos){
  return naIsOffsetValueValidi(pos.x) && naIsOffsetValueValidi(pos.y);
}
NA_IDEF NABool naIsSizeValid(NASize size){
  return naIsLengthValueValid(size.width) && naIsLengthValueValid(size.height);
}
NA_IDEF NABool naIsSizeiValid(NASizei size){
  return naIsLengthValueValidi(size.width) && naIsLengthValueValidi(size.height);
}
NA_IDEF NABool naIsRectValid(NARect rect){
  return (naIsPosValid(rect.pos) && naIsSizeValid(rect.size));
}
NA_IDEF NABool naIsRectiValid(NARecti rect){
  return (naIsPosiValid(rect.pos) && naIsSizeiValid(rect.size));
}



NA_IDEF NABool naIsVertexValid(NAVertex vertex){
  return naIsOffsetValueValid(vertex.x) && naIsOffsetValueValid(vertex.y) && naIsOffsetValueValid(vertex.z);
}
NA_IDEF NABool naIsVertexiValid(NAVertexi vertex){
  return naIsOffsetValueValidi(vertex.x) && naIsOffsetValueValidi(vertex.y) && naIsOffsetValueValidi(vertex.z);
}
NA_IDEF NABool naIsVolumeValid(NAVolume volume){
  return naIsLengthValueValid(volume.width) && naIsLengthValueValid(volume.height) && naIsLengthValueValid(volume.depth);
}
NA_IDEF NABool naIsVolumeiValid(NAVolumei volume){
  return naIsLengthValueValidi(volume.width) && naIsLengthValueValidi(volume.height) && naIsLengthValueValidi(volume.depth);
}
NA_IDEF NABool naIsBoxValid(NABox box){
  return (naIsVertexValid(box.vertex) && naIsVolumeValid(box.volume));
}
NA_IDEF NABool naIsBoxiValid(NABoxi box){
  return (naIsVertexiValid(box.vertex) && naIsVolumeiValid(box.volume));
}



NA_IDEF NABool naIsRangeEmpty(NARange range){
  return naIsLengthValueEmpty(range.length);
}
NA_IDEF NABool naIsRangefEmpty(NARangef range){
  return naIsLengthValueEmptyf(range.length);
}
NA_IDEF NABool naIsRangeiEmpty(NARangei range){
  return naIsLengthValueEmptyi(range.length);
}



NA_IDEF NABool naIsSizeEmpty (NASize size){
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmpty(size.width) || naIsLengthValueEmpty(size.height);
}
NA_IDEF NABool naIsSizeiEmpty(NASizei size){
  NAInt totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi(totalSize) && !naIsLengthValueEmptyi(size.width) && !naIsLengthValueEmptyi(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi(totalSize);
}
NA_IDEF NABool naIsSizeiEmptySlow(NASizei size){
  return (naIsLengthValueEmptyi(size.width) || naIsLengthValueEmptyi(size.height));
}
NA_IDEF NABool naIsRectEmpty (NARect rect){
  return naIsSizeEmpty(rect.size);
}
NA_IDEF NABool naIsRectEmptySlow (NARect rect){
  return naIsSizeEmpty(rect.size);
}
NA_IDEF NABool naIsRectiEmpty(NARecti rect){
  return naIsSizeiEmpty(rect.size);
}
NA_IDEF NABool naIsRectiEmptySlow(NARecti rect){
  return naIsSizeiEmptySlow(rect.size);
}



NA_IDEF NABool naIsVolumeEmpty (NAVolume volume){
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmpty(volume.width) || naIsLengthValueEmpty(volume.height) || naIsLengthValueEmpty(volume.depth);
}
NA_IDEF NABool naIsVolumeiEmpty(NAVolumei volume){
  NAInt totalvolume = volume.width * volume.height * volume.depth;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi(totalvolume) && !naIsLengthValueEmptyi(volume.width) && !naIsLengthValueEmptyi(volume.height) && !naIsLengthValueEmptyi(volume.depth))
      naError("You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi(totalvolume);
}
NA_IDEF NABool naIsVolumeiEmptySlow(NAVolumei volume){
  return (naIsLengthValueEmptyi(volume.width) || naIsLengthValueEmptyi(volume.height) || naIsLengthValueEmptyi(volume.depth));
}
NA_IDEF NABool naIsBoxEmpty (NABox box){
  return naIsVolumeEmpty(box.volume);
}
NA_IDEF NABool naIsBoxiEmpty(NABoxi box){
  return naIsVolumeiEmpty(box.volume);
}
NA_IDEF NABool naIsBoxiEmptySlow(NABoxi box){
  return naIsVolumeiEmptySlow(box.volume);
}



NA_IDEF NABool naIsRangeNegative(NARange range){
  return naIsLengthValueNegative(range.length);
}
NA_IDEF NABool naIsRangefNegative(NARangef range){
  return naIsLengthValueNegativef(range.length);
}
NA_IDEF NABool naIsRangeiNegative(NARangei range){
  return naIsLengthValueNegativei(range.length);
}



NA_IDEF NABool naIsSizeNegative(NASize size){
  return (naIsLengthValueNegative(size.width) || naIsLengthValueNegative(size.height));
}
NA_IDEF NABool naIsSizeiNegative(NASizei size){
  return (naIsLengthValueNegativei(size.width) || naIsLengthValueNegativei(size.height));
}
NA_IDEF NABool naIsRectNegative(NARect rect){
  return naIsSizeNegative(rect.size);
}
NA_IDEF NABool naIsRectiNegative(NARecti rect){
  return naIsSizeiNegative(rect.size);
}



NA_IDEF NABool naIsVolumeNegative(NAVolume volume){
  return (naIsLengthValueNegative(volume.width) || naIsLengthValueNegative(volume.height) || naIsLengthValueNegative(volume.depth));
}
NA_IDEF NABool naIsVolumeiNegative(NAVolumei volume){
  return (naIsLengthValueNegativei(volume.width) || naIsLengthValueNegativei(volume.height) || naIsLengthValueNegativei(volume.depth));
}
NA_IDEF NABool naIsBoxNegative(NABox box){
  return naIsVolumeNegative(box.volume);
}
NA_IDEF NABool naIsBoxiNegative(NABoxi box){
  return naIsVolumeiNegative(box.volume);
}



NA_IDEF NABool naIsRangeUseful(NARange range){
  return (naIsOffsetValueUseful(range.origin) && naIsLengthValueUseful(range.length));
}
NA_IDEF NABool naIsRangefUseful(NARangef range){
  return (naIsOffsetValueUsefulf(range.origin) && naIsLengthValueUsefulf(range.length));
}
NA_IDEF NABool naIsRangeiUseful(NARangei range){
  return (naIsOffsetValueUsefuli(range.origin) && naIsLengthValueUsefuli(range.length));
}



NA_IDEF NABool naIsPosUseful(NAPos pos){
  return naIsOffsetValueUseful(pos.x) && naIsOffsetValueUseful(pos.y);
}
NA_IDEF NABool naIsPosiUseful(NAPosi pos){
  return naIsOffsetValueUsefuli(pos.x) && naIsOffsetValueUsefuli(pos.y);
}
NA_IDEF NABool naIsSizeUseful(NASize size){
  return naIsLengthValueUseful(size.width) && naIsLengthValueUseful(size.height);
}
NA_IDEF NABool naIsSizeiUseful(NASizei size){
  return naIsLengthValueUsefuli(size.width) && naIsLengthValueUsefuli(size.height);
}
NA_IDEF NABool naIsRectUseful(NARect rect){
  return (naIsPosUseful(rect.pos) && naIsSizeUseful(rect.size));
}
NA_IDEF NABool naIsRectiUseful(NARecti rect){
  return (naIsPosiUseful(rect.pos) && naIsSizeiUseful(rect.size));
}


NA_IDEF NABool naIsVertexUseful(NAVertex vertex){
  return naIsOffsetValueUseful(vertex.x) && naIsOffsetValueUseful(vertex.y) && naIsOffsetValueUseful(vertex.x);
}
NA_IDEF NABool naIsVertexiUseful(NAVertexi vertex){
  return naIsOffsetValueUsefuli(vertex.x) && naIsOffsetValueUsefuli(vertex.y) && naIsOffsetValueUsefuli(vertex.x);
}
NA_IDEF NABool naIsVolumeUseful(NAVolume volume){
  return naIsLengthValueUseful(volume.width) && naIsLengthValueUseful(volume.height) && naIsLengthValueUseful(volume.depth);
}
NA_IDEF NABool naIsVolumeiUseful(NAVolumei volume){
  return naIsLengthValueUsefuli(volume.width) && naIsLengthValueUsefuli(volume.height) && naIsLengthValueUsefuli(volume.depth);
}
NA_IDEF NABool naIsBoxUseful(NABox box){
  return (naIsVertexUseful(box.vertex) && naIsVolumeUseful(box.volume));
}
NA_IDEF NABool naIsBoxiUseful(NABoxi box){
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
