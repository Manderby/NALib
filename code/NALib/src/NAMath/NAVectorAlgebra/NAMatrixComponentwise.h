
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Invert componentwise
// ///////////////////////////////

NA_IDEF void naInvCompM22d (double* NA_RESTRICT D, const double* NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("S equals D. Use S or E variant");
    if(naElementOverlap(S, D, 4, 4))
      naError("S overlaps with D.");
  #endif
  D[0] = naInv(S[0]); D[2] = naInv(S[2]);
  D[1] = naInv(S[1]); D[3] = naInv(S[3]);
}
NA_IDEF void naInvCompM33d (double* NA_RESTRICT D, const double* NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("S equals D. Use S or E variant");
    if(naElementOverlap(S, D, 9, 9))
      naError("S overlaps with D.");
  #endif
  D[0] = naInv(S[0]); D[3] = naInv(S[3]); D[6] = naInv(S[6]);
  D[1] = naInv(S[1]); D[4] = naInv(S[4]); D[7] = naInv(S[7]);
  D[2] = naInv(S[2]); D[5] = naInv(S[5]); D[8] = naInv(S[8]);
}
NA_IDEF void naInvCompM44d (double* NA_RESTRICT D, const double* NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("S equals D. Use S or E variant");
    if(naElementOverlap(S, D, 16, 16))
      naError("S overlaps with D.");
  #endif
  D[0] = naInv(S[0]); D[4] = naInv(S[4]); D[ 8] = naInv(S[ 8]); D[12] = naInv(S[12]);
  D[1] = naInv(S[1]); D[5] = naInv(S[5]); D[ 9] = naInv(S[ 9]); D[13] = naInv(S[13]);
  D[2] = naInv(S[2]); D[6] = naInv(S[6]); D[10] = naInv(S[10]); D[14] = naInv(S[14]);
  D[3] = naInv(S[3]); D[7] = naInv(S[7]); D[11] = naInv(S[11]); D[15] = naInv(S[15]);
}
NA_IDEF void naInvCompM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("S equals D. Use S or E variant");
    if(naElementOverlap(S, D, 4, 4))
      naError("S overlaps with D.");
  #endif
  D[0] = naInvf(S[0]); D[2] = naInvf(S[2]);
  D[1] = naInvf(S[1]); D[3] = naInvf(S[3]);
}
NA_IDEF void naInvCompM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("S equals D. Use S or E variant");
    if(naElementOverlap(S, D, 9, 9))
      naError("S overlaps with D.");
  #endif
  D[0] = naInvf(S[0]); D[3] = naInvf(S[3]); D[6] = naInvf(S[6]);
  D[1] = naInvf(S[1]); D[4] = naInvf(S[4]); D[7] = naInvf(S[7]);
  D[2] = naInvf(S[2]); D[5] = naInvf(S[5]); D[8] = naInvf(S[8]);
}
NA_IDEF void naInvCompM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("S equals D. Use S or E variant");
    if(naElementOverlap(S, D, 16, 16))
      naError("S overlaps with D.");
  #endif
  D[0] = naInvf(S[0]); D[4] = naInvf(S[4]); D[ 8] = naInvf(S[ 8]); D[12] = naInvf(S[12]);
  D[1] = naInvf(S[1]); D[5] = naInvf(S[5]); D[ 9] = naInvf(S[ 9]); D[13] = naInvf(S[13]);
  D[2] = naInvf(S[2]); D[6] = naInvf(S[6]); D[10] = naInvf(S[10]); D[14] = naInvf(S[14]);
  D[3] = naInvf(S[3]); D[7] = naInvf(S[7]); D[11] = naInvf(S[11]); D[15] = naInvf(S[15]);
}
NA_IDEF void naInvCompM22dE(double* D, const double* S){
  #ifndef NDEBUG
    if((S != D) && naElementOverlap(S, D, 4, 4))
      naError("S overlaps misaligned with D.");
  #endif
  D[0] = naInv(S[0]); D[2] = naInv(S[2]);
  D[1] = naInv(S[1]); D[3] = naInv(S[3]);
}
NA_IDEF void naInvCompM33dE(double* D, const double* S){
  #ifndef NDEBUG
    if((S != D) && naElementOverlap(S, D, 9, 9))
      naError("S overlaps misaligned with D.");
  #endif
  D[0] = naInv(S[0]); D[3] = naInv(S[3]); D[6] = naInv(S[6]);
  D[1] = naInv(S[1]); D[4] = naInv(S[4]); D[7] = naInv(S[7]);
  D[2] = naInv(S[2]); D[5] = naInv(S[5]); D[8] = naInv(S[8]);
}
NA_IDEF void naInvCompM44dE(double* D, const double* S){
  #ifndef NDEBUG
    if((S != D) && naElementOverlap(S, D, 16, 16))
      naError("S overlaps misaligned with D.");
  #endif
  D[0] = naInv(S[0]); D[4] = naInv(S[4]); D[ 8] = naInv(S[ 8]); D[12] = naInv(S[12]);
  D[1] = naInv(S[1]); D[5] = naInv(S[5]); D[ 9] = naInv(S[ 9]); D[13] = naInv(S[13]);
  D[2] = naInv(S[2]); D[6] = naInv(S[6]); D[10] = naInv(S[10]); D[14] = naInv(S[14]);
  D[3] = naInv(S[3]); D[7] = naInv(S[7]); D[11] = naInv(S[11]); D[15] = naInv(S[15]);
}
NA_IDEF void naInvCompM22fE(float*  D, const float*  S){
  #ifndef NDEBUG
    if((S != D) && naElementOverlap(S, D, 4, 4))
      naError("S overlaps misaligned with D.");
  #endif
  D[0] = naInvf(S[0]); D[2] = naInvf(S[2]);
  D[1] = naInvf(S[1]); D[3] = naInvf(S[3]);
}
NA_IDEF void naInvCompM33fE(float*  D, const float*  S){
  #ifndef NDEBUG
    if((S != D) && naElementOverlap(S, D, 9, 9))
      naError("S overlaps misaligned with D.");
  #endif
  D[0] = naInvf(S[0]); D[3] = naInvf(S[3]); D[6] = naInvf(S[6]);
  D[1] = naInvf(S[1]); D[4] = naInvf(S[4]); D[7] = naInvf(S[7]);
  D[2] = naInvf(S[2]); D[5] = naInvf(S[5]); D[8] = naInvf(S[8]);
}
NA_IDEF void naInvCompM44fE(float*  D, const float*  S){
  #ifndef NDEBUG
    if((S != D) && naElementOverlap(S, D, 16, 16))
      naError("S overlaps misaligned with D.");
  #endif
  D[0] = naInvf(S[0]); D[4] = naInvf(S[4]); D[ 8] = naInvf(S[ 8]); D[12] = naInvf(S[12]);
  D[1] = naInvf(S[1]); D[5] = naInvf(S[5]); D[ 9] = naInvf(S[ 9]); D[13] = naInvf(S[13]);
  D[2] = naInvf(S[2]); D[6] = naInvf(S[6]); D[10] = naInvf(S[10]); D[14] = naInvf(S[14]);
  D[3] = naInvf(S[3]); D[7] = naInvf(S[7]); D[11] = naInvf(S[11]); D[15] = naInvf(S[15]);
}
NA_IDEF void naInvCompM22dS(double* D){
  D[0] = naInv(D[0]); D[2] = naInv(D[2]);
  D[1] = naInv(D[1]); D[3] = naInv(D[3]);
}
NA_IDEF void naInvCompM33dS(double* D){
  D[0] = naInv(D[0]); D[3] = naInv(D[3]); D[6] = naInv(D[6]);
  D[1] = naInv(D[1]); D[4] = naInv(D[4]); D[7] = naInv(D[7]);
  D[2] = naInv(D[2]); D[5] = naInv(D[5]); D[8] = naInv(D[8]);
}
NA_IDEF void naInvCompM44dS(double* D){
  D[0] = naInv(D[0]); D[4] = naInv(D[4]); D[ 8] = naInv(D[ 8]); D[12] = naInv(D[12]);
  D[1] = naInv(D[1]); D[5] = naInv(D[5]); D[ 9] = naInv(D[ 9]); D[13] = naInv(D[13]);
  D[2] = naInv(D[2]); D[6] = naInv(D[6]); D[10] = naInv(D[10]); D[14] = naInv(D[14]);
  D[3] = naInv(D[3]); D[7] = naInv(D[7]); D[11] = naInv(D[11]); D[15] = naInv(D[15]);
}
NA_IDEF void naInvCompM22fS(float*  D){
  D[0] = naInvf(D[0]); D[2] = naInvf(D[2]);
  D[1] = naInvf(D[1]); D[3] = naInvf(D[3]);
}
NA_IDEF void naInvCompM33fS(float*  D){
  D[0] = naInvf(D[0]); D[3] = naInvf(D[3]); D[6] = naInvf(D[6]);
  D[1] = naInvf(D[1]); D[4] = naInvf(D[4]); D[7] = naInvf(D[7]);
  D[2] = naInvf(D[2]); D[5] = naInvf(D[5]); D[8] = naInvf(D[8]);
}
NA_IDEF void naInvCompM44fS(float*  D){
  D[0] = naInvf(D[0]); D[4] = naInvf(D[4]); D[ 8] = naInvf(D[ 8]); D[12] = naInvf(D[12]);
  D[1] = naInvf(D[1]); D[5] = naInvf(D[5]); D[ 9] = naInvf(D[ 9]); D[13] = naInvf(D[13]);
  D[2] = naInvf(D[2]); D[6] = naInvf(D[6]); D[10] = naInvf(D[10]); D[14] = naInvf(D[14]);
  D[3] = naInvf(D[3]); D[7] = naInvf(D[7]); D[11] = naInvf(D[11]); D[15] = naInvf(D[15]);
}



// ///////////////////////////////
// Multiply componentwise with scalar
// ///////////////////////////////

NA_IDEF void naMulCompM22d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naMulCompM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naMulCompM22dE(double*  D, const double*  A, double s){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33dE(double*  D, const double*  A, double s){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44dE(double* D, const double* A, double s){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naMulCompM22fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naMulCompM22dS(double*  D, double s){
  D[0] *= s; D[2] *= s;
  D[1] *= s; D[3] *= s;
}
NA_IDEF void naMulCompM33dS(double*  D, double s){
  D[0] *= s; D[3] *= s; D[6] *= s;
  D[1] *= s; D[4] *= s; D[7] *= s;
  D[2] *= s; D[5] *= s; D[8] *= s;
}
NA_IDEF void naMulCompM44dS(double* D, double s){
  D[0] *= s; D[4] *= s; D[ 8] *= s; D[12] *= s;
  D[1] *= s; D[5] *= s; D[ 9] *= s; D[13] *= s;
  D[2] *= s; D[6] *= s; D[10] *= s; D[14] *= s;
  D[3] *= s; D[7] *= s; D[11] *= s; D[15] *= s;
}
NA_IDEF void naMulCompM22fS(float* D, float s){
  D[0] *= s; D[2] *= s;
  D[1] *= s; D[3] *= s;
}
NA_IDEF void naMulCompM33fS(float* D, float s){
  D[0] *= s; D[3] *= s; D[6] *= s;
  D[1] *= s; D[4] *= s; D[7] *= s;
  D[2] *= s; D[5] *= s; D[8] *= s;
}
NA_IDEF void naMulCompM44fS(float* D, float s){
  D[0] *= s; D[4] *= s; D[ 8] *= s; D[12] *= s;
  D[1] *= s; D[5] *= s; D[ 9] *= s; D[13] *= s;
  D[2] *= s; D[6] *= s; D[10] *= s; D[14] *= s;
  D[3] *= s; D[7] *= s; D[11] *= s; D[15] *= s;
}




// ///////////////////////////////
// Multiply componentwise with vector
// ///////////////////////////////

NA_IDEF void naMulCompM22dV2d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33dV3d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44dV4d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22fV2f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33fV3f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44fV4f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22dV2dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33dV3dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44dV4dE( double* D, const double* A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22fV2fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33fV3fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44fV4fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22dV2dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] *= v[0]; D[2] *= v[1];
  D[1] *= v[0]; D[3] *= v[1];
}
NA_IDEF void naMulCompM33dV3dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] *= v[0]; D[3] *= v[1]; D[6] *= v[2];
  D[1] *= v[0]; D[4] *= v[1]; D[7] *= v[2];
  D[2] *= v[0]; D[5] *= v[1]; D[8] *= v[2];
}
NA_IDEF void naMulCompM44dV4dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] *= v[0]; D[4] *= v[1]; D[ 8] *= v[2]; D[12] *= v[3];
  D[1] *= v[0]; D[5] *= v[1]; D[ 9] *= v[2]; D[13] *= v[3];
  D[2] *= v[0]; D[6] *= v[1]; D[10] *= v[2]; D[14] *= v[3];
  D[3] *= v[0]; D[7] *= v[1]; D[11] *= v[2]; D[15] *= v[3];
}
NA_IDEF void naMulCompM22fV2fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] *= v[0]; D[2] *= v[1];
  D[1] *= v[0]; D[3] *= v[1];
}
NA_IDEF void naMulCompM33fV3fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] *= v[0]; D[3] *= v[1]; D[6] *= v[2];
  D[1] *= v[0]; D[4] *= v[1]; D[7] *= v[2];
  D[2] *= v[0]; D[5] *= v[1]; D[8] *= v[2];
}
NA_IDEF void naMulCompM44fV4fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] *= v[0]; D[4] *= v[1]; D[ 8] *= v[2]; D[12] *= v[3];
  D[1] *= v[0]; D[5] *= v[1]; D[ 9] *= v[2]; D[13] *= v[3];
  D[2] *= v[0]; D[6] *= v[1]; D[10] *= v[2]; D[14] *= v[3];
  D[3] *= v[0]; D[7] *= v[1]; D[11] *= v[2]; D[15] *= v[3];
}





// ///////////////////////////////
// Division componentwise with vector
// ///////////////////////////////

NA_IDEF void naDivCompM22dV2d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[2] = A[2]*naInv(v[1]);
  D[1] = A[1]*naInv(v[0]); D[3] = A[3]*naInv(v[1]);
}
NA_IDEF void naDivCompM33dV3d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[3] = A[3]*naInv(v[1]); D[6] = A[6]*naInv(v[2]);
  D[1] = A[1]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[7] = A[7]*naInv(v[2]);
  D[2] = A[2]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[8] = A[8]*naInv(v[2]);
}
NA_IDEF void naDivCompM44dV4d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[ 8] = A[ 8]*naInv(v[2]); D[12] = A[12]*naInv(v[3]);
  D[1] = A[1]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[ 9] = A[ 9]*naInv(v[2]); D[13] = A[13]*naInv(v[3]);
  D[2] = A[2]*naInv(v[0]); D[6] = A[6]*naInv(v[1]); D[10] = A[10]*naInv(v[2]); D[14] = A[14]*naInv(v[3]);
  D[3] = A[3]*naInv(v[0]); D[7] = A[7]*naInv(v[1]); D[11] = A[11]*naInv(v[2]); D[15] = A[15]*naInv(v[3]);
}
NA_IDEF void naDivCompM22fV2f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[2] = A[2]*naInvf(v[1]);
  D[1] = A[1]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]);
}
NA_IDEF void naDivCompM33fV3f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]); D[6] = A[6]*naInvf(v[2]);
  D[1] = A[1]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[7] = A[7]*naInvf(v[2]);
  D[2] = A[2]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[8] = A[8]*naInvf(v[2]);
}
NA_IDEF void naDivCompM44fV4f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[ 8] = A[ 8]*naInvf(v[2]); D[12] = A[12]*naInvf(v[3]);
  D[1] = A[1]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[ 9] = A[ 9]*naInvf(v[2]); D[13] = A[13]*naInvf(v[3]);
  D[2] = A[2]*naInvf(v[0]); D[6] = A[6]*naInvf(v[1]); D[10] = A[10]*naInvf(v[2]); D[14] = A[14]*naInvf(v[3]);
  D[3] = A[3]*naInvf(v[0]); D[7] = A[7]*naInvf(v[1]); D[11] = A[11]*naInvf(v[2]); D[15] = A[15]*naInvf(v[3]);
}
NA_IDEF void naDivCompM22dV2dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[2] = A[2]*naInv(v[1]);
  D[1] = A[1]*naInv(v[0]); D[3] = A[3]*naInv(v[1]);
}
NA_IDEF void naDivCompM33dV3dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[3] = A[3]*naInv(v[1]); D[6] = A[6]*naInv(v[2]);
  D[1] = A[1]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[7] = A[7]*naInv(v[2]);
  D[2] = A[2]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[8] = A[8]*naInv(v[2]);
}
NA_IDEF void naDivCompM44dV4dE( double* D, const double* A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[ 8] = A[ 8]*naInv(v[2]); D[12] = A[12]*naInv(v[3]);
  D[1] = A[1]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[ 9] = A[ 9]*naInv(v[2]); D[13] = A[13]*naInv(v[3]);
  D[2] = A[2]*naInv(v[0]); D[6] = A[6]*naInv(v[1]); D[10] = A[10]*naInv(v[2]); D[14] = A[14]*naInv(v[3]);
  D[3] = A[3]*naInv(v[0]); D[7] = A[7]*naInv(v[1]); D[11] = A[11]*naInv(v[2]); D[15] = A[15]*naInv(v[3]);
}
NA_IDEF void naDivCompM22fV2fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[2] = A[2]*naInvf(v[1]);
  D[1] = A[1]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]);
}
NA_IDEF void naDivCompM33fV3fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]); D[6] = A[6]*naInvf(v[2]);
  D[1] = A[1]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[7] = A[7]*naInvf(v[2]);
  D[2] = A[2]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[8] = A[8]*naInvf(v[2]);
}
NA_IDEF void naDivCompM44fV4fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[ 8] = A[ 8]*naInvf(v[2]); D[12] = A[12]*naInvf(v[3]);
  D[1] = A[1]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[ 9] = A[ 9]*naInvf(v[2]); D[13] = A[13]*naInvf(v[3]);
  D[2] = A[2]*naInvf(v[0]); D[6] = A[6]*naInvf(v[1]); D[10] = A[10]*naInvf(v[2]); D[14] = A[14]*naInvf(v[3]);
  D[3] = A[3]*naInvf(v[0]); D[7] = A[7]*naInvf(v[1]); D[11] = A[11]*naInvf(v[2]); D[15] = A[15]*naInvf(v[3]);
}
NA_IDEF void naDivCompM22dV2dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] *= naInv(v[0]); D[2] *= naInv(v[1]);
  D[1] *= naInv(v[0]); D[3] *= naInv(v[1]);
}
NA_IDEF void naDivCompM33dV3dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] *= naInv(v[0]); D[3] *= naInv(v[1]); D[6] *= naInv(v[2]);
  D[1] *= naInv(v[0]); D[4] *= naInv(v[1]); D[7] *= naInv(v[2]);
  D[2] *= naInv(v[0]); D[5] *= naInv(v[1]); D[8] *= naInv(v[2]);
}
NA_IDEF void naDivCompM44dV4dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] *= naInv(v[0]); D[4] *= naInv(v[1]); D[ 8] *= naInv(v[2]); D[12] *= naInv(v[3]);
  D[1] *= naInv(v[0]); D[5] *= naInv(v[1]); D[ 9] *= naInv(v[2]); D[13] *= naInv(v[3]);
  D[2] *= naInv(v[0]); D[6] *= naInv(v[1]); D[10] *= naInv(v[2]); D[14] *= naInv(v[3]);
  D[3] *= naInv(v[0]); D[7] *= naInv(v[1]); D[11] *= naInv(v[2]); D[15] *= naInv(v[3]);
}
NA_IDEF void naDivCompM22fV2fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 4, 2))
      naError("v overlaps with D.");
  #endif
  D[0] *= naInvf(v[0]); D[2] *= naInvf(v[1]);
  D[1] *= naInvf(v[0]); D[3] *= naInvf(v[1]);
}
NA_IDEF void naDivCompM33fV3fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 9, 3))
      naError("v overlaps with D.");
  #endif
  D[0] *= naInvf(v[0]); D[3] *= naInvf(v[1]); D[6] *= naInvf(v[2]);
  D[1] *= naInvf(v[0]); D[4] *= naInvf(v[1]); D[7] *= naInvf(v[2]);
  D[2] *= naInvf(v[0]); D[5] *= naInvf(v[1]); D[8] *= naInvf(v[2]);
}
NA_IDEF void naDivCompM44fV4fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naElementOverlap(D, v, 16, 4))
      naError("v overlaps with D.");
  #endif
  D[0] *= naInvf(v[0]); D[4] *= naInvf(v[1]); D[ 8] *= naInvf(v[2]); D[12] *= naInvf(v[3]);
  D[1] *= naInvf(v[0]); D[5] *= naInvf(v[1]); D[ 9] *= naInvf(v[2]); D[13] *= naInvf(v[3]);
  D[2] *= naInvf(v[0]); D[6] *= naInvf(v[1]); D[10] *= naInvf(v[2]); D[14] *= naInvf(v[3]);
  D[3] *= naInvf(v[0]); D[7] *= naInvf(v[1]); D[11] *= naInvf(v[2]); D[15] *= naInvf(v[3]);
}



// ///////////////////////////////
// Multiply componentwise with matrix
// ///////////////////////////////

NA_IDEF void naMulCompM22dM22d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33dM33d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44dM44d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22fM22f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33fM33f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44fM44f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22dM22dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33dM33dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44dM44dE( double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22fM22fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33fM33fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44fM44fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22dM22dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] *= B[0]; D[2] *= B[2];
  D[1] *= B[1]; D[3] *= B[3];
}
NA_IDEF void naMulCompM33dM33dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] *= B[0]; D[3] *= B[3]; D[6] *= B[6];
  D[1] *= B[1]; D[4] *= B[4]; D[7] *= B[7];
  D[2] *= B[2]; D[5] *= B[5]; D[8] *= B[8];
}
NA_IDEF void naMulCompM44dM44dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] *= B[0]; D[4] *= B[4]; D[ 8] *= B[ 8]; D[12] *= B[12];
  D[1] *= B[1]; D[5] *= B[5]; D[ 9] *= B[ 9]; D[13] *= B[13];
  D[2] *= B[2]; D[6] *= B[6]; D[10] *= B[10]; D[14] *= B[14];
  D[3] *= B[3]; D[7] *= B[7]; D[11] *= B[11]; D[15] *= B[15];
}
NA_IDEF void naMulCompM22fM22fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] *= B[0]; D[2] *= B[2];
  D[1] *= B[1]; D[3] *= B[3];
}
NA_IDEF void naMulCompM33fM33fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] *= B[0]; D[3] *= B[3]; D[6] *= B[6];
  D[1] *= B[1]; D[4] *= B[4]; D[7] *= B[7];
  D[2] *= B[2]; D[5] *= B[5]; D[8] *= B[8];
}
NA_IDEF void naMulCompM44fM44fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] *= B[0]; D[4] *= B[4]; D[ 8] *= B[ 8]; D[12] *= B[12];
  D[1] *= B[1]; D[5] *= B[5]; D[ 9] *= B[ 9]; D[13] *= B[13];
  D[2] *= B[2]; D[6] *= B[6]; D[10] *= B[10]; D[14] *= B[14];
  D[3] *= B[3]; D[7] *= B[7]; D[11] *= B[11]; D[15] *= B[15];
}





// ///////////////////////////////
// Division componentwise with matrix
// ///////////////////////////////


NA_IDEF void naDivCompM22dM22d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[2] = A[2]*naInv(B[2]);
  D[1] = A[1]*naInv(B[1]); D[3] = A[3]*naInv(B[3]);
}
NA_IDEF void naDivCompM33dM33d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[3] = A[3]*naInv(B[3]); D[6] = A[6]*naInv(B[6]);
  D[1] = A[1]*naInv(B[1]); D[4] = A[4]*naInv(B[4]); D[7] = A[7]*naInv(B[7]);
  D[2] = A[2]*naInv(B[2]); D[5] = A[5]*naInv(B[5]); D[8] = A[8]*naInv(B[8]);
}
NA_IDEF void naDivCompM44dM44d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[4] = A[4]*naInv(B[4]); D[ 8] = A[ 8]*naInv(B[ 8]); D[12] = A[12]*naInv(B[12]);
  D[1] = A[1]*naInv(B[1]); D[5] = A[5]*naInv(B[5]); D[ 9] = A[ 9]*naInv(B[ 9]); D[13] = A[13]*naInv(B[13]);
  D[2] = A[2]*naInv(B[2]); D[6] = A[6]*naInv(B[6]); D[10] = A[10]*naInv(B[10]); D[14] = A[14]*naInv(B[14]);
  D[3] = A[3]*naInv(B[3]); D[7] = A[7]*naInv(B[7]); D[11] = A[11]*naInv(B[11]); D[15] = A[15]*naInv(B[15]);
}
NA_IDEF void naDivCompM22fM22f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[2] = A[2]*naInvf(B[2]);
  D[1] = A[1]*naInvf(B[1]); D[3] = A[3]*naInvf(B[3]);
}
NA_IDEF void naDivCompM33fM33f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[3] = A[3]*naInvf(B[3]); D[6] = A[6]*naInvf(B[6]);
  D[1] = A[1]*naInvf(B[1]); D[4] = A[4]*naInvf(B[4]); D[7] = A[7]*naInvf(B[7]);
  D[2] = A[2]*naInvf(B[2]); D[5] = A[5]*naInvf(B[5]); D[8] = A[8]*naInvf(B[8]);
}
NA_IDEF void naDivCompM44fM44f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("A equals D. Use E or S variant");
    else if(naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[4] = A[4]*naInvf(B[4]); D[ 8] = A[ 8]*naInvf(B[ 8]); D[12] = A[12]*naInvf(B[12]);
  D[1] = A[1]*naInvf(B[1]); D[5] = A[5]*naInvf(B[5]); D[ 9] = A[ 9]*naInvf(B[ 9]); D[13] = A[13]*naInvf(B[13]);
  D[2] = A[2]*naInvf(B[2]); D[6] = A[6]*naInvf(B[6]); D[10] = A[10]*naInvf(B[10]); D[14] = A[14]*naInvf(B[14]);
  D[3] = A[3]*naInvf(B[3]); D[7] = A[7]*naInvf(B[7]); D[11] = A[11]*naInvf(B[11]); D[15] = A[15]*naInvf(B[15]);
}
NA_IDEF void naDivCompM22dM22dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[2] = A[2]*naInv(B[2]);
  D[1] = A[1]*naInv(B[1]); D[3] = A[3]*naInv(B[3]);
}
NA_IDEF void naDivCompM33dM33dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[3] = A[3]*naInv(B[3]); D[6] = A[6]*naInv(B[6]);
  D[1] = A[1]*naInv(B[1]); D[4] = A[4]*naInv(B[4]); D[7] = A[7]*naInv(B[7]);
  D[2] = A[2]*naInv(B[2]); D[5] = A[5]*naInv(B[5]); D[8] = A[8]*naInv(B[8]);
}
NA_IDEF void naDivCompM44dM44dE( double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[4] = A[4]*naInv(B[4]); D[ 8] = A[ 8]*naInv(B[ 8]); D[12] = A[12]*naInv(B[12]);
  D[1] = A[1]*naInv(B[1]); D[5] = A[5]*naInv(B[5]); D[ 9] = A[ 9]*naInv(B[ 9]); D[13] = A[13]*naInv(B[13]);
  D[2] = A[2]*naInv(B[2]); D[6] = A[6]*naInv(B[6]); D[10] = A[10]*naInv(B[10]); D[14] = A[14]*naInv(B[14]);
  D[3] = A[3]*naInv(B[3]); D[7] = A[7]*naInv(B[7]); D[11] = A[11]*naInv(B[11]); D[15] = A[15]*naInv(B[15]);
}
NA_IDEF void naDivCompM22fM22fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 4, 4))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[2] = A[2]*naInvf(B[2]);
  D[1] = A[1]*naInvf(B[1]); D[3] = A[3]*naInvf(B[3]);
}
NA_IDEF void naDivCompM33fM33fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 9, 9))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[3] = A[3]*naInvf(B[3]); D[6] = A[6]*naInvf(B[6]);
  D[1] = A[1]*naInvf(B[1]); D[4] = A[4]*naInvf(B[4]); D[7] = A[7]*naInvf(B[7]);
  D[2] = A[2]*naInvf(B[2]); D[5] = A[5]*naInvf(B[5]); D[8] = A[8]*naInvf(B[8]);
}
NA_IDEF void naDivCompM44fM44fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naElementOverlap(A, D, 16, 16))
      naError("A overlaps misaligned with D.");
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[4] = A[4]*naInvf(B[4]); D[ 8] = A[ 8]*naInvf(B[ 8]); D[12] = A[12]*naInvf(B[12]);
  D[1] = A[1]*naInvf(B[1]); D[5] = A[5]*naInvf(B[5]); D[ 9] = A[ 9]*naInvf(B[ 9]); D[13] = A[13]*naInvf(B[13]);
  D[2] = A[2]*naInvf(B[2]); D[6] = A[6]*naInvf(B[6]); D[10] = A[10]*naInvf(B[10]); D[14] = A[14]*naInvf(B[14]);
  D[3] = A[3]*naInvf(B[3]); D[7] = A[7]*naInvf(B[7]); D[11] = A[11]*naInvf(B[11]); D[15] = A[15]*naInvf(B[15]);
}
NA_IDEF void naDivCompM22dM22dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] *= naInv(B[0]); D[2] *= naInv(B[2]);
  D[1] *= naInv(B[1]); D[3] *= naInv(B[3]);
}
NA_IDEF void naDivCompM33dM33dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] *= naInv(B[0]); D[3] *= naInv(B[3]); D[6] *= naInv(B[6]);
  D[1] *= naInv(B[1]); D[4] *= naInv(B[4]); D[7] *= naInv(B[7]);
  D[2] *= naInv(B[2]); D[5] *= naInv(B[5]); D[8] *= naInv(B[8]);
}
NA_IDEF void naDivCompM44dM44dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] *= naInv(B[0]); D[4] *= naInv(B[4]); D[ 8] *= naInv(B[ 8]); D[12] *= naInv(B[12]);
  D[1] *= naInv(B[1]); D[5] *= naInv(B[5]); D[ 9] *= naInv(B[ 9]); D[13] *= naInv(B[13]);
  D[2] *= naInv(B[2]); D[6] *= naInv(B[6]); D[10] *= naInv(B[10]); D[14] *= naInv(B[14]);
  D[3] *= naInv(B[3]); D[7] *= naInv(B[7]); D[11] *= naInv(B[11]); D[15] *= naInv(B[15]);
}
NA_IDEF void naDivCompM22fM22fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 4, 4))
      naError("B overlaps with D.");
  #endif
  D[0] *= naInvf(B[0]); D[2] *= naInvf(B[2]);
  D[1] *= naInvf(B[1]); D[3] *= naInvf(B[3]);
}
NA_IDEF void naDivCompM33fM33fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 9, 9))
      naError("B overlaps with D.");
  #endif
  D[0] *= naInvf(B[0]); D[3] *= naInvf(B[3]); D[6] *= naInvf(B[6]);
  D[1] *= naInvf(B[1]); D[4] *= naInvf(B[4]); D[7] *= naInvf(B[7]);
  D[2] *= naInvf(B[2]); D[5] *= naInvf(B[5]); D[8] *= naInvf(B[8]);
}
NA_IDEF void naDivCompM44fM44fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naElementOverlap(D, B, 16, 16))
      naError("B overlaps with D.");
  #endif
  D[0] *= naInvf(B[0]); D[4] *= naInvf(B[4]); D[ 8] *= naInvf(B[ 8]); D[12] *= naInvf(B[12]);
  D[1] *= naInvf(B[1]); D[5] *= naInvf(B[5]); D[ 9] *= naInvf(B[ 9]); D[13] *= naInvf(B[13]);
  D[2] *= naInvf(B[2]); D[6] *= naInvf(B[6]); D[10] *= naInvf(B[10]); D[14] *= naInvf(B[14]);
  D[3] *= naInvf(B[3]); D[7] *= naInvf(B[7]); D[11] *= naInvf(B[11]); D[15] *= naInvf(B[15]);
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
