
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Length
// ///////////////////////////////

NA_IDEF double naLengthV1d(const double* v){
  return naAbs(v[0]);
}
NA_IDEF double naLengthV2d(const double* v){
  return naSqrt(naDotV2dS(v));
}
NA_IDEF double naLengthV3d(const double* v){
  return naSqrt(naDotV3dS(v));
}
NA_IDEF double naLengthV4d(const double* v){
  return naSqrt(naDotV4dS(v));
}
NA_IDEF float naLengthV1f(const float* v){
  return naAbsf(v[0]);
}
NA_IDEF float naLengthV2f(const float* v){
  return naSqrtf(naDotV2fS(v));
}
NA_IDEF float naLengthV3f(const float* v){
  return naSqrtf(naDotV3fS(v));
}
NA_IDEF float naLengthV4f(const float* v){
  return naSqrtf(naDotV4fS(v));
}


// ///////////////////////////////
// Distance
// ///////////////////////////////

NA_IDEF double naDistanceV1d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
  #endif
  return naAbs(a[0] - b[0]);
}
NA_IDEF double naDistanceV2d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  NAVec2d vec;
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
  #endif
  naSubV2d(vec, a, b);
  return naLengthV2d(vec);
}
NA_IDEF double naDistanceV3d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  NAVec3d vec;
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
  #endif
  naSubV3d(vec, a, b);
  return naLengthV3d(vec);
}
NA_IDEF double naDistanceV4d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  NAVec4d vec;
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
  #endif
  naSubV4d(vec, a, b);
  return naLengthV4d(vec);
}
NA_IDEF float naDistanceV1f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
  #endif
  return naAbsf(a[0] - b[0]);
}
NA_IDEF float naDistanceV2f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  NAVec2f vec;
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
  #endif
  naSubV2f(vec, a, b);
  return naLengthV2f(vec);
}
NA_IDEF float naDistanceV3f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  NAVec3f vec;
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
  #endif
  naSubV3f(vec, a, b);
  return naLengthV3f(vec);
}
NA_IDEF float naDistanceV4f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  NAVec4f vec;
  #if NA_DEBUG
    if(a == b)
      naError("a and b are the same pointer");
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
  #if NA_DEBUG
    if(naElementOverlap(n, d, 1, 1))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 2, 2))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 3, 3))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 4, 4))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 1, 1))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 2, 2))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 3, 3))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 4, 4))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("Normal is not normalized");
    if(a == d)
      naError("a equals d. Use E or S variant or restrict pointers.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dE(double* d, const double* a, const double* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 1, 1))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2dE(double* d, const double* a, const double* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 2, 2))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3dE(double* d, const double* a, const double* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 3, 3))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4dE(double* d, const double* a, const double* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 4, 4))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1fE(float* d, const float* a, const float* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 1, 1))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2fE(float* d, const float* a, const float* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 2, 2))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3fE(float* d, const float* a, const float* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 3, 3))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4fE(float* d, const float* a, const float* n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 4, 4))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("Normal is not normalized");
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 1, 1))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("Normal is not normalized");
  #endif
  naMadV1dS(d, n, -2. * naDotV1d(d, n));
}
NA_IDEF void naMirrorV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 2, 2))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("Normal is not normalized");
  #endif
  naMadV2dS(d, n, -2. * naDotV2d(d, n));
}
NA_IDEF void naMirrorV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 3, 3))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("Normal is not normalized");
  #endif
  naMadV3dS(d, n, -2. * naDotV3d(d, n));
}
NA_IDEF void naMirrorV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 4, 4))
      naError("n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("Normal is not normalized");
  #endif
  naMadV4dS(d, n, -2. * naDotV4d(d, n));
}
NA_IDEF void naMirrorV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 1, 1))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("Normal is not normalized");
  #endif
  naMadV1fS(d, n, -2.f * naDotV1f(d, n));
}
NA_IDEF void naMirrorV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 2, 2))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("Normal is not normalized");
  #endif
  naMadV2fS(d, n, -2.f * naDotV2f(d, n));
}
NA_IDEF void naMirrorV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 3, 3))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("Normal is not normalized");
  #endif
  naMadV3fS(d, n, -2.f * naDotV3f(d, n));
}
NA_IDEF void naMirrorV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #if NA_DEBUG
    if(naElementOverlap(n, d, 4, 4))
      naError("n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("Normal is not normalized");
  #endif
  naMadV4fS(d, n, -2.f * naDotV4f(d, n));
}




// ///////////////////////////////
// Normalize
// ///////////////////////////////

NA_IDEF double naNormalizeV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV4f(a);
  naDivV4f(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV1dE(double* d, const double* a){
  double length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2dE(double* d, const double* a){
  double length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3dE(double* d, const double* a){
  double length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4dE(double* d, const double* a){
  double length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1fE(float* d, const float* a){
  float length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 1, 1))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2fE(float* d, const float* a){
  float length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3fE(float* d, const float* a){
  float length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
  #endif
  length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4fE(float* d, const float* a){
  float length;
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
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
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  double bnorm = naDotV3dS(b);
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  double bnorm = naDotV4dS(b);
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  float bnorm = naDotV2fS(b);
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  float bnorm = naDotV3fS(b);
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  float bnorm = naDotV4fS(b);
  #if NA_DEBUG
    if(a == d)
      naError("a is equal to d. Use E or S variant.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dE(double* d, const double* a, const double* b){
  double bnorm = naDotV2dS(b);
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dE(double* d, const double* a, const double* b){
  double bnorm = naDotV3dS(b);
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dE(double* d, const double* a, const double* b){
  double bnorm = naDotV4dS(b);
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fE(float* d, const float* a, const float* b){
  float bnorm = naDotV2fS(b);
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fE(float* d, const float* a, const float* b){
  float bnorm = naDotV3fS(b);
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fE(float* d, const float* a, const float* b){
  float bnorm = naDotV4fS(b);
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV2dS(b);
  #if NA_DEBUG
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV3dS(b);
  #if NA_DEBUG
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV4dS(b);
  #if NA_DEBUG
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV2fS(b);
  #if NA_DEBUG
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV3fS(b);
  #if NA_DEBUG
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV4fS(b);
  #if NA_DEBUG
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("b looks like a zero vector.");
  #endif
  naMadV4fS(d, b, - naDotV4f(b, d) * naInvf(bnorm));
}



// ///////////////////////////////
// Orthonormalize
// ///////////////////////////////

NA_IDEF void naOrthonormalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == d)
      naError("a equals d. Use E or S variant.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == d)
      naError("a equals d. Use E or S variant.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == d)
      naError("a equals d. Use E or S variant.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == d)
      naError("a equals d. Use E or S variant.");
    else if(naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == d)
      naError("a equals d. Use E or S variant.");
    else if(naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(a == d)
      naError("a equals d. Use E or S variant.");
    else if(naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dE(double* d, const double* a, const double* b){
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fE(float* d, const float* a, const float* b){
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 2, 2))
      naError("a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fE(float* d, const float* a, const float* b){
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 3, 3))
      naError("a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fE(float* d, const float* a, const float* b){
  #if NA_DEBUG
    if((a != d) && naElementOverlap(a, d, 4, 4))
      naError("a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(!naAlmostOne(naLengthV3d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #if NA_DEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 2, 2))
      naError("b overlaps with d");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 3, 3))
      naError("b overlaps with d");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #if NA_DEBUG
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("b is not normalized");
    if(naElementOverlap(b, d, 4, 4))
      naError("b overlaps with d");
  #endif
  naMadV4fS(d, b, - naDotV4f(b, d));
  naNormalizeV4fS(d);
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
