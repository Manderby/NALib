
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Equal
// ///////////////////////////////

NA_IDEF NABool naEqualV1d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV1d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 1)
      naError("naEqualV1d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV2d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 2)
      naError("naEqualV2d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV3d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 3)
      naError("naEqualV3d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV4d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 4)
      naError("naEqualV4d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualV1f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV1f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 1)
      naError("naEqualV1f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV2f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 2)
      naError("naEqualV2f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV3f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 3)
      naError("naEqualV3f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV4f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 4)
      naError("naEqualV4f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualV1dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 1))
      naError("naEqualV1dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 2))
      naError("naEqualV2dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 3))
      naError("naEqualV3dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 4))
      naError("naEqualV4dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualV1fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 1))
      naError("naEqualV1fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 2))
      naError("naEqualV2fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 3))
      naError("naEqualV3fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 4))
      naError("naEqualV4fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
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
