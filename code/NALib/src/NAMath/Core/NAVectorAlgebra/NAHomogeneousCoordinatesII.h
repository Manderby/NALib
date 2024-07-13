
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Homogeneous Coordinates
// ///////////////////////////////

NA_IDEF void naScaleM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2) {
  D[0] = A[0] * s1;
  D[1] = A[1] * s1;
  D[2] = A[2] * s1;
  D[3] = A[3] * s2;
  D[4] = A[4] * s2;
  D[5] = A[5] * s2;
  D[6] = A[6];
  D[7] = A[7];
  D[8] = A[8];
}
NA_IDEF void naScaleM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2, double s3) {
  D[ 0] = A[ 0] * s1;
  D[ 1] = A[ 1] * s1;
  D[ 2] = A[ 2] * s1;
  D[ 3] = A[ 3] * s1;
  D[ 4] = A[ 4] * s2;
  D[ 5] = A[ 5] * s2;
  D[ 6] = A[ 6] * s2;
  D[ 7] = A[ 7] * s2;
  D[ 8] = A[ 8] * s3;
  D[ 9] = A[ 9] * s3;
  D[10] = A[10] * s3;
  D[11] = A[11] * s3;
  D[12] = A[12];
  D[13] = A[13];
  D[14] = A[14];
  D[15] = A[15];
}
NA_IDEF void naScaleM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2) {
  D[0] = A[0] * s1;
  D[1] = A[1] * s1;
  D[2] = A[2] * s1;
  D[3] = A[3] * s2;
  D[4] = A[4] * s2;
  D[5] = A[5] * s2;
  D[6] = A[6];
  D[7] = A[7];
  D[8] = A[8];
}
NA_IDEF void naScaleM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2, float s3) {
  D[ 0] = A[ 0] * s1;
  D[ 1] = A[ 1] * s1;
  D[ 2] = A[ 2] * s1;
  D[ 3] = A[ 3] * s1;
  D[ 4] = A[ 4] * s2;
  D[ 5] = A[ 5] * s2;
  D[ 6] = A[ 6] * s2;
  D[ 7] = A[ 7] * s2;
  D[ 8] = A[ 8] * s3;
  D[ 9] = A[ 9] * s3;
  D[10] = A[10] * s3;
  D[11] = A[11] * s3;
  D[12] = A[12];
  D[13] = A[13];
  D[14] = A[14];
  D[15] = A[15];
}



NA_IDEF void naScaleM33dS(double* D, double s1, double s2) {
  D[0] *= s1;
  D[1] *= s1;
  D[2] *= s1;
  D[3] *= s2;
  D[4] *= s2;
  D[5] *= s2;
}
NA_IDEF void naScaleM44dS(double* D, double s1, double s2, double s3) {
  D[ 0] *= s1;
  D[ 1] *= s1;
  D[ 2] *= s1;
  D[ 3] *= s1;
  D[ 4] *= s2;
  D[ 5] *= s2;
  D[ 6] *= s2;
  D[ 7] *= s2;
  D[ 8] *= s3;
  D[ 9] *= s3;
  D[10] *= s3;
  D[11] *= s3;
}
NA_IDEF void naScaleM33fS(float* D, float s1, float s2) {
  D[0] *= s1;
  D[1] *= s1;
  D[2] *= s1;
  D[3] *= s2;
  D[4] *= s2;
  D[5] *= s2;
}
NA_IDEF void naScaleM44fS(float* D, float s1, float s2, float s3) {
  D[ 0] *= s1;
  D[ 1] *= s1;
  D[ 2] *= s1;
  D[ 3] *= s1;
  D[ 4] *= s2;
  D[ 5] *= s2;
  D[ 6] *= s2;
  D[ 7] *= s2;
  D[ 8] *= s3;
  D[ 9] *= s3;
  D[10] *= s3;
  D[11] *= s3;
}



NA_IDEF void naRotateM44ZfS(float* D, float ang) {
  float cosAng = naCosf(ang);
  float sinAng = naSinf(ang);
  float d0  = cosAng * D[ 0] - sinAng * D[ 1];
  float d1  = sinAng * D[ 0] + cosAng * D[ 1];
  float d4  = cosAng * D[ 4] - sinAng * D[ 5];
  float d5  = sinAng * D[ 4] + cosAng * D[ 5];
  float d8  = cosAng * D[ 8] - sinAng * D[ 9];
  float d9  = sinAng * D[ 8] + cosAng * D[ 9];
  float d12 = cosAng * D[12] - sinAng * D[13];
  float d13 = sinAng * D[12] + cosAng * D[13];
  D[ 0] = d0;
  D[ 1] = d1;
  D[ 4] = d4;
  D[ 5] = d5;
  D[ 8] = d8;
  D[ 9] = d9;
  D[12] = d12;
  D[13] = d13;
}



NA_IDEF void naTranslateM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2) {
  D[0] = A[0];
  D[1] = A[1];
  D[2] = A[2];
  D[3] = A[3];
  D[4] = A[4];
  D[5] = A[5];
  D[6] = A[0] * s1 + A[3] * s2 + A[6];
  D[7] = A[1] * s1 + A[4] * s2 + A[7];
  D[8] = A[2] * s1 + A[5] * s2 + A[8];
}
NA_IDEF void naTranslateM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2, double s3) {
  D[ 0] = A[ 0];
  D[ 1] = A[ 1];
  D[ 2] = A[ 2];
  D[ 3] = A[ 3];
  D[ 4] = A[ 4];
  D[ 5] = A[ 5];
  D[ 6] = A[ 6];
  D[ 7] = A[ 7];
  D[ 8] = A[ 8];
  D[ 9] = A[ 9];
  D[10] = A[10];
  D[11] = A[11];
  D[12] = A[0] * s1 + A[4] * s2 + A[ 8] * s3 + A[12];
  D[13] = A[1] * s1 + A[5] * s2 + A[ 9] * s3 + A[13];
  D[14] = A[2] * s1 + A[6] * s2 + A[10] * s3 + A[14];
  D[15] = A[3] * s1 + A[7] * s2 + A[11] * s3 + A[15];
}
NA_IDEF void naTranslateM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2) {
  D[0] = A[0];
  D[1] = A[1];
  D[2] = A[2];
  D[3] = A[3];
  D[4] = A[4];
  D[5] = A[5];
  D[6] = A[0] * s1 + A[3] * s2 + A[6];
  D[7] = A[1] * s1 + A[4] * s2 + A[7];
  D[8] = A[2] * s1 + A[5] * s2 + A[8];
}
NA_IDEF void naTranslateM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2, float s3) {
  D[ 0] = A[ 0];
  D[ 1] = A[ 1];
  D[ 2] = A[ 2];
  D[ 3] = A[ 3];
  D[ 4] = A[ 4];
  D[ 5] = A[ 5];
  D[ 6] = A[ 6];
  D[ 7] = A[ 7];
  D[ 8] = A[ 8];
  D[ 9] = A[ 9];
  D[10] = A[10];
  D[11] = A[11];
  D[12] = A[0] * s1 + A[4] * s2 + A[ 8] * s3 + A[12];
  D[13] = A[1] * s1 + A[5] * s2 + A[ 9] * s3 + A[13];
  D[14] = A[2] * s1 + A[6] * s2 + A[10] * s3 + A[14];
  D[15] = A[3] * s1 + A[7] * s2 + A[11] * s3 + A[15];
}



NA_IDEF void naTranslateM33dS(double* D, double s1, double s2) {
  D[6] += D[0] * s1 + D[3] * s2;
  D[7] += D[1] * s1 + D[4] * s2;
  D[8] += D[2] * s1 + D[5] * s2;
}
NA_IDEF void naTranslateM44dS(double* D, double s1, double s2, double s3) {
  D[12] += D[0] * s1 + D[4] * s2 + D[ 8] * s3;
  D[13] += D[1] * s1 + D[5] * s2 + D[ 9] * s3;
  D[14] += D[2] * s1 + D[6] * s2 + D[10] * s3;
  D[15] += D[3] * s1 + D[7] * s2 + D[11] * s3;
}
NA_IDEF void naTranslateM33fS(float* D, float s1, float s2) {
  D[6] += D[0] * s1 + D[3] * s2;
  D[7] += D[1] * s1 + D[4] * s2;
  D[8] += D[2] * s1 + D[5] * s2;
}
NA_IDEF void naTranslateM44fS(float* D, float s1, float s2, float s3) {
  D[12] += D[0] * s1 + D[4] * s2 + D[ 8] * s3;
  D[13] += D[1] * s1 + D[5] * s2 + D[ 9] * s3;
  D[14] += D[2] * s1 + D[6] * s2 + D[10] * s3;
  D[15] += D[3] * s1 + D[7] * s2 + D[11] * s3;
}



NA_IDEF NAPos naGetMat33dTranslation(const double* A) {
  return naMakePos(A[6], A[7]);
}



NA_IDEF double naGetMat33dMagnification(const double* A) {
  double firstTerm = (A[0] + A[4]) * 0.5;
  return naSqrt(firstTerm * firstTerm - (A[0] * A[4] - A[1] * A[3]) * 0.5);
}



NA_IDEF void naFillV2WithM33Origin(double* NA_RESTRICT d, const double* NA_RESTRICT A) {
  d[0] = A[6] / A[8];
  d[1] = A[7] / A[8];
}

NA_IDEF void naFillV2WithM33Coord2(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v) {
  NAVec3d result;
  NAVec3d coord;
  coord[0] = v[0];
  coord[1] = v[1];
  coord[2] = 1.;
  naMulM33dV3d(result, A, coord);
  d[0] = result[0] / result[2];
  d[1] = result[1] / result[2];
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
