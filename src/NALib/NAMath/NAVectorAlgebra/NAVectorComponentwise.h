
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Invert componentwise
// ///////////////////////////////

NA_IDEF void naInvCompV1d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV1d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 1)
      naError("naInvCompV1d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
}
NA_IDEF void naInvCompV2d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV2d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 2)
      naError("naInvCompV2d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
}
NA_IDEF void naInvCompV3d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV3d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 3)
      naError("naInvCompV3d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
}
NA_IDEF void naInvCompV4d(double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV4d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 4)
      naError("naInvCompV4d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
  d[3] = naInv(s[3]);
}
NA_IDEF void naInvCompV1f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV1f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 1)
      naError("naInvCompV1f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
}
NA_IDEF void naInvCompV2f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV2f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 2)
      naError("naInvCompV2f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
}
NA_IDEF void naInvCompV3f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV3f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 3)
      naError("naInvCompV3f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
}
NA_IDEF void naInvCompV4f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV4f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 4)
      naError("naInvCompV4f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
  d[3] = naInvf(s[3]);
}
NA_IDEF void naInvCompV1dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 1))
      naError("naInvCompV1dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
}
NA_IDEF void naInvCompV2dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 2))
      naError("naInvCompV2dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
}
NA_IDEF void naInvCompV3dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 3))
      naError("naInvCompV3dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
}
NA_IDEF void naInvCompV4dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 4))
      naError("naInvCompV4dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
  d[3] = naInv(s[3]);
}
NA_IDEF void naInvCompV1fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 1))
      naError("naInvCompV1fE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
}
NA_IDEF void naInvCompV2fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 2))
      naError("naInvCompV2fE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
}
NA_IDEF void naInvCompV3fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 3))
      naError("naInvCompV3fE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
}
NA_IDEF void naInvCompV4fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 4))
      naError("naInvCompV4fE", "s overlaps misaligned with d.");
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
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV1dV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulCompV1dV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMulCompV1dV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV2dV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulCompV2dV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2dV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV3dV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMulCompV3dV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMulCompV3dV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV4dV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMulCompV4dV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMulCompV4dV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1fV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV1fV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulCompV1fV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMulCompV1fV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV2fV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulCompV2fV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2fV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV3fV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMulCompV3fV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMulCompV3fV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV4fV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMulCompV4fV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMulCompV4fV4f", "b overlaps with d. Use restrict pointers or the E variant");
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
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulCompV2dV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2dV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3dV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulCompV3dV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3dV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4dV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMulCompV4dV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4dV4dE", "b overlaps misaligned with d. Result undefined.");
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
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulCompV2fV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2fV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3fV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulCompV3fV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3fV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4fV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMulCompV4fV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4fV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1dV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV1dV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2dV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV2dV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3dV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV3dV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4dV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV4dV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}
NA_IDEF void naMulCompV1fV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV1fV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2fV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV2fV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3fV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV3fV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4fV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV4fV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
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
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3dV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4dV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
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
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3fV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4fV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
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
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV1dV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivCompV1dV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naDivCompV1dV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV2dV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivCompV2dV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2dV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV3dV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivCompV3dV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naDivCompV3dV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV4dV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivCompV4dV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naDivCompV4dV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
  d[3] = a[3] * naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV1fV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivCompV1fV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naDivCompV1fV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV2fV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivCompV2fV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2fV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV3fV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivCompV3fV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naDivCompV3fV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV4fV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivCompV4fV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naDivCompV4fV4f", "b overlaps with d. Use restrict pointers or the E variant");
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
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivCompV2dV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2dV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivCompV3dV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3dV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivCompV4dV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4dV4dE", "b overlaps misaligned with d. Result undefined.");
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
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivCompV2fV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2fV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivCompV3fV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3fV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivCompV4fV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4fV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivCompV1dV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV1dV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV2dV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV3dV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV4dV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
  d[3] *= naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV1fV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV2fV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV3fV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV4fV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
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
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
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
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
}



// Copyright (c) NALib, Tobias Stamm
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
