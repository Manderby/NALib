
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



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
NA_IDEF NABool naIsRangesValid(NARanges range) {
  return naIsOffsetValueValids(range.origin) && naIsLengthValueValids(range.length);
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
NA_IDEF NABool naIsRectfValid(NARectf rect) {
  return (naIsPosfValid(rect.pos) && naIsSizefValid(rect.size));
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
NA_IDEF NABool naIsRectsValid(NARects rect) {
  return (naIsPossValid(rect.pos) && naIsSizesValid(rect.size));
}



NA_IDEF NABool naIsVertexValid(NAVertex vertex) {
  return naIsOffsetValueValid(vertex.x) && naIsOffsetValueValid(vertex.y) && naIsOffsetValueValid(vertex.z);
}
NA_IDEF NABool naIsVertexfValid(NAVertexf vertex) {
  return naIsOffsetValueValidf(vertex.x) && naIsOffsetValueValidf(vertex.y) && naIsOffsetValueValidf(vertex.z);
}
NA_IDEF NABool naIsVertexiValid(NAVertexi vertex) {
  return naIsOffsetValueValidi(vertex.x) && naIsOffsetValueValidi(vertex.y) && naIsOffsetValueValidi(vertex.z);
}
NA_IDEF NABool naIsVertexi32Valid(NAVertexi32 vertex) {
  return naIsOffsetValueValidi32(vertex.x) && naIsOffsetValueValidi32(vertex.y) && naIsOffsetValueValidi32(vertex.z);
}
NA_IDEF NABool naIsVertexi64Valid(NAVertexi64 vertex) {
  return naIsOffsetValueValidi64(vertex.x) && naIsOffsetValueValidi64(vertex.y) && naIsOffsetValueValidi64(vertex.z);
}
NA_IDEF NABool naIsVertexsValid(NAVertexs vertex) {
  return naIsOffsetValueValids(vertex.x) && naIsOffsetValueValids(vertex.y) && naIsOffsetValueValids(vertex.z);
}



NA_IDEF NABool naIsVolumeValid(NAVolume volume) {
  return naIsLengthValueValid(volume.width) && naIsLengthValueValid(volume.height) && naIsLengthValueValid(volume.depth);
}
NA_IDEF NABool naIsVolumefValid(NAVolumef volume) {
  return naIsLengthValueValidf(volume.width) && naIsLengthValueValidf(volume.height) && naIsLengthValueValidf(volume.depth);
}
NA_IDEF NABool naIsVolumeiValid(NAVolumei volume) {
  return naIsLengthValueValidi(volume.width) && naIsLengthValueValidi(volume.height) && naIsLengthValueValidi(volume.depth);
}
NA_IDEF NABool naIsVolumei32Valid(NAVolumei32 volume) {
  return naIsLengthValueValidi32(volume.width) && naIsLengthValueValidi32(volume.height) && naIsLengthValueValidi32(volume.depth);
}
NA_IDEF NABool naIsVolumei64Valid(NAVolumei64 volume) {
  return naIsLengthValueValidi64(volume.width) && naIsLengthValueValidi64(volume.height) && naIsLengthValueValidi64(volume.depth);
}
NA_IDEF NABool naIsVolumesValid(NAVolumes volume) {
  return naIsLengthValueValids(volume.width) && naIsLengthValueValids(volume.height) && naIsLengthValueValids(volume.depth);
}



NA_IDEF NABool naIsBoxValid(NABox box) {
  return (naIsVertexValid(box.vertex) && naIsVolumeValid(box.volume));
}
NA_IDEF NABool naIsBoxfValid(NABoxf box) {
  return (naIsVertexfValid(box.vertex) && naIsVolumefValid(box.volume));
}
NA_IDEF NABool naIsBoxiValid(NABoxi box) {
  return (naIsVertexiValid(box.vertex) && naIsVolumeiValid(box.volume));
}
NA_IDEF NABool naIsBoxi32Valid(NABoxi32 box) {
  return (naIsVertexi32Valid(box.vertex) && naIsVolumei32Valid(box.volume));
}
NA_IDEF NABool naIsBoxi64Valid(NABoxi64 box) {
  return (naIsVertexi64Valid(box.vertex) && naIsVolumei64Valid(box.volume));
}
NA_IDEF NABool naIsBoxsValid(NABoxs box) {
  return (naIsVertexsValid(box.vertex) && naIsVolumesValid(box.volume));
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
NA_IDEF NABool naIsRangesEmpty(NARanges range) {
  return naIsLengthValueEmptys(range.length);
}



NA_IDEF NABool naIsSizeEmpty (NASize size) {
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmpty(size.width) || naIsLengthValueEmpty(size.height);
}
NA_IDEF NABool naIsSizefEmpty (NASizef size) {
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmptyf(size.width) || naIsLengthValueEmptyf(size.height);
}
NA_IDEF NABool naIsSizeiEmpty(NASizei size) {
  NAInt totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi(totalSize) && !naIsLengthValueEmptyi(size.width) && !naIsLengthValueEmptyi(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi(totalSize);
}
NA_IDEF NABool naIsSizei32Empty(NASizei32 size) {
  int32 totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi32(totalSize) && !naIsLengthValueEmptyi32(size.width) && !naIsLengthValueEmptyi32(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi32(totalSize);
}
NA_IDEF NABool naIsSizei64Empty(NASizei64 size) {
  int64 totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi64(totalSize) && !naIsLengthValueEmptyi64(size.width) && !naIsLengthValueEmptyi64(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi64(totalSize);
}
NA_IDEF NABool naIsSizesEmpty(NASizes size) {
  size_t totalSize = size.width * size.height;
  #if NA_DEBUG
    if(naIsLengthValueEmptys(totalSize) && !naIsLengthValueEmptys(size.width) && !naIsLengthValueEmptys(size.height))
      naError("You experience a rare occasion of integer overflow. Consider using naIsSizesEmptySlow");
  #endif
  return naIsLengthValueEmptys(totalSize);
}



NA_IDEF NABool naIsSizeiEmptySlow(NASizei size) {
  return (naIsLengthValueEmptyi(size.width) || naIsLengthValueEmptyi(size.height));
}
NA_IDEF NABool naIsSizei32EmptySlow(NASizei32 size) {
  return (naIsLengthValueEmptyi32(size.width) || naIsLengthValueEmptyi32(size.height));
}
NA_IDEF NABool naIsSizei64EmptySlow(NASizei64 size) {
  return (naIsLengthValueEmptyi64(size.width) || naIsLengthValueEmptyi64(size.height));
}
NA_IDEF NABool naIsSizesEmptySlow(NASizes size) {
  return (naIsLengthValueEmptys(size.width) || naIsLengthValueEmptys(size.height));
}



NA_IDEF NABool naIsRectEmpty (NARect rect) {
  return naIsSizeEmpty(rect.size);
}
NA_IDEF NABool naIsRectfEmpty (NARectf rect) {
  return naIsSizefEmpty(rect.size);
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
NA_IDEF NABool naIsRectsEmpty(NARects rect) {
  return naIsSizesEmpty(rect.size);
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
NA_IDEF NABool naIsRectsEmptySlow(NARects rect) {
  return naIsSizesEmptySlow(rect.size);
}



NA_IDEF NABool naIsVolumeEmpty (NAVolume volume) {
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmpty(volume.width) || naIsLengthValueEmpty(volume.height) || naIsLengthValueEmpty(volume.depth);
}
NA_IDEF NABool naIsVolumefEmpty (NAVolumef volume) {
  // Note that opposed to the integer variant, we always check the single
  // values rather than multiplying them. This is because a floating point
  // multiplication might lead to non-zero if one of the components is NaN.
  return naIsLengthValueEmptyf(volume.width) || naIsLengthValueEmptyf(volume.height) || naIsLengthValueEmptyf(volume.depth);
}
NA_IDEF NABool naIsVolumeiEmpty(NAVolumei volume) {
  NAInt totalvolume = volume.width * volume.height * volume.depth;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi(totalvolume) && !naIsLengthValueEmptyi(volume.width) && !naIsLengthValueEmptyi(volume.height) && !naIsLengthValueEmptyi(volume.depth))
      naError("You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi(totalvolume);
}
NA_IDEF NABool naIsVolumei32Empty(NAVolumei32 volume) {
  int32 totalvolume = volume.width * volume.height * volume.depth;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi32(totalvolume) && !naIsLengthValueEmptyi32(volume.width) && !naIsLengthValueEmptyi32(volume.height) && !naIsLengthValueEmptyi32(volume.depth))
      naError("You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi32(totalvolume);
}
NA_IDEF NABool naIsVolumei64Empty(NAVolumei64 volume) {
  int64 totalvolume = volume.width * volume.height * volume.depth;
  #if NA_DEBUG
    if(naIsLengthValueEmptyi64(totalvolume) && !naIsLengthValueEmptyi64(volume.width) && !naIsLengthValueEmptyi64(volume.height) && !naIsLengthValueEmptyi64(volume.depth))
      naError("You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
  #endif
  return naIsLengthValueEmptyi64(totalvolume);
}
NA_IDEF NABool naIsVolumesEmpty(NAVolumes volume) {
  size_t totalvolume = volume.width * volume.height * volume.depth;
  #if NA_DEBUG
    if(naIsLengthValueEmptys(totalvolume) && !naIsLengthValueEmptys(volume.width) && !naIsLengthValueEmptys(volume.height) && !naIsLengthValueEmptys(volume.depth))
      naError("You experience a rare occasion of integer overflow. Consider using naIsVolumeiEmptySlow");
  #endif
  return naIsLengthValueEmptys(totalvolume);
}



NA_IDEF NABool naIsVolumeiEmptySlow(NAVolumei volume) {
  return (naIsLengthValueEmptyi(volume.width) || naIsLengthValueEmptyi(volume.height) || naIsLengthValueEmptyi(volume.depth));
}
NA_IDEF NABool naIsVolumei32EmptySlow(NAVolumei32 volume) {
  return (naIsLengthValueEmptyi32(volume.width) || naIsLengthValueEmptyi32(volume.height) || naIsLengthValueEmptyi32(volume.depth));
}
NA_IDEF NABool naIsVolumei64EmptySlow(NAVolumei64 volume) {
  return (naIsLengthValueEmptyi64(volume.width) || naIsLengthValueEmptyi64(volume.height) || naIsLengthValueEmptyi64(volume.depth));
}
NA_IDEF NABool naIsVolumesEmptySlow(NAVolumes volume) {
  return (naIsLengthValueEmptys(volume.width) || naIsLengthValueEmptys(volume.height) || naIsLengthValueEmptys(volume.depth));
}



NA_IDEF NABool naIsBoxEmpty (NABox box) {
  return naIsVolumeEmpty(box.volume);
}
NA_IDEF NABool naIsBoxfEmpty (NABoxf box) {
  return naIsVolumefEmpty(box.volume);
}
NA_IDEF NABool naIsBoxiEmpty(NABoxi box) {
  return naIsVolumeiEmpty(box.volume);
}
NA_IDEF NABool naIsBoxi32Empty(NABoxi32 box) {
  return naIsVolumei32Empty(box.volume);
}
NA_IDEF NABool naIsBoxi64Empty(NABoxi64 box) {
  return naIsVolumei64Empty(box.volume);
}
NA_IDEF NABool naIsBoxsEmpty(NABoxs box) {
  return naIsVolumesEmpty(box.volume);
}



NA_IDEF NABool naIsBoxiEmptySlow(NABoxi box) {
  return naIsVolumeiEmptySlow(box.volume);
}
NA_IDEF NABool naIsBoxi32EmptySlow(NABoxi32 box) {
  return naIsVolumei32EmptySlow(box.volume);
}
NA_IDEF NABool naIsBoxi64EmptySlow(NABoxi64 box) {
  return naIsVolumei64EmptySlow(box.volume);
}
NA_IDEF NABool naIsBoxsEmptySlow(NABoxs box) {
  return naIsVolumesEmptySlow(box.volume);
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
NA_IDEF NABool naIsRangesNegative(NARanges range) {
  return naIsLengthValueNegatives(range.length);
}



NA_IDEF NABool naIsSizeNegative(NASize size) {
  return (naIsLengthValueNegative(size.width) || naIsLengthValueNegative(size.height));
}
NA_IDEF NABool naIsSizefNegative(NASizef size) {
  return (naIsLengthValueNegativef(size.width) || naIsLengthValueNegativef(size.height));
}
NA_IDEF NABool naIsSizeiNegative(NASizei size) {
  return (naIsLengthValueNegativei(size.width) || naIsLengthValueNegativei(size.height));
}
NA_IDEF NABool naIsSizei32Negative(NASizei32 size) {
  return (naIsLengthValueNegativei32(size.width) || naIsLengthValueNegativei32(size.height));
}
NA_IDEF NABool naIsSizei64Negative(NASizei64 size) {
  return (naIsLengthValueNegativei64(size.width) || naIsLengthValueNegativei64(size.height));
}
NA_IDEF NABool naIsSizesNegative(NASizes size) {
  return (naIsLengthValueNegatives(size.width) || naIsLengthValueNegatives(size.height));
}



NA_IDEF NABool naIsRectNegative(NARect rect) {
  return naIsSizeNegative(rect.size);
}
NA_IDEF NABool naIsRectfNegative(NARectf rect) {
  return naIsSizefNegative(rect.size);
}
NA_IDEF NABool naIsRectiNegative(NARecti rect) {
  return naIsSizeiNegative(rect.size);
}
NA_IDEF NABool naIsRecti32Negative(NARecti32 rect) {
  return naIsSizei32Negative(rect.size);
}
NA_IDEF NABool naIsRecti64Negative(NARecti64 rect) {
  return naIsSizei64Negative(rect.size);
}
NA_IDEF NABool naIsRectsNegative(NARects rect) {
  return naIsSizesNegative(rect.size);
}



NA_IDEF NABool naIsVolumeNegative(NAVolume volume) {
  return (naIsLengthValueNegative(volume.width) || naIsLengthValueNegative(volume.height) || naIsLengthValueNegative(volume.depth));
}
NA_IDEF NABool naIsVolumefNegative(NAVolumef volume) {
  return (naIsLengthValueNegativef(volume.width) || naIsLengthValueNegativef(volume.height) || naIsLengthValueNegativef(volume.depth));
}
NA_IDEF NABool naIsVolumeiNegative(NAVolumei volume) {
  return (naIsLengthValueNegativei(volume.width) || naIsLengthValueNegativei(volume.height) || naIsLengthValueNegativei(volume.depth));
}
NA_IDEF NABool naIsVolumei32Negative(NAVolumei32 volume) {
  return (naIsLengthValueNegativei32(volume.width) || naIsLengthValueNegativei32(volume.height) || naIsLengthValueNegativei32(volume.depth));
}
NA_IDEF NABool naIsVolumei64Negative(NAVolumei64 volume) {
  return (naIsLengthValueNegativei64(volume.width) || naIsLengthValueNegativei64(volume.height) || naIsLengthValueNegativei64(volume.depth));
}
NA_IDEF NABool naIsVolumesNegative(NAVolumes volume) {
  return (naIsLengthValueNegatives(volume.width) || naIsLengthValueNegatives(volume.height) || naIsLengthValueNegatives(volume.depth));
}



NA_IDEF NABool naIsBoxNegative(NABox box) {
  return naIsVolumeNegative(box.volume);
}
NA_IDEF NABool naIsBoxfNegative(NABoxf box) {
  return naIsVolumefNegative(box.volume);
}
NA_IDEF NABool naIsBoxiNegative(NABoxi box) {
  return naIsVolumeiNegative(box.volume);
}
NA_IDEF NABool naIsBoxi32Negative(NABoxi32 box) {
  return naIsVolumei32Negative(box.volume);
}
NA_IDEF NABool naIsBoxi64Negative(NABoxi64 box) {
  return naIsVolumei64Negative(box.volume);
}
NA_IDEF NABool naIsBoxsNegative(NABoxs box) {
  return naIsVolumesNegative(box.volume);
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
NA_IDEF NABool naIsRangesUseful(NARanges range) {
  return (naIsOffsetValueUsefuls(range.origin) && naIsLengthValueUsefuls(range.length));
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
NA_IDEF NABool naIsRectfUseful(NARectf rect) {
  return (naIsPosfUseful(rect.pos) && naIsSizefUseful(rect.size));
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
NA_IDEF NABool naIsRectsUseful(NARects rect) {
  return (naIsPossUseful(rect.pos) && naIsSizesUseful(rect.size));
}



NA_IDEF NABool naIsVertexUseful(NAVertex vertex) {
  return naIsOffsetValueUseful(vertex.x) && naIsOffsetValueUseful(vertex.y) && naIsOffsetValueUseful(vertex.x);
}
NA_IDEF NABool naIsVertexfUseful(NAVertexf vertex) {
  return naIsOffsetValueUsefulf(vertex.x) && naIsOffsetValueUsefulf(vertex.y) && naIsOffsetValueUsefulf(vertex.x);
}
NA_IDEF NABool naIsVertexiUseful(NAVertexi vertex) {
  return naIsOffsetValueUsefuli(vertex.x) && naIsOffsetValueUsefuli(vertex.y) && naIsOffsetValueUsefuli(vertex.x);
}
NA_IDEF NABool naIsVertexi32Useful(NAVertexi32 vertex) {
  return naIsOffsetValueUsefuli32(vertex.x) && naIsOffsetValueUsefuli32(vertex.y) && naIsOffsetValueUsefuli32(vertex.x);
}
NA_IDEF NABool naIsVertexi64Useful(NAVertexi64 vertex) {
  return naIsOffsetValueUsefuli64(vertex.x) && naIsOffsetValueUsefuli64(vertex.y) && naIsOffsetValueUsefuli64(vertex.x);
}
NA_IDEF NABool naIsVertexsUseful(NAVertexs vertex) {
  return naIsOffsetValueUsefuls(vertex.x) && naIsOffsetValueUsefuls(vertex.y) && naIsOffsetValueUsefuls(vertex.x);
}



NA_IDEF NABool naIsVolumeUseful(NAVolume volume) {
  return naIsLengthValueUseful(volume.width) && naIsLengthValueUseful(volume.height) && naIsLengthValueUseful(volume.depth);
}
NA_IDEF NABool naIsVolumefUseful(NAVolumef volume) {
  return naIsLengthValueUsefulf(volume.width) && naIsLengthValueUsefulf(volume.height) && naIsLengthValueUsefulf(volume.depth);
}
NA_IDEF NABool naIsVolumeiUseful(NAVolumei volume) {
  return naIsLengthValueUsefuli(volume.width) && naIsLengthValueUsefuli(volume.height) && naIsLengthValueUsefuli(volume.depth);
}
NA_IDEF NABool naIsVolumei32Useful(NAVolumei32 volume) {
  return naIsLengthValueUsefuli32(volume.width) && naIsLengthValueUsefuli32(volume.height) && naIsLengthValueUsefuli32(volume.depth);
}
NA_IDEF NABool naIsVolumei64Useful(NAVolumei64 volume) {
  return naIsLengthValueUsefuli64(volume.width) && naIsLengthValueUsefuli64(volume.height) && naIsLengthValueUsefuli64(volume.depth);
}
NA_IDEF NABool naIsVolumesUseful(NAVolumes volume) {
  return naIsLengthValueUsefuls(volume.width) && naIsLengthValueUsefuls(volume.height) && naIsLengthValueUsefuls(volume.depth);
}



NA_IDEF NABool naIsBoxUseful(NABox box) {
  return (naIsVertexUseful(box.vertex) && naIsVolumeUseful(box.volume));
}
NA_IDEF NABool naIsBoxfUseful(NABoxf box) {
  return (naIsVertexfUseful(box.vertex) && naIsVolumefUseful(box.volume));
}
NA_IDEF NABool naIsBoxiUseful(NABoxi box) {
  return (naIsVertexiUseful(box.vertex) && naIsVolumeiUseful(box.volume));
}
NA_IDEF NABool naIsBoxi32Useful(NABoxi32 box) {
  return (naIsVertexi32Useful(box.vertex) && naIsVolumei32Useful(box.volume));
}
NA_IDEF NABool naIsBoxi64Useful(NABoxi64 box) {
  return (naIsVertexi64Useful(box.vertex) && naIsVolumei64Useful(box.volume));
}
NA_IDEF NABool naIsBoxsUseful(NABoxs box) {
  return (naIsVertexsUseful(box.vertex) && naIsVolumesUseful(box.volume));
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
