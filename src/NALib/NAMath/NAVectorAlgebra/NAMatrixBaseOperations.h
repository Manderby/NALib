
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"




// ///////////////////////////////
// Neg
// ///////////////////////////////

NA_IDEF void naNegM22d(double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM22d", "A is equal to D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naNegM22d", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33d(double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM33d", "A is equal to D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naNegM33d", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44d(double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM44d", "A is equal to D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naNegM44d", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[4] = -A[4]; D[ 8] = -A[ 8]; D[12] = -A[12];
  D[1] = -A[1]; D[5] = -A[5]; D[ 9] = -A[ 9]; D[13] = -A[13];
  D[2] = -A[2]; D[6] = -A[6]; D[10] = -A[10]; D[14] = -A[14];
  D[3] = -A[3]; D[7] = -A[7]; D[11] = -A[11]; D[15] = -A[15];
}
NA_IDEF void naNegM22f(float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM22f", "A is equal to D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naNegM22f", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33f(float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM33f", "A is equal to D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naNegM33f", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44f(float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM44f", "A is equal to D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naNegM44f", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[4] = -A[4]; D[ 8] = -A[ 8]; D[12] = -A[12];
  D[1] = -A[1]; D[5] = -A[5]; D[ 9] = -A[ 9]; D[13] = -A[13];
  D[2] = -A[2]; D[6] = -A[6]; D[10] = -A[10]; D[14] = -A[14];
  D[3] = -A[3]; D[7] = -A[7]; D[11] = -A[11]; D[15] = -A[15];
}
NA_IDEF void naNegM22dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naNegM22dE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naNegM33dE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naNegM44dE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[4] = -A[4]; D[ 8] = -A[ 8]; D[12] = -A[12];
  D[1] = -A[1]; D[5] = -A[5]; D[ 9] = -A[ 9]; D[13] = -A[13];
  D[2] = -A[2]; D[6] = -A[6]; D[10] = -A[10]; D[14] = -A[14];
  D[3] = -A[3]; D[7] = -A[7]; D[11] = -A[11]; D[15] = -A[15];
}
NA_IDEF void naNegM22fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naNegM22fE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naNegM33fE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naNegM44fE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[4] = -A[4]; D[ 8] = -A[ 8]; D[12] = -A[12];
  D[1] = -A[1]; D[5] = -A[5]; D[ 9] = -A[ 9]; D[13] = -A[13];
  D[2] = -A[2]; D[6] = -A[6]; D[10] = -A[10]; D[14] = -A[14];
  D[3] = -A[3]; D[7] = -A[7]; D[11] = -A[11]; D[15] = -A[15];
}
NA_IDEF void naNegM22dS( double* D){
  D[0] = -D[0]; D[2] = -D[2];
  D[1] = -D[1]; D[3] = -D[3];
}
NA_IDEF void naNegM33dS( double* D){
  D[0] = -D[0]; D[3] = -D[3]; D[6] = -D[6];
  D[1] = -D[1]; D[4] = -D[4]; D[7] = -D[7];
  D[2] = -D[2]; D[5] = -D[5]; D[8] = -D[8];
}
NA_IDEF void naNegM44dS( double* D){
  D[0] = -D[0]; D[4] = -D[4]; D[ 8] = -D[ 8]; D[12] = -D[12];
  D[1] = -D[1]; D[5] = -D[5]; D[ 9] = -D[ 9]; D[13] = -D[13];
  D[2] = -D[2]; D[6] = -D[6]; D[10] = -D[10]; D[14] = -D[14];
  D[3] = -D[3]; D[7] = -D[7]; D[11] = -D[11]; D[15] = -D[15];
}
NA_IDEF void naNegM22fS( float* D){
  D[0] = -D[0]; D[2] = -D[2];
  D[1] = -D[1]; D[3] = -D[3];
}
NA_IDEF void naNegM33fS( float* D){
  D[0] = -D[0]; D[3] = -D[3]; D[6] = -D[6];
  D[1] = -D[1]; D[4] = -D[4]; D[7] = -D[7];
  D[2] = -D[2]; D[5] = -D[5]; D[8] = -D[8];
}
NA_IDEF void naNegM44fS( float* D){
  D[0] = -D[0]; D[4] = -D[4]; D[ 8] = -D[ 8]; D[12] = -D[12];
  D[1] = -D[1]; D[5] = -D[5]; D[ 9] = -D[ 9]; D[13] = -D[13];
  D[2] = -D[2]; D[6] = -D[6]; D[10] = -D[10]; D[14] = -D[14];
  D[3] = -D[3]; D[7] = -D[7]; D[11] = -D[11]; D[15] = -D[15];
}



// ///////////////////////////////
// Add
// ///////////////////////////////

NA_IDEF void naAddM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM22d", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naAddM22d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM33d", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naAddM33d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM33d", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[3] = A[3]+B[3]; D[6] = A[6]+B[6];
  D[1] = A[1]+B[1]; D[4] = A[4]+B[4]; D[7] = A[7]+B[7];
  D[2] = A[2]+B[2]; D[5] = A[5]+B[5]; D[8] = A[8]+B[8];
}
NA_IDEF void naAddM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM44d", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naAddM44d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM44d", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[4] = A[4]+B[4]; D[ 8] = A[ 8]+B[ 8]; D[12] = A[12]+B[12];
  D[1] = A[1]+B[1]; D[5] = A[5]+B[5]; D[ 9] = A[ 9]+B[ 9]; D[13] = A[13]+B[13];
  D[2] = A[2]+B[2]; D[6] = A[6]+B[6]; D[10] = A[10]+B[10]; D[14] = A[14]+B[14];
  D[3] = A[3]+B[3]; D[7] = A[7]+B[7]; D[11] = A[11]+B[11]; D[15] = A[15]+B[15];
}
NA_IDEF void naAddM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM22f", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naAddM22f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22f", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM33f", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naAddM33f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM33f", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[3] = A[3]+B[3]; D[6] = A[6]+B[6];
  D[1] = A[1]+B[1]; D[4] = A[4]+B[4]; D[7] = A[7]+B[7];
  D[2] = A[2]+B[2]; D[5] = A[5]+B[5]; D[8] = A[8]+B[8];
}
NA_IDEF void naAddM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM44f", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naAddM44f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM44f", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[4] = A[4]+B[4]; D[ 8] = A[ 8]+B[ 8]; D[12] = A[12]+B[12];
  D[1] = A[1]+B[1]; D[5] = A[5]+B[5]; D[ 9] = A[ 9]+B[ 9]; D[13] = A[13]+B[13];
  D[2] = A[2]+B[2]; D[6] = A[6]+B[6]; D[10] = A[10]+B[10]; D[14] = A[14]+B[14];
  D[3] = A[3]+B[3]; D[7] = A[7]+B[7]; D[11] = A[11]+B[11]; D[15] = A[15]+B[15];
}
NA_IDEF void naAddM22dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naAddM22dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22dE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33dE(double*  D, const double*  A, const double*  B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naAddM33dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM33dE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[3] = A[3]+B[3]; D[6] = A[6]+B[6];
  D[1] = A[1]+B[1]; D[4] = A[4]+B[4]; D[7] = A[7]+B[7];
  D[2] = A[2]+B[2]; D[5] = A[5]+B[5]; D[8] = A[8]+B[8];
}
NA_IDEF void naAddM44dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naAddM44dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM44dE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[4] = A[4]+B[4]; D[ 8] = A[ 8]+B[ 8]; D[12] = A[12]+B[12];
  D[1] = A[1]+B[1]; D[5] = A[5]+B[5]; D[ 9] = A[ 9]+B[ 9]; D[13] = A[13]+B[13];
  D[2] = A[2]+B[2]; D[6] = A[6]+B[6]; D[10] = A[10]+B[10]; D[14] = A[14]+B[14];
  D[3] = A[3]+B[3]; D[7] = A[7]+B[7]; D[11] = A[11]+B[11]; D[15] = A[15]+B[15];
}
NA_IDEF void naAddM22fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naAddM22fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22fE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naAddM33fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM33fE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[3] = A[3]+B[3]; D[6] = A[6]+B[6];
  D[1] = A[1]+B[1]; D[4] = A[4]+B[4]; D[7] = A[7]+B[7];
  D[2] = A[2]+B[2]; D[5] = A[5]+B[5]; D[8] = A[8]+B[8];
}
NA_IDEF void naAddM44fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naAddM44fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM44fE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[4] = A[4]+B[4]; D[ 8] = A[ 8]+B[ 8]; D[12] = A[12]+B[12];
  D[1] = A[1]+B[1]; D[5] = A[5]+B[5]; D[ 9] = A[ 9]+B[ 9]; D[13] = A[13]+B[13];
  D[2] = A[2]+B[2]; D[6] = A[6]+B[6]; D[10] = A[10]+B[10]; D[14] = A[14]+B[14];
  D[3] = A[3]+B[3]; D[7] = A[7]+B[7]; D[11] = A[11]+B[11]; D[15] = A[15]+B[15];
}
NA_IDEF void naAddM22dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22dS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33dS(double*  NA_RESTRICT D, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM33dS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM44dS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}
NA_IDEF void naAddM22fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22fS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM33fS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM44fS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}
NA_IDEF void naAddM22dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33dSE(double*  D, const double*  B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}
NA_IDEF void naAddM22fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 4)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 9)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 16)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}



// ///////////////////////////////
// Sub
// ///////////////////////////////

NA_IDEF void naSubM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM22d", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naSubM22d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM33d", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naSubM33d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM33d", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[3] = A[3]-B[3]; D[6] = A[6]-B[6];
  D[1] = A[1]-B[1]; D[4] = A[4]-B[4]; D[7] = A[7]-B[7];
  D[2] = A[2]-B[2]; D[5] = A[5]-B[5]; D[8] = A[8]-B[8];
}
NA_IDEF void naSubM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM44d", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naSubM44d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM44d", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[4] = A[4]-B[4]; D[ 8] = A[ 8]-B[ 8]; D[12] = A[12]-B[12];
  D[1] = A[1]-B[1]; D[5] = A[5]-B[5]; D[ 9] = A[ 9]-B[ 9]; D[13] = A[13]-B[13];
  D[2] = A[2]-B[2]; D[6] = A[6]-B[6]; D[10] = A[10]-B[10]; D[14] = A[14]-B[14];
  D[3] = A[3]-B[3]; D[7] = A[7]-B[7]; D[11] = A[11]-B[11]; D[15] = A[15]-B[15];
}
NA_IDEF void naSubM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM22f", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naSubM22f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22f", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM33f", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naSubM33f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM33f", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[3] = A[3]-B[3]; D[6] = A[6]-B[6];
  D[1] = A[1]-B[1]; D[4] = A[4]-B[4]; D[7] = A[7]-B[7];
  D[2] = A[2]-B[2]; D[5] = A[5]-B[5]; D[8] = A[8]-B[8];
}
NA_IDEF void naSubM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM44f", "A equals D. Use S or E variant");
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naSubM44f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM44f", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[4] = A[4]-B[4]; D[ 8] = A[ 8]-B[ 8]; D[12] = A[12]-B[12];
  D[1] = A[1]-B[1]; D[5] = A[5]-B[5]; D[ 9] = A[ 9]-B[ 9]; D[13] = A[13]-B[13];
  D[2] = A[2]-B[2]; D[6] = A[6]-B[6]; D[10] = A[10]-B[10]; D[14] = A[14]-B[14];
  D[3] = A[3]-B[3]; D[7] = A[7]-B[7]; D[11] = A[11]-B[11]; D[15] = A[15]-B[15];
}
NA_IDEF void naSubM22dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naSubM22dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22dE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33dE(double*  D, const double*  A, const double*  B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naSubM33dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM33dE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[3] = A[3]-B[3]; D[6] = A[6]-B[6];
  D[1] = A[1]-B[1]; D[4] = A[4]-B[4]; D[7] = A[7]-B[7];
  D[2] = A[2]-B[2]; D[5] = A[5]-B[5]; D[8] = A[8]-B[8];
}
NA_IDEF void naSubM44dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naSubM44dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM44dE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[4] = A[4]-B[4]; D[ 8] = A[ 8]-B[ 8]; D[12] = A[12]-B[12];
  D[1] = A[1]-B[1]; D[5] = A[5]-B[5]; D[ 9] = A[ 9]-B[ 9]; D[13] = A[13]-B[13];
  D[2] = A[2]-B[2]; D[6] = A[6]-B[6]; D[10] = A[10]-B[10]; D[14] = A[14]-B[14];
  D[3] = A[3]-B[3]; D[7] = A[7]-B[7]; D[11] = A[11]-B[11]; D[15] = A[15]-B[15];
}
NA_IDEF void naSubM22fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naSubM22fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22fE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naSubM33fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM33fE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[3] = A[3]-B[3]; D[6] = A[6]-B[6];
  D[1] = A[1]-B[1]; D[4] = A[4]-B[4]; D[7] = A[7]-B[7];
  D[2] = A[2]-B[2]; D[5] = A[5]-B[5]; D[8] = A[8]-B[8];
}
NA_IDEF void naSubM44fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naSubM44fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM44fE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[4] = A[4]-B[4]; D[ 8] = A[ 8]-B[ 8]; D[12] = A[12]-B[12];
  D[1] = A[1]-B[1]; D[5] = A[5]-B[5]; D[ 9] = A[ 9]-B[ 9]; D[13] = A[13]-B[13];
  D[2] = A[2]-B[2]; D[6] = A[6]-B[6]; D[10] = A[10]-B[10]; D[14] = A[14]-B[14];
  D[3] = A[3]-B[3]; D[7] = A[7]-B[7]; D[11] = A[11]-B[11]; D[15] = A[15]-B[15];
}
NA_IDEF void naSubM22dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22dS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33dS(double*  NA_RESTRICT D, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM33dS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM44dS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
}
NA_IDEF void naSubM22fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22fS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM33fS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM44fS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
}
NA_IDEF void naSubM22dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33dSE(double*  D, const double*  B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
}
NA_IDEF void naSubM22fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 4)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 9)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi((NASizeInt)(B-D)) < 16)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
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
