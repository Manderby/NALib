
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Homogeneous Coordinates
// ///////////////////////////////

NA_IDEF void naScaleM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2){
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



NA_IDEF void naTranslateM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2){
  D[0] = A[0];
  D[1] = A[1];
  D[2] = A[2];
  D[3] = A[3];
  D[4] = A[4];
  D[5] = A[5];
  D[6] = A[0] * s1 + A[3] * s2 + A[6];
  D[7] = A[1] * s1 + A[4] * s2 + A[7];
  D[8] = A[2] * s1 + A[5] * s2 + A[8];
//  D[0] = A[0] + A[2] * s1;
//  D[1] = A[1] + A[2] * s2;
//  D[2] = A[2] ;
//  D[3] = A[3] + A[5] * s1;
//  D[4] = A[4] + A[5] * s2;
//  D[5] = A[5] ;
//  D[6] = A[6] + A[8] * s1;
//  D[7] = A[7] + A[8] * s2;
//  D[8] = A[8] ;
}



NA_IDEF void naFillV2WithM33Origin(double* NA_RESTRICT d, const double* NA_RESTRICT A){
  d[0] = A[6] / A[8];
  d[1] = A[7] / A[8];
}

NA_IDEF void naFillV2WithM33Coord2(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
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
