
#ifndef NA_VECTOR_ALGEBRA_INCLUDED
#define NA_VECTOR_ALGEBRA_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


#include "../NABase.h"

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
// matrices of prefedined type and byteSize:

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
// naDoStructCountTypeSomethingES
//
// na           The prefix always available in NALib functions.
// Do           A verb denoting what will be done. For example Fill or Add.
// Struct       Either a V for vector or M for matrix.
// Count        The number of elements of the struct. For example 3 for a
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
// When NA_DEBUG is 0, no checks will be executed an no warnings will be
// emitted at all.
//
// Implementation note:
// The check if two arrays are misaligned is done using pointer arithmetic.
// It is so far unknown how these checks work out when the pointers are not
// aligned to the byteSize of their element type like float or double. But most
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

// Sets all components to binary zero
NA_IAPI void naZeroV1d(double* d);
NA_IAPI void naZeroV2d(double* d);
NA_IAPI void naZeroV3d(double* d);
NA_IAPI void naZeroV4d(double* d);
NA_IAPI void naZeroV1f(float*  d);
NA_IAPI void naZeroV2f(float*  d);
NA_IAPI void naZeroV3f(float*  d);
NA_IAPI void naZeroV4f(float*  d);
NA_IAPI void naZeroV1i(NAInt*  d);
NA_IAPI void naZeroV2i(NAInt*  d);
NA_IAPI void naZeroV3i(NAInt*  d);
NA_IAPI void naZeroV4i(NAInt*  d);

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

// Inverts the vector componentwise. d = 1 ./ s
// E-variant allow the pointers to be equal without warning.
// S-variant computes d /= 1. / d
NA_IAPI void naInvCompV1d (double* NA_RESTRICT d, const double*  NA_RESTRICT s);
NA_IAPI void naInvCompV2d (double* NA_RESTRICT d, const double*  NA_RESTRICT s);
NA_IAPI void naInvCompV3d (double* NA_RESTRICT d, const double*  NA_RESTRICT s);
NA_IAPI void naInvCompV4d (double* NA_RESTRICT d, const double*  NA_RESTRICT s);
NA_IAPI void naInvCompV1f (float*  NA_RESTRICT d, const float*   NA_RESTRICT s);
NA_IAPI void naInvCompV2f (float*  NA_RESTRICT d, const float*   NA_RESTRICT s);
NA_IAPI void naInvCompV3f (float*  NA_RESTRICT d, const float*   NA_RESTRICT s);
NA_IAPI void naInvCompV4f (float*  NA_RESTRICT d, const float*   NA_RESTRICT s);
NA_IAPI void naInvCompV1dE(double* d, double* s);
NA_IAPI void naInvCompV2dE(double* d, double* s);
NA_IAPI void naInvCompV3dE(double* d, double* s);
NA_IAPI void naInvCompV4dE(double* d, double* s);
NA_IAPI void naInvCompV1fE(float*  d, float*  s);
NA_IAPI void naInvCompV2fE(float*  d, float*  s);
NA_IAPI void naInvCompV3fE(float*  d, float*  s);
NA_IAPI void naInvCompV4fE(float*  d, float*  s);
NA_IAPI void naInvCompV1dS(double* d);
NA_IAPI void naInvCompV2dS(double* d);
NA_IAPI void naInvCompV3dS(double* d);
NA_IAPI void naInvCompV4dS(double* d);
NA_IAPI void naInvCompV1fS(float*  d);
NA_IAPI void naInvCompV2fS(float*  d);
NA_IAPI void naInvCompV3fS(float*  d);
NA_IAPI void naInvCompV4fS(float*  d);

// Multiplies two vectors componentwise. d = a .* b
// E-variant allow the pointers to be equal without warning.
// S-variant computes d ./= b
// SE-variant will not warn about pointers being equal.
NA_IAPI void naMulCompV1dV1d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV2dV2d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV3dV3d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV4dV4d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV1fV1f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV2fV2f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV3fV3f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV4fV4f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV1dV1dE (double* d, const double* a, const double* b);
NA_IAPI void naMulCompV2dV2dE (double* d, const double* a, const double* b);
NA_IAPI void naMulCompV3dV3dE (double* d, const double* a, const double* b);
NA_IAPI void naMulCompV4dV4dE (double* d, const double* a, const double* b);
NA_IAPI void naMulCompV1fV1fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naMulCompV2fV2fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naMulCompV3fV3fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naMulCompV4fV4fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naMulCompV1dV1dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV2dV2dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV3dV3dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV4dV4dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naMulCompV1fV1fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV2fV2fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV3fV3fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV4fV4fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naMulCompV1dV1dSE(double* d, const double* b);
NA_IAPI void naMulCompV2dV2dSE(double* d, const double* b);
NA_IAPI void naMulCompV3dV3dSE(double* d, const double* b);
NA_IAPI void naMulCompV4dV4dSE(double* d, const double* b);
NA_IAPI void naMulCompV1fV1fSE(float*  d, const float*  b);
NA_IAPI void naMulCompV2fV2fSE(float*  d, const float*  b);
NA_IAPI void naMulCompV3fV3fSE(float*  d, const float*  b);
NA_IAPI void naMulCompV4fV4fSE(float*  d, const float*  b);

// Divides two vectors componentwise. d = a ./ b
// E-variant allow the pointers to be equal without warning.
// S-variant computes d ./= b
// SE-variant will not warn about pointers being equal.
NA_IAPI void naDivCompV1dV1d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV2dV2d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV3dV3d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV4dV4d  (double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV1fV1f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV2fV2f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV3fV3f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV4fV4f  (float*  NA_RESTRICT d, const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV1dV1dE (double* d, const double* a, const double* b);
NA_IAPI void naDivCompV2dV2dE (double* d, const double* a, const double* b);
NA_IAPI void naDivCompV3dV3dE (double* d, const double* a, const double* b);
NA_IAPI void naDivCompV4dV4dE (double* d, const double* a, const double* b);
NA_IAPI void naDivCompV1fV1fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivCompV2fV2fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivCompV3fV3fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivCompV4fV4fE (float*  d, const float*  a, const float*  b);
NA_IAPI void naDivCompV1dV1dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV2dV2dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV3dV3dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV4dV4dS (double* NA_RESTRICT d, const double* NA_RESTRICT b);
NA_IAPI void naDivCompV1fV1fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV2fV2fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV3fV3fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV4fV4fS (float*  NA_RESTRICT d, const float*  NA_RESTRICT b);
NA_IAPI void naDivCompV1dV1dSE(double* d, const double* b);
NA_IAPI void naDivCompV2dV2dSE(double* d, const double* b);
NA_IAPI void naDivCompV3dV3dSE(double* d, const double* b);
NA_IAPI void naDivCompV4dV4dSE(double* d, const double* b);
NA_IAPI void naDivCompV1fV1fSE(float*  d, const float*  b);
NA_IAPI void naDivCompV2fV2fSE(float*  d, const float*  b);
NA_IAPI void naDivCompV3fV3fSE(float*  d, const float*  b);
NA_IAPI void naDivCompV4fV4fSE(float*  d, const float*  b);

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
// The S-variant normalizes the given vector itself.
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

// Inverts the matrix componentwise: D = 1 ./ S
// E variant allows S to be the same as D
// S variant computes D = 1 ./ D
NA_IAPI void naInvCompM22d (double* NA_RESTRICT D, const double* NA_RESTRICT S);
NA_IAPI void naInvCompM33d (double* NA_RESTRICT D, const double* NA_RESTRICT S);
NA_IAPI void naInvCompM44d (double* NA_RESTRICT D, const double* NA_RESTRICT S);
NA_IAPI void naInvCompM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S);
NA_IAPI void naInvCompM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S);
NA_IAPI void naInvCompM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S);
NA_IAPI void naInvCompM22dE(double* D, const double* S);
NA_IAPI void naInvCompM33dE(double* D, const double* S);
NA_IAPI void naInvCompM44dE(double* D, const double* S);
NA_IAPI void naInvCompM22fE(float*  D, const float*  S);
NA_IAPI void naInvCompM33fE(float*  D, const float*  S);
NA_IAPI void naInvCompM44fE(float*  D, const float*  S);
NA_IAPI void naInvCompM22dS(double* D);
NA_IAPI void naInvCompM33dS(double* D);
NA_IAPI void naInvCompM44dS(double* D);
NA_IAPI void naInvCompM22fS(float*  D);
NA_IAPI void naInvCompM33fS(float*  D);
NA_IAPI void naInvCompM44fS(float*  D);

// Multiplies the matrix componentwise with the scalar: D = A * s
// E variant allows A to be the same as D
// S variant computes D *= s
NA_IAPI void naMulCompM22d (double* NA_RESTRICT D, const double* NA_RESTRICT A, double s);
NA_IAPI void naMulCompM33d (double* NA_RESTRICT D, const double* NA_RESTRICT A, double s);
NA_IAPI void naMulCompM44d (double* NA_RESTRICT D, const double* NA_RESTRICT A, double s);
NA_IAPI void naMulCompM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, float  s);
NA_IAPI void naMulCompM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, float  s);
NA_IAPI void naMulCompM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, float  s);
NA_IAPI void naMulCompM22dE(double* D, const double* A, double s);
NA_IAPI void naMulCompM33dE(double* D, const double* A, double s);
NA_IAPI void naMulCompM44dE(double* D, const double* A, double s);
NA_IAPI void naMulCompM22fE(float*  D, const float*  A, float  s);
NA_IAPI void naMulCompM33fE(float*  D, const float*  A, float  s);
NA_IAPI void naMulCompM44fE(float*  D, const float*  A, float  s);
NA_IAPI void naMulCompM22dS(double* D, double s);
NA_IAPI void naMulCompM33dS(double* D, double s);
NA_IAPI void naMulCompM44dS(double* D, double s);
NA_IAPI void naMulCompM22fS(float*  D, float  s);
NA_IAPI void naMulCompM33fS(float*  D, float  s);
NA_IAPI void naMulCompM44fS(float*  D, float  s);

// Multiplies the column vectors of matrix m componentwise with vector v:
// D = A * diag(v)
// E variant allows A to be the same as D
// S variant computes D *= diag(v)
NA_IAPI void naMulCompM22dV2d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naMulCompM33dV3d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naMulCompM44dV4d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naMulCompM22fV2f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naMulCompM33fV3f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naMulCompM44fV4f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naMulCompM22dV2dE(double* D, const double* A, const double* v);
NA_IAPI void naMulCompM33dV3dE(double* D, const double* A, const double* v);
NA_IAPI void naMulCompM44dV4dE(double* D, const double* A, const double* v);
NA_IAPI void naMulCompM22fV2fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naMulCompM33fV3fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naMulCompM44fV4fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naMulCompM22dV2dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naMulCompM33dV3dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naMulCompM44dV4dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naMulCompM22fV2fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);
NA_IAPI void naMulCompM33fV3fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);
NA_IAPI void naMulCompM44fV4fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);

// Divides the column vectors of matrix m componentwise with vector v:
// D = A * diag(v.^-1)
// E variant allows A to be the same as D
// S variant computes D *= diag(v)
NA_IAPI void naDivCompM22dV2d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naDivCompM33dV3d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naDivCompM44dV4d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v);
NA_IAPI void naDivCompM22fV2f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naDivCompM33fV3f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naDivCompM44fV4f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT v);
NA_IAPI void naDivCompM22dV2dE(double* D, const double* A, const double* v);
NA_IAPI void naDivCompM33dV3dE(double* D, const double* A, const double* v);
NA_IAPI void naDivCompM44dV4dE(double* D, const double* A, const double* v);
NA_IAPI void naDivCompM22fV2fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naDivCompM33fV3fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naDivCompM44fV4fE(float*  D, const float*  A, const float*  v);
NA_IAPI void naDivCompM22dV2dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naDivCompM33dV3dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naDivCompM44dV4dS(double* NA_RESTRICT D, const double* NA_RESTRICT v);
NA_IAPI void naDivCompM22fV2fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);
NA_IAPI void naDivCompM33fV3fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);
NA_IAPI void naDivCompM44fV4fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT v);

// Multiplies all elements of matrix A componentwise with matrix B:
// D = A .* B
// E variant allows A to be the same as D
// S variant computes D .*= B
NA_IAPI void naMulCompM22dM22d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naMulCompM33dM33d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naMulCompM44dM44d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naMulCompM22fM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naMulCompM33fM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naMulCompM44fM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naMulCompM22dM22dE(double* D, const double* A, const double* B);
NA_IAPI void naMulCompM33dM33dE(double* D, const double* A, const double* B);
NA_IAPI void naMulCompM44dM44dE(double* D, const double* A, const double* B);
NA_IAPI void naMulCompM22fM22fE(float*  D, const float*  A, const float*  B);
NA_IAPI void naMulCompM33fM33fE(float*  D, const float*  A, const float*  B);
NA_IAPI void naMulCompM44fM44fE(float*  D, const float*  A, const float*  B);
NA_IAPI void naMulCompM22dM22dS(double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naMulCompM33dM33dS(double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naMulCompM44dM44dS(double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naMulCompM22fM22fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naMulCompM33fM33fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naMulCompM44fM44fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT B);

// Divides all elements of matrix A componentwise with matrix B:
// D = A ./ B
// E variant allows A to be the same as D
// S variant computes D ./= B
NA_IAPI void naDivCompM22dM22d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naDivCompM33dM33d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naDivCompM44dM44d (double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B);
NA_IAPI void naDivCompM22fM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naDivCompM33fM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naDivCompM44fM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT A, const float*  NA_RESTRICT B);
NA_IAPI void naDivCompM22dM22dE(double* D, const double* A, const double* B);
NA_IAPI void naDivCompM33dM33dE(double* D, const double* A, const double* B);
NA_IAPI void naDivCompM44dM44dE(double* D, const double* A, const double* B);
NA_IAPI void naDivCompM22fM22fE(float*  D, const float*  A, const float*  B);
NA_IAPI void naDivCompM33fM33fE(float*  D, const float*  A, const float*  B);
NA_IAPI void naDivCompM44fM44fE(float*  D, const float*  A, const float*  B);
NA_IAPI void naDivCompM22dM22dS(double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naDivCompM33dM33dS(double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naDivCompM44dM44dS(double* NA_RESTRICT D, const double* NA_RESTRICT B);
NA_IAPI void naDivCompM22fM22fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naDivCompM33fM33fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT B);
NA_IAPI void naDivCompM44fM44fS(float*  NA_RESTRICT D, const float*  NA_RESTRICT B);


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

// Returns true if the two matrices are equal
// E-variant allow the pointers to be equal without warning.
NA_IAPI NABool naEqualM11d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualM22d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualM33d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualM44d (const double* NA_RESTRICT a, const double* NA_RESTRICT b);
NA_IAPI NABool naEqualM11f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualM22f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualM33f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualM44f (const float*  NA_RESTRICT a, const float*  NA_RESTRICT b);
NA_IAPI NABool naEqualM11dE(const double* a, const double* b);
NA_IAPI NABool naEqualM22dE(const double* a, const double* b);
NA_IAPI NABool naEqualM33dE(const double* a, const double* b);
NA_IAPI NABool naEqualM44dE(const double* a, const double* b);
NA_IAPI NABool naEqualM11fE(const float*  a, const float*  b);
NA_IAPI NABool naEqualM22fE(const float*  a, const float*  b);
NA_IAPI NABool naEqualM33fE(const float*  a, const float*  b);
NA_IAPI NABool naEqualM44fE(const float*  a, const float*  b);

// Computes the determinant of a matrix.
NA_IAPI double naDetM22d(double* A);
NA_IAPI double naDetM33d(double* A);
NA_IAPI double naDetM44d(double* A);

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

// Computes the inverse of a matrix
// No E or S variants exist.
NA_IAPI void naInvM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naInvM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naInvM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A);
NA_IAPI void naInvM22f(float*  NA_RESTRICT D, const float*  NA_RESTRICT A);
NA_IAPI void naInvM33f(float*  NA_RESTRICT D, const float*  NA_RESTRICT A);
NA_IAPI void naInvM44f(float*  NA_RESTRICT D, const float*  NA_RESTRICT A);

// Homogeneous Coordinates Scale
NA_IAPI void naScaleM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2);
NA_IAPI void naScaleM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2, double s3);
NA_IAPI void naScaleM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2);
NA_IAPI void naScaleM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2, float s3);
NA_IAPI void naScaleM33dS(double* D, double s1, double s2);
NA_IAPI void naScaleM44dS(double* D, double s1, double s2, double s3);
NA_IAPI void naScaleM33fS(float* D, float s1, float s2);
NA_IAPI void naScaleM44fS(float* D, float s1, float s2, float s3);

// Homogeneous Coordinates Translate
NA_IAPI void naTranslateM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2);
NA_IAPI void naTranslateM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2, double s3);
NA_IAPI void naTranslateM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2);
NA_IAPI void naTranslateM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s1, float s2, float s3);
NA_IAPI void naTranslateM33dS(double* D, double s1, double s2);
NA_IAPI void naTranslateM44dS(double* D, double s1, double s2, double s3);
NA_IAPI void naTranslateM33fS(float* D, float s1, float s2);
NA_IAPI void naTranslateM44fS(float* D, float s1, float s2, float s3);

NA_IAPI void naFillV2WithM33Origin(double* NA_RESTRICT d, const double* NA_RESTRICT A);
NA_IAPI void naFillV2WithM33Coord2(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v);

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
NA_IAPI void naFillM33fWithRotationAlongAxis(  float* NA_RESTRICT d,
                                        const float* NA_RESTRICT axis,
                                               float angle);
NA_IAPI void naFillM33dWithRotationAngles(     double* d,
                                               double roll,
                                               double pitch,
                                               double yaw);
NA_IAPI void naFillM33fWithRotationAngles(     float* d,
                                               float roll,
                                               float pitch,
                                               float yaw);






// Inline implementations are in a separate file:
#include "Core/NAVectorAlgebra/NAVectorAlgebraII.h"





#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_VECTOR_ALGEBRA_INCLUDED



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
