
// This file is part of NALib, a collection of C and C++ source code
// intended for didactical purposes. Full license notice at the bottom.

#include "NAMathOperators.h"


typedef struct NAComplex NAComplex;
struct NAComplex{
  double re;
  double im;
};


// Returns a new struct filled with complex values.
static NAComplex  naMakeComplex(double re, double im);

// Returns a new struct filled with complex values computed from polar values.
static NAComplex  naMakeComplexFromPolar(double r, double phi);

// Performs base operations with a real number
static NAComplex  naAddComplexReal(NAComplex a, double b);
static NAComplex  naSubComplexReal(NAComplex a, double b);
static NAComplex  naMulComplexReal(NAComplex a, double b);
static NAComplex  naDivComplexReal(NAComplex a, double b);

// Performs base operations with another complex number
static NAComplex  naAddComplexComplex(NAComplex a, NAComplex b);
static NAComplex  naSubComplexComplex(NAComplex a, NAComplex b);
static NAComplex  naMulComplexComplex(NAComplex a, NAComplex b);
static NAComplex  naDivComplexComplex(NAComplex a, NAComplex b);

// negates, inverts and conjugates a complex number.
static NAComplex  naNegComplex   (NAComplex a);
static NAComplex  naInvComplex   (NAComplex a);
static NAComplex  naConjComplex  (NAComplex a);

// Returns the norm, the absolute value and the argument of a complex value.
static double     naNormComplex     (NAComplex a);
static double     naAbsComplex      (NAComplex a);
static double     naArgComplex      (NAComplex a);

// Performs the mathematical operators on a complex value
static NAComplex  naExpComplex   (NAComplex a);
static NAComplex  naSinComplex   (NAComplex a);
static NAComplex  naCosComplex   (NAComplex a);






















// ///////////////////////////////////////////////////////////////////////
// Inline Implementations: See readme file for more expanation.
// ///////////////////////////////////////////////////////////////////////

NA_INLINE_API static NAComplex naMakeComplex(double re, double im){
  NAComplex newcomplex = {re, im};
  return newcomplex;
}


NA_INLINE_API static NAComplex naMakeComplexFromPolar(double r, double phi){
  NAComplex newcomplex;
  #ifndef NDEBUG
    if(r<0.){
      naError("naMakeComplexFromPolar", "Radius is negative");
    }
  #endif
  newcomplex.re = r * naCos(phi);
  newcomplex.im = r * naSin(phi);
  return newcomplex;
}


NA_INLINE_API static NAComplex naAddComplexReal(NAComplex a, double b){
  NAComplex newcomplex = {
    a.re + b,
    a.im
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naSubComplexReal(NAComplex a, double b){
  NAComplex newcomplex = {
    a.re - b,
    a.im
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naMulComplexReal(NAComplex a, double b){
  NAComplex newcomplex = {
    a.re * b,
    a.im * b
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naDivComplexReal(NAComplex a, double b){
  double divisor = naInv(b);
  NAComplex newcomplex = {
    a.re * divisor,
    a.im * divisor
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naAddComplexComplex(NAComplex a, NAComplex b){
  NAComplex newcomplex = {
    a.re + b.re,
    a.im + b.im
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naSubComplexComplex(NAComplex a, NAComplex b){
  NAComplex newcomplex = {
    a.re - b.re,
    a.im - b.im
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naMulComplexComplex(NAComplex a, NAComplex b){
  NAComplex newcomplex = {
    a.re * b.re - a.im * b.im,
    a.re * b.im + a.im * b.re
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naDivComplexComplex(NAComplex a, NAComplex b){
  double divisor = naInv(naNormComplex(b));
  NAComplex newcomplex = {
    divisor * (a.re * b.re + a.im * b.im),
    divisor * (a.im * b.re - a.re * b.im)
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naNegComplex(NAComplex a){
  NAComplex newcomplex = {
    -a.re,
    -a.im
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naInvComplex(NAComplex a){
  double divisor = naInv(naNormComplex(a));
  NAComplex newcomplex = {
    divisor * a.re,
    divisor * -a.im
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naConjComplex(NAComplex a){
  NAComplex newcomplex = {
    a.re,
    -a.im
  };
  return newcomplex;
}


NA_INLINE_API static double naNormComplex(NAComplex a){
  return a.re * a.re + a.im * a.im;
}


NA_INLINE_API static double naAbsComplex(NAComplex a){
  return naSqrt(naNormComplex(a));
}


NA_INLINE_API static double naArgComplex(NAComplex a){
  return naAtan2(a.im, a.re);
}


NA_INLINE_API static NAComplex naExpComplex(NAComplex a){
  double exponent = naExp(a.re);
  NAComplex newcomplex = {
    exponent * naCos(a.im),
    exponent * naSin(a.im)
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naSinComplex(NAComplex a){
  double exponent1 = naExp(-a.im);
  double exponent2 = naExp(a.im);
  NAComplex newcomplex = {
    (naSin(a.re) * (exponent1 + exponent2)) * .5,
    (naCos(a.re) * (exponent2 - exponent1)) * .5
  };
  return newcomplex;
}


NA_INLINE_API static NAComplex naCosComplex(NAComplex a){
  double exponent1 = naExp(-a.im);
  double exponent2 = naExp(a.im);
  NAComplex newcomplex = {
    (naCos(a.re) * (exponent1 + exponent2)) * .5,
    (naSin(a.re) * (exponent1 - exponent2)) * .5
  };
  return newcomplex;
}



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
