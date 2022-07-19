
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Invert componentwise
// ///////////////////////////////

NA_IDEF void naInvCompV1d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 1, 1))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
}
NA_IDEF void naInvCompV2d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 2, 2))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
}
NA_IDEF void naInvCompV3d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 3, 3))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
}
NA_IDEF void naInvCompV4d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 4, 4))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
  d[3] = naInv(s[3]);
}
NA_IDEF void naInvCompV1f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 1, 1))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
}
NA_IDEF void naInvCompV2f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 2, 2))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
}
NA_IDEF void naInvCompV3f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 3, 3))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
}
NA_IDEF void naInvCompV4f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #if NA_DEBUG
    if(s==d)
      naError("s equals d. Use S or E variant");
    else if(naElementOverlap(d, s, 4, 4))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
  d[3] = naInvf(s[3]);
}
NA_IDEF void naInvCompV1dE(double* d, double* s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 1, 1)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
}
NA_IDEF void naInvCompV2dE(double* d, double* s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 2, 2)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
}
NA_IDEF void naInvCompV3dE(double* d, double* s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 3, 3)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
}
NA_IDEF void naInvCompV4dE(double* d, double* s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 4, 4)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
  d[3] = naInv(s[3]);
}
NA_IDEF void naInvCompV1fE(float*  d, float*  s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 1, 1)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
}
NA_IDEF void naInvCompV2fE(float*  d, float*  s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 2, 2)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
}
NA_IDEF void naInvCompV3fE(float*  d, float*  s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 3, 3)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
}
NA_IDEF void naInvCompV4fE(float*  d, float*  s){
  #if NA_DEBUG
    if((s != d) && (naElementOverlap(d, s, 4, 4)))
      naError("s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
  d[3] = naInvf(s[3]);
}
NA_IDEF void naInvCompV1dS(double* d){
  d[0] = naInv(d[0]);
}
NA_IDEF void naInvCompV2dS(double* d){
  d[0] = naInv(d[0]);
  d[1] = naInv(d[1]);
}
NA_IDEF void naInvCompV3dS(double* d){
  d[0] = naInv(d[0]);
  d[1] = naInv(d[1]);
  d[2] = naInv(d[2]);
}
NA_IDEF void naInvCompV4dS(double* d){
  d[0] = naInv(d[0]);
  d[1] = naInv(d[1]);
  d[2] = naInv(d[2]);
  d[3] = naInv(d[3]);
}
NA_IDEF void naInvCompV1fS(float*  d){
  d[0] = naInvf(d[0]);
}
NA_IDEF void naInvCompV2fS(float*  d){
  d[0] = naInvf(d[0]);
  d[1] = naInvf(d[1]);
}
NA_IDEF void naInvCompV3fS(float*  d){
  d[0] = naInvf(d[0]);
  d[1] = naInvf(d[1]);
  d[2] = naInvf(d[2]);
}
NA_IDEF void naInvCompV4fS(float*  d){
  d[0] = naInvf(d[0]);
  d[1] = naInvf(d[1]);
  d[2] = naInvf(d[2]);
  d[3] = naInvf(d[3]);
}



// ///////////////////////////////
// Multiply componentwise
// ///////////////////////////////

NA_IDEF void naMulCompV1dV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1fV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1dV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2dV2dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3dV3dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4dV4dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1fV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2fV2fE(float*  d, const float*  a, const float*  b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3fV3fE(float*  d, const float*  a, const float*  b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4fV4fE(float*  d, const float*  a, const float*  b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1dV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2dV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3dV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4dV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}
NA_IDEF void naMulCompV1fV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2fV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3fV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4fV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}
NA_IDEF void naMulCompV1dV1dSE(double* d, const double* b){
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2dV2dSE(double* d, const double* b){
  #if NA_DEBUG
    if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3dV3dSE(double* d, const double* b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4dV4dSE(double* d, const double* b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}
NA_IDEF void naMulCompV1fV1fSE(float*  d, const float*  b){
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2fV2fSE(float*  d, const float*  b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3fV3fSE(float*  d, const float*  b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4fV4fSE(float*  d, const float*  b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}



// ///////////////////////////////
// Divide componentwise
// ///////////////////////////////

NA_IDEF void naDivCompV1dV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
  d[3] = a[3] * naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 1, 1))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 1, 1))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 2, 2))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 2, 2))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 3, 3))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 3, 3))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a==d)
      naError("a equals d. Use S or E variant");
    else if(naElementOverlap(d, a, 4, 4))
      naError("a overlaps misaligned with d.");
    if(naElementOverlap(d, b, 4, 4))
      naError("b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivCompV1dV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] * naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
  d[3] = a[3] * naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2fE(float*  d, const float*  a, const float*  b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 2, 2)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 2, 2)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fE(float*  d, const float*  a, const float*  b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 3, 3)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 3, 3)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fE(float*  d, const float*  a, const float*  b){
  #if NA_DEBUG
    if((a!=d) && (naElementOverlap(d, a, 4, 4)))
      naError("a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naElementOverlap(d, b, 4, 4)))
      naError("b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivCompV1dV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
  d[3] *= naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #if NA_DEBUG
    if(d == b)
      naError("Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
}
NA_IDEF void naDivCompV1dV1dSE(double* d, const double* b){
  d[0] *= naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2dSE(double* d, const double* b){
  #if NA_DEBUG
    if(naElementOverlap(d, b, 2, 2))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dSE(double* d, const double* b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dSE(double* d, const double* b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
  d[3] *= naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1fSE(float*  d, const float*  b){
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2fSE(float*  d, const float*  b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 2, 2)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fSE(float*  d, const float*  b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 3, 3)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fSE(float*  d, const float*  b){
  #if NA_DEBUG
    if((d != b) && (naElementOverlap(d, b, 4, 4)))
      naError("b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
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
