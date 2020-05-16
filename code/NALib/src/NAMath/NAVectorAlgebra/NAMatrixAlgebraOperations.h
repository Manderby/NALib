
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Determinants
// ///////////////////////////////


NA_IDEF double naDetM22d(double* A){
  double determinant = A[0]*A[3] - A[2]*A[1];
  return determinant;
}
NA_IDEF double naDetM33d(double* A){
  double d0 = A[4] * A[8] - A[5] * A[7];
  double d1 = A[2] * A[7] - A[1] * A[8];
  double d2 = A[1] * A[5] - A[2] * A[4];
  double determinant = A[0]*d0 + A[3]*d1 + A[6]*d2;
  return determinant;
}
NA_IDEF double naDetM44d(double* A){
  double a01 = A[10]*A[15] - A[11]*A[14];
  double a02 = A[ 6]*A[15] - A[ 7]*A[14];
  double a03 = A[ 6]*A[11] - A[ 7]*A[10];
  double a04 = A[ 2]*A[15] - A[ 3]*A[14];
  double a05 = A[ 2]*A[11] - A[ 3]*A[10];
  double a06 = A[ 2]*A[ 7] - A[ 3]*A[ 6];
  double d0 = + A[ 5]*a01 - A[ 9]*a02 + A[13]*a03;
  double d1 = - A[ 1]*a01 + A[ 9]*a04 - A[13]*a05;
  double d2 = + A[ 1]*a02 - A[ 5]*a04 + A[13]*a06;
  double d3 = - A[ 1]*a03 + A[ 5]*a05 - A[ 9]*a06;
  double determinant = A[ 0]*d0 + A[ 4]*d1 + A[ 8]*d2 + A[12]*d3;
  return determinant;
}



// ///////////////////////////////
// Transpose
// ///////////////////////////////

NA_IDEF void naTransposeM22d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant.");
    else if(naElementOverlap(D, A, 4, 4))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant.");
    else if(naElementOverlap(D, A, 9, 9))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant.");
    else if(naElementOverlap(D, A, 16, 16))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22f( float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant.");
    else if(naElementOverlap(D, A, 4, 4))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33f( float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant.");
    else if(naElementOverlap(D, A, 9, 9))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44f( float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant.");
    else if(naElementOverlap(D, A, 16, 16))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(D, A, 4, 4))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(D, A, 9, 9))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(D, A, 16, 16))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(D, A, 4, 4))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(D, A, 9, 9))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(D, A, 16, 16))
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22dS(double* D){
  naSwap64(&(D[1]),  &(D[2]));
}
NA_IDEF void naTransposeM33dS(double* D){
  naSwap64(&(D[1]),  &(D[3]));
  naSwap64(&(D[2]),  &(D[6]));
  naSwap64(&(D[5]),  &(D[7]));
}
NA_IDEF void naTransposeM44dS(double* D){
  naSwap64(&(D[1]),  &(D[4]));
  naSwap64(&(D[2]),  &(D[8]));
  naSwap64(&(D[3]),  &(D[12]));
  naSwap64(&(D[6]),  &(D[9]));
  naSwap64(&(D[7]),  &(D[13]));
  naSwap64(&(D[11]), &(D[14]));
}
NA_IDEF void naTransposeM22fS(float* D){
  naSwap32(&(D[1]),  &(D[2]));
}
NA_IDEF void naTransposeM33fS(float* D){
  naSwap32(&(D[1]),  &(D[3]));
  naSwap32(&(D[2]),  &(D[6]));
  naSwap32(&(D[5]),  &(D[7]));
}
NA_IDEF void naTransposeM44fS(float* D){
  naSwap32(&(D[1]),  &(D[4]));
  naSwap32(&(D[2]),  &(D[8]));
  naSwap32(&(D[3]),  &(D[12]));
  naSwap32(&(D[6]),  &(D[9]));
  naSwap32(&(D[7]),  &(D[13]));
  naSwap32(&(D[11]), &(D[14]));
}



// //////////////////////////////////////
// Inverting
// //////////////////////////////////////

NA_IDEF void naInvM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
  double determinant = A[0]*A[3] - A[2]*A[1];
  double divisor = naInv(determinant);
  D[0] =    A[3] * divisor;
  D[1] =  - A[1] * divisor;
  D[2] =  - A[2] * divisor;
  D[3] =    A[0] * divisor;
}
NA_IDEF void naInvM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
  double d0 = A[4] * A[8] - A[5] * A[7];
  double d1 = A[2] * A[7] - A[1] * A[8];
  double d2 = A[1] * A[5] - A[2] * A[4];
  double determinant = A[0]*d0 + A[3]*d1 + A[6]*d2;
  double divisor = naInv(determinant);
  D[0] = d0 * divisor;
  D[1] = d1 * divisor;
  D[2] = d2 * divisor;
  D[3] = (A[5]*A[6] - A[3]*A[8]) * divisor;
  D[4] = (A[0]*A[8] - A[2]*A[6]) * divisor;
  D[5] = (A[2]*A[3] - A[0]*A[5]) * divisor;
  D[6] = (A[3]*A[7] - A[4]*A[6]) * divisor;
  D[7] = (A[1]*A[6] - A[0]*A[7]) * divisor;
  D[8] = (A[0]*A[4] - A[1]*A[3]) * divisor;
}
NA_IDEF void naInvM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
  double a01 = A[10]*A[15] - A[11]*A[14];
  double a02 = A[ 6]*A[15] - A[ 7]*A[14];
  double a03 = A[ 6]*A[11] - A[ 7]*A[10];
  double a04 = A[ 2]*A[15] - A[ 3]*A[14];
  double a05 = A[ 2]*A[11] - A[ 3]*A[10];
  double a06 = A[ 2]*A[ 7] - A[ 3]*A[ 6];
  double d0 = + A[ 5]*a01 - A[ 9]*a02 + A[13]*a03;
  double d1 = - A[ 1]*a01 + A[ 9]*a04 - A[13]*a05;
  double d2 = + A[ 1]*a02 - A[ 5]*a04 + A[13]*a06;
  double d3 = - A[ 1]*a03 + A[ 5]*a05 - A[ 9]*a06;
  double determinant = A[ 0]*d0 + A[ 4]*d1 + A[ 8]*d2 + A[12]*d3;
  double divisor = naInv(determinant);
  double a07 = A[ 9]*A[15] - A[11]*A[13];
  double a08 = A[ 5]*A[15] - A[ 7]*A[13];
  double a09 = A[ 5]*A[11] - A[ 7]*A[ 9];
  double a10 = A[ 1]*A[15] - A[ 3]*A[13];
  double a11 = A[ 1]*A[11] - A[ 3]*A[ 9];
  double a12 = A[ 1]*A[ 7] - A[ 3]*A[ 5];
  double a13 = A[ 9]*A[14] - A[10]*A[13];
  double a14 = A[ 5]*A[14] - A[ 6]*A[13];
  double a15 = A[ 5]*A[10] - A[ 6]*A[ 9];
  double a16 = A[ 1]*A[14] - A[ 2]*A[13];
  double a17 = A[ 1]*A[10] - A[ 2]*A[ 9];
  double a18 = A[ 1]*A[ 6] - A[ 2]*A[ 5];
  D[ 0] = d0 * divisor;
  D[ 1] = d1 * divisor;
  D[ 2] = d2 * divisor;
  D[ 3] = d3 * divisor;
  D[ 4] = ( - A[ 4]*a01 + A[ 8]*a02 - A[12]*a03) * divisor;
  D[ 5] = ( + A[ 0]*a01 - A[ 8]*a04 + A[12]*a05) * divisor;
  D[ 6] = ( - A[ 0]*a02 + A[ 4]*a04 - A[12]*a06) * divisor;
  D[ 7] = ( + A[ 0]*a03 - A[ 4]*a05 + A[ 8]*a06) * divisor;
  D[ 8] = ( + A[ 4]*a07 - A[ 8]*a08 + A[12]*a09) * divisor;
  D[ 9] = ( - A[ 0]*a07 + A[ 8]*a10 - A[12]*a11) * divisor;
  D[10] = ( + A[ 0]*a08 - A[ 4]*a10 + A[12]*a12) * divisor;
  D[11] = ( - A[ 0]*a09 + A[ 4]*a11 - A[ 8]*a12) * divisor;
  D[12] = ( - A[ 4]*a13 + A[ 8]*a14 - A[12]*a15) * divisor;
  D[13] = ( + A[ 0]*a13 - A[ 8]*a16 + A[12]*a17) * divisor;
  D[14] = ( - A[ 0]*a14 + A[ 4]*a16 - A[12]*a18) * divisor;
  D[15] = ( + A[ 0]*a15 - A[ 4]*a17 + A[ 8]*a18) * divisor;
}
NA_IDEF void naInvM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
  float determinant = A[0]*A[3] - A[2]*A[1];
  float divisor = naInvf(determinant);
  D[0] =    A[3] * divisor;
  D[1] =  - A[1] * divisor;
  D[2] =  - A[2] * divisor;
  D[3] =    A[0] * divisor;
}
NA_IDEF void naInvM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
  float d0 = A[4] * A[8] - A[5] * A[7];
  float d1 = A[2] * A[7] - A[1] * A[8];
  float d2 = A[1] * A[5] - A[2] * A[4];
  float determinant = A[0]*d0 + A[3]*d1 + A[6]*d2;
  float divisor = naInvf(determinant);
  D[0] = d0 * divisor;
  D[1] = d1 * divisor;
  D[2] = d2 * divisor;
  D[3] = (A[5]*A[6] - A[3]*A[8]) * divisor;
  D[4] = (A[0]*A[8] - A[2]*A[6]) * divisor;
  D[5] = (A[2]*A[3] - A[0]*A[5]) * divisor;
  D[6] = (A[3]*A[7] - A[4]*A[6]) * divisor;
  D[7] = (A[1]*A[6] - A[0]*A[7]) * divisor;
  D[8] = (A[0]*A[4] - A[1]*A[3]) * divisor;
}
NA_IDEF void naInvM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
  float a01 = A[10]*A[15] - A[11]*A[14];
  float a02 = A[ 6]*A[15] - A[ 7]*A[14];
  float a03 = A[ 6]*A[11] - A[ 7]*A[10];
  float a04 = A[ 2]*A[15] - A[ 3]*A[14];
  float a05 = A[ 2]*A[11] - A[ 3]*A[10];
  float a06 = A[ 2]*A[ 7] - A[ 3]*A[ 6];
  float d0 = + A[ 5]*a01 - A[ 9]*a02 + A[13]*a03;
  float d1 = - A[ 1]*a01 + A[ 9]*a04 - A[13]*a05;
  float d2 = + A[ 1]*a02 - A[ 5]*a04 + A[13]*a06;
  float d3 = - A[ 1]*a03 + A[ 5]*a05 - A[ 9]*a06;
  float determinant = A[ 0]*d0 + A[ 4]*d1 + A[ 8]*d2 + A[12]*d3;
  float divisor = naInvf(determinant);
  float a07 = A[ 9]*A[15] - A[11]*A[13];
  float a08 = A[ 5]*A[15] - A[ 7]*A[13];
  float a09 = A[ 5]*A[11] - A[ 7]*A[ 9];
  float a10 = A[ 1]*A[15] - A[ 3]*A[13];
  float a11 = A[ 1]*A[11] - A[ 3]*A[ 9];
  float a12 = A[ 1]*A[ 7] - A[ 3]*A[ 5];
  float a13 = A[ 9]*A[14] - A[10]*A[13];
  float a14 = A[ 5]*A[14] - A[ 6]*A[13];
  float a15 = A[ 5]*A[10] - A[ 6]*A[ 9];
  float a16 = A[ 1]*A[14] - A[ 2]*A[13];
  float a17 = A[ 1]*A[10] - A[ 2]*A[ 9];
  float a18 = A[ 1]*A[ 6] - A[ 2]*A[ 5];
  D[ 0] = d0 * divisor;
  D[ 1] = d1 * divisor;
  D[ 2] = d2 * divisor;
  D[ 3] = d3 * divisor;
  D[ 4] = ( - A[ 4]*a01 + A[ 8]*a02 - A[12]*a03) * divisor;
  D[ 5] = ( + A[ 0]*a01 - A[ 8]*a04 + A[12]*a05) * divisor;
  D[ 6] = ( - A[ 0]*a02 + A[ 4]*a04 - A[12]*a06) * divisor;
  D[ 7] = ( + A[ 0]*a03 - A[ 4]*a05 + A[ 8]*a06) * divisor;
  D[ 8] = ( + A[ 4]*a07 - A[ 8]*a08 + A[12]*a09) * divisor;
  D[ 9] = ( - A[ 0]*a07 + A[ 8]*a10 - A[12]*a11) * divisor;
  D[10] = ( + A[ 0]*a08 - A[ 4]*a10 + A[12]*a12) * divisor;
  D[11] = ( - A[ 0]*a09 + A[ 4]*a11 - A[ 8]*a12) * divisor;
  D[12] = ( - A[ 4]*a13 + A[ 8]*a14 - A[12]*a15) * divisor;
  D[13] = ( + A[ 0]*a13 - A[ 8]*a16 + A[12]*a17) * divisor;
  D[14] = ( - A[ 0]*a14 + A[ 4]*a16 - A[12]*a18) * divisor;
  D[15] = ( + A[ 0]*a15 - A[ 4]*a17 + A[ 8]*a18) * divisor;
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
