
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Equal
// ///////////////////////////////

NA_IDEF NABool naEqualM11d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 1, 1))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 4, 4))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 9, 9))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 16, 16))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
}
NA_IDEF NABool naEqualM11f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 1, 1))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 4, 4))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 9, 9))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("The two pointers are equal. Use E variant or restrict pointers.");
    else if(naElementOverlap(a, b, 16, 16))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
}
NA_IDEF NABool naEqualM11dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 1, 1)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 4, 4)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 9, 9)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 16, 16)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
}
NA_IDEF NABool naEqualM11fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 1, 1)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 4, 4)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 9, 9)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naElementOverlap(a, b, 16, 16)))
      naError("b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
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
