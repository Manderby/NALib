
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.

#ifndef NA_FLOATING_POINT_INCLUDED
#define NA_FLOATING_POINT_INCLUDED


#include "../NABase.h"

// Creates floats and doubles out of signed significands and exponents
NA_IAPI float  naMakeFloat (int32 signedSignificand, int32 signedExponent);
NA_IAPI float  naMakeFloatWithExponent (int32 signedExponent);
NA_IAPI float  naMakeFloatSubnormal (int32 signedSignificand);

NA_IAPI double naMakeDouble(NAInt64 signedSignificand, int32 signedExponent);
NA_IAPI double naMakeDoubleWithExponent(int32 signedExponent);
NA_IAPI double naMakeDoubleSubnormal(NAInt64 signedSignificand);


NA_IAPI int32   naGetDoubleExponent(double d);
NA_IAPI NAInt64 naGetDoubleInteger(double d);
NA_IAPI NAInt64 naGetDoubleFraction(double d);



// Inline implementations are in a separate file:
#include "NAFloatingPointII.h"


#endif // NA_FLOATING_POINT_INCLUDED

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
