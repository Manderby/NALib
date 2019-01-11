
// This file is part of NALib, a collection of C source code.
// Full license notice at the bottom.



// This file contains inline implementations of the file NAVectorAlgebra.h
// Do not include this file directly! It will automatically be included when
// including "NAVectorAlgebra.h"



// /////////////////////////////////////
// Warning: This is one huge collection!
//
// Vector algebra functions are not especially difficult to understand or
// implement but take a lot of code writing. Therefore it is split into 
// several files. You are free to explore all the inline code but there
// really isn't much to see.
// /////////////////////////////////////



#include "NARandom.h"
#include "NAMathOperators.h"

// ////////////////////////////////////////////
// Vectors
// ////////////////////////////////////////////

#include "NAVectorAlgebra/NAVectorFiller.h"
#include "NAVectorAlgebra/NAVectorBaseOperations.h"
#include "NAVectorAlgebra/NAVectorComponentwise.h"
#include "NAVectorAlgebra/NAVectorComparison.h"
#include "NAVectorAlgebra/NAVectorProducts.h"
#include "NAVectorAlgebra/NAVectorGeometry.h"

// ////////////////////////////////////////////
// Matrix, stored in COLUMN-FIRST order
// ////////////////////////////////////////////

#include "NAVectorAlgebra/NAMatrixFiller.h"
#include "NAVectorAlgebra/NAMatrixBaseOperations.h"
#include "NAVectorAlgebra/NAMatrixComponentwise.h"
#include "NAVectorAlgebra/NAMatrixMultiplication.h"
#include "NAVectorAlgebra/NAMatrixComparison.h"
#include "NAVectorAlgebra/NAMatrixAlgebraOperations.h"
#include "NAVectorAlgebra/NAMatrixRotations.h"




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
