
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
NA_IAPI NAComplex  naMakeComplexWithPolar(double r, double phi);

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
#include "Core/NAComplexII.h"



#ifdef __cplusplus
  } // extern "C"
#endif
#endif // NA_COMPLEX_INCLUDED



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
