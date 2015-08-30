
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#ifndef NA_VECTOR_ALGEBRA_INCLUDED
#define NA_VECTOR_ALGEBRA_INCLUDED
#ifdef __cplusplus 
  extern "C"{
#endif


#include "NASystem.h"

// In this file, you find basic vector algebra functions for both
// Vector and Matrix.
//
// Note that in NALib, vectors are plain C-arrays. The programmer has to make
// sure, the given arrays have indeed the required number of entries. The
// author decided to do this very plainly as this hardly ever becomes a
// problem, let alone being very easy to debug if there would be any error.
// During the last 10 years, there were barely any of 'em.
//
// Nontheless, you can use the following typedefs to easily create vectors and
// matrices of prefedined type and size:

typedef double NAVec1d[1];
typedef double NAVec2d[2];
typedef double NAVec3d[3];
typedef double NAVec4d[4];
typedef float  NAVec1f[1];
typedef float  NAVec2f[2];
typedef float  NAVec3f[3];
typedef float  NAVec4f[4];
typedef NAInt  NAVec1i[1];
typedef NAInt  NAVec2i[2];
typedef NAInt  NAVec3i[3];
typedef NAInt  NAVec4i[4];

typedef double NAMat22d[ 4];
typedef double NAMat33d[ 9];
typedef double NAMat44d[16];
typedef float  NAMat22f[ 4];
typedef float  NAMat33f[ 9];
typedef float  NAMat44f[16];

// In vector algebra, many combinations of datatypes, vector and matrix sizes
// and special cases exist. In NALib, the API uses the following naming scheme:
//
// naDoStructSizeTypeSomethingES
//
// na           The prefix always available in NALib functions.
// Do           A verb denoting what will be done. For example Fill or Add.
// Struct       Either a V for vector or M for matrix.
// Size         The number of elements of the struct. For example 3 for a
//              3D-vector or 44 for a 4x4 matrix.
// Type         The type of the elements. Following are the possible entries:
//              d    double
//              f    float
//              i    NAInt
// Something    Anything which explains the functionality more detailed.
// E and/or S   Some functions are available in E or S variants. They are
//              explained in the following text.
//
// In NALib, usually, vector algebra functions expect a destination buffer and
// additional input parameters. The result of a mathematical operation will be
// stored in the destination buffer.
//
// The compiler can highly optimize such code when parameters are declared as
// NA_RESTRICT which means the compiler will assume NO overlapping of the
// pointers. Unfortunately, often times when using vector algebra, you use
// the same buffers over and over again resulting in NALib-warnings that the
// pointers in fact DO overlap.
//
// Therefore, there exist functions with the ending E which can be read as
// "pointers may be EQUAL". There, the compiler does not use restrict pointers
// to do optimizations and hence works fine even for pointers which are equal.
// When pointers overlap in a misaligned way though, they will produce quite
// unpredictable results. NALib will still check for such misalignments.
//
// Often times, with a little more programming effort, you can provide true
// restrict pointers or even use the S variant:
//
// The solution with and without E is not the best there can be, as often
// times in vector algebra, the destination vector or matrix is also part of
// the input parameters and hence they, by definition, overlap. It would be a
// pity if the compiler would not be capable of optimizing such simple code.
// That is why some functions additionally have an S variant.
//
// The S can be read as "working on the SAME buffer". Meaning that the first
// input parameter is also the destination buffer. There, the implementation
// has been arranged such that this particular buffer will be accessed in the
// correct order and only the remaining input parameters must be restrict. Very
// often, this is the best solution but it only works, if the remaining input
// parameters (if any) are restrict.
//
// Therefore, there also exists the combination of the two variants: SE.
// SE-variants expect the first parameter to be the destination buffer as well
// as the first input parameter. Additionally, any further input parameter
// can be equal to the first parameter without warning. Misalignments will
// again still be warned.
//
// Here is an example of how to interprete this:
//
// naAddV3d  (d, a, b)    d = a + b     a and b may not be overlapped by d
// naAddV3dE (d, a, b)    d = a + b     a and b can be equal to d
// naAddV3dS (d, b)       d += b        only b needs to be non-overlapped by d
// naAddV3dSE(d, b)       d += b        b can be equal to d
//
// Note again, that the E variants really mean "Equal". If some pointers
// overlap in a misaligned way, NALib will still emit warnings!
//
// When NDEBUG is defined, no checks will be executed an no warnings will be
// emitted at all.
//
// Implementation note:
// The check if two arrays are misaligned is done using pointer arithmetic.
// It is so far unknown how these checks work out when the pointers are not
// aligned to the size of their element type like float or double. But most
// likely, such misalignments will show very quickly elsewhere.
//
// Until this day, misalignments have never been a real issue as they usually
// result in immediately visible errors. Nontheless, it is nice to know that
// NALib can detect such errors.


// Fills vector d with the given values
NA_IAPI void naFillV1d(double* d, double a0);
NA_IAPI void naFillV2d(double* d, double a0, double a1);
NA_IAPI void naFillV3d(double* d, double a0, double a1, double a2);
NA_IAPI void naFillV4d(double* d, double a0, double a1, double a2, double a3);
NA_IAPI void naFillV1f(float*  d, float  a0);
NA_IAPI void naFillV2f(float*  d, float  a0, float  a1);
NA_IAPI void naFillV3f(float*  d, float  a0, float  a1, float  a2);
NA_IAPI void naFillV4f(float*  d, float  a0, float  a1, float  a2, float  a3);
NA_IAPI void naFillV1i(NAInt*  d, NAInt  a0);
NA_IAPI void naFillV2i(NAInt*  d, NAInt  a0, NAInt  a1);
NA_IAPI void naFillV3i(NAInt*  d, NAInt  a0, NAInt  a1, NAInt  a2);
NA_IAPI void naFillV4i(NAInt*  d, NAInt  a0, NAInt  a1, NAInt  a2, NAInt  a3);

// Sets all components to Null
NA_IAPI void naNullV1d(double* d);
NA_IAPI void naNullV2d(double* d);
NA_IAPI void naNullV3d(double* d);
NA_IAPI void naNullV4d(double* d);
NA_IAPI void naNullV1f(float*  d);
NA_IAPI void naNullV2f(float*  d);
NA_IAPI void naNullV3f(float*  d);
NA_IAPI void naNullV4f(float*  d);
NA_IAPI void naNullV1i(NAInt*  d);
NA_IAPI void naNullV2i(NAInt*  d);
NA_IAPI void naNullV3i(NAInt*  d);
NA_IAPI void naNullV4i(NAInt*  d);

// Fills vector d with random values in [0, 1)
NA_IAPI void naFillV1dRandom(double* d);
NA_IAPI void naFillV2dRandom(double* d);
NA_IAPI void naFillV3dRandom(double* d);
NA_IAPI void naFillV4dRandom(double* d);
NA_IAPI void naFillV1fRandom(float*  d);
NA_IAPI void naFillV2fRandom(float*  d);
NA_IAPI void naFillV3fRandom(float*  d);
NA_IAPI void naFillV4fRandom(float*  d);

// Fills vector d with random vectors in a unit sphere. If filled is NA_TRUE,
// the length of d will be smallerequal than 1. If filled is NA_FALSE, the
// length of d will be precisely 1.
NA_IAPI void naFillV1dRandomSpherical(double* d, NABool filled);
NA_IAPI void naFillV2dRandomSpherical(double* d, NABool filled);
NA_IAPI void naFillV3dRandomSpherical(double* d, NABool filled);
NA_IAPI void naFillV4dRandomSpherical(double* d, NABool filled);
NA_IAPI void naFillV1fRandomSpherical(float*  d, NABool filled);
NA_IAPI void naFillV2fRandomSpherical(float*  d, NABool filled);
NA_IAPI void naFillV3fRandomSpherical(float*  d, NABool filled);
NA_IAPI void naFillV4fRandomSpherical(float*  d, NABool filled);

// Copies vector a to d
// E-variant allow the pointers to be equal without warning.
NA_IAPI void naCopyV1d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naCopyV2d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naCopyV3d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naCopyV4d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naCopyV1f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naCopyV2f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naCopyV3f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naCopyV4f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naCopyV1dE(double* d, const double* a);
NA_IAPI void naCopyV2dE(double* d, const double* a);
NA_IAPI void naCopyV3dE(double* d, const double* a);
NA_IAPI void naCopyV4dE(double* d, const double* a);
NA_IAPI void naCopyV1fE(float*  d, const float*  a);
NA_IAPI void naCopyV2fE(float*  d, const float*  a);
NA_IAPI void naCopyV3fE(float*  d, const float*  a);
NA_IAPI void naCopyV4fE(float*  d, const float*  a);

// Negates the Vector. d = -a
// E-variant allow the pointers to be equal without warning.
// S-variant computes d = -d
NA_IAPI void naNegV1d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naNegV2d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naNegV3d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naNegV4d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI void naNegV1f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naNegV2f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naNegV3f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naNegV4f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI void naNegV1dE(double* d, const double* a);
NA_IAPI void naNegV2dE(double* d, const double* a);
NA_IAPI void naNegV3dE(double* d, const double* a);
NA_IAPI void naNegV4dE(double* d, const double* a);
NA_IAPI void naNegV1fE(float*  d, const float*  a);
NA_IAPI void naNegV2fE(float*  d, const float*  a);
NA_IAPI void naNegV3fE(float*  d, const float*  a);
NA_IAPI void naNegV4fE(float*  d, const float*  a);
NA_IAPI void naNegV1dS(double* d);
NA_IAPI void naNegV2dS(double* d);
NA_IAPI void naNegV3dS(double* d);
NA_IAPI void naNegV4dS(double* d);
NA_IAPI void naNegV1fS(float*  d);
NA_IAPI void naNegV2fS(float*  d);
NA_IAPI void naNegV3fS(float*  d);
NA_IAPI void naNegV4fS(float*  d);

// Adds two vectors. d = a + b
// E-variant allow the pointers to be equal without warning.
// S-variant computes d += b
// SE-variant will not warn about b being equal to d.
NA_IAPI void naAddV1d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naAddV2d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naAddV3d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naAddV4d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naAddV1f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naAddV2f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naAddV3f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naAddV4f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naAddV1dE (double* d, const double* a, const double* b);
NA_IAPI void naAddV2dE (double* d, const double* a, const double* b);
NA_IAPI void naAddV3dE (double* d, const double* a, const double* b);
NA_IAPI void naAddV4dE (double* d, const double* a, const double* b);
NA_IAPI void naAddV1fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naAddV2fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naAddV3fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naAddV4fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naAddV1dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naAddV2dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naAddV3dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naAddV4dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naAddV1fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naAddV2fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naAddV3fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naAddV4fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naAddV1dSE(double* d, const double* b);
NA_IAPI void naAddV2dSE(double* d, const double* b);
NA_IAPI void naAddV3dSE(double* d, const double* b);
NA_IAPI void naAddV4dSE(double* d, const double* b);
NA_IAPI void naAddV1fSE(float*  d, const float*  b);
NA_IAPI void naAddV2fSE(float*  d, const float*  b);
NA_IAPI void naAddV3fSE(float*  d, const float*  b);
NA_IAPI void naAddV4fSE(float*  d, const float*  b);

// Subtracts two vectors. d = a - b
// E-variant allow the pointers to be equal without warning.
// S-variant computes d -= b
// SE-variant will not warn about b being equal to d.
NA_IAPI void naSubV1d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naSubV2d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naSubV3d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naSubV4d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naSubV1f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naSubV2f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naSubV3f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naSubV4f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naSubV1dE (double* d, const double* a, const double* b);
NA_IAPI void naSubV2dE (double* d, const double* a, const double* b);
NA_IAPI void naSubV3dE (double* d, const double* a, const double* b);
NA_IAPI void naSubV4dE (double* d, const double* a, const double* b);
NA_IAPI void naSubV1fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naSubV2fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naSubV3fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naSubV4fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naSubV1dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naSubV2dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naSubV3dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naSubV4dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naSubV1fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naSubV2fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naSubV3fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naSubV4fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naSubV1dSE(double* d, const double* b);
NA_IAPI void naSubV2dSE(double* d, const double* b);
NA_IAPI void naSubV3dSE(double* d, const double* b);
NA_IAPI void naSubV4dSE(double* d, const double* b);
NA_IAPI void naSubV1fSE(float*  d, const float*  b);
NA_IAPI void naSubV2fSE(float*  d, const float*  b);
NA_IAPI void naSubV3fSE(float*  d, const float*  b);
NA_IAPI void naSubV4fSE(float*  d, const float*  b);

// Multiplies vector with scalar. d = a * s
// E-variant allow the pointers to be equal without warning.
// S-variant computes d *= s
NA_IAPI void naMulV1d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naMulV2d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naMulV3d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naMulV4d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naMulV1f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naMulV2f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naMulV3f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naMulV4f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naMulV1dE(double* d, const double* a, double s);
NA_IAPI void naMulV2dE(double* d, const double* a, double s);
NA_IAPI void naMulV3dE(double* d, const double* a, double s);
NA_IAPI void naMulV4dE(double* d, const double* a, double s);
NA_IAPI void naMulV1fE(float*  d, const float*  a, float  s);
NA_IAPI void naMulV2fE(float*  d, const float*  a, float  s);
NA_IAPI void naMulV3fE(float*  d, const float*  a, float  s);
NA_IAPI void naMulV4fE(float*  d, const float*  a, float  s);
NA_IAPI void naMulV1dS(double* d, double s);
NA_IAPI void naMulV2dS(double* d, double s);
NA_IAPI void naMulV3dS(double* d, double s);
NA_IAPI void naMulV4dS(double* d, double s);
NA_IAPI void naMulV1fS(float*  d, float  s);
NA_IAPI void naMulV2fS(float*  d, float  s);
NA_IAPI void naMulV3fS(float*  d, float  s);
NA_IAPI void naMulV4fS(float*  d, float  s);

// Divides vector with scalar. d = a / s
// E-variant allow the pointers to be equal without warning.
// S-variant computes d /= s
NA_IAPI void naDivV1d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naDivV2d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naDivV3d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naDivV4d (double* NA_RESTRICT d, const double*  NA_RESTRICT a, double s);
NA_IAPI void naDivV1f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naDivV2f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naDivV3f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naDivV4f (float*  NA_RESTRICT d, const float*   NA_RESTRICT a, float  s);
NA_IAPI void naDivV1dE(double* d, const double* a, double s);
NA_IAPI void naDivV2dE(double* d, const double* a, double s);
NA_IAPI void naDivV3dE(double* d, const double* a, double s);
NA_IAPI void naDivV4dE(double* d, const double* a, double s);
NA_IAPI void naDivV1fE(float*  d, const float*  a, float  s);
NA_IAPI void naDivV2fE(float*  d, const float*  a, float  s);
NA_IAPI void naDivV3fE(float*  d, const float*  a, float  s);
NA_IAPI void naDivV4fE(float*  d, const float*  a, float  s);
NA_IAPI void naDivV1dS(double* d, double s);
NA_IAPI void naDivV2dS(double* d, double s);
NA_IAPI void naDivV3dS(double* d, double s);
NA_IAPI void naDivV4dS(double* d, double s);
NA_IAPI void naDivV1fS(float*  d, float  s);
NA_IAPI void naDivV2fS(float*  d, float  s);
NA_IAPI void naDivV3fS(float*  d, float  s);
NA_IAPI void naDivV4fS(float*  d, float  s);


// Where is multiply componentwise? It is called the dot-product. See there.


// Divides two vectors componentwise. d = a ./ b
// E-variant allow the pointers to be equal without warning.
// S-variant computes d ./= b
// SE-variant will not warn about pointers being equal.
NA_IAPI void naDivV1dV1d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivV2dV2d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivV3dV3d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivV4dV4d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivV1fV1f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivV2fV2f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivV3fV3f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivV4fV4f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivV1dV1dE (double* d, const double* a, const double* b);
NA_IAPI void naDivV2dV2dE (double* d, const double* a, const double* b);
NA_IAPI void naDivV3dV3dE (double* d, const double* a, const double* b);
NA_IAPI void naDivV4dV4dE (double* d, const double* a, const double* b);
NA_IAPI void naDivV1fV1fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivV2fV2fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivV3fV3fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivV4fV4fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivV1dV1dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivV2dV2dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivV3dV3dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivV4dV4dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivV1fV1fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivV2fV2fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivV3fV3fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivV4fV4fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivV1dV1dSE(double* d, const double* b);
NA_IAPI void naDivV2dV2dSE(double* d, const double* b);
NA_IAPI void naDivV3dV3dSE(double* d, const double* b);
NA_IAPI void naDivV4dV4dSE(double* d, const double* b);
NA_IAPI void naDivV1fV1fSE(float*  d, const float*  b);
NA_IAPI void naDivV2fV2fSE(float*  d, const float*  b);
NA_IAPI void naDivV3fV3fSE(float*  d, const float*  b);
NA_IAPI void naDivV4fV4fSE(float*  d, const float*  b);

// Multiply and add: Computes d = a + b * s
// E-variant allow the pointers to be equal without warning.
// S-variant computes d += b * s
// SE-variant will not warn about pointers being equal.
NA_IAPI void naMadV1d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV2d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV3d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV4d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV1f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV2f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV3f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV4f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV1dE (double* d, const double* a, const double* b, double s);
NA_IAPI void naMadV2dE (double* d, const double* a, const double* b, double s);
NA_IAPI void naMadV3dE (double* d, const double* a, const double* b, double s);
NA_IAPI void naMadV4dE (double* d, const double* a, const double* b, double s);
NA_IAPI void naMadV1fE (float*  d, const float*  a, const float*  b, float  s);
NA_IAPI void naMadV2fE (float*  d, const float*  a, const float*  b, float  s);
NA_IAPI void naMadV3fE (float*  d, const float*  a, const float*  b, float  s);
NA_IAPI void naMadV4fE (float*  d, const float*  a, const float*  b, float  s);
NA_IAPI void naMadV1dS (double* NA_RESTRICT d, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV2dS (double* NA_RESTRICT d, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV3dS (double* NA_RESTRICT d, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV4dS (double* NA_RESTRICT d, const double* NA_RESTRICT b, double s);
NA_IAPI void naMadV1fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV2fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV3fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV4fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b, float  s);
NA_IAPI void naMadV1dES(double* d, const double* b, double s);
NA_IAPI void naMadV2dES(double* d, const double* b, double s);
NA_IAPI void naMadV3dES(double* d, const double* b, double s);
NA_IAPI void naMadV4dES(double* d, const double* b, double s);
NA_IAPI void naMadV1fES(float*  d, const float*  b, float  s);
NA_IAPI void naMadV2fES(float*  d, const float*  b, float  s);
NA_IAPI void naMadV3fES(float*  d, const float*  b, float  s);
NA_IAPI void naMadV4fES(float*  d, const float*  b, float  s);

// Where is multiply and subtract? Just negate the scalar of Mad.

// Returns true if the two vectors are equal
// E-variant allow the pointers to be equal without warning.
NA_IAPI NABool naEqualV1d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualV2d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualV3d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualV4d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualV1f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualV2f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualV3f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualV4f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualV1dE(const double* a, const double* b);
NA_IAPI NABool naEqualV2dE(const double* a, const double* b);
NA_IAPI NABool naEqualV3dE(const double* a, const double* b);
NA_IAPI NABool naEqualV4dE(const double* a, const double* b);
NA_IAPI NABool naEqualV1fE(const float*  a, const float*  b);
NA_IAPI NABool naEqualV2fE(const float*  a, const float*  b);
NA_IAPI NABool naEqualV3fE(const float*  a, const float*  b);
NA_IAPI NABool naEqualV4fE(const float*  a, const float*  b);

// Computes the dot product (scalar product, inner product) of the two vectors.
// E-variant allow the pointers to be equal without warning.
// S-variant computes the dot product of a vector with itself.
NA_IAPI double naDotV1d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI double naDotV2d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI double naDotV3d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI double naDotV4d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI float  naDotV1f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI float  naDotV2f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI float  naDotV3f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI float  naDotV4f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI double naDotV1dE(const double* a, const double* b);
NA_IAPI double naDotV2dE(const double* a, const double* b);
NA_IAPI double naDotV3dE(const double* a, const double* b);
NA_IAPI double naDotV4dE(const double* a, const double* b);
NA_IAPI float  naDotV1fE(const float*  a, const float*  b);
NA_IAPI float  naDotV2fE(const float*  a, const float*  b);
NA_IAPI float  naDotV3fE(const float*  a, const float*  b);
NA_IAPI float  naDotV4fE(const float*  a, const float*  b);
NA_IAPI double naDotV1dS(const double* v);
NA_IAPI double naDotV2dS(const double* v);
NA_IAPI double naDotV3dS(const double* v);
NA_IAPI double naDotV4dS(const double* v);
NA_IAPI float  naDotV1fS(const float*  v);
NA_IAPI float  naDotV2fS(const float*  v);
NA_IAPI float  naDotV3fS(const float*  v);
NA_IAPI float  naDotV4fS(const float*  v);

// Cross product. Only defined for V3. d = a X b
// There do not exist E or S variants as the author does not see a sense in
// d being used as input. The result of the cross product should semantically
// not be confused with any of the input parameters.
NA_IAPI void naCrossV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naCrossV3f(float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);

// Computes the length (2-Norm) of the given vector
NA_IAPI double naLengthV1d(const double* v);
NA_IAPI double naLengthV2d(const double* v);
NA_IAPI double naLengthV3d(const double* v);
NA_IAPI double naLengthV4d(const double* v);
NA_IAPI float  naLengthV1f(const float*  v);
NA_IAPI float  naLengthV2f(const float*  v);
NA_IAPI float  naLengthV3f(const float*  v);
NA_IAPI float  naLengthV4f(const float*  v);

// Computes the (positive 2-norm) distance between two vectors
// When both parameters are const, overlappings are not an issue. Why is there
// still an E-variant? As a convenience for the programmer which gets informed,
// when two pointers are equal. The E-variant will not give that warning.
NA_IAPI double naDistanceV1d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI double naDistanceV2d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI double naDistanceV3d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI double naDistanceV4d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI float  naDistanceV1f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI float  naDistanceV2f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI float  naDistanceV3f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI float  naDistanceV4f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI double naDistanceV1dE(const double* a, const double* b);
NA_IAPI double naDistanceV2dE(const double* a, const double* b);
NA_IAPI double naDistanceV3dE(const double* a, const double* b);
NA_IAPI double naDistanceV4dE(const double* a, const double* b);
NA_IAPI float  naDistanceV1fE(const float*  a, const float*  b);
NA_IAPI float  naDistanceV2fE(const float*  a, const float*  b);
NA_IAPI float  naDistanceV3fE(const float*  a, const float*  b);
NA_IAPI float  naDistanceV4fE(const float*  a, const float*  b);

// Mirrors vector a on surface normal n and stores the result in d
// n should not be equal or misaligned to d.
// Implementation warns when n is not normalized.
// E-variant allow the a to be equal to d without warning. But not n.
// S-variant mirrors the vector d and stores it in itself.
NA_IAPI void naMirrorV1d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV2d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV3d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV4d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV1f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT n);
NA_IAPI void naMirrorV2f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT n);
NA_IAPI void naMirrorV3f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT n);
NA_IAPI void naMirrorV4f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT n);
NA_IAPI void naMirrorV1dE(double* d, const double* a, const double* n);
NA_IAPI void naMirrorV2dE(double* d, const double* a, const double* n);
NA_IAPI void naMirrorV3dE(double* d, const double* a, const double* n);
NA_IAPI void naMirrorV4dE(double* d, const double* a, const double* n);
NA_IAPI void naMirrorV1fE(float*  d, const float*  a, const float*  n);
NA_IAPI void naMirrorV2fE(float*  d, const float*  a, const float*  n);
NA_IAPI void naMirrorV3fE(float*  d, const float*  a, const float*  n);
NA_IAPI void naMirrorV4fE(float*  d, const float*  a, const float*  n);
NA_IAPI void naMirrorV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT n);
NA_IAPI void naMirrorV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT n);
NA_IAPI void naMirrorV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT n);
NA_IAPI void naMirrorV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT n);
NA_IAPI void naMirrorV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT n);

// Scales the vector a to a length of 1. Returns the scaled vector in d and the
// length of the vector before scaling as a return value.
// The E-variant allows d to be equal to a
// The S-variant normalizes the given vector.
// If the length of the vector was close to or equal to zero, a warning is
// emitted. In that case, you might want to call naLength first and divide the
// vector by yourself, if it is non-zero.
NA_IAPI double naNormalizeV1d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI double naNormalizeV2d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI double naNormalizeV3d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI double naNormalizeV4d (double* NA_RESTRICT d, const double* NA_RESTRICT a);
NA_IAPI float  naNormalizeV1f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI float  naNormalizeV2f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI float  naNormalizeV3f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI float  naNormalizeV4f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a);
NA_IAPI double naNormalizeV1dE(double* d, const double* a);
NA_IAPI double naNormalizeV2dE(double* d, const double* a);
NA_IAPI double naNormalizeV3dE(double* d, const double* a);
NA_IAPI double naNormalizeV4dE(double* d, const double* a);
NA_IAPI float  naNormalizeV1fE(float*  d, const float*  a);
NA_IAPI float  naNormalizeV2fE(float*  d, const float*  a);
NA_IAPI float  naNormalizeV3fE(float*  d, const float*  a);
NA_IAPI float  naNormalizeV4fE(float*  d, const float*  a);
NA_IAPI double naNormalizeV1dS(double* v);
NA_IAPI double naNormalizeV2dS(double* v);
NA_IAPI double naNormalizeV3dS(double* v);
NA_IAPI double naNormalizeV4dS(double* v);
NA_IAPI float  naNormalizeV1fS(float*  v);
NA_IAPI float  naNormalizeV2fS(float*  v);
NA_IAPI float  naNormalizeV3fS(float*  v);
NA_IAPI float  naNormalizeV4fS(float*  v);

// Makes vector a orthogonal to b and stores the result in d.
// Using Gram-Schmidt. No normalization takes place. If vector b is a zero-
// vector, a warning is emitted and you have to solve that issue by yourself.
// If a was colinear to b or a zero-vector, d will become a zero-vector.
// The E-variant allows d to be equal to a
// The S-variant stores the result directly in d.
NA_IAPI void naOrthogonalizeV2d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV3d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV4d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV2f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV3f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV4f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV2dE(double* d, const double* a, const double* b);
NA_IAPI void naOrthogonalizeV3dE(double* d, const double* a, const double* b);
NA_IAPI void naOrthogonalizeV4dE(double* d, const double* a, const double* b);
NA_IAPI void naOrthogonalizeV2fE(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthogonalizeV3fE(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthogonalizeV4fE(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthogonalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naOrthogonalizeV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b);

// Assumes b to be normalized and makes a ortho-normal to b. Stores the result
// in d. If b was not normal, a warning is emitted and the result is undefined.
// If a is colinear to b or one of the vectors is a zero-vector, d will become
// a zero-vector.
NA_IAPI void naOrthonormalizeV2d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV3d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV4d (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV2f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV3f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV4f (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV2dE(double* d, const double* a, const double* b);
NA_IAPI void naOrthonormalizeV3dE(double* d, const double* a, const double* b);
NA_IAPI void naOrthonormalizeV4dE(double* d, const double* a, const double* b);
NA_IAPI void naOrthonormalizeV2fE(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthonormalizeV3fE(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthonormalizeV4fE(float*  d, const float*  a, const float*  b);
NA_IAPI void naOrthonormalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naOrthonormalizeV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b);



// ///////////////////////////////
// Matrix
// ///////////////////////////////

// Usually in programming, matrices are stored like typeical C-arrays, speaking
// in row-first ordering. NALib is different in that respect. NALib stores the
// values of matrices internally in COLUMN-FIRST order!
//
// The indices of a matrix type therefore look as follows:
// (  m[0]   m[3]   m[6]  )
// (  m[1]   m[4]   m[7]  )
// (  m[2]   m[5]   m[8]  )
//
// But you can choose to fill the matrix according to your needs with the
// following API:

// Fills the matrix with the given values. Row first.
NA_IAPI void naFillM22dRowFirst( double* D,
   double s0 , double s1 , double s2 , double s3);
NA_IAPI void naFillM33dRowFirst( double* D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8);
NA_IAPI void naFillM44dRowFirst( double* D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8 , double s9 , double s10, double s11,
   double s12, double s13, double s14, double s15);
NA_IAPI void naFillM22fRowFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3);
NA_IAPI void naFillM33fRowFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8);
NA_IAPI void naFillM44fRowFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8 , float  s9 , float  s10, float  s11,
   float  s12, float  s13, float  s14, float  s15);

// Fills the matrix with the given values. Column first.
NA_IAPI void naFillM22dColumnFirst( double* D,
   double s0 , double s1 , double s2 , double s3);
NA_IAPI void naFillM33dColumnFirst( double* D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8);
NA_IAPI void naFillM44dColumnFirst( double* D,
   double s0 , double s1 , double s2 , double s3 , double s4 , double s5 ,
   double s6 , double s7 , double s8 , double s9 , double s10, double s11,
   double s12, double s13, double s14, double s15);
NA_IAPI void naFillM22fColumnFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3);
NA_IAPI void naFillM33fColumnFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8);
NA_IAPI void naFillM44fColumnFirst( float* D,
   float  s0 , float  s1 , float  s2 , float  s3 , float  s4 , float  s5 ,
   float  s6 , float  s7 , float  s8 , float  s9 , float  s10, float  s11,
   float  s12, float  s13, float  s14, float  s15);

// Fills the matrix with the given column vectors.
// the v vectors shall not overlap with d
NA_IAPI void naFillM22dWithAxis  (double* NA_RESTRICT  D,
                            const double* NA_RESTRICT v0,
                            const double* NA_RESTRICT v1);
NA_IAPI void naFillM33dWithAxis  (double* NA_RESTRICT  D,
                            const double* NA_RESTRICT v0,
                            const double* NA_RESTRICT v1,
                            const double* NA_RESTRICT v2);
NA_IAPI void naFillM44dWithAxis  (double* NA_RESTRICT  D,
                            const double* NA_RESTRICT v0,
                            const double* NA_RESTRICT v1,
                            const double* NA_RESTRICT v2,
                            const double* NA_RESTRICT v3);
NA_IAPI void naFillM22fWithAxis  (float*  NA_RESTRICT  D,
                            const float*  NA_RESTRICT v0,
                            const float*  NA_RESTRICT v1);
NA_IAPI void naFillM33fWithAxis  (float*  NA_RESTRICT  D,
                            const float*  NA_RESTRICT v0,
                            const float*  NA_RESTRICT v1,
                            const float*  NA_RESTRICT v2);
NA_IAPI void naFillM44fWithAxis  (float*  NA_RESTRICT  D,
                            const float*  NA_RESTRICT v0,
                            const float*  NA_RESTRICT v1,
                            const float*  NA_RESTRICT v2,
                            const float*  NA_RESTRICT v3);

// Fills the matrix diagonally with s and all other entries with zero.
// Use this function to create an identity matrix.
NA_IAPI void naFillM22dWithDiag(double* D, double s);
NA_IAPI void naFillM33dWithDiag(double* D, double s);
NA_IAPI void naFillM44dWithDiag(double* D, double s);
NA_IAPI void naFillM22fWithDiag(float*  D, float  s);
NA_IAPI void naFillM33fWithDiag(float*  D, float  s);
NA_IAPI void naFillM44fWithDiag(float*  D, float  s);

// Fills the matrix diagonally with the entries given in the diag vector and
// all other entries with zero.
NA_IAPI void naFillM22dWithDiagV2d(double* NA_RESTRICT D, const double* NA_RESTRICT diag);
NA_IAPI void naFillM33dWithDiagV3d(double* NA_RESTRICT D, const double* NA_RESTRICT diag);
NA_IAPI void naFillM44dWithDiagV4d(double* NA_RESTRICT D, const double* NA_RESTRICT diag);
NA_IAPI void naFillM22fWithDiagV2f( float* NA_RESTRICT D, const float* NA_RESTRICT diag);
NA_IAPI void naFillM33fWithDiagV3f( float* NA_RESTRICT D, const float* NA_RESTRICT diag);
NA_IAPI void naFillM44fWithDiagV4f( float* NA_RESTRICT D, const float* NA_RESTRICT diag);

// Copies the values of S to D.
// S shall not overlap D. Warnings will be emitted.
// The E variant allows S and D to be equal.
NA_IAPI void naCopyM22d (double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naCopyM33d (double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naCopyM44d (double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naCopyM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A);
NA_IAPI void naCopyM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A);
NA_IAPI void naCopyM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A);
NA_IAPI void naCopyM22dE(double* D, const double* A);
NA_IAPI void naCopyM33dE(double* D, const double* A);
NA_IAPI void naCopyM44dE(double* D, const double* A);
NA_IAPI void naCopyM22fE(float*  D, const float*  A);
NA_IAPI void naCopyM33fE(float*  D, const float*  A);
NA_IAPI void naCopyM44fE(float*  D, const float*  A);

// Negates the matrix. D = -A
// S shall not overlap D. Warnings will be emitted.
// The E variant allows S and D to be equal.
NA_IAPI void naNegM22d (double* NA_RESTRICT D, double* NA_RESTRICT A);
NA_IAPI void naNegM33d (double* NA_RESTRICT D, double* NA_RESTRICT A);
NA_IAPI void naNegM44d (double* NA_RESTRICT D, double* NA_RESTRICT A);
NA_IAPI void naNegM22f (float*  NA_RESTRICT D, float*  NA_RESTRICT A);
NA_IAPI void naNegM33f (float*  NA_RESTRICT D, float*  NA_RESTRICT A);
NA_IAPI void naNegM44f (float*  NA_RESTRICT D, float*  NA_RESTRICT A);
NA_IAPI void naNegM22dE(double* D, double* A);
NA_IAPI void naNegM33dE(double* D, double* A);
NA_IAPI void naNegM44dE(double* D, double* A);
NA_IAPI void naNegM22fE(float*  D, float*  A);
NA_IAPI void naNegM33fE(float*  D, float*  A);
NA_IAPI void naNegM44fE(float*  D, float*  A);
NA_IAPI void naNegM22dS(double* D);
NA_IAPI void naNegM33dS(double* D);
NA_IAPI void naNegM44dS(double* D);
NA_IAPI void naNegM22fS(float*  D);
NA_IAPI void naNegM33fS(float*  D);
NA_IAPI void naNegM44fS(float*  D);

// Transposes the matrix.
// E variant allows both pointers to be equal
// S variant transposes the matrix itself.
NA_IAPI void naTransposeM22d (double* NA_RESTRICT D, double* NA_RESTRICT A);
NA_IAPI void naTransposeM33d (double* NA_RESTRICT D, double* NA_RESTRICT A);
NA_IAPI void naTransposeM44d (double* NA_RESTRICT D, double* NA_RESTRICT A);
NA_IAPI void naTransposeM22f (float*  NA_RESTRICT D, float*  NA_RESTRICT A);
NA_IAPI void naTransposeM33f (float*  NA_RESTRICT D, float*  NA_RESTRICT A);
NA_IAPI void naTransposeM44f (float*  NA_RESTRICT D, float*  NA_RESTRICT A);
NA_IAPI void naTransposeM22dE(double* D, double* A);
NA_IAPI void naTransposeM33dE(double* D, double* A);
NA_IAPI void naTransposeM44dE(double* D, double* A);
NA_IAPI void naTransposeM22fE(float*  D, float*  A);
NA_IAPI void naTransposeM33fE(float*  D, float*  A);
NA_IAPI void naTransposeM44fE(float*  D, float*  A);
NA_IAPI void naTransposeM22dS(double* D);
NA_IAPI void naTransposeM33dS(double* D);
NA_IAPI void naTransposeM44dS(double* D);
NA_IAPI void naTransposeM22fS(float*  D);
NA_IAPI void naTransposeM33fS(float*  D);
NA_IAPI void naTransposeM44fS(float*  D);

// Adds two matrices: D = A + B
// E variant allows A and B to be the same as D
// S variant computes D += B
// ES variant allows B to be the same as D
NA_IAPI void naAddM22d  (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naAddM33d  (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naAddM44d  (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naAddM22f  (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naAddM33f  (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naAddM44f  (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naAddM22dE (double* D, const double* A, const double* B);
NA_IAPI void naAddM33dE (double* D, const double* A, const double* B);
NA_IAPI void naAddM44dE (double* D, const double* A, const double* B);
NA_IAPI void naAddM22fE (float*  D, const float*  A, const float*  B);
NA_IAPI void naAddM33fE (float*  D, const float*  A, const float*  B);
NA_IAPI void naAddM44fE (float*  D, const float*  A, const float*  B);
NA_IAPI void naAddM22dS (double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naAddM33dS (double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naAddM44dS (double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naAddM22fS (float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naAddM33fS (float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naAddM44fS (float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naAddM22dSE(double* D, const double* B);
NA_IAPI void naAddM33dSE(double* D, const double* B);
NA_IAPI void naAddM44dSE(double* D, const double* B);
NA_IAPI void naAddM22fSE(float*  D, const float*  B);
NA_IAPI void naAddM33fSE(float*  D, const float*  B);
NA_IAPI void naAddM44fSE(float*  D, const float*  B);

// Subtracts two matrices: D = A - B
// E variant allows A and B to be the same as D
// S variant computes D -= B
// ES variant allows B to be the same as D
NA_IAPI void naSubM22d  (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naSubM33d  (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naSubM44d  (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naSubM22f  (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naSubM33f  (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naSubM44f  (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naSubM22dE (double* D, const double* A, const double* B);
NA_IAPI void naSubM33dE (double* D, const double* A, const double* B);
NA_IAPI void naSubM44dE (double* D, const double* A, const double* B);
NA_IAPI void naSubM22fE (float*  D, const float*  A, const float*  B);
NA_IAPI void naSubM33fE (float*  D, const float*  A, const float*  B);
NA_IAPI void naSubM44fE (float*  D, const float*  A, const float*  B);
NA_IAPI void naSubM22dS (double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naSubM33dS (double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naSubM44dS (double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naSubM22fS (float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naSubM33fS (float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naSubM44fS (float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naSubM22dSE(double* D, const double* B);
NA_IAPI void naSubM33dSE(double* D, const double* B);
NA_IAPI void naSubM44dSE(double* D, const double* B);
NA_IAPI void naSubM22fSE(float*  D, const float*  B);
NA_IAPI void naSubM33fSE(float*  D, const float*  B);
NA_IAPI void naSubM44fSE(float*  D, const float*  B);

// Scales the matrix componentwise with the scalar: D = A * s
// E variant allows A to be the same as D
// S variant computes D *= s
NA_IAPI void naScaleM22d (double* NA_RESTRICT D, const double* NA_RESTRICT A, double s);
NA_IAPI void naScaleM33d (double* NA_RESTRICT D, const double* NA_RESTRICT A, double s);
NA_IAPI void naScaleM44d (double* NA_RESTRICT D, const double* NA_RESTRICT A, double s);
NA_IAPI void naScaleM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, float  s);
NA_IAPI void naScaleM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, float  s);
NA_IAPI void naScaleM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, float  s);
NA_IAPI void naScaleM22dE(double* D, const double* A, double s);
NA_IAPI void naScaleM33dE(double* D, const double* A, double s);
NA_IAPI void naScaleM44dE(double* D, const double* A, double s);
NA_IAPI void naScaleM22fE(float*  D, const float*  A, float  s);
NA_IAPI void naScaleM33fE(float*  D, const float*  A, float  s);
NA_IAPI void naScaleM44fE(float*  D, const float*  A, float  s);
NA_IAPI void naScaleM22dS(double* D, double s);
NA_IAPI void naScaleM33dS(double* D, double s);
NA_IAPI void naScaleM44dS(double* D, double s);
NA_IAPI void naScaleM22fS(float*  D, float  s);
NA_IAPI void naScaleM33fS(float*  D, float  s);
NA_IAPI void naScaleM44fS(float*  D, float  s);

// Scales the column vectors of matrix m componentwise with vector v:
// D = A * diag(v)
// E variant allows A to be the same as D
// S variant computes D *= diag(v)
NA_IAPI void naScaleM22dV2d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naScaleM33dV3d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naScaleM44dV4d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naScaleM22fV2f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naScaleM33fV3f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naScaleM44fV4f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naScaleM22dV2dE(double* D, const double* A, const double* v);
NA_IAPI void naScaleM33dV3dE(double* D, const double* A, const double* v);
NA_IAPI void naScaleM44dV4dE(double* D, const double* A, const double* v);
NA_IAPI void naScaleM22fV2fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naScaleM33fV3fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naScaleM44fV4fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naScaleM22dV2dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naScaleM33dV3dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naScaleM44dV4dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naScaleM22fV2fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);
NA_IAPI void naScaleM33fV3fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);
NA_IAPI void naScaleM44fV4fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);

// Multiplies matrix with vector. d = A * v
// No E or S variants exist. Matrix multiplications should always result in
// new values.
NA_IAPI void naMulM22dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naMulM33dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naMulM44dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naMulM22fV2f(float*  NA_RESTRICT d, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naMulM33fV3f(float*  NA_RESTRICT d, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naMulM44fV4f(float*  NA_RESTRICT d, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);

// Multiplies matrix with matrix. D = A * B
// No E or S variants exist. Matrix multiplications should always result in
// new values.
NA_IAPI void naMulM22dM22d(double* NA_RESTRICT D, const double* A, const double* B);
NA_IAPI void naMulM33dM33d(double* NA_RESTRICT D, const double* A, const double* B);
NA_IAPI void naMulM44dM44d(double* NA_RESTRICT D, const double* A, const double* B);
NA_IAPI void naMulM22fM22f(float*  NA_RESTRICT D, const float*  A, const float*  B);
NA_IAPI void naMulM33fM33f(float*  NA_RESTRICT D, const float*  A, const float*  B);
NA_IAPI void naMulM44fM44f(float*  NA_RESTRICT D, const float*  A, const float*  B);

// Computes the inverse of a matrix
// No E or S variants exist.
NA_IAPI void naInvertM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naInvertM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naInvertM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naInvertM22f(float*  NA_RESTRICT D, const float*  NA_RESTRICT A);
NA_IAPI void naInvertM33f(float*  NA_RESTRICT D, const float*  NA_RESTRICT A);
NA_IAPI void naInvertM44f(float*  NA_RESTRICT D, const float*  NA_RESTRICT A);

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
NA_IAPI void naFillM33dWithRotationAlongAxis(  double* NA_RESTRICT d,
                                        const double* NA_RESTRICT axis,
                                               double angle);
NA_IAPI void naFillM33dWithRotationAngles(     double* d,
                                               double roll,
                                               double pitch,
                                               double yaw);













// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////


#include "NARandom.h"
#include "NAMathOperators.h"


NA_IDEF void naFillV1d(double* d, double a0){
  d[0] = a0;
}
NA_IDEF void naFillV2d(double* d, double a0, double a1){
  d[0] = a0; d[1] = a1;
}
NA_IDEF void naFillV3d(double* d, double a0, double a1, double a2){
  d[0] = a0; d[1] = a1; d[2] = a2;
}
NA_IDEF void naFillV4d(double* d, double a0, double a1, double a2, double a3){
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


NA_IDEF void naNullV1d(double*  d){
  d[0] = 0.;
}
NA_IDEF void naNullV2d(double*  d){
  d[0] = 0.; d[1] = 0.;
}
NA_IDEF void naNullV3d(double*  d){
  d[0] = 0.; d[1] = 0.; d[2] = 0.;
}
NA_IDEF void naNullV4d(double*  d){
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



NA_IDEF void naFillV1dRandom(double* d){
  naFillV1d( d, naUniformRandZE());
}
NA_IDEF void naFillV2dRandom(double* d){
  naFillV2d( d, naUniformRandZE(),
              naUniformRandZE());
}
NA_IDEF void naFillV3dRandom(double* d){
  naFillV3d( d, naUniformRandZE(),
              naUniformRandZE(),
              naUniformRandZE());
}
NA_IDEF void naFillV4dRandom(double* d){
  naFillV4d( d, naUniformRandZE(),
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


NA_IDEF void naFillV1dRandomSpherical(double* d, NABool filled){
  if(filled){
    do{
      d[0] = naUniformRandZI() * 2. - 1.;
    }while(d[0] == 0.);
  }else{
    d[0] = (naUniformRandZE() < .5) ? -1. : 1.;
  }
}
NA_IDEF void naFillV2dRandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    norm = naDotV2d(d, d);
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is very rare.
  if(!filled){naDivV2d(d, d, naSqrt(norm));}
}
NA_IDEF void naFillV3dRandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    d[2] = naUniformRandZI() * 2. - 1.;
    norm = naDotV3d(d, d);
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is extremely! rare.
  if(!filled){naDivV3d(d, d, naSqrt(norm));}
}
NA_IDEF void naFillV4dRandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    d[2] = naUniformRandZI() * 2. - 1.;
    d[3] = naUniformRandZI() * 2. - 1.;
    norm = naDotV4d(d, d);
    // Note that with more and more dimensions it becomes more and more likely
    // that this do-while loop must be repeated many times. Up to 3 dimensions,
    // this method works quite well but starting with 4 dimensions, one should
    // really consider a different method. The author did not because he is
    // lazy.
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is EXORBITANTLY rare.
  if(!filled){naDivV4d(d, d, naSqrt(norm));}
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



NA_IDEF void naCopyV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV1d", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 1)
      naError("naCopyV1d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0];
}
NA_IDEF void naCopyV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV2d", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 2)
      naError("naCopyV2d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV3d", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 3)
      naError("naCopyV3d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV4d", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 4)
      naError("naCopyV4d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV1f", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 1)
      naError("naCopyV1f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0];
}
NA_IDEF void naCopyV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV2f", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 2)
      naError("naCopyV2f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV3f", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 3)
      naError("naCopyV3f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV4f", "Pointers are equal. Use E variant.");
    else if(naAbsi(d-a) < 4)
      naError("naCopyV4f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1dE (double* d, const double* a){
  d[0] = a[0];
}
NA_IDEF void naCopyV2dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 2))
      naError("naCopyV2dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 3))
      naError("naCopyV3dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 4))
      naError("naCopyV4dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1fE(float*  d, const float*  a){
  d[0] = a[0];
}
NA_IDEF void naCopyV2fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 2))
      naError("naCopyV2fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 3))
      naError("naCopyV3fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 4))
      naError("naCopyV4fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}



NA_IDEF void naNegV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV1d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 1)
      naError("naNegV1d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV2d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 2)
      naError("naNegV2d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV3d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 3)
      naError("naNegV3d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV4d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 4)
      naError("naNegV4d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV1d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 1)
      naError("naNegV1f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV2d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 2)
      naError("naNegV2f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV3d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 3)
      naError("naNegV3f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV4d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi(d-a) < 4)
      naError("naNegV4f", "Pointers overlap misaligned.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1dE (double* d, const double* a){
  d[0] = -a[0];
}
NA_IDEF void naNegV2dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 2))
      naError("naNegV2dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 3))
      naError("naNegV3dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 4))
      naError("naNegV4dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1fE(float*  d, const float*  a){
  d[0] = -a[0];
}
NA_IDEF void naNegV2fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 2))
      naError("naNegV2fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 3))
      naError("naNegV3fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi(d-a) < 4))
      naError("naNegV4fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1dS (double* d){
  d[0] = -d[0];
}
NA_IDEF void naNegV2dS (double* d){
  d[0] = -d[0]; d[1] = -d[1];
}
NA_IDEF void naNegV3dS (double* d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2];
}
NA_IDEF void naNegV4dS (double* d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2]; d[3] = -d[3];
}
NA_IDEF void naNegV1fS(float*  d){
  d[0] = -d[0];
}
NA_IDEF void naNegV2fS(float*  d){
  d[0] = -d[0]; d[1] = -d[1];
}
NA_IDEF void naNegV3fS(float*  d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2];
}
NA_IDEF void naNegV4fS(float*  d){
  d[0] = -d[0]; d[1] = -d[1]; d[2] = -d[2]; d[3] = -d[3];
}



// /////////////////////////////////////////
// Add
// /////////////////////////////////////////

NA_IDEF void naAddV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV1d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naAddV1d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naAddV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV2d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naAddV2d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naAddV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV3d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naAddV3d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naAddV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV4d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naAddV4d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naAddV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV1f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naAddV1f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naAddV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV2f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naAddV2f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naAddV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV3f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naAddV3f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naAddV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV4f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naAddV4f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naAddV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naAddV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 2))
      naError("naAddV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naAddV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 3))
      naError("naAddV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naAddV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 4))
      naError("naAddV4dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naAddV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 2))
      naError("naAddV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naAddV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 3))
      naError("naAddV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naAddV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 4))
      naError("naAddV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
  d[3] = a[3] + b[3];
}
NA_IDEF void naAddV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] += b[0];
}
NA_IDEF void naAddV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi(d-b) < 2)
      naError("naAddV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 3))
      naError("naAddV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 4))
      naError("naAddV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] += b[0];
}
NA_IDEF void naAddV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 2))
      naError("naAddV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 3))
      naError("naAddV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 4))
      naError("naAddV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1dSE(double* d, const double* b){
  d[0] += b[0];
}
NA_IDEF void naAddV2dSE(double* d, const double* b){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 2)
      naError("naAddV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 3))
      naError("naAddV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 4))
      naError("naAddV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}
NA_IDEF void naAddV1fSE(float*  d, const float*  b){
  d[0] += b[0];
}
NA_IDEF void naAddV2fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 2))
      naError("naAddV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 3))
      naError("naAddV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 4))
      naError("naAddV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
  d[3] += b[3];
}



// /////////////////////////////////////////
// Sub
// /////////////////////////////////////////

NA_IDEF void naSubV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV1d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naSubV1d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naSubV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV2d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naSubV2d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naSubV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV3d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naSubV3d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naSubV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV4d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naSubV4d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naSubV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV1f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naSubV1f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naSubV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV2f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naSubV2f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naSubV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV3f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naSubV3f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naSubV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV4f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naSubV4f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naSubV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naSubV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 2))
      naError("naSubV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naSubV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 3))
      naError("naSubV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naSubV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 4))
      naError("naSubV4dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naSubV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 2))
      naError("naSubV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naSubV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 3))
      naError("naSubV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naSubV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 4))
      naError("naSubV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
  d[3] = a[3] - b[3];
}
NA_IDEF void naSubV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] -= b[0];
}
NA_IDEF void naSubV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi(d-b) < 2)
      naError("naSubV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 3))
      naError("naSubV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 4))
      naError("naSubV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] -= b[0];
}
NA_IDEF void naSubV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 2))
      naError("naSubV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 3))
      naError("naSubV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 4))
      naError("naSubV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1dSE(double* d, const double* b){
  d[0] -= b[0];
}
NA_IDEF void naSubV2dSE(double* d, const double* b){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 2)
      naError("naSubV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 3))
      naError("naSubV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 4))
      naError("naSubV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}
NA_IDEF void naSubV1fSE(float*  d, const float*  b){
  d[0] -= b[0];
}
NA_IDEF void naSubV2fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 2))
      naError("naSubV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 3))
      naError("naSubV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 4))
      naError("naSubV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
  d[3] -= b[3];
}


// /////////////////////////////////////////////
// Mul
// /////////////////////////////////////////////

NA_IDEF void naMulV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV1d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naMulV1d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV2d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naMulV2d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV3d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naMulV3d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV4d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naMulV4d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV1f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naMulV1f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV2f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naMulV2f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV3f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naMulV3f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV4f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naMulV4f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1dE(double* d, const double* a, double s){
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naMulV2dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naMulV3dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naMulV4dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1fE(float*  d, const float*  a, float s){
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naMulV2fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naMulV3fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naMulV4fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
  d[3] = a[3] * s;
}
NA_IDEF void naMulV1dS(double* d, double s){
  d[0] *= s;
}
NA_IDEF void naMulV2dS(double* d, double s){
  d[0] *= s;
  d[1] *= s;
}
NA_IDEF void naMulV3dS(double* d, double s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
}
NA_IDEF void naMulV4dS(double* d, double s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
  d[3] *= s;
}
NA_IDEF void naMulV1fS(float* d, float s){
  d[0] *= s;
}
NA_IDEF void naMulV2fS(float* d, float s){
  d[0] *= s;
  d[1] *= s;
}
NA_IDEF void naMulV3fS(float* d, float s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
}
NA_IDEF void naMulV4fS(float* d, float s){
  d[0] *= s;
  d[1] *= s;
  d[2] *= s;
  d[3] *= s;
}



// ///////////////////////////////////////////
// Div
// ///////////////////////////////////////////

NA_IDEF void naDivV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naDivV1d", "a overlaps misaligned with d.");
  #endif
  double div = naInv(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naDivV2d", "a overlaps misaligned with d.");
  #endif
  double div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naDivV3d", "a overlaps misaligned with d.");
  #endif
  double div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naDivV4d", "a overlaps misaligned with d.");
  #endif
  double div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naDivV1f", "a overlaps misaligned with d.");
  #endif
  float div = naInvf(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naDivV2f", "a overlaps misaligned with d.");
  #endif
  float div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naDivV3f", "a overlaps misaligned with d.");
  #endif
  float div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naDivV4f", "a overlaps misaligned with d.");
  #endif
  float div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1dE(double* d, const double* a, double s){
  double div = naInv(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naDivV2dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  double div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naDivV3dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  double div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naDivV4dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  double div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1fE(float*  d, const float*  a, float s){
  float div = naInvf(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naDivV2fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  float div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naDivV3fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  float div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naDivV4fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  float div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
}
NA_IDEF void naDivV2dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
  d[1] *= div;
}
NA_IDEF void naDivV3dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
}
NA_IDEF void naDivV4dS(double* d, double s){
  double div = naInv(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
  d[3] *= div;
}
NA_IDEF void naDivV1fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
}
NA_IDEF void naDivV2fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
  d[1] *= div;
}
NA_IDEF void naDivV3fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
}
NA_IDEF void naDivV4fS(float* d, float s){
  float div = naInvf(s);
  d[0] *= div;
  d[1] *= div;
  d[2] *= div;
  d[3] *= div;
}



// //////////////////////////////////////////////
// Div componentwise
// //////////////////////////////////////////////

NA_IDEF void naDivV1dV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1dV1d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naDivV1dV1d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naDivV1dV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivV2dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2dV2d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naDivV2dV2d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naDivV2dV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivV3dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3dV3d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naDivV3dV3d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naDivV3dV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivV4dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4dV4d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naDivV4dV4d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naDivV4dV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivV1fV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1fV1f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naDivV1fV1f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naDivV1fV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivV2fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2fV2f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naDivV2fV2f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naDivV2fV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivV3fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3fV3f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naDivV3fV3f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naDivV3fV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivV4fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4fV4f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naDivV4fV4f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naDivV4fV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivV1dV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivV2dV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naDivV2dV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 2))
      naError("naDivV2dV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivV3dV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naDivV3dV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 3))
      naError("naDivV3dV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivV4dV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naDivV4dV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 4))
      naError("naDivV4dV4dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivV1fV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivV2fV2fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 2))
      naError("naDivV2fV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 2))
      naError("naDivV2fV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivV3fV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 3))
      naError("naDivV3fV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 3))
      naError("naDivV3fV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivV4fV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi(d-a) < 4))
      naError("naDivV4fV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi(d-b) < 4))
      naError("naDivV4fV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivV1dV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV1dV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivV2dV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV2dV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi(d-b) < 2)
      naError("naDivV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivV3dV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV3dV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 3))
      naError("naDivV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivV4dV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV4dV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 4))
      naError("naDivV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
}
NA_IDEF void naDivV1fV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV1fV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivV2fV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV2fV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 2))
      naError("naDivV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivV3fV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV3fV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 3))
      naError("naDivV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivV4fV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivV4fV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi(d-b) < 4))
      naError("naDivV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
}
NA_IDEF void naDivV1dV1dSE(double* d, const double* b){
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivV2dV2dSE(double* d, const double* b){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 2)
      naError("naDivV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivV3dV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 3))
      naError("naDivV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivV4dV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 4))
      naError("naDivV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
}
NA_IDEF void naDivV1fV1fSE(float*  d, const float*  b){
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivV2fV2fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 2))
      naError("naDivV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivV3fV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 3))
      naError("naDivV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivV4fV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi(d-b) < 4))
      naError("naDivV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
}




// /////////////////////////////////////////////
// Mad
// /////////////////////////////////////////////

NA_IDEF void naMadV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV1d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naMadV1d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naMadV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV2d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naMadV2d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naMadV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV3d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naMadV3d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naMadV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV4d", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naMadV4d", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naMadV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV1f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 1)
      naError("naMadV1f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 1)
      naError("naMadV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV2f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 2)
      naError("naMadV2f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 2)
      naError("naMadV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV3f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 3)
      naError("naMadV3f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 3)
      naError("naMadV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV4f", "a equals d. Use S or E variant");
    else if(naAbsi(d-a) < 4)
      naError("naMadV4f", "a overlaps misaligned with d.");
    if(naAbsi(d-b) < 4)
      naError("naMadV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 4))
      naError("naMadV1dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 4))
      naError("naMadV1dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 2))
      naError("naMadV2dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 2))
      naError("naMadV2dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 3))
      naError("naMadV3dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 3))
      naError("naMadV3dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 4))
      naError("naMadV4dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 4))
      naError("naMadV4dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 1))
      naError("naMadV1fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 1))
      naError("naMadV1fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 2))
      naError("naMadV2fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 2))
      naError("naMadV2fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 3))
      naError("naMadV3fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 3))
      naError("naMadV3fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi(d-a) < 4))
      naError("naMadV4fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi(d-b) < 4))
      naError("naMadV4fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 1)
      naError("naMadV1dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 2)
      naError("naMadV2dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 3)
      naError("naMadV3dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 4)
      naError("naMadV4dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 1)
      naError("naMadV1fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 2)
      naError("naMadV2fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 3)
      naError("naMadV3fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi(d-b) < 4)
      naError("naMadV4fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 4))
      naError("naMadV1dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 2))
      naError("naMadV2dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 3))
      naError("naMadV3dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 4))
      naError("naMadV4dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 1))
      naError("naMadV1fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 2))
      naError("naMadV2fE", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 3))
      naError("naMadV3fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi(d-b) < 4))
      naError("naMadV4fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}


// ////////////////////////////////////////////
// Equal
// ////////////////////////////////////////////

NA_IDEF NABool naEqualV1d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV1d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 1)
      naError("naEqualV1d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV2d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 2)
      naError("naEqualV2d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV3d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 3)
      naError("naEqualV3d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV4d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 4)
      naError("naEqualV4d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualV1f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV1f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 1)
      naError("naEqualV1f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV2f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 2)
      naError("naEqualV2f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV3f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 3)
      naError("naEqualV3f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV4f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 4)
      naError("naEqualV4f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualV1dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 1))
      naError("naEqualV1dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 2))
      naError("naEqualV2dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 3))
      naError("naEqualV3dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 4))
      naError("naEqualV4dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualV1fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 1))
      naError("naEqualV1fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualV2fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 2))
      naError("naEqualV2fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]));
}
NA_IDEF NABool naEqualV3fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 3))
      naError("naEqualV3fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]));
}
NA_IDEF NABool naEqualV4fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 4))
      naError("naEqualV4fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}



// ///////////////////////////////////////////
// Dot
// ///////////////////////////////////////////

NA_IDEF double naDotV1d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV1d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 1)
      naError("naEqualV1d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF double naDotV2d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV2d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 2)
      naError("naEqualV2d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF double naDotV3d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV3d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 3)
      naError("naEqualV3d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF double naDotV4d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV4d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 4)
      naError("naEqualV4d", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF float naDotV1f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV1f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 1)
      naError("naEqualV1f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF float naDotV2f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV2f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 2)
      naError("naEqualV2f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF float naDotV3f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV3f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 3)
      naError("naEqualV3f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF float naDotV4f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualV4f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi(a-b) < 4)
      naError("naEqualV4f", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF double naDotV1dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 1))
      naError("naEqualV1dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF double naDotV2dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 2))
      naError("naEqualV2dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF double naDotV3dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 3))
      naError("naEqualV3dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF double naDotV4dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 4))
      naError("naEqualV4dE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}
NA_IDEF float naDotV1fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 1))
      naError("naEqualV1fE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0];
}
NA_IDEF float naDotV2fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 2))
      naError("naEqualV2fE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1];
}
NA_IDEF float naDotV3fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 3))
      naError("naEqualV3fE", "b overlaps misaligned with a");
  #endif
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}
NA_IDEF float naDotV4fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi(a-b) < 4))
      naError("naEqualV4fE", "b overlaps misaligned with a");
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



// /////////////////////////////////////////
// Cross
// /////////////////////////////////////////

NA_IDEF void naCrossV3d(double* d, const double* a, const double* b){
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
}
NA_IDEF void naCrossV3f(float* d, const float* a, const float* b){
  d[0] = a[1] * b[2] - a[2] * b[1];
  d[1] = a[2] * b[0] - a[0] * b[2];
  d[2] = a[0] * b[1] - a[1] * b[0];
}



// /////////////////////////////////////////
// Length
// /////////////////////////////////////////

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


// /////////////////////////////////////////
// Distance
// /////////////////////////////////////////

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
    if(naAbsi(n-d) < 1)
      naError("naMirrorV1d", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV1d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 1)
      naError("naMirrorV1d", "a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 2)
      naError("naMirrorV2d", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV2d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 2)
      naError("naMirrorV2d", "a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 3)
      naError("naMirrorV3d", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV3d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 3)
      naError("naMirrorV3d", "a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 4)
      naError("naMirrorV4d", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV4d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 4)
      naError("naMirrorV4d", "a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 1)
      naError("naMirrorV1f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV1f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 1)
      naError("naMirrorV1f", "a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 2)
      naError("naMirrorV2f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV2f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 2)
      naError("naMirrorV2f", "a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 3)
      naError("naMirrorV3f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV3f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 3)
      naError("naMirrorV3f", "a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 4)
      naError("naMirrorV4f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("naMirrorV4f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV4f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi(a-d) < 4)
      naError("naMirrorV4f", "a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dE(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 1)
      naError("naMirrorV1dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1dE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 1)
      naError("naMirrorV1dE", "a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2dE(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 2)
      naError("naMirrorV2dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2dE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 2)
      naError("naMirrorV2dE", "a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3dE(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 3)
      naError("naMirrorV3dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3dE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 3)
      naError("naMirrorV3dE", "a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4dE(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 4)
      naError("naMirrorV4dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4dE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 4)
      naError("naMirrorV4dE", "a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1fE(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 1)
      naError("naMirrorV1fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1fE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 1)
      naError("naMirrorV1fE", "a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2fE(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 2)
      naError("naMirrorV2fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2fE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 2)
      naError("naMirrorV2fE", "a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3fE(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 3)
      naError("naMirrorV3fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3fE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 3)
      naError("naMirrorV3fE", "a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4fE(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 4)
      naError("naMirrorV4fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("naMirrorV4fE", "Normal is not normalized");
    if((a != d) && naAbsi(a-d) < 4)
      naError("naMirrorV4fE", "a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 1)
      naError("naMirrorV1dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1dS", "Normal is not normalized");
  #endif
  naMadV1dS(d, n, -2. * naDotV1d(d, n));
}
NA_IDEF void naMirrorV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 2)
      naError("naMirrorV2dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2dS", "Normal is not normalized");
  #endif
  naMadV2dS(d, n, -2. * naDotV2d(d, n));
}
NA_IDEF void naMirrorV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 3)
      naError("naMirrorV3dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3dS", "Normal is not normalized");
  #endif
  naMadV3dS(d, n, -2. * naDotV3d(d, n));
}
NA_IDEF void naMirrorV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 4)
      naError("naMirrorV4dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4dS", "Normal is not normalized");
  #endif
  naMadV4dS(d, n, -2. * naDotV4d(d, n));
}
NA_IDEF void naMirrorV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 1)
      naError("naMirrorV1fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1fS", "Normal is not normalized");
  #endif
  naMadV1fS(d, n, -2.f * naDotV1f(d, n));
}
NA_IDEF void naMirrorV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 2)
      naError("naMirrorV2fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2fS", "Normal is not normalized");
  #endif
  naMadV2fS(d, n, -2.f * naDotV2f(d, n));
}
NA_IDEF void naMirrorV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 3)
      naError("naMirrorV3fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3fS", "Normal is not normalized");
  #endif
  naMadV3fS(d, n, -2.f * naDotV3f(d, n));
}
NA_IDEF void naMirrorV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi(n-d) < 4)
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
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV1d", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 1)
      naError("naNormalizeV1d", "a overlaps misaligned with d");
  #endif
  double length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV2d", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 2)
      naError("naNormalizeV2d", "a overlaps misaligned with d");
  #endif
  double length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV3d", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 3)
      naError("naNormalizeV3d", "a overlaps misaligned with d");
  #endif
  double length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV4d", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 4)
      naError("naNormalizeV4d", "a overlaps misaligned with d");
  #endif
  double length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV1f", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 1)
      naError("naNormalizeV1f", "a overlaps misaligned with d");
  #endif
  float length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV2f", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 2)
      naError("naNormalizeV2f", "a overlaps misaligned with d");
  #endif
  float length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV3f", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 3)
      naError("naNormalizeV3f", "a overlaps misaligned with d");
  #endif
  float length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV4f", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 4)
      naError("naNormalizeV4f", "a overlaps misaligned with d");
  #endif
  float length = naLengthV4f(a);
  naDivV4f(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV1dE(double* d, const double* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 1)
      naError("naNormalizeV1dE", "a overlaps misaligned with d");
  #endif
  double length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2dE(double* d, const double* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 2)
      naError("naNormalizeV2dE", "a overlaps misaligned with d");
  #endif
  double length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3dE(double* d, const double* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 3)
      naError("naNormalizeV3dE", "a overlaps misaligned with d");
  #endif
  double length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4dE(double* d, const double* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 4)
      naError("naNormalizeV4dE", "a overlaps misaligned with d");
  #endif
  double length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1fE(float* d, const float* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 1)
      naError("naNormalizeV1fE", "a overlaps misaligned with d");
  #endif
  float length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2fE(float* d, const float* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 2)
      naError("naNormalizeV2fE", "a overlaps misaligned with d");
  #endif
  float length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3fE(float* d, const float* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 3)
      naError("naNormalizeV3fE", "a overlaps misaligned with d");
  #endif
  float length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4fE(float* d, const float* a){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 4)
      naError("naNormalizeV4fE", "a overlaps misaligned with d");
  #endif
  float length = naLengthV4f(a);
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


// ///////////////////////////////////////////////
// Orthogonalize
// ///////////////////////////////////////////////

NA_IDEF void naOrthogonalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthogonalizeV2d", "a is equal to d. Use E or S variant.");
    else if(naAbsi(a-d) < 2)
      naError("naOrthogonalizeV2d", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 2)
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
    else if(naAbsi(a-d) < 3)
      naError("naOrthogonalizeV3d", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 3)
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
    else if(naAbsi(a-d) < 4)
      naError("naOrthogonalizeV4d", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 4)
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
    else if(naAbsi(a-d) < 2)
      naError("naOrthogonalizeV2f", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 2)
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
    else if(naAbsi(a-d) < 3)
      naError("naOrthogonalizeV3f", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 3)
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
    else if(naAbsi(a-d) < 4)
      naError("naOrthogonalizeV4f", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dE(double* d, const double* a, const double* b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 2)
      naError("naOrthogonalizeV2d", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 2)
      naError("naOrthogonalizeV2d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV2d", "b looks like a zero vector.");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dE(double* d, const double* a, const double* b){
  double bnorm = naDotV3dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 3)
      naError("naOrthogonalizeV3d", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 3)
      naError("naOrthogonalizeV3d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV3d", "b looks like a zero vector.");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dE(double* d, const double* a, const double* b){
  double bnorm = naDotV4dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 4)
      naError("naOrthogonalizeV4d", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 4)
      naError("naOrthogonalizeV4d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV4d", "b looks like a zero vector.");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fE(float* d, const float* a, const float* b){
  float bnorm = naDotV2fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 2)
      naError("naOrthogonalizeV2f", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 2)
      naError("naOrthogonalizeV2f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV2f", "b looks like a zero vector.");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fE(float* d, const float* a, const float* b){
  float bnorm = naDotV3fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 3)
      naError("naOrthogonalizeV3f", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 3)
      naError("naOrthogonalizeV3f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV3f", "b looks like a zero vector.");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fE(float* d, const float* a, const float* b){
  float bnorm = naDotV4fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 4)
      naError("naOrthogonalizeV4f", "a overlaps misaligned with d");
    if(naAbsi(b-d) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if(naAbsi(b-d) < 2)
      naError("naOrthogonalizeV2d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV2d", "b looks like a zero vector.");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV3dS(b);
  #ifndef NDEBUG
    if(naAbsi(b-d) < 3)
      naError("naOrthogonalizeV3d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV3d", "b looks like a zero vector.");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV4dS(b);
  #ifndef NDEBUG
    if(naAbsi(b-d) < 4)
      naError("naOrthogonalizeV4d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV4d", "b looks like a zero vector.");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV2fS(b);
  #ifndef NDEBUG
    if(naAbsi(b-d) < 2)
      naError("naOrthogonalizeV2f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV2f", "b looks like a zero vector.");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV3fS(b);
  #ifndef NDEBUG
    if(naAbsi(b-d) < 3)
      naError("naOrthogonalizeV3f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV3f", "b looks like a zero vector.");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV4fS(b);
  #ifndef NDEBUG
    if(naAbsi(b-d) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4fS(d, b, - naDotV4f(b, d) * naInvf(bnorm));
}



// ///////////////////////////////////////////////
// Orthonormalize
// ///////////////////////////////////////////////

NA_IDEF void naOrthonormalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV2d", "a equals d. Use E or S variant.");
    else if(naAbsi(a-d) < 2)
      naError("naOrthonormalizeV2d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi(b-d) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV3d", "a equals d. Use E or S variant.");
    else if(naAbsi(a-d) < 3)
      naError("naOrthonormalizeV3d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi(b-d) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV4d", "a equals d. Use E or S variant.");
    else if(naAbsi(a-d) < 4)
      naError("naOrthonormalizeV4d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi(b-d) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV2f", "a equals d. Use E or S variant.");
    else if(naAbsi(a-d) < 2)
      naError("naOrthonormalizeV2f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi(b-d) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV3f", "a equals d. Use E or S variant.");
    else if(naAbsi(a-d) < 3)
      naError("naOrthonormalizeV3f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi(b-d) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV4f", "a equals d. Use E or S variant.");
    else if(naAbsi(a-d) < 4)
      naError("naOrthonormalizeV4f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi(b-d) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 2)
      naError("naOrthonormalizeV2d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi(b-d) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 3)
      naError("naOrthonormalizeV3d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi(b-d) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 4)
      naError("naOrthonormalizeV4d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi(b-d) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 2)
      naError("naOrthonormalizeV2f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi(b-d) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 3)
      naError("naOrthonormalizeV3f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi(b-d) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi(a-d) < 4)
      naError("naOrthonormalizeV4f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi(b-d) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi(b-d) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi(b-d) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi(b-d) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi(b-d) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi(b-d) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi(b-d) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4fS(d, b, - naDotV4f(b, d));
  naNormalizeV4fS(d);
}











// ////////////////////////////////////////////
// Matrix algebra, stored in COLUMN-FIRST order
// ////////////////////////////////////////////




// /////////////////////////////////
// Fill Row First
// /////////////////////////////////

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



// /////////////////////////////////
// Fill Column First
// /////////////////////////////////


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



// /////////////////////////////////
// Fill with Axis
// /////////////////////////////////

NA_IDEF void naFillM22dWithAxis(double* NA_RESTRICT D,
                         const double* NA_RESTRICT v0,
                         const double* NA_RESTRICT v1){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, D-v0))
      naError("naFillM22dWithAxis", "v0 overlaps with D");
    if(naInsidei(-3, 1, D-v1))
      naError("naFillM22dWithAxis", "v1 overlaps with D");
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
    if(naInsidei(-8, 2, D-v0))
      naError("naFillM33dWithAxis", "v0 overlaps with D");
    if(naInsidei(-8, 2, D-v1))
      naError("naFillM33dWithAxis", "v1 overlaps with D");
    if(naInsidei(-8, 2, D-v2))
      naError("naFillM33dWithAxis", "v2 overlaps with D");
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
    if(naInsidei(-15, 3, D-v0))
      naError("naFillM44dWithAxis", "v0 overlaps with D");
    if(naInsidei(-15, 3, D-v1))
      naError("naFillM44dWithAxis", "v1 overlaps with D");
    if(naInsidei(-15, 3, D-v2))
      naError("naFillM44dWithAxis", "v2 overlaps with D");
    if(naInsidei(-15, 3, D-v3))
      naError("naFillM44dWithAxis", "v3 overlaps with D");
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
    if(naInsidei(-3, 1, D-v0))
      naError("naFillM22fWithAxis", "v0 overlaps with D");
    if(naInsidei(-3, 1, D-v1))
      naError("naFillM22fWithAxis", "v1 overlaps with D");
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
    if(naInsidei(-8, 2, D-v0))
      naError("naFillM33fWithAxis", "v0 overlaps with D");
    if(naInsidei(-8, 2, D-v1))
      naError("naFillM33fWithAxis", "v1 overlaps with D");
    if(naInsidei(-8, 2, D-v2))
      naError("naFillM33fWithAxis", "v2 overlaps with D");
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
    if(naInsidei(-15, 3, D-v0))
      naError("naFillM44fWithAxis", "v0 overlaps with D");
    if(naInsidei(-15, 3, D-v1))
      naError("naFillM44fWithAxis", "v1 overlaps with D");
    if(naInsidei(-15, 3, D-v2))
      naError("naFillM44fWithAxis", "v2 overlaps with D");
    if(naInsidei(-15, 3, D-v3))
      naError("naFillM44fWithAxis", "v3 overlaps with D");
  #endif
  naFillM44fColumnFirst(D,
                v0[0], v0[1], v0[2], v0[3],
                v1[0], v1[1], v1[2], v1[3],
                v2[0], v2[1], v2[2], v2[3],
                v3[0], v3[1], v3[2], v3[3]);
}



// /////////////////////////////////
// Fill With Diag Scalar
// /////////////////////////////////


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



// /////////////////////////////////
// Fill With Diag Vector
// /////////////////////////////////

NA_IDEF void naFillM22dWithDiagV2d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, D-diag))
      naError("naFillM22dWithDiagV2d", "diag overlaps with D");
  #endif
  naFillM22dColumnFirst(D,
                diag[0], 0.     ,
                0.     , diag[1]);
}
NA_IDEF void naFillM33dWithDiagV3d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, D-diag))
      naError("naFillM33dWithDiagV3d", "diag overlaps with D");
  #endif
  naFillM33dColumnFirst(D,
                diag[0], 0.     , 0.,
                0.     , diag[1], 0.,
                0.     , 0.     , diag[2]);
}
NA_IDEF void naFillM44dWithDiagV4d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, D-diag))
      naError("naFillM44dWithDiagV4d", "diag overlaps with D");
  #endif
  naFillM44dColumnFirst(D,
                diag[0], 0.     , 0.     , 0.,
                0.     , diag[1], 0.     , 0.,
                0.     , 0.     , diag[2], 0.,
                0.     , 0.     , 0.     , diag[3]);
}
NA_IDEF void naFillM22fWithDiagV2f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, D-diag))
      naError("naFillM22fWithDiagV2f", "diag overlaps with D");
  #endif
  naFillM22fColumnFirst(D,  diag[0], 0.f    ,
                0.f    , diag[1]);
}
NA_IDEF void naFillM33fWithDiagV3f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, D-diag))
      naError("naFillM33fWithDiagV3f", "diag overlaps with D");
  #endif
  naFillM33fColumnFirst(D,
                diag[0], 0.f    , 0.f,
                0.f    , diag[1], 0.f,
                0.f    , 0.f    , diag[2]);
}
NA_IDEF void naFillM44fWithDiagV4f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, D-diag))
      naError("naFillM44fWithDiagV4f", "diag overlaps with D");
  #endif
  naFillM44fColumnFirst(D,
                diag[0], 0.f    , 0.f    , 0.f,
                0.f    , diag[1], 0.f    , 0.f,
                0.f    , 0.f    , diag[2], 0.f,
                0.f    , 0.f    , 0.f    , diag[3]);
}



// /////////////////////////////////
// Copy
// /////////////////////////////////

NA_IDEF void naCopyM22d( double* NA_RESTRICT D, const double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM22d", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi(D-A < 4))
      naError("naCopyM22d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33d( double* NA_RESTRICT D, const double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM33d", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi(D-A < 9))
      naError("naCopyM33d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44d( double* NA_RESTRICT D, const double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM44d", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi(D-A < 16))
      naError("naCopyM44d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22f( float* NA_RESTRICT D, const float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM22f", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi(D-A < 4))
      naError("naCopyM22f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33f( float* NA_RESTRICT D, const float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM33f", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi(D-A < 9))
      naError("naCopyM33f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44f( float* NA_RESTRICT D, const float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM44f", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi(D-A < 16))
      naError("naCopyM44f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A < 4))
      naError("naCopyM22dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A < 9))
      naError("naCopyM33dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A < 16))
      naError("naCopyM44dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A < 4))
      naError("naCopyM22fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A < 9))
      naError("naCopyM33fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A < 16))
      naError("naCopyM44fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}


// /////////////////////////////////
// Neg
// /////////////////////////////////

NA_IDEF void naNegM22d( double* D, double* A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM22d", "A is equal to D. Use E or S variant.");
    else if(naAbsi(D-A < 4))
      naError("naNegM22d", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33d( double* D, double* A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM33d", "A is equal to D. Use E or S variant.");
    else if(naAbsi(D-A < 9))
      naError("naNegM33d", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44d( double* D, double* A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM44d", "A is equal to D. Use E or S variant.");
    else if(naAbsi(D-A < 16))
      naError("naNegM44d", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[4] = -A[4]; D[ 8] = -A[ 8]; D[12] = -A[12];
  D[1] = -A[1]; D[5] = -A[5]; D[ 9] = -A[ 9]; D[13] = -A[13];
  D[2] = -A[2]; D[6] = -A[6]; D[10] = -A[10]; D[14] = -A[14];
  D[3] = -A[3]; D[7] = -A[7]; D[11] = -A[11]; D[15] = -A[15];
}
NA_IDEF void naNegM22f( float* D, float* A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM22f", "A is equal to D. Use E or S variant.");
    else if(naAbsi(D-A < 4))
      naError("naNegM22f", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33f( float* D, float* A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM33f", "A is equal to D. Use E or S variant.");
    else if(naAbsi(D-A < 9))
      naError("naNegM33f", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44f( float* D, float* A){
  #ifndef NDEBUG
    if(D == A)
      naError("naNegM44f", "A is equal to D. Use E or S variant.");
    else if(naAbsi(D-A < 16))
      naError("naNegM44f", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[4] = -A[4]; D[ 8] = -A[ 8]; D[12] = -A[12];
  D[1] = -A[1]; D[5] = -A[5]; D[ 9] = -A[ 9]; D[13] = -A[13];
  D[2] = -A[2]; D[6] = -A[6]; D[10] = -A[10]; D[14] = -A[14];
  D[3] = -A[3]; D[7] = -A[7]; D[11] = -A[11]; D[15] = -A[15];
}
NA_IDEF void naNegM22dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 4))
      naError("naNegM22dE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 9))
      naError("naNegM33dE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 16))
      naError("naNegM44dE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[4] = -A[4]; D[ 8] = -A[ 8]; D[12] = -A[12];
  D[1] = -A[1]; D[5] = -A[5]; D[ 9] = -A[ 9]; D[13] = -A[13];
  D[2] = -A[2]; D[6] = -A[6]; D[10] = -A[10]; D[14] = -A[14];
  D[3] = -A[3]; D[7] = -A[7]; D[11] = -A[11]; D[15] = -A[15];
}
NA_IDEF void naNegM22fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 4))
      naError("naNegM22fE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[2] = -A[2];
  D[1] = -A[1]; D[3] = -A[3];
}
NA_IDEF void naNegM33fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 9))
      naError("naNegM33fE", "A overlaps misaligned with D");
  #endif
  D[0] = -A[0]; D[3] = -A[3]; D[6] = -A[6];
  D[1] = -A[1]; D[4] = -A[4]; D[7] = -A[7];
  D[2] = -A[2]; D[5] = -A[5]; D[8] = -A[8];
}
NA_IDEF void naNegM44fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 16))
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



// /////////////////////////////////
// Transpose
// /////////////////////////////////

NA_IDEF void naTransposeM22d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM22d", "A equals D. Use E or S variant.");
    else if(naAbsi(D-A < 4))
      naError("naTransposeM22d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM33d", "A equals D. Use E or S variant.");
    else if(naAbsi(D-A < 9))
      naError("naTransposeM33d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM44d", "A equals D. Use E or S variant.");
    else if(naAbsi(D-A < 16))
      naError("naTransposeM44d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22f( float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM22f", "A equals D. Use E or S variant.");
    else if(naAbsi(D-A < 4))
      naError("naTransposeM22f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33f( float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM33f", "A equals D. Use E or S variant.");
    else if(naAbsi(D-A < 9))
      naError("naTransposeM33f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44f( float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM44f", "A equals D. Use E or S variant.");
    else if(naAbsi(D-A < 16))
      naError("naTransposeM44f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 4))
      naError("naTransposeM22dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 9))
      naError("naTransposeM33dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 16))
      naError("naTransposeM44dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 4))
      naError("naTransposeM22fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 9))
      naError("naTransposeM33fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi(D-A < 16))
      naError("naTransposeM44fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22dS(double* D){
  naSwap64(&(D[1]),  &(D[2]));
}
NA_IDEF void naTransposeM33dS(double* D){
  naSwap64(&(D[1]),  &(D[3]));
  naSwap64(&(D[2]),  &(D[6]));
  naSwap64(&(D[5]),  &(D[7]));
}
NA_IDEF void naTransposeM44dS(double* D){
  naSwap64(&(D[1]),  &(D[4]));
  naSwap64(&(D[2]),  &(D[8]));
  naSwap64(&(D[3]),  &(D[12]));
  naSwap64(&(D[6]),  &(D[9]));
  naSwap64(&(D[7]),  &(D[13]));
  naSwap64(&(D[11]), &(D[14]));
}
NA_IDEF void naTransposeM22fS(float* D){
  naSwap32(&(D[1]),  &(D[2]));
}
NA_IDEF void naTransposeM33fS(float* D){
  naSwap32(&(D[1]),  &(D[3]));
  naSwap32(&(D[2]),  &(D[6]));
  naSwap32(&(D[5]),  &(D[7]));
}
NA_IDEF void naTransposeM44fS(float* D){
  naSwap32(&(D[1]),  &(D[4]));
  naSwap32(&(D[2]),  &(D[8]));
  naSwap32(&(D[3]),  &(D[12]));
  naSwap32(&(D[6]),  &(D[9]));
  naSwap32(&(D[7]),  &(D[13]));
  naSwap32(&(D[11]), &(D[14]));
}


// //////////////////////////////////////
// Add
// //////////////////////////////////////

NA_IDEF void naAddM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM22d", "A equals D. Use S or E variant");
    else if(naAbsi(D-A) < 4)
      naError("naAddM22d", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naAddM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM33d", "A equals D. Use S or E variant");
    else if(naAbsi(D-A) < 9)
      naError("naAddM33d", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
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
    else if(naAbsi(D-A) < 16)
      naError("naAddM44d", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
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
    else if(naAbsi(D-A) < 4)
      naError("naAddM22f", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naAddM22f", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naAddM33f", "A equals D. Use S or E variant");
    else if(naAbsi(D-A) < 9)
      naError("naAddM33f", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
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
    else if(naAbsi(D-A) < 16)
      naError("naAddM44f", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
      naError("naAddM44f", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[4] = A[4]+B[4]; D[ 8] = A[ 8]+B[ 8]; D[12] = A[12]+B[12];
  D[1] = A[1]+B[1]; D[5] = A[5]+B[5]; D[ 9] = A[ 9]+B[ 9]; D[13] = A[13]+B[13];
  D[2] = A[2]+B[2]; D[6] = A[6]+B[6]; D[10] = A[10]+B[10]; D[14] = A[14]+B[14];
  D[3] = A[3]+B[3]; D[7] = A[7]+B[7]; D[11] = A[11]+B[11]; D[15] = A[15]+B[15];
}
NA_IDEF void naAddM22dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 4)
      naError("naAddM22dE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naAddM22dE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33dE(double*  D, const double*  A, const double*  B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 9)
      naError("naAddM33dE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
      naError("naAddM33dE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[3] = A[3]+B[3]; D[6] = A[6]+B[6];
  D[1] = A[1]+B[1]; D[4] = A[4]+B[4]; D[7] = A[7]+B[7];
  D[2] = A[2]+B[2]; D[5] = A[5]+B[5]; D[8] = A[8]+B[8];
}
NA_IDEF void naAddM44dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 16)
      naError("naAddM44dE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
      naError("naAddM44dE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[4] = A[4]+B[4]; D[ 8] = A[ 8]+B[ 8]; D[12] = A[12]+B[12];
  D[1] = A[1]+B[1]; D[5] = A[5]+B[5]; D[ 9] = A[ 9]+B[ 9]; D[13] = A[13]+B[13];
  D[2] = A[2]+B[2]; D[6] = A[6]+B[6]; D[10] = A[10]+B[10]; D[14] = A[14]+B[14];
  D[3] = A[3]+B[3]; D[7] = A[7]+B[7]; D[11] = A[11]+B[11]; D[15] = A[15]+B[15];
}
NA_IDEF void naAddM22fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 4)
      naError("naAddM22fE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naAddM22fE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[2] = A[2]+B[2];
  D[1] = A[1]+B[1]; D[3] = A[3]+B[3];
}
NA_IDEF void naAddM33fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 9)
      naError("naAddM33fE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
      naError("naAddM33fE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[3] = A[3]+B[3]; D[6] = A[6]+B[6];
  D[1] = A[1]+B[1]; D[4] = A[4]+B[4]; D[7] = A[7]+B[7];
  D[2] = A[2]+B[2]; D[5] = A[5]+B[5]; D[8] = A[8]+B[8];
}
NA_IDEF void naAddM44fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 16)
      naError("naAddM44fE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
      naError("naAddM44fE", "B overlaps with D.");
  #endif
  D[0] = A[0]+B[0]; D[4] = A[4]+B[4]; D[ 8] = A[ 8]+B[ 8]; D[12] = A[12]+B[12];
  D[1] = A[1]+B[1]; D[5] = A[5]+B[5]; D[ 9] = A[ 9]+B[ 9]; D[13] = A[13]+B[13];
  D[2] = A[2]+B[2]; D[6] = A[6]+B[6]; D[10] = A[10]+B[10]; D[14] = A[14]+B[14];
  D[3] = A[3]+B[3]; D[7] = A[7]+B[7]; D[11] = A[11]+B[11]; D[15] = A[15]+B[15];
}
NA_IDEF void naAddM22dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 4)
      naError("naAddM22dS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33dS(double*  NA_RESTRICT D, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 9)
      naError("naAddM33dS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 16)
      naError("naAddM44dS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}
NA_IDEF void naAddM22fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 4)
      naError("naAddM22fS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 9)
      naError("naAddM33fS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 16)
      naError("naAddM44fS", "B overlaps with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}
NA_IDEF void naAddM22dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 4)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33dSE(double*  D, const double*  B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 9)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 16)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}
NA_IDEF void naAddM22fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 4)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[2] += B[2];
  D[1] += B[1]; D[3] += B[3];
}
NA_IDEF void naAddM33fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 9)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[3] += B[3]; D[6] += B[6];
  D[1] += B[1]; D[4] += B[4]; D[7] += B[7];
  D[2] += B[2]; D[5] += B[5]; D[8] += B[8];
}
NA_IDEF void naAddM44fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 16)
      naError("naAddM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] += B[0]; D[4] += B[4]; D[ 8] += B[ 8]; D[12] += B[12];
  D[1] += B[1]; D[5] += B[5]; D[ 9] += B[ 9]; D[13] += B[13];
  D[2] += B[2]; D[6] += B[6]; D[10] += B[10]; D[14] += B[14];
  D[3] += B[3]; D[7] += B[7]; D[11] += B[11]; D[15] += B[15];
}




// //////////////////////////////////////
// Sub
// //////////////////////////////////////

NA_IDEF void naSubM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM22d", "A equals D. Use S or E variant");
    else if(naAbsi(D-A) < 4)
      naError("naSubM22d", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naSubM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM33d", "A equals D. Use S or E variant");
    else if(naAbsi(D-A) < 9)
      naError("naSubM33d", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
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
    else if(naAbsi(D-A) < 16)
      naError("naSubM44d", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
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
    else if(naAbsi(D-A) < 4)
      naError("naSubM22f", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naSubM22f", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A==D)
      naError("naSubM33f", "A equals D. Use S or E variant");
    else if(naAbsi(D-A) < 9)
      naError("naSubM33f", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
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
    else if(naAbsi(D-A) < 16)
      naError("naSubM44f", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
      naError("naSubM44f", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[4] = A[4]-B[4]; D[ 8] = A[ 8]-B[ 8]; D[12] = A[12]-B[12];
  D[1] = A[1]-B[1]; D[5] = A[5]-B[5]; D[ 9] = A[ 9]-B[ 9]; D[13] = A[13]-B[13];
  D[2] = A[2]-B[2]; D[6] = A[6]-B[6]; D[10] = A[10]-B[10]; D[14] = A[14]-B[14];
  D[3] = A[3]-B[3]; D[7] = A[7]-B[7]; D[11] = A[11]-B[11]; D[15] = A[15]-B[15];
}
NA_IDEF void naSubM22dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 4)
      naError("naSubM22dE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naSubM22dE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33dE(double*  D, const double*  A, const double*  B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 9)
      naError("naSubM33dE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
      naError("naSubM33dE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[3] = A[3]-B[3]; D[6] = A[6]-B[6];
  D[1] = A[1]-B[1]; D[4] = A[4]-B[4]; D[7] = A[7]-B[7];
  D[2] = A[2]-B[2]; D[5] = A[5]-B[5]; D[8] = A[8]-B[8];
}
NA_IDEF void naSubM44dE(double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 16)
      naError("naSubM44dE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
      naError("naSubM44dE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[4] = A[4]-B[4]; D[ 8] = A[ 8]-B[ 8]; D[12] = A[12]-B[12];
  D[1] = A[1]-B[1]; D[5] = A[5]-B[5]; D[ 9] = A[ 9]-B[ 9]; D[13] = A[13]-B[13];
  D[2] = A[2]-B[2]; D[6] = A[6]-B[6]; D[10] = A[10]-B[10]; D[14] = A[14]-B[14];
  D[3] = A[3]-B[3]; D[7] = A[7]-B[7]; D[11] = A[11]-B[11]; D[15] = A[15]-B[15];
}
NA_IDEF void naSubM22fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 4)
      naError("naSubM22fE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 4)
      naError("naSubM22fE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[2] = A[2]-B[2];
  D[1] = A[1]-B[1]; D[3] = A[3]-B[3];
}
NA_IDEF void naSubM33fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 9)
      naError("naSubM33fE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 9)
      naError("naSubM33fE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[3] = A[3]-B[3]; D[6] = A[6]-B[6];
  D[1] = A[1]-B[1]; D[4] = A[4]-B[4]; D[7] = A[7]-B[7];
  D[2] = A[2]-B[2]; D[5] = A[5]-B[5]; D[8] = A[8]-B[8];
}
NA_IDEF void naSubM44fE(float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((D != A) && naAbsi(D-A) < 16)
      naError("naSubM44fE", "A overlaps misaligned with D.");
    if(naAbsi(B-D) < 16)
      naError("naSubM44fE", "B overlaps with D.");
  #endif
  D[0] = A[0]-B[0]; D[4] = A[4]-B[4]; D[ 8] = A[ 8]-B[ 8]; D[12] = A[12]-B[12];
  D[1] = A[1]-B[1]; D[5] = A[5]-B[5]; D[ 9] = A[ 9]-B[ 9]; D[13] = A[13]-B[13];
  D[2] = A[2]-B[2]; D[6] = A[6]-B[6]; D[10] = A[10]-B[10]; D[14] = A[14]-B[14];
  D[3] = A[3]-B[3]; D[7] = A[7]-B[7]; D[11] = A[11]-B[11]; D[15] = A[15]-B[15];
}
NA_IDEF void naSubM22dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 4)
      naError("naSubM22dS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33dS(double*  NA_RESTRICT D, const double*  NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 9)
      naError("naSubM33dS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44dS(double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 16)
      naError("naSubM44dS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
}
NA_IDEF void naSubM22fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 4)
      naError("naSubM22fS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 9)
      naError("naSubM33fS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44fS(float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(B-D) < 16)
      naError("naSubM44fS", "B overlaps with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
}
NA_IDEF void naSubM22dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 4)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33dSE(double*  D, const double*  B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 9)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44dSE(double* D, const double* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 16)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
}
NA_IDEF void naSubM22fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 4)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[2] -= B[2];
  D[1] -= B[1]; D[3] -= B[3];
}
NA_IDEF void naSubM33fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 9)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[3] -= B[3]; D[6] -= B[6];
  D[1] -= B[1]; D[4] -= B[4]; D[7] -= B[7];
  D[2] -= B[2]; D[5] -= B[5]; D[8] -= B[8];
}
NA_IDEF void naSubM44fSE(float* D, const float* B){
  #ifndef NDEBUG
    if((B != D) && naAbsi(B-D) < 16)
      naError("naSubM22dSE", "B overlaps misaligned with D.");
  #endif
  D[0] -= B[0]; D[4] -= B[4]; D[ 8] -= B[ 8]; D[12] -= B[12];
  D[1] -= B[1]; D[5] -= B[5]; D[ 9] -= B[ 9]; D[13] -= B[13];
  D[2] -= B[2]; D[6] -= B[6]; D[10] -= B[10]; D[14] -= B[14];
  D[3] -= B[3]; D[7] -= B[7]; D[11] -= B[11]; D[15] -= B[15];
}



// ///////////////////////////////////////
// Scale with scalar
// ///////////////////////////////////////

NA_IDEF void naScaleM22d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM22d", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 4)
      naError("naScaleM22d", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naScaleM33d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM33d", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 9)
      naError("naScaleM33d", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naScaleM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM44d", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 16)
      naError("naScaleM44d", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naScaleM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM22f", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 4)
      naError("naScaleM22f", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naScaleM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM33f", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 9)
      naError("naScaleM33f", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naScaleM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM44f", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 16)
      naError("naScaleM44f", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naScaleM22dE(double*  D, const double*  A, double s){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 4)
      naError("naScaleM22dE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naScaleM33dE(double*  D, const double*  A, double s){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 9)
      naError("naScaleM33dE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naScaleM44dE(double* D, const double* A, double s){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 16)
      naError("naScaleM44dE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naScaleM22fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 4)
      naError("naScaleM22fE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naScaleM33fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 9)
      naError("naScaleM33fE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naScaleM44fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 16)
      naError("naScaleM44fE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naScaleM22dS(double*  D, double s){
  D[0] *= s; D[2] *= s;
  D[1] *= s; D[3] *= s;
}
NA_IDEF void naScaleM33dS(double*  D, double s){
  D[0] *= s; D[3] *= s; D[6] *= s;
  D[1] *= s; D[4] *= s; D[7] *= s;
  D[2] *= s; D[5] *= s; D[8] *= s;
}
NA_IDEF void naScaleM44dS(double* D, double s){
  D[0] *= s; D[4] *= s; D[ 8] *= s; D[12] *= s;
  D[1] *= s; D[5] *= s; D[ 9] *= s; D[13] *= s;
  D[2] *= s; D[6] *= s; D[10] *= s; D[14] *= s;
  D[3] *= s; D[7] *= s; D[11] *= s; D[15] *= s;
}
NA_IDEF void naScaleM22fS(float* D, float s){
  D[0] *= s; D[2] *= s;
  D[1] *= s; D[3] *= s;
}
NA_IDEF void naScaleM33fS(float* D, float s){
  D[0] *= s; D[3] *= s; D[6] *= s;
  D[1] *= s; D[4] *= s; D[7] *= s;
  D[2] *= s; D[5] *= s; D[8] *= s;
}
NA_IDEF void naScaleM44fS(float* D, float s){
  D[0] *= s; D[4] *= s; D[ 8] *= s; D[12] *= s;
  D[1] *= s; D[5] *= s; D[ 9] *= s; D[13] *= s;
  D[2] *= s; D[6] *= s; D[10] *= s; D[14] *= s;
  D[3] *= s; D[7] *= s; D[11] *= s; D[15] *= s;
}




// ///////////////////////////////////////
// Scale with vector
// ///////////////////////////////////////

NA_IDEF void naScaleM22dV2d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM22dV2d", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 4)
      naError("naScaleM22dV2d", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, D-v))
      naError("naScaleM22dV2d", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naScaleM33dV3d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM33dV3d", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 9)
      naError("naScaleM33dV3d", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, D-v))
      naError("naScaleM33dV3d", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naScaleM44dV4d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM44dV4d", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 16)
      naError("naScaleM44dV4d", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, D-v))
      naError("naScaleM44dV4d", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naScaleM22fV2f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM22fV2f", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 4)
      naError("naScaleM22fV2f", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, D-v))
      naError("naScaleM22fV2f", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naScaleM33fV3f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM33fV3f", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 9)
      naError("naScaleM33fV3f", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, D-v))
      naError("naScaleM33fV3f", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naScaleM44fV4f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naScaleM44fV4f", "A equals D. Use E or S variant");
    else if(naAbsi(A-D) < 16)
      naError("naScaleM44fV4f", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, D-v))
      naError("naScaleM44fV4f", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naScaleM22dV2dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 4)
      naError("naScaleM22dV2dE", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, D-v))
      naError("naScaleM22dV2dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naScaleM33dV3dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 9)
      naError("naScaleM33dV3dE", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, D-v))
      naError("naScaleM33dV3dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naScaleM44dV4dE( double* D, const double* A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 16)
      naError("naScaleM44dV4dE", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, D-v))
      naError("naScaleM44dV4dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naScaleM22fV2fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 4)
      naError("naScaleM22fV2fE", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, D-v))
      naError("naScaleM22fV2fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naScaleM33fV3fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 9)
      naError("naScaleM33fV3fE", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, D-v))
      naError("naScaleM33fV3fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naScaleM44fV4fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi(A-D) < 16)
      naError("naScaleM44fV4fE", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, D-v))
      naError("naScaleM44fV4fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naScaleM22dV2dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, D-v))
      naError("naScaleM22dV2dS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[2] *= v[1];
  D[1] *= v[0]; D[3] *= v[1];
}
NA_IDEF void naScaleM33dV3dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, D-v))
      naError("naScaleM33dV3dS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[3] *= v[1]; D[6] *= v[2];
  D[1] *= v[0]; D[4] *= v[1]; D[7] *= v[2];
  D[2] *= v[0]; D[5] *= v[1]; D[8] *= v[2];
}
NA_IDEF void naScaleM44dV4dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, D-v))
      naError("naScaleM44dV4dS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[4] *= v[1]; D[ 8] *= v[2]; D[12] *= v[3];
  D[1] *= v[0]; D[5] *= v[1]; D[ 9] *= v[2]; D[13] *= v[3];
  D[2] *= v[0]; D[6] *= v[1]; D[10] *= v[2]; D[14] *= v[3];
  D[3] *= v[0]; D[7] *= v[1]; D[11] *= v[2]; D[15] *= v[3];
}
NA_IDEF void naScaleM22fV2fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, D-v))
      naError("naScaleM22fV2fS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[2] *= v[1];
  D[1] *= v[0]; D[3] *= v[1];
}
NA_IDEF void naScaleM33fV3fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, D-v))
      naError("naScaleM33fV3fS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[3] *= v[1]; D[6] *= v[2];
  D[1] *= v[0]; D[4] *= v[1]; D[7] *= v[2];
  D[2] *= v[0]; D[5] *= v[1]; D[8] *= v[2];
}
NA_IDEF void naScaleM44fV4fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, D-v))
      naError("naScaleM44fV4fS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[4] *= v[1]; D[ 8] *= v[2]; D[12] *= v[3];
  D[1] *= v[0]; D[5] *= v[1]; D[ 9] *= v[2]; D[13] *= v[3];
  D[2] *= v[0]; D[6] *= v[1]; D[10] *= v[2]; D[14] *= v[3];
  D[3] *= v[0]; D[7] *= v[1]; D[11] *= v[2]; D[15] *= v[3];
}



// //////////////////////////////
// Multiply matrix with vector
// //////////////////////////////

NA_IDEF void naMulM22dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, A-d))
      naError("naMulM22dV2d", "A overlaps with d.");
    if(naAbsi(v-d) < 2)
      naError("naMulM22dV2d", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[2]*v[1];
  d[1] = A[1]*v[0] + A[3]*v[1];
}
NA_IDEF void naMulM33dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, A-d))
      naError("naMulM33dV3d", "A overlaps with d.");
    if(naAbsi(v-d) < 3)
      naError("naMulM33dV3d", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[3]*v[1] + A[6]*v[2];
  d[1] = A[1]*v[0] + A[4]*v[1] + A[7]*v[2];
  d[2] = A[2]*v[0] + A[5]*v[1] + A[8]*v[2];
}
NA_IDEF void naMulM44dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, A-d))
      naError("naMulM44dV4d", "A overlaps with d.");
    if(naAbsi(v-d) < 4)
      naError("naMulM44dV4d", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[4]*v[1] + A[ 8]*v[2] + A[12]*v[3];
  d[1] = A[1]*v[0] + A[5]*v[1] + A[ 9]*v[2] + A[13]*v[3];
  d[2] = A[2]*v[0] + A[6]*v[1] + A[10]*v[2] + A[14]*v[3];
  d[3] = A[3]*v[0] + A[7]*v[1] + A[11]*v[2] + A[15]*v[3];
}
NA_IDEF void naMulM22fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, A-d))
      naError("naMulM22fV2f", "A overlaps with d.");
    if(naAbsi(v-d) < 2)
      naError("naMulM22fV2f", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[2]*v[1];
  d[1] = A[1]*v[0] + A[3]*v[1];
}
NA_IDEF void naMulM33fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, A-d))
      naError("naMulM33fV3f", "A overlaps with d.");
    if(naAbsi(v-d) < 3)
      naError("naMulM33fV3f", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[3]*v[1] + A[6]*v[2];
  d[1] = A[1]*v[0] + A[4]*v[1] + A[7]*v[2];
  d[2] = A[2]*v[0] + A[5]*v[1] + A[8]*v[2];
}
NA_IDEF void naMulM44fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, A-d))
      naError("naMulM44fV4f", "A overlaps with d.");
    if(naAbsi(v-d) < 4)
      naError("naMulM44fV4f", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[4]*v[1] + A[ 8]*v[2] + A[12]*v[3];
  d[1] = A[1]*v[0] + A[5]*v[1] + A[ 9]*v[2] + A[13]*v[3];
  d[2] = A[2]*v[0] + A[6]*v[1] + A[10]*v[2] + A[14]*v[3];
  d[3] = A[3]*v[0] + A[7]*v[1] + A[11]*v[2] + A[15]*v[3];
}


// //////////////////////////////////////
// Multiply Matrix with Matrix
// //////////////////////////////////////

NA_IDEF void naMulM22dM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(D-A) < 4)
      naError("naMulM22dM22d", "A overlaps with D.");
    if(naAbsi(D-B) < 4)
      naError("naMulM22dM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[2]*B[1];
  D[1] = A[1]*B[0] + A[3]*B[1];
  D[2] = A[0]*B[2] + A[2]*B[3];
  D[3] = A[1]*B[2] + A[3]*B[3];
}
NA_IDEF void naMulM33dM33d(double* D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(D-A) < 9)
      naError("naMulM33dM33d", "A overlaps with D.");
    if(naAbsi(D-B) < 9)
      naError("naMulM22dM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[3]*B[1] + A[6]*B[2];
  D[1] = A[1]*B[0] + A[4]*B[1] + A[7]*B[2];
  D[2] = A[2]*B[0] + A[5]*B[1] + A[8]*B[2];
  D[3] = A[0]*B[3] + A[3]*B[4] + A[6]*B[5];
  D[4] = A[1]*B[3] + A[4]*B[4] + A[7]*B[5];
  D[5] = A[2]*B[3] + A[5]*B[4] + A[8]*B[5];
  D[6] = A[0]*B[6] + A[3]*B[7] + A[6]*B[8];
  D[7] = A[1]*B[6] + A[4]*B[7] + A[7]*B[8];
  D[8] = A[2]*B[6] + A[5]*B[7] + A[8]*B[8];
}
NA_IDEF void naMulM44dM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(D-A) < 16)
      naError("naMulM44dM44d", "A overlaps with D.");
    if(naAbsi(D-B) < 16)
      naError("naMulM22dM22d", "B overlaps with D.");
  #endif
  D[ 0] = A[0]*B[ 0] + A[4]*B[ 1] + A[ 8]*B[ 2] + A[12]*B[ 3];
  D[ 1] = A[1]*B[ 0] + A[5]*B[ 1] + A[ 9]*B[ 2] + A[13]*B[ 3];
  D[ 2] = A[2]*B[ 0] + A[6]*B[ 1] + A[10]*B[ 2] + A[14]*B[ 3];
  D[ 3] = A[3]*B[ 0] + A[7]*B[ 1] + A[11]*B[ 2] + A[15]*B[ 3];
  D[ 4] = A[0]*B[ 4] + A[4]*B[ 5] + A[ 8]*B[ 6] + A[12]*B[ 7];
  D[ 5] = A[1]*B[ 4] + A[5]*B[ 5] + A[ 9]*B[ 6] + A[13]*B[ 7];
  D[ 6] = A[2]*B[ 4] + A[6]*B[ 5] + A[10]*B[ 6] + A[14]*B[ 7];
  D[ 7] = A[3]*B[ 4] + A[7]*B[ 5] + A[11]*B[ 6] + A[15]*B[ 7];
  D[ 8] = A[0]*B[ 8] + A[4]*B[ 9] + A[ 8]*B[10] + A[12]*B[11];
  D[ 9] = A[1]*B[ 8] + A[5]*B[ 9] + A[ 9]*B[10] + A[13]*B[11];
  D[10] = A[2]*B[ 8] + A[6]*B[ 9] + A[10]*B[10] + A[14]*B[11];
  D[11] = A[3]*B[ 8] + A[7]*B[ 9] + A[11]*B[10] + A[15]*B[11];
  D[12] = A[0]*B[12] + A[4]*B[13] + A[ 8]*B[14] + A[12]*B[15];
  D[13] = A[1]*B[12] + A[5]*B[13] + A[ 9]*B[14] + A[13]*B[15];
  D[14] = A[2]*B[12] + A[6]*B[13] + A[10]*B[14] + A[14]*B[15];
  D[15] = A[3]*B[12] + A[7]*B[13] + A[11]*B[14] + A[15]*B[15];
}
NA_IDEF void naMulM22fM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(D-A) < 4)
      naError("naMulM22fM22f", "A overlaps with D.");
    if(naAbsi(D-B) < 4)
      naError("naMulM22dM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[2]*B[1];
  D[1] = A[1]*B[0] + A[3]*B[1];
  D[2] = A[0]*B[2] + A[2]*B[3];
  D[3] = A[1]*B[2] + A[3]*B[3];
}
NA_IDEF void naMulM33fM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(D-A) < 9)
      naError("naMulM33fM33f", "A overlaps with D.");
    if(naAbsi(D-B) < 9)
      naError("naMulM22dM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[3]*B[1] + A[6]*B[2];
  D[1] = A[1]*B[0] + A[4]*B[1] + A[7]*B[2];
  D[2] = A[2]*B[0] + A[5]*B[1] + A[8]*B[2];
  D[3] = A[0]*B[3] + A[3]*B[4] + A[6]*B[5];
  D[4] = A[1]*B[3] + A[4]*B[4] + A[7]*B[5];
  D[5] = A[2]*B[3] + A[5]*B[4] + A[8]*B[5];
  D[6] = A[0]*B[6] + A[3]*B[7] + A[6]*B[8];
  D[7] = A[1]*B[6] + A[4]*B[7] + A[7]*B[8];
  D[8] = A[2]*B[6] + A[5]*B[7] + A[8]*B[8];
}
NA_IDEF void naMulM44fM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi(D-A) < 16)
      naError("naMulM44fM44f", "A overlaps with D.");
    if(naAbsi(D-B) < 16)
      naError("naMulM22dM22d", "B overlaps with D.");
  #endif
  D[ 0] = A[0]*B[ 0] + A[4]*B[ 1] + A[ 8]*B[ 2] + A[12]*B[ 3];
  D[ 1] = A[1]*B[ 0] + A[5]*B[ 1] + A[ 9]*B[ 2] + A[13]*B[ 3];
  D[ 2] = A[2]*B[ 0] + A[6]*B[ 1] + A[10]*B[ 2] + A[14]*B[ 3];
  D[ 3] = A[3]*B[ 0] + A[7]*B[ 1] + A[11]*B[ 2] + A[15]*B[ 3];
  D[ 4] = A[0]*B[ 4] + A[4]*B[ 5] + A[ 8]*B[ 6] + A[12]*B[ 7];
  D[ 5] = A[1]*B[ 4] + A[5]*B[ 5] + A[ 9]*B[ 6] + A[13]*B[ 7];
  D[ 6] = A[2]*B[ 4] + A[6]*B[ 5] + A[10]*B[ 6] + A[14]*B[ 7];
  D[ 7] = A[3]*B[ 4] + A[7]*B[ 5] + A[11]*B[ 6] + A[15]*B[ 7];
  D[ 8] = A[0]*B[ 8] + A[4]*B[ 9] + A[ 8]*B[10] + A[12]*B[11];
  D[ 9] = A[1]*B[ 8] + A[5]*B[ 9] + A[ 9]*B[10] + A[13]*B[11];
  D[10] = A[2]*B[ 8] + A[6]*B[ 9] + A[10]*B[10] + A[14]*B[11];
  D[11] = A[3]*B[ 8] + A[7]*B[ 9] + A[11]*B[10] + A[15]*B[11];
  D[12] = A[0]*B[12] + A[4]*B[13] + A[ 8]*B[14] + A[12]*B[15];
  D[13] = A[1]*B[12] + A[5]*B[13] + A[ 9]*B[14] + A[13]*B[15];
  D[14] = A[2]*B[12] + A[6]*B[13] + A[10]*B[14] + A[14]*B[15];
  D[15] = A[3]*B[12] + A[7]*B[13] + A[11]*B[14] + A[15]*B[15];
}


// //////////////////////////////////////
// Inverting
// //////////////////////////////////////

NA_IDEF void naInvertM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
  double determinant = A[0]*A[3] - A[2]*A[1];
  double divisor = naInv(determinant);
  D[0] =    A[3] * divisor;
  D[1] =  - A[1] * divisor;
  D[2] =  - A[2] * divisor;
  D[3] =    A[0] * divisor;
}
NA_IDEF void naInvertM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
  double d0 = A[4] * A[8] - A[5] * A[7];
  double d1 = A[2] * A[7] - A[1] * A[8];
  double d2 = A[1] * A[5] - A[2] * A[4];
  double determinant = A[0]*d0 + A[3]*d1 + A[6]*d2;
  double divisor = naInv(determinant);
  D[0] = d0 * divisor;
  D[1] = d1 * divisor;
  D[2] = d2 * divisor;
  D[3] = (A[5]*A[6] - A[3]*A[8]) * divisor;
  D[4] = (A[0]*A[8] - A[2]*A[6]) * divisor;
  D[5] = (A[2]*A[3] - A[0]*A[5]) * divisor;
  D[6] = (A[3]*A[7] - A[4]*A[6]) * divisor;
  D[7] = (A[1]*A[6] - A[0]*A[7]) * divisor;
  D[8] = (A[0]*A[4] - A[1]*A[3]) * divisor;
}
NA_IDEF void naInvertM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
  double a01 = A[10]*A[15] - A[11]*A[14];
  double a02 = A[ 6]*A[15] - A[ 7]*A[14];
  double a03 = A[ 6]*A[11] - A[ 7]*A[10];
  double a04 = A[ 2]*A[15] - A[ 3]*A[14];
  double a05 = A[ 2]*A[11] - A[ 3]*A[10];
  double a06 = A[ 2]*A[ 7] - A[ 3]*A[ 6];
  double d0 = + A[ 5]*a01 - A[ 9]*a02 + A[13]*a03;
  double d1 = - A[ 1]*a01 + A[ 9]*a04 - A[13]*a05;
  double d2 = + A[ 1]*a02 - A[ 5]*a04 + A[13]*a06;
  double d3 = - A[ 1]*a03 + A[ 5]*a05 - A[ 9]*a06;
  double determinant = A[ 0]*d0 + A[ 4]*d1 + A[ 8]*d2 + A[12]*d3;
  double divisor = naInv(determinant);
  double a07 = A[ 9]*A[15] - A[11]*A[13];
  double a08 = A[ 5]*A[15] - A[ 7]*A[13];
  double a09 = A[ 5]*A[11] - A[ 7]*A[ 9];
  double a10 = A[ 1]*A[15] - A[ 3]*A[13];
  double a11 = A[ 1]*A[11] - A[ 3]*A[ 9];
  double a12 = A[ 1]*A[ 7] - A[ 3]*A[ 5];
  double a13 = A[ 9]*A[14] - A[10]*A[13];
  double a14 = A[ 5]*A[14] - A[ 6]*A[13];
  double a15 = A[ 5]*A[10] - A[ 6]*A[ 9];
  double a16 = A[ 1]*A[14] - A[ 2]*A[13];
  double a17 = A[ 1]*A[10] - A[ 2]*A[ 9];
  double a18 = A[ 1]*A[ 6] - A[ 2]*A[ 5];
  D[ 0] = d0 * divisor;
  D[ 1] = d1 * divisor;
  D[ 2] = d2 * divisor;
  D[ 3] = d3 * divisor;
  D[ 4] = ( - A[ 4]*a01 + A[ 8]*a02 - A[12]*a03) * divisor;
  D[ 5] = ( + A[ 0]*a01 - A[ 8]*a04 + A[12]*a05) * divisor;
  D[ 6] = ( - A[ 0]*a02 + A[ 4]*a04 - A[12]*a06) * divisor;
  D[ 7] = ( + A[ 0]*a03 - A[ 4]*a05 + A[ 8]*a06) * divisor;
  D[ 8] = ( + A[ 4]*a07 - A[ 8]*a08 + A[12]*a09) * divisor;
  D[ 9] = ( - A[ 0]*a07 + A[ 8]*a10 - A[12]*a11) * divisor;
  D[10] = ( + A[ 0]*a08 - A[ 4]*a10 + A[12]*a12) * divisor;
  D[11] = ( - A[ 0]*a09 + A[ 4]*a11 - A[ 8]*a12) * divisor;
  D[12] = ( - A[ 4]*a13 + A[ 8]*a14 - A[12]*a15) * divisor;
  D[13] = ( + A[ 0]*a13 - A[ 8]*a16 + A[12]*a17) * divisor;
  D[14] = ( - A[ 0]*a14 + A[ 4]*a16 - A[12]*a18) * divisor;
  D[15] = ( + A[ 0]*a15 - A[ 4]*a17 + A[ 8]*a18) * divisor;
}
NA_IDEF void naInvertM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
  float determinant = A[0]*A[3] - A[2]*A[1];
  float divisor = naInvf(determinant);
  D[0] =    A[3] * divisor;
  D[1] =  - A[1] * divisor;
  D[2] =  - A[2] * divisor;
  D[3] =    A[0] * divisor;
}
NA_IDEF void naInvertM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
  float d0 = A[4] * A[8] - A[5] * A[7];
  float d1 = A[2] * A[7] - A[1] * A[8];
  float d2 = A[1] * A[5] - A[2] * A[4];
  float determinant = A[0]*d0 + A[3]*d1 + A[6]*d2;
  float divisor = naInvf(determinant);
  D[0] = d0 * divisor;
  D[1] = d1 * divisor;
  D[2] = d2 * divisor;
  D[3] = (A[5]*A[6] - A[3]*A[8]) * divisor;
  D[4] = (A[0]*A[8] - A[2]*A[6]) * divisor;
  D[5] = (A[2]*A[3] - A[0]*A[5]) * divisor;
  D[6] = (A[3]*A[7] - A[4]*A[6]) * divisor;
  D[7] = (A[1]*A[6] - A[0]*A[7]) * divisor;
  D[8] = (A[0]*A[4] - A[1]*A[3]) * divisor;
}
NA_IDEF void naInvertM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
  float a01 = A[10]*A[15] - A[11]*A[14];
  float a02 = A[ 6]*A[15] - A[ 7]*A[14];
  float a03 = A[ 6]*A[11] - A[ 7]*A[10];
  float a04 = A[ 2]*A[15] - A[ 3]*A[14];
  float a05 = A[ 2]*A[11] - A[ 3]*A[10];
  float a06 = A[ 2]*A[ 7] - A[ 3]*A[ 6];
  float d0 = + A[ 5]*a01 - A[ 9]*a02 + A[13]*a03;
  float d1 = - A[ 1]*a01 + A[ 9]*a04 - A[13]*a05;
  float d2 = + A[ 1]*a02 - A[ 5]*a04 + A[13]*a06;
  float d3 = - A[ 1]*a03 + A[ 5]*a05 - A[ 9]*a06;
  float determinant = A[ 0]*d0 + A[ 4]*d1 + A[ 8]*d2 + A[12]*d3;
  float divisor = naInvf(determinant);
  float a07 = A[ 9]*A[15] - A[11]*A[13];
  float a08 = A[ 5]*A[15] - A[ 7]*A[13];
  float a09 = A[ 5]*A[11] - A[ 7]*A[ 9];
  float a10 = A[ 1]*A[15] - A[ 3]*A[13];
  float a11 = A[ 1]*A[11] - A[ 3]*A[ 9];
  float a12 = A[ 1]*A[ 7] - A[ 3]*A[ 5];
  float a13 = A[ 9]*A[14] - A[10]*A[13];
  float a14 = A[ 5]*A[14] - A[ 6]*A[13];
  float a15 = A[ 5]*A[10] - A[ 6]*A[ 9];
  float a16 = A[ 1]*A[14] - A[ 2]*A[13];
  float a17 = A[ 1]*A[10] - A[ 2]*A[ 9];
  float a18 = A[ 1]*A[ 6] - A[ 2]*A[ 5];
  D[ 0] = d0 * divisor;
  D[ 1] = d1 * divisor;
  D[ 2] = d2 * divisor;
  D[ 3] = d3 * divisor;
  D[ 4] = ( - A[ 4]*a01 + A[ 8]*a02 - A[12]*a03) * divisor;
  D[ 5] = ( + A[ 0]*a01 - A[ 8]*a04 + A[12]*a05) * divisor;
  D[ 6] = ( - A[ 0]*a02 + A[ 4]*a04 - A[12]*a06) * divisor;
  D[ 7] = ( + A[ 0]*a03 - A[ 4]*a05 + A[ 8]*a06) * divisor;
  D[ 8] = ( + A[ 4]*a07 - A[ 8]*a08 + A[12]*a09) * divisor;
  D[ 9] = ( - A[ 0]*a07 + A[ 8]*a10 - A[12]*a11) * divisor;
  D[10] = ( + A[ 0]*a08 - A[ 4]*a10 + A[12]*a12) * divisor;
  D[11] = ( - A[ 0]*a09 + A[ 4]*a11 - A[ 8]*a12) * divisor;
  D[12] = ( - A[ 4]*a13 + A[ 8]*a14 - A[12]*a15) * divisor;
  D[13] = ( + A[ 0]*a13 - A[ 8]*a16 + A[12]*a17) * divisor;
  D[14] = ( - A[ 0]*a14 + A[ 4]*a16 - A[12]*a18) * divisor;
  D[15] = ( + A[ 0]*a15 - A[ 4]*a17 + A[ 8]*a18) * divisor;
}




NA_IAPI void naFillM33dWithRotationAlongAxis(double* NA_RESTRICT d, const double* NA_RESTRICT axis, double angle){
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
  
  naFillM33dColumnFirst(d,
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


NA_IAPI void naFillM33dWithRotationAngles(double* d, double roll, double pitch, double yaw){
  double q1 = - naCos(.5 * (roll - yaw)) * naSin(.5 * pitch);
  double q2 = naSin(.5 * (roll - yaw)) * naSin(.5 * pitch);
  double q3 = - naSin(.5 * (roll + yaw)) * naCos(.5 * pitch);
  double q4 = naCos(.5 * (roll + yaw)) * naCos(.5 * pitch);

  naFillM33dColumnFirst(d,
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
