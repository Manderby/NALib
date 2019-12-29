
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Fill Row First
// ///////////////////////////////

NA_IDEF void naFillM22dRowFirst( double*  D,
   double s0 , double s1 , double s2 , double s3){
  D[0] = s0; D[2] = s1;
  D[1] = s2; D[3] = s3;
}
NA_IDEF void naFillM33dRowFirst( double*  D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8){
  D[0] = s0; D[3] = s1; D[6] = s2;
  D[1] = s3; D[4] = s4; D[7] = s5;
  D[2] = s6; D[5] = s7; D[8] = s8;
}
NA_IDEF void naFillM44dRowFirst( double*  D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8 , double s9 , double s10, double s11,
   double s12, double s13, double s14, double s15){
  D[0] =  s0; D[4] =  s1; D[ 8] = s2 ; D[12] = s3;
  D[1] =  s4; D[5] =  s5; D[ 9] = s6 ; D[13] = s7;
  D[2] =  s8; D[6] =  s9; D[10] = s10; D[14] = s11;
  D[3] = s12; D[7] = s13; D[11] = s14; D[15] = s15;
}
NA_IDEF void naFillM22fRowFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3){
  D[0] = s0; D[2] = s1;
  D[1] = s2; D[3] = s3;
}
NA_IDEF void naFillM33fRowFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8){
  D[0] = s0; D[3] = s1; D[6] = s2;
  D[1] = s3; D[4] = s4; D[7] = s5;
  D[2] = s6; D[5] = s7; D[8] = s8;
}
NA_IDEF void naFillM44fRowFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8 , float  s9 , float  s10, float  s11,
   float  s12, float  s13, float  s14, float  s15){
  D[0] =  s0; D[4] =  s1; D[ 8] = s2 ; D[12] = s3;
  D[1] =  s4; D[5] =  s5; D[ 9] = s6 ; D[13] = s7;
  D[2] =  s8; D[6] =  s9; D[10] = s10; D[14] = s11;
  D[3] = s12; D[7] = s13; D[11] = s14; D[15] = s15;
}



// ///////////////////////////////
// Fill Column First
// ///////////////////////////////


NA_IDEF void naFillM22dColumnFirst( double*  D,
   double s0 , double s1 , double s2 , double s3){
  D[0] = s0; D[2] = s2;
  D[1] = s1; D[3] = s3;
}
NA_IDEF void naFillM33dColumnFirst( double*  D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8){
  D[0] = s0; D[3] = s3; D[6] = s6;
  D[1] = s1; D[4] = s4; D[7] = s7;
  D[2] = s2; D[5] = s5; D[8] = s8;
}
NA_IDEF void naFillM44dColumnFirst( double*  D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8 , double s9 , double s10, double s11,
   double s12, double s13, double s14, double s15){
  D[0] = s0; D[4] = s4; D[ 8] = s8 ; D[12] = s12;
  D[1] = s1; D[5] = s5; D[ 9] = s9 ; D[13] = s13;
  D[2] = s2; D[6] = s6; D[10] = s10; D[14] = s14;
  D[3] = s3; D[7] = s7; D[11] = s11; D[15] = s15;
}
NA_IDEF void naFillM22fColumnFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3){
  D[0] = s0; D[2] = s2;
  D[1] = s1; D[3] = s3;
}
NA_IDEF void naFillM33fColumnFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8){
  D[0] = s0; D[3] = s3; D[6] = s6;
  D[1] = s1; D[4] = s4; D[7] = s7;
  D[2] = s2; D[5] = s5; D[8] = s8;
}
NA_IDEF void naFillM44fColumnFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8 , float  s9 , float  s10, float  s11,
   float  s12, float  s13, float  s14, float  s15){
  D[0] = s0; D[4] = s4; D[ 8] = s8 ; D[12] = s12;
  D[1] = s1; D[5] = s5; D[ 9] = s9 ; D[13] = s13;
  D[2] = s2; D[6] = s6; D[10] = s10; D[14] = s14;
  D[3] = s3; D[7] = s7; D[11] = s11; D[15] = s15;
}



// ///////////////////////////////
// Fill with Axis
// ///////////////////////////////

NA_IDEF void naFillM22dWithAxis(double* NA_RESTRICT D,
                         const double* NA_RESTRICT v0,
                         const double* NA_RESTRICT v1){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-v0)))
      naError("v0 overlaps with D");
    if(naInsidei(-3, 1, (NASizeInt)(D-v1)))
      naError("v1 overlaps with D");
  #endif
  naFillM22dColumnFirst(D,
                v0[0], v0[1],
                v1[0], v1[1]);
}
NA_IDEF void naFillM33dWithAxis(double* NA_RESTRICT D,
                         const double* NA_RESTRICT v0,
                         const double* NA_RESTRICT v1,
                         const double* NA_RESTRICT v2){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-v0)))
      naError("v0 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v1)))
      naError("v1 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v2)))
      naError("v2 overlaps with D");
  #endif
  naFillM33dColumnFirst(D,
                v0[0], v0[1], v0[2],
                v1[0], v1[1], v1[2],
                v2[0], v2[1], v2[2]);
}
NA_IDEF void naFillM44dWithAxis(double* NA_RESTRICT D,
                         const double* NA_RESTRICT v0,
                         const double* NA_RESTRICT v1,
                         const double* NA_RESTRICT v2,
                         const double* NA_RESTRICT v3){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-v0)))
      naError("v0 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v1)))
      naError("v1 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v2)))
      naError("v2 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v3)))
      naError("v3 overlaps with D");
  #endif
  naFillM44dColumnFirst(D,
                v0[0], v0[1], v0[2], v0[3],
                v1[0], v1[1], v1[2], v1[3],
                v2[0], v2[1], v2[2], v2[3],
                v3[0], v3[1], v3[2], v3[3]);
}
NA_IDEF void naFillM22fWithAxis(float* NA_RESTRICT D,
                          const float* NA_RESTRICT v0,
                          const float* NA_RESTRICT v1){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-v0)))
      naError("v0 overlaps with D");
    if(naInsidei(-3, 1, (NASizeInt)(D-v1)))
      naError("v1 overlaps with D");
  #endif
  naFillM22fColumnFirst(D,
                v0[0], v0[1],
                v1[0], v1[1]);
}
NA_IDEF void naFillM33fWithAxis(float* NA_RESTRICT D,
                          const float* NA_RESTRICT v0,
                          const float* NA_RESTRICT v1,
                          const float* NA_RESTRICT v2){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-v0)))
      naError("v0 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v1)))
      naError("v1 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v2)))
      naError("v2 overlaps with D");
  #endif
  naFillM33fColumnFirst(D,
                v0[0], v0[1], v0[2],
                v1[0], v1[1], v1[2],
                v2[0], v2[1], v2[2]);
}
NA_IDEF void naFillM44fWithAxis(float* NA_RESTRICT D,
                          const float* NA_RESTRICT v0,
                          const float* NA_RESTRICT v1,
                          const float* NA_RESTRICT v2,
                          const float* NA_RESTRICT v3){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-v0)))
      naError("v0 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v1)))
      naError("v1 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v2)))
      naError("v2 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v3)))
      naError("v3 overlaps with D");
  #endif
  naFillM44fColumnFirst(D,
                v0[0], v0[1], v0[2], v0[3],
                v1[0], v1[1], v1[2], v1[3],
                v2[0], v2[1], v2[2], v2[3],
                v3[0], v3[1], v3[2], v3[3]);
}



// ///////////////////////////////
// Fill With Diag Scalar
// ///////////////////////////////


NA_IDEF void naFillM22dWithDiag(double* D, double s){
  naFillM22dColumnFirst(D,
                s , 0.,
                0., s  );
}
NA_IDEF void naFillM33dWithDiag(double* D, double s){
  naFillM33dColumnFirst(D,
                s , 0., 0.,
                0., s , 0.,
                0., 0., s );
}
NA_IDEF void naFillM44dWithDiag(double* D, double s){
  naFillM44dColumnFirst(D,
                s , 0., 0., 0.,
                0., s , 0., 0.,
                0., 0., s , 0.,
                0., 0., 0., s );
}
NA_IDEF void naFillM22fWithDiag(float* D, float s){
  naFillM22fColumnFirst(D,
                s  , 0.f,
                0.f, s  );
}
NA_IDEF void naFillM33fWithDiag(float* D, float s){
  naFillM33fColumnFirst(D,
                s  , 0.f, 0.f,
                0.f, s  , 0.f,
                0.f, 0.f, s  );
}
NA_IDEF void naFillM44fWithDiag(float* D, float s){
  naFillM44fColumnFirst(D,
                s  , 0.f, 0.f, 0.f,
                0.f, s  , 0.f, 0.f,
                0.f, 0.f, s  , 0.f,
                0.f, 0.f, 0.f, s  );
}



// ///////////////////////////////
// Fill With Diag Vector
// ///////////////////////////////

NA_IDEF void naFillM22dWithDiagV2d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-diag)))
      naError("diag overlaps with D");
  #endif
  naFillM22dColumnFirst(D,
                diag[0], 0.     ,
                0.     , diag[1]);
}
NA_IDEF void naFillM33dWithDiagV3d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-diag)))
      naError("diag overlaps with D");
  #endif
  naFillM33dColumnFirst(D,
                diag[0], 0.     , 0.,
                0.     , diag[1], 0.,
                0.     , 0.     , diag[2]);
}
NA_IDEF void naFillM44dWithDiagV4d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-diag)))
      naError("diag overlaps with D");
  #endif
  naFillM44dColumnFirst(D,
                diag[0], 0.     , 0.     , 0.,
                0.     , diag[1], 0.     , 0.,
                0.     , 0.     , diag[2], 0.,
                0.     , 0.     , 0.     , diag[3]);
}
NA_IDEF void naFillM22fWithDiagV2f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-diag)))
      naError("diag overlaps with D");
  #endif
  naFillM22fColumnFirst(D,  diag[0], 0.f    ,
                0.f    , diag[1]);
}
NA_IDEF void naFillM33fWithDiagV3f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-diag)))
      naError("diag overlaps with D");
  #endif
  naFillM33fColumnFirst(D,
                diag[0], 0.f    , 0.f,
                0.f    , diag[1], 0.f,
                0.f    , 0.f    , diag[2]);
}
NA_IDEF void naFillM44fWithDiagV4f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-diag)))
      naError("diag overlaps with D");
  #endif
  naFillM44fColumnFirst(D,
                diag[0], 0.f    , 0.f    , 0.f,
                0.f    , diag[1], 0.f    , 0.f,
                0.f    , 0.f    , diag[2], 0.f,
                0.f    , 0.f    , 0.f    , diag[3]);
}



// ///////////////////////////////
// Copy
// ///////////////////////////////

NA_IDEF void naCopyM22d( double* NA_RESTRICT D, const double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33d( double* NA_RESTRICT D, const double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44d( double* NA_RESTRICT D, const double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22f( float* NA_RESTRICT D, const float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33f( float* NA_RESTRICT D, const float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44f( float* NA_RESTRICT D, const float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
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
