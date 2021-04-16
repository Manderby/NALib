
// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebraII.h"



// ///////////////////////////////
// Filler
// ///////////////////////////////

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



// ///////////////////////////////
// Zero Filler
// ///////////////////////////////

NA_IDEF void naZeroV1d(double*  d){
  d[0] = 0.;
}
NA_IDEF void naZeroV2d(double*  d){
  d[0] = 0.; d[1] = 0.;
}
NA_IDEF void naZeroV3d(double*  d){
  d[0] = 0.; d[1] = 0.; d[2] = 0.;
}
NA_IDEF void naZeroV4d(double*  d){
  d[0] = 0.; d[1] = 0.; d[2] = 0.; d[3] = 0.;
}
NA_IDEF void naZeroV1f(float* d){
  d[0] = 0.f;
}
NA_IDEF void naZeroV2f(float* d){
  d[0] = 0.f; d[1] = 0.f;
}
NA_IDEF void naZeroV3f(float* d){
  d[0] = 0.f; d[1] = 0.f; d[2] = 0.f;
}
NA_IDEF void naZeroV4f(float* d){
  d[0] = 0.f; d[1] = 0.f; d[2] = 0.f; d[3] = 0.f;
}
NA_IDEF void naZeroV1i(NAInt* d){
  d[0] = 0;
}
NA_IDEF void naZeroV2i(NAInt* d){
  d[0] = 0; d[1] = 0;
}
NA_IDEF void naZeroV3i(NAInt* d){
  d[0] = 0; d[1] = 0; d[2] = 0;
}
NA_IDEF void naZeroV4i(NAInt* d){
  d[0] = 0; d[1] = 0; d[2] = 0; d[3] = 0;
}



// ///////////////////////////////
// Random Filler
// ///////////////////////////////

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



// ///////////////////////////////
// Random Spherical Filler
// ///////////////////////////////

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



// ///////////////////////////////
// Copier
// ///////////////////////////////

NA_IDEF void naCopyV1d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 1, 1))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0];
}
NA_IDEF void naCopyV2d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 2, 2))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 3, 3))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4d(double* NA_RESTRICT d, const double* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 4, 4))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 1, 1))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0];
}
NA_IDEF void naCopyV2f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 2, 2))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 3, 3))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4f(float* NA_RESTRICT d, const float* NA_RESTRICT a){
  #if NA_DEBUG
    if(d == a)
      naError("Pointers are equal. Use E variant.");
    else if(naElementOverlap(d, a, 4, 4))
      naError("Pointers overlap. Use restrict pointers or the E variant");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1dE (double* d, const double* a){
  d[0] = a[0];
}
NA_IDEF void naCopyV2dE (double* d, const double* a){
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 2, 2)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3dE (double* d, const double* a){
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 3, 3)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4dE (double* d, const double* a){
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 4, 4)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}
NA_IDEF void naCopyV1fE(float*  d, const float*  a){
  d[0] = a[0];
}
NA_IDEF void naCopyV2fE(float*  d, const float*  a){
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 2, 2)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1];
}
NA_IDEF void naCopyV3fE(float*  d, const float*  a){
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 3, 3)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2];
}
NA_IDEF void naCopyV4fE(float*  d, const float*  a){
  #if NA_DEBUG
    if((d != a) && (naElementOverlap(d, a, 4, 4)))
      naError("Pointers are overlapping misaligned. Result undefined.");
  #endif
  d[0] = a[0]; d[1] = a[1]; d[2] = a[2]; d[3] = a[3];
}



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
