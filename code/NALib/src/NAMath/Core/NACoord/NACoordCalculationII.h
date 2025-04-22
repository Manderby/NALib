
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NARange naMakeRangePositive(NARange range) {
  double rangeNegative = (range.length < 0.) ? 1. : 0.;
  range.origin += rangeNegative * range.length;
  range.length -= rangeNegative * 2. * range.length;
  return range;
}
NA_IDEF NARangef naMakeRangefPositive(NARangef range) {
  float rangeNegative = (range.length < 0.f) ? 1.f : 0.f;
  range.origin += rangeNegative * range.length;
  range.length -= rangeNegative * 2.f * range.length;
  return range;
}
NA_IDEF NARangei32 naMakeRangei32Positive(NARangei32 range) {
  int32 rangeNegative = (int32)(range.length < 0);
  range.origin += rangeNegative * range.length;
  range.length -= rangeNegative * 2 * range.length;
  return range;
}
NA_IDEF NARangei64 naMakeRangei64Positive(NARangei64 range) {
  int64 rangeNegative = naCastBoolToi64(naSmalleri64(range.length, NA_ZERO_i64));
  range.origin = naAddi64(range.origin, naMuli64(rangeNegative, range.length));
  range.length = naSubi64(range.length, naMuli64(naMuli64(rangeNegative, naCastu32Toi64(2)), range.length));
  return range;
}
NA_IDEF NARanges naMakeRangesPositive(NARanges range) {
  #if NA_DEBUG
    naError("NARanges is always positive.");
  #endif
  return range;
}



NA_IDEF NARect naMakeRectPositive(NARect rect) {
  NARange range1 = naMakeRangePositive(naMakeRangeE(rect.pos.x, rect.size.width));
  NARange range2 = naMakeRangePositive(naMakeRangeE(rect.pos.y, rect.size.height));
  return naMakeRectS(range1.origin, range2.origin, range1.length, range2.length);
}
NA_IDEF NARectf naMakeRectfPositive(NARectf rect) {
  NARangef range1 = naMakeRangefPositive(naMakeRangefE(rect.pos.x, rect.size.width));
  NARangef range2 = naMakeRangefPositive(naMakeRangefE(rect.pos.y, rect.size.height));
  return naMakeRectfS(range1.origin, range2.origin, range1.length, range2.length);
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
NA_IDEF NARects naMakeRectsPositive(NARects rect) {
  NARanges range1 = naMakeRangesPositive(naMakeRangesE(rect.pos.x, rect.size.width));
  NARanges range2 = naMakeRangesPositive(naMakeRangesE(rect.pos.y, rect.size.height));
  return naMakeRectsS(range1.origin, range2.origin, range1.length, range2.length);
}



NA_IDEF NABox naMakeBoxPositive(NABox box) {
  NARange range1 = naMakeRangePositive(naMakeRangeE(box.vertex.x, box.volume.width));
  NARange range2 = naMakeRangePositive(naMakeRangeE(box.vertex.y, box.volume.height));
  NARange range3 = naMakeRangePositive(naMakeRangeE(box.vertex.z, box.volume.depth));
  return naMakeBoxS(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}
NA_IDEF NABoxf naMakeBoxfPositive(NABoxf box) {
  NARangef range1 = naMakeRangefPositive(naMakeRangefE(box.vertex.x, box.volume.width));
  NARangef range2 = naMakeRangefPositive(naMakeRangefE(box.vertex.y, box.volume.height));
  NARangef range3 = naMakeRangefPositive(naMakeRangefE(box.vertex.z, box.volume.depth));
  return naMakeBoxfS(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}
NA_IDEF NABoxi32 naMakeBoxi32Positive(NABoxi32 box) {
  NARangei32 range1 = naMakeRangei32Positive(naMakeRangei32E(box.vertex.x, box.volume.width));
  NARangei32 range2 = naMakeRangei32Positive(naMakeRangei32E(box.vertex.y, box.volume.height));
  NARangei32 range3 = naMakeRangei32Positive(naMakeRangei32E(box.vertex.z, box.volume.depth));
  return naMakeBoxi32S(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}
NA_IDEF NABoxi64 naMakeBoxi64Positive(NABoxi64 box) {
  NARangei64 range1 = naMakeRangei64Positive(naMakeRangei64E(box.vertex.x, box.volume.width));
  NARangei64 range2 = naMakeRangei64Positive(naMakeRangei64E(box.vertex.y, box.volume.height));
  NARangei64 range3 = naMakeRangei64Positive(naMakeRangei64E(box.vertex.z, box.volume.depth));
  return naMakeBoxi64S(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}
NA_IDEF NABoxs naMakeBoxsPositive(NABoxs box) {
  NARanges range1 = naMakeRangesPositive(naMakeRangesE(box.vertex.x, box.volume.width));
  NARanges range2 = naMakeRangesPositive(naMakeRangesE(box.vertex.y, box.volume.height));
  NARanges range3 = naMakeRangesPositive(naMakeRangesE(box.vertex.z, box.volume.depth));
  return naMakeBoxsS(range1.origin, range2.origin, range3.origin, range1.length, range2.length, range3.length);
}



NA_IDEF double naGetRangeCenter(NARange range) {
  double center;
  #if NA_DEBUG
    if(!naIsRangeUseful(range))
      naError("range is not useful");
  #endif
  center = range.origin + 0.5 * range.length;
  return center;
}
NA_IDEF float naGetRangefCenter(NARangef range) {
  float center;
  #if NA_DEBUG
    if(!naIsRangefUseful(range))
      naError("range is not useful");
  #endif
  center = range.origin + 0.5f * range.length;
  return center;
}
NA_IDEF int32 naGetRangei32Center(NARangei32 range) {
  int32 center;
  #if NA_DEBUG
    if(!naIsRangei32Useful(range))
      naError("range is not useful");
  #endif
  center = range.origin + range.length / 2;
  return center;
}
NA_IDEF int64 naGetRangei64Center(NARangei64 range) {
  int64 center;
  #if NA_DEBUG
    if(!naIsRangei64Useful(range))
      naError("range is not useful");
  #endif
  center = naAddi64(range.origin, naDivi64(range.length, naCastu32Toi64(2)));
  return center;
}
NA_IDEF size_t naGetRangesCenter(NARanges range) {
  size_t center;
  #if NA_DEBUG
    if(!naIsRangesUseful(range))
      naError("range is not useful");
  #endif
  center = range.origin + range.length / 2;
  return center;
}



NA_IDEF NAPos naGetRectCenter(NARect rect) {
  NAPos center;
  #if NA_DEBUG
    if(!naIsRectUseful(rect))
      naError("rect is not useful");
  #endif
  center.x = rect.pos.x + 0.5 * rect.size.width;
  center.y = rect.pos.y + 0.5 * rect.size.height;
  return center;
}
NA_IDEF NAPosf naGetRectfCenter(NARectf rect) {
  NAPosf center;
  #if NA_DEBUG
    if(!naIsRectfUseful(rect))
      naError("rect is not useful");
  #endif
  center.x = rect.pos.x + 0.5f * rect.size.width;
  center.y = rect.pos.y + 0.5f * rect.size.height;
  return center;
}
NA_IDEF NAPosi32 naGetRecti32Center(NARecti32 rect) {
  NAPosi32 center;
  #if NA_DEBUG
    if(!naIsRecti32Useful(rect))
      naError("rect is not useful");
  #endif
  center.x = rect.pos.x + rect.size.width / 2;
  center.y = rect.pos.y + rect.size.height / 2;
  return center;
}
NA_IDEF NAPosi64 naGetRecti64Center(NARecti64 rect) {
  NAPosi64 center;
  #if NA_DEBUG
    if(!naIsRecti64Useful(rect))
      naError("rect is not useful");
  #endif
  center.x = naAddi64(rect.pos.x, naDivi64(rect.size.width, naCastu32Toi64(2)));
  center.y = naAddi64(rect.pos.y, naDivi64(rect.size.height, naCastu32Toi64(2)));
  return center;
}
NA_IDEF NAPoss naGetRectsCenter(NARects rect) {
  NAPoss center;
  #if NA_DEBUG
    if(!naIsRectsUseful(rect))
      naError("rect is not useful");
  #endif
  center.x = rect.pos.x + rect.size.width / 2;
  center.y = rect.pos.y + rect.size.height / 2;
  return center;
}



NA_IDEF NAVertex naGetBoxCenter(NABox box) {
  NAVertex center;
  #if NA_DEBUG
    if(!naIsBoxUseful(box))
      naError("box is not useful");
  #endif
  center.x = box.vertex.x + 0.5 * box.volume.width;
  center.y = box.vertex.y + 0.5 * box.volume.height;
  center.z = box.vertex.z + 0.5 * box.volume.depth;
  return center;
}
NA_IDEF NAVertexf naGetBoxfCenter(NABoxf box) {
  NAVertexf center;
  #if NA_DEBUG
    if(!naIsBoxfUseful(box))
      naError("box is not useful");
  #endif
  center.x = box.vertex.x + 0.5f * box.volume.width;
  center.y = box.vertex.y + 0.5f * box.volume.height;
  center.z = box.vertex.z + 0.5f * box.volume.depth;
  return center;
}
NA_IDEF NAVertexi32 naGetBoxi32Center(NABoxi32 box) {
  NAVertexi32 center;
  #if NA_DEBUG
    if(!naIsBoxi32Useful(box))
      naError("box is not useful");
  #endif
  center.x = box.vertex.x + box.volume.width / 2;
  center.y = box.vertex.y + box.volume.height / 2;
  center.z = box.vertex.z + box.volume.depth / 2;
  return center;
}
NA_IDEF NAVertexi64 naGetBoxi64Center(NABoxi64 box) {
  NAVertexi64 center;
  #if NA_DEBUG
    if(!naIsBoxi64Useful(box))
      naError("box is not useful");
  #endif
  center.x = naAddi64(box.vertex.x, naDivi64(box.volume.width, naCastu32Toi64(2)));
  center.y = naAddi64(box.vertex.y, naDivi64(box.volume.height, naCastu32Toi64(2)));
  center.z = naAddi64(box.vertex.z, naDivi64(box.volume.depth, naCastu32Toi64(2)));
  return center;
}
NA_IDEF NAVertexs naGetBoxsCenter(NABoxs box) {
  NAVertexs center;
  #if NA_DEBUG
    if(!naIsBoxsUseful(box))
      naError("box is not useful");
  #endif
  center.x = box.vertex.x + box.volume.width / 2;
  center.y = box.vertex.y + box.volume.height / 2;
  center.z = box.vertex.z + box.volume.depth / 2;
  return center;
}



NA_IDEF double naGetRangeCenteredOffset(NARange range, double length) {
  double offset;
  offset = range.origin + 0.5 * (range.length - length);
  return offset;
}
NA_IDEF float naGetRangefCenteredOffset(NARangef range, float length) {
  float offset;
  offset = range.origin + 0.5f * (range.length - length);
  return offset;
}

NA_IDEF NAPos naGetRectCenteredOffset(NARect rect, NASize size) {
  NAPos offset;
  offset.x = rect.pos.x + 0.5 * (rect.size.width - size.width);
  offset.y = rect.pos.y + 0.5 * (rect.size.height - size.height);
  return offset;
}
NA_IDEF NAPosf naGetRectfCenteredOffset(NARectf rect, NASizef size) {
  NAPosf offset;
  offset.x = rect.pos.x + 0.5f * (rect.size.width - size.width);
  offset.y = rect.pos.y + 0.5f * (rect.size.height - size.height);
  return offset;
}

NA_IDEF NAVertex naGetBoxCeneteredOffset(NABox box, NAVolume volume) {
  NAVertex offset;
  offset.x = box.vertex.x + 0.5 * (box.volume.width - volume.width);
  offset.y = box.vertex.y + 0.5 * (box.volume.height - volume.height);
  offset.z = box.vertex.z + 0.5 * (box.volume.depth - volume.depth);
  return offset;
}
NA_IDEF NAVertexf naGetBoxfCeneteredOffset(NABoxf box, NAVolumef volume) {
  NAVertexf offset;
  offset.x = box.vertex.x + 0.5f * (box.volume.width - volume.width);
  offset.y = box.vertex.y + 0.5f * (box.volume.height - volume.height);
  offset.z = box.vertex.z + 0.5f * (box.volume.depth - volume.depth);
  return offset;
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
