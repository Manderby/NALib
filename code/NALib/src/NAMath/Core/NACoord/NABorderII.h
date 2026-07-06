
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NABorder1D naMakeBorder1D(double begin1, double end1) {
  NABorder1D newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValid(begin1) || !naIsLengthValueValid(end1))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  return newBorder;
}

NA_IDEF NABorder1Df naMakeBorder1Df(float begin1, float end1) {
  NABorder1Df newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(begin1) || !naIsLengthValueValidf(end1))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  return newBorder;
}

NA_IDEF NABorder1Di32 naMakeBorder1Di32(int32 begin1, int32 end1) {
  NABorder1Di32 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi32(begin1) || !naIsLengthValueValidi32(end1))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  return newBorder;
}

NA_IDEF NABorder1Di64 naMakeBorder1Di64(int64 begin1, int64 end1) {
  NABorder1Di64 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi64(begin1) || !naIsLengthValueValidi64(end1))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  return newBorder;
}

NA_IDEF NABorder1Ds naMakeBorder1Ds(size_t begin1, size_t end1) {
  NABorder1Ds newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValids(begin1) || !naIsLengthValueValids(end1))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  return newBorder;
}



NA_IDEF NABorder2D naMakeBorder2D(double begin1, double end1, double begin2, double end2) {
  NABorder2D newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValid(begin1) || !naIsLengthValueValid(end1) || !naIsLengthValueValid(begin2) || !naIsLengthValueValid(end2))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  return newBorder;
}

NA_IDEF NABorder2Df naMakeBorder2Df(float begin1, float end1, float begin2, float end2) {
  NABorder2Df newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(begin1) || !naIsLengthValueValidf(end1) || !naIsLengthValueValidf(begin2) || !naIsLengthValueValidf(end2))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  return newBorder;
}

NA_IDEF NABorder2Di32 naMakeBorder2Di32(int32 begin1, int32 end1, int32 begin2, int32 end2) {
  NABorder2Di32 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi32(begin1) || !naIsLengthValueValidi32(end1) || !naIsLengthValueValidi32(begin2) || !naIsLengthValueValidi32(end2))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  return newBorder;
}

NA_IDEF NABorder2Di64 naMakeBorder2Di64(int64 begin1, int64 end1, int64 begin2, int64 end2) {
  NABorder2Di64 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi64(begin1) || !naIsLengthValueValidi64(end1) || !naIsLengthValueValidi64(begin2) || !naIsLengthValueValidi64(end2))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  return newBorder;
}

NA_IDEF NABorder2Ds naMakeBorder2Ds(size_t begin1, size_t end1, size_t begin2, size_t end2) {
  NABorder2Ds newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValids(begin1) || !naIsLengthValueValids(end1) || !naIsLengthValueValids(begin2) || !naIsLengthValueValids(end2))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  return newBorder;
}



NA_IDEF NABorder3D naMakeBorder3D(double begin1, double end1, double begin2, double end2, double begin3, double end3) {
  NABorder3D newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValid(begin1) || !naIsLengthValueValid(end1) || !naIsLengthValueValid(begin2) || !naIsLengthValueValid(end2) || !naIsLengthValueValid(begin3) || !naIsLengthValueValid(end3))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  newBorder.begin3 = begin3;
  newBorder.end3 = end3;
  return newBorder;
}

NA_IDEF NABorder3Df naMakeBorder3Df(float begin1, float end1, float begin2, float end2, float begin3, float end3) {
  NABorder3Df newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidf(begin1) || !naIsLengthValueValidf(end1) || !naIsLengthValueValidf(begin2) || !naIsLengthValueValidf(end2) || !naIsLengthValueValidf(begin3) || !naIsLengthValueValidf(end3))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  newBorder.begin3 = begin3;
  newBorder.end3 = end3;
  return newBorder;
}

NA_IDEF NABorder3Di32 naMakeBorder3Di32(int32 begin1, int32 end1, int32 begin2, int32 end2, int32 begin3, int32 end3) {
  NABorder3Di32 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi32(begin1) || !naIsLengthValueValidi32(end1) || !naIsLengthValueValidi32(begin2) || !naIsLengthValueValidi32(end2) || !naIsLengthValueValidi32(begin3) || !naIsLengthValueValidi32(end3))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  newBorder.begin3 = begin3;
  newBorder.end3 = end3;
  return newBorder;
}

NA_IDEF NABorder3Di64 naMakeBorder3Di64(int64 begin1, int64 end1, int64 begin2, int64 end2, int64 begin3, int64 end3) {
  NABorder3Di64 newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValidi64(begin1) || !naIsLengthValueValidi64(end1) || !naIsLengthValueValidi64(begin2) || !naIsLengthValueValidi64(end2) || !naIsLengthValueValidi64(begin3) || !naIsLengthValueValidi64(end3))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  newBorder.begin3 = begin3;
  newBorder.end3 = end3;
  return newBorder;
}

NA_IDEF NABorder3Ds naMakeBorder3Ds(size_t begin1, size_t end1, size_t begin2, size_t end2, size_t begin3, size_t end3) {
  NABorder3Ds newBorder;
  #if NA_DEBUG
    if(!naIsLengthValueValids(begin1) || !naIsLengthValueValids(end1) || !naIsLengthValueValids(begin2) || !naIsLengthValueValids(end2) || !naIsLengthValueValids(begin3) || !naIsLengthValueValids(end3))
      naError("Invalid values given.");
  #endif
  newBorder.begin1 = begin1;
  newBorder.end1 = end1;
  newBorder.begin2 = begin2;
  newBorder.end2 = end2;
  newBorder.begin3 = begin3;
  newBorder.end3 = end3;
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
  newRange.origin = range.origin - border.begin1;
  newRange.length = range.length + border.begin1 + border.end1;
  return newRange;
}
NA_IDEF NARangef naMakeRangefWithBorder(NARangef range, NABorder1Df border) {
  NARangef newRange;
  newRange.origin = range.origin - border.begin1;
  newRange.length = range.length + border.begin1 + border.end1;
  return newRange;
}
NA_IDEF NARangei32 naMakeRangei32WithBorder(NARangei32 range, NABorder1Di32 border) {
  NARangei32 newRange;
  newRange.origin = range.origin - border.begin1;
  newRange.length = range.length + border.begin1 + border.end1;
  return newRange;
}
NA_IDEF NARangei64 naMakeRangei64WithBorder(NARangei64 range, NABorder1Di64 border) {
  NARangei64 newRange;
  newRange.origin = naSubi64(range.origin, border.begin1);
  newRange.length = naAddi64(naAddi64(range.length, border.begin1), border.end1);
  return newRange;
}
NA_IDEF NARanges naMakeRangesWithBorder(NARanges range, NABorder1Ds border) {
  NARanges newRange;
  newRange.origin = range.origin - border.begin1;
  newRange.length = range.length + border.begin1 + border.end1;
  return newRange;
}



NA_IDEF NARect naMakeRectWithBorder(NARect rect, NABorder2D border) {
  NARect newRect;
  newRect.pos.x = rect.pos.x - border.begin1;
  newRect.pos.y = rect.pos.y - border.begin2;
  newRect.size.width = rect.size.width + border.begin1 + border.end1;
  newRect.size.height = rect.size.height + border.begin2 + border.end2;
  return newRect;
}
NA_IDEF NARectf naMakeRectfWithBorder(NARectf rect, NABorder2Df border) {
  NARectf newRect;
  newRect.pos.x = rect.pos.x - border.begin1;
  newRect.pos.y = rect.pos.y - border.begin2;
  newRect.size.width = rect.size.width + border.begin1 + border.end1;
  newRect.size.height = rect.size.height + border.begin2 + border.end2;
  return newRect;
}
NA_IDEF NARecti32 naMakeRecti32WithBorder(NARecti32 rect, NABorder2Di32 border) {
  NARecti32 newRect;
  newRect.pos.x = rect.pos.x - border.begin1;
  newRect.pos.y = rect.pos.y - border.begin2;
  newRect.size.width = rect.size.width + border.begin1 + border.end1;
  newRect.size.height = rect.size.height + border.begin2 + border.end2;
  return newRect;
}
NA_IDEF NARecti64 naMakeRecti64WithBorder(NARecti64 rect, NABorder2Di64 border) {
  NARecti64 newRect;
  newRect.pos.x = naSubi64(rect.pos.x, border.begin1);
  newRect.pos.y = naSubi64(rect.pos.y, border.begin2);
  newRect.size.width = naAddi64(naAddi64(rect.size.width, border.begin1), border.end1);
  newRect.size.height = naAddi64(naAddi64(rect.size.height, border.begin2), border.end2);
  return newRect;
}
NA_IDEF NARects naMakeRectsWithBorder(NARects rect, NABorder2Ds border) {
  NARects newRect;
  newRect.pos.x = rect.pos.x - border.begin1;
  newRect.pos.y = rect.pos.y - border.begin2;
  newRect.size.width = rect.size.width + border.begin1 + border.end1;
  newRect.size.height = rect.size.height + border.begin2 + border.end2;
  return newRect;
}



NA_IDEF NABox naMakeBoxWithBorder(NABox box, NABorder3D border) {
  NABox newBox;
  newBox.vertex.x = box.vertex.x - border.begin1;
  newBox.vertex.y = box.vertex.y - border.begin2;
  newBox.vertex.z = box.vertex.z - border.begin3;
  newBox.volume.width = box.volume.width + border.begin1 + border.end1;
  newBox.volume.height = box.volume.height + border.begin2 + border.end2;
  newBox.volume.depth = box.volume.depth + border.begin3 + border.end3;
  return newBox;
}
NA_IDEF NABoxf naMakeBoxfWithBorder(NABoxf box, NABorder3Df border) {
  NABoxf newBox;
  newBox.vertex.x = box.vertex.x - border.begin1;
  newBox.vertex.y = box.vertex.y - border.begin2;
  newBox.vertex.z = box.vertex.z - border.begin3;
  newBox.volume.width = box.volume.width + border.begin1 + border.end1;
  newBox.volume.height = box.volume.height + border.begin2 + border.end2;
  newBox.volume.depth = box.volume.depth + border.begin3 + border.end3;
  return newBox;
}
NA_IDEF NABoxi32 naMakeBoxi32WithBorder(NABoxi32 box, NABorder3Di32 border) {
  NABoxi32 newBox;
  newBox.vertex.x = box.vertex.x - border.begin1;
  newBox.vertex.y = box.vertex.y - border.begin2;
  newBox.vertex.z = box.vertex.z - border.begin3;
  newBox.volume.width = box.volume.width + border.begin1 + border.end1;
  newBox.volume.height = box.volume.height + border.begin2 + border.end2;
  newBox.volume.depth = box.volume.depth + border.begin3 + border.end3;
  return newBox;
}
NA_IDEF NABoxi64 naMakeBoxi64WithBorder(NABoxi64 box, NABorder3Di64 border) {
  NABoxi64 newBox;
  newBox.vertex.x = naSubi64(box.vertex.x, border.begin1);
  newBox.vertex.y = naSubi64(box.vertex.y, border.begin2);
  newBox.vertex.z = naSubi64(box.vertex.z, border.begin3);
  newBox.volume.width = naAddi64(naAddi64(box.volume.width, border.begin1), border.end1);
  newBox.volume.height = naAddi64(naAddi64(box.volume.height, border.begin2), border.end2);
  newBox.volume.depth = naAddi64(naAddi64(box.volume.depth, border.begin3), border.end3);
  return newBox;
}
NA_IDEF NABoxs naMakeBoxsWithBorder(NABoxs box, NABorder3Ds border) {
  NABoxs newBox;
  newBox.vertex.x = box.vertex.x - border.begin1;
  newBox.vertex.y = box.vertex.y - border.begin2;
  newBox.vertex.z = box.vertex.z - border.begin3;
  newBox.volume.width = box.volume.width + border.begin1 + border.end1;
  newBox.volume.height = box.volume.height + border.begin2 + border.end2;
  newBox.volume.depth = box.volume.depth + border.begin3 + border.end3;
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
