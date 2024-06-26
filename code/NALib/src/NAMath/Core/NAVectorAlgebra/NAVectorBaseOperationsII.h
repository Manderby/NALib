
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Neg
// ///////////////////////////////

NA_IDEF void naNegV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 1, 1))
      naError("Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 2, 2))
      naError("Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 3, 3))
      naError("Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 4, 4))
      naError("Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 1, 1))
      naError("Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 2, 2))
      naError("Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 3, 3))
      naError("Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a) {
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use S or E variant.");
    else if(naElementOverlap(d, a, 4, 4))
      naError("Pointers overlap misaligned.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1dE (double* d, const double* a) {
  d[0] = -a[0];
}
NA_IDEF void naNegV2dE (double* d, const double* a) {
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 2, 2)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3dE (double* d, const double* a) {
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 3, 3)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4dE (double* d, const double* a) {
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 4, 4)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1fE(float*  d, const float*  a) {
  d[0] = -a[0];
}
NA_IDEF void naNegV2fE(float*  d, const float*  a) {
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 2, 2)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3fE(float*  d, const float*  a) {
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 3, 3)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4fE(float*  d, const float*  a) {
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 4, 4)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1dS (double* d) {
  d[0] = -d[0];
}
NA_IDEF void naNegV2dS (double* d) {
  d[0] = -d[0]; d[1] = -d[1];
}
NA_IDEF void naNegV3dS (double* d) {
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2];
}
NA_IDEF void naNegV4dS (double* d) {
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2]; d[3] = -d[3];
}
NA_IDEF void naNegV1fS(float*  d) {
  d[0] = -d[0];
}
NA_IDEF void naNegV2fS(float*  d) {
  d[0] = -d[0]; d[1] = -d[1];
}
NA_IDEF void naNegV3fS(float*  d) {
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2];
}
NA_IDEF void naNegV4fS(float*  d) {
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2]; d[3] = -d[3];
}



// ///////////////////////////////
// Add
// ///////////////////////////////

NA_IDEF void naAddV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1dE(double* d, const double* a, const double* b) {
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2dE(double* d, const double* a, const double* b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3dE(double* d, const double* a, const double* b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4dE(double* d, const double* a, const double* b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1fE(float*  d, const float*  a, const float*  b) {
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2fE(float*  d, const float*  a, const float*  b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3fE(float*  d, const float*  a, const float*  b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4fE(float*  d, const float*  a, const float*  b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] += b[0];
}
NA_IDEF void naAddV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] += b[0];
}
NA_IDEF void naAddV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1dSE(double* d, const double* b) {
  d[0] += b[0];
}
NA_IDEF void naAddV2dSE(double* d, const double* b) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dSE(double* d, const double* b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4dSE(double* d, const double* b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1fSE(float*  d, const float*  b) {
  d[0] += b[0];
}
NA_IDEF void naAddV2fSE(float*  d, const float*  b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fSE(float*  d, const float*  b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4fSE(float*  d, const float*  b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}



// ///////////////////////////////
// Sub
// ///////////////////////////////

NA_IDEF void naSubV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1dE(double* d, const double* a, const double* b) {
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2dE(double* d, const double* a, const double* b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3dE(double* d, const double* a, const double* b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4dE(double* d, const double* a, const double* b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1fE(float*  d, const float*  a, const float*  b) {
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2fE(float*  d, const float*  a, const float*  b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3fE(float*  d, const float*  a, const float*  b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4fE(float*  d, const float*  a, const float*  b) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] -= b[0];
}
NA_IDEF void naSubV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] -= b[0];
}
NA_IDEF void naSubV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b) {
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1dSE(double* d, const double* b) {
  d[0] -= b[0];
}
NA_IDEF void naSubV2dSE(double* d, const double* b) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dSE(double* d, const double* b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4dSE(double* d, const double* b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1fSE(float*  d, const float*  b) {
  d[0] -= b[0];
}
NA_IDEF void naSubV2fSE(float*  d, const float*  b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fSE(float*  d, const float*  b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4fSE(float*  d, const float*  b) {
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}


// ///////////////////////////////
// Mul
// ///////////////////////////////

NA_IDEF void naMulV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1dE(double* d, const double* a, double s) {
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2dE(double* d, const double* a, double s) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3dE(double* d, const double* a, double s) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4dE(double* d, const double* a, double s) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d ,a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1fE(float*  d, const float*  a, float s) {
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2fE(float*  d, const float*  a, float s) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3fE(float*  d, const float*  a, float s) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4fE(float*  d, const float*  a, float s) {
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1dS(double* d, double s) {
  d[0] *= s;
}
NA_IDEF void naMulV2dS(double* d, double s) {
  d[0] *= s;
  d[1] *= s;
}
NA_IDEF void naMulV3dS(double* d, double s) {
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
}
NA_IDEF void naMulV4dS(double* d, double s) {
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
  d[3] *= s;
}
NA_IDEF void naMulV1fS(float* d, float s) {
  d[0] *= s;
}
NA_IDEF void naMulV2fS(float* d, float s) {
  d[0] *= s;
  d[1] *= s;
}
NA_IDEF void naMulV3fS(float* d, float s) {
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
}
NA_IDEF void naMulV4fS(float* d, float s) {
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
  d[3] *= s;
}



// ///////////////////////////////
// Div
// ///////////////////////////////

NA_IDEF void naDivV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  double divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInv(s);
  d[0] = a[0] * divisor;
}
NA_IDEF void naDivV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  double divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInv(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
}
NA_IDEF void naDivV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  double divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInv(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
}
NA_IDEF void naDivV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s) {
  double divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInv(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
  d[3] = a[3] * divisor;
}
NA_IDEF void naDivV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  float divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInvf(s);
  d[0] = a[0] * divisor;
}
NA_IDEF void naDivV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  float divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInvf(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
}
NA_IDEF void naDivV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  float divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInvf(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
}
NA_IDEF void naDivV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s) {
  float divisor;
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
  #endif
  divisor = naInvf(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
  d[3] = a[3] * divisor;
}
NA_IDEF void naDivV1dE(double* d, const double* a, double s) {
  double divisor = naInv(s);
  d[0] = a[0] * divisor;
}
NA_IDEF void naDivV2dE(double* d, const double* a, double s) {
  double divisor;
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  divisor = naInv(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
}
NA_IDEF void naDivV3dE(double* d, const double* a, double s) {
  double divisor;
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  divisor = naInv(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
}
NA_IDEF void naDivV4dE(double* d, const double* a, double s) {
  double divisor;
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  divisor = naInv(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
  d[3] = a[3] * divisor;
}
NA_IDEF void naDivV1fE(float*  d, const float*  a, float s) {
  float divisor = naInvf(s);
  d[0] = a[0] * divisor;
}
NA_IDEF void naDivV2fE(float*  d, const float*  a, float s) {
  float divisor;
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  divisor = naInvf(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
}
NA_IDEF void naDivV3fE(float*  d, const float*  a, float s) {
  float divisor;
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  divisor = naInvf(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
}
NA_IDEF void naDivV4fE(float*  d, const float*  a, float s) {
  float divisor;
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
  #endif
  divisor = naInvf(s);
  d[0] = a[0] * divisor;
  d[1] = a[1] * divisor;
  d[2] = a[2] * divisor;
  d[3] = a[3] * divisor;
}
NA_IDEF void naDivV1dS(double* d, double s) {
  double divisor = naInv(s);
  d[0] *= divisor;
}
NA_IDEF void naDivV2dS(double* d, double s) {
  double divisor = naInv(s);
  d[0] *= divisor;
  d[1] *= divisor;
}
NA_IDEF void naDivV3dS(double* d, double s) {
  double divisor = naInv(s);
  d[0] *= divisor;
  d[1] *= divisor;
  d[2] *= divisor;
}
NA_IDEF void naDivV4dS(double* d, double s) {
  double divisor = naInv(s);
  d[0] *= divisor;
  d[1] *= divisor;
  d[2] *= divisor;
  d[3] *= divisor;
}
NA_IDEF void naDivV1fS(float* d, float s) {
  float divisor = naInvf(s);
  d[0] *= divisor;
}
NA_IDEF void naDivV2fS(float* d, float s) {
  float divisor = naInvf(s);
  d[0] *= divisor;
  d[1] *= divisor;
}
NA_IDEF void naDivV3fS(float* d, float s) {
  float divisor = naInvf(s);
  d[0] *= divisor;
  d[1] *= divisor;
  d[2] *= divisor;
}
NA_IDEF void naDivV4fS(float* d, float s) {
  float divisor = naInvf(s);
  d[0] *= divisor;
  d[1] *= divisor;
  d[2] *= divisor;
  d[3] *= divisor;
}



// ///////////////////////////////
// Mad
// ///////////////////////////////

NA_IDEF void naMadV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dE(double* d, const double* a, const double* b, double s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2dE(double* d, const double* a, const double* b, double s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3dE(double* d, const double* a, const double* b, double s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4dE(double* d, const double* a, const double* b, double s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1fE(float* d, const float* a, const float* b, float s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 1, 1)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 1, 1)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2fE(float* d, const float* a, const float* b, float s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3fE(float* d, const float* a, const float* b, float s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4fE(float* d, const float* a, const float* b, float s) {
  #if NA_DEBUG
    if((a != d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d.");
    if((b != d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s) {
  #if NA_DEBUG
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1dES(double* d, const double* b, double s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dES(double* d, const double* b, double s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dES(double* d, const double* b, double s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dES(double* d, const double* b, double s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fES(float* d, const float* b, float s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 1, 1)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fES(float* d, const float* b, float s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fES(float* d, const float* b, float s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fES(float* d, const float* b, float s) {
  #if NA_DEBUG
    if((b != d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
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
