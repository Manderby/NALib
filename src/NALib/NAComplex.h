
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_COMPLEX_INCLUDED
#define NA_COMPLEX_INCLUDED
#ifdef __cplusplus
  extern "C"{
#endif


// Type is presented as non-opaque as the programmer wants to access the two
// parts of a complex number.
typedef struct NAComplex NAComplex;
struct NAComplex{
  double re;
  double im;
};


// Returns a new struct filled with complex values.
NA_IAPI NAComplex  naMakeComplex(double re, double im);

// Returns a new struct filled with complex values computed from polar values.
NA_IAPI NAComplex  naMakeComplexFromPolar(double r, double phi);

// Performs base operations with a real number
NA_IAPI NAComplex  naAddComplexReal(NAComplex a, double b);
NA_IAPI NAComplex  naSubComplexReal(NAComplex a, double b);
NA_IAPI NAComplex  naMulComplexReal(NAComplex a, double b);
NA_IAPI NAComplex  naDivComplexReal(NAComplex a, double b);

// Performs base operations with another complex number
NA_IAPI NAComplex  naAddComplexComplex(NAComplex a, NAComplex b);
NA_IAPI NAComplex  naSubComplexComplex(NAComplex a, NAComplex b);
NA_IAPI NAComplex  naMulComplexComplex(NAComplex a, NAComplex b);
NA_IAPI NAComplex  naDivComplexComplex(NAComplex a, NAComplex b);

// negates, inverts and conjugates a complex number.
NA_IAPI NAComplex  naNegComplex   (NAComplex a);
NA_IAPI NAComplex  naInvComplex   (NAComplex a);
NA_IAPI NAComplex  naConjComplex  (NAComplex a);

// Returns the norm, the absolute value and the argument of a complex value.
NA_IAPI double     naNormComplex  (NAComplex a);
NA_IAPI double     naAbsComplex   (NAComplex a);
NA_IAPI double     naArgComplex   (NAComplex a);

// Performs the mathematical operators on a complex value
NA_IAPI NAComplex  naExpComplex   (NAComplex a);
NA_IAPI NAComplex  naSinComplex   (NAComplex a);
NA_IAPI NAComplex  naCosComplex   (NAComplex a);




// Inline implementations are in a separate file:
#include "NAMath/NAComplexII.h"





#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_COMPLEX_INCLUDED


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
