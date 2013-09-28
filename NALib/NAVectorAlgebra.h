
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_VECTOR_ALGEBRA_INCLUDED
#define NA_VECTOR_ALGEBRA_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"

// Note that in NALib, vectors are plain C-arrays. This means that even though
// all arguments are typedef'd, the programmer is responsible for making sure
// the given arrays have indeed the required number of entries. The author
// decided to do this very plainly as this hardly ever becomes a problem, let
// alone being very easy to debug if there would be any. During the last 10
// years, there were barely any of 'em.

typedef float NAVec1f[1];
typedef float NAVec2f[2];
typedef float NAVec3f[3];
typedef float NAVec4f[4];
typedef double NAVec1[1];
typedef double NAVec2[2];
typedef double NAVec3[3];
typedef double NAVec4[4];

// Note that a mathematical operation with a vector result will always be
// performed on the d vector. This might at first be incovenient when
// programming, but the author has not seen many occasions where it becomes
// cumbersome. More so, when using this consistently, the compiler can highly
// optimize the code.

// Fills vector d with the given values
NA_IAPI void naSetV1f(NAVec1f d, float  a0);
NA_IAPI void naSetV2f(NAVec2f d, float  a0, float  a1);
NA_IAPI void naSetV3f(NAVec3f d, float  a0, float  a1, float  a2);
NA_IAPI void naSetV4f(NAVec4f d, float  a0, float  a1, float  a2, float  a3);
NA_IAPI void naSetV1 (NAVec1  d, double a0);
NA_IAPI void naSetV2 (NAVec2  d, double a0, double a1);
NA_IAPI void naSetV3 (NAVec3  d, double a0, double a1, double a2);
NA_IAPI void naSetV4 (NAVec4  d, double a0, double a1, double a2, double a3);

// Fills vector d with random values in [0, 1)
NA_IAPI void naSetV1fRandom(NAVec1f d);
NA_IAPI void naSetV2fRandom(NAVec2f d);
NA_IAPI void naSetV3fRandom(NAVec3f d);
NA_IAPI void naSetV4fRandom(NAVec4f d);

// Fills vector d with random vectors in a unit sphere. If filled is NA_TRUE,
// the length of d will be smallerequal than 1. If filled is NA_FALSE, the
// length of d will be precisely 1.
NA_IAPI void naSetV1fRandomSpherical(NAVec1f d, NABool filled);
NA_IAPI void naSetV2fRandomSpherical(NAVec2f d, NABool filled);
NA_IAPI void naSetV3fRandomSpherical(NAVec3f d, NABool filled);
NA_IAPI void naSetV4fRandomSpherical(NAVec4f d, NABool filled);

// Copies vector s to d
NA_IAPI void naCpyV1f(NAVec1f d, const NAVec1f s);
NA_IAPI void naCpyV2f(NAVec2f d, const NAVec2f s);
NA_IAPI void naCpyV3f(NAVec3f d, const NAVec3f s);
NA_IAPI void naCpyV4f(NAVec4f d, const NAVec4f s);

// Negates the Vector. d = -d
NA_IAPI void naNegV1f(NAVec1f d);
NA_IAPI void naNegV2f(NAVec2f d);
NA_IAPI void naNegV3f(NAVec3f d);
NA_IAPI void naNegV4f(NAVec4f d);

// Adds vector a to d and stores it in d
NA_IAPI void naAddV1f(NAVec1f d, const NAVec1f a);
NA_IAPI void naAddV2f(NAVec2f d, const NAVec2f a);
NA_IAPI void naAddV3f(NAVec3f d, const NAVec3f a);
NA_IAPI void naAddV4f(NAVec4f d, const NAVec4f a);

// Subtracts vector a from d and stores it in d
NA_IAPI void naSubV1f(NAVec1f d, const NAVec1f a);
NA_IAPI void naSubV2f(NAVec2f d, const NAVec2f a);
NA_IAPI void naSubV3f(NAVec3f d, const NAVec3f a);
NA_IAPI void naSubV4f(NAVec4f d, const NAVec4f a);

// Multiplies vector d with scalar s and stores it in d
NA_IAPI void naMulV1f(NAVec1f d, float s);
NA_IAPI void naMulV2f(NAVec2f d, float s);
NA_IAPI void naMulV3f(NAVec3f d, float s);
NA_IAPI void naMulV4f(NAVec4f d, float s);

// Divides vector d by scalar s and stores it in d
NA_IAPI void naDivV1f(NAVec1f d, float s);
NA_IAPI void naDivV2f(NAVec2f d, float s);
NA_IAPI void naDivV3f(NAVec3f d, float s);
NA_IAPI void naDivV4f(NAVec4f d, float s);

// Divides vector d componentwise by vector a and stores it in d
NA_IAPI void naDivV1fComponentwise(NAVec1f d, const NAVec1f a);
NA_IAPI void naDivV2fComponentwise(NAVec2f d, const NAVec2f a);
NA_IAPI void naDivV3fComponentwise(NAVec3f d, const NAVec3f a);
NA_IAPI void naDivV4fComponentwise(NAVec4f d, const NAVec4f a);

// Multiply and add: Computes d += b * s
NA_IAPI void naMadV1f(NAVec1f d, const NAVec1f b, float s);
NA_IAPI void naMadV2f(NAVec2f d, const NAVec1f b, float s);
NA_IAPI void naMadV3f(NAVec3f d, const NAVec1f b, float s);
NA_IAPI void naMadV4f(NAVec4f d, const NAVec1f b, float s);

// Returns true if the two vectors are equal
NA_IAPI NABool naEqualV1f(const NAVec1f a, const NAVec1f b);
NA_IAPI NABool naEqualV2f(const NAVec2f a, const NAVec2f b);
NA_IAPI NABool naEqualV3f(const NAVec3f a, const NAVec3f b);
NA_IAPI NABool naEqualV4f(const NAVec4f a, const NAVec4f b);

// Computes the dot product (scalar product, inner product) of the two vectors.
NA_IAPI float naDotV1f(const NAVec1f a, const NAVec1f b);
NA_IAPI float naDotV2f(const NAVec2f a, const NAVec2f b);
NA_IAPI float naDotV3f(const NAVec3f a, const NAVec3f b);
NA_IAPI float naDotV4f(const NAVec4f a, const NAVec4f b);

// Cross product. Only defined for V3. d = a X b
NA_IAPI void naCrossV3f(NAVec3f d, const NAVec3f a, const NAVec3f b);

// Computes the length (2-Norm) of the given vector
NA_IAPI float naLengthV1f(const NAVec1f a);
NA_IAPI float naLengthV2f(const NAVec2f a);
NA_IAPI float naLengthV3f(const NAVec3f a);
NA_IAPI float naLengthV4f(const NAVec4f a);

// Computes the (positive 2-norm) distance between two vectors
NA_IAPI float naDistanceV1f(const NAVec1f a, const NAVec1f b);
NA_IAPI float naDistanceV2f(const NAVec2f a, const NAVec2f b);
NA_IAPI float naDistanceV3f(const NAVec3f a, const NAVec3f b);
NA_IAPI float naDistanceV4f(const NAVec4f a, const NAVec4f b);

// Mirrors vector d on a surface with normal n
NA_IAPI void naMirrorV1f(NAVec1f d, const NAVec1f n);
NA_IAPI void naMirrorV2f(NAVec2f d, const NAVec2f n);
NA_IAPI void naMirrorV3f(NAVec3f d, const NAVec3f n);
NA_IAPI void naMirrorV4f(NAVec4f d, const NAVec4f n);

// Scales the vector d to a length of 1. Returns the length of the vector
// before scaling it. If d was a zero-vector, it will be a zero-vector again
// and the returned length is precisely zero.
NA_IAPI float naNormalizeV1f(NAVec1f d);
NA_IAPI float naNormalizeV2f(NAVec2f d);
NA_IAPI float naNormalizeV3f(NAVec3f d);
NA_IAPI float naNormalizeV4f(NAVec4f d);

// Makes vector d orthogonal to a. Using Gram-Schmidt. If vector a is a
// zero-vector, d is untouched. If d was colinear to d or a zero-vector, it
// will become a zero-vector.
NA_IAPI void naOrthogonalizeV2f(NAVec2f d, const NAVec2f a);
NA_IAPI void naOrthogonalizeV3f(NAVec3f d, const NAVec3f a);
NA_IAPI void naOrthogonalizeV4f(NAVec4f d, const NAVec4f a);

// Assumes a to be normalized and makes d ortho-normal to a. If a was not
// normal or a zero-vector, the result is undefined. If d is colinear to a or
// a zero-vector, it will become a zero-vector.
NA_IAPI void naOrthonormalizeV2f(NAVec2f d, const NAVec2f a);
NA_IAPI void naOrthonormalizeV3f(NAVec3f d, const NAVec3f a);
NA_IAPI void naOrthonormalizeV4f(NAVec4f d, const NAVec4f a);

// Homogenous position functions. Sets d = {a0, a1, a2, 1} for example
NA_IAPI void naSetV2fHomPos(NAVec2f d, float a0);
NA_IAPI void naSetV3fHomPos(NAVec3f d, float a0, float a1);
NA_IAPI void naSetV4fHomPos(NAVec4f d, float a0, float a1, float a2);

// Homogenous direction functions. Sets d = {a0, a1, a2, 0} for example
NA_IAPI void naSetV2fHomDir(NAVec2f d, float a0);
NA_IAPI void naSetV3fHomDir(NAVec3f d, float a0, float a1);
NA_IAPI void naSetV4fHomDir(NAVec4f d, float a0, float a1, float a2);

// Angle functions. Assumes either x and y or r and theta to be stored in a
// two dimensional vector.
NA_IAPI float naAngleV2f(const float* xy);
NA_IAPI void  naPolarToCartesianf(float* xy, const float* rtheta);
NA_IAPI void  CMLCartesiantoPolarf(float* rtheta, const float* xy);




// ///////////////////////////////
// Matrix
// ///////////////////////////////

// Info: NALib stores the values of matrices in COLUMN-FIRST order!
// The indices of a matrix type therefore look as follows:
// (  m[0]   m[3]   m[6]  )
// (  m[1]   m[4]   m[7]  )
// (  m[2]   m[5]   m[8]  )

typedef float NAMat33f[9];

// Fills the matrix with diagonal 1. Identity matrix
NA_IAPI void naSetMat33Identityf(NAMat33f m);

// Copies the values of s to d;
NA_IAPI void naCpyMat33f( NAMat33f d, const NAMat33f s);

// Fills the matrix with the given values. Warning: COLUMN-FIRST order!
NA_IAPI void naSetMat33f          (NAMat33f m,
                                            float a0, float a1, float a2,
                                            float a3, float a4, float a5,
                                            float a6, float a7, float a8);

// Fills the matrix with the three column vectors given.
NA_IAPI void naSetMat33fWithAxis  (NAMat33f m,
                                    const NAVec3f v0,
                                    const NAVec3f v1,
                                    const NAVec3f v2);

// Fills the matrix as a diagonal matrix with the diagonal entries given.
NA_IAPI void naSetMat33fWithDiag(NAMat33f m, const NAVec3f diag);

// Scales the column vectors of matrix m with the entries of vector v
NA_IAPI void naScaleMat33f(NAMat33f m, const NAVec3f v);

// Multiplies matrix m with vector v and stores the result vector in d
NA_IAPI void naMulMat33fVec3f(NAVec3f d, const NAMat33f m, const NAVec3f v);


// Multiplies Matrix m with matrix a and stores the result matrix in d
NA_IAPI void naMulMat33fMat33f(NAMat33f d, const NAMat33f m, const NAMat33f a);

// Computes the inverse of m and stores it in d
NA_IAPI void naInvertMat33f(NAMat33f d, const NAMat33f m);



















#include "NAMathOperators.h"

NA_IAPI void naSetV1f(NAVec1f d, float a0){
  d[0] = a0;
}
NA_IAPI void naSetV2f(NAVec2f d, float a0, float a1){
  d[0] = a0; d[1] = a1;
}
NA_IAPI void naSetV3f(NAVec3f d, float a0, float a1, float a2){
  d[0] = a0; d[1] = a1; d[2] = a2;
}
NA_IAPI void naSetV4f(NAVec4f d, float a0, float a1, float a2, float a3){
  d[0] = a0; d[1] = a1; d[2] = a2; d[3] = a3;
}
NA_IAPI void naSetV1(NAVec1 d, double a0){
  d[0] = a0;
}
NA_IAPI void naSetV2(NAVec2 d, double a0, double a1){
  d[0] = a0; d[1] = a1;
}
NA_IAPI void naSetV3(NAVec3 d, double a0, double a1, double a2){
  d[0] = a0; d[1] = a1; d[2] = a2;
}
NA_IAPI void naSetV4(NAVec4 d, double a0, double a1, double a2, double a3){
  d[0] = a0; d[1] = a1; d[2] = a2; d[3] = a3;
}


NA_IAPI void naSetV1fRandom(NAVec1f d){
  naSetV1f(d, naUniformRandZEf());
}
NA_IAPI void naSetV2fRandom(NAVec2f d){
  naSetV2f(d, naUniformRandZEf(),
              naUniformRandZEf());
}
NA_IAPI void naSetV3fRandom(NAVec3f d){
  naSetV3f(d, naUniformRandZEf(),
              naUniformRandZEf(),
              naUniformRandZEf());
}
NA_IAPI void naSetV4fRandom(NAVec4f d){
  naSetV4f(d, naUniformRandZEf(),
              naUniformRandZEf(),
              naUniformRandZEf(),
              naUniformRandZEf());
}


NA_IAPI void naSetV1fRandomSpherical(NAVec1f d, NABool filled){
  if(filled){
    do{
      d[0] = naUniformRandZIf() * 2.f - 1.f;
    }while(d[0] == 0.f);
  }else{
    d[0] = (naUniformRandZEf() < .5f) ? -1.f : 1.f;
  }
}
NA_IAPI void naSetV2fRandomSpherical(NAVec2f d, NABool filled){
  float norm;
  do{
    d[0] = naUniformRandZIf() * 2.f - 1.f;
    d[1] = naUniformRandZIf() * 2.f - 1.f;
    norm = naDotV2f(d, d);
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is very rare.
  if(!filled){naDivV2f(d, naSqrtf(norm));}
}
NA_IAPI void naSetV3fRandomSpherical(NAVec3f d, NABool filled){
  float norm;
  do{
    d[0] = naUniformRandZIf() * 2.f - 1.f;
    d[1] = naUniformRandZIf() * 2.f - 1.f;
    d[2] = naUniformRandZIf() * 2.f - 1.f;
    norm = naDotV3f(d, d);
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is extremely! rare.
  if(!filled){naDivV3f(d, naSqrtf(norm));}
}
NA_IAPI void naSetV4fRandomSpherical(NAVec4f d, NABool filled){
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
  if(!filled){naDivV4f(d, naSqrtf(norm));}
}


NA_IAPI void naCpyV1f(NAVec1f d, const NAVec1f s){
  d[0] = s[0];
}
NA_IAPI void naCpyV2f(NAVec2f d, const NAVec2f s){
  d[0] = s[0]; d[1] = s[1];
}
NA_IAPI void naCpyV3f(NAVec3f d, const NAVec3f s){
  d[0] = s[0]; d[1] = s[1]; d[2] = s[2];
}
NA_IAPI void naCpyV4f(NAVec4f d, const NAVec4f s){
  d[0] = s[0]; d[1] = s[1]; d[2] = s[2]; d[3] = s[3];
}


NA_IAPI void naNegV1f(NAVec1f d){
  d[0] = -d[0];
}
NA_IAPI void naNegV2f(NAVec2f d){
  d[0] = -d[0]; d[1] = -d[1];
}
NA_IAPI void naNegV3f(NAVec3f d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2];
}
NA_IAPI void naNegV4f(NAVec4f d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2]; d[3] = -d[3];
}


NA_IAPI void naAddV1f(NAVec1f d, const NAVec1f a){
  d[0] +=a [0];
}
NA_IAPI void naAddV2f(NAVec2f d, const NAVec2f a){
  d[0] += a[0]; d[1] += a[1];
}
NA_IAPI void naAddV3f(NAVec3f d, const NAVec3f a){
  d[0] += a[0]; d[1] += a[1]; d[2] += a[2];
}
NA_IAPI void naAddV4f(NAVec4f d, const NAVec4f a){
  d[0] += a[0]; d[1] += a[1]; d[2] += a[2]; d[3] += a[3];
}


NA_IAPI void naSubV1f(NAVec1f d, const NAVec1f a){
  d[0] -= a[0];
}
NA_IAPI void naSubV2f(NAVec2f d, const NAVec2f a){
  d[0] -= a[0]; d[1] -= a[1];
}
NA_IAPI void naSubV3f(NAVec3f d, const NAVec3f a){
  d[0] -= a[0]; d[1] -= a[1]; d[2] -= a[2];
}
NA_IAPI void naSubV4f(NAVec4f d, const NAVec4f a){
  d[0] -= a[0]; d[1] -= a[1]; d[2] -= a[2]; d[3] -= a[3];
}


NA_IAPI void naMulV1f(NAVec1f d, float s){
  d[0] *= s;
}
NA_IAPI void naMulV2f(NAVec2f d, float s){
  d[0] *= s; d[1] *= s;
}
NA_IAPI void naMulV3f(NAVec3f d, float s){
  d[0] *= s; d[1] *= s; d[2] *= s;
}
NA_IAPI void naMulV4f(NAVec4f d, float s){
  d[0] *= s; d[1] *= s; d[2] *= s; d[3] *= s;
}


NA_IAPI void naDivV1f(NAVec1f d, float s){
  naMulV1f(d, naInvf(s));
}
NA_IAPI void naDivV2f(NAVec2f d, float s){
  naMulV2f(d, naInvf(s));
}
NA_IAPI void naDivV3f(NAVec3f d, float s){
  naMulV3f(d, naInvf(s));
}
NA_IAPI void naDivV4f(NAVec4f d, float s){
  naMulV4f(d, naInvf(s));
}


NA_IAPI void naDivV1fComponentwise(NAVec1f d, const NAVec1f a){
  d[0] *= naInvf(a[0]);
}
NA_IAPI void naDivV2fComponentwise(NAVec2f d, const NAVec2f a){
  d[0] *= naInvf(a[0]);
  d[1] *= naInvf(a[1]);
}
NA_IAPI void naDivV3fComponentwise(NAVec3f d, const NAVec3f a){
  d[0] *= naInvf(a[0]);
  d[1] *= naInvf(a[1]);
  d[2] *= naInvf(a[2]);
}
NA_IAPI void naDivV4fComponentwise(NAVec4f d, const NAVec4f a){
  d[0] *= naInvf(a[0]);
  d[1] *= naInvf(a[1]);
  d[2] *= naInvf(a[2]);
  d[3] *= naInvf(a[3]);
}


NA_IAPI void naMadV1f(NAVec1f d, const NAVec1f b, float s){
  d[0] += b[0] * s;
}
NA_IAPI void naMadV2f(NAVec2f d, const NAVec1f b, float s){
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IAPI void naMadV3f(NAVec3f d, const NAVec1f b, float s){
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IAPI void naMadV4f(NAVec4f d, const NAVec1f b, float s){
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}


NA_IAPI NABool naEqualV1f(const NAVec1f a, const NAVec1f b){
  return ((a[0] == b[0]));
}
NA_IAPI NABool naEqualV2f(const NAVec2f a, const NAVec2f b){
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IAPI NABool naEqualV3f(const NAVec3f a, const NAVec3f b){
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IAPI NABool naEqualV4f(const NAVec4f a, const NAVec4f b){
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}


NA_IAPI float naDotV1f(const NAVec1f a, const NAVec1f b){
  return a[0] * b[0];
}
NA_IAPI float naDotV2f(const NAVec2f a, const NAVec2f b){
  return a[0] * b[0] + a[1] * b[1];
}
NA_IAPI float naDotV3f(const NAVec3f a, const NAVec3f b){
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IAPI float naDotV4f(const NAVec4f a, const NAVec4f b){
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}


NA_IAPI void naCrossV3f(NAVec3f d, const NAVec3f a, const NAVec3f b){
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
}


NA_IAPI float naLengthV1f(const NAVec1f a){
  return naAbsf(a[0]);
}
NA_IAPI float naLengthV2f(const NAVec2f a){
  return naSqrtf(naDotV2f(a, a));
}
NA_IAPI float naLengthV3f(const NAVec3f a){
  return naSqrtf(naDotV3f(a, a));
}
NA_IAPI float naLengthV4f(const NAVec4f a){
  return naSqrtf(naDotV4f(a, a));
}


NA_IAPI float naDistanceV1f(const NAVec1f a, const NAVec1f b){
  return naAbsf(a[0] - b[0]);
}
NA_IAPI float naDistanceV2f(const NAVec2f a, const NAVec2f b){
  NAVec2f vec;
  naCpyV2f(vec, a);
  naSubV2f(vec, b);
  return naLengthV2f(vec);
}
NA_IAPI float naDistanceV3f(const NAVec3f a, const NAVec3f b){
  NAVec3f vec;
  naCpyV3f(vec, a);
  naSubV3f(vec, b);
  return naLengthV3f(vec);
}
NA_IAPI float naDistanceV4f(const NAVec4f a, const NAVec4f b){
  NAVec4f vec;
  naCpyV4f(vec, a);
  naSubV4f(vec, b);
  return naLengthV4f(vec);
}


NA_IAPI void naMirrorV1f(NAVec1f d, const NAVec1f n){
  naMadV1f(d, n, -2.f * naDotV1f(d, n));
}
NA_IAPI void naMirrorV2f(NAVec2f d, const NAVec2f n){
  naMadV2f(d, n, -2.f * naDotV2f(d, n));
}
NA_IAPI void naMirrorV3f(NAVec3f d, const NAVec3f n){
  naMadV3f(d, n, -2.f * naDotV3f(d, n));
}
NA_IAPI void naMirrorV4f(NAVec4f d, const NAVec4f n){
  naMadV4f(d, n, -2.f * naDotV4f(d, n));
}


NA_IAPI float naNormalizeV1f(NAVec1f d){
  float length = naLengthV1f(d);
  if(length != 0.f){
    naDivV1f(d, length);
  }
  return length;
}
NA_IAPI float naNormalizeV2f(NAVec2f d){
  float length = naLengthV2f(d);
  if(length != 0.f){
    naDivV2f(d, length);
  }
  return length;
}
NA_IAPI float naNormalizeV3f(NAVec3f d){
  float length = naLengthV3f(d);
  if(length != 0.f){
    naDivV3f(d, length);
  }
  return length;
}
NA_IAPI float naNormalizeV4f(NAVec4f d){
  float length = naLengthV4f(d);
  if(length != 0.f){
    naDivV4f(d, length);
  }
  return length;
}


NA_IAPI void naOrthogonalizeV2f(NAVec2f d, const NAVec2f a){
  float anorm = naDotV2f(a, a);
  if(anorm == 0.f){return;}
  naMadV2f(d, a, - naDotV2f(a, d) * naInvf(anorm));
}
NA_IAPI void naOrthogonalizeV3f(NAVec3f d, const NAVec3f a){
  float anorm = naDotV3f(a, a);
  if(anorm == 0.f){return;}
  naMadV3f(d, a, - naDotV3f(a, d) * naInvf(anorm));
}
NA_IAPI void naOrthogonalizeV4f(NAVec4f d, const NAVec4f a){
  float anorm = naDotV4f(a, a);
  if(anorm == 0.f){return;}
  naMadV4f(d, a, - naDotV4f(a, d) * naInvf(anorm));
}


NA_IAPI void naOrthonormalizeV2f(NAVec2f d, const NAVec2f a){
  naMadV2f(d, a, - naDotV2f(a, d));
  naNormalizeV2f(d);
}
NA_IAPI void naOrthonormalizeV3f(NAVec3f d, const NAVec3f a){
  naMadV3f(d, a, - naDotV3f(a, d));
  naNormalizeV3f(d);
}
NA_IAPI void naOrthonormalizeV4f(NAVec4f d, const NAVec4f a){
  naMadV4f(d, a, - naDotV4f(a, d));
  naNormalizeV4f(d);
}


NA_IAPI void naSetV2fHomPos(NAVec2f d, float a0){
  d[0] = a0; d[1] = 1.f;
}
NA_IAPI void naSetV3fHomPos(NAVec3f d, float a0, float a1){
  d[0] = a0; d[1] = a1; d[2] = 1.f;
}
NA_IAPI void naSetV4fHomPos(NAVec4f d, float a0, float a1, float a2){
  d[0] = a0; d[1] = a1; d[2] = a2; d[3] = 1.f;
}


NA_IAPI void naSetV2fHomDir(NAVec2f d, float a0){
  d[0] = a0; d[1] = 0.f;
}
NA_IAPI void naSetV3fHomDir(NAVec3f d, float a0, float a1){
  d[0] = a0; d[1] = a1; d[2] = 0.f;
}
NA_IAPI void naSetV4fHomDir(NAVec4f d, float a0, float a1, float a2){
  d[0] = a0; d[1] = a1; d[2] = a2; d[3] = 0.f;
}


NA_IAPI float naAngleV2f(const float* xy){
  return naAtan2f(xy[1], xy[0]);
}
NA_IAPI void naPolarToCartesianf(float* xy, const float* rtheta){
  float r = rtheta[0];
  xy[0] = r * naCosf(rtheta[1]);
  xy[1] = r * naSinf(rtheta[1]);
}
NA_IAPI void naCartesianToPolarf(float* rtheta, const float* xy){
  float length = naLengthV2f(xy);
  rtheta[1] = (length == 0.) ? 0.f : naAngleV2f(xy);
  rtheta[0] = length;
}






// ////////////////////////////////////////////
// 3x3 matrix algebra, stored in COLUMN-FIRST order
// ////////////////////////////////////////////


NA_IAPI void naSetMat33Identityf(NAMat33f m){
  naSetMat33f(m,
              1.f, 0.f, 0.f,
              0.f, 1.f, 0.f,
              0.f, 0.f, 1.f);
}


// Warning: NALib expects the values in COLUMN-FIRST order!
NA_IAPI void naSetMat33f(NAMat33f m,
                                  float a0, float a1, float a2,
                                  float a3, float a4, float a5,
                                  float a6, float a7, float a8){
  m[0] = a0; m[3] = a3; m[6] = a6;
  m[1] = a1; m[4] = a4; m[7] = a7;
  m[2] = a2; m[5] = a5; m[8] = a8;
}


NA_IAPI void naSetMat33fWithAxis(NAMat33f m,
                                  const NAVec3f v0,
                                  const NAVec3f v1,
                                  const NAVec3f v2){
  naSetMat33f(m,
              v0[0], v0[1], v0[2],
              v1[0], v1[1], v1[2],
              v2[0], v2[1], v2[2]);
}


NA_IAPI void naSetMat33fWithDiag(NAMat33f m,
                                      const NAVec3f diag){
  naSetMat33f(m,
              diag[0], 0.f    , 0.f,
              0.f    , diag[1], 0.f,
              0.f    , 0.f    , diag[2]);
}


NA_IAPI void naCpyMat33f( NAMat33f d,
                          const NAMat33f s){
  d[0] = s[0]; d[3] = s[3]; d[6] = s[6];
  d[1] = s[1]; d[4] = s[4]; d[7] = s[7];
  d[2] = s[2]; d[5] = s[5]; d[8] = s[8];
}


NA_IAPI void naMulMat33fVec3f(NAVec3f d,
                            const NAMat33f m,
                             const NAVec3f v){
  d[0] = m[0] * v[0] + m[3] * v[1] + m[6] * v[2];
  d[1] = m[1] * v[0] + m[4] * v[1] + m[7] * v[2];
  d[2] = m[2] * v[0] + m[5] * v[1] + m[8] * v[2];
}


NA_IAPI void naScaleMat33f( NAMat33f m,
                                const NAVec3f v){
  m[0] *= v[0]; m[3] *= v[1]; m[6] *= v[2];
  m[1] *= v[0]; m[4] *= v[1]; m[7] *= v[2];
  m[2] *= v[0]; m[5] *= v[1]; m[8] *= v[2];
}


NA_IAPI void naMulMat33fMat33f(NAMat33f d,
                              const NAMat33f m,
                              const NAMat33f a){
  naSetMat33f(d,  m[0] * a[0] + m[3] * a[1] + m[6] * a[2],
                  m[1] * a[0] + m[4] * a[1] + m[7] * a[2],
                  m[2] * a[0] + m[5] * a[1] + m[8] * a[2],
                  m[0] * a[3] + m[3] * a[4] + m[6] * a[5],
                  m[1] * a[3] + m[4] * a[4] + m[7] * a[5],
                  m[2] * a[3] + m[5] * a[4] + m[8] * a[5],
                  m[0] * a[6] + m[3] * a[7] + m[6] * a[8],
                  m[1] * a[6] + m[4] * a[7] + m[7] * a[8],
                  m[2] * a[6] + m[5] * a[7] + m[8] * a[8]);
}


NA_IAPI void naInvertMat33f(NAMat33f d,
                            const NAMat33f m){
  float d0 = m[4] * m[8] - m[5] * m[7];
  float d1 = m[2] * m[7] - m[1] * m[8];
  float d2 = m[1] * m[5] - m[2] * m[4];
  float discriminant = m[0]*d0 + m[3]*d1 + m[6]*d2;
  float divisor = naInvf(discriminant);
  naSetMat33f(d,  d0 * divisor,
                  d1 * divisor,
                  d2 * divisor,
                  (m[5]*m[6] - m[3]*m[8]) * divisor,
                  (m[0]*m[8] - m[2]*m[6]) * divisor,
                  (m[2]*m[3] - m[0]*m[5]) * divisor,
                  (m[3]*m[7] - m[4]*m[6]) * divisor,
                  (m[1]*m[6] - m[0]*m[7]) * divisor,
                  (m[0]*m[4] - m[1]*m[3]) * divisor);
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
