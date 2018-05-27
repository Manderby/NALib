
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebra.h"



// /////////////////////////////////////
// Warning: This is one huge file!
//
// Vector algebra functions are not especially difficult to understand or
// implement but take a lot of code writing. You are free to explore all the
// inline code underneath the API but there really isn't much to see.
// /////////////////////////////////////



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
    norm = naDotV2dE(d, d);
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is very rare.
  if(!filled){naDivV2dS(d, naSqrt(norm));}
}
NA_IDEF void naFillV3dRandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    d[2] = naUniformRandZI() * 2. - 1.;
    norm = naDotV3dE(d, d);
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is extremely! rare.
  if(!filled){naDivV3dS(d, naSqrt(norm));}
}
NA_IDEF void naFillV4dRandomSpherical(double* d, NABool filled){
  double norm;
  do{
    d[0] = naUniformRandZI() * 2. - 1.;
    d[1] = naUniformRandZI() * 2. - 1.;
    d[2] = naUniformRandZI() * 2. - 1.;
    d[3] = naUniformRandZI() * 2. - 1.;
    norm = naDotV4dE(d, d);
    // Note that with more and more dimensions it becomes more and more likely
    // that this do-while loop must be repeated many times. Up to 3 dimensions,
    // this method works quite well but starting with 4 dimensions, one should
    // really consider a different method. The author did not because he is
    // lazy.
  }while((norm > 1.) || (norm == 0.));
  // note that test case (norm == 0) is EXORBITANTLY rare.
  if(!filled){naDivV4dS(d, naSqrt(norm));}
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
    norm = naDotV2fE(d, d);
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is very rare.
  if(!filled){naDivV2fS(d, naSqrtf(norm));}
}
NA_IDEF void naFillV3fRandomSpherical(float* d, NABool filled){
  float norm;
  do{
    d[0] = naUniformRandZIf() * 2.f - 1.f;
    d[1] = naUniformRandZIf() * 2.f - 1.f;
    d[2] = naUniformRandZIf() * 2.f - 1.f;
    norm = naDotV3fE(d, d);
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is extremely! rare.
  if(!filled){naDivV3fS(d, naSqrtf(norm));}
}
NA_IDEF void naFillV4fRandomSpherical(float* d, NABool filled){
  float norm;
  do{
    d[0] = naUniformRandZIf() * 2.f - 1.f;
    d[1] = naUniformRandZIf() * 2.f - 1.f;
    d[2] = naUniformRandZIf() * 2.f - 1.f;
    d[3] = naUniformRandZIf() * 2.f - 1.f;
    norm = naDotV4fE(d, d);
    // Note that with more and more dimensions it becomes more and more likely
    // that this do-while loop must be repeated many times. Up to 3 dimensions,
    // this method works quite well but starting with 4 dimensions, one should
    // really consider a different method. The author did not because he is
    // lazy.
  }while((norm > 1.f) || (norm == 0.f));
  // note that test case (norm == 0) is EXORBITANTLY rare.
  if(!filled){naDivV4fS(d, naSqrtf(norm));}
}



NA_IDEF void naCopyV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV1d", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naCopyV1d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0];
}
NA_IDEF void naCopyV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV2d", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naCopyV2d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV3d", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naCopyV3d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV4d", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naCopyV4d", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV1f", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naCopyV1f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0];
}
NA_IDEF void naCopyV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV2f", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naCopyV2f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV3f", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naCopyV3f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naCopyV4f", "Pointers are equal. Use E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naCopyV4f", "Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1dE (double* d, const double* a){
  d[0] = a[0];
}
NA_IDEF void naCopyV2dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naCopyV2dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naCopyV3dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naCopyV4dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1fE(float*  d, const float*  a){
  d[0] = a[0];
}
NA_IDEF void naCopyV2fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naCopyV2fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naCopyV3fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naCopyV4fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}



NA_IDEF void naNegV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV1d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naNegV1d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV2d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naNegV2d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV3d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naNegV3d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV4d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naNegV4d", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV1d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naNegV1f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0];
}
NA_IDEF void naNegV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV2d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naNegV2f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV3d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naNegV3f", "Pointers overlap misaligned");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #ifndef NDEBUG
    if(d == a)
      naError("naNegV4d", "Pointers are equal. Use S or E variant.");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naNegV4f", "Pointers overlap misaligned.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1dE (double* d, const double* a){
  d[0] = -a[0];
}
NA_IDEF void naNegV2dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naNegV2dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naNegV3dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4dE (double* d, const double* a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naNegV4dE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2]; d[3] = -a[3];
}
NA_IDEF void naNegV1fE(float*  d, const float*  a){
  d[0] = -a[0];
}
NA_IDEF void naNegV2fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naNegV2fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1];
}
NA_IDEF void naNegV3fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naNegV3fE", "Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = -a[0]; d[1] = -a[1]; d[2] = -a[2];
}
NA_IDEF void naNegV4fE(float*  d, const float*  a){
  #ifndef NDEBUG
    if((d != a) && (naAbsi((NASizeInt)(d-a)) < 4))
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naAddV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naAddV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naAddV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naAddV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naAddV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naAddV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naAddV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
}
NA_IDEF void naAddV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naAddV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naAddV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naAddV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naAddV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
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
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naAddV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naAddV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naAddV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naAddV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
}
NA_IDEF void naAddV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naAddV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] + b[0];
  d[1] = a[1] + b[1];
  d[2] = a[2] + b[2];
}
NA_IDEF void naAddV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naAddV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
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
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naAddV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
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
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naAddV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naAddV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
}
NA_IDEF void naAddV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naAddV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] += b[0];
  d[1] += b[1];
  d[2] += b[2];
}
NA_IDEF void naAddV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naSubV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naSubV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naSubV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naSubV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naSubV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naSubV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naSubV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
}
NA_IDEF void naSubV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naSubV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naSubV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naSubV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naSubV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
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
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naSubV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naSubV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naSubV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naSubV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
}
NA_IDEF void naSubV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naSubV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] - b[0];
  d[1] = a[1] - b[1];
  d[2] = a[2] - b[2];
}
NA_IDEF void naSubV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naSubV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
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
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naSubV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
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
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naSubV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naSubV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
}
NA_IDEF void naSubV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naSubV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] -= b[0];
  d[1] -= b[1];
  d[2] -= b[2];
}
NA_IDEF void naSubV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulV1d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulV2d", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulV1f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
}
NA_IDEF void naMulV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulV2f", "a overlaps misaligned with d.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
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
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulV2dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulV3dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4dE(double* d, const double* a, double s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
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
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulV2fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
}
NA_IDEF void naMulV3fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulV3fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * s;
  d[1] = a[1] * s;
  d[2] = a[2] * s;
}
NA_IDEF void naMulV4fE(float*  d, const float*  a, float s){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
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
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivV1d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivV2d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivV3d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, double s){
  double div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivV4d", "a overlaps misaligned with d.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
  d[3] = a[3] * div;
}
NA_IDEF void naDivV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivV1f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
}
NA_IDEF void naDivV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivV2f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivV3f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, float s){
  float div;
  #ifndef NDEBUG
    if(a==d)
      naError("naDivV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivV4f", "a overlaps misaligned with d.");
  #endif
  div = naInvf(s);
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
  double div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivV2dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3dE(double* d, const double* a, double s){
  double div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivV3dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInv(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4dE(double* d, const double* a, double s){
  double div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivV4dE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInv(s);
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
  float div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivV2fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
}
NA_IDEF void naDivV3fE(float*  d, const float*  a, float s){
  float div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivV3fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInvf(s);
  d[0] = a[0] * div;
  d[1] = a[1] * div;
  d[2] = a[2] * div;
}
NA_IDEF void naDivV4fE(float*  d, const float*  a, float s){
  float div;
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivV4fE", "a overlaps misaligned with d. Result undefined.");
  #endif
  div = naInvf(s);
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
// Invert componentwise
// //////////////////////////////////////////////

NA_IDEF void naInvCompV1d (double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV1d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 1)
      naError("naInvCompV1d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
}
NA_IDEF void naInvCompV2d (double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV2d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 2)
      naError("naInvCompV2d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
}
NA_IDEF void naInvCompV3d (double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV3d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 3)
      naError("naInvCompV3d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
}
NA_IDEF void naInvCompV4d (double* NA_RESTRICT d, const double*  NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV4d", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 4)
      naError("naInvCompV4d", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
  d[3] = naInv(s[3]);
}
NA_IDEF void naInvCompV1f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV1f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 1)
      naError("naInvCompV1f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
}
NA_IDEF void naInvCompV2f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV2f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 2)
      naError("naInvCompV2f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
}
NA_IDEF void naInvCompV3f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV3f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 3)
      naError("naInvCompV3f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
}
NA_IDEF void naInvCompV4f(float*  NA_RESTRICT d, const float*   NA_RESTRICT s){
  #ifndef NDEBUG
    if(s==d)
      naError("naInvCompV4f", "s equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-s)) < 4)
      naError("naInvCompV4f", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
  d[3] = naInvf(s[3]);
}
NA_IDEF void naInvCompV1dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 1))
      naError("naInvCompV1dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
}
NA_IDEF void naInvCompV2dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 2))
      naError("naInvCompV2dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
}
NA_IDEF void naInvCompV3dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 3))
      naError("naInvCompV3dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
}
NA_IDEF void naInvCompV4dE(double* d, double* s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 4))
      naError("naInvCompV4dE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInv(s[0]);
  d[1] = naInv(s[1]);
  d[2] = naInv(s[2]);
  d[3] = naInv(s[3]);
}
NA_IDEF void naInvCompV1fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 1))
      naError("naInvCompV1fE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
}
NA_IDEF void naInvCompV2fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 2))
      naError("naInvCompV2fE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
}
NA_IDEF void naInvCompV3fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 3))
      naError("naInvCompV3fE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
}
NA_IDEF void naInvCompV4fE(float*  d, float*  s){
  #ifndef NDEBUG
    if((s != d) && (naAbsi((NASizeInt)(d-s)) < 4))
      naError("naInvCompV4fE", "s overlaps misaligned with d.");
  #endif
  d[0] = naInvf(s[0]);
  d[1] = naInvf(s[1]);
  d[2] = naInvf(s[2]);
  d[3] = naInvf(s[3]);
}
NA_IDEF void naInvCompV1dS(double* d){
  d[0] = naInv(d[0]);
}
NA_IDEF void naInvCompV2dS(double* d){
  d[0] = naInv(d[0]);
  d[1] = naInv(d[1]);
}
NA_IDEF void naInvCompV3dS(double* d){
  d[0] = naInv(d[0]);
  d[1] = naInv(d[1]);
  d[2] = naInv(d[2]);
}
NA_IDEF void naInvCompV4dS(double* d){
  d[0] = naInv(d[0]);
  d[1] = naInv(d[1]);
  d[2] = naInv(d[2]);
  d[3] = naInv(d[3]);
}
NA_IDEF void naInvCompV1fS(float*  d){
  d[0] = naInvf(d[0]);
}
NA_IDEF void naInvCompV2fS(float*  d){
  d[0] = naInvf(d[0]);
  d[1] = naInvf(d[1]);
}
NA_IDEF void naInvCompV3fS(float*  d){
  d[0] = naInvf(d[0]);
  d[1] = naInvf(d[1]);
  d[2] = naInvf(d[2]);
}
NA_IDEF void naInvCompV4fS(float*  d){
  d[0] = naInvf(d[0]);
  d[1] = naInvf(d[1]);
  d[2] = naInvf(d[2]);
  d[3] = naInvf(d[3]);
}



// //////////////////////////////////////////////
// Multiply componentwise
// //////////////////////////////////////////////

NA_IDEF void naMulCompV1dV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV1dV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulCompV1dV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMulCompV1dV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV2dV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulCompV2dV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2dV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV3dV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMulCompV3dV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMulCompV3dV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV4dV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMulCompV4dV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMulCompV4dV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1fV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV1fV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMulCompV1fV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMulCompV1fV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV2fV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMulCompV2fV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2fV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV3fV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMulCompV3fV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMulCompV3fV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naMulCompV4fV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMulCompV4fV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMulCompV4fV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1dV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2dV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulCompV2dV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2dV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3dV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulCompV3dV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3dV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4dV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMulCompV4dV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4dV4dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1fV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] * b[0];
}
NA_IDEF void naMulCompV2fV2fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMulCompV2fV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2fV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
}
NA_IDEF void naMulCompV3fV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMulCompV3fV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3fV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
}
NA_IDEF void naMulCompV4fV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMulCompV4fV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4fV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * b[0];
  d[1] = a[1] * b[1];
  d[2] = a[2] * b[2];
  d[3] = a[3] * b[3];
}
NA_IDEF void naMulCompV1dV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV1dV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2dV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV2dV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3dV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV3dV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4dV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV4dV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}
NA_IDEF void naMulCompV1fV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV1fV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2fV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV2fV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3fV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV3fV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4fV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naMulCompV4fV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}
NA_IDEF void naMulCompV1dV1dSE(double* d, const double* b){
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2dV2dSE(double* d, const double* b){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMulCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3dV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4dV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}
NA_IDEF void naMulCompV1fV1fSE(float*  d, const float*  b){
  d[0] *= b[0];
}
NA_IDEF void naMulCompV2fV2fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMulCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
}
NA_IDEF void naMulCompV3fV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMulCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
}
NA_IDEF void naMulCompV4fV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMulCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= b[0];
  d[1] *= b[1];
  d[2] *= b[2];
  d[3] *= b[3];
}




// //////////////////////////////////////////////
// Divide componentwise
// //////////////////////////////////////////////

NA_IDEF void naDivCompV1dV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV1dV1d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivCompV1dV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naDivCompV1dV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV2dV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivCompV2dV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2dV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV3dV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivCompV3dV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naDivCompV3dV3d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV4dV4d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivCompV4dV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naDivCompV4dV4d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
  d[3] = a[3] * naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV1fV1f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naDivCompV1fV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naDivCompV1fV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV2fV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naDivCompV2fV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2fV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV3fV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naDivCompV3fV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naDivCompV3fV3f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a==d)
      naError("naDivCompV4fV4f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naDivCompV4fV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naDivCompV4fV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivCompV1dV1dE(double* d, const double* a, const double* b){
  d[0] = a[0] * naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivCompV2dV2dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2dV2dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivCompV3dV3dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3dV3dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivCompV4dV4dE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4dV4dE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInv(b[0]);
  d[1] = a[1] * naInv(b[1]);
  d[2] = a[2] * naInv(b[2]);
  d[3] = a[3] * naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1fE(float*  d, const float*  a, const float*  b){
  d[0] = a[0] * naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naDivCompV2fV2fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2fV2fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naDivCompV3fV3fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3fV3fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fE(float*  d, const float*  a, const float*  b){
  #ifndef NDEBUG
    if((a!=d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naDivCompV4fV4fE", "a overlaps misaligned with d. Result undefined.");
    if((b!=d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4fV4fE", "b overlaps misaligned with d. Result undefined.");
  #endif
  d[0] = a[0] * naInvf(b[0]);
  d[1] = a[1] * naInvf(b[1]);
  d[2] = a[2] * naInvf(b[2]);
  d[3] = a[3] * naInvf(b[3]);
}
NA_IDEF void naDivCompV1dV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV1dV1dS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV2dV2dS", "Pointers d and b are equal. Use SE variant");
    else if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV3dV3dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV4dV4dS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
  d[3] *= naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV1fV1fS", "Pointers d and b are equal. Use SE variant");
  #endif
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV2fV2fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV3fV3fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fS(float*  NA_RESTRICT d, const float*  NA_RESTRICT b){
  #ifndef NDEBUG
    if(d == b)
      naError("naDivCompV4fV4fS", "Pointers d and b are equal. Use SE variant");
    else if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
  d[3] *= naInvf(b[3]);
}
NA_IDEF void naDivCompV1dV1dSE(double* d, const double* b){
  d[0] *= naInv(b[0]);
}
NA_IDEF void naDivCompV2dV2dSE(double* d, const double* b){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naDivCompV2dV2dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
}
NA_IDEF void naDivCompV3dV3dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3dV3dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
}
NA_IDEF void naDivCompV4dV4dSE(double* d, const double* b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4dV4dS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInv(b[0]);
  d[1] *= naInv(b[1]);
  d[2] *= naInv(b[2]);
  d[3] *= naInv(b[3]);
}
NA_IDEF void naDivCompV1fV1fSE(float*  d, const float*  b){
  d[0] *= naInvf(b[0]);
}
NA_IDEF void naDivCompV2fV2fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naDivCompV2fV2fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
}
NA_IDEF void naDivCompV3fV3fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naDivCompV3fV3fS", "b and d are overlapping misaligned. Result undefined.");
  #endif
  d[0] *= naInvf(b[0]);
  d[1] *= naInvf(b[1]);
  d[2] *= naInvf(b[2]);
}
NA_IDEF void naDivCompV4fV4fSE(float*  d, const float*  b){
  #ifndef NDEBUG
    if((d != b) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naDivCompV4fV4fS", "b and d are overlapping misaligned. Result undefined.");
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMadV1d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV2d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMadV2d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2d", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV3d", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMadV3d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMadV4d", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
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
    else if(naAbsi((NASizeInt)(d-a)) < 1)
      naError("naMadV1f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV2f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 2)
      naError("naMadV2f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(a==d)
      naError("naMadV3f", "a equals d. Use S or E variant");
    else if(naAbsi((NASizeInt)(d-a)) < 3)
      naError("naMadV3f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 3)
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
    else if(naAbsi((NASizeInt)(d-a)) < 4)
      naError("naMadV4f", "a overlaps misaligned with d.");
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMadV4f", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMadV1dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV1dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMadV2dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMadV3dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4dE(double* d, const double* a, const double* b, double s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMadV4dE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV4dE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 1))
      naError("naMadV1fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 1))
      naError("naMadV1fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
}
NA_IDEF void naMadV2fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 2))
      naError("naMadV2fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
}
NA_IDEF void naMadV3fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 3))
      naError("naMadV3fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
}
NA_IDEF void naMadV4fE(float* d, const float* a, const float* b, float s){
  #ifndef NDEBUG
    if((a != d) && (naAbsi((NASizeInt)(d-a)) < 4))
      naError("naMadV4fE", "a overlaps misaligned with d.");
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV4fE", "b overlaps misaligned with d");
  #endif
  d[0] = a[0] + b[0] * s;
  d[1] = a[1] + b[1] * s;
  d[2] = a[2] + b[2] * s;
  d[3] = a[3] + b[3] * s;
}
NA_IDEF void naMadV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMadV3dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b, double s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMadV4dS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 1)
      naError("naMadV1fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 2)
      naError("naMadV2fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 3)
      naError("naMadV3fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b, float s){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(d-b)) < 4)
      naError("naMadV4fS", "b overlaps with d. Use restrict pointers or the E variant");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV1dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4dES(double* d, const double* b, double s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
      naError("naMadV4dES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
  d[3] += b[3] * s;
}
NA_IDEF void naMadV1fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 1))
      naError("naMadV1fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
}
NA_IDEF void naMadV2fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 2))
      naError("naMadV2fE", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
}
NA_IDEF void naMadV3fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 3))
      naError("naMadV3fES", "b overlaps misaligned with d");
  #endif
  d[0] += b[0] * s;
  d[1] += b[1] * s;
  d[2] += b[2] * s;
}
NA_IDEF void naMadV4fES(float* d, const float* b, float s){
  #ifndef NDEBUG
    if((b != d) && (naAbsi((NASizeInt)(d-b)) < 4))
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



// ///////////////////////////////////////////
// Dot
// ///////////////////////////////////////////

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



// /////////////////////////////////////////
// Cross
// /////////////////////////////////////////

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
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1d", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV1d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1d", "a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2d", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV2d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2d", "a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3d", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV3d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3d", "a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4d", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4d", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV4d", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4d", "a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV1f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1f", "a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV2f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2f", "a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV3f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3f", "a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4f", "n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("naMirrorV4f", "Normal is not normalized");
    if(a == d)
      naError("naMirrorV4f", "a equals d. Use E or S variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4f", "a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1dE", "a overlaps misaligned with d");
  #endif
  naMadV1d(d, a, n, -2. * naDotV1d(a, n));
}
NA_IDEF void naMirrorV2dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2dE", "a overlaps misaligned with d");
  #endif
  naMadV2d(d, a, n, -2. * naDotV2d(a, n));
}
NA_IDEF void naMirrorV3dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3dE", "a overlaps misaligned with d");
  #endif
  naMadV3d(d, a, n, -2. * naDotV3d(a, n));
}
NA_IDEF void naMirrorV4dE(double* d, const double* a, const double* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4dE", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4dE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4dE", "a overlaps misaligned with d");
  #endif
  naMadV4d(d, a, n, -2. * naDotV4d(a, n));
}
NA_IDEF void naMirrorV1fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naMirrorV1fE", "a overlaps misaligned with d");
  #endif
  naMadV1f(d, a, n, -2.f * naDotV1f(a, n));
}
NA_IDEF void naMirrorV2fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naMirrorV2fE", "a overlaps misaligned with d");
  #endif
  naMadV2f(d, a, n, -2.f * naDotV2f(a, n));
}
NA_IDEF void naMirrorV3fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naMirrorV3fE", "a overlaps misaligned with d");
  #endif
  naMadV3f(d, a, n, -2.f * naDotV3f(a, n));
}
NA_IDEF void naMirrorV4fE(float* d, const float* a, const float* n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4fE", "n overlaps with d");
    if(!naAlmostOnef(naLengthV4f(n)))
      naError("naMirrorV4fE", "Normal is not normalized");
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naMirrorV4fE", "a overlaps misaligned with d");
  #endif
  naMadV4f(d, a, n, -2.f * naDotV4f(a, n));
}
NA_IDEF void naMirrorV1dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV1d(n)))
      naError("naMirrorV1dS", "Normal is not normalized");
  #endif
  naMadV1dS(d, n, -2. * naDotV1d(d, n));
}
NA_IDEF void naMirrorV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV2d(n)))
      naError("naMirrorV2dS", "Normal is not normalized");
  #endif
  naMadV2dS(d, n, -2. * naDotV2d(d, n));
}
NA_IDEF void naMirrorV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV3d(n)))
      naError("naMirrorV3dS", "Normal is not normalized");
  #endif
  naMadV3dS(d, n, -2. * naDotV3d(d, n));
}
NA_IDEF void naMirrorV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
      naError("naMirrorV4dS", "n overlaps with d");
    if(!naAlmostOne(naLengthV4d(n)))
      naError("naMirrorV4dS", "Normal is not normalized");
  #endif
  naMadV4dS(d, n, -2. * naDotV4d(d, n));
}
NA_IDEF void naMirrorV1fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 1)
      naError("naMirrorV1fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV1f(n)))
      naError("naMirrorV1fS", "Normal is not normalized");
  #endif
  naMadV1fS(d, n, -2.f * naDotV1f(d, n));
}
NA_IDEF void naMirrorV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 2)
      naError("naMirrorV2fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV2f(n)))
      naError("naMirrorV2fS", "Normal is not normalized");
  #endif
  naMadV2fS(d, n, -2.f * naDotV2f(d, n));
}
NA_IDEF void naMirrorV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 3)
      naError("naMirrorV3fS", "n overlaps with d");
    if(!naAlmostOnef(naLengthV3f(n)))
      naError("naMirrorV3fS", "Normal is not normalized");
  #endif
  naMadV3fS(d, n, -2.f * naDotV3f(d, n));
}
NA_IDEF void naMirrorV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT n){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(n-d)) < 4)
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
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV1d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1d", "a overlaps misaligned with d");
  #endif
  length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV2d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2d", "a overlaps misaligned with d");
  #endif
  length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV3d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3d", "a overlaps misaligned with d");
  #endif
  length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  double length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV4d", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4d", "a overlaps misaligned with d");
  #endif
  length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV1f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1f", "a overlaps misaligned with d");
  #endif
  length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV2f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2f", "a overlaps misaligned with d");
  #endif
  length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV3f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3f", "a overlaps misaligned with d");
  #endif
  length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  float length;
  #ifndef NDEBUG
    if(a == d)
      naError("naNormalizeV4f", "a is equal to d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4f", "a overlaps misaligned with d");
  #endif
  length = naLengthV4f(a);
  naDivV4f(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV1dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV1d(a);
  naDivV1d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV2dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV2d(a);
  naDivV2d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV3dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV3d(a);
  naDivV3d(d, a, length);
  return length;
}
NA_IDEF double naNormalizeV4dE(double* d, const double* a){
  double length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4dE", "a overlaps misaligned with d");
  #endif
  length = naLengthV4d(a);
  naDivV4d(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV1fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 1)
      naError("naNormalizeV1fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV1f(a);
  naDivV1f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV2fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naNormalizeV2fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV2f(a);
  naDivV2f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV3fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naNormalizeV3fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV3f(a);
  naDivV3f(d, a, length);
  return length;
}
NA_IDEF float naNormalizeV4fE(float* d, const float* a){
  float length;
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naNormalizeV4fE", "a overlaps misaligned with d");
  #endif
  length = naLengthV4f(a);
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
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
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
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
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
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
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
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
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
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
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
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dE(double* d, const double* a, const double* b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV2d", "b looks like a zero vector.");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dE(double* d, const double* a, const double* b){
  double bnorm = naDotV3dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV3d", "b looks like a zero vector.");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dE(double* d, const double* a, const double* b){
  double bnorm = naDotV4dS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4d", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV4d", "b looks like a zero vector.");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fE(float* d, const float* a, const float* b){
  float bnorm = naDotV2fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthogonalizeV2f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV2f", "b looks like a zero vector.");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fE(float* d, const float* a, const float* b){
  float bnorm = naDotV3fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthogonalizeV3f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV3f", "b looks like a zero vector.");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fE(float* d, const float* a, const float* b){
  float bnorm = naDotV4fS(b);
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthogonalizeV4f", "a overlaps misaligned with d");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV4f", "b looks like a zero vector.");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV2dS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV2d", "b looks like a zero vector.");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV3dS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV3d", "b looks like a zero vector.");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  double bnorm = naDotV4dS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthogonalizeV4d", "b overlaps with d");
    if(naAlmostZero(bnorm))
      naError("naOrthogonalizeV4d", "b looks like a zero vector.");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d) * naInv(bnorm));
}
NA_IDEF void naOrthogonalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV2fS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthogonalizeV2f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV2f", "b looks like a zero vector.");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV3fS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthogonalizeV3f", "b overlaps with d");
    if(naAlmostZerof(bnorm))
      naError("naOrthogonalizeV3f", "b looks like a zero vector.");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d) * naInvf(bnorm));
}
NA_IDEF void naOrthogonalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  float bnorm = naDotV4fS(b);
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(b-d)) < 4)
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
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV3d", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV4d", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV2f", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV3f", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == d)
      naError("naOrthonormalizeV4f", "a equals d. Use E or S variant.");
    else if(naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2d(d, a, b, - naDotV2d(b, a));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3d(d, a, b, - naDotV3d(b, a));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dE(double* d, const double* a, const double* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4d", "a overlaps misaligned with d");
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4d(d, a, b, - naDotV4d(b, a));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 2)
      naError("naOrthonormalizeV2f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2f(d, a, b, - naDotV2f(b, a));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 3)
      naError("naOrthonormalizeV3f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3f(d, a, b, - naDotV3f(b, a));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fE(float* d, const float* a, const float* b){
  #ifndef NDEBUG
    if((a != d) && naAbsi((NASizeInt)(a-d)) < 4)
      naError("naOrthonormalizeV4f", "a overlaps misaligned with d");
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4f", "b overlaps with d");
  #endif
  naMadV4f(d, a, b, - naDotV4f(b, a));
  naNormalizeV4fS(d);
}
NA_IDEF void naOrthonormalizeV2dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV2d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2d", "b overlaps with d");
  #endif
  naMadV2dS(d, b, - naDotV2d(b, d));
  naNormalizeV2dS(d);
}
NA_IDEF void naOrthonormalizeV3dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV3d(b)))
      naError("naOrthonormalizeV3d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3d", "b overlaps with d");
  #endif
  naMadV3dS(d, b, - naDotV3d(b, d));
  naNormalizeV3dS(d);
}
NA_IDEF void naOrthonormalizeV4dS(double* NA_RESTRICT d, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOne(naLengthV4d(b)))
      naError("naOrthonormalizeV4d", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
      naError("naOrthonormalizeV4d", "b overlaps with d");
  #endif
  naMadV4dS(d, b, - naDotV4d(b, d));
  naNormalizeV4dS(d);
}
NA_IDEF void naOrthonormalizeV2fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV2f(b)))
      naError("naOrthonormalizeV2f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 2)
      naError("naOrthonormalizeV2f", "b overlaps with d");
  #endif
  naMadV2fS(d, b, - naDotV2f(b, d));
  naNormalizeV2fS(d);
}
NA_IDEF void naOrthonormalizeV3fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV3f(b)))
      naError("naOrthonormalizeV3f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 3)
      naError("naOrthonormalizeV3f", "b overlaps with d");
  #endif
  naMadV3fS(d, b, - naDotV3f(b, d));
  naNormalizeV3fS(d);
}
NA_IDEF void naOrthonormalizeV4fS(float* NA_RESTRICT d, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(!naAlmostOnef(naLengthV4f(b)))
      naError("naOrthonormalizeV4f", "b is not normalized");
    if(naAbsi((NASizeInt)(b-d)) < 4)
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
    if(naInsidei(-3, 1, (NASizeInt)(D-v0)))
      naError("naFillM22dWithAxis", "v0 overlaps with D");
    if(naInsidei(-3, 1, (NASizeInt)(D-v1)))
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
    if(naInsidei(-8, 2, (NASizeInt)(D-v0)))
      naError("naFillM33dWithAxis", "v0 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v1)))
      naError("naFillM33dWithAxis", "v1 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v2)))
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
    if(naInsidei(-15, 3, (NASizeInt)(D-v0)))
      naError("naFillM44dWithAxis", "v0 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v1)))
      naError("naFillM44dWithAxis", "v1 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v2)))
      naError("naFillM44dWithAxis", "v2 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v3)))
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
    if(naInsidei(-3, 1, (NASizeInt)(D-v0)))
      naError("naFillM22fWithAxis", "v0 overlaps with D");
    if(naInsidei(-3, 1, (NASizeInt)(D-v1)))
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
    if(naInsidei(-8, 2, (NASizeInt)(D-v0)))
      naError("naFillM33fWithAxis", "v0 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v1)))
      naError("naFillM33fWithAxis", "v1 overlaps with D");
    if(naInsidei(-8, 2, (NASizeInt)(D-v2)))
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
    if(naInsidei(-15, 3, (NASizeInt)(D-v0)))
      naError("naFillM44fWithAxis", "v0 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v1)))
      naError("naFillM44fWithAxis", "v1 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v2)))
      naError("naFillM44fWithAxis", "v2 overlaps with D");
    if(naInsidei(-15, 3, (NASizeInt)(D-v3)))
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
    if(naInsidei(-3, 1, (NASizeInt)(D-diag)))
      naError("naFillM22dWithDiagV2d", "diag overlaps with D");
  #endif
  naFillM22dColumnFirst(D,
                diag[0], 0.     ,
                0.     , diag[1]);
}
NA_IDEF void naFillM33dWithDiagV3d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-diag)))
      naError("naFillM33dWithDiagV3d", "diag overlaps with D");
  #endif
  naFillM33dColumnFirst(D,
                diag[0], 0.     , 0.,
                0.     , diag[1], 0.,
                0.     , 0.     , diag[2]);
}
NA_IDEF void naFillM44dWithDiagV4d(double* NA_RESTRICT D, const double* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-diag)))
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
    if(naInsidei(-3, 1, (NASizeInt)(D-diag)))
      naError("naFillM22fWithDiagV2f", "diag overlaps with D");
  #endif
  naFillM22fColumnFirst(D,  diag[0], 0.f    ,
                0.f    , diag[1]);
}
NA_IDEF void naFillM33fWithDiagV3f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-diag)))
      naError("naFillM33fWithDiagV3f", "diag overlaps with D");
  #endif
  naFillM33fColumnFirst(D,
                diag[0], 0.f    , 0.f,
                0.f    , diag[1], 0.f,
                0.f    , 0.f    , diag[2]);
}
NA_IDEF void naFillM44fWithDiagV4f(float* NA_RESTRICT D, const float* NA_RESTRICT diag){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-diag)))
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
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naCopyM22d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33d( double* NA_RESTRICT D, const double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM33d", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
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
    else if(naAbsi((NASizeInt)(D-A)) < 16)
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
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naCopyM22f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33f( float* NA_RESTRICT D, const float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(D == A)
      naError("naCopyM33f", "A is equal to D. Use E variant or povide strict pointer.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
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
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naCopyM44f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naCopyM22dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naCopyM33dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44dE( double* D, const double* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naCopyM44dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[4]; D[ 8] = A[ 8]; D[12] = A[12];
  D[1] = A[1]; D[5] = A[5]; D[ 9] = A[ 9]; D[13] = A[13];
  D[2] = A[2]; D[6] = A[6]; D[10] = A[10]; D[14] = A[14];
  D[3] = A[3]; D[7] = A[7]; D[11] = A[11]; D[15] = A[15];
}
NA_IDEF void naCopyM22fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naCopyM22fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[2];
  D[1] = A[1]; D[3] = A[3];
}
NA_IDEF void naCopyM33fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naCopyM33fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[3]; D[6] = A[6];
  D[1] = A[1]; D[4] = A[4]; D[7] = A[7];
  D[2] = A[2]; D[5] = A[5]; D[8] = A[8];
}
NA_IDEF void naCopyM44fE( float* D, const float* A){
  #ifndef NDEBUG
    if((D != A) && naAbsi((NASizeInt)(D-A)) < 16)
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



// /////////////////////////////////
// Transpose
// /////////////////////////////////

NA_IDEF void naTransposeM22d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM22d", "A equals D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naTransposeM22d", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33d( double* NA_RESTRICT D, double* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM33d", "A equals D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
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
    else if(naAbsi((NASizeInt)(D-A)) < 16)
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
    else if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naTransposeM22f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33f( float* NA_RESTRICT D, float* NA_RESTRICT A){
  #ifndef NDEBUG
    if(A == D)
      naError("naTransposeM33f", "A equals D. Use E or S variant.");
    else if(naAbsi((NASizeInt)(D-A)) < 9)
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
    else if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naTransposeM44f", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naTransposeM22dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naTransposeM33dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44dE( double* D, double* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 16)
      naError("naTransposeM44dE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[4] = A[1]; D[8] = A[2]; D[12] = A[3];
  D[1] = A[4]; D[5] = A[5]; D[9] = A[6]; D[13] = A[7];
  D[2] = A[8]; D[6] = A[9]; D[10] = A[10]; D[14] = A[11];
  D[3] = A[12]; D[7] = A[13]; D[11] = A[14]; D[15] = A[15];
}
NA_IDEF void naTransposeM22fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 4)
      naError("naTransposeM22fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[2] = A[1];
  D[1] = A[2]; D[3] = A[3];
}
NA_IDEF void naTransposeM33fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 9)
      naError("naTransposeM33fE", "A overlaps misaligned with D");
  #endif
  D[0] = A[0]; D[3] = A[1]; D[6] = A[2];
  D[1] = A[3]; D[4] = A[4]; D[7] = A[5];
  D[2] = A[6]; D[5] = A[7]; D[8] = A[8];
}
NA_IDEF void naTransposeM44fE( float* D, float* A){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(D-A)) < 16)
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




// //////////////////////////////////////
// Sub
// //////////////////////////////////////

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


// ///////////////////////////////////////
// Invert componentwise
// ///////////////////////////////////////

NA_IDEF void naInvCompM22d (double* NA_RESTRICT D, const double* NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("naInvCompM22d", "S equals D. Use S or E variant");
    if(naAbsi((NASizeInt)(S-D)) < 4)
      naError("naInvCompM22d", "S overlaps with D.");
  #endif
  D[0] = naInv(S[0]); D[2] = naInv(S[2]);
  D[1] = naInv(S[1]); D[3] = naInv(S[3]);
}
NA_IDEF void naInvCompM33d (double* NA_RESTRICT D, const double* NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("naInvCompM33d", "S equals D. Use S or E variant");
    if(naAbsi((NASizeInt)(S-D)) < 9)
      naError("naInvCompM33d", "S overlaps with D.");
  #endif
  D[0] = naInv(S[0]); D[3] = naInv(S[3]); D[6] = naInv(S[6]);
  D[1] = naInv(S[1]); D[4] = naInv(S[4]); D[7] = naInv(S[7]);
  D[2] = naInv(S[2]); D[5] = naInv(S[5]); D[8] = naInv(S[8]);
}
NA_IDEF void naInvCompM44d (double* NA_RESTRICT D, const double* NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("naInvCompM44d", "S equals D. Use S or E variant");
    if(naAbsi((NASizeInt)(S-D)) < 16)
      naError("naInvCompM44d", "S overlaps with D.");
  #endif
  D[0] = naInv(S[0]); D[4] = naInv(S[4]); D[ 8] = naInv(S[ 8]); D[12] = naInv(S[12]);
  D[1] = naInv(S[1]); D[5] = naInv(S[5]); D[ 9] = naInv(S[ 9]); D[13] = naInv(S[13]);
  D[2] = naInv(S[2]); D[6] = naInv(S[6]); D[10] = naInv(S[10]); D[14] = naInv(S[14]);
  D[3] = naInv(S[3]); D[7] = naInv(S[7]); D[11] = naInv(S[11]); D[15] = naInv(S[15]);
}
NA_IDEF void naInvCompM22f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("naInvCompM22f", "S equals D. Use S or E variant");
    if(naAbsi((NASizeInt)(S-D)) < 4)
      naError("naInvCompM22f", "S overlaps with D.");
  #endif
  D[0] = naInvf(S[0]); D[2] = naInvf(S[2]);
  D[1] = naInvf(S[1]); D[3] = naInvf(S[3]);
}
NA_IDEF void naInvCompM33f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("naInvCompM33f", "S equals D. Use S or E variant");
    if(naAbsi((NASizeInt)(S-D)) < 9)
      naError("naInvCompM33f", "S overlaps with D.");
  #endif
  D[0] = naInvf(S[0]); D[3] = naInvf(S[3]); D[6] = naInvf(S[6]);
  D[1] = naInvf(S[1]); D[4] = naInvf(S[4]); D[7] = naInvf(S[7]);
  D[2] = naInvf(S[2]); D[5] = naInvf(S[5]); D[8] = naInvf(S[8]);
}
NA_IDEF void naInvCompM44f (float*  NA_RESTRICT D, const float*  NA_RESTRICT S){
  #ifndef NDEBUG
    if(S==D)
      naError("naInvCompM44f", "S equals D. Use S or E variant");
    if(naAbsi((NASizeInt)(S-D)) < 16)
      naError("naInvCompM44f", "S overlaps with D.");
  #endif
  D[0] = naInvf(S[0]); D[4] = naInvf(S[4]); D[ 8] = naInvf(S[ 8]); D[12] = naInvf(S[12]);
  D[1] = naInvf(S[1]); D[5] = naInvf(S[5]); D[ 9] = naInvf(S[ 9]); D[13] = naInvf(S[13]);
  D[2] = naInvf(S[2]); D[6] = naInvf(S[6]); D[10] = naInvf(S[10]); D[14] = naInvf(S[14]);
  D[3] = naInvf(S[3]); D[7] = naInvf(S[7]); D[11] = naInvf(S[11]); D[15] = naInvf(S[15]);
}
NA_IDEF void naInvCompM22dE(double* D, const double* S){
  #ifndef NDEBUG
    if((S != D) && naAbsi((NASizeInt)(S-D)) < 4)
      naError("naInvCompM22dE", "S overlaps misaligned with D.");
  #endif
  D[0] = naInv(S[0]); D[2] = naInv(S[2]);
  D[1] = naInv(S[1]); D[3] = naInv(S[3]);
}
NA_IDEF void naInvCompM33dE(double* D, const double* S){
  #ifndef NDEBUG
    if((S != D) && naAbsi((NASizeInt)(S-D)) < 9)
      naError("naInvCompM33dE", "S overlaps misaligned with D.");
  #endif
  D[0] = naInv(S[0]); D[3] = naInv(S[3]); D[6] = naInv(S[6]);
  D[1] = naInv(S[1]); D[4] = naInv(S[4]); D[7] = naInv(S[7]);
  D[2] = naInv(S[2]); D[5] = naInv(S[5]); D[8] = naInv(S[8]);
}
NA_IDEF void naInvCompM44dE(double* D, const double* S){
  #ifndef NDEBUG
    if((S != D) && naAbsi((NASizeInt)(S-D)) < 16)
      naError("naInvCompM44dE", "S overlaps misaligned with D.");
  #endif
  D[0] = naInv(S[0]); D[4] = naInv(S[4]); D[ 8] = naInv(S[ 8]); D[12] = naInv(S[12]);
  D[1] = naInv(S[1]); D[5] = naInv(S[5]); D[ 9] = naInv(S[ 9]); D[13] = naInv(S[13]);
  D[2] = naInv(S[2]); D[6] = naInv(S[6]); D[10] = naInv(S[10]); D[14] = naInv(S[14]);
  D[3] = naInv(S[3]); D[7] = naInv(S[7]); D[11] = naInv(S[11]); D[15] = naInv(S[15]);
}
NA_IDEF void naInvCompM22fE(float*  D, const float*  S){
  #ifndef NDEBUG
    if((S != D) && naAbsi((NASizeInt)(S-D)) < 4)
      naError("naInvCompM22fE", "S overlaps misaligned with D.");
  #endif
  D[0] = naInvf(S[0]); D[2] = naInvf(S[2]);
  D[1] = naInvf(S[1]); D[3] = naInvf(S[3]);
}
NA_IDEF void naInvCompM33fE(float*  D, const float*  S){
  #ifndef NDEBUG
    if((S != D) && naAbsi((NASizeInt)(S-D)) < 9)
      naError("naInvCompM33fE", "S overlaps misaligned with D.");
  #endif
  D[0] = naInvf(S[0]); D[3] = naInvf(S[3]); D[6] = naInvf(S[6]);
  D[1] = naInvf(S[1]); D[4] = naInvf(S[4]); D[7] = naInvf(S[7]);
  D[2] = naInvf(S[2]); D[5] = naInvf(S[5]); D[8] = naInvf(S[8]);
}
NA_IDEF void naInvCompM44fE(float*  D, const float*  S){
  #ifndef NDEBUG
    if((S != D) && naAbsi((NASizeInt)(S-D)) < 16)
      naError("naInvCompM44fE", "S overlaps misaligned with D.");
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



// ///////////////////////////////////////
// Multiply componentwise with scalar
// ///////////////////////////////////////

NA_IDEF void naMulCompM22d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM22d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22d", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33d(double*  NA_RESTRICT D, const double*  NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM33d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33d", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM44d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44d", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naMulCompM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM22f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22f", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM33f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33f", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A, float s){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM44f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44f", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naMulCompM22dE(double*  D, const double*  A, double s){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22dE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33dE(double*  D, const double*  A, double s){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33dE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44dE(double* D, const double* A, double s){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44dE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[4] = A[4]*s; D[ 8] = A[ 8]*s; D[12] = A[12]*s;
  D[1] = A[1]*s; D[5] = A[5]*s; D[ 9] = A[ 9]*s; D[13] = A[13]*s;
  D[2] = A[2]*s; D[6] = A[6]*s; D[10] = A[10]*s; D[14] = A[14]*s;
  D[3] = A[3]*s; D[7] = A[7]*s; D[11] = A[11]*s; D[15] = A[15]*s;
}
NA_IDEF void naMulCompM22fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22fE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[2] = A[2]*s;
  D[1] = A[1]*s; D[3] = A[3]*s;
}
NA_IDEF void naMulCompM33fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33fE", "A overlaps misaligned with D.");
  #endif
  D[0] = A[0]*s; D[3] = A[3]*s; D[6] = A[6]*s;
  D[1] = A[1]*s; D[4] = A[4]*s; D[7] = A[7]*s;
  D[2] = A[2]*s; D[5] = A[5]*s; D[8] = A[8]*s;
}
NA_IDEF void naMulCompM44fE(float* D, const float* A, float s){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44fE", "A overlaps misaligned with D.");
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




// ///////////////////////////////////////
// Multiply componentwise with vector
// ///////////////////////////////////////

NA_IDEF void naMulCompM22dV2d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM22dV2d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22dV2d", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naMulCompM22dV2d", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33dV3d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM33dV3d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33dV3d", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naMulCompM33dV3d", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44dV4d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM44dV4d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44dV4d", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naMulCompM44dV4d", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22fV2f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM22fV2f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22fV2f", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naMulCompM22fV2f", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33fV3f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM33fV3f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33fV3f", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naMulCompM33fV3f", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44fV4f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM44fV4f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44fV4f", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naMulCompM44fV4f", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22dV2dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22dV2dE", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naMulCompM22dV2dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33dV3dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33dV3dE", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naMulCompM33dV3dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44dV4dE( double* D, const double* A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44dV4dE", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naMulCompM44dV4dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22fV2fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22fV2fE", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naMulCompM22fV2fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[2] = A[2]*v[1];
  D[1] = A[1]*v[0]; D[3] = A[3]*v[1];
}
NA_IDEF void naMulCompM33fV3fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33fV3fE", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naMulCompM33fV3fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[3] = A[3]*v[1]; D[6] = A[6]*v[2];
  D[1] = A[1]*v[0]; D[4] = A[4]*v[1]; D[7] = A[7]*v[2];
  D[2] = A[2]*v[0]; D[5] = A[5]*v[1]; D[8] = A[8]*v[2];
}
NA_IDEF void naMulCompM44fV4fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44fV4fE", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naMulCompM44fV4fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*v[0]; D[4] = A[4]*v[1]; D[ 8] = A[ 8]*v[2]; D[12] = A[12]*v[3];
  D[1] = A[1]*v[0]; D[5] = A[5]*v[1]; D[ 9] = A[ 9]*v[2]; D[13] = A[13]*v[3];
  D[2] = A[2]*v[0]; D[6] = A[6]*v[1]; D[10] = A[10]*v[2]; D[14] = A[14]*v[3];
  D[3] = A[3]*v[0]; D[7] = A[7]*v[1]; D[11] = A[11]*v[2]; D[15] = A[15]*v[3];
}
NA_IDEF void naMulCompM22dV2dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naMulCompM22dV2dS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[2] *= v[1];
  D[1] *= v[0]; D[3] *= v[1];
}
NA_IDEF void naMulCompM33dV3dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naMulCompM33dV3dS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[3] *= v[1]; D[6] *= v[2];
  D[1] *= v[0]; D[4] *= v[1]; D[7] *= v[2];
  D[2] *= v[0]; D[5] *= v[1]; D[8] *= v[2];
}
NA_IDEF void naMulCompM44dV4dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naMulCompM44dV4dS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[4] *= v[1]; D[ 8] *= v[2]; D[12] *= v[3];
  D[1] *= v[0]; D[5] *= v[1]; D[ 9] *= v[2]; D[13] *= v[3];
  D[2] *= v[0]; D[6] *= v[1]; D[10] *= v[2]; D[14] *= v[3];
  D[3] *= v[0]; D[7] *= v[1]; D[11] *= v[2]; D[15] *= v[3];
}
NA_IDEF void naMulCompM22fV2fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naMulCompM22fV2fS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[2] *= v[1];
  D[1] *= v[0]; D[3] *= v[1];
}
NA_IDEF void naMulCompM33fV3fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naMulCompM33fV3fS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[3] *= v[1]; D[6] *= v[2];
  D[1] *= v[0]; D[4] *= v[1]; D[7] *= v[2];
  D[2] *= v[0]; D[5] *= v[1]; D[8] *= v[2];
}
NA_IDEF void naMulCompM44fV4fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naMulCompM44fV4fS", "v overlaps with D.");
  #endif
  D[0] *= v[0]; D[4] *= v[1]; D[ 8] *= v[2]; D[12] *= v[3];
  D[1] *= v[0]; D[5] *= v[1]; D[ 9] *= v[2]; D[13] *= v[3];
  D[2] *= v[0]; D[6] *= v[1]; D[10] *= v[2]; D[14] *= v[3];
  D[3] *= v[0]; D[7] *= v[1]; D[11] *= v[2]; D[15] *= v[3];
}





// ///////////////////////////////////////
// Division componentwise with vector
// ///////////////////////////////////////

NA_IDEF void naDivCompM22dV2d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM22dV2d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22dV2d", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naDivCompM22dV2d", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[2] = A[2]*naInv(v[1]);
  D[1] = A[1]*naInv(v[0]); D[3] = A[3]*naInv(v[1]);
}
NA_IDEF void naDivCompM33dV3d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM33dV3d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33dV3d", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naDivCompM33dV3d", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[3] = A[3]*naInv(v[1]); D[6] = A[6]*naInv(v[2]);
  D[1] = A[1]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[7] = A[7]*naInv(v[2]);
  D[2] = A[2]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[8] = A[8]*naInv(v[2]);
}
NA_IDEF void naDivCompM44dV4d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM44dV4d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44dV4d", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naDivCompM44dV4d", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[ 8] = A[ 8]*naInv(v[2]); D[12] = A[12]*naInv(v[3]);
  D[1] = A[1]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[ 9] = A[ 9]*naInv(v[2]); D[13] = A[13]*naInv(v[3]);
  D[2] = A[2]*naInv(v[0]); D[6] = A[6]*naInv(v[1]); D[10] = A[10]*naInv(v[2]); D[14] = A[14]*naInv(v[3]);
  D[3] = A[3]*naInv(v[0]); D[7] = A[7]*naInv(v[1]); D[11] = A[11]*naInv(v[2]); D[15] = A[15]*naInv(v[3]);
}
NA_IDEF void naDivCompM22fV2f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM22fV2f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22fV2f", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naDivCompM22fV2f", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[2] = A[2]*naInvf(v[1]);
  D[1] = A[1]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]);
}
NA_IDEF void naDivCompM33fV3f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM33fV3f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33fV3f", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naDivCompM33fV3f", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]); D[6] = A[6]*naInvf(v[2]);
  D[1] = A[1]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[7] = A[7]*naInvf(v[2]);
  D[2] = A[2]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[8] = A[8]*naInvf(v[2]);
}
NA_IDEF void naDivCompM44fV4f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM44fV4f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44fV4f", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naDivCompM44fV4f", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[ 8] = A[ 8]*naInvf(v[2]); D[12] = A[12]*naInvf(v[3]);
  D[1] = A[1]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[ 9] = A[ 9]*naInvf(v[2]); D[13] = A[13]*naInvf(v[3]);
  D[2] = A[2]*naInvf(v[0]); D[6] = A[6]*naInvf(v[1]); D[10] = A[10]*naInvf(v[2]); D[14] = A[14]*naInvf(v[3]);
  D[3] = A[3]*naInvf(v[0]); D[7] = A[7]*naInvf(v[1]); D[11] = A[11]*naInvf(v[2]); D[15] = A[15]*naInvf(v[3]);
}
NA_IDEF void naDivCompM22dV2dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22dV2dE", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naDivCompM22dV2dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[2] = A[2]*naInv(v[1]);
  D[1] = A[1]*naInv(v[0]); D[3] = A[3]*naInv(v[1]);
}
NA_IDEF void naDivCompM33dV3dE( double* D, const double*  A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33dV3dE", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naDivCompM33dV3dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[3] = A[3]*naInv(v[1]); D[6] = A[6]*naInv(v[2]);
  D[1] = A[1]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[7] = A[7]*naInv(v[2]);
  D[2] = A[2]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[8] = A[8]*naInv(v[2]);
}
NA_IDEF void naDivCompM44dV4dE( double* D, const double* A, const double* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44dV4dE", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naDivCompM44dV4dE", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInv(v[0]); D[4] = A[4]*naInv(v[1]); D[ 8] = A[ 8]*naInv(v[2]); D[12] = A[12]*naInv(v[3]);
  D[1] = A[1]*naInv(v[0]); D[5] = A[5]*naInv(v[1]); D[ 9] = A[ 9]*naInv(v[2]); D[13] = A[13]*naInv(v[3]);
  D[2] = A[2]*naInv(v[0]); D[6] = A[6]*naInv(v[1]); D[10] = A[10]*naInv(v[2]); D[14] = A[14]*naInv(v[3]);
  D[3] = A[3]*naInv(v[0]); D[7] = A[7]*naInv(v[1]); D[11] = A[11]*naInv(v[2]); D[15] = A[15]*naInv(v[3]);
}
NA_IDEF void naDivCompM22fV2fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22fV2fE", "A overlaps misaligned with D.");
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naDivCompM22fV2fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[2] = A[2]*naInvf(v[1]);
  D[1] = A[1]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]);
}
NA_IDEF void naDivCompM33fV3fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33fV3fE", "A overlaps misaligned with D.");
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naDivCompM33fV3fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[3] = A[3]*naInvf(v[1]); D[6] = A[6]*naInvf(v[2]);
  D[1] = A[1]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[7] = A[7]*naInvf(v[2]);
  D[2] = A[2]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[8] = A[8]*naInvf(v[2]);
}
NA_IDEF void naDivCompM44fV4fE( float* D, const float* A, const float* v){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44fV4fE", "A overlaps misaligned with D.");
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naDivCompM44fV4fE", "v overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(v[0]); D[4] = A[4]*naInvf(v[1]); D[ 8] = A[ 8]*naInvf(v[2]); D[12] = A[12]*naInvf(v[3]);
  D[1] = A[1]*naInvf(v[0]); D[5] = A[5]*naInvf(v[1]); D[ 9] = A[ 9]*naInvf(v[2]); D[13] = A[13]*naInvf(v[3]);
  D[2] = A[2]*naInvf(v[0]); D[6] = A[6]*naInvf(v[1]); D[10] = A[10]*naInvf(v[2]); D[14] = A[14]*naInvf(v[3]);
  D[3] = A[3]*naInvf(v[0]); D[7] = A[7]*naInvf(v[1]); D[11] = A[11]*naInvf(v[2]); D[15] = A[15]*naInvf(v[3]);
}
NA_IDEF void naDivCompM22dV2dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naDivCompM22dV2dS", "v overlaps with D.");
  #endif
  D[0] *= naInv(v[0]); D[2] *= naInv(v[1]);
  D[1] *= naInv(v[0]); D[3] *= naInv(v[1]);
}
NA_IDEF void naDivCompM33dV3dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naDivCompM33dV3dS", "v overlaps with D.");
  #endif
  D[0] *= naInv(v[0]); D[3] *= naInv(v[1]); D[6] *= naInv(v[2]);
  D[1] *= naInv(v[0]); D[4] *= naInv(v[1]); D[7] *= naInv(v[2]);
  D[2] *= naInv(v[0]); D[5] *= naInv(v[1]); D[8] *= naInv(v[2]);
}
NA_IDEF void naDivCompM44dV4dS( double* NA_RESTRICT D, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naDivCompM44dV4dS", "v overlaps with D.");
  #endif
  D[0] *= naInv(v[0]); D[4] *= naInv(v[1]); D[ 8] *= naInv(v[2]); D[12] *= naInv(v[3]);
  D[1] *= naInv(v[0]); D[5] *= naInv(v[1]); D[ 9] *= naInv(v[2]); D[13] *= naInv(v[3]);
  D[2] *= naInv(v[0]); D[6] *= naInv(v[1]); D[10] *= naInv(v[2]); D[14] *= naInv(v[3]);
  D[3] *= naInv(v[0]); D[7] *= naInv(v[1]); D[11] *= naInv(v[2]); D[15] *= naInv(v[3]);
}
NA_IDEF void naDivCompM22fV2fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(D-v)))
      naError("naDivCompM22fV2fS", "v overlaps with D.");
  #endif
  D[0] *= naInvf(v[0]); D[2] *= naInvf(v[1]);
  D[1] *= naInvf(v[0]); D[3] *= naInvf(v[1]);
}
NA_IDEF void naDivCompM33fV3fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(D-v)))
      naError("naDivCompM33fV3fS", "v overlaps with D.");
  #endif
  D[0] *= naInvf(v[0]); D[3] *= naInvf(v[1]); D[6] *= naInvf(v[2]);
  D[1] *= naInvf(v[0]); D[4] *= naInvf(v[1]); D[7] *= naInvf(v[2]);
  D[2] *= naInvf(v[0]); D[5] *= naInvf(v[1]); D[8] *= naInvf(v[2]);
}
NA_IDEF void naDivCompM44fV4fS( float* NA_RESTRICT D, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(D-v)))
      naError("naDivCompM44fV4fS", "v overlaps with D.");
  #endif
  D[0] *= naInvf(v[0]); D[4] *= naInvf(v[1]); D[ 8] *= naInvf(v[2]); D[12] *= naInvf(v[3]);
  D[1] *= naInvf(v[0]); D[5] *= naInvf(v[1]); D[ 9] *= naInvf(v[2]); D[13] *= naInvf(v[3]);
  D[2] *= naInvf(v[0]); D[6] *= naInvf(v[1]); D[10] *= naInvf(v[2]); D[14] *= naInvf(v[3]);
  D[3] *= naInvf(v[0]); D[7] *= naInvf(v[1]); D[11] *= naInvf(v[2]); D[15] *= naInvf(v[3]);
}



// ///////////////////////////////////////
// Multiply componentwise with matrix
// ///////////////////////////////////////

NA_IDEF void naMulCompM22dM22d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM22dVM22d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22dM22d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulCompM22dM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33dM33d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM33dM33d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33dM33d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulCompM33dM33d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44dM44d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM44dM34d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44dM44d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulCompM44dM44d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22fM22f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM22fM22f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22fM22f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulCompM22fM22f", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33fM33f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM33fM33f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33fM33f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulCompM33fM33f", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44fM44f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naMulCompM44fM44f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44fM44f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulCompM44fM44f", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22dM22dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22dM22dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulCompM22dM22dE", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33dM33dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33dM33dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulCompM33dM33dE", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44dM44dE( double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44dM44dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulCompM44dM44dE", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22fM22fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naMulCompM22fM22fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulCompM22fM22fE", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[2] = A[2]*B[2];
  D[1] = A[1]*B[1]; D[3] = A[3]*B[3];
}
NA_IDEF void naMulCompM33fM33fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naMulCompM33fM33fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulCompM33fM33fE", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[3] = A[3]*B[3]; D[6] = A[6]*B[6];
  D[1] = A[1]*B[1]; D[4] = A[4]*B[4]; D[7] = A[7]*B[7];
  D[2] = A[2]*B[2]; D[5] = A[5]*B[5]; D[8] = A[8]*B[8];
}
NA_IDEF void naMulCompM44fM44fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naMulCompM44fM44fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulCompM44fM44fE", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0]; D[4] = A[4]*B[4]; D[ 8] = A[ 8]*B[ 8]; D[12] = A[12]*B[12];
  D[1] = A[1]*B[1]; D[5] = A[5]*B[5]; D[ 9] = A[ 9]*B[ 9]; D[13] = A[13]*B[13];
  D[2] = A[2]*B[2]; D[6] = A[6]*B[6]; D[10] = A[10]*B[10]; D[14] = A[14]*B[14];
  D[3] = A[3]*B[3]; D[7] = A[7]*B[7]; D[11] = A[11]*B[11]; D[15] = A[15]*B[15];
}
NA_IDEF void naMulCompM22dM22dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulCompM22dM22dS", "B overlaps with D.");
  #endif
  D[0] *= B[0]; D[2] *= B[2];
  D[1] *= B[1]; D[3] *= B[3];
}
NA_IDEF void naMulCompM33dM33dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulCompM33dM33dS", "B overlaps with D.");
  #endif
  D[0] *= B[0]; D[3] *= B[3]; D[6] *= B[6];
  D[1] *= B[1]; D[4] *= B[4]; D[7] *= B[7];
  D[2] *= B[2]; D[5] *= B[5]; D[8] *= B[8];
}
NA_IDEF void naMulCompM44dM44dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulCompM44dM44dS", "B overlaps with D.");
  #endif
  D[0] *= B[0]; D[4] *= B[4]; D[ 8] *= B[ 8]; D[12] *= B[12];
  D[1] *= B[1]; D[5] *= B[5]; D[ 9] *= B[ 9]; D[13] *= B[13];
  D[2] *= B[2]; D[6] *= B[6]; D[10] *= B[10]; D[14] *= B[14];
  D[3] *= B[3]; D[7] *= B[7]; D[11] *= B[11]; D[15] *= B[15];
}
NA_IDEF void naMulCompM22fM22fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulCompM22fM22fS", "B overlaps with D.");
  #endif
  D[0] *= B[0]; D[2] *= B[2];
  D[1] *= B[1]; D[3] *= B[3];
}
NA_IDEF void naMulCompM33fM33fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulCompM33fM33fS", "B overlaps with D.");
  #endif
  D[0] *= B[0]; D[3] *= B[3]; D[6] *= B[6];
  D[1] *= B[1]; D[4] *= B[4]; D[7] *= B[7];
  D[2] *= B[2]; D[5] *= B[5]; D[8] *= B[8];
}
NA_IDEF void naMulCompM44fM44fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulCompM44fM44fS", "B overlaps with D.");
  #endif
  D[0] *= B[0]; D[4] *= B[4]; D[ 8] *= B[ 8]; D[12] *= B[12];
  D[1] *= B[1]; D[5] *= B[5]; D[ 9] *= B[ 9]; D[13] *= B[13];
  D[2] *= B[2]; D[6] *= B[6]; D[10] *= B[10]; D[14] *= B[14];
  D[3] *= B[3]; D[7] *= B[7]; D[11] *= B[11]; D[15] *= B[15];
}





// ///////////////////////////////////////
// Division componentwise with matrix
// ///////////////////////////////////////


NA_IDEF void naDivCompM22dM22d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM22dM22d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22dM22d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naDivCompM22dM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[2] = A[2]*naInv(B[2]);
  D[1] = A[1]*naInv(B[1]); D[3] = A[3]*naInv(B[3]);
}
NA_IDEF void naDivCompM33dM33d( double* NA_RESTRICT D, const double*  NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM33dM33d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33dM33d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naDivCompM33dM33d", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[3] = A[3]*naInv(B[3]); D[6] = A[6]*naInv(B[6]);
  D[1] = A[1]*naInv(B[1]); D[4] = A[4]*naInv(B[4]); D[7] = A[7]*naInv(B[7]);
  D[2] = A[2]*naInv(B[2]); D[5] = A[5]*naInv(B[5]); D[8] = A[8]*naInv(B[8]);
}
NA_IDEF void naDivCompM44dM44d( double* NA_RESTRICT D, const double* NA_RESTRICT A, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM44dM44d", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44dM44d", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naDivCompM44dM44d", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[4] = A[4]*naInv(B[4]); D[ 8] = A[ 8]*naInv(B[ 8]); D[12] = A[12]*naInv(B[12]);
  D[1] = A[1]*naInv(B[1]); D[5] = A[5]*naInv(B[5]); D[ 9] = A[ 9]*naInv(B[ 9]); D[13] = A[13]*naInv(B[13]);
  D[2] = A[2]*naInv(B[2]); D[6] = A[6]*naInv(B[6]); D[10] = A[10]*naInv(B[10]); D[14] = A[14]*naInv(B[14]);
  D[3] = A[3]*naInv(B[3]); D[7] = A[7]*naInv(B[7]); D[11] = A[11]*naInv(B[11]); D[15] = A[15]*naInv(B[15]);
}
NA_IDEF void naDivCompM22fM22f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM22fM22f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22fM22f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naDivCompM22fM22f", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[2] = A[2]*naInvf(B[2]);
  D[1] = A[1]*naInvf(B[1]); D[3] = A[3]*naInvf(B[3]);
}
NA_IDEF void naDivCompM33fM33f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM33fM33f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33fM33f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naDivCompM33fM33f", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[3] = A[3]*naInvf(B[3]); D[6] = A[6]*naInvf(B[6]);
  D[1] = A[1]*naInvf(B[1]); D[4] = A[4]*naInvf(B[4]); D[7] = A[7]*naInvf(B[7]);
  D[2] = A[2]*naInvf(B[2]); D[5] = A[5]*naInvf(B[5]); D[8] = A[8]*naInvf(B[8]);
}
NA_IDEF void naDivCompM44fM44f( float* NA_RESTRICT D, const float* NA_RESTRICT A, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(A == D)
      naError("naDivCompM44fM44f", "A equals D. Use E or S variant");
    else if(naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44fM44f", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naDivCompM44fM44f", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[4] = A[4]*naInvf(B[4]); D[ 8] = A[ 8]*naInvf(B[ 8]); D[12] = A[12]*naInvf(B[12]);
  D[1] = A[1]*naInvf(B[1]); D[5] = A[5]*naInvf(B[5]); D[ 9] = A[ 9]*naInvf(B[ 9]); D[13] = A[13]*naInvf(B[13]);
  D[2] = A[2]*naInvf(B[2]); D[6] = A[6]*naInvf(B[6]); D[10] = A[10]*naInvf(B[10]); D[14] = A[14]*naInvf(B[14]);
  D[3] = A[3]*naInvf(B[3]); D[7] = A[7]*naInvf(B[7]); D[11] = A[11]*naInvf(B[11]); D[15] = A[15]*naInvf(B[15]);
}
NA_IDEF void naDivCompM22dM22dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22dM22dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naDivCompM22dM22dE", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[2] = A[2]*naInv(B[2]);
  D[1] = A[1]*naInv(B[1]); D[3] = A[3]*naInv(B[3]);
}
NA_IDEF void naDivCompM33dM33dE( double* D, const double*  A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33dM33dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naDivCompM33dM33dE", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[3] = A[3]*naInv(B[3]); D[6] = A[6]*naInv(B[6]);
  D[1] = A[1]*naInv(B[1]); D[4] = A[4]*naInv(B[4]); D[7] = A[7]*naInv(B[7]);
  D[2] = A[2]*naInv(B[2]); D[5] = A[5]*naInv(B[5]); D[8] = A[8]*naInv(B[8]);
}
NA_IDEF void naDivCompM44dM44dE( double* D, const double* A, const double* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44dM44dE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naDivCompM44dM44dE", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInv(B[0]); D[4] = A[4]*naInv(B[4]); D[ 8] = A[ 8]*naInv(B[ 8]); D[12] = A[12]*naInv(B[12]);
  D[1] = A[1]*naInv(B[1]); D[5] = A[5]*naInv(B[5]); D[ 9] = A[ 9]*naInv(B[ 9]); D[13] = A[13]*naInv(B[13]);
  D[2] = A[2]*naInv(B[2]); D[6] = A[6]*naInv(B[6]); D[10] = A[10]*naInv(B[10]); D[14] = A[14]*naInv(B[14]);
  D[3] = A[3]*naInv(B[3]); D[7] = A[7]*naInv(B[7]); D[11] = A[11]*naInv(B[11]); D[15] = A[15]*naInv(B[15]);
}
NA_IDEF void naDivCompM22fM22fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 4)
      naError("naDivCompM22fM22fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naDivCompM22fM22fE", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[2] = A[2]*naInvf(B[2]);
  D[1] = A[1]*naInvf(B[1]); D[3] = A[3]*naInvf(B[3]);
}
NA_IDEF void naDivCompM33fM33fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 9)
      naError("naDivCompM33fM33fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naDivCompM33fM33fE", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[3] = A[3]*naInvf(B[3]); D[6] = A[6]*naInvf(B[6]);
  D[1] = A[1]*naInvf(B[1]); D[4] = A[4]*naInvf(B[4]); D[7] = A[7]*naInvf(B[7]);
  D[2] = A[2]*naInvf(B[2]); D[5] = A[5]*naInvf(B[5]); D[8] = A[8]*naInvf(B[8]);
}
NA_IDEF void naDivCompM44fM44fE( float* D, const float* A, const float* B){
  #ifndef NDEBUG
    if((A != D) && naAbsi((NASizeInt)(A-D)) < 16)
      naError("naDivCompM44fM44fE", "A overlaps misaligned with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naDivCompM44fM44fE", "B overlaps with D.");
  #endif
  D[0] = A[0]*naInvf(B[0]); D[4] = A[4]*naInvf(B[4]); D[ 8] = A[ 8]*naInvf(B[ 8]); D[12] = A[12]*naInvf(B[12]);
  D[1] = A[1]*naInvf(B[1]); D[5] = A[5]*naInvf(B[5]); D[ 9] = A[ 9]*naInvf(B[ 9]); D[13] = A[13]*naInvf(B[13]);
  D[2] = A[2]*naInvf(B[2]); D[6] = A[6]*naInvf(B[6]); D[10] = A[10]*naInvf(B[10]); D[14] = A[14]*naInvf(B[14]);
  D[3] = A[3]*naInvf(B[3]); D[7] = A[7]*naInvf(B[7]); D[11] = A[11]*naInvf(B[11]); D[15] = A[15]*naInvf(B[15]);
}
NA_IDEF void naDivCompM22dM22dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naDivCompM22dM22dS", "B overlaps with D.");
  #endif
  D[0] *= naInv(B[0]); D[2] *= naInv(B[2]);
  D[1] *= naInv(B[1]); D[3] *= naInv(B[3]);
}
NA_IDEF void naDivCompM33dM33dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naDivCompM33dM33dS", "B overlaps with D.");
  #endif
  D[0] *= naInv(B[0]); D[3] *= naInv(B[3]); D[6] *= naInv(B[6]);
  D[1] *= naInv(B[1]); D[4] *= naInv(B[4]); D[7] *= naInv(B[7]);
  D[2] *= naInv(B[2]); D[5] *= naInv(B[5]); D[8] *= naInv(B[8]);
}
NA_IDEF void naDivCompM44dM44dS( double* NA_RESTRICT D, const double* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naDivCompM44dM44dS", "B overlaps with D.");
  #endif
  D[0] *= naInv(B[0]); D[4] *= naInv(B[4]); D[ 8] *= naInv(B[ 8]); D[12] *= naInv(B[12]);
  D[1] *= naInv(B[1]); D[5] *= naInv(B[5]); D[ 9] *= naInv(B[ 9]); D[13] *= naInv(B[13]);
  D[2] *= naInv(B[2]); D[6] *= naInv(B[6]); D[10] *= naInv(B[10]); D[14] *= naInv(B[14]);
  D[3] *= naInv(B[3]); D[7] *= naInv(B[7]); D[11] *= naInv(B[11]); D[15] *= naInv(B[15]);
}
NA_IDEF void naDivCompM22fM22fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naDivCompM22fM22fS", "B overlaps with D.");
  #endif
  D[0] *= naInvf(B[0]); D[2] *= naInvf(B[2]);
  D[1] *= naInvf(B[1]); D[3] *= naInvf(B[3]);
}
NA_IDEF void naDivCompM33fM33fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naDivCompM33fM33fS", "B overlaps with D.");
  #endif
  D[0] *= naInvf(B[0]); D[3] *= naInvf(B[3]); D[6] *= naInvf(B[6]);
  D[1] *= naInvf(B[1]); D[4] *= naInvf(B[4]); D[7] *= naInvf(B[7]);
  D[2] *= naInvf(B[2]); D[5] *= naInvf(B[5]); D[8] *= naInvf(B[8]);
}
NA_IDEF void naDivCompM44fM44fS( float* NA_RESTRICT D, const float* NA_RESTRICT B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naDivCompM44fM44fS", "B overlaps with D.");
  #endif
  D[0] *= naInvf(B[0]); D[4] *= naInvf(B[4]); D[ 8] *= naInvf(B[ 8]); D[12] *= naInvf(B[12]);
  D[1] *= naInvf(B[1]); D[5] *= naInvf(B[5]); D[ 9] *= naInvf(B[ 9]); D[13] *= naInvf(B[13]);
  D[2] *= naInvf(B[2]); D[6] *= naInvf(B[6]); D[10] *= naInvf(B[10]); D[14] *= naInvf(B[14]);
  D[3] *= naInvf(B[3]); D[7] *= naInvf(B[7]); D[11] *= naInvf(B[11]); D[15] *= naInvf(B[15]);
}



// //////////////////////////////
// Multiply matrix with vector
// //////////////////////////////

NA_IDEF void naMulM22dV2d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(A-d)))
      naError("naMulM22dV2d", "A overlaps with d.");
    if(naAbsi((NASizeInt)(v-d)) < 2)
      naError("naMulM22dV2d", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[2]*v[1];
  d[1] = A[1]*v[0] + A[3]*v[1];
}
NA_IDEF void naMulM33dV3d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(A-d)))
      naError("naMulM33dV3d", "A overlaps with d.");
    if(naAbsi((NASizeInt)(v-d)) < 3)
      naError("naMulM33dV3d", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[3]*v[1] + A[6]*v[2];
  d[1] = A[1]*v[0] + A[4]*v[1] + A[7]*v[2];
  d[2] = A[2]*v[0] + A[5]*v[1] + A[8]*v[2];
}
NA_IDEF void naMulM44dV4d(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(A-d)))
      naError("naMulM44dV4d", "A overlaps with d.");
    if(naAbsi((NASizeInt)(v-d)) < 4)
      naError("naMulM44dV4d", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[4]*v[1] + A[ 8]*v[2] + A[12]*v[3];
  d[1] = A[1]*v[0] + A[5]*v[1] + A[ 9]*v[2] + A[13]*v[3];
  d[2] = A[2]*v[0] + A[6]*v[1] + A[10]*v[2] + A[14]*v[3];
  d[3] = A[3]*v[0] + A[7]*v[1] + A[11]*v[2] + A[15]*v[3];
}
NA_IDEF void naMulM22fV2f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-3, 1, (NASizeInt)(A-d)))
      naError("naMulM22fV2f", "A overlaps with d.");
    if(naAbsi((NASizeInt)(v-d)) < 2)
      naError("naMulM22fV2f", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[2]*v[1];
  d[1] = A[1]*v[0] + A[3]*v[1];
}
NA_IDEF void naMulM33fV3f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-8, 2, (NASizeInt)(A-d)))
      naError("naMulM33fV3f", "A overlaps with d.");
    if(naAbsi((NASizeInt)(v-d)) < 3)
      naError("naMulM33fV3f", "v overlaps with d.");
  #endif
  d[0] = A[0]*v[0] + A[3]*v[1] + A[6]*v[2];
  d[1] = A[1]*v[0] + A[4]*v[1] + A[7]*v[2];
  d[2] = A[2]*v[0] + A[5]*v[1] + A[8]*v[2];
}
NA_IDEF void naMulM44fV4f(float* NA_RESTRICT d, const float* NA_RESTRICT A, const float* NA_RESTRICT v){
  #ifndef NDEBUG
    if(naInsidei(-15, 3, (NASizeInt)(A-d)))
      naError("naMulM44fV4f", "A overlaps with d.");
    if(naAbsi((NASizeInt)(v-d)) < 4)
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

NA_IDEF void naMulM22dM22d(double* NA_RESTRICT D, const double* A, const double* B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naMulM22dM22d", "A overlaps with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulM22dM22d", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[2]*B[1];
  D[1] = A[1]*B[0] + A[3]*B[1];
  D[2] = A[0]*B[2] + A[2]*B[3];
  D[3] = A[1]*B[2] + A[3]*B[3];
}
NA_IDEF void naMulM33dM33d(double* NA_RESTRICT D, const double* A, const double* B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naMulM33dM33d", "A overlaps with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulM33dM33d", "B overlaps with D.");
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
NA_IDEF void naMulM44dM44d(double* NA_RESTRICT D, const double* A, const double* B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naMulM44dM44d", "A overlaps with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulM44dM44d", "B overlaps with D.");
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
NA_IDEF void naMulM22fM22f(float* NA_RESTRICT D, const float* A, const float* B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-A)) < 4)
      naError("naMulM22fM22f", "A overlaps with D.");
    if(naAbsi((NASizeInt)(D-B)) < 4)
      naError("naMulM22fM22f", "B overlaps with D.");
  #endif
  D[0] = A[0]*B[0] + A[2]*B[1];
  D[1] = A[1]*B[0] + A[3]*B[1];
  D[2] = A[0]*B[2] + A[2]*B[3];
  D[3] = A[1]*B[2] + A[3]*B[3];
}
NA_IDEF void naMulM33fM33f(float* NA_RESTRICT D, const float* A, const float* B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-A)) < 9)
      naError("naMulM33fM33f", "A overlaps with D.");
    if(naAbsi((NASizeInt)(D-B)) < 9)
      naError("naMulM33fM33f", "B overlaps with D.");
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
NA_IDEF void naMulM44fM44f(float* NA_RESTRICT D, const float* A, const float* B){
  #ifndef NDEBUG
    if(naAbsi((NASizeInt)(D-A)) < 16)
      naError("naMulM44fM44f", "A overlaps with D.");
    if(naAbsi((NASizeInt)(D-B)) < 16)
      naError("naMulM44fM44f", "B overlaps with D.");
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




// ////////////////////////////////////////////
// Equal
// ////////////////////////////////////////////

NA_IDEF NABool naEqualM11d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM11d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 1)
      naError("naEqualM11d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM22d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 4)
      naError("naEqualM22d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM33d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 9)
      naError("naEqualM33d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44d(const double* NA_RESTRICT a, const double* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM44d", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 16)
      naError("naEqualM44d", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
}
NA_IDEF NABool naEqualM11f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM11f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 1)
      naError("naEqualM11f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM22f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 4)
      naError("naEqualM22f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM33f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 9)
      naError("naEqualM33f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44f(const float* NA_RESTRICT a, const float* NA_RESTRICT b){
  #ifndef NDEBUG
    if(a == b)
      naError("naEqualM44f", "The two pointers are equal. Use E variant or restrict pointers.");
    else if(naAbsi((NASizeInt)(a-b)) < 16)
      naError("naEqualM44f", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
}
NA_IDEF NABool naEqualM11dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 1))
      naError("naEqualM11dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 4))
      naError("naEqualM22dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 9))
      naError("naEqualM33dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44dE(const double* a, const double* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 16))
      naError("naEqualM44dE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
}
NA_IDEF NABool naEqualM11fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 1))
      naError("naEqualM11fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]));
}
NA_IDEF NABool naEqualM22fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 4))
      naError("naEqualM22fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]));
}
NA_IDEF NABool naEqualM33fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 9))
      naError("naEqualM33fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]));
}
NA_IDEF NABool naEqualM44fE(const float* a, const float* b){
  #ifndef NDEBUG
    if((a != b) && (naAbsi((NASizeInt)(a-b)) < 16))
      naError("naEqualM44fE", "b overlaps misaligned with a");
  #endif
  return ((a[0] == b[0]) && (a[1] == b[1]) && (a[2] == b[2]) && (a[3] == b[3]) && (a[4] == b[4]) && (a[5] == b[5]) && (a[6] == b[6]) && (a[7] == b[7]) && (a[8] == b[8]) && (a[9] == b[9]) && (a[10] == b[10]) && (a[11] == b[11]) && (a[12] == b[12]) && (a[13] == b[13]) && (a[14] == b[14]) && (a[15] == b[15]));
}




// //////////////////////////////////////
// Determinants
// //////////////////////////////////////


NA_IDEF double naDetM22d(double* A){
  double determinant = A[0]*A[3] - A[2]*A[1];
  return determinant;
}
NA_IDEF double naDetM33d(double* A){
  double d0 = A[4] * A[8] - A[5] * A[7];
  double d1 = A[2] * A[7] - A[1] * A[8];
  double d2 = A[1] * A[5] - A[2] * A[4];
  double determinant = A[0]*d0 + A[3]*d1 + A[6]*d2;
  return determinant;
}
NA_IDEF double naDetM44d(double* A){
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
  return determinant;
}



// //////////////////////////////////////
// Inverting
// //////////////////////////////////////

NA_IDEF void naInvM22d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
  double determinant = A[0]*A[3] - A[2]*A[1];
  double divisor = naInv(determinant);
  D[0] =    A[3] * divisor;
  D[1] =  - A[1] * divisor;
  D[2] =  - A[2] * divisor;
  D[3] =    A[0] * divisor;
}
NA_IDEF void naInvM33d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
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
NA_IDEF void naInvM44d(double* NA_RESTRICT D, const double* NA_RESTRICT A){
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
NA_IDEF void naInvM22f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
  float determinant = A[0]*A[3] - A[2]*A[1];
  float divisor = naInvf(determinant);
  D[0] =    A[3] * divisor;
  D[1] =  - A[1] * divisor;
  D[2] =  - A[2] * divisor;
  D[3] =    A[0] * divisor;
}
NA_IDEF void naInvM33f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
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
NA_IDEF void naInvM44f(float* NA_RESTRICT D, const float* NA_RESTRICT A){
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



// ////////////////////////////////
// HOMOGENEOUS COORDINATES
// ////////////////////////////////

NA_IDEF void naScaleM33dS(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2){
  D[0] = A[0] * s1;
  D[1] = A[1] * s1;
  D[2] = A[2] * s1;
  D[3] = A[3] * s2;
  D[4] = A[4] * s2;
  D[5] = A[5] * s2;
  D[6] = A[6];
  D[7] = A[7];
  D[8] = A[8];
}



NA_IDEF void naTranslateM33dS(double* NA_RESTRICT D, const double* NA_RESTRICT A, double s1, double s2){
  D[0] = A[0];
  D[1] = A[1];
  D[2] = A[2];
  D[3] = A[3];
  D[4] = A[4];
  D[5] = A[5];
  D[6] = A[0] * s1 + A[3] * s2 + A[6];
  D[7] = A[1] * s1 + A[4] * s2 + A[7];
  D[8] = A[2] * s1 + A[5] * s2 + A[8];
//  D[0] = A[0] + A[2] * s1;
//  D[1] = A[1] + A[2] * s2;
//  D[2] = A[2] ;
//  D[3] = A[3] + A[5] * s1;
//  D[4] = A[4] + A[5] * s2;
//  D[5] = A[5] ;
//  D[6] = A[6] + A[8] * s1;
//  D[7] = A[7] + A[8] * s2;
//  D[8] = A[8] ;
}



NA_IDEF void naFillV2WithM33Origin(double* NA_RESTRICT d, const double* NA_RESTRICT A){
  d[0] = A[6] / A[8];
  d[1] = A[7] / A[8];
}

NA_IDEF void naFillV2WithM33Coord2(double* NA_RESTRICT d, const double* NA_RESTRICT A, const double* NA_RESTRICT v){
  NAVec3d result;
  NAVec3d coord;
  coord[0] = v[0];
  coord[1] = v[1];
  coord[2] = 1.;
  naMulM33dV3d(result, A, coord);
  d[0] = result[0] / result[2];
  d[1] = result[1] / result[2];
}



// ////////////////////////////////
// ROTATION MATRIX
// ////////////////////////////////

NA_IDEF void naFillM33dWithRotationAlongAxis(double* NA_RESTRICT d, const double* NA_RESTRICT axis, double angle){
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


NA_IDEF void naFillM33fWithRotationAlongAxis(float* NA_RESTRICT d, const float* NA_RESTRICT axis, float angle){
  float sinang    = naSinf(.5f * angle);
  float sinang2   = sinang * sinang;
  float sinangT2  = 2.f * sinang;
  float cosang    = naCosf(.5f * angle);
  float ax0       = axis[0] * cosang;
  float ax00      = axis[0] * axis[0];
  float ax01      = axis[0] * axis[1] * sinang;
  float ax02      = axis[0] * axis[2] * sinang;
  float ax1       = axis[1] * cosang;
  float ax11      = axis[1] * axis[1];
  float ax12      = axis[1] * axis[2] * sinang;
  float ax2       = axis[2] * cosang;
  float ax22      = axis[2] * axis[2];
  
  naFillM33fColumnFirst(d,
          1.f + sinang2 * (ax00 - ax11 - ax22 - 1.f),
          sinangT2 * (ax01 + ax2),
          sinangT2 * (ax02 - ax1),
          sinangT2 * (ax01 - ax2),
          1.f + sinang2 * (-ax00 + ax11 - ax22 - 1.f),
          sinangT2 * (ax12 + ax0),
          sinangT2 * (ax02 + ax1),
          sinangT2 * (ax12 - ax0),
          1.f + sinang2 * (-ax00 - ax11 + ax22 - 1.f));
}



NA_IDEF void naFillM33dWithRotationAngles(double* d, double roll, double pitch, double yaw){
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

NA_IDEF void naFillM33fWithRotationAngles(float* d, float roll, float pitch, float yaw){
  float q1 = - naCosf(.5f * (roll - yaw)) * naSinf(.5f * pitch);
  float q2 = naSinf(.5f * (roll - yaw)) * naSinf(.5f * pitch);
  float q3 = - naSinf(.5f * (roll + yaw)) * naCosf(.5f * pitch);
  float q4 = naCosf(.5f * (roll + yaw)) * naCosf(.5f * pitch);

  naFillM33fColumnFirst(d,
          1.f - 2.f * ( q2*q2 - q3*q3),
          2.f * (q1*q2 + q3*q4),
          2.f * (q1*q3 - q2*q4),
          2.f * (q1*q2 - q3*q4),
          1.f - 2.f * ( q1*q1 - q3*q3),
          2.f * (q1*q4 + q2*q3),
          2.f * (q1*q3 + q2*q4),
          2.f * (q2*q3 - q1*q4),
          1.f - 2.f * ( q1*q1 - q2*q2));
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
