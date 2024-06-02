
// This file contains inline implementations of the file NACoord.h
// Do not include this file directly! It will automatically be included when
// including "NACoord.h"



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

NA_IDEF NAVertexf naMakeVertexf(float x, float y, float z) {
  NAVertexf newVertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValidf(x) && naIsOffsetValueValidf(y) && naIsOffsetValueValidf(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefulf(x) && naIsOffsetValueUsefulf(y) && naIsOffsetValueUsefulf(z)))
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

NA_IDEF NAVertexs naMakeVertexs(size_t x, size_t y, size_t z) {
  NAVertexs newVertex;
  #if NA_DEBUG
    if(!(naIsOffsetValueValids(x) && naIsOffsetValueValids(y) && naIsOffsetValueValids(z)))
      naError("Invalid values given.");
    if(!(naIsOffsetValueUsefuls(x) && naIsOffsetValueUsefuls(y) && naIsOffsetValueUsefuls(z)))
      naError("Values given are not useful.");
  #endif
  newVertex.x = x;
  newVertex.y = y;
  newVertex.z = z;
  return newVertex;
}



NA_IDEF NAVertex naMakeVertexZero(void) {
  NAVertex newVertex;
  newVertex.x = 0.;
  newVertex.y = 0.;
  newVertex.z = 0.;
  return newVertex;
}

NA_IDEF NAVertexf naMakeVertexfZero(void) {
  NAVertexf newVertex;
  newVertex.x = 0.f;
  newVertex.y = 0.f;
  newVertex.z = 0.f;
  return newVertex;
}

NA_IDEF NAVertexi32 naMakeVertexi32Zero(void) {
  NAVertexi32 newVertex;
  newVertex.x = 0;
  newVertex.y = 0;
  newVertex.z = 0;
  return newVertex;
}

NA_IDEF NAVertexi64 naMakeVertexi64Zero(void) {
  NAVertexi64 newVertex;
  newVertex.x = 0;
  newVertex.y = 0;
  newVertex.z = 0;
  return newVertex;
}

NA_IDEF NAVertexs naMakeVertexsZero(void) {
  NAVertexs newVertex;
  newVertex.x = 0;
  newVertex.y = 0;
  newVertex.z = 0;
  return newVertex;
}



//NA_IDEF NAVertex naMakeVertexWithV3(const double* v) {
//  NAVertex newVertex;
//  #if NA_DEBUG
//    if(!(naIsOffsetValueValid(v[0]) && naIsOffsetValueValid(v[1]) && naIsOffsetValueValid(v[2])))
//      naError("Invalid values given.");
//    if(!(naIsOffsetValueUseful(v[0]) && naIsOffsetValueUseful(v[1]) && naIsOffsetValueUseful(v[2])))
//      naError("Values given are not useful.");
//  #endif
//  newVertex.x = v[0];
//  newVertex.y = v[1];
//  newVertex.z = v[2];
//  return newVertex;
//}



NA_IDEF void naFillVertexWithCopy(NAVertex* vertexd, const NAVertex* vertexs) {
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naFillVertexfWithCopy(NAVertexf* vertexd, const NAVertexf* vertexs) {
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naFillVertexi32WithCopy(NAVertexi32* vertexd, const NAVertexi32* vertexs) {
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naFillVertexi64WithCopy(NAVertexi64* vertexd, const NAVertexi64* vertexs) {
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
}
NA_IDEF void naFillVertexsWithCopy(NAVertexs* vertexd, const NAVertexs* vertexs) {
  vertexd->x = vertexs->x;
  vertexd->y = vertexs->y;
  vertexd->z = vertexs->z;
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
