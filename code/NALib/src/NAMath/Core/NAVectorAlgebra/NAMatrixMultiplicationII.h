
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Multiply matrix with vector
// ///////////////////////////////

NA_IDEF void naMulM22dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v) {
  #if NA_DEBUG
    if(naElementOverlap(A, d, 4, 2))
      naError("A overlaps with d.");
    if(naElementOverlap(v, d, 2, 2))
      naError("v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[2]*v[1];
  d[1] = A[1]*v[0] + A[3]*v[1];
}
NA_IDEF void naMulM33dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v) {
  #if NA_DEBUG
    if(naElementOverlap(A, d, 9, 3))
      naError("A overlaps with d.");
    if(naElementOverlap(v, d, 3, 3))
      naError("v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[3]*v[1] + A[6]*v[2];
  d[1] = A[1]*v[0] + A[4]*v[1] + A[7]*v[2];
  d[2] = A[2]*v[0] + A[5]*v[1] + A[8]*v[2];
}
NA_IDEF void naMulM44dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v) {
  #if NA_DEBUG
    if(naElementOverlap(A, d, 16, 4))
      naError("A overlaps with d.");
    if(naElementOverlap(v, d, 4, 4))
      naError("v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[4]*v[1] + A[ 8]*v[2] + A[12]*v[3];
  d[1] = A[1]*v[0] + A[5]*v[1] + A[ 9]*v[2] + A[13]*v[3];
  d[2] = A[2]*v[0] + A[6]*v[1] + A[10]*v[2] + A[14]*v[3];
  d[3] = A[3]*v[0] + A[7]*v[1] + A[11]*v[2] + A[15]*v[3];
}
NA_IDEF void naMulM22fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v) {
  #if NA_DEBUG
    if(naElementOverlap(A, d, 4, 2))
      naError("A overlaps with d.");
    if(naElementOverlap(v, d, 2, 2))
      naError("v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[2]*v[1];
  d[1] = A[1]*v[0] + A[3]*v[1];
}
NA_IDEF void naMulM33fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v) {
  #if NA_DEBUG
    if(naElementOverlap(A, d, 9, 3))
      naError("A overlaps with d.");
    if(naElementOverlap(v, d, 3, 3))
      naError("v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[3]*v[1] + A[6]*v[2];
  d[1] = A[1]*v[0] + A[4]*v[1] + A[7]*v[2];
  d[2] = A[2]*v[0] + A[5]*v[1] + A[8]*v[2];
}
NA_IDEF void naMulM44fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v) {
  #if NA_DEBUG
    if(naElementOverlap(A, d, 16, 4))
      naError("A overlaps with d.");
    if(naElementOverlap(v, d, 4, 4))
      naError("v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[4]*v[1] + A[ 8]*v[2] + A[12]*v[3];
  d[1] = A[1]*v[0] + A[5]*v[1] + A[ 9]*v[2] + A[13]*v[3];
  d[2] = A[2]*v[0] + A[6]*v[1] + A[10]*v[2] + A[14]*v[3];
  d[3] = A[3]*v[0] + A[7]*v[1] + A[11]*v[2] + A[15]*v[3];
}


// ///////////////////////////////
// Multiply Matrix with Matrix
// ///////////////////////////////

NA_IDEF void naMulM22dM22d(double* NA_RESTRICT D, const double* A, const double* B) {
  #if NA_DEBUG
    if(naElementOverlap(D, A, 4, 4))
      naError("A overlaps with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[2]*B[1];
  D[1] = A[1]*B[0] + A[3]*B[1];
  D[2] = A[0]*B[2] + A[2]*B[3];
  D[3] = A[1]*B[2] + A[3]*B[3];
}
NA_IDEF void naMulM33dM33d(double* NA_RESTRICT D, const double* A, const double* B) {
  #if NA_DEBUG
    if(naElementOverlap(D, A, 9, 9))
      naError("A overlaps with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[3]*B[1] + A[6]*B[2];
  D[1] = A[1]*B[0] + A[4]*B[1] + A[7]*B[2];
  D[2] = A[2]*B[0] + A[5]*B[1] + A[8]*B[2];
  D[3] = A[0]*B[3] + A[3]*B[4] + A[6]*B[5];
  D[4] = A[1]*B[3] + A[4]*B[4] + A[7]*B[5];
  D[5] = A[2]*B[3] + A[5]*B[4] + A[8]*B[5];
  D[6] = A[0]*B[6] + A[3]*B[7] + A[6]*B[8];
  D[7] = A[1]*B[6] + A[4]*B[7] + A[7]*B[8];
  D[8] = A[2]*B[6] + A[5]*B[7] + A[8]*B[8];
}
NA_IDEF void naMulM44dM44d(double* NA_RESTRICT D, const double* A, const double* B) {
  #if NA_DEBUG
    if(naElementOverlap(D, A, 16, 16))
      naError("A overlaps with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[ 0] = A[0]*B[ 0] + A[4]*B[ 1] + A[ 8]*B[ 2] + A[12]*B[ 3];
  D[ 1] = A[1]*B[ 0] + A[5]*B[ 1] + A[ 9]*B[ 2] + A[13]*B[ 3];
  D[ 2] = A[2]*B[ 0] + A[6]*B[ 1] + A[10]*B[ 2] + A[14]*B[ 3];
  D[ 3] = A[3]*B[ 0] + A[7]*B[ 1] + A[11]*B[ 2] + A[15]*B[ 3];
  D[ 4] = A[0]*B[ 4] + A[4]*B[ 5] + A[ 8]*B[ 6] + A[12]*B[ 7];
  D[ 5] = A[1]*B[ 4] + A[5]*B[ 5] + A[ 9]*B[ 6] + A[13]*B[ 7];
  D[ 6] = A[2]*B[ 4] + A[6]*B[ 5] + A[10]*B[ 6] + A[14]*B[ 7];
  D[ 7] = A[3]*B[ 4] + A[7]*B[ 5] + A[11]*B[ 6] + A[15]*B[ 7];
  D[ 8] = A[0]*B[ 8] + A[4]*B[ 9] + A[ 8]*B[10] + A[12]*B[11];
  D[ 9] = A[1]*B[ 8] + A[5]*B[ 9] + A[ 9]*B[10] + A[13]*B[11];
  D[10] = A[2]*B[ 8] + A[6]*B[ 9] + A[10]*B[10] + A[14]*B[11];
  D[11] = A[3]*B[ 8] + A[7]*B[ 9] + A[11]*B[10] + A[15]*B[11];
  D[12] = A[0]*B[12] + A[4]*B[13] + A[ 8]*B[14] + A[12]*B[15];
  D[13] = A[1]*B[12] + A[5]*B[13] + A[ 9]*B[14] + A[13]*B[15];
  D[14] = A[2]*B[12] + A[6]*B[13] + A[10]*B[14] + A[14]*B[15];
  D[15] = A[3]*B[12] + A[7]*B[13] + A[11]*B[14] + A[15]*B[15];
}
NA_IDEF void naMulM22fM22f(float* NA_RESTRICT D, const float* A, const float* B) {
  #if NA_DEBUG
    if(naElementOverlap(D, A, 4, 4))
      naError("A overlaps with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[2]*B[1];
  D[1] = A[1]*B[0] + A[3]*B[1];
  D[2] = A[0]*B[2] + A[2]*B[3];
  D[3] = A[1]*B[2] + A[3]*B[3];
}
NA_IDEF void naMulM33fM33f(float* NA_RESTRICT D, const float* A, const float* B) {
  #if NA_DEBUG
    if(naElementOverlap(D, A, 9, 9))
      naError("A overlaps with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[3]*B[1] + A[6]*B[2];
  D[1] = A[1]*B[0] + A[4]*B[1] + A[7]*B[2];
  D[2] = A[2]*B[0] + A[5]*B[1] + A[8]*B[2];
  D[3] = A[0]*B[3] + A[3]*B[4] + A[6]*B[5];
  D[4] = A[1]*B[3] + A[4]*B[4] + A[7]*B[5];
  D[5] = A[2]*B[3] + A[5]*B[4] + A[8]*B[5];
  D[6] = A[0]*B[6] + A[3]*B[7] + A[6]*B[8];
  D[7] = A[1]*B[6] + A[4]*B[7] + A[7]*B[8];
  D[8] = A[2]*B[6] + A[5]*B[7] + A[8]*B[8];
}
NA_IDEF void naMulM44fM44f(float* NA_RESTRICT D, const float* A, const float* B) {
  #if NA_DEBUG
    if(naElementOverlap(D, A, 16, 16))
      naError("A overlaps with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[ 0] = A[0]*B[ 0] + A[4]*B[ 1] + A[ 8]*B[ 2] + A[12]*B[ 3];
  D[ 1] = A[1]*B[ 0] + A[5]*B[ 1] + A[ 9]*B[ 2] + A[13]*B[ 3];
  D[ 2] = A[2]*B[ 0] + A[6]*B[ 1] + A[10]*B[ 2] + A[14]*B[ 3];
  D[ 3] = A[3]*B[ 0] + A[7]*B[ 1] + A[11]*B[ 2] + A[15]*B[ 3];
  D[ 4] = A[0]*B[ 4] + A[4]*B[ 5] + A[ 8]*B[ 6] + A[12]*B[ 7];
  D[ 5] = A[1]*B[ 4] + A[5]*B[ 5] + A[ 9]*B[ 6] + A[13]*B[ 7];
  D[ 6] = A[2]*B[ 4] + A[6]*B[ 5] + A[10]*B[ 6] + A[14]*B[ 7];
  D[ 7] = A[3]*B[ 4] + A[7]*B[ 5] + A[11]*B[ 6] + A[15]*B[ 7];
  D[ 8] = A[0]*B[ 8] + A[4]*B[ 9] + A[ 8]*B[10] + A[12]*B[11];
  D[ 9] = A[1]*B[ 8] + A[5]*B[ 9] + A[ 9]*B[10] + A[13]*B[11];
  D[10] = A[2]*B[ 8] + A[6]*B[ 9] + A[10]*B[10] + A[14]*B[11];
  D[11] = A[3]*B[ 8] + A[7]*B[ 9] + A[11]*B[10] + A[15]*B[11];
  D[12] = A[0]*B[12] + A[4]*B[13] + A[ 8]*B[14] + A[12]*B[15];
  D[13] = A[1]*B[12] + A[5]*B[13] + A[ 9]*B[14] + A[13]*B[15];
  D[14] = A[2]*B[12] + A[6]*B[13] + A[10]*B[14] + A[14]*B[15];
  D[15] = A[3]*B[12] + A[7]*B[13] + A[11]*B[14] + A[15]*B[15];
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
