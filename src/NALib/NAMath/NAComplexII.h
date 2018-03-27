
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.


// This file contains inline implementations of the file NAComplex.h
// Do not include this file directly! It will automatically be included when
// including "NAComplex.h"




NA_IDEF NAComplex naMakeComplex(double re, double im){
  NAComplex newcomplex = {re, im};
  return newcomplex;
}


NA_IDEF NAComplex naMakeComplexFromPolar(double r, double phi){
  NAComplex newcomplex;
  #ifndef NDEBUG
    if(r<0.)
      naError("naMakeComplexFromPolar", "Radius is negative");
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
  return naAtan2d(a.im, a.re);
}


NA_IDEF NAComplex naExpComplex(NAComplex a){
  double exponent = naExpd(a.re);
  NAComplex newcomplex = {
    exponent * naCos(a.im),
    exponent * naSin(a.im)
  };
  return newcomplex;
}


NA_IDEF NAComplex naSinComplex(NAComplex a){
  double exponent1 = naExpd(-a.im);
  double exponent2 = naExpd(a.im);
  NAComplex newcomplex = {
    (naSin(a.re) * (exponent1 + exponent2)) * .5,
    (naCos(a.re) * (exponent2 - exponent1)) * .5
  };
  return newcomplex;
}


NA_IDEF NAComplex naCosComplex(NAComplex a){
  double exponent1 = naExpd(-a.im);
  double exponent2 = naExpd(a.im);
  NAComplex newcomplex = {
    (naCos(a.re) * (exponent1 + exponent2)) * .5,
    (naSin(a.re) * (exponent1 - exponent2)) * .5
  };
  return newcomplex;
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
