
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Dot
// ///////////////////////////////

NA_IDEF double naDotV1d(const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 1, 1))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF double naDotV2d(const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 2, 2))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF double naDotV3d(const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 3, 3))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF double naDotV4d(const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 4, 4))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF float naDotV1f(const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 1, 1))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF float naDotV2f(const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 2, 2))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF float naDotV3f(const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 3, 3))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF float naDotV4f(const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 4, 4))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF double naDotV1dE(const double* a, const double* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 1, 1)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF double naDotV2dE(const double* a, const double* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 2, 2)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF double naDotV3dE(const double* a, const double* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 3, 3)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF double naDotV4dE(const double* a, const double* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 4, 4)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF float naDotV1fE(const float* a, const float* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 1, 1)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF float naDotV2fE(const float* a, const float* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 2, 2)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF float naDotV3fE(const float* a, const float* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 3, 3)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF float naDotV4fE(const float* a, const float* b) {
  #if NA_DEBUG
    if((a != b) && (naElementOverlap(a, b, 4, 4)))
      naError("b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF double naDotV1dS(const double* v) {
  return v[0] * v[0];
}
NA_IDEF double naDotV2dS(const double* v) {
  return v[0] * v[0] + v[1] * v[1];
}
NA_IDEF double naDotV3dS(const double* v) {
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
NA_IDEF double naDotV4dS(const double* v) {
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}
NA_IDEF float naDotV1fS(const float* v) {
  return v[0] * v[0];
}
NA_IDEF float naDotV2fS(const float* v) {
  return v[0] * v[0] + v[1] * v[1];
}
NA_IDEF float naDotV3fS(const float* v) {
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
NA_IDEF float naDotV4fS(const float* v) {
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}



// ///////////////////////////////
// Cross
// ///////////////////////////////

NA_IDEF void naCrossV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == d)
      naError("a and d are equal.");
    if(b == d)
      naError("b and d are equal.");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d");
    else if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
}
NA_IDEF void naCrossV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a == d)
      naError("a and d are equal.");
    if(b == d)
      naError("b and d are equal.");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d");
    else if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
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
