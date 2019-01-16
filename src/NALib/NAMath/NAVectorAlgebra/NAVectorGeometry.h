
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Length
// ///////////////////////////////

NA_IDEF double naLengthV1d(const double* a){
  return naAbs(a[0]);
}
NA_IDEF double naLengthV2d(const double* a){
  return naSqrt(naDotV2dS(a));
}
NA_IDEF double naLengthV3d(const double* a){
  return naSqrt(naDotV3dS(a));
}
NA_IDEF double naLengthV4d(const double* a){
  return naSqrt(naDotV4dS(a));
}
NA_IDEF float naLengthV1f(const float* a){
  return naAbsf(a[0]);
}
NA_IDEF float naLengthV2f(const float* a){
  return naSqrtf(naDotV2fS(a));
}
NA_IDEF float naLengthV3f(const float* a){
  return naSqrtf(naDotV3fS(a));
}
NA_IDEF float naLengthV4f(const float* a){
  return naSqrtf(naDotV4fS(a));
}


// ///////////////////////////////
// Distance
// ///////////////////////////////

NA_IDEF double naDistanceV1d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV1d", "a and b are the same pointer");
  #endif
  return naAbs(a[0] - b[0]);
}
NA_IDEF double naDistanceV2d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  NAVec2d vec;
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV2d", "a and b are the same pointer");
  #endif
  naSubV2d(vec, a, b);
  return naLengthV2d(vec);
}
NA_IDEF double naDistanceV3d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  NAVec3d vec;
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV3d", "a and b are the same pointer");
  #endif
  naSubV3d(vec, a, b);
  return naLengthV3d(vec);
}
NA_IDEF double naDistanceV4d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  NAVec4d vec;
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV4d", "a and b are the same pointer");
  #endif
  naSubV4d(vec, a, b);
  return naLengthV4d(vec);
}
NA_IDEF float naDistanceV1f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV1f", "a and b are the same pointer");
  #endif
  return naAbsf(a[0] - b[0]);
}
NA_IDEF float naDistanceV2f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  NAVec2f vec;
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV2f", "a and b are the same pointer");
  #endif
  naSubV2f(vec, a, b);
  return naLengthV2f(vec);
}
NA_IDEF float naDistanceV3f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  NAVec3f vec;
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV3f", "a and b are the same pointer");
  #endif
  naSubV3f(vec, a, b);
  return naLengthV3f(vec);
}
NA_IDEF float naDistanceV4f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  NAVec4f vec;
  #ifndef NDEBUG
    if(a == b)
      naError("naDistanceV4f", "a and b are the same pointer");
  #endif
  naSubV4f(vec, a, b);
  return naLengthV4f(vec);
}
NA_IDEF double naDistanceV1dE(const double* a, const double* b){
  return naAbs(a[0] - b[0]);
}
NA_IDEF double naDistanceV2dE(const double* a, const double* b){
  NAVec2d vec;
  naSubV2d(vec, a, b);
  return naLengthV2d(vec);
}
NA_IDEF double naDistanceV3dE(const double* a, const double* b){
  NAVec3d vec;
  naSubV3d(vec, a, b);
  return naLengthV3d(vec);
}
NA_IDEF double naDistanceV4dE(const double* a, const double* b){
  NAVec4d vec;
  naSubV4d(vec, a, b);
  return naLengthV4d(vec);
}
NA_IDEF float naDistanceV1fE(const float* a, const float* b){
  return naAbsf(a[0] - b[0]);
}
NA_IDEF float naDistanceV2fE(const float* a, const float* b){
  NAVec2f vec;
  naSubV2f(vec, a, b);
  return naLengthV2f(vec);
}
NA_IDEF float naDistanceV3fE(const float* a, const float* b){
  NAVec3f vec;
  naSubV3f(vec, a, b);
  return naLengthV3f(vec);
}
NA_IDEF float naDistanceV4fE(const float* a, const float* b){
  NAVec4f vec;
  naSubV4f(vec, a, b);
  return naLengthV4f(vec);
}



// ///////////////////////////////
// Mirror
// ///////////////////////////////

NA_IDEF void naMirrorV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1d", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV1d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1d", "a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2d", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV2d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2d", "a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3d", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV3d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3d", "a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4d", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV4d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4d", "a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV1f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1f", "a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV2f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2f", "a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV3f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3f", "a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("naMirrorV4f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV4f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4f", "a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1dE", "a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2dE", "a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3dE", "a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4dE", "a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1fE", "a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2fE", "a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3fE", "a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("naMirrorV4fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4fE", "a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1dS", "Normal is not normalized");
  #endif
  naMadV1dS(d, n, -2. * naDotV1d(d, n));
}
NA_IDEF void naMirrorV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2dS", "Normal is not normalized");
  #endif
  naMadV2dS(d, n, -2. * naDotV2d(d, n));
}
NA_IDEF void naMirrorV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3dS", "Normal is not normalized");
  #endif
  naMadV3dS(d, n, -2. * naDotV3d(d, n));
}
NA_IDEF void naMirrorV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4dS", "Normal is not normalized");
  #endif
  naMadV4dS(d, n, -2. * naDotV4d(d, n));
}
NA_IDEF void naMirrorV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1fS", "Normal is not normalized");
  #endif
  naMadV1fS(d, n, -2.f * naDotV1f(d, n));
}
NA_IDEF void naMirrorV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2fS", "Normal is not normalized");
  #endif
  naMadV2fS(d, n, -2.f * naDotV2f(d, n));
}
NA_IDEF void naMirrorV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3fS", "Normal is not normalized");
  #endif
  naMadV3fS(d, n, -2.f * naDotV3f(d, n));
}
NA_IDEF void naMirrorV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("naMirrorV4fS", "Normal is not normalized");
  #endif
  naMadV4fS(d, n, -2.f * naDotV4f(d, n));
}




// ///////////////////////////////
// Normalize
// ///////////////////////////////

NA_IDEF double naNormalizeV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV1d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1d", "a overlaps misaligned with d");
  #endif
  length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV2d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2d", "a overlaps misaligned with d");
  #endif
  length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV3d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3d", "a overlaps misaligned with d");
  #endif
  length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV4d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4d", "a overlaps misaligned with d");
  #endif
  length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV1f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1f", "a overlaps misaligned with d");
  #endif
  length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV2f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2f", "a overlaps misaligned with d");
  #endif
  length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV3f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3f", "a overlaps misaligned with d");
  #endif
  length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV4f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4f", "a overlaps misaligned with d");
  #endif
  length = naLengthV4f(a);
  naDivV4f(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV1dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV4f(a);
  naDivV4f(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV1dS(double* v){
  double length = naLengthV1d(v);
  naDivV1dS(v, length);
  return length;
}
NA_IDEF double naNormalizeV2dS(double* v){
  double length = naLengthV2d(v);
  naDivV2dS(v, length);
  return length;
}
NA_IDEF double naNormalizeV3dS(double* v){
  double length = naLengthV3d(v);
  naDivV3dS(v, length);
  return length;
}
NA_IDEF double naNormalizeV4dS(double* v){
  double length = naLengthV4d(v);
  naDivV4dS(v, length);
  return length;
}
NA_IDEF float naNormalizeV1fS(float* v){
  float length = naLengthV1f(v);
  naDivV1fS(v, length);
  return length;
}
NA_IDEF float naNormalizeV2fS(float* v){
  float length = naLengthV2f(v);
  naDivV2fS(v, length);
  return length;
}
NA_IDEF float naNormalizeV3fS(float* v){
  float length = naLengthV3f(v);
  naDivV3fS(v, length);
  return length;
}
NA_IDEF float naNormalizeV4fS(float* v){
  float length = naLengthV4f(v);
  naDivV4fS(v, length);
  return length;
}


// ///////////////////////////////
// Orthogonalize
// ///////////////////////////////

NA_IDEF void naOrthogonalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthogonalizeV2d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV2d", "b looks like a zero vector.");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  double bnorm = naDotV3dS(b);
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthogonalizeV3d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV3d", "b looks like a zero vector.");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  double bnorm = naDotV4dS(b);
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthogonalizeV4d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV4d", "b looks like a zero vector.");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  float bnorm = naDotV2fS(b);
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthogonalizeV2f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV2f", "b looks like a zero vector.");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  float bnorm = naDotV3fS(b);
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthogonalizeV3f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV3f", "b looks like a zero vector.");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  float bnorm = naDotV4fS(b);
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthogonalizeV4f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dE(double* d, const double* a, const double* b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV2d", "b looks like a zero vector.");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dE(double* d, const double* a, const double* b){
  double bnorm = naDotV3dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV3d", "b looks like a zero vector.");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dE(double* d, const double* a, const double* b){
  double bnorm = naDotV4dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV4d", "b looks like a zero vector.");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fE(float* d, const float* a, const float* b){
  float bnorm = naDotV2fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV2f", "b looks like a zero vector.");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fE(float* d, const float* a, const float* b){
  float bnorm = naDotV3fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV3f", "b looks like a zero vector.");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fE(float* d, const float* a, const float* b){
  float bnorm = naDotV4fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV2d", "b looks like a zero vector.");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV3dS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV3d", "b looks like a zero vector.");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV4dS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV4d", "b looks like a zero vector.");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV2fS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV2f", "b looks like a zero vector.");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV3fS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV3f", "b looks like a zero vector.");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV4fS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4fS(d, b, - naDotV4f(b, d) * naInvf(bnorm));
}



// ///////////////////////////////
// Orthonormalize
// ///////////////////////////////

NA_IDEF void naOrthonormalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV2d", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV3d", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV4d", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV2f", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV3f", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV4f", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4fS(d, b, - naDotV4f(b, d));
  naNormalizeV4fS(d);
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
