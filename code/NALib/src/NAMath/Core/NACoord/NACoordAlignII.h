
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



NA_IDEF NAPos naMakePosWithAlignment(NAPos origin, NARect alignRect) {
  NAPos newOrigin;
  #if NA_DEBUG
    if(naIsRectEmpty(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  newOrigin.x = naAlignValue(origin.x, alignRect.pos.x, alignRect.size.width);
  newOrigin.y = naAlignValue(origin.y, alignRect.pos.y, alignRect.size.height);
  return newOrigin;
}
NA_IDEF NAPosf naMakePosfWithAlignment(NAPosf origin, NARectf alignRect) {
  NAPosf newOrigin;
  #if NA_DEBUG
    if(naIsRectfEmpty(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  newOrigin.x = naAlignValuef(origin.x, alignRect.pos.x, alignRect.size.width);
  newOrigin.y = naAlignValuef(origin.y, alignRect.pos.y, alignRect.size.height);
  return newOrigin;
}
NA_IDEF NAPosi32 naMakePosi32WithAlignment(NAPosi32 origin, NARecti32 alignRect) {
  NAPosi32 newOrigin;
  #if NA_DEBUG
    if(naIsRecti32EmptySlow(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  newOrigin.x = naAlignValuei32(origin.x, alignRect.pos.x, alignRect.size.width);
  newOrigin.y = naAlignValuei32(origin.y, alignRect.pos.y, alignRect.size.height);
  return newOrigin;
}
NA_IDEF NAPosi64 naMakePosi64WithAlignment(NAPosi64 origin, NARecti64 alignRect) {
  NAPosi64 newOrigin;
  #if NA_DEBUG
    if(naIsRecti64EmptySlow(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  newOrigin.x = naAlignValuei64(origin.x, alignRect.pos.x, alignRect.size.width);
  newOrigin.y = naAlignValuei64(origin.y, alignRect.pos.y, alignRect.size.height);
  return newOrigin;
}
NA_IDEF NAPoss naMakePossWithAlignment(NAPoss origin, NARects alignRect) {
  NAPoss newOrigin;
  #if NA_DEBUG
    if(naIsRectsEmptySlow(alignRect))
      naCrash("alignment rect is empty.");
  #endif
  newOrigin.x = naAlignValues(origin.x, alignRect.pos.x, alignRect.size.width);
  newOrigin.y = naAlignValues(origin.y, alignRect.pos.y, alignRect.size.height);
  return newOrigin;
}



NA_IDEF NAVertex naMakeVertexWithAlignment(NAVertex origin, NABox alignBox) {
  NAVertex newOrigin;
  #if NA_DEBUG
    if(naIsBoxEmpty(alignBox))
      naCrash("alignment box is empty.");
  #endif
  newOrigin.x = naAlignValue(origin.x, alignBox.vertex.x, alignBox.volume.width);
  newOrigin.y = naAlignValue(origin.y, alignBox.vertex.y, alignBox.volume.height);
  newOrigin.z = naAlignValue(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return newOrigin;
}
NA_IDEF NAVertexf naMakeVertexfWithAlignment(NAVertexf origin, NABoxf alignBox) {
  NAVertexf newOrigin;
  #if NA_DEBUG
    if(naIsBoxfEmpty(alignBox))
      naCrash("alignment box is empty.");
  #endif
  newOrigin.x = naAlignValuef(origin.x, alignBox.vertex.x, alignBox.volume.width);
  newOrigin.y = naAlignValuef(origin.y, alignBox.vertex.y, alignBox.volume.height);
  newOrigin.z = naAlignValuef(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return newOrigin;
}
NA_IDEF NAVertexi32 naMakeVertexi32WithAlignment(NAVertexi32 origin, NABoxi32 alignBox) {
  NAVertexi32 newOrigin;
  #if NA_DEBUG
    if(naIsBoxi32EmptySlow(alignBox))
      naCrash("alignment box is empty.");
  #endif
  newOrigin.x = naAlignValuei32(origin.x, alignBox.vertex.x, alignBox.volume.width);
  newOrigin.y = naAlignValuei32(origin.y, alignBox.vertex.y, alignBox.volume.height);
  newOrigin.z = naAlignValuei32(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return newOrigin;
}
NA_IDEF NAVertexi64 naMakeVertexi64WithAlignment(NAVertexi64 origin, NABoxi64 alignBox) {
  NAVertexi64 newOrigin;
  #if NA_DEBUG
    if(naIsBoxi64EmptySlow(alignBox))
      naCrash("alignment box is empty.");
  #endif
  newOrigin.x = naAlignValuei64(origin.x, alignBox.vertex.x, alignBox.volume.width);
  newOrigin.y = naAlignValuei64(origin.y, alignBox.vertex.y, alignBox.volume.height);
  newOrigin.z = naAlignValuei64(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return newOrigin;
}
NA_IDEF NAVertexs naMakeVertexsWithAlignment(NAVertexs origin, NABoxs alignBox) {
  NAVertexs newOrigin;
  #if NA_DEBUG
    if(naIsBoxsEmptySlow(alignBox))
      naCrash("alignment box is empty.");
  #endif
  newOrigin.x = naAlignValues(origin.x, alignBox.vertex.x, alignBox.volume.width);
  newOrigin.y = naAlignValues(origin.y, alignBox.vertex.y, alignBox.volume.height);
  newOrigin.z = naAlignValues(origin.z, alignBox.vertex.z, alignBox.volume.depth);
  return newOrigin;
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
