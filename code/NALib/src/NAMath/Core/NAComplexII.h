
// This file contains inline implementations of the file NAComplex.h
// Do not include this file directly! It will automatically be included when
// including "NAComplex.h"



#include "NAMath/NAMathOperators.h"


NA_IDEF NAComplex naMakeComplex(double re, double im){
  NAComplex newcomplex = {re, im};
  return newcomplex;
}


NA_IDEF NAComplex naMakeComplexFromPolar(double r, double phi){
  NAComplex newcomplex;
  #if NA_DEBUG
    if(r < 0.)
      naError("Radius is negative");
  #endif
  newcomplex.re = r * naCos(phi);
  newcomplex.im = r * naSin(phi);
  return newcomplex;
}


NA_IDEF NAComplex naAddComplexReal(NAComplex a, double b){
  NAComplex newcomplex = {
    a.re + b,
    a.im
  };
  return newcomplex;
}


NA_IDEF NAComplex naSubComplexReal(NAComplex a, double b){
  NAComplex newcomplex = {
    a.re - b,
    a.im
  };
  return newcomplex;
}


NA_IDEF NAComplex naMulComplexReal(NAComplex a, double b){
  NAComplex newcomplex = {
    a.re * b,
    a.im * b
  };
  return newcomplex;
}


NA_IDEF NAComplex naDivComplexReal(NAComplex a, double b){
  double divisor = naInv(b);
  NAComplex newcomplex = {
    a.re * divisor,
    a.im * divisor
  };
  return newcomplex;
}


NA_IDEF NAComplex naAddComplexComplex(NAComplex a, NAComplex b){
  NAComplex newcomplex = {
    a.re + b.re,
    a.im + b.im
  };
  return newcomplex;
}


NA_IDEF NAComplex naSubComplexComplex(NAComplex a, NAComplex b){
  NAComplex newcomplex = {
    a.re - b.re,
    a.im - b.im
  };
  return newcomplex;
}


NA_IDEF NAComplex naMulComplexComplex(NAComplex a, NAComplex b){
  NAComplex newcomplex = {
    a.re * b.re - a.im * b.im,
    a.re * b.im + a.im * b.re
  };
  return newcomplex;
}


NA_IDEF NAComplex naDivComplexComplex(NAComplex a, NAComplex b){
  double divisor = naInv(naNormComplex(b));
  NAComplex newcomplex = {
    divisor * (a.re * b.re + a.im * b.im),
    divisor * (a.im * b.re - a.re * b.im)
  };
  return newcomplex;
}


NA_IDEF NAComplex naNegComplex(NAComplex a){
  NAComplex newcomplex = {
    -a.re,
    -a.im
  };
  return newcomplex;
}


NA_IDEF NAComplex naInvComplex(NAComplex a){
  double divisor = naInv(naNormComplex(a));
  NAComplex newcomplex = {
    divisor * a.re,
    divisor * -a.im
  };
  return newcomplex;
}


NA_IDEF NAComplex naConjComplex(NAComplex a){
  NAComplex newcomplex = {
    a.re,
    -a.im
  };
  return newcomplex;
}


NA_IDEF double naNormComplex(NAComplex a){
  return a.re * a.re + a.im * a.im;
}


NA_IDEF double naAbsComplex(NAComplex a){
  return naSqrt(naNormComplex(a));
}


NA_IDEF double naArgComplex(NAComplex a){
  return naAtan2(a.im, a.re);
}


NA_IDEF NAComplex naExpComplex(NAComplex a){
  double exponent = naExp(a.re);
  NAComplex newcomplex = {
    exponent * naCos(a.im),
    exponent * naSin(a.im)
  };
  return newcomplex;
}


NA_IDEF NAComplex naSinComplex(NAComplex a){
  double exponent1 = naExp(-a.im);
  double exponent2 = naExp(a.im);
  NAComplex newcomplex = {
    (naSin(a.re) * (exponent1 + exponent2)) * .5,
    (naCos(a.re) * (exponent2 - exponent1)) * .5
  };
  return newcomplex;
}


NA_IDEF NAComplex naCosComplex(NAComplex a){
  double exponent1 = naExp(-a.im);
  double exponent2 = naExp(a.im);
  NAComplex newcomplex = {
    (naCos(a.re) * (exponent1 + exponent2)) * .5,
    (naSin(a.re) * (exponent1 - exponent2)) * .5
  };
  return newcomplex;
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
