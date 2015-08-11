
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_VECTOR_ALGEBRA_INCLUDED
#define NA_VECTOR_ALGEBRA_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"

// In this file, you find basic Vector algebra functions for both
// Vector and Matrix.




// ///////////////////////////////
// Vector
// ///////////////////////////////

// Note that in NALib, vectors are plain C-arrays. This means that even though
// all arguments are typedef'd, the programmer is responsible for making sure
// the given arrays have indeed the required number of entries. The author
// decided to do this very plainly as this hardly ever becomes a problem, let
// alone being very easy to debug if there would be any. During the last 10
// years, there were barely any of 'em.
//
// Nontheless, you can use the following typedefs to easily create vectors of
// prefedined type and size:

typedef double NAVec1 [1];
typedef double NAVec2 [2];
typedef double NAVec3 [3];
typedef double NAVec4 [4];
typedef float  NAVec1f[1];
typedef float  NAVec2f[2];
typedef float  NAVec3f[3];
typedef float  NAVec4f[4];
typedef NAInt  NAVec1i[1];
typedef NAInt  NAVec2i[2];
typedef NAInt  NAVec3i[3];
typedef NAInt  NAVec4i[4];

// Note that a mathematical operation with a vector result will always be
// performed on the d vector. This might at first be incovenient when
// programming, but the author has not seen many occasions where it becomes
// cumbersome. More so, when using this consistently, the compiler can highly
// optimize the code.

// Fills vector d with the given values
NA_IAPI void naFillV1 (double* d, double a0);
NA_IAPI void naFillV2 (double* d, double a0, double a1);
NA_IAPI void naFillV3 (double* d, double a0, double a1, double a2);
NA_IAPI void naFillV4 (double* d, double a0, double a1, double a2, double a3);
NA_IAPI void naFillV1f(float*  d, float  a0);
NA_IAPI void naFillV2f(float*  d, float  a0, float  a1);
NA_IAPI void naFillV3f(float*  d, float  a0, float  a1, float  a2);
NA_IAPI void naFillV4f(float*  d, float  a0, float  a1, float  a2, float  a3);
NA_IAPI void naFillV1i(NAInt*  d, NAInt  a0);
NA_IAPI void naFillV2i(NAInt*  d, NAInt  a0, NAInt  a1);
NA_IAPI void naFillV3i(NAInt*  d, NAInt  a0, NAInt  a1, NAInt  a2);
NA_IAPI void naFillV4i(NAInt*  d, NAInt  a0, NAInt  a1, NAInt  a2, NAInt  a3);

// Sets all components to Null
NA_IAPI void naNullV1 (double*  d);
NA_IAPI void naNullV2 (double*  d);
NA_IAPI void naNullV3 (double*  d);
NA_IAPI void naNullV4 (double*  d);
NA_IAPI void naNullV1f(float*   d);
NA_IAPI void naNullV2f(float*   d);
NA_IAPI void naNullV3f(float*   d);
NA_IAPI void naNullV4f(float*   d);
NA_IAPI void naNullV1i(NAInt*   d);
NA_IAPI void naNullV2i(NAInt*   d);
NA_IAPI void naNullV3i(NAInt*   d);
NA_IAPI void naNullV4i(NAInt*   d);

// Fills vector d with random values in [0, 1)
NA_IAPI void naFillV1Random (double* d);
NA_IAPI void naFillV2Random (double* d);
NA_IAPI void naFillV3Random (double* d);
NA_IAPI void naFillV4Random (double* d);
NA_IAPI void naFillV1fRandom(float*  d);
NA_IAPI void naFillV2fRandom(float*  d);
NA_IAPI void naFillV3fRandom(float*  d);
NA_IAPI void naFillV4fRandom(float*  d);

// Fills vector d with random vectors in a unit sphere. If filled is NA_TRUE,
// the length of d will be smallerequal than 1. If filled is NA_FALSE, the
// length of d will be precisely 1.
NA_IAPI void naFillV1RandomSpherical (double*  d, NABool filled);
NA_IAPI void naFillV2RandomSpherical (double*  d, NABool filled);
NA_IAPI void naFillV3RandomSpherical (double*  d, NABool filled);
NA_IAPI void naFillV4RandomSpherical (double*  d, NABool filled);
NA_IAPI void naFillV1fRandomSpherical(float*   d, NABool filled);
NA_IAPI void naFillV2fRandomSpherical(float*   d, NABool filled);
NA_IAPI void naFillV3fRandomSpherical(float*   d, NABool filled);
NA_IAPI void naFillV4fRandomSpherical(float*   d, NABool filled);

// Copies vector s to d
NA_IAPI void naCopyV1 (double*  d, const double*  s);
NA_IAPI void naCopyV2 (double*  d, const double*  s);
NA_IAPI void naCopyV3 (double*  d, const double*  s);
NA_IAPI void naCopyV4 (double*  d, const double*  s);
NA_IAPI void naCopyV1f(float*   d, const float*   s);
NA_IAPI void naCopyV2f(float*   d, const float*   s);
NA_IAPI void naCopyV3f(float*   d, const float*   s);
NA_IAPI void naCopyV4f(float*   d, const float*   s);

// Negates the Vector. d = -s
NA_IAPI void naNegV1 (double* d, const double*  s);
NA_IAPI void naNegV2 (double* d, const double*  s);
NA_IAPI void naNegV3 (double* d, const double*  s);
NA_IAPI void naNegV4 (double* d, const double*  s);
NA_IAPI void naNegV1f(float*  d, const float*   s);
NA_IAPI void naNegV2f(float*  d, const float*   s);
NA_IAPI void naNegV3f(float*  d, const float*   s);
NA_IAPI void naNegV4f(float*  d, const float*   s);

// Adds vector a and b and stores the result in d
NA_IAPI void naAddV1 (double* d, const double*  a, const double*  b);
NA_IAPI void naAddV2 (double* d, const double*  a, const double*  b);
NA_IAPI void naAddV3 (double* d, const double*  a, const double*  b);
NA_IAPI void naAddV4 (double* d, const double*  a, const double*  b);
NA_IAPI void naAddV1f(float*  d, const float*   a, const float*   b);
NA_IAPI void naAddV2f(float*  d, const float*   a, const float*   b);
NA_IAPI void naAddV3f(float*  d, const float*   a, const float*   b);
NA_IAPI void naAddV4f(float*  d, const float*   a, const float*   b);

// Subtracts vector b from a and stores the result in d
NA_IAPI void naSubV1 (double* d, const double*  a, const double*  b);
NA_IAPI void naSubV2 (double* d, const double*  a, const double*  b);
NA_IAPI void naSubV3 (double* d, const double*  a, const double*  b);
NA_IAPI void naSubV4 (double* d, const double*  a, const double*  b);
NA_IAPI void naSubV1f(float*  d, const float*   a, const float*   b);
NA_IAPI void naSubV2f(float*  d, const float*   a, const float*   b);
NA_IAPI void naSubV3f(float*  d, const float*   a, const float*   b);
NA_IAPI void naSubV4f(float*  d, const float*   a, const float*   b);

// Multiplies vector a with scalar s and stores the result in d
NA_IAPI void naMulV1 (double* d, const double*  a, double s);
NA_IAPI void naMulV2 (double* d, const double*  a, double s);
NA_IAPI void naMulV3 (double* d, const double*  a, double s);
NA_IAPI void naMulV4 (double* d, const double*  a, double s);
NA_IAPI void naMulV1f(float*  d, const float*   a, float  s);
NA_IAPI void naMulV2f(float*  d, const float*   a, float  s);
NA_IAPI void naMulV3f(float*  d, const float*   a, float  s);
NA_IAPI void naMulV4f(float*  d, const float*   a, float  s);

// Divides vector a by scalar s and stores the result in d
NA_IAPI void naDivV1 (double* d, const double*  a, double s);
NA_IAPI void naDivV2 (double* d, const double*  a, double s);
NA_IAPI void naDivV3 (double* d, const double*  a, double s);
NA_IAPI void naDivV4 (double* d, const double*  a, double s);
NA_IAPI void naDivV1f(float*  d, const float*   a, float  s);
NA_IAPI void naDivV2f(float*  d, const float*   a, float  s);
NA_IAPI void naDivV3f(float*  d, const float*   a, float  s);
NA_IAPI void naDivV4f(float*  d, const float*   a, float  s);

// Divides vector a componentwise by vector b and stores the result in d
NA_IAPI void naDivV1V1  (double*  d, const double*  a, const double*  b);
NA_IAPI void naDivV2V2  (double*  d, const double*  a, const double*  b);
NA_IAPI void naDivV3V3  (double*  d, const double*  a, const double*  b);
NA_IAPI void naDivV4V4  (double*  d, const double*  a, const double*  b);
NA_IAPI void naDivV1fV1f(float*   d, const float*   a, const float*   b);
NA_IAPI void naDivV2fV2f(float*   d, const float*   a, const float*   b);
NA_IAPI void naDivV3fV3f(float*   d, const float*   a, const float*   b);
NA_IAPI void naDivV4fV4f(float*   d, const float*   a, const float*   b);

// Multiply and add: Computes d = a + b * s
NA_IAPI void naMadV1 (double* d, const double*  a, const double*  b, double s);
NA_IAPI void naMadV2 (double* d, const double*  a, const double*  b, double s);
NA_IAPI void naMadV3 (double* d, const double*  a, const double*  b, double s);
NA_IAPI void naMadV4 (double* d, const double*  a, const double*  b, double s);
NA_IAPI void naMadV1f(float*  d, const float*   a, const float*   b, float  s);
NA_IAPI void naMadV2f(float*  d, const float*   a, const float*   b, float  s);
NA_IAPI void naMadV3f(float*  d, const float*   a, const float*   b, float  s);
NA_IAPI void naMadV4f(float*  d, const float*   a, const float*   b, float  s);

// Returns true if the two vectors are equal
NA_IAPI NABool naEqualV1 (const double* a, const double*  b);
NA_IAPI NABool naEqualV2 (const double* a, const double*  b);
NA_IAPI NABool naEqualV3 (const double* a, const double*  b);
NA_IAPI NABool naEqualV4 (const double* a, const double*  b);
NA_IAPI NABool naEqualV1f(const float*  a, const float*   b);
NA_IAPI NABool naEqualV2f(const float*  a, const float*   b);
NA_IAPI NABool naEqualV3f(const float*  a, const float*   b);
NA_IAPI NABool naEqualV4f(const float*  a, const float*   b);

// Computes the dot product (scalar product, inner product) of the two vectors.
NA_IAPI double naDotV1 (const double* a, const double*  b);
NA_IAPI double naDotV2 (const double* a, const double*  b);
NA_IAPI double naDotV3 (const double* a, const double*  b);
NA_IAPI double naDotV4 (const double* a, const double*  b);
NA_IAPI float  naDotV1f(const float*  a, const float*   b);
NA_IAPI float  naDotV2f(const float*  a, const float*   b);
NA_IAPI float  naDotV3f(const float*  a, const float*   b);
NA_IAPI float  naDotV4f(const float*  a, const float*   b);

// Cross product. Only defined for V3. d = a X b
NA_IAPI void naCrossV3 (double* d, const double*  a, const double*  b);
NA_IAPI void naCrossV3f(float*  d, const float*   a, const float*   b);

// Computes the length (2-Norm) of the given vector
NA_IAPI double naLengthV1 (const double*  a);
NA_IAPI double naLengthV2 (const double*  a);
NA_IAPI double naLengthV3 (const double*  a);
NA_IAPI double naLengthV4 (const double*  a);
NA_IAPI float  naLengthV1f(const float*   a);
NA_IAPI float  naLengthV2f(const float*   a);
NA_IAPI float  naLengthV3f(const float*   a);
NA_IAPI float  naLengthV4f(const float*   a);

// Computes the (positive 2-norm) distance between two vectors
NA_IAPI double naDistanceV1 (const double*  a, const double*  b);
NA_IAPI double naDistanceV2 (const double*  a, const double*  b);
NA_IAPI double naDistanceV3 (const double*  a, const double*  b);
NA_IAPI double naDistanceV4 (const double*  a, const double*  b);
NA_IAPI float  naDistanceV1f(const float*   a, const float*   b);
NA_IAPI float  naDistanceV2f(const float*   a, const float*   b);
NA_IAPI float  naDistanceV3f(const float*   a, const float*   b);
NA_IAPI float  naDistanceV4f(const float*   a, const float*   b);

// Mirrors vector a on surface normal n and stores the result in d
NA_IAPI void naMirrorV1 (double*  d, const double*  a, const double*  n);
NA_IAPI void naMirrorV2 (double*  d, const double*  a, const double*  n);
NA_IAPI void naMirrorV3 (double*  d, const double*  a, const double*  n);
NA_IAPI void naMirrorV4 (double*  d, const double*  a, const double*  n);
NA_IAPI void naMirrorV1f(float*   d, const float*   a, const float*   n);
NA_IAPI void naMirrorV2f(float*   d, const float*   a, const float*   n);
NA_IAPI void naMirrorV3f(float*   d, const float*   a, const float*   n);
NA_IAPI void naMirrorV4f(float*   d, const float*   a, const float*   n);

// Scales the vector s to a length of 1. Returns the scaled vector in d and the
// length of the vector before scaling as a return value. If a was a
// zero-vector, d will be a zero-vector and the returned length is
// precisely zero.
NA_IAPI double naNormalizeV1 (double* d, const double*  s);
NA_IAPI double naNormalizeV2 (double* d, const double*  s);
NA_IAPI double naNormalizeV3 (double* d, const double*  s);
NA_IAPI double naNormalizeV4 (double* d, const double*  s);
NA_IAPI float  naNormalizeV1f(float*  d, const float*   s);
NA_IAPI float  naNormalizeV2f(float*  d, const float*   s);
NA_IAPI float  naNormalizeV3f(float*  d, const float*   s);
NA_IAPI float  naNormalizeV4f(float*  d, const float*   s);

// Makes vector a orthogonal to b and stores the result in d.
// Using Gram-Schmidt. If vector b is a zero-vector, d becomes a.
// If a was colinear to b or a zero-vector, d will become a zero-vector.
NA_IAPI void naOrthogonalizeV2 (double* d, const double*  a, const double*  b);
NA_IAPI void naOrthogonalizeV3 (double* d, const double*  a, const double*  b);
NA_IAPI void naOrthogonalizeV4 (double* d, const double*  a, const double*  b);
NA_IAPI void naOrthogonalizeV2f(float*  d, const float*   a, const float*   b);
NA_IAPI void naOrthogonalizeV3f(float*  d, const float*   a, const float*   b);
NA_IAPI void naOrthogonalizeV4f(float*  d, const float*   a, const float*   b);

// Assumes b to be normalized and makes a ortho-normal to b. Stores the result
// in d. If b was not normal, the result is undefined. If a is colinear to b
// or one of the vectors is a zero-vector, d will become a zero-vector.
NA_IAPI void naOrthonormalizeV2 (double* d, const double* a, const double* b);
NA_IAPI void naOrthonormalizeV3 (double* d, const double* a, const double* b);
NA_IAPI void naOrthonormalizeV4 (double* d, const double* a, const double* b);
NA_IAPI void naOrthonormalizeV2f(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthonormalizeV3f(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthonormalizeV4f(float*  d, const float*  a, const float*  b);



// ///////////////////////////////
// Matrix
// ///////////////////////////////

// Info: NALib stores the values of matrices in COLUMN-FIRST order!
// The indices of a matrix type therefore look as follows:
// (  m[0]   m[3]   m[6]  )
// (  m[1]   m[4]   m[7]  )
// (  m[2]   m[5]   m[8]  )

typedef double NAMat22 [ 4];
typedef double NAMat33 [ 9];
typedef double NAMat44 [16];
typedef float  NAMat22f[ 4];
typedef float  NAMat33f[ 9];
typedef float  NAMat44f[16];

// Fills the matrix with the given values. Warning: Provide the values in
// COLUMN-FIRST order!
NA_IAPI void naFillM22 ( double*  d,
   double a0 , double a1 , double a2 , double a3);
NA_IAPI void naFillM33 ( double*  d,
   double a0 , double a1 , double a2 , double a3 , double a4 , double a5 ,
   double a6 , double a7 , double a8);
NA_IAPI void naFillM44 ( double*  d,
   double a0 , double a1 , double a2 , double a3 , double a4 , double a5 ,
   double a6 , double a7 , double a8 , double a9 , double a10, double a11,
   double a12, double a13, double a14, double a15);
NA_IAPI void naFillM22f( float* d,
   float  a0 , float  a1 , float  a2 , float  a3);
NA_IAPI void naFillM33f( float* d,
   float  a0 , float  a1 , float  a2 , float  a3 , float  a4 , float  a5 ,
   float  a6 , float  a7 , float  a8);
NA_IAPI void naFillM44f( float* d,
   float  a0 , float  a1 , float  a2 , float  a3 , float  a4 , float  a5 ,
   float  a6 , float  a7 , float  a8 , float  a9 , float  a10, float  a11,
   float  a12, float  a13, float  a14, float  a15);

// Fills the matrix with the given column vectors.
NA_IAPI void naFillM22WithAxis   (double*  d,
                            const double* v0,
                            const double* v1);
NA_IAPI void naFillM33WithAxis   (double*  d,
                            const double* v0,
                            const double* v1,
                            const double* v2);
NA_IAPI void naFillM44WithAxis   (double*  d,
                            const double* v0,
                            const double* v1,
                            const double* v2,
                            const double* v3);
NA_IAPI void naFillM22fWithAxis  (float*   d,
                            const float*  v0,
                            const float*  v1);
NA_IAPI void naFillM33fWithAxis  (float*   d,
                            const float*  v0,
                            const float*  v1,
                            const float*  v2);
NA_IAPI void naFillM44fWithAxis  (float*   d,
                            const float*  v0,
                            const float*  v1,
                            const float*  v2,
                            const float*  v3);

// Fills the matrix diagonally with s and all other entries with zero.
// Use this function to create an identity matrix.
NA_IAPI void naFillM22WithDiag (double*  d, double s);
NA_IAPI void naFillM33WithDiag (double*  d, double s);
NA_IAPI void naFillM44WithDiag (double*  d, double s);
NA_IAPI void naFillM22fWithDiag(float*   d, float  s);
NA_IAPI void naFillM33fWithDiag(float*   d, float  s);
NA_IAPI void naFillM44fWithDiag(float*   d, float  s);

// Fills the matrix diagonally with the entries given in the diag vector and
// all other entries with zero.
NA_IAPI void naFillM22WithDiagV2  (double* d, const double*  diag);
NA_IAPI void naFillM33WithDiagV3  (double* d, const double*  diag);
NA_IAPI void naFillM44WithDiagV4  (double* d, const double*  diag);
NA_IAPI void naFillM22fWithDiagV2f(float*  d, const float*   diag);
NA_IAPI void naFillM33fWithDiagV3f(float*  d, const float*   diag);
NA_IAPI void naFillM44fWithDiagV4f(float*  d, const float*   diag);

// Copies the values of s to d;
NA_IAPI void naCopyM22 (double* d, const double*  s);
NA_IAPI void naCopyM33 (double* d, const double*  s);
NA_IAPI void naCopyM44 (double* d, const double*  s);
NA_IAPI void naCopyM22f(float*  d, const float*   s);
NA_IAPI void naCopyM33f(float*  d, const float*   s);
NA_IAPI void naCopyM44f(float*  d, const float*   s);

// Negates the matrix
NA_IAPI void naNegM22 (double*  d, double*  s);
NA_IAPI void naNegM33 (double*  d, double*  s);
NA_IAPI void naNegM44 (double*  d, double*  s);
NA_IAPI void naNegM22f(float*   d, float*   s);
NA_IAPI void naNegM33f(float*   d, float*   s);
NA_IAPI void naNegM44f(float*   d, float*   s);

// Adds matrix a and b and stores the result in d
NA_IAPI void naAddM22 (double*  d, const double*  a, const double*  b);
NA_IAPI void naAddM33 (double*  d, const double*  a, const double*  b);
NA_IAPI void naAddM44 (double*  d, const double*  a, const double*  b);
NA_IAPI void naAddM22f(float*   d, const float*   a, const float*   b);
NA_IAPI void naAddM33f(float*   d, const float*   a, const float*   b);
NA_IAPI void naAddM44f(float*   d, const float*   a, const float*   b);

// Subtracts matrix b from a and stores the result in d
NA_IAPI void naSubM22 (double*  d, const double*  a, const double*  b);
NA_IAPI void naSubM33 (double*  d, const double*  a, const double*  b);
NA_IAPI void naSubM44 (double*  d, const double*  a, const double*  b);
NA_IAPI void naSubM22f(float*   d, const float*   a, const float*   b);
NA_IAPI void naSubM33f(float*   d, const float*   a, const float*   b);
NA_IAPI void naSubM44f(float*   d, const float*   a, const float*   b);

// Scales the whole matrix m with the given scalar and stores the result in d.
NA_IAPI void naScaleM22 (double*  d, const double*  a, double s);
NA_IAPI void naScaleM33 (double*  d, const double*  a, double s);
NA_IAPI void naScaleM44 (double*  d, const double*  a, double s);
NA_IAPI void naScaleM22f(float*   d, const float*   a, float  s);
NA_IAPI void naScaleM33f(float*   d, const float*   a, float  s);
NA_IAPI void naScaleM44f(float*   d, const float*   a, float  s);

// Scales the column vectors of matrix m with the entries of vector v
NA_IAPI void naScaleM22V2  (double* d, const double*  a, const double*  v);
NA_IAPI void naScaleM33V3  (double* d, const double*  a, const double*  v);
NA_IAPI void naScaleM44V4  (double* d, const double*  a, const double*  v);
NA_IAPI void naScaleM22fV2f(float*  d, const float*   a, const float*   v);
NA_IAPI void naScaleM33fV3f(float*  d, const float*   a, const float*   v);
NA_IAPI void naScaleM44fV4f(float*  d, const float*   a, const float*   v);

// Multiplies matrix m with vector v and stores the result vector in d
NA_IAPI void naMulM22V2  (double* d, const double*  m, const double*  v);
NA_IAPI void naMulM33V3  (double* d, const double*  m, const double*  v);
NA_IAPI void naMulM44V4  (double* d, const double*  m, const double*  v);
NA_IAPI void naMulM22fV2f(float*  d, const float*   m, const float*   v);
NA_IAPI void naMulM33fV3f(float*  d, const float*   m, const float*   v);
NA_IAPI void naMulM44fV4f(float*  d, const float*   m, const float*   v);

// Multiplies Matrix m with matrix a and stores the result matrix in d
NA_IAPI void naMulM22M22  (double*  d, const double*  m, const double*  a);
NA_IAPI void naMulM33M33  (double*  d, const double*  m, const double*  a);
NA_IAPI void naMulM44M44  (double*  d, const double*  m, const double*  a);
NA_IAPI void naMulM22fM22f(float*   d, const float*   m, const float*   a);
NA_IAPI void naMulM33fM33f(float*   d, const float*   m, const float*   a);
NA_IAPI void naMulM44fM44f(float*   d, const float*   m, const float*   a);

// Computes the inverse of m and stores it in d
NA_IAPI void naInvertM22 (double* d, const double*  m);
NA_IAPI void naInvertM33 (double* d, const double*  m);
NA_IAPI void naInvertM44 (double* d, const double*  m);
NA_IAPI void naInvertM22f(float*  d, const float*   m);
NA_IAPI void naInvertM33f(float*  d, const float*   m);
NA_IAPI void naInvertM44f(float*  d, const float*   m);

// Creates a 3D rotation matrix using Quaternions.
//
// Quaternions are a very complex mathematical thing but they are quite handy
// when it comes to performing rotations in 3D space based on angles. Usually,
// quaternions are stored with 4 values but as NALib so far has no purpose for
// a full implementation of the whole mathematical structure behind, we simply
// create 3x3 matrices out of given angles.
//
// The following functions create 3x3 rotation matrices which you can apply
// to any 3D vector by multiplying them with M * p.
//
// Warning: Quaternions are fast but unreliable in normalization, especially
// for extreme angles. If you need to have normalized vectors, perform a
// normalization after each rotation!
//
// One function expects an axis and a (right handed) rotation angle.
// The other function expects three rotation angles roll, pitch and yaw.
NA_IAPI void naFillM33WithRotationAlongAxis(  double* d,
                                        const double* axis,
                                               double angle);
NA_IAPI void naFillM33WithRotationAngles(     double* d,
                                               double roll,
                                               double pitch,
                                               double yaw);













// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NARandom.h"
#include "NAMathOperators.h"


NA_IDEF void naFillV1(double* d, double a0){
  d[0] = a0;
}
NA_IDEF void naFillV2(double* d, double a0, double a1){
  d[0] = a0; d[1] = a1;
}
NA_IDEF void naFillV3(double* d, double a0, double a1, double a2){
  d[0] = a0; d[1] = a1; d[2] = a2;
}
NA_IDEF void naFillV4(double* d, double a0, double a1, double a2, double a3){
  d[0] = a0; d[1] = a1; d[2] = a2; d[3] = a3;
}
NA_IDEF void naFillV1f(float* d, float a0){
  d[0] = a0;
}
NA_IDEF void naFillV2f(float* d, float a0, float a1){
  d[0] = a0; d[1] = a1;
}
NA_IDEF void naFillV3f(float* d, float a0, float a1, float a2){
  d[0] = a0; d[1] = a1; d[2] = a2;
}
NA_IDEF void naFillV4f(float* d, float a0, float a1, float a2, float a3){
  d[0] = a0; d[1] = a1; d[2] = a2; d[3] = a3;
}
NA_IDEF void naFillV1i(NAInt* d, NAInt a0){
  d[0] = a0;
}
NA_IDEF void naFillV2i(NAInt* d, NAInt a0, NAInt a1){
  d[0] = a0; d[1] = a1;
}
NA_IDEF void naFillV3i(NAInt* d, NAInt a0, NAInt a1, NAInt a2){
  d[0] = a0; d[1] = a1; d[2] = a2;
}
NA_IDEF void naFillV4i(NAInt* d, NAInt a0, NAInt a1, NAInt a2, NAInt a3){
  d[0] = a0; d[1] = a1; d[2] = a2; d[3] = a3;
}


NA_IDEF void naNullV1 (double*  d){
  d[0] = 0.;
}
NA_IDEF void naNullV2 (double*  d){
  d[0] = 0.; d[1] = 0.;
}
NA_IDEF void naNullV3 (double*  d){
  d[0] = 0.; d[1] = 0.; d[2] = 0.;
}
NA_IDEF void naNullV4 (double*  d){
  d[0] = 0.; d[1] = 0.; d[2] = 0.; d[3] = 0.;
}
NA_IDEF void naNullV1f(float* d){
  d[0] = 0.f;
}
NA_IDEF void naNullV2f(float* d){
  d[0] = 0.f; d[1] = 0.f;
}
NA_IDEF void naNullV3f(float* d){
  d[0] = 0.f; d[1] = 0.f; d[2] = 0.f;
}
NA_IDEF void naNullV4f(float* d){
  d[0] = 0.f; d[1] = 0.f; d[2] = 0.f; d[3] = 0.f;
}
NA_IDEF void naNullV1i(NAInt* d){
  d[0] = 0;
}
NA_IDEF void naNullV2i(NAInt* d){
  d[0] = 0; d[1] = 0;
}
NA_IDEF void naNullV3i(NAInt* d){
  d[0] = 0; d[1] = 0; d[2] = 0;
}
NA_IDEF void naNullV4i(NAInt* d){
  d[0] = 0; d[1] = 0; d[2] = 0; d[3] = 0;
}



NA_IDEF void naFillV1Random(double* d){
  naFillV1( d, naUniformRandZE());
}
NA_IDEF void naFillV2Random(double* d){
  naFillV2( d, naUniformRandZE(),
              naUniformRandZE());
}
NA_IDEF void naFillV3Random(double* d){
  naFillV3( d, naUniformRandZE(),
              naUniformRandZE(),
              naUniformRandZE());
}
NA_IDEF void naFillV4Random(double* d){
  naFillV4( d, naUniformRandZE(),
              naUniformRandZE(),
              naUniformRandZE(),
              naUniformRandZE());
}
NA_IDEF void naFillV1fRandom(float* d){
  naFillV1f(d, naUniformRandZEf());
}
NA_IDEF void naFillV2fRandom(float* d){
  naFillV2f(d, naUniformRandZEf(),
              naUniformRandZEf());
}
NA_IDEF void naFillV3fRandom(float* d){
  naFillV3f(d, naUniformRandZEf(),
              naUniformRandZEf(),
              naUniformRandZEf());
}
NA_IDEF void naFillV4fRandom(float* d){
  naFillV4f(d, naUniformRandZEf(),
              naUniformRandZEf(),
              naUniformRandZEf(),
              naUniformRandZEf());
}


NA_IDEF void naFillV1RandomSpherical(double* d, NABool filled){
  if(filled){
    do{
      d[0] = naUniformRandZI() * 2. - 1.;
    }while(d[0] == 0.);
  }else{
    d[0] = (naUniformRandZE() < .5) ? -1. : 1.;
  }
}
NA_IDEF void naFillV2RandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    norm = naDotV2(d, d);
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is very rare.
  if(!filled){naDivV2(d, d, naSqrt(norm));}
}
NA_IDEF void naFillV3RandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    d[2] = naUniformRandZI() * 2. - 1.;
    norm = naDotV3(d, d);
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is extremely! rare.
  if(!filled){naDivV3(d, d, naSqrt(norm));}
}
NA_IDEF void naFillV4RandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    d[2] = naUniformRandZI() * 2. - 1.;
    d[3] = naUniformRandZI() * 2. - 1.;
    norm = naDotV4(d, d);
    // Note that with more and more dimensions it becomes more and more likely
    // that this do-while loop must be repeated many times. Up to 3 dimensions,
    // this method works quite well but starting with 4 dimensions, one should
    // really consider a different method. The author did not because he is
    // lazy.
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is EXORBITANTLY rare.
  if(!filled){naDivV4(d, d, naSqrt(norm));}
}
NA_IDEF void naFillV1fRandomSpherical(float* d, NABool filled){
  if(filled){
    do{
      d[0] = naUniformRandZIf() * 2.f - 1.f;
    }while(d[0] == 0.f);
  }else{
    d[0] = (naUniformRandZEf() < .5f) ? -1.f : 1.f;
  }
}
NA_IDEF void naFillV2fRandomSpherical(float* d, NABool filled){
  float norm;
  do{
    d[0] = naUniformRandZIf() * 2.f - 1.f;
    d[1] = naUniformRandZIf() * 2.f - 1.f;
    norm = naDotV2f(d, d);
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is very rare.
  if(!filled){naDivV2f(d, d, naSqrtf(norm));}
}
NA_IDEF void naFillV3fRandomSpherical(float* d, NABool filled){
  float norm;
  do{
    d[0] = naUniformRandZIf() * 2.f - 1.f;
    d[1] = naUniformRandZIf() * 2.f - 1.f;
    d[2] = naUniformRandZIf() * 2.f - 1.f;
    norm = naDotV3f(d, d);
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is extremely! rare.
  if(!filled){naDivV3f(d, d, naSqrtf(norm));}
}
NA_IDEF void naFillV4fRandomSpherical(float* d, NABool filled){
  float norm;
  do{
    d[0] = naUniformRandZIf() * 2.f - 1.f;
    d[1] = naUniformRandZIf() * 2.f - 1.f;
    d[2] = naUniformRandZIf() * 2.f - 1.f;
    d[3] = naUniformRandZIf() * 2.f - 1.f;
    norm = naDotV4f(d, d);
    // Note that with more and more dimensions it becomes more and more likely
    // that this do-while loop must be repeated many times. Up to 3 dimensions,
    // this method works quite well but starting with 4 dimensions, one should
    // really consider a different method. The author did not because he is
    // lazy.
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is EXORBITANTLY rare.
  if(!filled){naDivV4f(d, d, naSqrtf(norm));}
}


NA_IDEF void naCopyV1(double* d, const double* s){
  d[0] = s[0];
}
NA_IDEF void naCopyV2(double* d, const double* s){
  d[0] = s[0]; d[1] = s[1];
}
NA_IDEF void naCopyV3(double* d, const double* s){
  d[0] = s[0]; d[1] = s[1]; d[2] = s[2];
}
NA_IDEF void naCopyV4(double* d, const double* s){
  d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
}
NA_IDEF void naCopyV1f(float* d, const float* s){
  d[0] = s[0];
}
NA_IDEF void naCopyV2f(float* d, const float* s){
  d[0] = s[0]; d[1] = s[1];
}
NA_IDEF void naCopyV3f(float* d, const float* s){
  d[0] = s[0]; d[1] = s[1]; d[2] = s[2];
}
NA_IDEF void naCopyV4f(float* d, const float* s){
  d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
}


NA_IDEF void naNegV1(double* d, const double* s){
  d[0] = -s[0];
}
NA_IDEF void naNegV2(double* d, const double* s){
  d[0] = -s[0]; d[1] = -s[1];
}
NA_IDEF void naNegV3(double* d, const double* s){
  d[0] = -s[0]; d[1] = -s[1]; d[2] = -s[2];
}
NA_IDEF void naNegV4(double* d, const double* s){
  d[0] = -s[0]; d[1] = -s[1]; d[2] = -s[2]; d[3] = -s[3];
}
NA_IDEF void naNegV1f(float* d, const float* s){
  d[0] = -s[0];
}
NA_IDEF void naNegV2f(float* d, const float* s){
  d[0] = -s[0]; d[1] = -s[1];
}
NA_IDEF void naNegV3f(float* d, const float* s){
  d[0] = -s[0]; d[1] = -s[1]; d[2] = -s[2];
}
NA_IDEF void naNegV4f(float* d, const float* s){
  d[0] = -s[0]; d[1] = -s[1]; d[2] = -s[2]; d[3] = -s[3];
}


NA_IDEF void naAddV1(double* d, const double* a, const double* b){
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2(double* d, const double* a, const double* b){
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3(double* d, const double* a, const double* b){
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4(double* d, const double* a, const double* b){
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1f(float* d, const float* a, const float* b){
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2f(float* d, const float* a, const float* b){
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3f(float* d, const float* a, const float* b){
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4f(float* d, const float* a, const float* b){
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}


NA_IDEF void naSubV1(double* d, const double* a, const double* b){
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2(double* d, const double* a, const double* b){
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3(double* d, const double* a, const double* b){
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4(double* d, const double* a, const double* b){
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1f(float* d, const float* a, const float* b){
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2f(float* d, const float* a, const float* b){
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3f(float* d, const float* a, const float* b){
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4f(float* d, const float* a, const float* b){
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}


NA_IDEF void naMulV1(double* d, const double* a, double s){
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2(double* d, const double* a, double s){
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3(double* d, const double* a, double s){
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4(double* d, const double* a, double s){
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1f(float* d, const float* a, float s){
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2f(float* d, const float* a, float s){
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3f(float* d, const float* a, float s){
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4f(float* d, const float* a, float s){
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}


NA_IDEF void naDivV1(double* d, const double* a, double s){
  naMulV1(d, a, naInv(s));
}
NA_IDEF void naDivV2(double* d, const double* a, double s){
  naMulV2(d, a, naInv(s));
}
NA_IDEF void naDivV3(double* d, const double* a, double s){
  naMulV3(d, a, naInv(s));
}
NA_IDEF void naDivV4(double* d, const double* a, double s){
  naMulV4(d, a, naInv(s));
}
NA_IDEF void naDivV1f(float* d, const float* a, float s){
  naMulV1f(d, a, naInvf(s));
}
NA_IDEF void naDivV2f(float* d, const float* a, float s){
  naMulV2f(d, a, naInvf(s));
}
NA_IDEF void naDivV3f(float* d, const float* a, float s){
  naMulV3f(d, a, naInvf(s));
}
NA_IDEF void naDivV4f(float* d, const float* a, float s){
  naMulV4f(d, a, naInvf(s));
}


NA_IDEF void naDivV1V1(double* d, const double* a, const double* b){
  d[0] = a[0] * naInv(b[0]);
}
NA_IDEF void naDivV2V2(double* d, const double* a, const double* b){
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
}
NA_IDEF void naDivV3V3(double* d, const double* a, const double* b){
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
}
NA_IDEF void naDivV4V4(double* d, const double* a, const double* b){
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
  d[3] = a[3] * naInv(b[3]);
}
NA_IDEF void naDivV1fV1f(float* d, const float* a, const float* b){
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivV2fV2f(float* d, const float* a, const float* b){
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivV3fV3f(float* d, const float* a, const float* b){
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivV4fV4f(float* d, const float* a, const float* b){
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}


NA_IDEF void naMadV1(double* d, const double* a, const double* b, double s){
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2(double* d, const double* a, const double* b, double s){
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3(double* d, const double* a, const double* b, double s){
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4(double* d, const double* a, const double* b, double s){
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1f(float* d, const float* a, const float* b, float s){
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2f(float* d, const float* a, const float* b, float s){
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3f(float* d, const float* a, const float* b, float s){
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4f(float* d, const float* a, const float* b, float s){
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}


NA_IDEF NABool naEqualV1(const double* a, const double* b){
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2(const double* a, const double* b){
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3(const double* a, const double* b){
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4(const double* a, const double* b){
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualV1f(const float* a, const float* b){
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2f(const float* a, const float* b){
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3f(const float* a, const float* b){
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4f(const float* a, const float* b){
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}


NA_IDEF double naDotV1(const double* a, const double* b){
  return a[0] * b[0];
}
NA_IDEF double naDotV2(const double* a, const double* b){
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF double naDotV3(const double* a, const double* b){
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF double naDotV4(const double* a, const double* b){
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF float naDotV1f(const float* a, const float* b){
  return a[0] * b[0];
}
NA_IDEF float naDotV2f(const float* a, const float* b){
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF float naDotV3f(const float* a, const float* b){
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF float naDotV4f(const float* a, const float* b){
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}


NA_IDEF void naCrossV3(double* d, const double* a, const double* b){
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
}
NA_IDEF void naCrossV3f(float* d, const float* a, const float* b){
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
}


NA_IDEF double naLengthV1(const double* a){
  return naAbs(a[0]);
}
NA_IDEF double naLengthV2(const double* a){
  return naSqrt(naDotV2(a, a));
}
NA_IDEF double naLengthV3(const double* a){
  return naSqrt(naDotV3(a, a));
}
NA_IDEF double naLengthV4(const double* a){
  return naSqrt(naDotV4(a, a));
}
NA_IDEF float naLengthV1f(const float* a){
  return naAbsf(a[0]);
}
NA_IDEF float naLengthV2f(const float* a){
  return naSqrtf(naDotV2f(a, a));
}
NA_IDEF float naLengthV3f(const float* a){
  return naSqrtf(naDotV3f(a, a));
}
NA_IDEF float naLengthV4f(const float* a){
  return naSqrtf(naDotV4f(a, a));
}


NA_IDEF double naDistanceV1(const double* a, const double* b){
  return naAbs(a[0] - b[0]);
}
NA_IDEF double naDistanceV2(const double* a, const double* b){
  NAVec2 vec;
  naSubV2(vec, a, b);
  return naLengthV2(vec);
}
NA_IDEF double naDistanceV3(const double* a, const double* b){
  NAVec3 vec;
  naSubV3(vec, a, b);
  return naLengthV3(vec);
}
NA_IDEF double naDistanceV4(const double* a, const double* b){
  NAVec4 vec;
  naSubV4(vec, a, b);
  return naLengthV4(vec);
}
NA_IDEF float naDistanceV1f(const float* a, const float* b){
  return naAbsf(a[0] - b[0]);
}
NA_IDEF float naDistanceV2f(const float* a, const float* b){
  NAVec2f vec;
  naSubV2f(vec, a, b);
  return naLengthV2f(vec);
}
NA_IDEF float naDistanceV3f(const float* a, const float* b){
  NAVec3f vec;
  naSubV3f(vec, a, b);
  return naLengthV3f(vec);
}
NA_IDEF float naDistanceV4f(const float* a, const float* b){
  NAVec4f vec;
  naSubV4f(vec, a, b);
  return naLengthV4f(vec);
}


NA_IDEF void naMirrorV1(double* d, const double* a, const double* n){
  naMadV1(d, a, n, -2. * naDotV1(a, n));
}
NA_IDEF void naMirrorV2(double* d, const double* a, const double* n){
  naMadV2(d, a, n, -2. * naDotV2(a, n));
}
NA_IDEF void naMirrorV3(double* d, const double* a, const double* n){
  naMadV3(d, a, n, -2. * naDotV3(a, n));
}
NA_IDEF void naMirrorV4(double* d, const double* a, const double* n){
  naMadV4(d, a, n, -2. * naDotV4(a, n));
}
NA_IDEF void naMirrorV1f(float* d, const float* a, const float* n){
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2f(float* d, const float* a, const float* n){
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3f(float* d, const float* a, const float* n){
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4f(float* d, const float* a, const float* n){
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}


NA_IDEF double naNormalizeV1(double* d, const double* s){
  double length = naLengthV1(s);
  if(length == 0.){
    naFillV1(d, 0.);
  }else{
    naDivV1(d, s, length);
  }
  return length;
}
NA_IDEF double naNormalizeV2(double* d, const double* s){
  double length = naLengthV2(s);
  if(length == 0.){
    naFillV2(d, 0., 0.);
  }else{
    naDivV2(d, s, length);
  }
  return length;
}
NA_IDEF double naNormalizeV3(double* d, const double* s){
  double length = naLengthV3(s);
  if(length == 0.){
    naFillV3(d, 0., 0., 0.);
  }else{
    naDivV3(d, s, length);
  }
  return length;
}
NA_IDEF double naNormalizeV4(double* d, const double* s){
  double length = naLengthV4(s);
  if(length == 0.){
    naFillV4(d, 0., 0., 0., 0.);
  }else{
    naDivV4(d, s, length);
  }
  return length;
}
NA_IDEF float naNormalizeV1f(float* d, const float* s){
  float length = naLengthV1f(s);
  if(length == 0.f){
    naFillV1f(d, 0.f);
  }else{
    naDivV1f(d, s, length);
  }
  return length;
}
NA_IDEF float naNormalizeV2f(float* d, const float* s){
  float length = naLengthV2f(s);
  if(length == 0.f){
    naFillV2f(d, 0.f, 0.f);
  }else{
    naDivV2f(d, s, length);
  }
  return length;
}
NA_IDEF float naNormalizeV3f(float* d, const float* s){
  float length = naLengthV3f(s);
  if(length == 0.f){
    naFillV3f(d, 0.f, 0.f, 0.f);
  }else{
    naDivV3f(d, s, length);
  }
  return length;
}
NA_IDEF float naNormalizeV4f(float* d, const float* s){
  float length = naLengthV4f(s);
  if(length == 0.f){
    naFillV4f(d, 0.f, 0.f, 0.f, 0.f);
  }else{
    naDivV4f(d, s, length);
  }
  return length;
}


NA_IDEF void naOrthogonalizeV2(double* d, const double* a, const double* b){
  double bnorm = naDotV2(b, b);
  if(bnorm == 0.){
    naCopyV2(d, a);
  }else{
    naMadV2(d, a, b, - naDotV2(b, a) * naInv(bnorm));
  }
}
NA_IDEF void naOrthogonalizeV3(double* d, const double* a, const double* b){
  double bnorm = naDotV3(b, b);
  if(bnorm == 0.){
    naCopyV3(d, a);
  }else{
    naMadV3(d, a, b, - naDotV3(b, a) * naInv(bnorm));
  }
}
NA_IDEF void naOrthogonalizeV4(double* d, const double* a, const double* b){
  double bnorm = naDotV4(b, b);
  if(bnorm == 0.){
    naCopyV4(d, a);
  }else{
    naMadV4(d, a, b, - naDotV4(b, a) * naInv(bnorm));
  }
}
NA_IDEF void naOrthogonalizeV2f(float* d, const float* a, const float* b){
  float bnorm = naDotV2f(b, b);
  if(bnorm == 0.f){
    naCopyV2f(d, a);
  }else{
    naMadV2f(d, a, b, - naDotV2f(b, a) * naInvf(bnorm));
  }
}
NA_IDEF void naOrthogonalizeV3f(float* d, const float* a, const float* b){
  float bnorm = naDotV3f(b, b);
  if(bnorm == 0.f){
    naCopyV3f(d, a);
  }else{
    naMadV3f(d, a, b, - naDotV3f(b, a) * naInvf(bnorm));
  }
}
NA_IDEF void naOrthogonalizeV4f(float* d, const float* a, const float* b){
  float bnorm = naDotV4f(b, b);
  if(bnorm == 0.f){
    naCopyV4f(d, a);
  }else{
    naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
  }
}


NA_IDEF void naOrthonormalizeV2(double* d, const double* a, const double* b){
  naMadV2(d, a, b, - naDotV2(b, d));
  naNormalizeV2(d, d);
}
NA_IDEF void naOrthonormalizeV3(double* d, const double* a, const double* b){
  naMadV3(d, a, b, - naDotV3(b, d));
  naNormalizeV3(d, d);
}
NA_IDEF void naOrthonormalizeV4(double* d, const double* a, const double* b){
  naMadV4(d, a, b, - naDotV4(b, d));
  naNormalizeV4(d, d);
}
NA_IDEF void naOrthonormalizeV2f(float* d, const float* a, const float* b){
  naMadV2f(d, a, b, - naDotV2f(b, d));
  naNormalizeV2f(d, d);
}
NA_IDEF void naOrthonormalizeV3f(float* d, const float* a, const float* b){
  naMadV3f(d, a, b, - naDotV3f(b, d));
  naNormalizeV3f(d, d);
}
NA_IDEF void naOrthonormalizeV4f(float* d, const float* a, const float* b){
  naMadV4f(d, a, b, - naDotV4f(b, d));
  naNormalizeV4f(d, d);
}











// ////////////////////////////////////////////
// Matrix algebra, stored in COLUMN-FIRST order
// ////////////////////////////////////////////




NA_IDEF void naFillM22 ( double*  d,
   double a0 , double a1 , double a2 , double a3){
  d[0] = a0; d[2] = a2;
  d[1] = a1; d[3] = a3;
}
NA_IDEF void naFillM33 ( double*  d,
   double a0 , double a1 , double a2 , double a3 , double a4 , double a5 ,
   double a6 , double a7 , double a8){
  d[0] = a0; d[3] = a3; d[6] = a6;
  d[1] = a1; d[4] = a4; d[7] = a7;
  d[2] = a2; d[5] = a5; d[8] = a8;
}
NA_IDEF void naFillM44 ( double*  d,
   double a0 , double a1 , double a2 , double a3 , double a4 , double a5 ,
   double a6 , double a7 , double a8 , double a9 , double a10, double a11,
   double a12, double a13, double a14, double a15){
  d[0] = a0; d[4] = a4; d[ 8] = a8 ; d[12] = a12;
  d[1] = a1; d[5] = a5; d[ 9] = a9 ; d[13] = a13;
  d[2] = a2; d[6] = a6; d[10] = a10; d[14] = a14;
  d[3] = a3; d[7] = a7; d[11] = a11; d[15] = a15;
}
NA_IDEF void naFillM22f( float* d,
   float  a0 , float  a1 , float  a2 , float  a3){
  d[0] = a0; d[2] = a2;
  d[1] = a1; d[3] = a3;
}
NA_IDEF void naFillM33f( float* d,
   float  a0 , float  a1 , float  a2 , float  a3 , float  a4 , float  a5 ,
   float  a6 , float  a7 , float  a8){
  d[0] = a0; d[3] = a3; d[6] = a6;
  d[1] = a1; d[4] = a4; d[7] = a7;
  d[2] = a2; d[5] = a5; d[8] = a8;
}
NA_IDEF void naFillM44f( float* d,
   float  a0 , float  a1 , float  a2 , float  a3 , float  a4 , float  a5 ,
   float  a6 , float  a7 , float  a8 , float  a9 , float  a10, float  a11,
   float  a12, float  a13, float  a14, float  a15){
  d[0] = a0; d[4] = a4; d[ 8] = a8 ; d[12] = a12;
  d[1] = a1; d[5] = a5; d[ 9] = a9 ; d[13] = a13;
  d[2] = a2; d[6] = a6; d[10] = a10; d[14] = a14;
  d[3] = a3; d[7] = a7; d[11] = a11; d[15] = a15;
}


NA_IDEF void naFillM22WithAxis(double* d,
                         const double* v0,
                         const double* v1){
  naFillM22(d, v0[0], v0[1], v1[0], v1[1]);
}
NA_IDEF void naFillM33WithAxis(double* d,
                         const double* v0,
                         const double* v1,
                         const double* v2){
  naFillM33(d, v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2]);
}
NA_IDEF void naFillM44WithAxis(double* d,
                         const double* v0,
                         const double* v1,
                         const double* v2,
                         const double* v3){
  naFillM44 (d,  v0[0], v0[1], v0[2], v0[3], v1[0], v1[1], v1[2], v1[3], v2[0],
                v2[1], v2[2], v2[3], v3[0], v3[1], v3[2], v3[3]);
}
NA_IDEF void naFillM22fWithAxis(float* d,
                          const float* v0,
                          const float* v1){
  naFillM22f(d, v0[0], v0[1], v1[0], v1[1]);
}
NA_IDEF void naFillM33fWithAxis(float* d,
                          const float* v0,
                          const float* v1,
                          const float* v2){
  naFillM33f(d, v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2]);
}
NA_IDEF void naFillM44fWithAxis(float* d,
                          const float* v0,
                          const float* v1,
                          const float* v2,
                          const float* v3){
  naFillM44f(d,  v0[0], v0[1], v0[2], v0[3], v1[0], v1[1], v1[2], v1[3], v2[0],
                v2[1], v2[2], v2[3], v3[0], v3[1], v3[2], v3[3]);
}


NA_IDEF void naFillM22WithDiag(double* d, double s){
  naFillM22 (d,  s , 0.,
                0., s  );
}
NA_IDEF void naFillM33WithDiag(double* d, double s){
  naFillM33(d, s , 0., 0.,
              0., s , 0.,
              0., 0., s );
}
NA_IDEF void naFillM44WithDiag(double* d, double s){
  naFillM44(d, s , 0., 0., 0.,
              0., s , 0., 0.,
              0., 0., s , 0.,
              0., 0., 0., s );
}
NA_IDEF void naFillM22fWithDiag(float* d, float s){
  naFillM22f(d,  s  , 0.f,
                0.f, s  );
}
NA_IDEF void naFillM33fWithDiag(float* d, float s){
  naFillM33f(d, s  , 0.f, 0.f,
               0.f, s  , 0.f,
               0.f, 0.f, s  );
}
NA_IDEF void naFillM44fWithDiag(float* d, float s){
  naFillM44f(d, s  , 0.f, 0.f, 0.f,
               0.f, s  , 0.f, 0.f,
               0.f, 0.f, s  , 0.f,
               0.f, 0.f, 0.f, s  );
}


NA_IDEF void naFillM22WithDiagV2(double* d, const double* diag){
  naFillM22(d, diag[0], 0.     ,
              0.     , diag[1]);
}
NA_IDEF void naFillM33WithDiagV3(double* d, const double* diag){
  naFillM33(d,
              diag[0], 0.     , 0.,
              0.     , diag[1], 0.,
              0.     , 0.     , diag[2]);
}
NA_IDEF void naFillM44WithDiagV4(double* d, const double* diag){
  naFillM44 (d,  diag[0], 0.     , 0.     , 0.,
                0.     , diag[1], 0.     , 0.,
                0.     , 0.     , diag[2], 0.,
                0.     , 0.     , 0.     , diag[3]);
}
NA_IDEF void naFillM22fWithDiagV2f(float* d, const float* diag){
  naFillM22f(d,  diag[0], 0.f    ,
                0.f    , diag[1]);
}
NA_IDEF void naFillM33fWithDiagV3f(float* d, const float* diag){
  naFillM33f(d,  diag[0], 0.f    , 0.f,
                0.f    , diag[1], 0.f,
                0.f    , 0.f    , diag[2]);
}
NA_IDEF void naFillM44fWithDiagV4f(float* d, const float* diag){
  naFillM44f(d,  diag[0], 0.f    , 0.f    , 0.f,
                0.f    , diag[1], 0.f    , 0.f,
                0.f    , 0.f    , diag[2], 0.f,
                0.f    , 0.f    , 0.f    , diag[3]);
}


NA_IDEF void naCopyM22( double* d, const double* s){
  d[0] = s[0]; d[2] = s[2];
  d[1] = s[1]; d[3] = s[3];
}
NA_IDEF void naCopyM33( double* d, const double* s){
  d[0] = s[0]; d[3] = s[3]; d[6] = s[6];
  d[1] = s[1]; d[4] = s[4]; d[7] = s[7];
  d[2] = s[2]; d[5] = s[5]; d[8] = s[8];
}
NA_IDEF void naCopyM44( double* d, const double* s){
  d[0] = s[0]; d[4] = s[4]; d[ 8] = s[ 8]; d[12] = s[12];
  d[1] = s[1]; d[5] = s[5]; d[ 9] = s[ 9]; d[13] = s[13];
  d[2] = s[2]; d[6] = s[6]; d[10] = s[10]; d[14] = s[14];
  d[3] = s[3]; d[7] = s[7]; d[11] = s[11]; d[15] = s[15];
}
NA_IDEF void naCopyM22f( float* d, const float* s){
  d[0] = s[0]; d[2] = s[2];
  d[1] = s[1]; d[3] = s[3];
}
NA_IDEF void naCopyM33f( float* d, const float* s){
  d[0] = s[0]; d[3] = s[3]; d[6] = s[6];
  d[1] = s[1]; d[4] = s[4]; d[7] = s[7];
  d[2] = s[2]; d[5] = s[5]; d[8] = s[8];
}
NA_IDEF void naCopyM44f( float* d, const float* s){
  d[0] = s[0]; d[4] = s[4]; d[ 8] = s[ 8]; d[12] = s[12];
  d[1] = s[1]; d[5] = s[5]; d[ 9] = s[ 9]; d[13] = s[13];
  d[2] = s[2]; d[6] = s[6]; d[10] = s[10]; d[14] = s[14];
  d[3] = s[3]; d[7] = s[7]; d[11] = s[11]; d[15] = s[15];
}


NA_IDEF void naNegM22( double* d, double* s){
  d[0] = -s[0]; d[2] = -s[2];
  d[1] = -s[1]; d[3] = -s[3];
}
NA_IDEF void naNegM33( double* d, double* s){
  d[0] = -s[0]; d[3] = -s[3]; d[6] = -s[6];
  d[1] = -s[1]; d[4] = -s[4]; d[7] = -s[7];
  d[2] = -s[2]; d[5] = -s[5]; d[8] = -s[8];
}
NA_IDEF void naNegM44( double* d, double* s){
  d[0] = -s[0]; d[4] = -s[4]; d[ 8] = -s[ 8]; d[12] = -s[12];
  d[1] = -s[1]; d[5] = -s[5]; d[ 9] = -s[ 9]; d[13] = -s[13];
  d[2] = -s[2]; d[6] = -s[6]; d[10] = -s[10]; d[14] = -s[14];
  d[3] = -s[3]; d[7] = -s[7]; d[11] = -s[11]; d[15] = -s[15];
}
NA_IDEF void naNegM22f( float* d, float* s){
  d[0] = -s[0]; d[2] = -s[2];
  d[1] = -s[1]; d[3] = -s[3];
}
NA_IDEF void naNegM33f( float* d, float* s){
  d[0] = -s[0]; d[3] = -s[3]; d[6] = -s[6];
  d[1] = -s[1]; d[4] = -s[4]; d[7] = -s[7];
  d[2] = -s[2]; d[5] = -s[5]; d[8] = -s[8];
}
NA_IDEF void naNegM44f( float* d, float* s){
  d[0] = -s[0]; d[4] = -s[4]; d[ 8] = -s[ 8]; d[12] = -s[12];
  d[1] = -s[1]; d[5] = -s[5]; d[ 9] = -s[ 9]; d[13] = -s[13];
  d[2] = -s[2]; d[6] = -s[6]; d[10] = -s[10]; d[14] = -s[14];
  d[3] = -s[3]; d[7] = -s[7]; d[11] = -s[11]; d[15] = -s[15];
}


NA_IDEF void naAddM22 (double*  d, const double*  a, const double*  b){
  d[0] = a[0]+b[0]; d[2] = a[2]+b[2];
  d[1] = a[1]+b[1]; d[3] = a[3]+b[3];
}
NA_IDEF void naAddM33 (double*  d, const double*  a, const double*  b){
  d[0] = a[0]+b[0]; d[3] = a[3]+b[3]; d[6] = a[6]+b[6];
  d[1] = a[1]+b[1]; d[4] = a[4]+b[4]; d[7] = a[7]+b[7];
  d[2] = a[2]+b[2]; d[5] = a[5]+b[5]; d[8] = a[8]+b[8];
}
NA_IDEF void naAddM44(double* d, const double* a, const double* b){
  d[0] = a[0]+b[0]; d[4] = a[4]+b[4]; d[ 8] = a[ 8]+b[ 8]; d[12] = a[12]+b[12];
  d[1] = a[1]+b[1]; d[5] = a[5]+b[5]; d[ 9] = a[ 9]+b[ 9]; d[13] = a[13]+b[13];
  d[2] = a[2]+b[2]; d[6] = a[6]+b[6]; d[10] = a[10]+b[10]; d[14] = a[14]+b[14];
  d[3] = a[3]+b[3]; d[7] = a[7]+b[7]; d[11] = a[11]+b[11]; d[15] = a[15]+b[15];
}
NA_IDEF void naAddM22f(float* d, const float* a, const float* b){
  d[0] = a[0]+b[0]; d[2] = a[2]+b[2];
  d[1] = a[1]+b[1]; d[3] = a[3]+b[3];
}
NA_IDEF void naAddM33f(float* d, const float* a, const float* b){
  d[0] = a[0]+b[0]; d[3] = a[3]+b[3]; d[6] = a[6]+b[6];
  d[1] = a[1]+b[1]; d[4] = a[4]+b[4]; d[7] = a[7]+b[7];
  d[2] = a[2]+b[2]; d[5] = a[5]+b[5]; d[8] = a[8]+b[8];
}
NA_IDEF void naAddM44f(float* d, const float* a, const float* b){
  d[0] = a[0]+b[0]; d[4] = a[4]+b[4]; d[ 8] = a[ 8]+b[ 8]; d[12] = a[12]+b[12];
  d[1] = a[1]+b[1]; d[5] = a[5]+b[5]; d[ 9] = a[ 9]+b[ 9]; d[13] = a[13]+b[13];
  d[2] = a[2]+b[2]; d[6] = a[6]+b[6]; d[10] = a[10]+b[10]; d[14] = a[14]+b[14];
  d[3] = a[3]+b[3]; d[7] = a[7]+b[7]; d[11] = a[11]+b[11]; d[15] = a[15]+b[15];
}


NA_IDEF void naSubM22 (double*  d, const double*  a, const double*  b){
  d[0] = a[0]-b[0]; d[2] = a[2]-b[2];
  d[1] = a[1]-b[1]; d[3] = a[3]-b[3];
}
NA_IDEF void naSubM33 (double*  d, const double*  a, const double*  b){
  d[0] = a[0]-b[0]; d[3] = a[3]-b[3]; d[6] = a[6]-b[6];
  d[1] = a[1]-b[1]; d[4] = a[4]-b[4]; d[7] = a[7]-b[7];
  d[2] = a[2]-b[2]; d[5] = a[5]-b[5]; d[8] = a[8]-b[8];
}
NA_IDEF void naSubM44(double* d, const double* a, const double* b){
  d[0] = a[0]-b[0]; d[4] = a[4]-b[4]; d[ 8] = a[ 8]-b[ 8]; d[12] = a[12]-b[12];
  d[1] = a[1]-b[1]; d[5] = a[5]-b[5]; d[ 9] = a[ 9]-b[ 9]; d[13] = a[13]-b[13];
  d[2] = a[2]-b[2]; d[6] = a[6]-b[6]; d[10] = a[10]-b[10]; d[14] = a[14]-b[14];
  d[3] = a[3]-b[3]; d[7] = a[7]-b[7]; d[11] = a[11]-b[11]; d[15] = a[15]-b[15];
}
NA_IDEF void naSubM22f(float* d, const float* a, const float* b){
  d[0] = a[0]-b[0]; d[2] = a[2]-b[2];
  d[1] = a[1]-b[1]; d[3] = a[3]-b[3];
}
NA_IDEF void naSubM33f(float* d, const float* a, const float* b){
  d[0] = a[0]-b[0]; d[3] = a[3]-b[3]; d[6] = a[6]-b[6];
  d[1] = a[1]-b[1]; d[4] = a[4]-b[4]; d[7] = a[7]-b[7];
  d[2] = a[2]-b[2]; d[5] = a[5]-b[5]; d[8] = a[8]-b[8];
}
NA_IDEF void naSubM44f(float* d, const float* a, const float* b){
  d[0] = a[0]-b[0]; d[4] = a[4]-b[4]; d[ 8] = a[ 8]-b[ 8]; d[12] = a[12]-b[12];
  d[1] = a[1]-b[1]; d[5] = a[5]-b[5]; d[ 9] = a[ 9]-b[ 9]; d[13] = a[13]-b[13];
  d[2] = a[2]-b[2]; d[6] = a[6]-b[6]; d[10] = a[10]-b[10]; d[14] = a[14]-b[14];
  d[3] = a[3]-b[3]; d[7] = a[7]-b[7]; d[11] = a[11]-b[11]; d[15] = a[15]-b[15];
}


NA_IDEF void naScaleM22 (double*  d, const double*  a, double s){
  d[0] = a[0]*s; d[2] = a[2]*s;
  d[1] = a[1]*s; d[3] = a[3]*s;
}
NA_IDEF void naScaleM33 (double*  d, const double*  a, double s){
  d[0] = a[0]*s; d[3] = a[3]*s; d[6] = a[6]*s;
  d[1] = a[1]*s; d[4] = a[4]*s; d[7] = a[7]*s;
  d[2] = a[2]*s; d[5] = a[5]*s; d[8] = a[8]*s;
}
NA_IDEF void naScaleM44(double* d, const double* a, double s){
  d[0] = a[0]*s; d[4] = a[4]*s; d[ 8] = a[ 8]*s; d[12] = a[12]*s;
  d[1] = a[1]*s; d[5] = a[5]*s; d[ 9] = a[ 9]*s; d[13] = a[13]*s;
  d[2] = a[2]*s; d[6] = a[6]*s; d[10] = a[10]*s; d[14] = a[14]*s;
  d[3] = a[3]*s; d[7] = a[7]*s; d[11] = a[11]*s; d[15] = a[15]*s;
}
NA_IDEF void naScaleM22f(float* d, const float* a, float s){
  d[0] = a[0]*s; d[2] = a[2]*s;
  d[1] = a[1]*s; d[3] = a[3]*s;
}
NA_IDEF void naScaleM33f(float* d, const float* a, float s){
  d[0] = a[0]*s; d[3] = a[3]*s; d[6] = a[6]*s;
  d[1] = a[1]*s; d[4] = a[4]*s; d[7] = a[7]*s;
  d[2] = a[2]*s; d[5] = a[5]*s; d[8] = a[8]*s;
}
NA_IDEF void naScaleM44f(float* d, const float* a, float s){
  d[0] = a[0]*s; d[4] = a[4]*s; d[ 8] = a[ 8]*s; d[12] = a[12]*s;
  d[1] = a[1]*s; d[5] = a[5]*s; d[ 9] = a[ 9]*s; d[13] = a[13]*s;
  d[2] = a[2]*s; d[6] = a[6]*s; d[10] = a[10]*s; d[14] = a[14]*s;
  d[3] = a[3]*s; d[7] = a[7]*s; d[11] = a[11]*s; d[15] = a[15]*s;
}



NA_IDEF void naScaleM22V2( double* d, const double*  a, const double* v){
  d[0] = a[0]*v[0]; d[2] = a[2]*v[1];
  d[1] = a[1]*v[0]; d[3] = a[3]*v[1];
}
NA_IDEF void naScaleM33V3( double* d, const double*  a, const double* v){
  d[0] = a[0]*v[0]; d[3] = a[3]*v[1]; d[6] = a[6]*v[2];
  d[1] = a[1]*v[0]; d[4] = a[4]*v[1]; d[7] = a[7]*v[2];
  d[2] = a[2]*v[0]; d[5] = a[5]*v[1]; d[8] = a[8]*v[2];
}
NA_IDEF void naScaleM44V4( double* d, const double* a, const double* v){
  d[0] = a[0]*v[0]; d[4] = a[4]*v[1]; d[ 8] = a[ 8]*v[2]; d[12] = a[12]*v[3];
  d[1] = a[1]*v[0]; d[5] = a[5]*v[1]; d[ 9] = a[ 9]*v[2]; d[13] = a[13]*v[3];
  d[2] = a[2]*v[0]; d[6] = a[6]*v[1]; d[10] = a[10]*v[2]; d[14] = a[14]*v[3];
  d[3] = a[3]*v[0]; d[7] = a[7]*v[1]; d[11] = a[11]*v[2]; d[15] = a[15]*v[3];
}
NA_IDEF void naScaleM22fV2f( float* d, const float* a, const float* v){
  d[0] = a[0]*v[0]; d[2] = a[2]*v[1];
  d[1] = a[1]*v[0]; d[3] = a[3]*v[1];
}
NA_IDEF void naScaleM33fV3f( float* d, const float* a, const float* v){
  d[0] = a[0]*v[0]; d[3] = a[3]*v[1]; d[6] = a[6]*v[2];
  d[1] = a[1]*v[0]; d[4] = a[4]*v[1]; d[7] = a[7]*v[2];
  d[2] = a[2]*v[0]; d[5] = a[5]*v[1]; d[8] = a[8]*v[2];
}
NA_IDEF void naScaleM44fV4f( float* d, const float* a, const float* v){
  d[0] = a[0]*v[0]; d[4] = a[4]*v[1]; d[ 8] = a[ 8]*v[2]; d[12] = a[12]*v[3];
  d[1] = a[1]*v[0]; d[5] = a[5]*v[1]; d[ 9] = a[ 9]*v[2]; d[13] = a[13]*v[3];
  d[2] = a[2]*v[0]; d[6] = a[6]*v[1]; d[10] = a[10]*v[2]; d[14] = a[14]*v[3];
  d[3] = a[3]*v[0]; d[7] = a[7]*v[1]; d[11] = a[11]*v[2]; d[15] = a[15]*v[3];
}



NA_IDEF void naMulM22V2(double* d, const double* m, const double* v){
  d[0] = m[0]*v[0] + m[2]*v[1];
  d[1] = m[1]*v[0] + m[3]*v[1];
}
NA_IDEF void naMulM33V3(double* d, const double* m, const double* v){
  d[0] = m[0]*v[0] + m[3]*v[1] + m[6]*v[2];
  d[1] = m[1]*v[0] + m[4]*v[1] + m[7]*v[2];
  d[2] = m[2]*v[0] + m[5]*v[1] + m[8]*v[2];
}
NA_IDEF void naMulM44V4(double* d, const double* m, const double* v){
  d[0] = m[0]*v[0] + m[4]*v[1] + m[ 8]*v[2] + m[12]*v[3];
  d[1] = m[1]*v[0] + m[5]*v[1] + m[ 9]*v[2] + m[13]*v[3];
  d[2] = m[2]*v[0] + m[6]*v[1] + m[10]*v[2] + m[14]*v[3];
  d[3] = m[3]*v[0] + m[7]*v[1] + m[11]*v[2] + m[15]*v[3];
}
NA_IDEF void naMulM22fV2f(float* d, const float* m, const float* v){
  d[0] = m[0]*v[0] + m[2]*v[1];
  d[1] = m[1]*v[0] + m[3]*v[1];
}
NA_IDEF void naMulM33fV3f(float* d, const float* m, const float* v){
  d[0] = m[0]*v[0] + m[3]*v[1] + m[6]*v[2];
  d[1] = m[1]*v[0] + m[4]*v[1] + m[7]*v[2];
  d[2] = m[2]*v[0] + m[5]*v[1] + m[8]*v[2];
}
NA_IDEF void naMulM44fV4f(float* d, const float* m, const float* v){
  d[0] = m[0]*v[0] + m[4]*v[1] + m[ 8]*v[2] + m[12]*v[3];
  d[1] = m[1]*v[0] + m[5]*v[1] + m[ 9]*v[2] + m[13]*v[3];
  d[2] = m[2]*v[0] + m[6]*v[1] + m[10]*v[2] + m[14]*v[3];
  d[3] = m[3]*v[0] + m[7]*v[1] + m[11]*v[2] + m[15]*v[3];
}




NA_IDEF void naMulM22M22(double* d, const double* m, const double* a){
  naFillM22(d,   m[0]*a[0] + m[2]*a[1],
                m[1]*a[0] + m[3]*a[1],
                m[0]*a[2] + m[2]*a[3],
                m[1]*a[2] + m[3]*a[3]);
}
NA_IDEF void naMulM33M33(double* d, const double* m, const double* a){
  naFillM33(d,   m[0]*a[0] + m[3]*a[1] + m[6]*a[2],
                m[1]*a[0] + m[4]*a[1] + m[7]*a[2],
                m[2]*a[0] + m[5]*a[1] + m[8]*a[2],
                m[0]*a[3] + m[3]*a[4] + m[6]*a[5],
                m[1]*a[3] + m[4]*a[4] + m[7]*a[5],
                m[2]*a[3] + m[5]*a[4] + m[8]*a[5],
                m[0]*a[6] + m[3]*a[7] + m[6]*a[8],
                m[1]*a[6] + m[4]*a[7] + m[7]*a[8],
                m[2]*a[6] + m[5]*a[7] + m[8]*a[8]);
}
NA_IDEF void naMulM44M44(double* d, const double* m, const double* a){
  naFillM44(d,   m[0]*a[ 0] + m[4]*a[ 1] + m[ 8]*a[ 2] + m[12]*a[ 3],
                m[1]*a[ 0] + m[5]*a[ 1] + m[ 9]*a[ 2] + m[13]*a[ 3],
                m[2]*a[ 0] + m[6]*a[ 1] + m[10]*a[ 2] + m[14]*a[ 3],
                m[3]*a[ 0] + m[7]*a[ 1] + m[11]*a[ 2] + m[15]*a[ 3],
                m[0]*a[ 4] + m[4]*a[ 5] + m[ 8]*a[ 6] + m[12]*a[ 7],
                m[1]*a[ 4] + m[5]*a[ 5] + m[ 9]*a[ 6] + m[13]*a[ 7],
                m[2]*a[ 4] + m[6]*a[ 5] + m[10]*a[ 6] + m[14]*a[ 7],
                m[3]*a[ 4] + m[7]*a[ 5] + m[11]*a[ 6] + m[15]*a[ 7],
                m[0]*a[ 8] + m[4]*a[ 9] + m[ 8]*a[10] + m[12]*a[11],
                m[1]*a[ 8] + m[5]*a[ 9] + m[ 9]*a[10] + m[13]*a[11],
                m[2]*a[ 8] + m[6]*a[ 9] + m[10]*a[10] + m[14]*a[11],
                m[3]*a[ 8] + m[7]*a[ 9] + m[11]*a[10] + m[15]*a[11],
                m[0]*a[12] + m[4]*a[13] + m[ 8]*a[14] + m[12]*a[15],
                m[1]*a[12] + m[5]*a[13] + m[ 9]*a[14] + m[13]*a[15],
                m[2]*a[12] + m[6]*a[13] + m[10]*a[14] + m[14]*a[15],
                m[3]*a[12] + m[7]*a[13] + m[11]*a[14] + m[15]*a[15]);
}
NA_IDEF void naMulM22fM22f(float* d, const float* m, const float* a){
  naFillM22f(d,  m[0]*a[0] + m[2]*a[1],
                m[1]*a[0] + m[3]*a[1],
                m[0]*a[2] + m[2]*a[3],
                m[1]*a[2] + m[3]*a[3]);
}
NA_IDEF void naMulM33fM33f(float* d, const float* m, const float* a){
  naFillM33f(d,  m[0]*a[0] + m[3]*a[1] + m[6]*a[2],
                m[1]*a[0] + m[4]*a[1] + m[7]*a[2],
                m[2]*a[0] + m[5]*a[1] + m[8]*a[2],
                m[0]*a[3] + m[3]*a[4] + m[6]*a[5],
                m[1]*a[3] + m[4]*a[4] + m[7]*a[5],
                m[2]*a[3] + m[5]*a[4] + m[8]*a[5],
                m[0]*a[6] + m[3]*a[7] + m[6]*a[8],
                m[1]*a[6] + m[4]*a[7] + m[7]*a[8],
                m[2]*a[6] + m[5]*a[7] + m[8]*a[8]);
}
NA_IDEF void naMulM44fM44f(float* d, const float* m, const float* a){
  naFillM44f(d,  m[0]*a[ 0] + m[4]*a[ 1] + m[ 8]*a[ 2] + m[12]*a[ 3],
                m[1]*a[ 0] + m[5]*a[ 1] + m[ 9]*a[ 2] + m[13]*a[ 3],
                m[2]*a[ 0] + m[6]*a[ 1] + m[10]*a[ 2] + m[14]*a[ 3],
                m[3]*a[ 0] + m[7]*a[ 1] + m[11]*a[ 2] + m[15]*a[ 3],
                m[0]*a[ 4] + m[4]*a[ 5] + m[ 8]*a[ 6] + m[12]*a[ 7],
                m[1]*a[ 4] + m[5]*a[ 5] + m[ 9]*a[ 6] + m[13]*a[ 7],
                m[2]*a[ 4] + m[6]*a[ 5] + m[10]*a[ 6] + m[14]*a[ 7],
                m[3]*a[ 4] + m[7]*a[ 5] + m[11]*a[ 6] + m[15]*a[ 7],
                m[0]*a[ 8] + m[4]*a[ 9] + m[ 8]*a[10] + m[12]*a[11],
                m[1]*a[ 8] + m[5]*a[ 9] + m[ 9]*a[10] + m[13]*a[11],
                m[2]*a[ 8] + m[6]*a[ 9] + m[10]*a[10] + m[14]*a[11],
                m[3]*a[ 8] + m[7]*a[ 9] + m[11]*a[10] + m[15]*a[11],
                m[0]*a[12] + m[4]*a[13] + m[ 8]*a[14] + m[12]*a[15],
                m[1]*a[12] + m[5]*a[13] + m[ 9]*a[14] + m[13]*a[15],
                m[2]*a[12] + m[6]*a[13] + m[10]*a[14] + m[14]*a[15],
                m[3]*a[12] + m[7]*a[13] + m[11]*a[14] + m[15]*a[15]);
}


NA_IDEF void naInvertM22(double* d, const double* m){
  double determinant = m[0]*m[3] - m[2]*m[1];
  double divisor = naInv(determinant);
  naFillM22(d,     m[3] * divisor,
                - m[1] * divisor,
                - m[2] * divisor,
                  m[0] * divisor);
}
NA_IDEF void naInvertM33(double* d, const double* m){
  double d0 = m[4] * m[8] - m[5] * m[7];
  double d1 = m[2] * m[7] - m[1] * m[8];
  double d2 = m[1] * m[5] - m[2] * m[4];
  double determinant = m[0]*d0 + m[3]*d1 + m[6]*d2;
  double divisor = naInv(determinant);
  naFillM33(d,     d0 * divisor,
                  d1 * divisor,
                  d2 * divisor,
                  (m[5]*m[6] - m[3]*m[8]) * divisor,
                  (m[0]*m[8] - m[2]*m[6]) * divisor,
                  (m[2]*m[3] - m[0]*m[5]) * divisor,
                  (m[3]*m[7] - m[4]*m[6]) * divisor,
                  (m[1]*m[6] - m[0]*m[7]) * divisor,
                  (m[0]*m[4] - m[1]*m[3]) * divisor);
}
NA_IDEF void naInvertM44(double* d, const double* m){
    double a01 = m[10]*m[15] - m[11]*m[14];
    double a02 = m[ 6]*m[15] - m[ 7]*m[14];
    double a03 = m[ 6]*m[11] - m[ 7]*m[10];
    double a04 = m[ 2]*m[15] - m[ 3]*m[14];
    double a05 = m[ 2]*m[11] - m[ 3]*m[10];
    double a06 = m[ 2]*m[ 7] - m[ 3]*m[ 6];
    double d0 = + m[ 5]*a01 - m[ 9]*a02 + m[13]*a03;
    double d1 = - m[ 1]*a01 + m[ 9]*a04 - m[13]*a05;
    double d2 = + m[ 1]*a02 - m[ 5]*a04 + m[13]*a06;
    double d3 = - m[ 1]*a03 + m[ 5]*a05 - m[ 9]*a06;
    double determinant = m[ 0]*d0 + m[ 4]*d1 + m[ 8]*d2 + m[12]*d3;
    double divisor = naInv(determinant);
    double a07 = m[ 9]*m[15] - m[11]*m[13];
    double a08 = m[ 5]*m[15] - m[ 7]*m[13];
    double a09 = m[ 5]*m[11] - m[ 7]*m[ 9];
    double a10 = m[ 1]*m[15] - m[ 3]*m[13];
    double a11 = m[ 1]*m[11] - m[ 3]*m[ 9];
    double a12 = m[ 1]*m[ 7] - m[ 3]*m[ 5];
    double a13 = m[ 9]*m[14] - m[10]*m[13];
    double a14 = m[ 5]*m[14] - m[ 6]*m[13];
    double a15 = m[ 5]*m[10] - m[ 6]*m[ 9];
    double a16 = m[ 1]*m[14] - m[ 2]*m[13];
    double a17 = m[ 1]*m[10] - m[ 2]*m[ 9];
    double a18 = m[ 1]*m[ 6] - m[ 2]*m[ 5];
    naFillM44(d,   d0 * divisor,
                  d1 * divisor,
                  d2 * divisor,
                  d3 * divisor,
                  ( - m[ 4]*a01 + m[ 8]*a02 - m[12]*a03) * divisor,
                  ( + m[ 0]*a01 - m[ 8]*a04 + m[12]*a05) * divisor,
                  ( - m[ 0]*a02 + m[ 4]*a04 - m[12]*a06) * divisor,
                  ( + m[ 0]*a03 - m[ 4]*a05 + m[ 8]*a06) * divisor,
                  ( + m[ 4]*a07 - m[ 8]*a08 + m[12]*a09) * divisor,
                  ( - m[ 0]*a07 + m[ 8]*a10 - m[12]*a11) * divisor,
                  ( + m[ 0]*a08 - m[ 4]*a10 + m[12]*a12) * divisor,
                  ( - m[ 0]*a09 + m[ 4]*a11 - m[ 8]*a12) * divisor,
                  ( - m[ 4]*a13 + m[ 8]*a14 - m[12]*a15) * divisor,
                  ( + m[ 0]*a13 - m[ 8]*a16 + m[12]*a17) * divisor,
                  ( - m[ 0]*a14 + m[ 4]*a16 - m[12]*a18) * divisor,
                  ( + m[ 0]*a15 - m[ 4]*a17 + m[ 8]*a18) * divisor);
}
NA_IDEF void naInvertM22f(float* d, const float* m){
  float determinant = m[0]*m[3] - m[2]*m[1];
  float divisor = naInvf(determinant);
  naFillM22f(d,    m[3] * divisor,
                - m[1] * divisor,
                - m[2] * divisor,
                  m[0] * divisor);
}
NA_IDEF void naInvertM33f(float* d, const float* m){
  float d0 = m[4] * m[8] - m[5] * m[7];
  float d1 = m[2] * m[7] - m[1] * m[8];
  float d2 = m[1] * m[5] - m[2] * m[4];
  float determinant = m[0]*d0 + m[3]*d1 + m[6]*d2;
  float divisor = naInvf(determinant);
  naFillM33f(d,    d0 * divisor,
                  d1 * divisor,
                  d2 * divisor,
                  (m[5]*m[6] - m[3]*m[8]) * divisor,
                  (m[0]*m[8] - m[2]*m[6]) * divisor,
                  (m[2]*m[3] - m[0]*m[5]) * divisor,
                  (m[3]*m[7] - m[4]*m[6]) * divisor,
                  (m[1]*m[6] - m[0]*m[7]) * divisor,
                  (m[0]*m[4] - m[1]*m[3]) * divisor);
}
NA_IDEF void naInvertM44f(float* d, const float* m){
    float a01 = m[10]*m[15] - m[11]*m[14];
    float a02 = m[ 6]*m[15] - m[ 7]*m[14];
    float a03 = m[ 6]*m[11] - m[ 7]*m[10];
    float a04 = m[ 2]*m[15] - m[ 3]*m[14];
    float a05 = m[ 2]*m[11] - m[ 3]*m[10];
    float a06 = m[ 2]*m[ 7] - m[ 3]*m[ 6];
    float d0 = + m[ 5]*a01 - m[ 9]*a02 + m[13]*a03;
    float d1 = - m[ 1]*a01 + m[ 9]*a04 - m[13]*a05;
    float d2 = + m[ 1]*a02 - m[ 5]*a04 + m[13]*a06;
    float d3 = - m[ 1]*a03 + m[ 5]*a05 - m[ 9]*a06;
    float determinant = m[ 0]*d0 + m[ 4]*d1 + m[ 8]*d2 + m[12]*d3;
    float divisor = naInvf(determinant);
    float a07 = m[ 9]*m[15] - m[11]*m[13];
    float a08 = m[ 5]*m[15] - m[ 7]*m[13];
    float a09 = m[ 5]*m[11] - m[ 7]*m[ 9];
    float a10 = m[ 1]*m[15] - m[ 3]*m[13];
    float a11 = m[ 1]*m[11] - m[ 3]*m[ 9];
    float a12 = m[ 1]*m[ 7] - m[ 3]*m[ 5];
    float a13 = m[ 9]*m[14] - m[10]*m[13];
    float a14 = m[ 5]*m[14] - m[ 6]*m[13];
    float a15 = m[ 5]*m[10] - m[ 6]*m[ 9];
    float a16 = m[ 1]*m[14] - m[ 2]*m[13];
    float a17 = m[ 1]*m[10] - m[ 2]*m[ 9];
    float a18 = m[ 1]*m[ 6] - m[ 2]*m[ 5];
    naFillM44f(d,  d0 * divisor,
                  d1 * divisor,
                  d2 * divisor,
                  d3 * divisor,
                  ( - m[ 4]*a01 + m[ 8]*a02 - m[12]*a03) * divisor,
                  ( + m[ 0]*a01 - m[ 8]*a04 + m[12]*a05) * divisor,
                  ( - m[ 0]*a02 + m[ 4]*a04 - m[12]*a06) * divisor,
                  ( + m[ 0]*a03 - m[ 4]*a05 + m[ 8]*a06) * divisor,
                  ( + m[ 4]*a07 - m[ 8]*a08 + m[12]*a09) * divisor,
                  ( - m[ 0]*a07 + m[ 8]*a10 - m[12]*a11) * divisor,
                  ( + m[ 0]*a08 - m[ 4]*a10 + m[12]*a12) * divisor,
                  ( - m[ 0]*a09 + m[ 4]*a11 - m[ 8]*a12) * divisor,
                  ( - m[ 4]*a13 + m[ 8]*a14 - m[12]*a15) * divisor,
                  ( + m[ 0]*a13 - m[ 8]*a16 + m[12]*a17) * divisor,
                  ( - m[ 0]*a14 + m[ 4]*a16 - m[12]*a18) * divisor,
                  ( + m[ 0]*a15 - m[ 4]*a17 + m[ 8]*a18) * divisor);
}




NA_IAPI void naFillM33WithRotationAlongAxis(double* d, const double* axis, double angle){
  double sinang    = naSin(.5 * angle);
  double sinang2   = sinang * sinang;
  double sinangT2  = 2. * sinang;
  double cosang    = naCos(.5 * angle);
  double ax0       = axis[0] * cosang;
  double ax00      = axis[0] * axis[0];
  double ax01      = axis[0] * axis[1] * sinang;
  double ax02      = axis[0] * axis[2] * sinang;
  double ax1       = axis[1] * cosang;
  double ax11      = axis[1] * axis[1];
  double ax12      = axis[1] * axis[2] * sinang;
  double ax2       = axis[2] * cosang;
  double ax22      = axis[2] * axis[2];
  
  naFillM33(d,
          1. + sinang2 * (ax00 - ax11 - ax22 - 1.),
          sinangT2 * (ax01 + ax2),
          sinangT2 * (ax02 - ax1),
          sinangT2 * (ax01 - ax2),
          1. + sinang2 * (-ax00 + ax11 - ax22 - 1.),
          sinangT2 * (ax12 + ax0),
          sinangT2 * (ax02 + ax1),
          sinangT2 * (ax12 - ax0),
          1. + sinang2 * (-ax00 - ax11 + ax22 - 1.));
}


NA_IAPI void naFillM33WithRotationAngles(double* d, double roll, double pitch, double yaw){
  double q1 = - naCos(.5 * (roll - yaw)) * naSin(.5 * pitch);
  double q2 = naSin(.5 * (roll - yaw)) * naSin(.5 * pitch);
  double q3 = - naSin(.5 * (roll + yaw)) * naCos(.5 * pitch);
  double q4 = naCos(.5 * (roll + yaw)) * naCos(.5 * pitch);

  naFillM33(d,
          1. - 2. * ( q2*q2 - q3*q3),
          2. * (q1*q2 + q3*q4),
          2. * (q1*q3 - q2*q4),
          2. * (q1*q2 - q3*q4),
          1. - 2. * ( q1*q1 - q3*q3),
          2. * (q1*q4 + q2*q3),          
          2. * (q1*q3 + q2*q4),
          2. * (q2*q3 - q1*q4),
          1. - 2. * ( q1*q1 - q2*q2));
}


#ifdef __cplusplus 
  } // extern "C"
#endif
#endif // NA_VECTOR_ALGEBRA_INCLUDED


// Copyright (c) NALib, Tobias Stamm, Manderim GmbH
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
// in all copies or substantial portions of the source-code.
//
// In case the source-code of this software is inaccessible to the end-user,
// the above copyright notice and this permission notice shall be included
// in any source-code which is dependent on this software and is accessible
// to the end-user.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
