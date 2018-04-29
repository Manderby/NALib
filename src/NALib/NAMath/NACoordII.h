
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"




// /////////////////////////////////////
// Warning: This is one huge file!
//
// Coordinate functions are not especially difficult to understand or implement
// but take a lot of code writing. You are free to explore all the inline code
// underneath the API but there really isn't much to see.
// /////////////////////////////////////



#include "../NAMathOperators.h"
#include "../NACore/NAValueHelper.h"


NA_IDEF NARange naMakeRange(double origin, double length){
  NARange newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(origin) && naIsLengthValueValid(length)))
      naError("naMakeRange", "Invalid values given.");
    if(!naIsLengthValueUseful(length))
      naError("naMakeRange", "length is not useful.");
  #endif
  newrange.origin = origin;
  newrange.length = length;
  return newrange;
}
NA_IDEF NARangef naMakeRangef(float origin, float length){
  NARangef newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidf(origin) && naIsLengthValueValidf(length)))
      naError("naMakeRangef", "Invalid values given.");
    if(!naIsLengthValueUsefulf(length))
      naError("naMakeRangef", "length is not useful.");
  #endif
  newrange.origin = origin;
  newrange.length = length;
  return newrange;
}
NA_IDEF NARangei naMakeRangei(NAInt origin, NAInt length){
  NARangei newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidi(origin) && naIsLengthValueValidi(length)))
      naError("naMakeRangei", "Invalid values given.");
    if(!naIsLengthValueUsefuli(length))
      naError("naMakeRangei", "length is not useful.");
  #endif
  newrange.origin = origin;
  newrange.length = length;
  return newrange;
}
NA_IDEF NARange naMakeRangeE (double origin, double length){
  NARange newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(origin) && naIsLengthValueValid(length)))
      naError("naMakeRange", "Invalid values given.");
    if(!naIsLengthValueValid(length))
      naError("naMakeRange", "length is not valid.");
  #endif
  newrange.origin = origin;
  newrange.length = length;
  return newrange;
}
NA_IDEF NARangei naMakeRangeiE(NAInt origin, NAInt length){
  NARangei newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidi(origin) && naIsLengthValueValidi(length)))
      naError("naMakeRangeiE", "Invalid values given.");
    if(!naIsLengthValueValidi(length))
      naError("naMakeRangeiE", "length is not valid.");
  #endif
  newrange.origin = origin;
  newrange.length = length;
  return newrange;
}
NA_IDEF NARange naMakeRangeWithRangei(NARangei range){
  NARange newrange = naMakeRangeE((double)range.origin, (double)range.length);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithRange(NARange range){
  NARangei newrange = naMakeRangeiE((NAInt)range.origin, (NAInt)range.length);
  return newrange;
}
NA_IDEF NARange naMakeRangeEmpty(){
  NARange newrange;
  newrange.length = 0.;
  return newrange;
}
NA_IDEF NARangef naMakeRangefEmpty(){
  NARangef newrange;
  newrange.length = 0.f;
  return newrange;
}
NA_IDEF NARangei naMakeRangeiEmpty(){
  NARangei newrange;
  newrange.length = 0;
  return newrange;
}



NA_IDEF NAPos naMakePos(double x, double y){
  NAPos newpos;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(x) && naIsOffsetValueValid(y)))
      naError("naMakePos", "Invalid values given.");
    if(!(naIsOffsetValueUseful(x) && naIsOffsetValueUseful(y)))
      naError("naMakePos", "Values given are not useful.");
  #endif
  newpos.x = x;
  newpos.y = y;
  return newpos;
}
NA_IDEF NAPosi naMakePosi(NAInt x, NAInt y){
  NAPosi newpos;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidi(x) && naIsOffsetValueValidi(y)))
      naError("naMakePosi", "Invalid values given.");
    if(!(naIsOffsetValueUsefuli(x) && naIsOffsetValueUsefuli(y)))
      naError("naMakePosi", "Values given are not useful.");
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
  NAPosi newpos = naMakePosi(naMakeIntWithIntegerDouble(pos.x), naMakeIntWithIntegerDouble(pos.y));
  return newpos;
}



NA_IDEF NASize naMakeSize(double width, double height){
  NASize newsize;
  #ifndef NDEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height)))
      naError("naMakeSize", "Invalid values given.");
    if(!(naIsLengthValueUseful(width) && naIsLengthValueUseful(height)))
      naError("naMakeSize", "Values given are not useful.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASizei naMakeSizei(NAInt width, NAInt height){
  NASizei newsize;
  #ifndef NDEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height)))
      naError("naMakeSizei", "Invalid values given.");
    if(!(naIsLengthValueUsefuli(width) && naIsLengthValueUsefuli(height)))
      naError("naMakeSizei", "Values given are not useful.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASize naMakeSizeE(double width, double height){
  NASize newsize;
  #ifndef NDEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height)))
      naError("naMakeSize", "Invalid values given.");
  #endif
  newsize.width = width;
  newsize.height = height;
  return newsize;
}
NA_IDEF NASizei naMakeSizeiE(NAInt width, NAInt height){
  NASizei newsize;
  #ifndef NDEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height)))
      naError("naMakeSizei", "Invalid values given.");
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
  NASizei newsize = naMakeSizeiE(naMakeIntWithIntegerDouble(size.width), naMakeIntWithIntegerDouble(size.height));
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
  NARect newrect;
  #ifndef NDEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("naMakeRect", "Invalid values given.");
    if(!naIsPosUseful(pos) || !naIsSizeUseful(size))
      naError("naMakeRect", "Values given are not useful.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}
NA_IDEF NARecti naMakeRecti(NAPosi pos, NASizei size){
  NARecti newrect;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos) || !naIsSizeiValid(size))
      naError("naMakeRecti", "Invalid values given.");
    if(!naIsPosiUseful(pos) || !naIsSizeiUseful(size))
      naError("naMakeRecti", "Values given are not useful.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}
NA_IDEF NARect naMakeRectE(NAPos pos, NASize size){
  NARect newrect;
  #ifndef NDEBUG
    if(!naIsPosValid(pos) || !naIsSizeValid(size))
      naError("naMakeRect", "Invalid values given.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}
NA_IDEF NARecti naMakeRectiE(NAPosi pos, NASizei size){
  NARecti newrect;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos) || !naIsSizeiValid(size))
      naError("naMakeRecti", "Invalid values given.");
  #endif
  newrect.pos = pos;
  newrect.size = size;
  return newrect;
}
NA_IDEF NARect naMakeRectS(double x, double y, double width, double height){
  NARect newrect;
  #ifndef NDEBUG
    if(!naIsOffsetValueValid(x) || !naIsOffsetValueValid(y) || !naIsLengthValueValid(width) || !naIsLengthValueValid(height))
      naError("naMakeRectS", "Invalid values given.");
    if(!naIsOffsetValueUseful(x) || !naIsOffsetValueUseful(y) || !naIsLengthValueUseful(width) || !naIsLengthValueUseful(height))
      naError("naMakeRectS", "Values given are not useful.");
  #endif
  newrect.pos = naMakePos(x, y);
  newrect.size = naMakeSize(width, height);
  return newrect;
}
NA_IDEF NARecti naMakeRectiS(NAInt x, NAInt y, NAInt width, NAInt height){
  NARecti newrect;
  #ifndef NDEBUG
    if(!naIsOffsetValueValidi(x) || !naIsOffsetValueValidi(y) || !naIsLengthValueValidi(width) || !naIsLengthValueValidi(height))
      naError("naMakeRectiS", "Invalid values given.");
    if(!naIsOffsetValueUsefuli(x) || !naIsOffsetValueUsefuli(y) || !naIsLengthValueUsefuli(width) || !naIsLengthValueUsefuli(height))
      naError("naMakeRectiS", "Values given are not useful.");
  #endif
  newrect.pos = naMakePosi(x, y);
  newrect.size = naMakeSizei(width, height);
  return newrect;
}
NA_IDEF NARect naMakeRectSE(double x, double y, double width, double height){
  NARect newrect;
  #ifndef NDEBUG
    if(!naIsOffsetValueValid(x) || !naIsOffsetValueValid(y) || !naIsLengthValueValid(width) || !naIsLengthValueValid(height))
      naError("naMakeRectSE", "Invalid values given.");
  #endif
  newrect.pos = naMakePos(x, y);
  newrect.size = naMakeSizeE(width, height);
  return newrect;
}
NA_IDEF NARecti naMakeRectiSE(NAInt x, NAInt y, NAInt width, NAInt height){
  NARecti newrect;
  #ifndef NDEBUG
    if(!naIsOffsetValueValidi(x) || !naIsOffsetValueValidi(y) || !naIsLengthValueValidi(width) || !naIsLengthValueValidi(height))
      naError("naMakeRectiSE", "Invalid values given.");
  #endif
  newrect.pos = naMakePosi(x, y);
  newrect.size = naMakeSizeiE(width, height);
  return newrect;
}
NA_IDEF NARect naMakeRectWithRecti(NARecti rect){
  NARect newrect = naMakeRectSE((double)rect.pos.x, (double)rect.pos.y, (double)rect.size.width, (double)rect.size.height);
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithRect(NARect rect){
  NARecti newrect = naMakeRectiSE((NAInt)rect.pos.x, (NAInt)rect.pos.y, (NAInt)rect.size.width, (NAInt)rect.size.height);
  return newrect;
}
NA_IDEF NARect naMakeRectEmpty(){
  NARect newrect;
  newrect.size.width = 0.;
  return newrect;
}
NA_IDEF NARecti naMakeRectiEmpty(){
  NARecti newrect;
  newrect.size.width = 0;
  return newrect;
}



NA_IDEF NABounds4 naMakeBounds4(double top, double right, double bottom, double left){
  NABounds4 newbounds;
  #ifndef NDEBUG
    if(!naIsLengthValueValid(top) || !naIsLengthValueValid(right) || !naIsLengthValueValid(bottom) || !naIsLengthValueValid(left))
      naError("naMakeBounds4", "Invalid values given.");
  #endif
  newbounds.top = top;
  newbounds.right = right;
  newbounds.bottom = bottom;
  newbounds.left = left;
  return newbounds;
}
NA_IDEF NABounds4i naMakeBounds4i(NAInt top, NAInt right, NAInt bottom, NAInt left){
  NABounds4i newbounds;
  #ifndef NDEBUG
    if(!naIsLengthValueValidi(top) || !naIsLengthValueValidi(right) || !naIsLengthValueValidi(bottom) || !naIsLengthValueValidi(left))
      naError("naMakeBounds4i", "Invalid values given.");
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
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(x) && naIsOffsetValueValid(y) && naIsOffsetValueValid(z)))
      naError("naMakeVertex", "Invalid values given.");
    if(!(naIsOffsetValueUseful(x) && naIsOffsetValueUseful(y) && naIsOffsetValueUseful(z)))
      naError("naMakeVertex", "Values given are not useful.");
  #endif
  newvertex.x = x;
  newvertex.y = y;
  newvertex.z = z;
  return newvertex;
}
NA_IDEF NAVertexi naMakeVertexi(NAInt x, NAInt y, NAInt z){
  NAVertexi newvertex;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidi(x) && naIsOffsetValueValidi(y) && naIsOffsetValueValidi(z)))
      naError("naMakeVertexi", "Invalid values given.");
    if(!(naIsOffsetValueUsefuli(x) && naIsOffsetValueUsefuli(y) && naIsOffsetValueUsefuli(z)))
      naError("naMakeVertexi", "Values given are not useful.");
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



NA_IDEF NAVolume naMakeVolume(double width, double height, double depth){
  NAVolume newvolume;
  #ifndef NDEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("naMakeVolume", "Invalid values given.");
    if(!(naIsLengthValueUseful(width) && naIsLengthValueUseful(height) && naIsLengthValueUseful(depth)))
      naError("naMakeVolume", "Values given are not useful.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolumei naMakeVolumei(NAInt width, NAInt height, NAInt depth){
  NAVolumei newvolume;
  #ifndef NDEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height) && naIsLengthValueValidi(depth)))
      naError("naMakeVolumei", "Invalid values given.");
    if(!(naIsLengthValueUsefuli(width) && naIsLengthValueUsefuli(height) && naIsLengthValueUsefuli(depth)))
      naError("naMakeVolumei", "Values given are not useful.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolume naMakeVolumeE(double width, double height, double depth){
  NAVolume newvolume;
  #ifndef NDEBUG
    if(!(naIsLengthValueValid(width) && naIsLengthValueValid(height) && naIsLengthValueValid(depth)))
      naError("naMakeVolume", "Invalid values given.");
  #endif
  newvolume.width = width;
  newvolume.height = height;
  newvolume.depth = depth;
  return newvolume;
}
NA_IDEF NAVolumei naMakeVolumeiE(NAInt width, NAInt height, NAInt depth){
  NAVolumei newvolume;
  #ifndef NDEBUG
    if(!(naIsLengthValueValidi(width) && naIsLengthValueValidi(height) && naIsLengthValueValidi(depth)))
      naError("naMakeVolumei", "Invalid values given.");
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
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("naMakeBox", "Invalid values given.");
    if(!naIsVertexUseful(vertex) || !naIsVolumeUseful(volume))
      naError("naMakeBox", "Values given are not useful.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxi(NAVertexi vertex, NAVolumei volume){
  NABoxi newbox;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("naMakeBoxi", "Invalid values given.");
    if(!naIsVertexiUseful(vertex) || !naIsVolumeiUseful(volume))
      naError("naMakeBoxi", "Values given are not useful.");
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
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex) || !naIsVolumeValid(volume))
      naError("naMakeBox", "Invalid values given.");
  #endif
  newbox.vertex = vertex;
  newbox.volume = volume;
  return newbox;
}
NA_IDEF NABoxi naMakeBoxiE(NAVertexi vertex, NAVolumei volume){
  NABoxi newbox;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex) || !naIsVolumeiValid(volume))
      naError("naMakeBoxi", "Invalid values given.");
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
NA_IDEF NAPos naMakePosWithCGPoint(CGPoint cgpoint){
  NAPos newpos;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(cgpoint.x) && naIsOffsetValueValid(cgpoint.y)))
      naError("naMakePosWithCGPoint", "Invalid values given.");
    if(!(naIsOffsetValueUseful(cgpoint.x) && naIsOffsetValueUseful(cgpoint.y)))
      naError("naMakePosWithCGPoint", "Values given are not useful.");
  #endif
  newpos.x = cgpoint.x;
  newpos.y = cgpoint.y;
  return newpos;
}
NA_IDEF NASize naMakeSizeWithCGSize(CGSize cgsize){
  NASize newsize;
  #ifndef NDEBUG
    if(!(naIsLengthValueValid(cgsize.width) && naIsLengthValueValid(cgsize.height)))
      naError("naMakeSizeWithCGSize", "Invalid values given.");
    if(!(naIsLengthValueUseful(cgsize.width) && naIsLengthValueUseful(cgsize.height)))
      naError("naMakeSizeWithCGSize", "Values given are not useful.");
  #endif
  newsize.width = cgsize.width;
  newsize.height = cgsize.height;
  return newsize;
}
NA_IDEF NARect naMakeRectWithCGRect(CGRect cgrect){
  NARect newrect;
  newrect.pos = naMakePosWithCGPoint(cgrect.origin);
  newrect.size = naMakeSizeWithCGSize(cgrect.size);
  return newrect;
}
#endif



#if defined __OBJC__ && defined __AVAILABILITYMACROS__
NA_IDEF NAPos naMakePosWithNSPoint(NSPoint nspoint){
  NAPos newpos;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(nspoint.x) && naIsOffsetValueValid(nspoint.y)))
      naError("naMakePosWithNSPoint", "Invalid values given.");
    if(!(naIsOffsetValueUseful(nspoint.x) && naIsOffsetValueUseful(nspoint.y)))
      naError("naMakePosWithNSPoint", "Values given are not useful.");
  #endif
  newpos.x = nspoint.x;
  newpos.y = nspoint.y;
  return newpos;
}
NA_IDEF NSPoint naMakeNSPoitWithPos(NAPos napos){
  NSPoint newpoint;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(napos.x) && naIsOffsetValueValid(napos.y)))
      naError("naMakeNSPoitWithPos", "Invalid values given.");
    if(!(naIsOffsetValueUseful(napos.x) && naIsOffsetValueUseful(napos.y)))
      naError("naMakeNSPoitWithPos", "Values given are not useful.");
  #endif
  newpoint.x = napos.x;
  newpoint.y = napos.y;
  return newpoint;
}
NA_IDEF NASize naMakeSizeWithNSSize(NSSize nssize){
  NASize newsize;
  #ifndef NDEBUG
    if(!(naIsLengthValueValid(nssize.width) && naIsLengthValueValid(nssize.height)))
      naError("naMakeSizeWithNSSize", "Invalid values given.");
    if(!(naIsLengthValueUseful(nssize.width) && naIsLengthValueUseful(nssize.height)))
      naError("naMakeSizeWithNSSize", "Values given are not useful.");
  #endif
  newsize.width = nssize.width;
  newsize.height = nssize.height;
  return newsize;
}
NA_IDEF NSSize naMakeNSSizeWithSize(NASize nasize){
  NSSize newsize;
  #ifndef NDEBUG
    if(!(naIsLengthValueValid(nasize.width) && naIsLengthValueValid(nasize.height)))
      naError("naMakeNSSizeWithSize", "Invalid values given.");
    if(!(naIsLengthValueUseful(nasize.width) && naIsLengthValueUseful(nasize.height)))
      naError("naMakeNSSizeWithSize", "Values given are not useful.");
  #endif
  newsize.width = nasize.width;
  newsize.height = nasize.height;
  return newsize;
}
NA_IDEF NARect naMakeRectWithNSRect(NSRect nsrect){
  NARect newrect;
  newrect.pos = naMakePosWithNSPoint(nsrect.origin);
  newrect.size = naMakeSizeWithNSSize(nsrect.size);
  return newrect;
}
NA_IDEF NSRect naMakeNSRectWithRect(NARect narect){
  NSRect newrect;
  newrect = NSMakeRect((CGFloat)narect.pos.x, (CGFloat)narect.pos.y, (CGFloat)narect.size.width, (CGFloat)narect.size.height);
  return newrect;
}
#endif



NA_IDEF NAPosi naMakePosiWithAlignment(NAPosi origin, NARecti alignrect){
  NAPosi neworigin;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(alignrect)){
      naCrash("naMakePosiWithAlignment", "alignment rect is empty.");
      return naMakePosi(0, 0);
    }
  #endif
  neworigin.x = naAlignValue(origin.x, alignrect.pos.x, alignrect.size.width);
  neworigin.y = naAlignValue(origin.y, alignrect.pos.y, alignrect.size.height);
  return neworigin;
}
NA_IDEF NAVertexi naMakeVertexiWithAlignment(NAVertexi origin, NABoxi alignbox){
  NAVertexi neworigin;
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(alignbox)){
      naCrash("naMakeVertexiWithAlignment", "alignment box is empty.");
      return naMakeVertexi(0, 0, 0);
    }
  #endif
  neworigin.x = naAlignValue(origin.x, alignbox.vertex.x, alignbox.volume.width);
  neworigin.y = naAlignValue(origin.y, alignbox.vertex.y, alignbox.volume.height);
  neworigin.z = naAlignValue(origin.z, alignbox.vertex.z, alignbox.volume.depth);
  return neworigin;
}



NA_IDEF NARange naMakeRangeWithStartAndEnd(double start, double end){
  NARange newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValid(start) && naIsOffsetValueValid(end)))
      naError("naMakeRangeWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.origin = start;
  newrange.length = naMakeLengthWithStartAndEnd(start, end);
  return newrange;
}
NA_IDEF NARangef naMakeRangefWithStartAndEnd(float start, float end){
  NARangef newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidf(start) && naIsOffsetValueValidf(end)))
      naError("naMakeRangefWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.origin = start;
  newrange.length = naMakeLengthWithStartAndEndf(start, end);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithStartAndEnd(NAInt start, NAInt end){
  NARangei newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidi(start) && naIsOffsetValueValidi(end)))
      naError("naMakeRangeiWithStartAndEnd", "Invalid values given.");
  #endif
  newrange.origin = start;
  newrange.length = naMakeLengthWithStartAndEndi(start, end);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithMinAndMax(NAInt min, NAInt max){
  NARangei newrange;
  #ifndef NDEBUG
    if(!(naIsOffsetValueValidi(min) && naIsOffsetValueValidi(max)))
      naError("naMakeRangeiWithMinAndMax", "Invalid values given.");
    if(max < min)
      naError("naMakeRangeiWithMinAndMax", "min should be smallerequal to max.");
  #endif
  newrange.origin = min;
  newrange.length = naMakeLengthWithMinAndMaxi(min, max);
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithRangeAndOffset(NARangei range, NAInt offset){
  NARangei newrange;
  #ifndef NDEBUG
    if(!naIsOffsetValueValidi(offset))
      naError("naMakeRangeiWithRangeAndOffset", "Invalid offset given.");
  #endif
  if(naIsRangeiEmpty(range)){
    newrange.length = 1;
    newrange.origin = offset;
  }else if(offset < range.origin){
    newrange.length = naMakeLengthWithStartAndEndi(offset, naGetRangeiEnd(range));
    newrange.origin = offset;
  }else if(offset > naGetRangeiMax(range)){
    newrange.length = naMakeLengthWithMinAndMaxi(range.origin, offset);
    newrange.origin = range.origin;
  }else{
    newrange = range;
  }
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithRangeAndRange (NARangei range1, NARangei range2){
  NARangei newrange;
  #ifndef NDEBUG
    if(!naIsRangeiValid(range1))
      naError("naMakeRangeiWithRangeAndRange", "Invalid range 1 given.");
    if(!naIsRangeiValid(range2))
      naError("naMakeRangeiWithRangeAndRange", "Invalid range 2 given.");
  #endif
  if(naIsRangeiEmpty(range1) && naIsRangeiEmpty(range2)){
    newrange = naMakeRangeiE(0, 0);
  }else if(naIsRangeiEmpty(range1)){
    newrange = range2;
  }else if(naIsRangeiEmpty(range2)){
    newrange = range1;
  }else{
    newrange = naMakeRangeiWithStartAndEnd(naMini(range1.origin, range2.origin), naMaxi(naGetRangeiEnd(range1), naGetRangeiEnd(range2)));
  }
  return newrange;
}



NA_IDEF NARect naMakeRectWithPosAndPos(NAPos pos1, NAPos pos2){
  NARect newrect;
  #ifndef NDEBUG
    if(!naIsPosValid(pos1))
      naError("naMakeRectWithPosAndPos", "pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("naMakeRectWithPosAndPos", "pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x){
    newrect.pos.x = pos1.x;
    newrect.size.width = naMakeLengthWithStartAndEnd(pos1.x, pos2.x);
  }else{
    newrect.pos.x = pos2.x;
    newrect.size.width = naMakeLengthWithStartAndEnd(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newrect.pos.y = pos1.y;
    newrect.size.height = naMakeLengthWithStartAndEnd(pos1.y, pos2.y);
  }else{
    newrect.pos.y = pos2.y;
    newrect.size.height = naMakeLengthWithStartAndEnd(pos2.y, pos1.y);
  }
  return newrect;
}
NA_IDEF NARect naMakeRectWithRectAndPos(NARect rect, NAPos pos){
  NARect newrect;
  double end;
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naMakeRectWithRectAndPos", "rect is empty.");
    if(!naIsRectUseful(rect))
      naError("naMakeRectWithRectAndPos", "rect is not useful.");
    if(!naIsPosValid(pos))
      naError("naMakeRectWithRectAndPos", "pos is invalid.");
  #endif
  newrect.pos.x = naMin(rect.pos.x, pos.x);
  newrect.pos.y = naMin(rect.pos.y, pos.y);
  end = naGetRectEndX(rect);
  newrect.size.width  = naMakeLengthWithStartAndEnd(newrect.pos.x, naMax(end, pos.x));
  end = naGetRectEndY(rect);
  newrect.size.height = naMakeLengthWithStartAndEnd(newrect.pos.y, naMax(end, pos.y));
  return newrect;
}
NA_IDEF NARect naMakeRectWithRectAndPosE(NARect rect, NAPos pos){
  NARect newrect;
  double end;
  #ifndef NDEBUG
    if(!naIsRectEmpty(rect) && !naIsRectValid(rect))
      naError("naMakeRectiWithRectAndPos", "rect is invalid.");
    if(!naIsPosValid(pos))
      naError("naMakeRectiWithRectAndPos", "pos is invalid.");
  #endif
  if(naIsRectEmpty(rect)){
    newrect.pos = pos;
    newrect.size = naMakeSize(1., 1.);
  }else{
    newrect.pos.x = naMin(rect.pos.x, pos.x);
    newrect.pos.y = naMin(rect.pos.y, pos.y);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    end = naGetRectEndX(rect);
    newrect.size.width  = naMakeLengthWithStartAndEnd(newrect.pos.x, naMax(end, pos.x));
    end = naGetRectEndY(rect);
    newrect.size.height = naMakeLengthWithStartAndEnd(newrect.pos.y, naMax(end, pos.y));
  }
  return newrect;
}
NA_IDEF NARect naMakeRectWithRectAndRect(NARect rect1, NARect rect2){
  NARect newrect;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(naIsRectEmpty(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is empty.");
    if(!naIsRectValid(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is invalid.");
    if(naIsRectEmpty(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is empty.");
    if(!naIsRectValid(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMin(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMin(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newrect.size.width  = naMakeLengthWithStartAndEnd(newrect.pos.x, naMax(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naMakeLengthWithStartAndEnd(newrect.pos.y, naMax(end1, end2));
  return newrect;
}
NA_IDEF NARect naMakeRectWithRectAndRectE(NARect rect1, NARect rect2){
  NARect newrect;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(!naIsRectEmpty(rect1) && !naIsRectValid(rect1))
      naError("naMakeRectWithRectAndRect", "rect1 is invalid.");
    if(!naIsRectEmpty(rect2) && !naIsRectValid(rect2))
      naError("naMakeRectWithRectAndRect", "rect2 is invalid.");
  #endif
  if(naIsRectEmpty(rect1)){return rect2;}
  if(naIsRectEmpty(rect2)){return rect1;}
  newrect.pos.x = naMin(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMin(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newrect.size.width  = naMakeLengthWithStartAndEnd(newrect.pos.x, naMax(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naMakeLengthWithStartAndEnd(newrect.pos.y, naMax(end1, end2));
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithPosAndPos(NAPosi pos1, NAPosi pos2){
  NARecti newrect;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos1))
      naError("naMakeRectiWithPosAndPos", "pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("naMakeRectiWithPosAndPos", "pos2 is invalid.");
  #endif
  if(pos2.x > pos1.x){
    newrect.pos.x = pos1.x;
    newrect.size.width = naMakeLengthWithMinAndMaxi(pos1.x, pos2.x);
  }else{
    newrect.pos.x = pos2.x;
    newrect.size.width = naMakeLengthWithMinAndMaxi(pos2.x, pos1.x);
  }
  if(pos2.y > pos1.y){
    newrect.pos.y = pos1.y;
    newrect.size.height = naMakeLengthWithMinAndMaxi(pos1.y, pos2.y);
  }else{
    newrect.pos.y = pos2.y;
    newrect.size.height = naMakeLengthWithMinAndMaxi(pos2.y, pos1.y);
  }
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithRectAndPos(NARecti rect, NAPosi pos){
  NARecti newrect;
  NAInt max;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naMakeRectiWithRectAndPos", "rect is empty.");
    if(!naIsRectiUseful(rect))
      naError("naMakeRectiWithRectAndPos", "rect is not useful.");
    if(!naIsPosiValid(pos))
      naError("naMakeRectiWithRectAndPos", "pos is invalid.");
  #endif
  newrect.pos.x = naMini(rect.pos.x, pos.x);
  newrect.pos.y = naMini(rect.pos.y, pos.y);
  // Note: We do compute the end instead of the max, because this may require
  // one less instruction. Save whatever you can!
  max = naGetRectiMaxX(rect);
  newrect.size.width  = naMakeLengthWithMinAndMaxi(newrect.pos.x, naMaxi(max, pos.x));
  max = naGetRectiMaxY(rect);
  newrect.size.height = naMakeLengthWithMinAndMaxi(newrect.pos.y, naMaxi(max, pos.y));
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithRectAndPosE(NARecti rect, NAPosi pos){
  NARecti newrect;
  NAInt max;
  #ifndef NDEBUG
    if(!naIsRectiValid(rect))
      naError("naMakeRectiWithRectAndPos", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naMakeRectiWithRectAndPos", "pos is invalid.");
  #endif
  if(naIsRectiEmpty(rect)){
    newrect.pos = pos;
    newrect.size = naMakeSizei(1, 1);
  }else{
    newrect.pos.x = naMini(rect.pos.x, pos.x);
    newrect.pos.y = naMini(rect.pos.y, pos.y);
    // Note: We do compute the end instead of the max, because this may require
    // one less instruction. Save whatever you can!
    max = naGetRectiMaxX(rect);
    newrect.size.width  = naMakeLengthWithMinAndMaxi(newrect.pos.x, naMaxi(max, pos.x));
    max = naGetRectiMaxY(rect);
    newrect.size.height = naMakeLengthWithMinAndMaxi(newrect.pos.y, naMaxi(max, pos.y));
  }
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithRectAndRect(NARecti rect1, NARecti rect2){
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect1))
      naError("naMakeRectiWithRectAndRect", "rect1 is empty.");
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectAndRect", "rect1 is invalid.");
    if(naIsRectiEmptySlow(rect2))
      naError("naMakeRectiWithRectAndRect", "rect2 is empty.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectAndRect", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naMakeLengthWithStartAndEndi(newrect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height = naMakeLengthWithStartAndEndi(newrect.pos.y, naMaxi(end1, end2));
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithRectAndRectE(NARecti rect1, NARecti rect2){
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectAndRect", "rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectAndRect", "rect2 is invalid.");
  #endif
  if(naIsRectiEmpty(rect1)){return rect2;}
  if(naIsRectiEmpty(rect2)){return rect1;}
  newrect.pos.x = naMini(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMini(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naMakeLengthWithStartAndEndi(newrect.pos.x, naMaxi(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height = naMakeLengthWithStartAndEndi(newrect.pos.y, naMaxi(end1, end2));
  return newrect;
}



NA_IDEF NABox naMakeBoxWithVertexAndVertex(NAVertex vertex1, NAVertex vertex2){
  NABox newbox;
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex1))
      naError("naMakeBoxWithVertexAndVertex", "vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("naMakeBoxWithVertexAndVertex", "vertex2 is invalid.");
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
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naMakeBoxWithBoxAndVertex", "box is empty.");
    if(!naIsBoxUseful(box))
      naError("naMakeBoxWithBoxAndVertex", "box is not useful.");
    if(!naIsVertexValid(vertex))
      naError("naMakeBoxWithBoxAndVertex", "vertex is invalid.");
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
  #ifndef NDEBUG
    if(naIsBoxEmpty(box1))
      naError("naMakeBoxWithBoxAndBox", "box1 is empty.");
    if(!naIsBoxValid(box1))
      naError("naMakeBoxWithBoxAndBox", "box1 is invalid.");
    if(naIsBoxEmpty(box2))
      naError("naMakeBoxWithBoxAndBox", "box2 is empty.");
    if(!naIsBoxValid(box2))
      naError("naMakeBoxWithBoxAndBox", "box2 is invalid.");
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
  #ifndef NDEBUG
    if(!naIsBoxValid(box1))
      naError("naMakeBoxWithBoxAndBox", "box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("naMakeBoxWithBoxAndBox", "box2 is invalid.");
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
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex1))
      naError("naMakeBoxiWithVertexAndVertex", "vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("naMakeBoxiWithVertexAndVertex", "vertex2 is invalid.");
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
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naMakeBoxiWithBoxAndVertex", "box is empty.");
    if(!naIsBoxiUseful(box))
      naError("naMakeBoxiWithBoxAndVertex", "box is not useful.");
    if(!naIsVertexiValid(vertex))
      naError("naMakeBoxiWithBoxAndVertex", "vertex is invalid.");
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
  NAInt max;
  #ifndef NDEBUG
    if(!naIsBoxiValid(box))
      naError("naMakeBoxiWithBoxAndVertex", "box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("naMakeBoxiWithBoxAndVertex", "vertex is invalid.");
  #endif
  if(naIsBoxiEmpty(box)){
    newbox.vertex = vertex;
    newbox.volume = naMakeVolumei(1, 1, 1);
  }else{
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
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box1))
      naError("naMakeBoxiWithBoxAndBox", "box1 is empty.");
    if(!naIsBoxiValid(box1))
      naError("naMakeBoxiWithBoxAndBox", "box1 is invalid.");
    if(naIsBoxiEmptySlow(box2))
      naError("naMakeBoxiWithBoxAndBox", "box2 is empty.");
    if(!naIsBoxiValid(box2))
      naError("naMakeBoxiWithBoxAndBox", "box2 is invalid.");
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
  #ifndef NDEBUG
    if(!naIsBoxiValid(box1))
      naError("naMakeBoxiWithBoxAndBox", "box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("naMakeBoxiWithBoxAndBox", "box2 is invalid.");
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
  NARange newrange;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(!naIsRangeValid(range1))
      naError("naMakeRangeWithRangeIntersection", "range1 is invalid.");
    if(!naIsRangeValid(range2))
      naError("naMakeRangeWithRangeIntersection", "range2 is invalid.");
  #endif
  newrange.origin = naMax(range1.origin, range2.origin);
  end1 = naGetRangeEnd(range1);
  end2 = naGetRangeEnd(range2);
  newrange.length  = naMakeLengthWithStartAndEnd(newrange.origin, naMin(end1, end2));
  return newrange;
}
NA_IDEF NARangei naMakeRangeiWithRangeIntersection(NARangei range1, NARangei range2){
  NARangei newrange;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsRangeiValid(range1))
      naError("naMakeRangeiWithRangeIntersection", "range1 is invalid.");
    if(!naIsRangeiValid(range2))
      naError("naMakeRangeiWithRangeIntersection", "range2 is invalid.");
  #endif
  newrange.origin = naMaxi(range1.origin, range2.origin);
  end1 = naGetRangeiEnd(range1);
  end2 = naGetRangeiEnd(range2);
  newrange.length  = naMakeLengthWithStartAndEndi(newrange.origin, naMini(end1, end2));
  return newrange;
}
NA_IDEF NARect naMakeRectWithRectIntersection(NARect rect1, NARect rect2){
  NARect newrect;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(!naIsRectValid(rect1))
      naError("naMakeRectWithRectIntersection", "rect1 is invalid.");
    if(!naIsRectValid(rect2))
      naError("naMakeRectWithRectIntersection", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMax(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMax(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectEndX(rect1);
  end2 = naGetRectEndX(rect2);
  newrect.size.width  = naMakeLengthWithStartAndEnd(newrect.pos.x, naMin(end1, end2));
  end1 = naGetRectEndY(rect1);
  end2 = naGetRectEndY(rect2);
  newrect.size.height  = naMakeLengthWithStartAndEnd(newrect.pos.y, naMin(end1, end2));
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithRectIntersection(NARecti rect1, NARecti rect2){
  NARecti newrect;
  NAInt end1;
  NAInt end2;
  #ifndef NDEBUG
    if(!naIsRectiValid(rect1))
      naError("naMakeRectiWithRectIntersection", "rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("naMakeRectiWithRectIntersection", "rect2 is invalid.");
  #endif
  newrect.pos.x = naMaxi(rect1.pos.x, rect2.pos.x);
  newrect.pos.y = naMaxi(rect1.pos.y, rect2.pos.y);
  end1 = naGetRectiEndX(rect1);
  end2 = naGetRectiEndX(rect2);
  newrect.size.width  = naMakeLengthWithStartAndEndi(newrect.pos.x, naMini(end1, end2));
  end1 = naGetRectiEndY(rect1);
  end2 = naGetRectiEndY(rect2);
  newrect.size.height  = naMakeLengthWithStartAndEndi(newrect.pos.y, naMini(end1, end2));
  return newrect;
}
NA_IDEF NABox naMakeBoxWithBoxIntersection(NABox box1, NABox box2){
  NABox newbox;
  double end1;
  double end2;
  #ifndef NDEBUG
    if(!naIsBoxValid(box1))
      naError("naMakeBoxWithBoxIntersection", "box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("naMakeBoxWithBoxIntersection", "box2 is invalid.");
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
  #ifndef NDEBUG
    if(!naIsBoxiValid(box1))
      naError("naMakeBoxiWithBoxIntersection", "box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("naMakeBoxiWithBoxIntersection", "box2 is invalid.");
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
  NARect newrect;
  newrect.pos.x = rect.pos.x - border;
  newrect.pos.y = rect.pos.y - border;
  newrect.size.width = rect.size.width + 2 * border;
  newrect.size.height = rect.size.height + 2 * border;
  return newrect;
}
NA_IDEF NARecti naMakeRectiWithRectAndBorder(NARecti rect, NAInt border){
  NARecti newrect;
  newrect.pos.x = rect.pos.x - border;
  newrect.pos.y = rect.pos.y - border;
  newrect.size.width = rect.size.width + 2 * border;
  newrect.size.height = rect.size.height + 2 * border;
  return newrect;
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



NA_IDEF NARect naMakeRectPositive(NARect rect){
  naMakePositive(&(rect.pos.x), &(rect.size.width));
  naMakePositive(&(rect.pos.y), &(rect.size.height));
  return rect;
}
NA_IDEF NARecti naMakeRectiPositive(NARecti rect){
  naMakePositivei(&(rect.pos.x), &(rect.size.width));
  naMakePositivei(&(rect.pos.y), &(rect.size.height));
  return rect;
}



NA_IDEF NABox naMakeBoxPositive(NABox box){
  naMakePositive(&(box.vertex.x), &(box.volume.width));
  naMakePositive(&(box.vertex.y), &(box.volume.height));
  naMakePositive(&(box.vertex.z), &(box.volume.depth));
  return box;
}
NA_IDEF NABoxi naMakeBoxiPositive(NABoxi box){
  naMakePositivei(&(box.vertex.x), &(box.volume.width));
  naMakePositivei(&(box.vertex.y), &(box.volume.height));
  naMakePositivei(&(box.vertex.z), &(box.volume.depth));
  return box;
}



// /////////////////////////////////
// COPY
// /////////////////////////////////

NA_IDEF void naCopyPos(NAPos* posd,  NAPos* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCopyPosi(NAPosi* posd,  NAPosi* poss){
  posd->x = poss->x;
  posd->y = poss->y;
}
NA_IDEF void naCopySize(NASize* sized,  NASize* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCopySizei(NASizei* sized,  NASizei* sizes){
  sized->width  = sizes->width;
  sized->height = sizes->height;
}
NA_IDEF void naCopyRect(NARect* rectd,  NARect* rects){
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}
NA_IDEF void naCopyRecti(NARecti* rectd,  NARecti* rects){
  rectd->pos = rects->pos;
  rectd->size  = rects->size;
}



NA_IDEF void naCopyVertex(NAVertex* vertexd,  NAVertex* vertexs){
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naCopyVertexi(NAVertexi* vertexd,  NAVertexi* vertexs){
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naCopyVolume(NAVolume* volumed,  NAVolume* volumes){
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naCopyVolumei(NAVolumei* volumed,  NAVolumei* volumes){
  volumed->width  = volumes->width;
  volumed->height = volumes->height;
  volumed->depth  = volumes->depth;
}
NA_IDEF void naCopyBox(NABox* boxd,  NABox* boxs){
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
}
NA_IDEF void naCopyBoxi(NABoxi* boxd,  NABoxi* boxs){
  boxd->vertex = boxs->vertex;
  boxd->volume  = boxs->volume;
}



// /////////////////////////////////
// GET
// /////////////////////////////////

NA_IDEF NAPos naGetRectCenter(NARect rect){
  NAPos newpos;
  #ifndef NDEBUG
    if(!naIsRectUseful(rect))
      naError("naGetRectCenter", "rect is not useful");
  #endif
  newpos.x = rect.pos.x + 0.5 * rect.size.width;
  newpos.y = rect.pos.y + 0.5 * rect.size.height;
  return newpos;
}
NA_IDEF NAPosi naGetRectiCenter(NARecti rect){
  NAPosi newpos;
  #ifndef NDEBUG
    if(!naIsRectiUseful(rect))
      naError("naGetRectiCenter", "rect is not useful");
  #endif
  newpos.x = rect.pos.x + rect.size.width / 2;
  newpos.y = rect.pos.y + rect.size.height / 2;
  return newpos;
}
NA_IDEF NAVertex naGetBoxCenter(NABox box){
  NAVertex newvertex;
  #ifndef NDEBUG
    if(!naIsBoxUseful(box))
      naError("naGetBoxCenter", "box is not useful");
  #endif
  newvertex.x = box.vertex.x + 0.5 * box.volume.width;
  newvertex.y = box.vertex.y + 0.5 * box.volume.height;
  newvertex.z = box.vertex.z + 0.5 * box.volume.depth;
  return newvertex;
}
NA_IDEF NAVertexi naGetBoxiCenter(NABoxi box){
  NAVertexi newvertex;
  #ifndef NDEBUG
    if(!naIsBoxiUseful(box))
      naError("naGetBoxiCenter", "box is not useful");
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

NA_IDEF NARangei naClampRangeiToRange(NARangei range, NARangei clamprange){
  NAInt value;
  NARangei newrange;
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naClampRangeiToRange", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naClampRangeiToRange", "range is invalid.");
    if(naIsRangeiEmpty(clamprange))
      naError("naClampRangeiToRange", "clamprange is empty.");
    if(!naIsRangeiValid(clamprange))
      naError("naClampRangeiToRange", "clamprange is invalid.");
  #endif
  newrange = range;

  value = clamprange.origin - newrange.origin;
  if(value > 0){
    newrange.length -= value;
    newrange.origin = clamprange.origin;
  }
  value = naGetRangeiEnd(clamprange);
  if(naGetRangeiEnd(newrange) > value){
    newrange.length = value - newrange.origin;
  }
  
  return newrange;
}
NA_IDEF NARangei naClampRangeiToMin(NARangei range, NAInt min){
  NAInt value;
  NARangei newrange;
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naClampRangeiToMin", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naClampRangeiToMin", "range is invalid.");
    if(!naIsOffsetValueValidi(min))
      naError("naClampRangeiToMin", "min is invalid.");
  #endif
  newrange = range;

  value = min - newrange.origin;
  if(value > 0){
    newrange.length -= value;
    newrange.origin = min;
  }
  return newrange;
}
NA_IDEF NARangei naClampRangeiToEnd(NARangei range, NAInt end){
  NAInt value;
  NARangei newrange;
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naClampRangeiToMin", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naClampRangeiToMin", "range is invalid.");
    if(!naIsOffsetValueValidi(end))
      naError("naClampRangeiToMin", "min is invalid.");
  #endif
  newrange = range;

  value = end;
  if(naGetRangeiEnd(newrange) > value){
    newrange.length = value - newrange.origin;
  }
  
  return newrange;
}



NA_IDEF NAPos naClampPosToRect(NAPos pos, NARect clamprect){
  double end;
  NAPos newpos;
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naClampPosToRect", "pos is invalid.");
    if(naIsRectEmpty(clamprect))
      naError("naClampPosToRect", "clamprect is empty.");
    if(!naIsRectValid(clamprect))
      naError("naClampPosToRect", "clamprect is invalid.");
  #endif
  newpos = pos;
  if(newpos.x < clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  end = naGetRectEndX(clamprect);
  if(newpos.x > end){newpos.x = end;}
  if(newpos.y < clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  end = naGetRectEndY(clamprect);
  if(newpos.y > end){newpos.y = end;}
  return newpos;
}
NA_IDEF NAPosi naClampPosiToRect(NAPosi pos, NARecti clamprect){
  NAInt max;
  NAPosi newpos;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naClampPosiToRect", "pos is invalid.");
    if(naIsRectiEmptySlow(clamprect))
      naError("naClampPosiToRect", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampPosiToRect", "clamprect is invalid.");
  #endif
  newpos = pos;
  if(newpos.x <  clamprect.pos.x) {newpos.x = clamprect.pos.x;}
  max = naGetRectiMaxX(clamprect);
  if(newpos.x > max){newpos.x = max;}
  if(newpos.y <  clamprect.pos.y) {newpos.y = clamprect.pos.y;}
  max = naGetRectiMaxY(clamprect);
  if(newpos.y > max){newpos.y = max;}
  return newpos;
}
NA_IDEF NARect naClampRectToRect(NARect rect, NARect clamprect){
  // Todo: use helper functions in this function.
  double value;
  NARect newrect;
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naClampRectToRect", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naClampRectToRect", "rect is invalid.");
    if(naIsRectEmpty(clamprect))
      naError("naClampRectToRect", "clamprect is empty.");
    if(!naIsRectValid(clamprect))
      naError("naClampRectToRect", "clamprect is invalid.");
  #endif
  newrect = rect;
  // Adjust in first dimension
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectEndX(clamprect);
  if(naGetRectEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
  }
  
  // Adjust in second dimension
  value = clamprect.pos.y - newrect.pos.y;
  if(value > 0){
    newrect.size.height -= value;
    newrect.pos.y = clamprect.pos.y;
  }
  value = naGetRectEndY(clamprect);
  if(naGetRectEndY(newrect) > value){
    newrect.size.height = value - newrect.pos.y;
  }
  return newrect;
}
NA_IDEF NARecti naClampRectiToRect(NARecti rect, NARecti clamprect){
  // Todo: use helper functions in this function.
  NAInt value;
  NARecti newrect;
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naClampRectiToRect", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naClampRectiToRect", "rect is invalid.");
    if(naIsRectiEmptySlow(clamprect))
      naError("naClampRectiToRect", "clamprect is empty.");
    if(!naIsRectiValid(clamprect))
      naError("naClampRectiToRect", "clamprect is invalid.");
  #endif

  newrect = rect;

  // Adjust in first dimension
  value = clamprect.pos.x - newrect.pos.x;
  if(value > 0){
    newrect.size.width -= value;
    if(newrect.size.width <= 0){return newrect;}
    newrect.pos.x = clamprect.pos.x;
  }
  value = naGetRectiEndX(clamprect);
  if(naGetRectiEndX(newrect) > value){
    newrect.size.width = value - newrect.pos.x;
    if(newrect.size.width <= 0){return newrect;}
  }
  
  // Adjust in second dimension
  value = clamprect.pos.y - newrect.pos.y;
  if(value > 0){
    newrect.size.height -= value;
    if(newrect.size.height <= 0){return newrect;}
    newrect.pos.y = clamprect.pos.y;
  }
  value = naGetRectiEndY(clamprect);
  if(naGetRectiEndY(newrect) > value){
    newrect.size.height = value - newrect.pos.y;
    if(newrect.size.height <= 0){return newrect;}
  }
  return newrect;
}



NA_IDEF NAVertex naClampVertexToBox(NAVertex vertex, NABox clampbox){
  double end;
  NAVertex newvertex;
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naClampVertexToBox", "vertex is invalid.");
    if(naIsBoxEmpty(clampbox))
      naError("naClampVertexToBox", "clampbox is empty.");
    if(!naIsBoxValid(clampbox))
      naError("naClampVertexToBox", "clampbox is invalid.");
  #endif
  newvertex = vertex;
  if(newvertex.x < clampbox.vertex.x) {newvertex.x = clampbox.vertex.x;}
  end = naGetBoxEndX(clampbox);
  if(newvertex.x > end){newvertex.x = end;}
  if(newvertex.y < clampbox.vertex.y) {newvertex.y = clampbox.vertex.y;}
  end = naGetBoxEndY(clampbox);
  if(newvertex.y > end){newvertex.y = end;}
  if(newvertex.z < clampbox.vertex.z) {newvertex.z = clampbox.vertex.z;}
  end = naGetBoxEndZ(clampbox);
  if(newvertex.z > end){newvertex.z = end;}
  return newvertex;
}
NA_IDEF NAVertexi naClampVertexiToBox(NAVertexi vertex, NABoxi clampbox){
  NAInt max;
  NAVertexi newvertex;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naClampVertexiToBox", "vertex is invalid.");
    if(naIsBoxiEmptySlow(clampbox))
      naError("naClampVertexiToBox", "clampbox is empty.");
    if(!naIsBoxiValid(clampbox))
      naError("naClampVertexiToBox", "clampbox is invalid.");
  #endif
  newvertex = vertex;
  if(newvertex.x <  clampbox.vertex.x) {newvertex.x = clampbox.vertex.x;}
  max = naGetBoxiMaxX(clampbox);
  if(newvertex.x > max){newvertex.x = max;}
  if(newvertex.y <  clampbox.vertex.y) {newvertex.y = clampbox.vertex.y;}
  max = naGetBoxiMaxY(clampbox);
  if(newvertex.y > max){newvertex.y = max;}
  if(newvertex.z <  clampbox.vertex.z) {newvertex.z = clampbox.vertex.z;}
  max = naGetBoxiMaxZ(clampbox);
  if(newvertex.z > max){newvertex.z = max;}
  return newvertex;
}
NA_IDEF NABox naClampBoxToBox(NABox box, NABox clampbox){
  // Todo: use helper functions in this function.
  double value;
  NABox newbox;
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naClampBoxToBox", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naClampBoxToBox", "box is invalid.");
    if(naIsBoxEmpty(clampbox))
      naError("naClampBoxToBox", "clampbox is empty.");
    if(!naIsBoxValid(clampbox))
      naError("naClampBoxToBox", "clampbox is invalid.");
  #endif
  newbox = box;
  // Adjust in first dimension
  value = clampbox.vertex.x - newbox.vertex.x;
  if(value > 0){
    newbox.volume.width -= value;
    newbox.vertex.x = clampbox.vertex.x;
  }
  value = naGetBoxEndX(clampbox);
  if(naGetBoxEndX(newbox) > value){
    newbox.volume.width = value - newbox.vertex.x;
  }
  
  // Adjust in second dimension
  value = clampbox.vertex.y - newbox.vertex.y;
  if(value > 0){
    newbox.volume.height -= value;
    newbox.vertex.y = clampbox.vertex.y;
  }
  value = naGetBoxEndY(clampbox);
  if(naGetBoxEndY(newbox) > value){
    newbox.volume.height = value - newbox.vertex.y;
  }

  // Adjust in third dimension
  value = clampbox.vertex.z - newbox.vertex.z;
  if(value > 0){
    newbox.volume.depth -= value;
    newbox.vertex.z = clampbox.vertex.z;
  }
  value = naGetBoxEndZ(clampbox);
  if(naGetBoxEndZ(newbox) > value){
    newbox.volume.depth = value - newbox.vertex.z;
  }
  return newbox;
}
NA_IDEF NABoxi naClampBoxiToBox(NABoxi box, NABoxi clampbox){
  // Todo: use helper functions in this function.
  NAInt value;
  NABoxi newbox;
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naClampBoxiToBox", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naClampBoxiToBox", "box is invalid.");
    if(naIsBoxiEmptySlow(clampbox))
      naError("naClampBoxiToBox", "clampbox is empty.");
    if(!naIsBoxiValid(clampbox))
      naError("naClampBoxiToBox", "clampbox is invalid.");
  #endif

  newbox = box;

  // Adjust in first dimension
  value = clampbox.vertex.x - newbox.vertex.x;
  if(value > 0){
    newbox.volume.width -= value;
    if(newbox.volume.width <= 0){return newbox;}
    newbox.vertex.x = clampbox.vertex.x;
  }
  value = naGetBoxiEndX(clampbox);
  if(naGetBoxiEndX(newbox) > value){
    newbox.volume.width = value - newbox.vertex.x;
    if(newbox.volume.width <= 0){return newbox;}
  }
  
  // Adjust in second dimension
  value = clampbox.vertex.y - newbox.vertex.y;
  if(value > 0){
    newbox.volume.height -= value;
    if(newbox.volume.height <= 0){return newbox;}
    newbox.vertex.y = clampbox.vertex.y;
  }
  value = naGetBoxiEndY(clampbox);
  if(naGetBoxiEndY(newbox) > value){
    newbox.volume.height = value - newbox.vertex.y;
    if(newbox.volume.height <= 0){return newbox;}
  }

  // Adjust in third dimension
  value = clampbox.vertex.z - newbox.vertex.z;
  if(value > 0){
    newbox.volume.depth -= value;
    if(newbox.volume.depth <= 0){return newbox;}
    newbox.vertex.z = clampbox.vertex.z;
  }
  value = naGetBoxiEndZ(clampbox);
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
  #ifndef NDEBUG
    if(!naIsRangeiValid(range1))
      naError("naEqualPos", "range1 is invalid.");
    if(!naIsRangeiValid(range2))
      naError("naEqualPos", "range2 is invalid.");
  #endif
  return ((range1.origin == range2.origin) && (range1.length == range2.length));
}



NA_IDEF NABool naEqualPos(NAPos pos1, NAPos pos2){
  #ifndef NDEBUG
    if(!naIsPosValid(pos1))
      naError("naEqualPos", "pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("naEqualPos", "pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualPosi(NAPosi pos1, NAPosi pos2){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos1))
      naError("naEqualPosi", "pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("naEqualPosi", "pos2 is invalid.");
  #endif
  return ((pos1.x == pos2.x) && (pos1.y == pos2.y));
}
NA_IDEF NABool naEqualSize(NASize size1, NASize size2){
  #ifndef NDEBUG
    if(naIsSizeEmpty(size1) || naIsSizeEmpty(size2))
      naError("naEqualSize", "Equality test not valid for empty sizes.");
    if(!naIsSizeValid(size1))
      naError("naEqualSize", "size1 is invalid.");
    if(!naIsSizeValid(size2))
      naError("naEqualSize", "size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IDEF NABool naEqualSizei(NASizei size1, NASizei size2){
  #ifndef NDEBUG
    if(naIsSizeiEmpty(size1) || naIsSizeiEmpty(size2))
      naError("naEqualSizei", "Equality test not valid for empty sizes.");
    if(!naIsSizeiValid(size1))
      naError("naEqualSizei", "size1 is invalid.");
    if(!naIsSizeiValid(size2))
      naError("naEqualSizei", "size2 is invalid.");
  #endif
  return ((size1.width == size2.width) && (size1.height == size2.height));
}
NA_IDEF NABool naEqualRect(NARect rect1, NARect rect2){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect1) || naIsRectEmpty(rect2))
      naError("naEqualRect", "Equality test not valid for empty rects.");
    if(!naIsRectValid(rect1))
      naError("naEqualRect", "rect1 is invalid.");
    if(!naIsRectValid(rect2))
      naError("naEqualRect", "rect2 is invalid.");
  #endif
  return     (naEqualPos (rect1.pos,  rect2.pos)
          &&  naEqualSize(rect1.size, rect2.size));
}
NA_IDEF NABool naEqualRecti(NARecti rect1, NARecti rect2){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect1) || naIsRectiEmpty(rect2))
      naError("naEqualRecti", "Equality test not valid for empty rects.");
    if(!naIsRectiValid(rect1))
      naError("naEqualRecti", "rect1 is invalid.");
    if(!naIsRectiValid(rect2))
      naError("naEqualRecti", "rect2 is invalid.");
  #endif
  return    (naEqualPosi (rect1.pos,  rect2.pos)
          && naEqualSizei(rect1.size, rect2.size));
}



NA_IDEF NABool naEqualVertex(NAVertex vertex1, NAVertex vertex2){
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex1))
      naError("naEqualVertex", "vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("naEqualVertex", "vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVertexi(NAVertexi vertex1, NAVertexi vertex2){
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex1))
      naError("naEqualVertexi", "vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("naEqualVertexi", "vertex2 is invalid.");
  #endif
  return ((vertex1.x == vertex2.x) && (vertex1.y == vertex2.y) && (vertex1.z == vertex2.z));
}
NA_IDEF NABool naEqualVolume(NAVolume volume1, NAVolume volume2){
  #ifndef NDEBUG
    if(naIsVolumeEmpty(volume1) || naIsVolumeEmpty(volume2))
      naError("naEqualVolume", "Equality test not valid for empty volumes.");
    if(!naIsVolumeValid(volume1))
      naError("naEqualVolume", "volume1 is invalid.");
    if(!naIsVolumeValid(volume2))
      naError("naEqualVolume", "volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}
NA_IDEF NABool naEqualVolumei(NAVolumei volume1, NAVolumei volume2){
  #ifndef NDEBUG
    if(naIsVolumeiEmpty(volume1) || naIsVolumeiEmpty(volume2))
      naError("naEqualVolumei", "Equality test not valid for empty volumes.");
    if(!naIsVolumeiValid(volume1))
      naError("naEqualVolumei", "volume1 is invalid.");
    if(!naIsVolumeiValid(volume2))
      naError("naEqualVolumei", "volume2 is invalid.");
  #endif
  return ((volume1.width == volume2.width) && (volume1.height == volume2.height) && (volume1.depth == volume2.depth));
}
NA_IDEF NABool naEqualBox(NABox box1, NABox box2){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box1) || naIsBoxEmpty(box2))
      naError("naEqualBox", "Equality test not valid for empty boxs.");
    if(!naIsBoxValid(box1))
      naError("naEqualBox", "box1 is invalid.");
    if(!naIsBoxValid(box2))
      naError("naEqualBox", "box2 is invalid.");
  #endif
  return     (naEqualVertex (box1.vertex,  box2.vertex)
          &&  naEqualVolume(box1.volume, box2.volume));
}
NA_IDEF NABool naEqualBoxi(NABoxi box1, NABoxi box2){
  #ifndef NDEBUG
    if(naIsBoxiEmpty(box1) || naIsBoxiEmpty(box2))
      naError("naEqualBoxi", "Equality test not valid for empty boxs.");
    if(!naIsBoxiValid(box1))
      naError("naEqualBoxi", "box1 is invalid.");
    if(!naIsBoxiValid(box2))
      naError("naEqualBoxi", "box2 is invalid.");
  #endif
  return    (naEqualVertexi (box1.vertex,  box2.vertex)
          && naEqualVolumei(box1.volume, box2.volume));
}



// ///////////////////////////////////////
// ADD SUB
// ///////////////////////////////////////

NA_IDEF NAPos naAddPosSize(NAPos pos, NASize size){
  NAPos newpos;
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naAddPosSize", "pos is invalid.");
    if(!naIsSizeValid(size))
      naError("naAddPosSize", "size is invalid.");
  #endif
  newpos.x = pos.x + size.width;
  newpos.y = pos.y + size.height;
  return newpos;
}
NA_IDEF NAPosi naAddPosiSize(NAPosi pos, NASizei size){
  NAPosi newpos;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naAddPosiSize", "pos is invalid.");
    if(!naIsSizeiValid(size))
      naError("naAddPosiSize", "size is invalid.");
  #endif
  newpos.x = pos.x + size.width;
  newpos.y = pos.y + size.height;
  return newpos;
}

NA_IDEF NASize naSubPosPos(NAPos pos1, NAPos pos2){
  NASize newsize;
  #ifndef NDEBUG
    if(!naIsPosValid(pos1))
      naError("naSubPosPos", "pos1 is invalid.");
    if(!naIsPosValid(pos2))
      naError("naSubPosPos", "pos2 is invalid.");
  #endif
  newsize.width  = pos1.x - pos2.x;
  newsize.height = pos1.y - pos2.y;
  return newsize;
}
NA_IDEF NASizei naSubPosiPos(NAPosi pos1, NAPosi pos2){
  NASizei newsize;
  #ifndef NDEBUG
    if(!naIsPosiValid(pos1))
      naError("naSubPosiPos", "pos1 is invalid.");
    if(!naIsPosiValid(pos2))
      naError("naSubPosiPos", "pos2 is invalid.");
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
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naAddVertexVolume", "vertex is invalid.");
    if(!naIsVolumeValid(volume))
      naError("naAddVertexVolume", "volume is invalid.");
  #endif
  newvertex.x = vertex.x + volume.width;
  newvertex.y = vertex.y + volume.height;
  newvertex.z = vertex.z + volume.depth;
  return newvertex;
}
NA_IDEF NAVertexi naAddVertexiVolume(NAVertexi vertex, NAVolumei volume){
  NAVertexi newvertex;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naAddVertexiVolume", "vertex is invalid.");
    if(!naIsVolumeiValid(volume))
      naError("naAddVertexiVolume", "volume is invalid.");
  #endif
  newvertex.x = vertex.x + volume.width;
  newvertex.y = vertex.y + volume.height;
  newvertex.z = vertex.z + volume.depth;
  return newvertex;
}

NA_IDEF NAVolume naSubVertexPos(NAVertex vertex1, NAVertex vertex2){
  NAVolume newvolume;
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex1))
      naError("naSubVertexPos", "vertex1 is invalid.");
    if(!naIsVertexValid(vertex2))
      naError("naSubVertexPos", "vertex2 is invalid.");
  #endif
  newvolume.width  = vertex1.x - vertex2.x;
  newvolume.height = vertex1.y - vertex2.y;
  newvolume.depth  = vertex1.z - vertex2.z;
  return newvolume;
}
NA_IDEF NAVolumei naSubVertexiVertex(NAVertexi vertex1, NAVertexi vertex2){
  NAVolumei newvolume;
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex1))
      naError("naSubVertexiVertex", "vertex1 is invalid.");
    if(!naIsVertexiValid(vertex2))
      naError("naSubVertexiVertex", "vertex2 is invalid.");
  #endif
  newvolume.width  = vertex1.x - vertex2.x;
  newvolume.height = vertex1.y - vertex2.y;
  newvolume.depth  = vertex1.z - vertex2.y;
  return newvolume;
}



// ///////////////////////////////////////
// CONTAIN
// ///////////////////////////////////////

NA_IDEF NABool naContainsRangeOffset(NARange outerrange, double offset){
  #ifndef NDEBUG
    if(!naIsOffsetValueValid(offset))
      naError("naContainsRangeOffset", "offset is invalid.");
    if(!naIsRangeUseful(outerrange))
      naError("naContainsRangeOffset", "Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerrange.origin)
        || (offset > naGetRangeEnd(outerrange)));
}
NA_IDEF NABool naContainsRangefOffset(NARangef outerrange, float offset){
  #ifndef NDEBUG
    if(!naIsOffsetValueValidf(offset))
      naError("naContainsRangefOffset", "offset is invalid.");
    if(!naIsRangefUseful(outerrange))
      naError("naContainsRangefOffset", "Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerrange.origin)
        || (offset > naGetRangefEnd(outerrange)));
}
NA_IDEF NABool naContainsRangeiOffset(NARangei outerrange, NAInt offset){
  #ifndef NDEBUG
    if(!naIsOffsetValueValidi(offset))
      naError("naContainsRangeiOffset", "offset is invalid.");
    if(!naIsRangeiUseful(outerrange))
      naError("naContainsRangeiOffset", "Inside test not valid for ranges which are not useful.");
  #endif
  return !((offset < outerrange.origin)
        || (offset > naGetRangeiMax(outerrange)));
}
NA_IDEF NABool naContainsRangeiRange(NARangei outerrange, NARangei range){
  #ifndef NDEBUG
    if(!naIsRangeiUseful(range))
      naError("naContainsRangeiRange", "Inside test not valid if range is not useful.");
    if(!naIsRangeiUseful(outerrange))
      naError("naContainsRangeiRange", "Inside test not valid if outerrange is not useful.");
  #endif
  return  ((range.origin           >= outerrange.origin)
        && (naGetRangeiEnd(range) <=  naGetRangeiEnd(outerrange)));
}




NA_IDEF NABool naContainsSizePos(NASize outersize, NAPos pos){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naContainsSizePos", "pos is invalid.");
    if(!naIsSizeUseful(outersize))
      naError("naContainsSizePos", "Inside test not valid for sizes which are not useful.");
  #endif
  return !((pos.x < 0)
        || (pos.x > outersize.width)
        || (pos.y < 0)
        || (pos.y > outersize.height));
}
NA_IDEF NABool naContainsSizeiPos(NASizei outersize, NAPosi pos){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naContainsSizeiPos", "pos is invalid.");
    if(naIsSizeiUseful(outersize))
      naError("naContainsSizeiPos", "Inside test not valid for sizes which are not useful.");
  #endif
  return  ((pos.x >= 0)
        && (pos.x <  outersize.width)
        && (pos.y >= 0)
        && (pos.y <  outersize.height));
}
NA_IDEF NABool naContainsRectPos(NARect outerrect, NAPos pos){
  #ifndef NDEBUG
    if(!naIsPosValid(pos))
      naError("naContainsRectPos", "pos is invalid.");
    if(naIsRectEmpty(outerrect))
      naError("naContainsRectPos", "Inside test not valid for empty rects.");
    if(!naIsRectValid(outerrect))
      naError("naContainsRectPos", "outerrect is invalid.");
  #endif
  return !((pos.x < outerrect.pos.x)
        || (pos.x > naGetRectEndX(outerrect))
        || (pos.y < outerrect.pos.y)
        || (pos.y > naGetRectEndY(outerrect)));
}
NA_IDEF NABool naContainsRectiPos(NARecti outerrect, NAPosi pos){
  #ifndef NDEBUG
    if(!naIsPosiValid(pos))
      naError("naContainsRectiPos", "pos is invalid.");
    if(naIsRectiEmptySlow(outerrect))
      naError("naContainsRectiPos", "Inside test not valid for empty rects.");
    if(!naIsRectiValid(outerrect))
      naError("naContainsRectiPos", "outerrect is invalid.");
  #endif
  return  ((pos.x >= outerrect.pos.x)
        && (pos.x <  naGetRectiEndX(outerrect))
        && (pos.y >= outerrect.pos.y)
        && (pos.y <  naGetRectiEndY(outerrect)));
}
NA_IDEF NABool naContainsSizeSize(NASize outersize, NASize size){
  #ifndef NDEBUG
    if(!naIsSizeUseful(size))
      naError("naContainsSizeSize", "Inside test not valid if size is not useful.");
    if(!naIsSizeUseful(outersize))
      naError("naContainsSizeSize", "Inside test not valid if outersize is not useful.");
  #endif
  return !((size.width  > outersize.width)
        || (size.height > outersize.height));
}
NA_IDEF NABool naContainsSizeiSize(NASizei outersize, NASizei size){
  #ifndef NDEBUG
    if(!naIsSizeiUseful(size))
      naError("naContainsSizeiSize", "Inside test not valid if size is not useful.");
    if(!naIsSizeiUseful(outersize))
      naError("naContainsSizeiSize", "Inside test not valid if outersize is not useful.");
  #endif
  return !((size.width  > outersize.width)
        || (size.height > outersize.height));
}
NA_IDEF NABool naContainsRectRect(NARect outerrect, NARect rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect) || naIsRectEmpty(outerrect))
      naError("naContainsRectRect", "Inside test not valid for empty rects.");
    if(!naIsRectValid(rect))
      naError("naContainsRectRect", "rect is invalid.");
    if(!naIsRectValid(outerrect))
      naError("naContainsRectRect", "outerrect is invalid.");
  #endif
  return !((rect.pos.x          < outerrect.pos.x)
        || (naGetRectEndX(rect) > naGetRectEndX(outerrect))
        || (rect.pos.y          < outerrect.pos.y)
        || (naGetRectEndY(rect) > naGetRectEndY(outerrect)));
}
NA_IDEF NABool naContainsRectiRect(NARecti outerrect, NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmpty(rect) || naIsRectiEmpty(outerrect))
      naError("naContainsRectiRect", "Inside test not valid for empty rects.");
    if(!naIsRectiValid(rect))
      naError("naContainsRectiRect", "rect is invalid.");
    if(!naIsRectiValid(outerrect))
      naError("naContainsRectiRect", "outerrect is invalid.");
  #endif
  return  ((rect.pos.x           >= outerrect.pos.x)
        && (naGetRectiEndX(rect) <=  naGetRectiEndX(outerrect))
        && (rect.pos.y           >= outerrect.pos.y)
        && (naGetRectiEndY(rect) <=  naGetRectiEndY(outerrect)));
}



NA_IDEF NABool naContainsVolumeVertex(NAVolume outervolume, NAVertex vertex){
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naContainsVolumeVertex", "vertex is invalid.");
    if(!naIsVolumeUseful(outervolume))
      naError("naContainsVolumeVertex", "Inside test not valid for volumes which are not useful.");
  #endif
  return !((vertex.x < 0)
        || (vertex.x > outervolume.width)
        || (vertex.y < 0)
        || (vertex.y > outervolume.height)
        || (vertex.z < 0)
        || (vertex.z > outervolume.depth));
}
NA_IDEF NABool naContainsVolumeiVertex(NAVolumei outervolume, NAVertexi vertex){
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naContainsVolumeiVertex", "vertex is invalid.");
    if(naIsVolumeiUseful(outervolume))
      naError("naContainsVolumeiVertex", "Inside test not valid for volumes which are not useful.");
  #endif
  return  ((vertex.x >= 0)
        && (vertex.x <  outervolume.width)
        && (vertex.y >= 0)
        && (vertex.y <  outervolume.height)
        && (vertex.z >= 0)
        && (vertex.z <  outervolume.depth));
}
NA_IDEF NABool naContainsBoxVertex(NABox outerbox, NAVertex vertex){
  #ifndef NDEBUG
    if(!naIsVertexValid(vertex))
      naError("naContainsBoxVertex", "vertex is invalid.");
    if(naIsBoxEmpty(outerbox))
      naError("naContainsBoxVertex", "Inside test not valid for empty boxs.");
    if(!naIsBoxValid(outerbox))
      naError("naContainsBoxVertex", "outerbox is invalid.");
  #endif
  return !((vertex.x < outerbox.vertex.x)
        || (vertex.x > naGetBoxEndX(outerbox))
        || (vertex.y < outerbox.vertex.y)
        || (vertex.y > naGetBoxEndY(outerbox))
        || (vertex.z < outerbox.vertex.z)
        || (vertex.z > naGetBoxEndZ(outerbox)));
}
NA_IDEF NABool naContainsBoxiVertex(NABoxi outerbox, NAVertexi vertex){
  #ifndef NDEBUG
    if(!naIsVertexiValid(vertex))
      naError("naContainsBoxiVertex", "vertex is invalid.");
    if(naIsBoxiEmptySlow(outerbox))
      naError("naContainsBoxiVertex", "Inside test not valid for empty boxs.");
    if(!naIsBoxiValid(outerbox))
      naError("naContainsBoxiVertex", "outerbox is invalid.");
  #endif
  return  ((vertex.x >= outerbox.vertex.x)
        && (vertex.x <  naGetBoxiEndX(outerbox))
        && (vertex.y >= outerbox.vertex.y)
        && (vertex.y <  naGetBoxiEndY(outerbox))
        && (vertex.z >= outerbox.vertex.z)
        && (vertex.z <  naGetBoxiEndZ(outerbox)));
}
NA_IDEF NABool naContainsVolumeVolume(NAVolume outervolume, NAVolume volume){
  #ifndef NDEBUG
    if(!naIsVolumeUseful(volume))
      naError("naContainsVolumeVolume", "Inside test not valid if volume is not useful.");
    if(!naIsVolumeUseful(outervolume))
      naError("naContainsVolumeVolume", "Inside test not valid if outervolume is not useful.");
  #endif
  return !((volume.width  > outervolume.width)
        || (volume.height > outervolume.height)
        || (volume.depth  > outervolume.depth));
}
NA_IDEF NABool naContainsVolumeiVolume(NAVolumei outervolume, NAVolumei volume){
  #ifndef NDEBUG
    if(!naIsVolumeiUseful(volume))
      naError("naContainsVolumeiVolume", "Inside test not valid if volume is not useful.");
    if(!naIsVolumeiUseful(outervolume))
      naError("naContainsVolumeiVolume", "Inside test not valid if outervolume is not useful.");
  #endif
  return !((volume.width  > outervolume.width)
        || (volume.height > outervolume.height)
        || (volume.depth  > outervolume.depth));
}
NA_IDEF NABool naContainsBoxBox(NABox outerbox, NABox box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box) || naIsBoxEmpty(outerbox))
      naError("naContainsBoxBox", "Inside test not valid for empty boxs.");
    if(!naIsBoxValid(box))
      naError("naContainsBoxBox", "box is invalid.");
    if(!naIsBoxValid(outerbox))
      naError("naContainsBoxBox", "outerbox is invalid.");
  #endif
  return !((box.vertex.x          < outerbox.vertex.x)
        || (naGetBoxEndX(box) > naGetBoxEndX(outerbox))
        || (box.vertex.y          < outerbox.vertex.y)
        || (naGetBoxEndY(box) > naGetBoxEndY(outerbox))
        || (box.vertex.z          < outerbox.vertex.z)
        || (naGetBoxEndZ(box) > naGetBoxEndZ(outerbox)));
}
NA_IDEF NABool naContainsBoxiBox(NABoxi outerbox, NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmpty(box) || naIsBoxiEmpty(outerbox))
      naError("naContainsBoxiBox", "Inside test not valid for empty boxs.");
    if(!naIsBoxiValid(box))
      naError("naContainsBoxiBox", "box is invalid.");
    if(!naIsBoxiValid(outerbox))
      naError("naContainsBoxiBox", "outerbox is invalid.");
  #endif
  return  ((box.vertex.x           >= outerbox.vertex.x)
        && (naGetBoxiEndX(box) <=  naGetBoxiEndX(outerbox))
        && (box.vertex.y           >= outerbox.vertex.y)
        && (naGetBoxiEndY(box) <=  naGetBoxiEndY(outerbox))
        && (box.vertex.z           >= outerbox.vertex.z)
        && (naGetBoxiEndZ(box) <=  naGetBoxiEndZ(outerbox)));
}




NA_IDEF double naGetRangeEnd(NARange range){
  #ifndef NDEBUG
    if(naIsRangeEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naMakeEndWithStartAndLength(range.origin, range.length);
}
NA_IDEF float naGetRangefEnd(NARangef range){
  #ifndef NDEBUG
    if(naIsRangefEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangefValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthf(range.origin, range.length);
}
NA_IDEF double naGetRangeEndE(NARange range){
  #ifndef NDEBUG
    if(!naIsRangeValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naMakeEndWithStartAndLength(range.origin, range.length);
}
NA_IDEF float naGetRangefEndE(NARangef range){
  #ifndef NDEBUG
    if(!naIsRangefValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthf(range.origin, range.length);
}



NA_IDEF NAPos naGetRectEnd(NARect rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEnd", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEnd", "rect is invalid.");
  #endif
  return naMakePos(naGetRectEndX(rect), naGetRectEndY(rect));
}
NA_IDEF double naGetRectEndX(NARect rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndX", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndX", "rect is invalid.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.x, rect.size.width);
}
NA_IDEF double naGetRectEndY(NARect rect){
  #ifndef NDEBUG
    if(naIsRectEmpty(rect))
      naError("naGetRectEndY", "rect is empty.");
    if(!naIsRectValid(rect))
      naError("naGetRectEndY", "rect is invalid.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
}
NA_IDEF double naGetRectEndXE(NARect rect){
  #ifndef NDEBUG
    if(!naIsRectEmpty(rect) && !naIsRectValid(rect))
      naError("naGetRectEndX", "rect is invalid.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.x, rect.size.width);
}
NA_IDEF double naGetRectEndYE(NARect rect){
  #ifndef NDEBUG
    if(!naIsRectEmpty(rect) && !naIsRectValid(rect))
      naError("naGetRectEndY", "rect is invalid.");
  #endif
  return naMakeEndWithStartAndLength(rect.pos.y, rect.size.height);
}



NA_IDEF NAVertex naGetBoxEnd(NABox box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naGetBoxEnd", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naGetBoxEnd", "box is invalid.");
  #endif
  return naMakeVertex(naGetBoxEndX(box), naGetBoxEndY(box), naGetBoxEndZ(box));
}
NA_IDEF double naGetBoxEndX(NABox box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naGetBoxEndX", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naGetBoxEndX", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
}
NA_IDEF double naGetBoxEndY(NABox box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naGetBoxEndY", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naGetBoxEndY", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.y, box.volume.height);
}
NA_IDEF double naGetBoxEndZ(NABox box){
  #ifndef NDEBUG
    if(naIsBoxEmpty(box))
      naError("naGetBoxEndZ", "box is empty.");
    if(!naIsBoxValid(box))
      naError("naGetBoxEndZ", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
}
NA_IDEF double naGetBoxEndXE(NABox box){
  #ifndef NDEBUG
    if(!naIsBoxValid(box))
      naError("naGetBoxEndX", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.x, box.volume.width);
}
NA_IDEF double naGetBoxEndYE(NABox box){
  #ifndef NDEBUG
    if(!naIsBoxValid(box))
      naError("naGetBoxEndY", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.y, box.volume.height);
}
NA_IDEF double naGetBoxEndZE(NABox box){
  #ifndef NDEBUG
    if(!naIsBoxValid(box))
      naError("naGetBoxEndZ", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLength(box.vertex.z, box.volume.depth);
}



NA_IDEF NAInt naGetRangeiEnd(NARangei range){
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(range.origin, range.length);
}
NA_IDEF NAInt naGetRangeiMax(NARangei range){
  #ifndef NDEBUG
    if(naIsRangeiEmpty(range))
      naError("naGetRangeEnd", "range is empty.");
    if(!naIsRangeiValid(range))
      naError("naGetRangeEnd", "range is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(range.origin, range.length);
}




NA_IDEF NAPosi naGetRectiEnd (NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiEnd", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEnd", "rect is invalid.");
  #endif
  return naMakePosi(naGetRectiEndX(rect), naGetRectiEndY(rect));
}
NA_IDEF NAInt naGetRectiEndX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiEndX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndX", "rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiEndY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiEndY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiEndY", "rect is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(rect.pos.y, rect.size.height);
}
NA_IDEF NAPosi naGetRectiMax(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiMax", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMax", "rect is invalid.");
  #endif
  return naMakePosi(naGetRectiMaxX(rect), naGetRectiMaxY(rect));
}
NA_IDEF NAInt naGetRectiMaxX(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiMaxX", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxX", "rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(rect.pos.x, rect.size.width);
}
NA_IDEF NAInt naGetRectiMaxY(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiMaxY", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiMaxY", "rect is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(rect.pos.y, rect.size.height);
}



NA_IDEF NAVertexi naGetBoxiEnd (NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEnd", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEnd", "box is invalid.");
  #endif
  return naMakeVertexi(naGetBoxiEndX(box), naGetBoxiEndY(box), naGetBoxiEndZ(box));
}
NA_IDEF NAInt naGetBoxiEndX(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEndX", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEndX", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiEndY(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEndY", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEndY", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiEndZ(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiEndZ", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiEndZ", "box is invalid.");
  #endif
  return naMakeEndWithStartAndLengthi(box.vertex.z, box.volume.depth);
}
NA_IDEF NAVertexi naGetBoxiMax (NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiMax", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiMax", "box is invalid.");
  #endif
  return naMakeVertexi(naGetBoxiMaxX(box), naGetBoxiMaxY(box), naGetBoxiMaxZ(box));
}
NA_IDEF NAInt naGetBoxiMaxX(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiMaxX", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiMaxX", "box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.x, box.volume.width);
}
NA_IDEF NAInt naGetBoxiMaxY(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiMaxY", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiMaxY", "box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.y, box.volume.height);
}
NA_IDEF NAInt naGetBoxiMaxZ(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiMaxZ", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiMaxZ", "box is invalid.");
  #endif
  return naMakeMaxWithMinAndLengthi(box.vertex.z, box.volume.depth);
}



NA_IDEF NAInt naGetIndexWithOriginAndPosRowFirst(NAPosi origin, NAPosi offset, NAInt width){
  return ((offset.y - origin.y)) * width + (offset.x - origin.x);
}
NA_IDEF NAInt naGetIndexWithOriginAndPosColumnFirst(NAPosi origin, NAPosi offset, NAInt height){
  return ((offset.x - origin.x)) * height + (offset.y - origin.y);
}
NA_IDEF NAInt naGetIndexWithOriginAndVertexRowFirst(NAVertexi origin, NAVertexi vertex, NAInt width, NAInt height){
  return (((vertex.z - origin.z)) * height + (vertex.y - origin.y)) * width + (vertex.x - origin.x);
}
NA_IDEF NAInt naGetIndexWithOriginAndVertexColumnFirst(NAVertexi origin, NAVertexi vertex, NAInt depth, NAInt height){
  return ((vertex.x - origin.x) * height + (vertex.y - origin.y)) * depth + (vertex.z - origin.z);
}




NA_IDEF NAInt naGetRectiIndexCount(NARecti rect){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiIndexCount", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexCount", "rect is invalid.");
  #endif
  return rect.size.width * rect.size.height;
}
NA_IDEF NAInt naGetRectiIndexOfPosRowFirst(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiIndexOfPosRowFirst", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexOfPosRowFirst", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naGetRectiIndexOfPosRowFirst", "pos is invalid.");
    if(!naContainsRectiPos(rect, pos))
      naError("naGetRectiIndexOfPosRowFirst", "pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosRowFirst(rect.pos, pos, rect.size.width);
}
NA_IDEF NAInt naGetRectiIndexOfPosColumnFirst(NARecti rect, NAPosi pos){
  #ifndef NDEBUG
    if(naIsRectiEmptySlow(rect))
      naError("naGetRectiIndexOfPosColumnFirst", "rect is empty.");
    if(!naIsRectiValid(rect))
      naError("naGetRectiIndexOfPosColumnFirst", "rect is invalid.");
    if(!naIsPosiValid(pos))
      naError("naGetRectiIndexOfPosColumnFirst", "pos is invalid.");
    if(!naContainsRectiPos(rect, pos))
      naError("naGetRectiIndexOfPosColumnFirst", "pos is not inside rect.");
  #endif
  return naGetIndexWithOriginAndPosColumnFirst(rect.pos, pos, rect.size.height);
}



NA_IDEF NAInt naGetBoxiIndexCount(NABoxi box){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiIndexCount", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiIndexCount", "box is invalid.");
  #endif
  return box.volume.width * box.volume.height * box.volume.depth;
}
NA_IDEF NAInt naGetBoxiIndexOfVertexRowFirst(NABoxi box, NAVertexi vertex){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiIndexOfVertexRowFirst", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiIndexOfVertexRowFirst", "box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("naGetBoxiIndexOfVertexRowFirst", "vertex is invalid.");
    if(!naContainsBoxiVertex(box, vertex))
      naError("naGetBoxiIndexOfVertexRowFirst", "vertex is not inside box.");
  #endif
  return naGetIndexWithOriginAndVertexRowFirst(box.vertex, vertex, box.volume.width, box.volume.height);
}
NA_IDEF NAInt naGetBoxiIndexOfVertexColumnFirst(NABoxi box, NAVertexi vertex){
  #ifndef NDEBUG
    if(naIsBoxiEmptySlow(box))
      naError("naGetBoxiIndexOfVertexColumnFirst", "box is empty.");
    if(!naIsBoxiValid(box))
      naError("naGetBoxiIndexOfVertexColumnFirst", "box is invalid.");
    if(!naIsVertexiValid(vertex))
      naError("naGetBoxiIndexOfVertexColumnFirst", "vertex is invalid.");
    if(!naContainsBoxiVertex(box, vertex))
      naError("naGetBoxiIndexOfVertexColumnFirst", "vertex is not inside box.");
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
  NAInt totalsize = size.width * size.height;
  #ifndef NDEBUG
    if(naIsLengthValueEmptyi(totalsize) && !naIsLengthValueEmptyi(size.width) && !naIsLengthValueEmptyi(size.height))
      naError("naIsSizeiEmpty", "You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi(totalsize);
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
  #ifndef NDEBUG
    if(naIsLengthValueEmptyi(totalvolume) && !naIsLengthValueEmptyi(volume.width) && !naIsLengthValueEmptyi(volume.height) && !naIsLengthValueEmptyi(volume.depth))
      naError("naIsVolumeiEmpty", "You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
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










// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
