
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Neg
// ///////////////////////////////

NA_IDEF void naNegV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV1d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naNegV1d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV2d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naNegV2d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV3d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naNegV3d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV4d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naNegV4d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV1d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naNegV1f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV2d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naNegV2f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV3d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naNegV3f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV4d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naNegV4f", "Pointers overlap misaligned.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1dE (double* d, const double* a){
  d[0] = -a[0];
}
NA_IDEF void naNegV2dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naNegV2dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naNegV3dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naNegV4dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1fE(float*  d, const float*  a){
  d[0] = -a[0];
}
NA_IDEF void naNegV2fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naNegV2fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naNegV3fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naNegV4fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1dS (double* d){
  d[0] = -d[0];
}
NA_IDEF void naNegV2dS (double* d){
  d[0] = -d[0]; d[1] = -d[1];
}
NA_IDEF void naNegV3dS (double* d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2];
}
NA_IDEF void naNegV4dS (double* d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2]; d[3] = -d[3];
}
NA_IDEF void naNegV1fS(float*  d){
  d[0] = -d[0];
}
NA_IDEF void naNegV2fS(float*  d){
  d[0] = -d[0]; d[1] = -d[1];
}
NA_IDEF void naNegV3fS(float*  d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2];
}
NA_IDEF void naNegV4fS(float*  d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2]; d[3] = -d[3];
}



// ///////////////////////////////
// Add
// ///////////////////////////////

NA_IDEF void naAddV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naAddV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naAddV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naAddV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naAddV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naAddV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naAddV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naAddV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naAddV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naAddV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naAddV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naAddV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naAddV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naAddV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naAddV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naAddV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naAddV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naAddV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naAddV4dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naAddV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naAddV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naAddV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naAddV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] += b[0];
}
NA_IDEF void naAddV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naAddV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] += b[0];
}
NA_IDEF void naAddV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naAddV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1dSE(double* d, const double* b){
  d[0] += b[0];
}
NA_IDEF void naAddV2dSE(double* d, const double* b){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naAddV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1fSE(float*  d, const float*  b){
  d[0] += b[0];
}
NA_IDEF void naAddV2fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naAddV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}



// ///////////////////////////////
// Sub
// ///////////////////////////////

NA_IDEF void naSubV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naSubV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naSubV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naSubV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naSubV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naSubV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naSubV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naSubV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naSubV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naSubV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naSubV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naSubV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naSubV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naSubV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naSubV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naSubV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naSubV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naSubV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naSubV4dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naSubV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naSubV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naSubV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naSubV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] -= b[0];
}
NA_IDEF void naSubV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naSubV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] -= b[0];
}
NA_IDEF void naSubV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naSubV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1dSE(double* d, const double* b){
  d[0] -= b[0];
}
NA_IDEF void naSubV2dSE(double* d, const double* b){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naSubV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1fSE(float*  d, const float*  b){
  d[0] -= b[0];
}
NA_IDEF void naSubV2fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naSubV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}


// ///////////////////////////////
// Mul
// ///////////////////////////////

NA_IDEF void naMulV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulV1d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulV2d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMulV3d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMulV4d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulV1f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulV2f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMulV3f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMulV4f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1dE(double* d, const double* a, double s){
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulV2dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulV3dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMulV4dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1fE(float*  d, const float*  a, float s){
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulV2fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulV3fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMulV4fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1dS(double* d, double s){
  d[0] *= s;
}
NA_IDEF void naMulV2dS(double* d, double s){
  d[0] *= s;
  d[1] *= s;
}
NA_IDEF void naMulV3dS(double* d, double s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
}
NA_IDEF void naMulV4dS(double* d, double s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
  d[3] *= s;
}
NA_IDEF void naMulV1fS(float* d, float s){
  d[0] *= s;
}
NA_IDEF void naMulV2fS(float* d, float s){
  d[0] *= s;
  d[1] *= s;
}
NA_IDEF void naMulV3fS(float* d, float s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
}
NA_IDEF void naMulV4fS(float* d, float s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
  d[3] *= s;
}



// ///////////////////////////////
// Div
// ///////////////////////////////

NA_IDEF void naDivV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivV1d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivV2d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivV3d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivV4d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivV1f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivV2f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivV3f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivV4f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1dE(double* d, const double* a, double s){
  double div = naInv(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2dE(double* d, const double* a, double s){
  double div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivV2dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3dE(double* d, const double* a, double s){
  double div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivV3dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4dE(double* d, const double* a, double s){
  double div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivV4dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1fE(float*  d, const float*  a, float s){
  float div = naInvf(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2fE(float*  d, const float*  a, float s){
  float div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivV2fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3fE(float*  d, const float*  a, float s){
  float div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivV3fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4fE(float*  d, const float*  a, float s){
  float div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivV4fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
}
NA_IDEF void naDivV2dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
  d[1] *= div;
}
NA_IDEF void naDivV3dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
}
NA_IDEF void naDivV4dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
  d[3] *= div;
}
NA_IDEF void naDivV1fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
}
NA_IDEF void naDivV2fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
  d[1] *= div;
}
NA_IDEF void naDivV3fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
}
NA_IDEF void naDivV4fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
  d[3] *= div;
}



// ///////////////////////////////
// Mad
// ///////////////////////////////

NA_IDEF void naMadV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMadV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMadV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMadV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMadV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMadV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMadV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMadV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMadV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMadV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMadV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMadV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMadV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMadV1dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV1dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMadV2dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMadV3dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMadV4dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV4dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 1))
      naError("naMadV1fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 1))
      naError("naMadV1fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMadV2fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMadV3fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMadV4fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV4fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMadV3dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMadV4dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMadV3fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMadV4fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV1dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV4dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 1))
      naError("naMadV1fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2fE", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV4fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
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
