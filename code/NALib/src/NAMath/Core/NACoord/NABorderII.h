
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NABezel4 naMakeBezel4(double right, double top, double left, double bottom) {
  NABezel4 newBezel;
  #if NA_DEBUG
    naError("NABezel4 is deprecated. Use NABorder2D");
    if(!naIsLengthValueValid(right) || !naIsLengthValueValid(top) || !naIsLengthValueValid(left) || !naIsLengthValueValid(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4f naMakeBezel4f(float right, float top, float left, float bottom) {
  NABezel4f newBezel;
  #if NA_DEBUG
    naError("NABezel4f is deprecated. Use NABorder2Df");
    if(!naIsLengthValueValidf(right) || !naIsLengthValueValidf(top) || !naIsLengthValueValidf(left) || !naIsLengthValueValidf(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4i32 naMakeBezel4i32(int32 right, int32 top, int32 left, int32 bottom) {
  NABezel4i32 newBezel;
  #if NA_DEBUG
    naError("NABezel4i32 is deprecated. Use NABorder2Di32");
    if(!naIsLengthValueValidi32(right) || !naIsLengthValueValidi32(top) || !naIsLengthValueValidi32(left) || !naIsLengthValueValidi32(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4i64 naMakeBezel4i64(int64 right, int64 top, int64 left, int64 bottom) {
  NABezel4i64 newBezel;
  #if NA_DEBUG
    naError("NABezel4i64 is deprecated. Use NABorder2Di64");
    if(!naIsLengthValueValidi64(right) || !naIsLengthValueValidi64(top) || !naIsLengthValueValidi64(left) || !naIsLengthValueValidi64(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}

NA_IDEF NABezel4s naMakeBezel4s(size_t right, size_t top, size_t left, size_t bottom) {
  NABezel4s newBezel;
  #if NA_DEBUG
    naError("NABezel4s is deprecated. Use NABorder2Ds");
    if(!naIsLengthValueValids(right) || !naIsLengthValueValids(top) || !naIsLengthValueValids(left) || !naIsLengthValueValids(bottom))
      naError("Invalid values given.");
  #endif
  newBezel.right = right;
  newBezel.top = top;
  newBezel.left = left;
  newBezel.bottom = bottom;
  return newBezel;
}



NA_IDEF NABorder1D naMakeBorder1D(double left, double right) {
  NABorder1D newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValid(left) || !naIsLengthValueValid(right))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  return newBorder;
}

NA_IDEF NABorder1Df naMakeBorder1Df(float left, float right) {
  NABorder1Df newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(left) || !naIsLengthValueValidf(right))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  return newBorder;
}

NA_IDEF NABorder1Di32 naMakeBorder1Di32(int32 left, int32 right) {
  NABorder1Di32 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi32(left) || !naIsLengthValueValidi32(right))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  return newBorder;
}

NA_IDEF NABorder1Di64 naMakeBorder1Di64(int64 left, int64 right) {
  NABorder1Di64 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi64(left) || !naIsLengthValueValidi64(right))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  return newBorder;
}

NA_IDEF NABorder1Ds naMakeBorder1Ds(size_t left, size_t right) {
  NABorder1Ds newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValids(left) || !naIsLengthValueValids(right))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  return newBorder;
}



NA_IDEF NABorder2D naMakeBorder2D(double left, double right, double bottom, double top) {
  NABorder2D newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValid(left) || !naIsLengthValueValid(right) || !naIsLengthValueValid(bottom) || !naIsLengthValueValid(top))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  return newBorder;
}

NA_IDEF NABorder2Df naMakeBorder2Df(float left, float right, float bottom, float top) {
  NABorder2Df newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(left) || !naIsLengthValueValidf(right) || !naIsLengthValueValidf(bottom) || !naIsLengthValueValidf(top))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  return newBorder;
}

NA_IDEF NABorder2Di32 naMakeBorder2Di32(int32 left, int32 right, int32 bottom, int32 top) {
  NABorder2Di32 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi32(left) || !naIsLengthValueValidi32(right) || !naIsLengthValueValidi32(bottom) || !naIsLengthValueValidi32(top))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  return newBorder;
}

NA_IDEF NABorder2Di64 naMakeBorder2Di64(int64 left, int64 right, int64 bottom, int64 top) {
  NABorder2Di64 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi64(left) || !naIsLengthValueValidi64(right) || !naIsLengthValueValidi64(bottom) || !naIsLengthValueValidi64(top))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  return newBorder;
}

NA_IDEF NABorder2Ds naMakeBorder2Ds(size_t left, size_t right, size_t bottom, size_t top) {
  NABorder2Ds newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValids(left) || !naIsLengthValueValids(right) || !naIsLengthValueValids(bottom) || !naIsLengthValueValids(top))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  return newBorder;
}



NA_IDEF NABorder3D naMakeBorder3D(double left, double right, double bottom, double top, double back, double front) {
  NABorder3D newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValid(left) || !naIsLengthValueValid(right) || !naIsLengthValueValid(bottom) || !naIsLengthValueValid(top) || !naIsLengthValueValid(back) || !naIsLengthValueValid(front))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  newBorder.back = back;
  newBorder.front = front;
  return newBorder;
}

NA_IDEF NABorder3Df naMakeBorder3Df(float left, float right, float bottom, float top, float back, float front) {
  NABorder3Df newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(left) || !naIsLengthValueValidf(right) || !naIsLengthValueValidf(bottom) || !naIsLengthValueValidf(top) || !naIsLengthValueValidf(back) || !naIsLengthValueValidf(front))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  newBorder.back = back;
  newBorder.front = front;
  return newBorder;
}

NA_IDEF NABorder3Di32 naMakeBorder3Di32(int32 left, int32 right, int32 bottom, int32 top, int32 back, int32 front) {
  NABorder3Di32 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi32(left) || !naIsLengthValueValidi32(right) || !naIsLengthValueValidi32(bottom) || !naIsLengthValueValidi32(top) || !naIsLengthValueValidi32(back) || !naIsLengthValueValidi32(front))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  newBorder.back = back;
  newBorder.front = front;
  return newBorder;
}

NA_IDEF NABorder3Di64 naMakeBorder3Di64(int64 left, int64 right, int64 bottom, int64 top, int64 back, int64 front) {
  NABorder3Di64 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi64(left) || !naIsLengthValueValidi64(right) || !naIsLengthValueValidi64(bottom) || !naIsLengthValueValidi64(top) || !naIsLengthValueValidi64(back) || !naIsLengthValueValidi64(front))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  newBorder.back = back;
  newBorder.front = front;
  return newBorder;
}

NA_IDEF NABorder3Ds naMakeBorder3Ds(size_t left, size_t right, size_t bottom, size_t top, size_t back, size_t front) {
  NABorder3Ds newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValids(left) || !naIsLengthValueValids(right) || !naIsLengthValueValids(bottom) || !naIsLengthValueValids(top) || !naIsLengthValueValids(back) || !naIsLengthValueValids(front))
      naError("Invalid values given.");
  #endif
  newBorder.left = left;
  newBorder.right = right;
  newBorder.bottom = bottom;
  newBorder.top = top;
  newBorder.back = back;
  newBorder.front = front;
  return newBorder;
}



NA_IDEF NABorder1D naMakeBorder1DWithThickness(double thickness) {
  NABorder1D newBorder = naMakeBorder1D(thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder1Df naMakeBorder1DfWithThickness(float thickness) {
  NABorder1Df newBorder = naMakeBorder1Df(thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder1Di32 naMakeBorder1Di32WithThickness(int32 thickness) {
  NABorder1Di32 newBorder = naMakeBorder1Di32(thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder1Di64 naMakeBorder1Di64WithThickness(int64 thickness) {
  NABorder1Di64 newBorder = naMakeBorder1Di64(thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder1Ds naMakeBorder1DsWithThickness(size_t thickness) {
  NABorder1Ds newBorder = naMakeBorder1Ds(thickness, thickness);
  return newBorder;
}



NA_IDEF NABorder2D naMakeBorder2DWithThickness(double thickness) {
  NABorder2D newBorder = naMakeBorder2D(thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder2Df naMakeBorder2DfWithThickness(float thickness) {
  NABorder2Df newBorder = naMakeBorder2Df(thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder2Di32 naMakeBorder2Di32WithThickness(int32 thickness) {
  NABorder2Di32 newBorder = naMakeBorder2Di32(thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder2Di64 naMakeBorder2Di64WithThickness(int64 thickness) {
  NABorder2Di64 newBorder = naMakeBorder2Di64(thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder2Ds naMakeBorder2DsWithThickness(size_t thickness) {
  NABorder2Ds newBorder = naMakeBorder2Ds(thickness, thickness, thickness, thickness);
  return newBorder;
}



NA_IDEF NABorder3D naMakeBorder3DWithThickness(double thickness) {
  NABorder3D newBorder = naMakeBorder3D(thickness, thickness, thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder3Df naMakeBorder3DfWithThickness(float thickness) {
  NABorder3Df newBorder = naMakeBorder3Df(thickness, thickness, thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder3Di32 naMakeBorder3Di32WithThickness(int32 thickness) {
  NABorder3Di32 newBorder = naMakeBorder3Di32(thickness, thickness, thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder3Di64 naMakeBorder3Di64WithThickness(int64 thickness) {
  NABorder3Di64 newBorder = naMakeBorder3Di64(thickness, thickness, thickness, thickness, thickness, thickness);
  return newBorder;
}

NA_IDEF NABorder3Ds naMakeBorder3DsWithThickness(size_t thickness) {
  NABorder3Ds newBorder = naMakeBorder3Ds(thickness, thickness, thickness, thickness, thickness, thickness);
  return newBorder;
}



NA_IDEF NARange naMakeRangeWithBorder(NARange range, NABorder1D border) {
  NARange newRange;
  newRange.origin = range.origin - border.left;
  newRange.length = range.length + border.left + border.right;
  return newRange;
}
NA_IDEF NARangef naMakeRangefWithBorder(NARangef range, NABorder1Df border) {
  NARangef newRange;
  newRange.origin = range.origin - border.left;
  newRange.length = range.length + border.left + border.right;
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32WithBorder(NARangei32 range, NABorder1Di32 border) {
  NARangei32 newRange;
  newRange.origin = range.origin - border.left;
  newRange.length = range.length + border.left + border.right;
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64WithBorder(NARangei64 range, NABorder1Di64 border) {
  NARangei64 newRange;
  newRange.origin = range.origin - border.left;
  newRange.length = range.length + border.left + border.right;
  return newRange;
}
NA_IDEF NARanges naMakeRangesWithBorder(NARanges range, NABorder1Ds border) {
  NARanges newRange;
  newRange.origin = range.origin - border.left;
  newRange.length = range.length + border.left + border.right;
  return newRange;
}



NA_IDEF NARect naMakeRectWithBorder(NARect rect, NABorder2D border) {
  NARect newRect;
  newRect.pos.x = rect.pos.x - border.left;
  newRect.pos.y = rect.pos.y - border.bottom;
  newRect.size.width = rect.size.width + border.left + border.right;
  newRect.size.height = rect.size.height + border.bottom + border.top;
  return newRect;
}
NA_IDEF NARectf naMakeRectfWithBorder(NARectf rect, NABorder2Df border) {
  NARectf newRect;
  newRect.pos.x = rect.pos.x - border.left;
  newRect.pos.y = rect.pos.y - border.bottom;
  newRect.size.width = rect.size.width + border.left + border.right;
  newRect.size.height = rect.size.height + border.bottom + border.top;
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32WithBorder(NARecti32 rect, NABorder2Di32 border) {
  NARecti32 newRect;
  newRect.pos.x = rect.pos.x - border.left;
  newRect.pos.y = rect.pos.y - border.bottom;
  newRect.size.width = rect.size.width + border.left + border.right;
  newRect.size.height = rect.size.height + border.bottom + border.top;
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64WithBorder(NARecti64 rect, NABorder2Di64 border) {
  NARecti64 newRect;
  newRect.pos.x = rect.pos.x - border.left;
  newRect.pos.y = rect.pos.y - border.bottom;
  newRect.size.width = rect.size.width + border.left + border.right;
  newRect.size.height = rect.size.height + border.bottom + border.top;
  return newRect;
}
NA_IDEF NARects naMakeRectsWithBorder(NARects rect, NABorder2Ds border) {
  NARects newRect;
  newRect.pos.x = rect.pos.x - border.left;
  newRect.pos.y = rect.pos.y - border.bottom;
  newRect.size.width = rect.size.width + border.left + border.right;
  newRect.size.height = rect.size.height + border.bottom + border.top;
  return newRect;
}



NA_IDEF NABox naMakeBoxWithBorder(NABox box, NABorder3D border) {
  NABox newBox;
  newBox.vertex.x = box.vertex.x - border.left;
  newBox.vertex.y = box.vertex.y - border.bottom;
  newBox.vertex.z = box.vertex.z - border.back;
  newBox.volume.width = box.volume.width + border.left + border.right;
  newBox.volume.height = box.volume.height + border.bottom + border.top;
  newBox.volume.depth = box.volume.depth + border.back + border.front;
  return newBox;
}
NA_IDEF NABoxf naMakeBoxfWithBorder(NABoxf box, NABorder3Df border) {
  NABoxf newBox;
  newBox.vertex.x = box.vertex.x - border.left;
  newBox.vertex.y = box.vertex.y - border.bottom;
  newBox.vertex.z = box.vertex.z - border.back;
  newBox.volume.width = box.volume.width + border.left + border.right;
  newBox.volume.height = box.volume.height + border.bottom + border.top;
  newBox.volume.depth = box.volume.depth + border.back + border.front;
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32WithBorder(NABoxi32 box, NABorder3Di32 border) {
  NABoxi32 newBox;
  newBox.vertex.x = box.vertex.x - border.left;
  newBox.vertex.y = box.vertex.y - border.bottom;
  newBox.vertex.z = box.vertex.z - border.back;
  newBox.volume.width = box.volume.width + border.left + border.right;
  newBox.volume.height = box.volume.height + border.bottom + border.top;
  newBox.volume.depth = box.volume.depth + border.back + border.front;
  return newBox;
}
NA_IDEF NABoxi64 naMakeBoxi64WithBorder(NABoxi64 box, NABorder3Di64 border) {
  NABoxi64 newBox;
  newBox.vertex.x = box.vertex.x - border.left;
  newBox.vertex.y = box.vertex.y - border.bottom;
  newBox.vertex.z = box.vertex.z - border.back;
  newBox.volume.width = box.volume.width + border.left + border.right;
  newBox.volume.height = box.volume.height + border.bottom + border.top;
  newBox.volume.depth = box.volume.depth + border.back + border.front;
  return newBox;
}
NA_IDEF NABoxs naMakeBoxsWithBorder(NABoxs box, NABorder3Ds border) {
  NABoxs newBox;
  newBox.vertex.x = box.vertex.x - border.left;
  newBox.vertex.y = box.vertex.y - border.bottom;
  newBox.vertex.z = box.vertex.z - border.back;
  newBox.volume.width = box.volume.width + border.left + border.right;
  newBox.volume.height = box.volume.height + border.bottom + border.top;
  newBox.volume.depth = box.volume.depth + border.back + border.front;
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
