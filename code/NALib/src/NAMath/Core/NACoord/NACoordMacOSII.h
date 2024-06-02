
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



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
