
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Dot
// ///////////////////////////////

NA_IDEF double naDotV1d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV1d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 1)
      naError("naDotV1d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF double naDotV2d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV2d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 2)
      naError("naDotV2d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF double naDotV3d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV3d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 3)
      naError("naDotV3d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF double naDotV4d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV4d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 4)
      naError("naDotV4d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF float naDotV1f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV1f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 1)
      naError("naDotV1f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF float naDotV2f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV2f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 2)
      naError("naDotV2f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF float naDotV3f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV3f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 3)
      naError("naDotV3f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF float naDotV4f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naDotV4f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 4)
      naError("naDotV4f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF double naDotV1dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 1))
      naError("naDotV1dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF double naDotV2dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 2))
      naError("naDotV2dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF double naDotV3dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 3))
      naError("naDotV3dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF double naDotV4dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 4))
      naError("naDotV4dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF float naDotV1fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 1))
      naError("naDotV1fE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF float naDotV2fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 2))
      naError("naDotV2fE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF float naDotV3fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 3))
      naError("naDotV3fE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF float naDotV4fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 4))
      naError("naDotV4fE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF double naDotV1dS(const double* v){
  return v[0] * v[0];
}
NA_IDEF double naDotV2dS(const double* v){
  return v[0] * v[0] + v[1] * v[1];
}
NA_IDEF double naDotV3dS(const double* v){
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
NA_IDEF double naDotV4dS(const double* v){
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}
NA_IDEF float naDotV1fS(const float* v){
  return v[0] * v[0];
}
NA_IDEF float naDotV2fS(const float* v){
  return v[0] * v[0] + v[1] * v[1];
}
NA_IDEF float naDotV3fS(const float* v){
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}
NA_IDEF float naDotV4fS(const float* v){
  return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}



// ///////////////////////////////
// Cross
// ///////////////////////////////

NA_IDEF void naCrossV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naCrossV3d", "a and d are equal.");
    if(b == d)
      naError("naCrossV3d", "b and d are equal.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naCrossV3d", "a overlaps misaligned with d");
    else if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naCrossV3d", "b overlaps misaligned with d");
  #endif
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
}
NA_IDEF void naCrossV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naCrossV3f", "a and d are equal.");
    if(b == d)
      naError("naCrossV3f", "b and d are equal.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naCrossV3f", "a overlaps misaligned with d");
    else if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naCrossV3f", "b overlaps misaligned with d");
  #endif
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
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
